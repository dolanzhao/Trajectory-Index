//
//  trj_node.cpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#include "trj_node.hpp"
#include "util.hpp"
#include "quad_tree.hpp"
#include "dbscan_whynot.hpp"

TrjNode::TrjNode()
: _x(0.0)
, _y(0.0)
, _id(0)
, _isInitSuccess(false)
, _belongQuadTreeNode(NULL)
, _belongDbscanClass(NULL)
{
    
}

TrjNode::~TrjNode()
{
    if(_isInitSuccess)
    {
//        printf("销毁轨迹点 ID:%d X:%lf Y:%lf \n", _id, _x, _y); //for debug
    }
}

TrjNode* TrjNode::create(std::string nodeTextData)
{
    TrjNode* trjNode = new TrjNode();
    if (trjNode->init(nodeTextData)) {
        return trjNode;
    }
    delete trjNode;
    return NULL;
}

int TrjNode::getTrjNodeId()
{
    return _id;
}

double TrjNode::getTrjNodeX()
{
    return _x;
}

double TrjNode::getTrjNodeY()
{
    return _y;
}

bool TrjNode::init(std::string nodeTextData)
{
    std::vector<std::string> splitResult = Util::split(nodeTextData, NodeInternalDateSep);
    if(splitResult.size() != 3)
    {
        printf("销毁数据有误，数据缺失");//for debug
        _isInitSuccess = false;
        return false;
    }
    _id = Util::stringToInt(splitResult[0]);
    if(_id < NODE_ID_MIN)
    {
        printf("销毁数据有误，id越界");//for debug
        _isInitSuccess = false;
        return false;
    }
    _x = Util::stringToInt(splitResult[1]);
    if(_x <= NODE_X_MIN && _x >= NODE_X_MAX)
    {
        printf("销毁数据有误，数据点x坐标越界");//for debug
        _isInitSuccess = false;
        return false;
    }
    
    _y = Util::stringToInt(splitResult[2]);
    if(_y <= NODE_Y_MIN && _y>= NODE_Y_MAX)
    {
        printf("销毁数据有误，数据点y坐标越界");//for debug
        _isInitSuccess = false;
        return false;
    }
    
    _isInitSuccess = true;
    return true;
}

bool TrjNode::operator==(const TrjNode& operatorNode)
{
    if (_id == operatorNode._id) {
        return true;
    }
    else
    {
        return false;
    }
}






TrjNodeManage* TrjNodeManage::create(std::string testDate)
{
    TrjNodeManage* trjNodeManage = new TrjNodeManage();
    trjNodeManage->init(testDate);
    return trjNodeManage;
}

TrjNodeManage::TrjNodeManage()
: _maxId(-1)
, _minId(-1)
, _quadTree(NULL)
, _time(-1)
{
    
}

TrjNodeManage::~TrjNodeManage()
{
    if(_quadTree)
    {
        delete _quadTree;
    }
    std::map<int, TrjNode*>::iterator it = _nodeMap.begin();
    for (; it != _nodeMap.end(); it++) {
        delete it->second;
    }
    
    _nodeMap.clear();
}

void TrjNodeManage::init(std::string textData)
{
    
    if(_quadTree)
    {
        delete _quadTree;
        _quadTree = NULL;
    }
    std::map<int, TrjNode*>::iterator nodeIt = _nodeMap.begin();
    for (; nodeIt != _nodeMap.end(); nodeIt++) {
        delete nodeIt->second;
    }
    
    double xMax = 0, xMin = 0, yMax = 0, yMin = 0;
    std::vector<std::string> splitResult = Util::split(textData, NodeDateSep);
    _time = Util::stringToInt(splitResult[0]);
    std::vector<std::string>::iterator it = splitResult.begin();
    for(it++; it != splitResult.end(); it++) {
        TrjNode* tempTrjNode = TrjNode::create(*it);
        if (tempTrjNode) {
            xMax = xMax > tempTrjNode->getTrjNodeX() ? xMax : tempTrjNode->getTrjNodeX();
            xMin = xMin < tempTrjNode->getTrjNodeX() ? xMin : tempTrjNode->getTrjNodeX();
            yMax = yMax > tempTrjNode->getTrjNodeY() ? yMax : tempTrjNode->getTrjNodeY();
            yMin = yMin < tempTrjNode->getTrjNodeY() ? yMin : tempTrjNode->getTrjNodeY();
            int tempId = tempTrjNode->getTrjNodeId();
            if (_maxId < tempId) {
                _maxId = tempId;
            }
            if (_minId > tempId) {
                _minId = tempId;
            }
            _nodeMap.insert(std::map<int, TrjNode*>::value_type(tempId, tempTrjNode));
            tempTrjNode->_belongTrjNodeManage = this;
        }
    }
    _quadTree = new QuadTree();
    QuadTreeNode* rootNode = QuadTreeNode::create(xMin, yMin, xMax, yMax, _quadTree, NULL);
    
    
    std::map<int, TrjNode*>::iterator it2 = _nodeMap.begin();
    for(; it2 != _nodeMap.end(); it2++) {
        rootNode->addNode(it2->second);
    }
    
    if (rootNode->_trjNodeNumber <= CELL_NODE_MAX) {
        rootNode->_isLeaf = true;
    }
    rootNode->separate();
    
    
}

void TrjNodeManage::printCellNode()
{
    std::vector<QuadTreeNode*>::iterator it = _quadTree->_quadTreeNodeCache.begin();
    for (; it != _quadTree->_quadTreeNodeCache.end(); it++) {
        printf("has child %d %lf %lf %lf %lf:", (int)((*it)->_childNodeVector.size()), (*it)->_xMin, (*it)->_yMin, (*it)->_xMax, (*it)->_yMax);
        std::vector<TrjNode*>::iterator it2 = (*it)->_trjNodeVector.begin();
        for (; it2 != (*it)->_trjNodeVector.end(); it2 ++) {
            printf("%d ", (*it2)->getTrjNodeId());
        }
        if ((*it)->_isLeaf) {
            printf("##");
        }
        printf("\n");
    }
}

std::vector<TrjNode*> TrjNodeManage::getCoverTrj(int id, double r)
{
    return Util::getCoverTrj(_nodeMap[id], *(_quadTree->_quadTreeNodeCache.begin()), r);
}

std::vector<TrjNode*> TrjNodeManage::getCoverTrj(TrjNode* trjNode, double r)
{
    return Util::getCoverTrj(trjNode, *(_quadTree->_quadTreeNodeCache.begin()), r);
}

std::vector<TrjNode*> TrjNodeManage::getCoverTrjByRR(int id, double rr)
{
    return Util::getCoverTrjByRR(_nodeMap[id], *(_quadTree->_quadTreeNodeCache.begin()), rr);
}

std::vector<TrjNode*> TrjNodeManage::getCoverTrjByRR(TrjNode* trjNode, double rr)
{
    return Util::getCoverTrjByRR(trjNode, *(_quadTree->_quadTreeNodeCache.begin()), rr);
}

int TrjNodeManage::getPreciseNumber(int id, double r)
{
    return (int)this->getCoverTrj(id, r).size();
}

int TrjNodeManage::getRoughlyNumber(int id, double r)
{
    int result = 0;
    std::vector<QuadTreeNode*>::iterator it = _quadTree->_quadTreeNodeCache.begin();
    for (; it != _quadTree->_quadTreeNodeCache.end(); it++) {
        if ((*it)->_isLeaf && Util::minDistanceSqu(_nodeMap[id], *it) <= r * r) {
            result += (*it)->_trjNodeNumber;
        }
    }
    return result;
}
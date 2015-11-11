//
//  dbscan_class.cpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/11.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#include "dbscan_class.hpp"
#include "util.hpp"
#include "trj_node.hpp"
#include "quad_tree.hpp"

DbscanClass::DbscanClass()
: _id(-1)
, _trjNodeNumber(0)
, _isInitSuccess(false)
{
    
}

DbscanClass::~DbscanClass()
{
    if(_isInitSuccess)
    {
//        printf("销毁DBscan ID:%d \n", _id); //for debug
    }
}

DbscanClass* DbscanClass::create(std::string data)
{
    DbscanClass* dbscanClass = new DbscanClass();
    if (dbscanClass->init(data)) {
        return dbscanClass;
    }
    delete dbscanClass;
    return NULL;
}

bool DbscanClass::init(std::string data)
{
    std::vector<std::string> splitResult = Util::split(data, NodeInternalDateSep);
    if(splitResult.size() < 2)
    {
        printf("销毁数据有误，数据缺失");//for debug
        _isInitSuccess = false;
        return false;
    }
    _id = Util::stringToInt(splitResult[0]);
    if(_id < CLASS_ID_MIN)
    {
        printf("销毁数据有误，class id越界");//for debug
        _isInitSuccess = false;
        return false;
    }
    std::vector<std::string>::iterator it = splitResult.begin();
    for (it ++ ; it != splitResult.end(); it ++) {
        int nodeId = Util::stringToInt(*it);
        if (nodeId > TRJNODEMANAGE->_maxId || nodeId < TRJNODEMANAGE->_minId) {
            printf("销毁数据有误，class:%d 中 node id:%d 越界", _id, nodeId);//for debug
            _isInitSuccess = false;
            return false;
        }
        else
        {
            this->addNode(TRJNODEMANAGE->_nodeMap[nodeId]);
        }
    }
    printf("创建簇，class:%d\n", _id);//for debug
    _isInitSuccess = true;
    return true;
}


std::vector<TrjNode*> DbscanClass::getTrjNodeBelongQuadTreeNode(QuadTreeNode* node)
{
    std::vector<TrjNode*> result;
    std::vector<TrjNode*>::iterator it = _containNodes.begin();
    for (; it != _containNodes.end(); it++) {
        if (*((*it)->_belongQuadTreeNode) == *node) {
            result.push_back(*it);
        }
    }
    return result;
}

void DbscanClass::addNode(TrjNode* node)
{
    _containNodes.push_back(node);
    node->_belongDbscanClass = this;
    if (!Util::vectorContainQuadTreeNode(node->_belongQuadTreeNode, &_containQuadTreeNode)) {
        _containQuadTreeNode.push_back(node->_belongQuadTreeNode);
    }
    _trjNodeNumber ++;
}





static DbscanClassManage* _dbScanManageInstance = NULL;

DbscanClassManage* DbscanClassManage::instance()
{
    if(_dbScanManageInstance == NULL)
    {
        _dbScanManageInstance = new DbscanClassManage();
    }
    return _dbScanManageInstance;
}

void DbscanClassManage::purgeInstance()
{
    delete _dbScanManageInstance;
    _dbScanManageInstance = NULL;
}

DbscanClassManage::DbscanClassManage()
: _time(-1)
, _k(0)
, _m(0)
, _e(0)
{
    
}

DbscanClassManage::~DbscanClassManage()
{
    std::map<int, DbscanClass*>::iterator it = _dbscanClassMap.begin();
    for (; it != _dbscanClassMap.end(); it++) {
        delete it->second;
    }
    _dbscanClassMap.clear();
}

void DbscanClassManage::updateClass(std::string textData)
{
    std::map<int, DbscanClass*>::iterator dbscanIt = _dbscanClassMap.begin();
    for (; dbscanIt != _dbscanClassMap.end(); dbscanIt++) {
        delete dbscanIt->second;
    }
    _dbscanClassMap.clear();
    
    std::vector<std::string> splitResult = Util::split(textData, NodeDateSep);
    _time = Util::stringToInt(splitResult[0]);
    std::vector<std::string>::iterator it = splitResult.begin();
    for(it++; it != splitResult.end(); it++) {
        DbscanClass* dbscanClass = DbscanClass::create(*it);
        if (dbscanClass) {
            _dbscanClassMap.insert(std::map<int, DbscanClass*>::value_type(dbscanClass->_id, dbscanClass));
        }
    }
}







//
//  quad_tree.cpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#include "quad_tree.hpp"
#include "trj_node.hpp"
#include "util.hpp"

QuadTreeNode::QuadTreeNode()
: _isLeaf(false)
, _isBeMerge(false)
, _trjNodeNumber(0)
, _id(-1)
{
    
}

QuadTreeNode::~QuadTreeNode()
{
    
}

QuadTreeNode* QuadTreeNode::create(double xMin, double yMin, double xMax, double yMax, QuadTree* belongTree, QuadTreeNode* parentNode)
{
    QuadTreeNode* quadTreeNode = new QuadTreeNode();
    if(quadTreeNode->init(xMin, yMin, xMax, yMax, belongTree, parentNode))
    {
        return quadTreeNode;
    }
    delete quadTreeNode;
    return NULL;
    
}

bool QuadTreeNode::init(double xMin, double yMin, double xMax, double yMax, QuadTree* belongTree, QuadTreeNode* parentNode)
{
    if (xMin >= xMax || yMin >= yMax) {
        return false;
    }
    
    _xMin = xMin;
    _xMax = xMax;
    _yMin = yMin;
    _yMax = yMax;
    _belongTree = belongTree;
    _belongTree->_nodeMaxId++;
    _id = _belongTree->_nodeMaxId;
    _parentNode = parentNode;
    
    if (parentNode == NULL) {
        _belongTree->_quadTreeNodeCache.push_back(this);
    }
    
    return true;
}


void QuadTreeNode::addNode(TrjNode* node)
{
    if (Util::belongTo(node->getTrjNodeX(), _xMin, _xMax) && Util::belongTo(node->getTrjNodeY(), _yMin, _yMax)) {
        _trjNodeVector.push_back(node);
        _trjNodeNumber ++;
    }
}

void QuadTreeNode::mergeWithNextNerborNode(QuadTreeNode* nerborNode)
{
    if (nerborNode->_isLeaf) {
        return;
    }
    _xMax = _xMax > nerborNode->_xMax ? _xMax : nerborNode->_xMax;
    _xMin = _xMin < nerborNode->_xMin ? _xMin : nerborNode->_xMin;
    _yMin = _yMin < nerborNode->_yMin ? _yMin : nerborNode->_yMin;
    _yMax = _yMax > nerborNode->_yMax ? _yMax : nerborNode->_yMax;
    
    _trjNodeNumber += nerborNode->_trjNodeNumber;
    _trjNodeVector.insert(_trjNodeVector.end(), nerborNode->_trjNodeVector.begin(), nerborNode->_trjNodeVector.end());
    
    _nextNode = nerborNode->_nextNode;
    
    _isLeaf = true;

    nerborNode->_isBeMerge = true;
}

void QuadTreeNode::separate()
{
    if (_isLeaf) {
        std::vector<TrjNode*>::iterator it = _trjNodeVector.begin();
        for (; it != _trjNodeVector.end(); it ++) {
            (*it)->_belongQuadTreeNode = this;
        }
        return;
    }
    QuadTreeNode* quadTreeNode1 = QuadTreeNode::create(_xMin, (_yMin + _yMax) * 0.5, (_xMin + _xMax) * 0.5, _yMax, _belongTree, this);
    
    QuadTreeNode* quadTreeNode2 = QuadTreeNode::create((_xMin + _xMax) * 0.5, (_yMin + _yMax) * 0.5, _xMax, _yMax, _belongTree, this);
    
    QuadTreeNode* quadTreeNode3 = QuadTreeNode::create((_xMin + _xMax) * 0.5, _yMin, _xMax, (_yMin + _yMax) * 0.5, _belongTree, this);
    
    QuadTreeNode* quadTreeNode4 = QuadTreeNode::create(_xMin, _yMin, (_xMin + _xMax) * 0.5, (_yMin + _yMax) * 0.5, _belongTree, this);
    std::vector<TrjNode*>::iterator it = _trjNodeVector.begin();
    for (; it != _trjNodeVector.end(); it ++) {
        if((*it)->getTrjNodeX() <= (_xMin + _xMax) * 0.5)
        {
            if((*it)->getTrjNodeY() <= (_yMin + _yMax) * 0.5)
            {
                quadTreeNode4->addNode(*it);
            }
            else
            {
                quadTreeNode1->addNode(*it);
            }
        }
        else
        {
            if((*it)->getTrjNodeY() <= (_yMin + _yMax) * 0.5)
            {
                quadTreeNode3->addNode(*it);
            }
            else
            {
                quadTreeNode2->addNode(*it);
            }
        }
    }
    quadTreeNode1->_lastNode = quadTreeNode4;
    quadTreeNode1->_nextNode = quadTreeNode2;
    quadTreeNode2->_lastNode = quadTreeNode1;
    quadTreeNode2->_nextNode = quadTreeNode3;
    quadTreeNode3->_lastNode = quadTreeNode2;
    quadTreeNode3->_nextNode = quadTreeNode4;
    quadTreeNode4->_lastNode = quadTreeNode3;
    quadTreeNode4->_nextNode = quadTreeNode1;
    if (quadTreeNode1->_trjNodeNumber <= CELL_NODE_MAX && quadTreeNode2->_trjNodeNumber <= CELL_NODE_MAX && (quadTreeNode1->_trjNodeNumber + quadTreeNode2->_trjNodeNumber <= CELL_NODE_MAX)) {
        quadTreeNode1->mergeWithNextNerborNode(quadTreeNode2);
    }
    if (!quadTreeNode2->_isBeMerge && quadTreeNode2->_trjNodeNumber <= CELL_NODE_MAX && quadTreeNode3->_trjNodeNumber <= CELL_NODE_MAX && (quadTreeNode2->_trjNodeNumber + quadTreeNode3->_trjNodeNumber <= CELL_NODE_MAX)) {
        quadTreeNode2->mergeWithNextNerborNode(quadTreeNode3);
    }
    if (!quadTreeNode3->_isBeMerge && quadTreeNode3->_trjNodeNumber <= CELL_NODE_MAX && quadTreeNode4->_trjNodeNumber <= CELL_NODE_MAX && (quadTreeNode3->_trjNodeNumber + quadTreeNode4->_trjNodeNumber <= CELL_NODE_MAX)) {
        quadTreeNode3->mergeWithNextNerborNode(quadTreeNode4);
    }
    if (!quadTreeNode4->_isBeMerge && quadTreeNode4->_trjNodeNumber <= CELL_NODE_MAX && quadTreeNode1->_trjNodeNumber <= CELL_NODE_MAX && (quadTreeNode4->_trjNodeNumber + quadTreeNode1->_trjNodeNumber <= CELL_NODE_MAX)) {
        quadTreeNode4->mergeWithNextNerborNode(quadTreeNode1);
    }
    
    if (!quadTreeNode1->_isBeMerge) {
        if (quadTreeNode1->_trjNodeNumber <= CELL_NODE_MAX) {
            quadTreeNode1->_isLeaf = true;
        }
        quadTreeNode1->separate();
        _childNodeVector.push_back(quadTreeNode1);
    }
    else
    {
        delete quadTreeNode1;
    }
    if (!quadTreeNode2->_isBeMerge) {
        if (quadTreeNode2->_trjNodeNumber <= CELL_NODE_MAX) {
            quadTreeNode2->_isLeaf = true;
        }
        quadTreeNode2->separate();
        _childNodeVector.push_back(quadTreeNode2);
    }
    else
    {
        delete quadTreeNode2;
    }
    if (!quadTreeNode3->_isBeMerge) {
        if (quadTreeNode3->_trjNodeNumber <= CELL_NODE_MAX) {
            quadTreeNode3->_isLeaf = true;
        }
        quadTreeNode3->separate();
        _childNodeVector.push_back(quadTreeNode3);
    }
    else
    {
        delete quadTreeNode3;
    }
    if (!quadTreeNode4->_isBeMerge) {
        if (quadTreeNode4->_trjNodeNumber <= CELL_NODE_MAX) {
            quadTreeNode4->_isLeaf = true;
        }
        quadTreeNode4->separate();
        _childNodeVector.push_back(quadTreeNode4);
    }
    else
    {
        delete quadTreeNode4;
    }
    
    _belongTree->_quadTreeNodeCache.insert(_belongTree->_quadTreeNodeCache.end(), _childNodeVector.begin(), _childNodeVector.end());
}

bool QuadTreeNode::operator==(const QuadTreeNode& operatorNode)
{
    if (this->_id == operatorNode._id) {
        return true;
    }
    else
    {
        return false;
    }
}


QuadTree::QuadTree()
: _nodeMaxId(0)
{
    
}

QuadTree::~QuadTree()
{
    std::vector<QuadTreeNode*>::iterator it = _quadTreeNodeCache.begin();
    for (; it != _quadTreeNodeCache.end(); it ++) {
        delete *it;
    }
}



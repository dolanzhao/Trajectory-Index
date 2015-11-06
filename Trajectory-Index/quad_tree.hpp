//
//  quad_tree.hpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#ifndef quad_tree_hpp
#define quad_tree_hpp

#include <stdio.h>
#include "def.h"

class TrjNode;
class QuadTree;
class QuadTreeNode
{
public:
    
    QuadTreeNode();
    
    ~QuadTreeNode();
    
    static QuadTreeNode* create(double xMin, double yMin, double xMax, double yMax, QuadTree* belongTree, QuadTreeNode* _parentNode);

    void addNode(TrjNode* node);
    
    void mergeWithNextNerborNode(QuadTreeNode* nerborNode);
    
    void separate();
    
private:
    
    bool init(double xMin, double yMin, double xMax, double yMax, QuadTree* belongTree, QuadTreeNode* _parentNode);
    
public:
    
    QuadTree* _belongTree;
    
    QuadTreeNode* _parentNode;
    
    QuadTreeNode* _lastNode;
    
    QuadTreeNode* _nextNode;
    
    std::vector<QuadTreeNode*> _childNodeVector;
    
    int _trjNodeNumber;
    
    std::vector<TrjNode*> _trjNodeVector;
    
    double _xMax;
    
    double _xMin;
    
    double _yMax;
    
    double _yMin;
    
    bool _isLeaf;
    
    bool _isBeMerge;
};


class QuadTree
{
public:
    
    QuadTree();
    
    ~QuadTree();
    
private:
    
public:
    
    std::vector<QuadTreeNode*> _quadTreeNodeCache;
};

#endif /* quad_tree_hpp */

//
//  trj_node.hpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#ifndef trj_node_hpp
#define trj_node_hpp

#include "def.h"

class QuadTree;
class QuadTreeNode;
class TrjNode
{
public:
    
    TrjNode();
    
    ~TrjNode();
    
    static TrjNode* create(std::string nodeTextData);
    
    int getTrjNodeId();
    
    double getTrjNodeX();
    
    double getTrjNodeY();

private:
    
    bool init(std::string nodeTextData);
    
public:
    
    bool _isInitSuccess;
    
    int _id;
    
    double _x;
    
    double _y;
    
    QuadTreeNode* _belongQuadTreeNode;
    
};

class TrjNodeManage
{
public:
    
    static TrjNodeManage* instance(std::string textData);
    
    static void purgeInstance();
    
    void printCellNode();
    
    std::vector<TrjNode*> getCoverTrj(int id, double r);
    
    int getPreciseNumber(int id, double r);
    
    int getRoughlyNumber(int id, double r);
    
private:
    
    TrjNodeManage();
    
    ~TrjNodeManage();
    
    void init(std::string textData);
    
protected:
    
    std::map<int, TrjNode*> _nodeMap;
    
    QuadTree* _quadTree;
    
};


#endif /* trj_node_hpp */

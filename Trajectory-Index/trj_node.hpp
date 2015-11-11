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

#define TRJNODEMANAGE TrjNodeManage::instance()

class QuadTree;
class QuadTreeNode;
class DbscanClass;
class TrjNode
{
public:
    
    TrjNode();
    
    ~TrjNode();
    
    static TrjNode* create(std::string nodeTextData);
    
    int getTrjNodeId();
    
    double getTrjNodeX();
    
    double getTrjNodeY();
    
    bool operator==(const TrjNode & operatorNode);

private:
    
    bool init(std::string nodeTextData);
    
public:
    
    bool _isInitSuccess;
    
    int _id;
    
    double _x;
    
    double _y;
    
    QuadTreeNode* _belongQuadTreeNode;
    
    DbscanClass* _belongDbscanClass;
    
};

class TrjNodeManage
{
public:
    
    static TrjNodeManage* instance();
    
    static void purgeInstance();
    
    void updateNode(std::string textData);
    
    void printCellNode();
    
    std::vector<TrjNode*> getCoverTrj(int id, double r);
    
    std::vector<TrjNode*> getCoverTrj(TrjNode* trjNode, double r);
    
    std::vector<TrjNode*> getCoverTrjByRR(int id, double rr);
    
    std::vector<TrjNode*> getCoverTrjByRR(TrjNode* trjNode, double rr);
    
    int getPreciseNumber(int id, double r);
    
    int getRoughlyNumber(int id, double r);
    
private:
    
    TrjNodeManage();
    
    ~TrjNodeManage();
    
protected:
    
    QuadTree* _quadTree;
    
public:
    
    int _maxId;
    
    int _minId;
    
    int _time;
    
    std::map<int, TrjNode*> _nodeMap;
    
};


#endif /* trj_node_hpp */

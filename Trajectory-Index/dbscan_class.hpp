//
//  dbscan_class.hpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/11.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#ifndef dbscan_class_hpp
#define dbscan_class_hpp

#include "def.h"

class TrjNode;
class QuadTreeNode;
class DbscanClass
{
public:
    
    DbscanClass();
    
    ~DbscanClass();
    
    static DbscanClass* create(std::string data, int time);
    
    std::vector<TrjNode*> getTrjNodeBelongQuadTreeNode(QuadTreeNode* node);
    
    int getId();
    
private:
    
    bool init(std::string data, int time);
    
    void addNode(TrjNode* node);
    
    int _id;
    
public:
    
    bool _isInitSuccess;
    
    int _trjNodeNumber;
    
    std::vector<TrjNode*> _containNodes;
    
    std::vector<QuadTreeNode*> _containQuadTreeNode;
    
};



class DbscanClassManage
{
public:
    
    static DbscanClassManage* create(std::string textData);
    
    DbscanClassManage();
    
    ~DbscanClassManage();
    
private:
    
    void init(std::string textData);
    
public:
    
    int _time;
    
    std::map<int, DbscanClass*> _dbscanClassMap;
    
};

#endif /* dbscan_class_hpp */

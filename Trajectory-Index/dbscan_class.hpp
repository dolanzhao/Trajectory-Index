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

#define DBSCANCLASSMANAGE DbscanClassManage::instance()

class TrjNode;
class QuadTreeNode;
class DbscanClass
{
public:
    
    DbscanClass();
    
    ~DbscanClass();
    
    static DbscanClass* create(std::string data);
    
    std::vector<TrjNode*> getTrjNodeBelongQuadTreeNode(QuadTreeNode* node);
    
private:
    
    bool init(std::string data);
    
    void addNode(TrjNode* node);
    
public:
    
    bool _isInitSuccess;
    
    int _id;
    
    int _trjNodeNumber;
    
    std::vector<TrjNode*> _containNodes;
    
    std::vector<QuadTreeNode*> _containQuadTreeNode;
    
};



class DbscanClassManage
{
public:
    
    static DbscanClassManage* instance();
    
    static void purgeInstance();
    
    void updateClass(std::string textData);
    
private:
    
    DbscanClassManage();
    
    ~DbscanClassManage();
    
public:
    
    int _time;
    
    int _k;
    
    double _e;
    
    int _m;
    
    std::map<int, DbscanClass*> _dbscanClassMap;
    
};

#endif /* dbscan_class_hpp */

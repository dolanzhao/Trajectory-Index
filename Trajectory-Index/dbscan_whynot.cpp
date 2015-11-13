//
//  dbscan_whynot.cpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/13.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#include "dbscan_whynot.hpp"
#include "dbscan_class.hpp"
#include "trj_node.hpp"
#include "quad_tree.hpp"
#include "util.hpp"

static DbscanWhynot* _dbscanWhynotInstance = NULL;

DbscanWhynot* DbscanWhynot::instance()
{
    if(_dbscanWhynotInstance == NULL)
    {
        _dbscanWhynotInstance = new DbscanWhynot();
    }
    return _dbscanWhynotInstance;
}

void DbscanWhynot::purgeInstance()
{
    delete _dbscanWhynotInstance;
    _dbscanWhynotInstance = NULL;
}

DbscanWhynot::DbscanWhynot()
: _k(0)
, _m(0)
, _e(0)
{
    
}

DbscanWhynot::~DbscanWhynot()
{
    std::map<int,TrjNodeManage*>::iterator it1 = _trjNodeManageMap.begin();
    for (; it1 != _trjNodeManageMap.end(); it1++) {
        delete it1->second;
    }
    _trjNodeManageMap.clear();
    
    std::map<int,DbscanClassManage*>::iterator it2 = _dbscanClassManageMap.begin();
    for (; it2 != _dbscanClassManageMap.end(); it2++) {
        delete it2->second;
    }
    _dbscanClassManageMap.clear();
    
    
}

void DbscanWhynot::init(std::string trjNodeData, std::string dbscanClassData, int k, double e, int m)
{
    _k = k, _e = e, _m = m;
    std::vector<std::string> trjNodeDataVector = Util::split(trjNodeData, "#");
    std::vector<std::string> dbscanClassDataVector = Util::split(dbscanClassData, "#");
    
    std::vector<std::string>::iterator trjDataIt;
    std::vector<std::string>::iterator dbscanDataIt;
    _timeNumber = 0;
    for (; trjDataIt != trjNodeDataVector.begin(); trjDataIt++) {
        TrjNodeManage*trjNodeManage = TrjNodeManage::create(*trjDataIt);
        _trjNodeManageMap.insert(std::map<int, TrjNodeManage*>::value_type(trjNodeManage->_time, trjNodeManage));
        _timeNumber++;
    }
    
    for (; dbscanDataIt != dbscanClassDataVector.begin(); dbscanDataIt++) {
        DbscanClassManage* dbscanClassManage = DbscanClassManage::create(*dbscanDataIt);
        _dbscanClassManageMap.insert(std::map<int, DbscanClassManage*>::value_type(dbscanClassManage->_time, dbscanClassManage));
    }
}







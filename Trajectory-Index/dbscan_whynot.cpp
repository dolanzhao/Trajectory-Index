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

void DbscanWhynot::initNode(std::string nodeFilePath)
{
    
    std::ifstream file(nodeFilePath.c_str());
    std::string  lineStr;
    _timeNumber = 0;
    while(getline(file,lineStr))
    {
        TrjNodeManage*trjNodeManage = TrjNodeManage::create(lineStr);
        _trjNodeManageMap.insert(std::map<int, TrjNodeManage*>::value_type(trjNodeManage->_time, trjNodeManage));
        _timeNumber++;
    }
}

void DbscanWhynot::updateDbscanClass(std::string dbscanClassDataPath, std::string dbscanArgvData)
{
    std::vector<std::string> argvVector =  Util::split(dbscanArgvData, ";");
    _k = Util::stringToInt(argvVector[0]), _m = Util::stringToInt(argvVector[1]), _e = Util::stringToDouble(argvVector[2]), _misId = Util::stringToInt(argvVector[3]);
    
    std::map<int,DbscanClassManage*>::iterator it2 = _dbscanClassManageMap.begin();
    for (; it2 != _dbscanClassManageMap.end(); it2++) {
        delete it2->second;
    }
    _dbscanClassManageMap.clear();
    
    std::vector<std::string> dbscanClassDataVector = Util::readFileToVector(dbscanClassDataPath);
    std::vector<std::string>::iterator dbscanDataIt = dbscanClassDataVector.begin();
    for (; dbscanDataIt != dbscanClassDataVector.end(); dbscanDataIt++) {
        if (std::strcmp("", (*dbscanDataIt).c_str()) != 0) {
            DbscanClassManage* dbscanClassManage = DbscanClassManage::create(*dbscanDataIt);
            _dbscanClassManageMap.insert(std::map<int, DbscanClassManage*>::value_type(dbscanClassManage->_time, dbscanClassManage));
        }
        
    }
    
    _timeTargetMap.clear();
}

void DbscanWhynot::addTimeTargetSet(int minTime, int maxTime, int targetContainNodeId)
{
    for(int i = minTime; i <= maxTime; i++)
    {
        DbscanClass* tempDbscanClass = DBSCANWHYNOT->_trjNodeManageMap[i]->_nodeMap[targetContainNodeId]->_belongDbscanClass;
       _timeTargetMap.insert(std::map<int, int>::value_type(i, tempDbscanClass->_id));
    }
}







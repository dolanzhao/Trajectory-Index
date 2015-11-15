//
//  convoy.cpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/15.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#include "convoy.hpp"
#include "util.hpp"

Convoy* Convoy::create(std::string textData)
{
    Convoy* convoy = new Convoy();
    convoy->init(textData);
    return convoy;
}

Convoy::Convoy()
{
    
}

Convoy::~Convoy()
{
    
}

void Convoy::init(std::string textData)
{
    std::vector<std::string> tempString = Util::split(textData, "|");
    _id = Util::stringToInt(tempString[0]);
    _startTime = Util::stringToInt(Util::split(tempString[2], ";")[0]);
    _endTime = Util::stringToInt(Util::split(tempString[2], ";")[1]);
    std::vector<std::string> trjNodeStringVector = Util::split(tempString[1], ";");
    std::vector<std::string>::iterator it = trjNodeStringVector.begin();
    for (; it != trjNodeStringVector.end(); it ++) {
        _trjNodeId.insert(Util::stringToInt(*it));
    }
}


ConvoyManage* ConvoyManage::create(std::string filePath, std::string key)
{
    ConvoyManage* convoyManage = new ConvoyManage();
    convoyManage->init(filePath, key);
    return convoyManage;
}

ConvoyManage::ConvoyManage()
{
    
}

ConvoyManage::~ConvoyManage()
{
    std::map<int, Convoy*>::iterator it = _convoyMap.begin();
    
    for (; it != _convoyMap.end(); it++) {
        delete it->second;
    }
}

void ConvoyManage::init(std::string filePath, std::string key)
{
    _key = key;
    std::vector<std::string> convoyResult = Util::readFileToVector(filePath);
    std::vector<std::string>::iterator convoyIt = convoyResult.begin();
    for (; convoyIt != convoyResult.end(); convoyIt++)
    {
        Convoy* convoy = Convoy::create(*convoyIt);
        _convoyMap.insert(std::map<int, Convoy*>::value_type(convoy->_id, convoy));
    }
    
}

std::set<int> ConvoyManage::getAppearNode(int start, int end)
{
    std::set<int> result;
    std::map<int, Convoy*>::iterator it = _convoyMap.begin();
    for (; it != _convoyMap.end(); it++) {
        if ((it->second)->_startTime >= end && (it->second)->_endTime <= start) {
            result.insert((it->second)->_trjNodeId.begin(), (it->second)->_trjNodeId.end());
        }
    }
    
    return result;
}

int ConvoyManage::getStartTime()
{
    std::map<int, Convoy*>::iterator it = _convoyMap.begin();
    int result = it->second->_startTime;
    for (it++ ; it != _convoyMap.end(); it++) {
        if ((it->second)->_startTime < result) {
            result = it->second->_startTime;
        }
    }
    return result;
}

int ConvoyManage::getEndTime()
{
    std::map<int, Convoy*>::iterator it = _convoyMap.begin();
    int result = it->second->_endTime;
    for (it++ ; it != _convoyMap.end(); it++) {
        if ((it->second)->_endTime > result) {
            result = it->second->_endTime;
        }
    }
    return result;
}

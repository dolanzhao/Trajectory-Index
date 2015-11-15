//
//  convoy.hpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/15.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#ifndef convoy_hpp
#define convoy_hpp

#include "def.h"

class Convoy
{
public:
    
    static Convoy* create(std::string textData);
    
    Convoy();
    
    ~Convoy();
    
private:
    
    void init(std::string textData);
    
public:
    
    std::set<int> _trjNodeId;
    
    int _startTime;
    
    int _endTime;
    
    int _id;
    
};

class ConvoyManage
{
public:
    
    static ConvoyManage* create(std::string textData, std::string key);
    
    ConvoyManage();
    
    ~ConvoyManage();
    
    std::set<int> getAppearNode(int start, int end);
    
    int getStartTime();
    
    int getEndTime();
    
private:
    
    void init(std::string filePath, std::string key);
    
public:
    
    std::string _key;
    
    std::map<int, Convoy*> _convoyMap;
    
};

#endif /* convoy_hpp */

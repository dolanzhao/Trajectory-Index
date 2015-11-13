//
//  dbscan_whynot.hpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/13.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#ifndef dbscan_whynot_hpp
#define dbscan_whynot_hpp

#include "def.h"

#define DBSCANWHYNOT DbscanWhynot::instance()

class DbscanClassManage;
class TrjNodeManage;
class DbscanWhynot
{
public:
    
    static DbscanWhynot* instance();
    
    static void purgeInstance();

    void init(std::string trjNodeData, std::string dbscanClassData, int k, double e, int m);
    
private:
    
    DbscanWhynot();
    
    ~DbscanWhynot();

public:
    
    int _k;
    
    double _e;
    
    int _m;
    
    int _timeNumber;
    
    std::map<int,DbscanClassManage*> _dbscanClassManageMap;
    
    std::map<int,TrjNodeManage*> _trjNodeManageMap;
    
};


#endif /* dbscan_whynot_hpp */

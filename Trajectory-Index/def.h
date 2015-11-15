//
//  def.h
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#ifndef def_h
#define def_h

// 公共包含头文件
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <time.h>
#include <fstream>
#include <pthread.h> 

#define NodeDateSep "|"
#define NodeInternalDateSep ";"

const int NODE_ID_MIN = 0;
const int CLASS_ID_MIN = -1;
const double NODE_X_MIN = -10000.0;
const double NODE_X_MAX = 10000.0;
const double NODE_Y_MIN = -10000.0;
const double NODE_Y_MAX = 10000.0;

const double CELL_NODE_MAX = 6;
//const double CELL_NODE_MIN = 1;


enum WhyNotType
{
    MOE = 0,
    MOM,
    MOME,
    MOK
};

typedef struct{
    int _startTime;
    int _endTime;
    int _m;
    double _e;
    int _k;
    double _time;
    WhyNotType _type;
} ResultType;


#define FileLocal "/Users/dolan/workspace/GitClone/RExDBSCAN/dataset/"
enum DataSet
{
//    BusTrj = 0,
//    ChameleonTrj,
//    SyntheticTrj,
//    TdriveTrj,
//    TgpsTrj,
//    TruckTrj,
    Test,
    DataSetNumber
};

const std::string DataSetStr[] =
{
//    "bus",
//    "chameleon",
//    "synthetic",
//    "tdrive",
//    "tgps",
//    "truck",
    "test"
};

#endif /* def_h */

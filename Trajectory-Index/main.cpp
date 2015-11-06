//
//  main.cpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#include <iostream>
#include "trj_node.hpp"
#include "util.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    int id = 3;
    double r = 2.223;
    
    TrjNodeManage::instance("1|1;3;1|2;3;2|3;3;3|4;4;1|5;4;2|6;4;3|7;5;1|8;5;3|9;0;1|10;0;2|11;0;3|12;1;1|13;1;2|14;1;3")->printCellNode();
    printf("id:%d r:%lf  :", id, r);
    Util::printNodeId(TrjNodeManage::instance("1|1;3;1|2;3;2|3;3;3|4;4;1|5;4;2|6;4;3|7;5;1|8;5;3|9;0;1|10;0;2|11;0;3|12;1;1|13;1;2|14;1;3")->getCoverTrj(id, r));
    printf("\n");
    printf("PreciseNumber: %d \n", TrjNodeManage::instance("1|1;3;1|2;3;2|3;3;3|4;4;1|5;4;2|6;4;3|7;5;1|8;5;3|9;0;1|10;0;2|11;0;3|12;1;1|13;1;2|14;1;3")->getPreciseNumber(id, r));
    printf("RoughlyNumber: %d \n", TrjNodeManage::instance("1|1;3;1|2;3;2|3;3;3|4;4;1|5;4;2|6;4;3|7;5;1|8;5;3|9;0;1|10;0;2|11;0;3|12;1;1|13;1;2|14;1;3")->getRoughlyNumber(id, r));
    
    TrjNodeManage::purgeInstance();
    
    return 0;
}

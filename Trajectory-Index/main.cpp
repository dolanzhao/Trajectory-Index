//
//  main.cpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#include <iostream>
#include "trj_node.hpp"
#include "dbscan_class.hpp"
#include "util.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...

    
    TRJNODEMANAGE->updateNode("1|1;2;3|2;2;2|3;3;3|4;3;2|5;4;3|6;4;2|7;5.5;3|8;8;3|9;8;2|10;9;3|11;9;2|12;10;3|13;10;2|14;6;5");
    DBSCANCLASSMANAGE->updateClass("1|1;1;2;3;4;5;6;7|2;8;9;10;11;12;13|-1;14");
    DBSCANCLASSMANAGE->_e = 1.5;
    DBSCANCLASSMANAGE->_m = 4;
    
    printf("%lf %d\n", Util::getNewE(2, 7, 4, 1.5 * 1.5), Util::getNewM(1, 8, 4, 1.5 * 1.5));
    
    TrjNodeManage::purgeInstance();
    
    return 0;
}

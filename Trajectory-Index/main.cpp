//
//  main.cpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#include <iostream>
#include "dbscan_whynot.hpp"
#include "util.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    DBSCANWHYNOT->init("", 1, 1.5, 4);
    DBSCANWHYNOT->purgeInstance();
    return 0;
}

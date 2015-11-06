//
//  util.hpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#ifndef util_hpp
#define util_hpp

#include "def.h"


class TrjNode;
class QuadTreeNode;
class Util
{
public:
    
    static std::vector<std::string> split(const  std::string& s, const std::string& delim);
    
    static int stringToInt(const std::string& str);
    
    static float stringToFloat(const std::string& str);
    
    static double stringToDouble(const std::string& str);
    
    static bool belongTo(double temp, double min, double max);
    
    static double maxDistanceSqu(TrjNode* trjNode, QuadTreeNode* quadTreeNode);
    
    static double minDistanceSqu(TrjNode* trjNode, QuadTreeNode* quadTreeNode);
    
    static double distanceSqu(TrjNode* trjNode1, TrjNode* trjNode2);
    
    static std::vector<TrjNode*> getCoverTrj(TrjNode* trjNode, QuadTreeNode* quadTreeNode, double r);
    
    static void printNodeId(std::vector<TrjNode*> data);
    
};


#endif /* util_hpp */

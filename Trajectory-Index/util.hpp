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
class DbscanClass;
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
    
    static std::vector<TrjNode*> getCoverTrjByRR(TrjNode* trjNode, QuadTreeNode* quadTreeNode, double rr);
    
    static void printNodeId(std::vector<TrjNode*> data);
    
    static bool vectorContainQuadTreeNode(QuadTreeNode* node, std::vector<QuadTreeNode*>* container);
    
    static bool vectorContainTrjNode(TrjNode* node, std::vector<TrjNode*>* container);
    
    static double distanceSqu(TrjNode* trjNode, DbscanClass* dbNode);
    
    static double distanceSqu(DbscanClass* dbNode1, DbscanClass* dbNode2);
    
    static void expendTrjNodes(std::vector<TrjNode*>* hasOpNodes,std::vector<TrjNode*>* formalTrjNodes, double e, int m);
    
    static std::vector<TrjNode*> trjNodeVectorPlus(std::vector<TrjNode*>* vector1, std::vector<TrjNode*>* vector2);
    
    static std::list<double> getDistanceSortToUpList(DbscanClass* dbNode, std::vector<TrjNode*>* trjNodeVector);
    
    static double getNewE(int dbscanClassId, int trjNodeId, int m, double ee);
    
    static int getNewM(int dbscanClassId, int trjNodeId, int m, double ee);
    
    static std::map<int, double> getNewME(int dbscanClassId, int trjNodeId, int m, double ee);
    
};


#endif /* util_hpp */

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
class TrjNodeManage;
class Convoy;
class ConvoyManage;
class Util
{
public:
    
    static std::vector<std::string> split(const  std::string& s, const std::string& delim);
    
    static std::string intToString(int num);
    
    static std::string doubleToString(double num);
    
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
    
    static std::set<double> getAllDistanceSortToUpList(std::vector<TrjNode*>* trjNodeVector);
    
    static std::set<double> getAllDistanceSortToUpList(std::map<int, TrjNode*>* trjNodeMap);
    
    static double getNewE(int t, int dbscanClassId, int trjNodeId, int m, double ee);
    
    static int getNewM(int t, int dbscanClassId, int trjNodeId, int m, double ee);
    
    static std::map<int, double> getNewME(int t, int dbscanClassId, int trjNodeId, int m, double ee);
    
    static ResultType getMinE();
    
    static ResultType getMinM();
    
    static ResultType getMinK(std::vector<int> trjVector);
    
    static bool isInOneClass(std::vector<int>* trjVector, TrjNodeManage* trjNodeManage);
    
    static std::vector<ResultType>  getMinME();
    
    static std::string readFile(std::string filePath);
    
    static std::vector<std::string> readFileToVector(std::string filePath);
    
    static void writeFile(std::string filePath, std::vector<ResultType> result);
    
    static void writeFileQuery(std::string filePath, std::string dataSet, std::string mindataSet, std::vector<ResultType> result);
    
    static int compareScore(ConvoyManage* oldConvoyM, ConvoyManage* newConvoyM);
    
};


#endif /* util_hpp */

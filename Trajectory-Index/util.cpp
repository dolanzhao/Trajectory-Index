//
//  util.cpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#include "util.hpp"
#include <sstream>
#include <math.h>
#include "trj_node.hpp"
#include "quad_tree.hpp"
#include "dbscan_class.hpp"
#include "dbscan_whynot.hpp"
#include <assert.h>
#include "convoy.hpp"
#include <boost/algorithm/string.hpp>


std::vector<std::string> Util::split(const  std::string& s, const std::string& delim)
{
    std::vector<std::string> elems;
    boost::split(elems,s,boost::is_any_of(delim.c_str()));
    return elems;
}

std::string Util::intToString(int num)
{
    std::stringstream temp;
    temp<<num;
    std::string str = temp.str();
    return str;
}

std::string Util::doubleToString(double num)
{
    std::stringstream temp;
    temp<<num;
    std::string str = temp.str();
    return str;
}

int Util::stringToInt(const std::string& str)
{
    int num = 0;
    std::stringstream ss(str);
    ss >> num;
    return num;
}

float Util::stringToFloat(const std::string &str)
{
    float num = 0;
    std::stringstream ss(str);
    ss >> num;
    return num;
}

double Util::stringToDouble(const std::string &str)
{
    double num = 0;
    std::stringstream ss(str);
    ss >> num;
    return num;
}

bool Util::belongTo(double temp, double min, double max)
{
    if (temp >= min && temp <= max) {
        return true;
    }
    else
    {
        return false;
    }
}

double Util::maxDistanceSqu(TrjNode* trjNode, QuadTreeNode* quadTreeNode)
{
    double x1 = fabs(trjNode->getTrjNodeX() - quadTreeNode->_xMax), x2 = fabs(trjNode->getTrjNodeX() - quadTreeNode->_xMin);
    double xMax = x1 > x2 ? x1 : x2;
    
    double y1 = fabs(trjNode->getTrjNodeY() - quadTreeNode->_yMax), y2 = fabs(trjNode->getTrjNodeY() - quadTreeNode->_yMin);
    double yMax = y1 > y2 ? y1 : y2;
    
    return xMax * xMax + yMax * yMax;
}

double Util::minDistanceSqu(TrjNode* trjNode, QuadTreeNode* quadTreeNode)
{
    if (trjNode->getTrjNodeX() >= quadTreeNode->_xMin && trjNode->getTrjNodeX() <= quadTreeNode->_xMax && trjNode->getTrjNodeY() <= quadTreeNode->_yMax && trjNode->getTrjNodeY() >= quadTreeNode->_yMin) {
        return 0;
    }
    double x1 = fabs(trjNode->getTrjNodeX() - quadTreeNode->_xMax), x2 = fabs(trjNode->getTrjNodeX() - quadTreeNode->_xMin);
    double xMin = x1 < x2 ? x1 : x2;
    
    double y1 = fabs(trjNode->getTrjNodeY() - quadTreeNode->_yMax), y2 = fabs(trjNode->getTrjNodeY() - quadTreeNode->_yMin);
    double yMin = y1 < y2 ? y1 : y2;
    
    return xMin * xMin + yMin * yMin;
}

double Util::distanceSqu(TrjNode* trjNode1, TrjNode* trjNode2)
{
    double x = fabs(trjNode1->getTrjNodeX() - trjNode2->getTrjNodeX());
    double y = fabs(trjNode1->getTrjNodeY() - trjNode2->getTrjNodeY());
    
    return x * x + y * y;
}

std::vector<TrjNode*>  Util::getCoverTrj(TrjNode* trjNode, QuadTreeNode* quadTreeNode, double r)
{
    std::vector<TrjNode*> result;
    if ( Util::minDistanceSqu(trjNode, quadTreeNode) > r * r) {
        return result;
    }
    if (Util::maxDistanceSqu(trjNode, quadTreeNode) <= r * r) {
        result.insert(result.end(), quadTreeNode->_trjNodeVector.begin(), quadTreeNode->_trjNodeVector.end());
        return result;
    }
    if (quadTreeNode->_isLeaf) {
        std::vector<TrjNode*>::iterator it = quadTreeNode->_trjNodeVector.begin();
        for (; it != quadTreeNode->_trjNodeVector.end(); it ++) {
            if(*it != trjNode && Util::distanceSqu(*it, trjNode) <= r * r)
            {
                result.push_back(*it);
            }
        }
        return result;
    }
    else
    {
        std::vector<QuadTreeNode*>::iterator it = quadTreeNode->_childNodeVector.begin();
        for (; it != quadTreeNode->_childNodeVector.end(); it ++) {
            std::vector<TrjNode*> temp = Util::getCoverTrj(trjNode, *it, r);
            result.insert(result.end(), temp.begin(), temp.end());
        }
        return result;
    }
}

std::vector<TrjNode*>  Util::getCoverTrjByRR(TrjNode* trjNode, QuadTreeNode* quadTreeNode, double rr)
{
    std::vector<TrjNode*> result;
    if ( Util::minDistanceSqu(trjNode, quadTreeNode) > rr) {
        return result;
    }
    if (Util::maxDistanceSqu(trjNode, quadTreeNode) <= rr) {
        result.insert(result.end(), quadTreeNode->_trjNodeVector.begin(), quadTreeNode->_trjNodeVector.end());
        return result;
    }
    if (quadTreeNode->_isLeaf) {
        std::vector<TrjNode*>::iterator it = quadTreeNode->_trjNodeVector.begin();
        for (; it != quadTreeNode->_trjNodeVector.end(); it ++) {
            if(*it != trjNode && Util::distanceSqu(*it, trjNode) <= rr)
            {
                result.push_back(*it);
            }
        }
        return result;
    }
    else
    {
        std::vector<QuadTreeNode*>::iterator it = quadTreeNode->_childNodeVector.begin();
        for (; it != quadTreeNode->_childNodeVector.end(); it ++) {
            std::vector<TrjNode*> temp = Util::getCoverTrjByRR(trjNode, *it, rr);
            result.insert(result.end(), temp.begin(), temp.end());
        }
        return result;
    }
}

void Util::printNodeId(std::vector<TrjNode*> data)
{
    std::vector<TrjNode*>::iterator it = data.begin();
    for (; it != data.end(); it ++) {
        printf("%d ", (*it)->getTrjNodeId());
    }
}

bool Util::vectorContainQuadTreeNode(QuadTreeNode* node, std::vector<QuadTreeNode*>* container)
{
    std::vector<QuadTreeNode*>::iterator it = container->begin();
    for (; it != container->end(); it++) {
        if (*(*it) == *node) {
            return true;
        }
    }
    return false;
}

bool Util::vectorContainTrjNode(TrjNode* node, std::vector<TrjNode*>* container)
{
    if (container->size() == 0) {
        return false;
    }
    std::vector<TrjNode*>::iterator it = container->begin();
    for (; it != container->end(); it++) {
        if (*(*it) == *node) {
            return true;
        }
    }
    return false;
}

double Util::distanceSqu(TrjNode* trjNode, DbscanClass* dbNode)
{
    double result = (NODE_X_MAX - NODE_X_MIN) * (NODE_X_MAX - NODE_X_MIN);
    std::vector<QuadTreeNode*> tempQtNode = dbNode->_containQuadTreeNode;
    std::vector<TrjNode*> tempTrjNode = dbNode->_containNodes;
    std::vector<QuadTreeNode*>::iterator tempIt = tempQtNode.begin();
    for (; tempIt != tempQtNode.end(); tempIt++) {
        if (Util::minDistanceSqu(trjNode, *tempIt) >= result) {
            continue;
        }
        std::vector<TrjNode*> tempTrjNode = dbNode->getTrjNodeBelongQuadTreeNode(*tempIt);
        std::vector<TrjNode*>::iterator tempInIt = tempTrjNode.begin();
        for (; tempInIt != tempTrjNode.end(); tempInIt++) {
            double tempDis = Util::distanceSqu(trjNode, *tempInIt);
            if(trjNode->_belongTrjNodeManage->getCoverTrjByRR(*tempInIt, tempDis).size() >= DBSCANWHYNOT->_m)
            {
                if (result > tempDis) {
                    result = tempDis;
                }
            }
        }
    }
    
    return result;
}

double Util::distanceSqu(DbscanClass* dbNode1, DbscanClass* dbNode2)
{
    double result = 0;
    for (int i = 0; i < dbNode1->_trjNodeNumber; i++) {
        double temp = Util::distanceSqu(dbNode1->_containNodes[i], dbNode2);
        if (result > temp) {
            result = temp;
        }
    }
    return result;
}

std::list<double> Util::getDistanceSortToUpList(DbscanClass* dbNode, std::vector<TrjNode*>* trjNodeVector)
{
    std::list<double> result;
    std::vector<TrjNode*>::iterator it = trjNodeVector->begin();
    for (; it != trjNodeVector->end(); it++) {
        double temp = Util::distanceSqu(*it, dbNode);
        result.push_back(temp);
    }
    result.sort();
    return result;
}

std::vector<TrjNode*> Util::trjNodeVectorPlus(std::vector<TrjNode*>* vector1, std::vector<TrjNode*>* vector2)
{
    std::vector<TrjNode*> result;
    std::vector<TrjNode*>::iterator it = vector1->begin();
    for (; it != vector1->end(); it++) {
        bool contain = false;
        std::vector<TrjNode*>::iterator it2 = vector2->begin();
        for (; it2 != vector2->end(); it2++) {
            if (*it2 == *it) {
                contain = true;
            }
        }
        if (!contain) {
            result.push_back(*it);
        }
    }
    return result;
}

double Util::getNewE(int t, int dbscanClassId, int trjNodeId, int m, double ee)
{
    DbscanClass* dbNode = DBSCANWHYNOT->_dbscanClassManageMap[t]->_dbscanClassMap[dbscanClassId];
    TrjNode* trjNode = DBSCANWHYNOT->_trjNodeManageMap[t]->_nodeMap[trjNodeId];
    double result = Util::distanceSqu(trjNode, dbNode);
    if (result <= ee) {
        return ee;
    }
    std::vector<TrjNode*> formalTrjNodes = dbNode->_containNodes;
    std::vector<TrjNode*> ignoreNode;
    Util::expendTrjNodes(&ignoreNode, &formalTrjNodes, result, m);
    std::vector<TrjNode*> tempVector = Util::trjNodeVectorPlus(&formalTrjNodes, &(dbNode->_containNodes));
    std::list<double> resultList = Util::getDistanceSortToUpList(dbNode, &tempVector);
    std::list<double>::iterator it = resultList.begin();
    for (; it != resultList.end(); it++) {
        if (*it < result) {
            ignoreNode.clear();
            if (trjNode->_belongDbscanClass->getId() == -1 && trjNode->_belongTrjNodeManage->getCoverTrj(trjNode, *it).size() < m) {
                break;
            }
            std::vector<TrjNode*> tempTrjNodes = dbNode->_containNodes;
            Util::expendTrjNodes(&ignoreNode, &tempTrjNodes, *it, m);
            if (Util::vectorContainTrjNode(trjNode, &tempTrjNodes)) {
                return *it;
            }
        }
        else
        {
            break;
        }
    }
    return result;
}

int Util::getNewM(int t, int dbscanClassId, int trjNodeId, int m, double ee)
{
    int result = 1;
    DbscanClass* dbNode = DBSCANWHYNOT->_dbscanClassManageMap[t]->_dbscanClassMap[dbscanClassId];
    TrjNode* trjNode = DBSCANWHYNOT->_trjNodeManageMap[t]->_nodeMap[trjNodeId];
    
    for(int i = m; i >= 2; i--)
    {
        std::vector<TrjNode*> formalTrjNodes = dbNode->_containNodes;
        std::vector<TrjNode*> ignoreNode;
        Util::expendTrjNodes(&ignoreNode, &formalTrjNodes, ee, i);
        if (Util::vectorContainTrjNode(trjNode, &formalTrjNodes)) {
            result = i;
            break;
        }
    }
    return result;
}

std::map<int, double> Util::getNewME(int t, int dbscanClassId, int trjNodeId, int m, double ee)
{
    std::map<int, double> result;
    DbscanClass* dbNode = DBSCANWHYNOT->_dbscanClassManageMap[t]->_dbscanClassMap[dbscanClassId];
    TrjNode* trjNode = DBSCANWHYNOT->_trjNodeManageMap[t]->_nodeMap[trjNodeId];
    double up = Util::distanceSqu(trjNode, dbNode);
    for (int i = m; i > 1; i--) {
        std::vector<TrjNode*> formalTrjNodes = dbNode->_containNodes;
        std::vector<TrjNode*> ignoreNode;
        Util::expendTrjNodes(&ignoreNode, &formalTrjNodes, up, i);
        std::vector<TrjNode*> tempVector = Util::trjNodeVectorPlus(&formalTrjNodes, &(dbNode->_containNodes));
        std::list<double> resultList = Util::getDistanceSortToUpList(dbNode, &tempVector);
        std::list<double>::iterator it = resultList.begin();
        for (; it != resultList.end(); it++) {
            if (*it < up) {
                ignoreNode.clear();
                std::vector<TrjNode*> tempTrjNodes = dbNode->_containNodes;
                Util::expendTrjNodes(&ignoreNode, &tempTrjNodes, *it, m);
                if (Util::vectorContainTrjNode(trjNode, &tempTrjNodes)) {
                    up = *it;
                    break;
                }
            }
            else
            {
                break;
            }
        }
        result.insert(std::map<int, double>::value_type(i, up));
    }
    return result;
}

ResultType Util::getMinE()
{
    ResultType result;
    clock_t t_start, t_end;
    t_start = clock() ;
    std::map<int, double> minEMap;
    
    std::map<int,int>::iterator it = DBSCANWHYNOT->_timeTargetMap.begin();
    for (; it != DBSCANWHYNOT->_timeTargetMap.end(); it++) {
        minEMap.insert(std::map<int, double>::value_type(it->first, Util::getNewE(it->first, it->second, DBSCANWHYNOT->_misId, DBSCANWHYNOT->_m, DBSCANWHYNOT->_e * DBSCANWHYNOT->_e)));
    }
    
    result._m = DBSCANWHYNOT->_m;
    result._k = DBSCANWHYNOT->_k;
    result._type = MOE;
    std::map<int, double>::iterator it2 = minEMap.begin();
    std::list<std::map<int, double>::iterator> tempList;
    double minE = NODE_X_MAX * NODE_X_MAX;
    int startTime = 0, endTime = 0;
    for (; it2 != minEMap.end(); it2++) {
        std::map<int, double>::iterator it3 = it2;
        if(it2 != minEMap.begin() && (it2->first - (--it3)->first) > 1) {
            tempList.clear();
        }
        if (tempList.size() < DBSCANWHYNOT->_k) {
            tempList.push_back(it2);
            if (tempList.size() ==  DBSCANWHYNOT->_k) {
                double tempMax = DBSCANWHYNOT->_e;
                std::list<std::map<int, double>::iterator>::iterator it3 = tempList.begin();
                for (; it3 != tempList.end(); it3++) {
                    if ((*it3)->second > tempMax) {
                        tempMax = (*it3)->second;
                    }
                }
                if(minE > tempMax)
                {
                    startTime = tempList.front()->first;
                    endTime = tempList.back()->first;
                    minE = tempMax;
                }
            }
        }
        else
        {
            tempList.pop_front();
            tempList.push_back(it2);
            double tempMax = DBSCANWHYNOT->_e;
            std::list<std::map<int, double>::iterator>::iterator it3 = tempList.begin();
            for (; it3 != tempList.end(); it3++) {
                if ((*it3)->second > tempMax) {
                    tempMax = (*it3)->second;
                }
            }
            if(minE > tempMax)
            {
                startTime = tempList.front()->first;
                endTime = tempList.back()->first;
                minE = tempMax;
            }
        }
    }
    result._e = sqrt(minE);
    t_end = clock();
    result._time = (double)(t_end-t_start)/CLOCKS_PER_SEC;
    result._startTime = startTime;
    result._endTime = endTime;
    
    return result;
}

ResultType Util::getMinM()
{
    clock_t t_start, t_end;
    t_start = clock() ;
    ResultType result;
    std::map<int, int> minMMap;
    
    std::map<int,int>::iterator it = DBSCANWHYNOT->_timeTargetMap.begin();
    for (; it != DBSCANWHYNOT->_timeTargetMap.end(); it++) {
        int result = Util::getNewM(it->first, it->second, DBSCANWHYNOT->_misId, DBSCANWHYNOT->_m, DBSCANWHYNOT->_e * DBSCANWHYNOT->_e);
        if (result > 1) {
            minMMap.insert(std::map<int, int>::value_type(it->first, result));
        }
        
    }
    result._e = DBSCANWHYNOT->_e;
    result._k = DBSCANWHYNOT->_k;
    result._type = MOM;
    std::map<int, int>::iterator it2 = minMMap.begin();
    std::list<std::map<int, int>::iterator> tempList;
    int maxM = 1;
    int startTime = 0, endTime = 0;
    for (; it2 != minMMap.end(); it2++) {
        std::map<int, int>::iterator it3 = it2;
        if( it2 != minMMap.begin() && (it2->first - (--it3)->first) > 1) {
            tempList.clear();
        }
        if (tempList.size() < DBSCANWHYNOT->_k) {
            tempList.push_back(it2);
            int tempMin = DBSCANWHYNOT->_m;
            std::list<std::map<int, int>::iterator>::iterator it3 = tempList.begin();
            for (; it3 != tempList.end(); it3++) {
                if ((*it3)->second < tempMin) {
                    tempMin = (*it3)->second;
                }
            }
            if(maxM < tempMin)
            {
                startTime = tempList.front()->first;
                endTime = tempList.back()->first;
                maxM = tempMin;
            }
        }
        else
        {
            tempList.pop_front();
            tempList.push_back(it2);
            int tempMin = DBSCANWHYNOT->_m;
            std::list<std::map<int, int>::iterator>::iterator it3 = tempList.begin();
            for (; it3 != tempList.end(); it3++) {
                if ((*it3)->second < tempMin) {
                    tempMin = (*it3)->second;
                }
            }
            if(maxM < tempMin)
            {
                startTime = tempList.front()->first;
                endTime = tempList.back()->first;
                maxM = tempMin;
            }
        }
    }
    t_end = clock();
    result._time = (double)(t_end-t_start)/CLOCKS_PER_SEC;
    result._m = maxM;
    result._startTime = startTime;
    result._endTime = endTime;
    
    return result;
}

std::vector<ResultType> Util::getMinME()
{
    std::vector<ResultType> resultVector;
    
    for (int m = DBSCANWHYNOT->_m - 1; m > 1; m --) {
        ResultType result;
        clock_t t_start,t_end;
        t_start = clock();
        std::map<int, double> minEMap;
        
        std::map<int,int>::iterator it = DBSCANWHYNOT->_timeTargetMap.begin();
        for (; it != DBSCANWHYNOT->_timeTargetMap.end(); it++) {
            minEMap.insert(std::map<int, double>::value_type(it->first, Util::getNewE(it->first, it->second, DBSCANWHYNOT->_misId, m, DBSCANWHYNOT->_e * DBSCANWHYNOT->_e)));
        }
        
        result._m = m;
        result._k = DBSCANWHYNOT->_k;
        result._type = MOME;
        std::map<int, double>::iterator it2 = minEMap.begin();
        std::list<std::map<int, double>::iterator> tempList;
        double minE = NODE_X_MAX * NODE_X_MAX;
        int startTime = 0, endTime = 0;
        for (; it2 != minEMap.end(); it2++) {
            std::map<int, double>::iterator it3 = it2;
            if( it2 != minEMap.begin() && (it2->first - (--it3)->first) > 1) {
                tempList.clear();
            }
            if (tempList.size() < DBSCANWHYNOT->_k) {
                tempList.push_back(it2);
                if (tempList.size() ==  DBSCANWHYNOT->_k) {
                    double tempMax = DBSCANWHYNOT->_e;
                    std::list<std::map<int, double>::iterator>::iterator it3 = tempList.begin();
                    for (; it3 != tempList.end(); it3++) {
                        if ((*it3)->second > tempMax) {
                            tempMax = (*it3)->second;
                        }
                    }
                    if(minE > tempMax)
                    {
                        startTime = tempList.front()->first;
                        endTime = tempList.back()->first;
                        minE = tempMax;
                    }
                }
            }
            else
            {
                tempList.pop_front();
                tempList.push_back(it2);
                double tempMax = DBSCANWHYNOT->_e;
                std::list<std::map<int, double>::iterator>::iterator it3 = tempList.begin();
                for (; it3 != tempList.end(); it3++) {
                    if ((*it3)->second > tempMax) {
                        tempMax = (*it3)->second;
                    }
                }
                if(minE > tempMax)
                {
                    startTime = tempList.front()->first;
                    endTime = tempList.back()->first;
                    minE = tempMax;
                }
            }
        }
        result._e = sqrt(minE);
        t_end = clock();
        result._time = (double)(t_end-t_start)/CLOCKS_PER_SEC;
        result._startTime = startTime;
        result._endTime = endTime;
        
        resultVector.push_back(result);
    }
    
    return resultVector;
}

bool Util::isInOneClass(std::vector<int>* trjVector, TrjNodeManage* trjNodeManage)
{
    std::vector<int>::iterator it = (*trjVector).begin();
    if(trjNodeManage->_nodeMap[*(it)] == NULL)
    {
        return false;
    }
    int classId = trjNodeManage->_nodeMap[*(it)]->_belongDbscanClass->getId();
    if (classId == -1)
    {
        return false;
    }
    for (it ++ ; it != (*trjVector).end(); it ++) {
        if(trjNodeManage->_nodeMap[*(it)] != NULL)
        {
            int tempClassId = trjNodeManage->_nodeMap[*(it)]->_belongDbscanClass->getId();
            if (classId != tempClassId) {
                return false;
            }

        }
        else
        {
            return false;
        }
    }
    return true;
}

ResultType Util::getMinK(std::vector<int> trjVector)
{
    std::vector<int>::iterator trjVectorIt = trjVector.begin();
    for (; trjVectorIt != trjVector.end(); trjVectorIt++) {
        printf(" %d", *trjVectorIt);
    }
    printf("\n");
    ResultType result;
    result._e = DBSCANWHYNOT->_e;
    result._m = DBSCANWHYNOT->_m;
    result._type = MOK;
    clock_t t_start,t_end;
    t_start = clock();
    std::vector<int> contain;
    std::map<int,TrjNodeManage*>* trjNodeManageMap = &DBSCANWHYNOT->_trjNodeManageMap;
    std::map<int,TrjNodeManage*>::iterator it = (*trjNodeManageMap).begin();
    std::map<int,DbscanClassManage*>::iterator it2 = DBSCANWHYNOT->_dbscanClassManageMap.begin();
    for (; it2 != DBSCANWHYNOT->_dbscanClassManageMap.end(); it2++, it++) {
        if (Util::isInOneClass(&trjVector, it->second)) {
            contain.push_back(it->first);
        }
    }
    
    int k = 0, temp = 0, start = 0, end = 0;
    std::vector<int>::iterator containIt = contain.begin();
    for (; containIt != contain.end(); containIt++) {
        if (containIt != contain.begin()) {
            std::vector<int>::iterator tempFrontContainIt = containIt;
            tempFrontContainIt--;
            if ((*containIt - *tempFrontContainIt) > 1) {
                if (temp > k) {
                    k = temp;
                    start = *containIt - k + 1;
                    end = *containIt;
                }
                temp = 1;
            }
            else
            {
                temp++;
            }
        }
        else
        {
             temp++;
        }
    }
    
    result._k = k;
    result._startTime = start;
    result._endTime = end;
    t_end = clock();
    result._time = (double)(t_end-t_start)/CLOCKS_PER_SEC;
    
    return result;
}


void Util::expendTrjNodes(std::vector<TrjNode*>* ignoreNode, std::vector<TrjNode*>* formalTrjNodes, double ee, int m)
{
//    std::vector<TrjNode*>::iterator it = formalTrjNodes->begin();
    for (int i = 0; i < formalTrjNodes->size(); i++) {
        
        if (!Util::vectorContainTrjNode((*formalTrjNodes)[i], ignoreNode)) {
            ignoreNode->push_back((*formalTrjNodes)[i]);
            std::vector<TrjNode*> temp = (*formalTrjNodes)[i]->_belongTrjNodeManage->getCoverTrjByRR((*formalTrjNodes)[i], ee);
            if (temp.size() >= m) {
                std::vector<TrjNode*>::iterator it2 = temp.begin();
                for (; it2 != temp.end(); it2++) {
                    if (!Util::vectorContainTrjNode(*it2, formalTrjNodes)) {
                        formalTrjNodes->push_back(*it2);
                    }
                }
            }
        }
    }
    if (formalTrjNodes->size() > ignoreNode->size()) {
        expendTrjNodes(ignoreNode, formalTrjNodes, ee, m);
    }
}

std::vector<std::string> Util::readFileToVector(std::string filePath)
{
    std::vector<std::string> result;
    std::ifstream file(filePath.c_str());
    std::string  lineStr;
    while(getline(file,lineStr))
    {
        result.push_back(lineStr);
    }
    return result;
}

std::string Util::readFile(std::string filePath)
{
    unsigned long * pSize;
    unsigned char * pBuffer;
    printf("file path %s", filePath.c_str());
    FILE *fp = fopen(filePath.c_str(), "r");
    
    fseek(fp,0,SEEK_END);
    *pSize = ftell(fp);
    fseek(fp,0,SEEK_SET);
    pBuffer = new unsigned char[*pSize];
    *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
    fclose(fp);
    std::string result = (char*)pBuffer;
    return result;
}

void Util::writeFile(std::string filePath, std::vector<ResultType> result)
{
    std::string resultStr = "WhyNotType;CostTime;K;E;M;StartTime;EndTime";
    std::vector<ResultType>::iterator it = result.begin();
    for (; it != result.end(); it++) {
        resultStr += "\n" + Util::intToString((int)(*it)._type)
        + ";" + Util::doubleToString((*it)._time)
        + ";" + Util::intToString((*it)._k)
        + ";" + Util::intToString((*it)._m)
        + ";" + Util::doubleToString((*it)._e)
        + ";" + Util::intToString((*it)._startTime)
        + ";" + Util::intToString((*it)._endTime);
    }
    
    std::ofstream fout(filePath.c_str());
    fout<<resultStr;
}

int Util::compareScore(ConvoyManage* oldConvoyM, ConvoyManage* newConvoyM)
{
    std::set<int> oldTrjNumber = oldConvoyM->getAppearNode(newConvoyM->getStartTime(), newConvoyM->getEndTime());
    std::set<int> newTrjNumber = newConvoyM->getAppearNode(newConvoyM->getStartTime(), newConvoyM->getEndTime());
    std::set<int> result(newTrjNumber.begin(), newTrjNumber.end());
    result.insert(oldTrjNumber.begin(), oldTrjNumber.end());
    return (int)(oldTrjNumber.size() + newTrjNumber.size() - result.size());
}
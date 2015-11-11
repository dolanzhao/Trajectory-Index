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

std::vector<std::string> Util::split(const  std::string& s, const std::string& delim)
{
    std::vector<std::string> elems;
    size_t pos = 0;
    size_t len = s.length();
    size_t delim_len = delim.length();
    if (delim_len == 0) return elems;
    while (pos < len)
    {
        int find_pos = (int)s.find(delim, pos);
        if (find_pos < 0)
        {
            elems.push_back(s.substr(pos, len - pos));
            break;
        }
        elems.push_back(s.substr(pos, find_pos - pos));
        pos = find_pos + delim_len;
    }
    return elems;
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
//    std::vector<QuadTreeNode*> tempQtNode = dbNode->_containQuadTreeNode;
    std::vector<TrjNode*> tempTrjNode = dbNode->_containNodes;
    std::vector<TrjNode*>::iterator tempIt = tempTrjNode.begin();
    for (; tempIt != tempTrjNode.end(); tempIt++) {
//        if (Util::minDistanceSqu(trjNode, *tempIt) >= result) {
//            continue;
//        }
//        std::vector<TrjNode*> tempTrjNode = dbNode->getTrjNodeBelongQuadTreeNode(*tempIt);
//        std::vector<TrjNode*>::iterator tempInIt = tempTrjNode.begin();
//        for (; tempInIt != tempTrjNode.end(); tempInIt++) {
            double tempDis = Util::distanceSqu(trjNode, *tempIt);
            if (result > tempDis) {
                result = tempDis;
            }
//        }
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

double Util::getNewE(int dbscanClassId, int trjNodeId, int m, double ee)
{
    DbscanClass* dbNode = DBSCANCLASSMANAGE->_dbscanClassMap[dbscanClassId];
    TrjNode* trjNode = TRJNODEMANAGE->_nodeMap[trjNodeId];
    double result = Util::distanceSqu(trjNode, dbNode);
    if (result < ee) {
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

int Util::getNewM(int dbscanClassId, int trjNodeId, int m, double ee)
{
    int result = 1;
    DbscanClass* dbNode = DBSCANCLASSMANAGE->_dbscanClassMap[dbscanClassId];
    TrjNode* trjNode = TRJNODEMANAGE->_nodeMap[trjNodeId];
    
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

std::map<int, double> Util::getNewME(int dbscanClassId, int trjNodeId, int m, double ee)
{
    std::map<int, double> result;
    DbscanClass* dbNode = DBSCANCLASSMANAGE->_dbscanClassMap[dbscanClassId];
    TrjNode* trjNode = TRJNODEMANAGE->_nodeMap[trjNodeId];
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



void Util::expendTrjNodes(std::vector<TrjNode*>* ignoreNode, std::vector<TrjNode*>* formalTrjNodes, double ee, int m)
{
//    std::vector<TrjNode*>::iterator it = formalTrjNodes->begin();
    for (int i = 0; i < formalTrjNodes->size(); i++) {
        
        if (!Util::vectorContainTrjNode((*formalTrjNodes)[i], ignoreNode)) {
            ignoreNode->push_back((*formalTrjNodes)[i]);
            std::vector<TrjNode*> temp = TRJNODEMANAGE->getCoverTrjByRR((*formalTrjNodes)[i], ee);
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



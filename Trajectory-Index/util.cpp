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
            if(Util::distanceSqu(*it, trjNode) <= r * r)
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

void Util::printNodeId(std::vector<TrjNode*> data)
{
    std::vector<TrjNode*>::iterator it = data.begin();
    for (; it != data.end(); it ++) {
        printf("%d ", (*it)->getTrjNodeId());
    }
}
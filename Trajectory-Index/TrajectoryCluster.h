#ifndef TrajectoryCluster_H
#define TrajectoryCluster_H

#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include "DataPoint.h"
#include "Parameters.h"
#include "RStarTree.h"
#include <algorithm>
#include <boost/unordered_map.hpp> 
#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;
using namespace boost;
using namespace std;

//聚类分析类型
class TrajectoryCluster
{
private:
    vector<DataPoint> dataSets;        //数据集合
    unsigned int dimNum;            //维度
    double radius;                    //半径
    unsigned int dataNum;            //数据数量
    unsigned int minPTs;            //邻域最小数据个数
    double GetDistance(DataPoint& dp1, DataPoint& dp2);                    //距离函数
    void SetArrivalPoints(DataPoint& dp, RTree &rtree);                                //设置数据点的领域点列表
    void KeyPointCluster( unsigned long i, unsigned long clusterId );    //对数据点领域内的点执行聚类操作
    int Init(vector<string> &pointSet, double &radius, int &minPTs, RTree &rtree);   
public:


    TrajectoryCluster(){};                    //默认构造函数
    
    bool DoDBSCANRecursive();            //DBSCAN递归算法
    bool WriteToFile(double radius, int minPTs, ofstream &clResult, ofstream &coreIDsRecord, ofstream &borderIDsRecord);    //将聚类结果写入文件
    //random
    //bool DoDBSCANRecursive(fstream &coreIDsRecord, double radius, int minPTs, ofstream &randomResult, ofstream &randomIDResult);  
    //int RandomDBSCAN(double radius, int minPTs, ofstream &randomResult, ofstream &randomIDResult);

    int queryTrajectoryCluster(int &durationTime, double &radius, int &minPTs, fstream &clInput, int &dataSize, ofstream &queryResult, ofstream &tpcdResult, ofstream &clusterResult, ofstream &kconvoyResult);
    int queryTrajectoryCluster(int &durationTime, double &radius, int &minPTs, fstream &clInput, ofstream &queryResult, int &tstart, int &tend);
    int getClusterSet(vector<vector<long> > &clusterSet, int &minPTs, ofstream &clusterResult, ofstream &tpcdResult);
    int getClusterSet(vector<vector<long> > &clusterSet, int &minPTs);
    ~TrajectoryCluster(){};
};


#endif

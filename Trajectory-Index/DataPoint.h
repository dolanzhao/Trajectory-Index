#ifndef DataPoint_H
#define DataPoint_H

#include <vector>

using namespace std;

const int DIME_NUM=2;        //数据维度为2，全局常量

//数据点类型
class DataPoint
{
private:
    unsigned long dpID;                //数据点ID
    unsigned long filedpID;
    double dimension[DIME_NUM];        //维度数据
    long clusterId;                    //所属聚类ID
    bool isKey;                        //是否核心对象
    bool visited;                    //是否已访问
    vector<unsigned long> arrivalPoints;    //领域数据点id列表
public:
    DataPoint();                                                    //默认构造函数
    DataPoint(unsigned long dpID,double* dimension , bool isKey);    //构造函数
    unsigned long GetFileDpId();                //GetDpId方法
    void SetFileDpId(unsigned long filedpID);        //SetDpId方法
    unsigned long GetDpId();                //GetDpId方法
    void SetDpId(unsigned long dpID);        //SetDpId方法
    double* GetDimension();                    //GetDimension方法
    void SetDimension(double* dimension);    //SetDimension方法
    bool IsKey();                            //GetIsKey方法
    void SetKey(bool isKey);                //SetKey方法
    bool isVisited();                        //GetIsVisited方法
    void SetVisited(bool visited);            //SetIsVisited方法
    long GetClusterId();                    //GetClusterId方法
    void SetClusterId(long classId);        //SetClusterId方法
    vector<unsigned long>& GetArrivalPoints();    //GetArrivalPoints方法
};


#endif

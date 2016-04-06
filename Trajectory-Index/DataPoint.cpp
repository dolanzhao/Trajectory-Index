#include "DataPoint.h"

//默认构造函数
DataPoint::DataPoint()
{
}

//构造函数
DataPoint::DataPoint(unsigned long dpID,double* dimension , bool isKey):isKey(isKey),dpID(dpID)
{
    //传递每维的维度数据
    for(int i=0; i<DIME_NUM;i++)
    {
        this->dimension[i]=dimension[i];
    }
}

//设置维度数据
void DataPoint::SetDimension(double* dimension)
{
    for(int i=0; i<DIME_NUM;i++)
    {
        this->dimension[i]=dimension[i];
    }
}

//获取维度数据
double* DataPoint::GetDimension()
{
    return this->dimension;
}

//获取是否为核心对象
bool DataPoint::IsKey()
{
    return this->isKey;
}

//设置核心对象标志
void DataPoint::SetKey(bool isKey)
{
    this->isKey = isKey;
}
//获取FileDpId方法
unsigned long DataPoint::GetFileDpId()
{
    return this->filedpID;
}

//设置FileDpId方法
void DataPoint::SetFileDpId(unsigned long filedpID)
{
    this->filedpID = filedpID;
}
//获取DpId方法
unsigned long DataPoint::GetDpId()
{
    return this->dpID;
}

//设置DpId方法
void DataPoint::SetDpId(unsigned long dpID)
{
    this->dpID = dpID;
}

//GetIsVisited方法
bool DataPoint::isVisited()
{
    return this->visited;
}


//SetIsVisited方法
void DataPoint::SetVisited( bool visited )
{
    this->visited = visited;
}

//GetClusterId方法
long DataPoint::GetClusterId()
{
    return this->clusterId;
}

//GetClusterId方法
void DataPoint::SetClusterId( long clusterId )
{
    this->clusterId = clusterId;
}

//GetArrivalPoints方法
vector<unsigned long>& DataPoint::GetArrivalPoints()
{
    return arrivalPoints;
}

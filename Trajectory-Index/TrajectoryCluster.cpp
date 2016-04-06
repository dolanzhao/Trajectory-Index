#include "TrajectoryCluster.h"
#include <fstream>
#include <iosfwd>
#include <math.h>
#include <string>
#include <boost/unordered_map.hpp> 
#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;
using namespace boost;
using namespace std;
#include <iostream>

/*
函数：聚类初始化操作
说明：将数据文件名，半径，领域最小数据个数信息写入聚类算法类，读取文件，把数据信息读入写进算法类数据集合中
参数：
double radius;    //半径
int minPTs;        //领域最小数据个数  
返回值： 0;    */


int gid=1;
//judge if one int is contained in one vector
int containInt(int &rowID,vector<int> &rowIDSet)
{
   	int cid=0;
   	for(int i=0;i<rowIDSet.size();i++)
   	{
      		if(rowIDSet[i]==rowID)
      		{
         		cid=1;
         		break;
      		}
   	}
   	return cid;
}
//judge if one long is contained in one vector
int containLong(long &rowID,vector<long> &rowIDSet)
{
   	int cid=0;
   	for(int i=0;i<rowIDSet.size();i++)
   	{
      		if(rowIDSet[i]==rowID)
      		{
         		cid=1;
         		break;
      		}
   	}
   	return cid;
}
int TrajectoryCluster::Init(vector<string> &pointSet, double &radius, int &minPTs, RTree &rtree)
{
	this->radius = radius;        //设置半径
    	this->minPTs = minPTs;        //设置领域最小数据个数
    	this->dimNum = DIME_NUM;    //设置数据维度
    	unsigned long pointNum=0;            //数据个数统计
    	rtree.create(32,0.7);
    	double tempx;
	int i=0, j=0;
	//cout<<pointSet[0]<<endl;
    	for(i=1;i<pointSet.size();i++)                //从文件中读取POI信息，将POI信息写入POI列表中
    	{
        	DataPoint tempDP;                //临时数据点对象
        	double tempDimData[DIME_NUM];    //临时数据点维度信息
		vector<string> dp2;		
		vector<string> dp;
		split(dp2,pointSet[i],is_any_of(";"));
		split(dp,dp2[1],is_any_of(","));
        	for(j=0; j<DIME_NUM; j++)    //读文件，读取每一维数据
        	{
                tempDimData[j]=tech::stod(dp[j]);
        	}
        	tempDP.SetDimension(tempDimData);    //将维度信息存入数据点对象内
        	tempDP.SetDpId(pointNum);                    //将数据点对象ID设置为i
		tempDP.SetFileDpId(tech::stod(dp[0]));
        	tempDP.SetVisited(false);            //数据点对象isVisited设置为false
        	tempDP.SetClusterId(-1);            //设置默认簇ID为-1
        	dataSets.push_back(tempDP);            //将对象压入数据集合容器
        	//i++;        //计数+1
		//insert r*tree
		Rectangle mbr;
		mbr.min.x=tempDimData[0];
		mbr.min.y=tempDimData[1];
		mbr.max.x=mbr.min.x;
		mbr.max.y=mbr.min.y;
		Object obj;
		obj.mbr=mbr;
		obj.id=pointNum;
		rtree.insertData(obj);
		pointNum++;
    	}
    	dataNum =pointNum;            //设置数据对象集合大小为i
	//cout<<"dataNum: "<<dataNum<<endl;
    	for(unsigned long i=0; i<dataNum;i++)
    	{
        	SetArrivalPoints(dataSets[i],rtree);            //计算数据点领域内对象
    	}
    	return true;    //返回
}
int TrajectoryCluster::getClusterSet(vector<vector<long> > &clusterSet, int &minPTs, ofstream &clusterResult, ofstream &tpcdResult)
{
	int i=0;
	boost::unordered_map<long,vector<long> > clusterMap;
	boost::unordered_map<long,vector<long> >::iterator cmit;
	for(i=0; i<dataNum; i++)
	{
		long clid=dataSets[i].GetClusterId();
		//cout<<i<<" : "<<dataSets[i].GetDpId()<<" : "<<clid<<endl;
		tpcdResult<<dataSets[i].GetDpId()+1<<";"<<clid<<";"<<dataSets[i].GetArrivalPoints().size()<<"|";				
		cmit=clusterMap.find(clid);
		if(cmit!=clusterMap.end())
		{
			cmit->second.push_back(dataSets[i].GetDpId()+1);
		}else
		{
			vector<long> idSet;
			idSet.push_back(dataSets[i].GetDpId()+1);
			clusterMap.insert(pair<long, vector<long> > (clid,idSet));
		}
	
    	}
	tpcdResult<<endl;
	for(cmit=clusterMap.begin();cmit!=clusterMap.end();++cmit)
	{
		clusterResult<<cmit->first<<";";
		for(i=0; i<cmit->second.size()-1; i++)
		{
			clusterResult<<cmit->second[i]<<";"; 
		}
		clusterResult<<cmit->second[i]<<"|";
		if((cmit->first!=-1)&&(cmit->second.size()>=minPTs))
		{
			clusterSet.push_back(cmit->second);
		}
	}
	clusterResult<<endl;
	//cout<<"e: "<<minPTs<<endl;
	//cout<<clusterSet.size()<<endl;
	return 0;
	
}
int copyVector(vector<long> &v1, vector<long> &candidate, vector<long> &v2)
{
	int i=0;
	long sTime=candidate[candidate.size()-2];
	//cout<<sTime<<endl;
	v1.clear();
	for(i=0;i<v2.size();i++)
	{
		v1.push_back(v2[i]);
	}
	v1.push_back(sTime);
	return 0;

}

int check(vector<long> &v1, vector<vector<long> > &v2)
{
	int res=0;
	int num=0;
	int num2=0;
	for(int i=0; i<v2.size(); i++)
	{
		num=0;
		for(int j=0;j<v1.size();j++)
		{
			if(containLong(v1[j],v2[i])==0)
			{
				break;
			}else
			{
				num++;
			}
			if(num==v1.size())
			{
				num2++;
			}
		}
	}
	if(num2>=2)
	{
		res=1;
	}
	return res;

}
int checkEqual(vector<long> &v1, vector<long> &v2)
{
	int res=0;
	int eNum=0;
	for(int i=0; i<v1.size()-2; i++)
	{
		if(v1[i]==v2[i])
		{
			eNum++;
		}
		else
		{
			break;
		}
	}
	if(eNum==v1.size()-2)
	{
		res=1;
	}
	return res;
}
int updateCandidate(vector<long> &v1, vector<vector<long> > &compareResult)
{
	//vector<vector<long> > interResult;
	int res=0;
	for(int i=0; i<compareResult.size();i++)
	{
		if(v1.size()!=compareResult[i].size())
		{
			continue;
		}else
		{	
			if(checkEqual(v1, compareResult[i])==1)
			{
				if(v1[v1.size()-2]<compareResult[i][compareResult[i].size()-2])
				{
					compareResult[i][compareResult[i].size()-2]=v1[v1.size()-2];
				}
				res=1;
				break;
			}	
		}
	}
	if(res==0)
	{
		compareResult.push_back(v1);
		//cout<<"compareResult: "<<compareResult.size()<<endl;
	}
	return 0;
}
int getIntersection(vector<vector<long> > &candidateConvoy, vector<vector<long> > &clusterSet, int &minPTs, int &durationTime, ofstream &queryResult, int &tID)
{
	vector<vector<long> > compareResult;
	long sTime=0, eTime=0;
	vector<long> v3;
	vector<long> v1;
	int i=0, j=0, k=0, m=0, update=0, interS=0;
	vector<int> alExist;
	//cout<<tID<<" : "<<candidateConvoy.size()<<endl;
	if(candidateConvoy.size()==0)
	{
		for(i=0; i<clusterSet.size(); i++)
		{
			clusterSet[i].push_back(tID);
			clusterSet[i].push_back(tID);
			candidateConvoy.push_back(clusterSet[i]);
		}
	}
	else
	{
		for(j=0;j<candidateConvoy.size();j++)
		{
			interS=0;
			update=0;
			for(k=0;k<clusterSet.size();k++)
			{
				sort(candidateConvoy[j].begin(),candidateConvoy[j].end()-2);
				sort(clusterSet[k].begin(),clusterSet[k].end());
				v3.clear();
				set_intersection(candidateConvoy[j].begin(),candidateConvoy[j].end()-2,clusterSet[k].begin(),clusterSet[k].end(),back_inserter(v3));
				if(v3.size()==0)
				{
					continue;
				}else
				{
					interS=1;
					if(v3.size()>=minPTs)
					{
						if(v3.size()==clusterSet[k].size())
						{
							alExist.push_back(k);
						}
						if(v3.size()==(candidateConvoy[j].size()-2))
						{
							update=1;
						}
						//vector<long> v1;
						copyVector(v1,candidateConvoy[j],v3);	
						v1.push_back(tID);
						updateCandidate(v1, compareResult);
						/*
						if(check(v3,candidateConvoy)==1)
						{
							//cout<<tID<< " : " <<endl;
							if(v3.size()==clusterSet[k].size())
							{
								//cout<<"dfd f "<<tID<<endl;
								updateCandidate(v1, compareResult);
								//cout<<v1.size()<<" : "<<compareResult.size()<<endl;
							}else
							{
								//cout<<"tID: "<<tID<<endl;
								compareResult.push_back(v1);
							}
						}else
						{
							compareResult.push_back(v1);
						}*/		
					}

				}
			}
			if(interS==0||update==0)
			{
				sTime=candidateConvoy[j][candidateConvoy[j].size()-2];
				eTime=candidateConvoy[j][candidateConvoy[j].size()-1];
				if((eTime-sTime+1)>=durationTime)
				{
					queryResult<<gid<<"|";
					for(m=0; m<candidateConvoy[j].size()-3; m++)
					{
						queryResult<<candidateConvoy[j][m]<<";";
					}
					queryResult<<candidateConvoy[j][m]<<"|"<<sTime<<";"<<eTime<<endl;
					gid++;
				}
			
			}
		}		
		candidateConvoy.clear();
		//cout<<"compareResult: "<<compareResult.size()<<endl;
		for(i=0;i<compareResult.size();i++)
		{
			candidateConvoy.push_back(compareResult[i]);
		}
		for(i=0;i<clusterSet.size();i++)
		{
			if(containInt(i,alExist)==0)
			{
				//cout<<i<<endl;
				clusterSet[i].push_back(tID);
				clusterSet[i].push_back(tID);
				candidateConvoy.push_back(clusterSet[i]);
			}
		}

	}
	//cout<<"houxuan: "<<candidateConvoy.size()<<endl;
	return 0;
	
}
int TrajectoryCluster::queryTrajectoryCluster(int &durationTime, double &radius, int &minPTs, fstream &clInput, int &dataSize, ofstream &queryResult, ofstream &tpcdResult, ofstream &clusterResult, ofstream &kconvoyResult)
{
	cout<<durationTime<<": "<<radius<<": "<<minPTs<<endl;
	string oneline;
	int tID=0;
	int i=0,j=0,k=0;
	long eTime=0, sTime=0;
	vector<vector<long> > candidateConvoy; 
	vector<vector<long> > clusterSet;
	vector<string> pVector;
	//vector<vector<long> > convoy;
	while(getline(clInput,oneline))
	{
		//cout<<oneline<<endl;
		vector<string> tmp;
		split(tmp,oneline,is_any_of("|"));
		tID=stoi(tmp[0]);
		if(tmp.size()<minPTs+1)
		{
			for(i=0; i<candidateConvoy.size(); i++)
			{
				sTime=candidateConvoy[i][candidateConvoy[i].size()-2];
				eTime=candidateConvoy[i][candidateConvoy[i].size()-1];
				if((eTime-sTime+1)>=durationTime)
				{
					queryResult<<gid<<"|";
					for(j=0; j<candidateConvoy[i].size()-3; j++)
					{
						queryResult<<candidateConvoy[i][j]<<";";
					}
					queryResult<<candidateConvoy[i][j]<<"|"<<sTime<<";"<<eTime<<endl;
					gid++;
				}
				
			}
			clusterResult<<tID<<"|-1;";
			for(j=1;j<tmp.size()-1;j++)
			{
				split(pVector, tmp[j], is_any_of(";"));
				clusterResult<<pVector[0]<<";";
			}
			split(pVector, tmp[j], is_any_of(";"));
			clusterResult<<pVector[0]<<"|"<<endl;
			candidateConvoy.clear();
			continue;
		}
		dataSets.clear();
		Storage st;
		RTree rtree(&st);
		Init(tmp,radius,minPTs,rtree);
		DoDBSCANRecursive();
		clusterSet.clear();
		clusterResult<<tID<<"|";
		tpcdResult<<tID<<"|";		
		getClusterSet(clusterSet, minPTs, clusterResult, tpcdResult);
		getIntersection(candidateConvoy,clusterSet,minPTs,durationTime,queryResult,tID);
		if(tID==dataSize)
		{
			break;
		}
		
	}
	//cout<<"gid: "<<gid<<endl;
	for(i=0; i<candidateConvoy.size(); i++)
	{
		sTime=candidateConvoy[i][candidateConvoy[i].size()-2];
		eTime=candidateConvoy[i][candidateConvoy[i].size()-1];
		if((eTime-sTime+1)>=durationTime)
		{
			queryResult<<gid<<"|";
			for(j=0; j<candidateConvoy[i].size()-3; j++)
			{
				queryResult<<candidateConvoy[i][j]<<";";
			}
			queryResult<<candidateConvoy[i][j]<<"|"<<sTime<<";"<<eTime<<endl;
			gid++;
		}
	}
//	cout<<"the number of conovy: "<<gid-1<<endl;
	return 0;
}

//tstart to tend executing convoy
//new
int TrajectoryCluster::getClusterSet(vector<vector<long> > &clusterSet, int &minPTs)
{
	int i=0;
	boost::unordered_map<long,vector<long> > clusterMap;
	boost::unordered_map<long,vector<long> >::iterator cmit;
	for(i=0; i<dataNum; i++)
	{
		long clid=dataSets[i].GetClusterId();				
		cmit=clusterMap.find(clid);
		if(cmit!=clusterMap.end())
		{
			cmit->second.push_back(dataSets[i].GetDpId()+1);
		}else
		{
			vector<long> idSet;
			idSet.push_back(dataSets[i].GetDpId()+1);
			clusterMap.insert(pair<long, vector<long> > (clid,idSet));
		}
	
    	}
	//tpcdResult<<endl;
	for(cmit=clusterMap.begin();cmit!=clusterMap.end();++cmit)
	{
		/*clusterResult<<cmit->first<<";";
		for(i=0; i<cmit->second.size()-1; i++)
		{
			clusterResult<<cmit->second[i]<<";"; 
		}
		clusterResult<<cmit->second[i]<<"|";*/
		if((cmit->first!=-1)&&(cmit->second.size()>=minPTs))
		{
			clusterSet.push_back(cmit->second);
		}
	}
	//clusterResult<<endl;
	//cout<<"e: "<<minPTs<<endl;
	//cout<<clusterSet.size()<<endl;
	return 0;

}
int TrajectoryCluster::queryTrajectoryCluster(int &durationTime, double &radius, int &minPTs, fstream &clInput, ofstream &queryResult, int &tstart, int &tend)
{
	cout<<durationTime<<": "<<radius<<" : "<<minPTs<<endl;
	clock_t start, finish;
	double duration=0;
	string oneline;
	int tID=0;
	int i=0,j=0,k=0,ttnum=0;
	vector<vector<long> > candidateConvoy; 
	vector<vector<long> > clusterSet;
	//vector<vector<long> > convoy;
	if(tstart>1)
	{
		while(getline(clInput,oneline))
		{
			ttnum++;
			if(ttnum==tstart-1)
			{
				break;
			}
		}
	}
	while(getline(clInput,oneline))
	{
		vector<string> tmp;
		split(tmp,oneline,is_any_of("|"));
		tID=stoi(tmp[0]);
		if(tmp.size()<minPTs+1)
		{
			if(tID==tend)
			{
				break;
			}else
			{
				continue;
			}
		}
		dataSets.clear();
		Storage st;
		RTree rtree(&st);
		Init(tmp,radius,minPTs,rtree);
		start= clock();
		DoDBSCANRecursive();
		clusterSet.clear();
		//clusterResult<<tID<<"|";
		//tpcdResult<<tID<<"|";	
			
		getClusterSet(clusterSet, minPTs);
		getIntersection(candidateConvoy,clusterSet,minPTs,durationTime,queryResult,tID);
		if(tID==tend)
		{
			break;
		}
		finish= clock();
		duration += (double)(finish-start)/CLOCKS_PER_SEC;
	}
	long eTime=0, sTime=0;
	//cout<<"candidate convoy: "<<candidateConvoy.size()<<endl;
	for(i=0; i<candidateConvoy.size(); i++)
	{
		sTime=candidateConvoy[i][candidateConvoy[i].size()-2];
		eTime=candidateConvoy[i][candidateConvoy[i].size()-1];
		if((eTime-sTime+1)>=durationTime)
		{
			queryResult<<gid<<"|";
			for(j=0; j<candidateConvoy[i].size()-3; j++)
			{
				queryResult<<candidateConvoy[i][j]<<";";
			}
			queryResult<<candidateConvoy[i][j]<<"|"<<sTime<<";"<<eTime<<endl;
			gid++;
		}
	}
//	cout<<"the number of conovy: "<<gid-1<<endl;
//	cout<<"the execution time of convoy: "<<duration<<" s"<<endl;
	return 0;
}
/*
函数：将已经过聚类算法处理的数据集合写回文件
说明：将已经过聚类结果写回文件
参数：
char* fileName;    //要写入的文件名
返回值： true    */

vector<long> clidSet;
vector<long> cluIdSet;
long borderIDNum=0;
bool TrajectoryCluster::WriteToFile(double radius, int minPTs, ofstream &clResult, ofstream &coreIDsRecord, ofstream &borderIDsRecord)
{
    	int i=0, j=0, m=0;
    	boost::unordered_map<long,vector<vector<double> > > cluster;
    	boost::unordered_map<long,vector<vector<double> > >::iterator cit;
    	int coreIDNum=0;
	long clusterPointNum=0;
	//cout<<dataSets.size()<<endl;
    	for(i=0;i<dataNum;i++)
   	{
		//get core points 
		long clid=dataSets[i].GetClusterId();
		if(clid!=-1)
		{
			clusterPointNum++;
			if(dataSets[i].IsKey())
			{
				if(!tech::ifContainInt(clid,cluIdSet))
				{
					coreIDsRecord<<i<<",";
					cluIdSet.push_back(clid);
				}
				coreIDNum++;
			}else
			{
				borderIDsRecord<<i<<",";
				borderIDNum++;
			}
			cit=cluster.find(clid);
			if(cit!=cluster.end())
			{
				vector<double> coordinate;
				for(j=0; j<DIME_NUM; j++)
				{
					coordinate.push_back(dataSets[i].GetDimension()[j]);
				}
				cit->second.push_back(coordinate);
			}else
			{
				vector<vector<double> > cooset;
				vector<double> coordinate;
				for( j=0; j<DIME_NUM; j++)
				{
					coordinate.push_back(dataSets[i].GetDimension()[j]);
				}
				cooset.push_back(coordinate);
				cluster.insert(pair<long, vector<vector<double> > > (clid,cooset));
			}
	
    		}
	}
	borderIDsRecord<<endl;
	borderIDsRecord<<clusterPointNum<<endl;
	coreIDsRecord<<endl;
	for(i=0;i<dataNum;i++)
	{
		for(j=0;j<DIME_NUM-1;j++)
		{
			clResult<<dataSets[i].GetDimension()[j]<<",";
		}
		clResult<<dataSets[i].GetDimension()[j]<<" | ";
		
	}
	clResult<<endl;
	clResult<<endl;
	int clusterNum=0;
	for(cit=cluster.begin();cit!=cluster.end();++cit)
	{
		
		clResult<<"clusterID: "<<cit->first<<endl;
		for(i=0;i<cit->second.size();i++)
		{
			for(j=0;j<DIME_NUM-1;j++)
			{
				clResult<<"("<<cit->second[i][j]<<",";		

			}
			clResult<<cit->second[i][j]<<"),";
		}
		clResult<<endl;
		clusterNum++;
	}	
	clResult<<endl;
	clResult<<"the parameter: radius:"<<radius<<" minPTs:"<<minPTs<<endl;
        clResult<<"the number of data points: "<<dataNum<<endl;
	clResult<<"the number of clustered points: "<<clusterPointNum<<endl;
	clResult<<"the number of noise points: "<<dataNum-clusterPointNum<<endl;
	clResult<<"the number of cluster: "<<clusterNum<<endl;
	
	//print the screen
	float coreProCl=0, coreProAll=0;
	coreProCl=(float)coreIDNum/(float)clusterPointNum;
	coreProAll=(float)coreIDNum/(float)dataNum;
	cout<<"the parameter: radius:"<<radius<<" minPTs:"<<minPTs<<endl;
        cout<<"the number of data points: "<<dataNum<<endl;
	cout<<"the number of cluster: "<<clusterNum<<endl;
	cout<<"the number of clustered points: "<<clusterPointNum<<endl;
	cout<<"the number of core points: "<<coreIDNum<<endl;
	cout<<"the number of border points: "<<borderIDNum<<endl;
	cout<<"the number of noise points: "<<dataNum-clusterPointNum<<endl;
	cout<<"the pro of core points in clusted points: "<<coreProCl<<endl;
	cout<<"the pro of core points in all points: "<<coreProAll<<endl;
    	
    	return true;    //返回
}
/*
函数：设置数据点的领域点列表
说明：设置数据点的领域点列表
参数：
返回值： true;    */
void TrajectoryCluster::SetArrivalPoints(DataPoint& dp, RTree &rtree)
{
    vector<Object> objects;
    Rectangle range;
    //cout<<"radius: "<<radius<<endl;
    range.min.x = dp.GetDimension()[0] - radius;
    range.min.y = dp.GetDimension()[1] - radius;
    range.max.x = dp.GetDimension()[0] + radius;
    range.max.y = dp.GetDimension()[1] + radius;
    //get objects
    rtree.rangeQuery(objects, range);
    for(unsigned long i=0; i<objects.size(); i++)                //对每个数据点执行
    {
	long tmpid=objects[i].id;
        double distance =GetDistance(dataSets[tmpid], dp);    //获取与特定点之间的距离
        if(distance <= radius)        
            dp.GetArrivalPoints().push_back(tmpid);            //将特定点id压力dp的领域列表中
    }
    //cout<<dp.GetArrivalPoints().size()<<endl;
    if(dp.GetArrivalPoints().size() >= minPTs)            //若dp领域内数据点数据量> minPTs执行
    {
	
        dp.SetKey(true);    //将dp核心对象标志位设为true
        return;                //返回
    }
    dp.SetKey(false);    //若非核心对象，则将dp核心对象标志位设为false
}


/*
函数：执行聚类操作
说明：执行聚类操作
参数：
返回值： true;    */
bool TrajectoryCluster::DoDBSCANRecursive()
{
    unsigned long clusterId=1;                        //聚类id计数，初始化为0
    for(unsigned long i=0; i<dataNum;i++)            //对每一个数据点执行
    {
        DataPoint& dp=dataSets[i];                    //取到第i个数据点对象
        if(!dp.isVisited() && dp.IsKey())            //若对象没被访问过，并且是核心对象执行
        {
            dp.SetClusterId(clusterId);                //设置该对象所属簇ID为clusterId
            dp.SetVisited(true);                    //设置该对象已被访问过
            KeyPointCluster(i,clusterId);            //对该对象领域内点进行聚类
            clusterId++;                            //clusterId自增1
        }
        //cout << "孤立点\T" << i << endl;
    }
    //cout<<"The number of data points: "<<dataNum<<endl;
   // cout <<"共聚类" <<clusterId-1<<"个"<< endl;        //算法完成后，输出聚类个数
    return true;    //返回
}

/*
函数：对数据点领域内的点执行聚类操作
说明：采用递归的方法，深度优先聚类数据
参数：
unsigned long dpID;            //数据点id
unsigned long clusterId;    //数据点所属簇id
返回值： void;    */
void TrajectoryCluster::KeyPointCluster(unsigned long dpID, unsigned long clusterId)
{
    DataPoint& srcDp = dataSets[dpID];        //获取数据点对象
    if(!srcDp.IsKey())    return;
    vector<unsigned long>& arrvalPoints = srcDp.GetArrivalPoints();        //获取对象领域内点ID列表
    for(unsigned long i=0; i<arrvalPoints.size(); i++)
    {
        DataPoint& desDp = dataSets[arrvalPoints[i]];    //获取领域内点数据点
        if(!desDp.isVisited())                            //若该对象没有被访问过执行
        {
            //cout << "数据点\t"<< desDp.GetDpId()<<"聚类ID为\t" <<clusterId << endl;
            desDp.SetClusterId(clusterId);        //设置该对象所属簇的ID为clusterId，即将该对象吸入簇中
            desDp.SetVisited(true);                //设置该对象已被访问
            if(desDp.IsKey())                    //若该对象是核心对象
            {
                KeyPointCluster(desDp.GetDpId(),clusterId);    //递归地对该领域点数据的领域内的点执行聚类操作，采用深度优先方法
            }
        }
    }
    
}

//两数据点之间距离
/*
函数：获取两数据点之间距离
说明：获取两数据点之间的欧式距离
参数：
DataPoint& dp1;        //数据点1
DataPoint& dp2;        //数据点2
返回值： double;    //两点之间的距离   */    
double TrajectoryCluster::GetDistance(DataPoint& dp1, DataPoint& dp2)
{
    double distance =0;        //初始化距离为0
    for(int i=0; i<DIME_NUM;i++)    //对数据每一维数据执行
    {
        //cout<<dp1.GetDimension()[i]<<" | "<<dp1.GetDimension()[i]<<endl;
        distance += pow(dp1.GetDimension()[i] - dp2.GetDimension()[i],2);    //距离+每一维差的平方
    }
    return pow(distance,0.5);        //开方并返回距离
}


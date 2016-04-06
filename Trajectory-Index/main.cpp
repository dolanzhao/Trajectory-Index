#include "TrajectoryCluster.h"
#include "AnswerWhyNot.h"
#include "Parameters.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <time.h>
using namespace std;

double getConvoyResult(std::vector<std::string> argv, int type, std::string id)
{
    //time variables
    clock_t start, finish;
    double duration=0;
    //convoy query
    std::string typeStr = "";
    if (type == 0) {
        typeStr = "_e";
    }
    else if (type == 1) {
        typeStr = "_m";
    }
    else if (type == 2) {
        typeStr = "_me" + id;
    }
    else if (type == 3) {
        typeStr = "_k";
    }
    
    if(!std::strcmp(argv[1].c_str(), "query") && std::strcmp(argv[7].c_str(), "1"))
    {
        TrajectoryCluster trajectoryCluster;
        string dataDir, convoyResultDir, tpcdResultDir, clusterResultDir, kconvoyResultDir;
        dataDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+argv[2]+"/"+argv[3]+".txt";
        convoyResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+argv[2]+"/"+argv[4]+"/"+"convoyResult"+typeStr+".txt";
        if(!std::strcmp(argv[8].c_str(), "interval"))
        {
            convoyResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+argv[2]+"/"+argv[4]+"/"+"convoyResult_interval"+typeStr+".txt";
        }
        tpcdResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+argv[2]+"/"+argv[4]+"/"+"tpcdResult.txt";
        clusterResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+argv[2]+"/"+argv[4]+"/"+"clusterResult"+typeStr+".txt";
        kconvoyResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+argv[2]+"/"+argv[4]+"/"+"kconvoyResult.txt";
        string argvstr="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+argv[2]+"/"+typeStr+"argv.txt";
        char* argvs=(char*)argvstr.data();
        int k=atoi(argv[5].c_str());
        double e=atof(argv[6].c_str());
        int m=atoi(argv[7].c_str());
        
        fstream clInput(const_cast<const char *>(dataDir.c_str()));
        ofstream convoyResult(const_cast<const char *>(convoyResultDir.c_str()));
        ofstream tpcdResult(const_cast<const char *>(tpcdResultDir.c_str()));
        ofstream clusterResult(const_cast<const char *>(clusterResultDir.c_str()));
        ofstream kconvoyResult(const_cast<const char *>(kconvoyResultDir.c_str()));
        ofstream oo;
        oo.open(argvs, ios::app);
        
        oo<<argv[4]<<"|"<<k<<";"<<m<<";"<<e<<";";
        start= clock();
        string oneline;
        //cout<<"data: "<<dataDir<<endl;
        if(!std::strcmp(argv[8].c_str(), "all"))
        {
            int dataSize=atoi(argv[9].c_str());
            trajectoryCluster.queryTrajectoryCluster(k, e, m, clInput, dataSize, convoyResult, tpcdResult, clusterResult, kconvoyResult);
        }
        //trajectory.getTlist(tpcdResult, queryResult, targetGroupID, targetGroup, Tlist, tpcdMap);
        if(!std::strcmp(argv[8].c_str(), "interval"))
        {
            int tstart=atoi(argv[9].c_str()), tend=atoi(argv[10].c_str());
            trajectoryCluster.queryTrajectoryCluster(k, e, m, clInput, convoyResult, tstart, tend);
        }
        finish= clock();
        clInput.close();
        convoyResult.close();
        tpcdResult.close();
        clusterResult.close();
        kconvoyResult.close();
        duration = (double)(finish-start)/CLOCKS_PER_SEC;
        //        cout<<"Running time: "<<duration<<" s"<<endl;
        cout<<"clustering is done"<<endl;
    }
    return duration;
}

int main(int argc, char *argv[])
{
    std::string result = "";
    for (int i = 45; i < 46; i++) {
        std::string filePath = "/Users/dolan/workspace/GitClone/RExDBSCAN/query/whyNotResult" + tech::itos(i) + ".txt";
        std::vector<std::string> queryVector = tech::readFileToVector(filePath.c_str());
        std::vector<std::string>::iterator it = queryVector.begin();
        for (; it != queryVector.end(); it ++) {
            std::vector<std::string> tempVector = tech::split(*it, "|");
            std::vector<std::string> idVector = tech::split(tempVector[0], ";");
            std::vector<std::string> queryVector1 = tech::split(tempVector[1], ";");
            std::vector<std::string> queryVector2;
            queryVector2.push_back("");
            queryVector2.insert(queryVector2.end(), queryVector1.begin(),  queryVector1.end());
            double dr = getConvoyResult(queryVector2, tech::stoi(idVector[0]), idVector[1]);
            std::string typeStr = "";
            if (tech::stoi(idVector[0]) == 0) {
                typeStr = "moe";
            }
            else if (tech::stoi(idVector[0]) == 1) {
                typeStr = "mom";
            }
            else if (tech::stoi(idVector[0]) == 2) {
                typeStr = "mome" + idVector[1];
            }
            else if (tech::stoi(idVector[0]) == 3) {
                typeStr = "mok";
            }
            result += queryVector2[2] + "|" + queryVector2[4] + "|" + typeStr + "|" + tech::dtos(dr) + "\n";
        }
        std::string path = "whyNotResult" + tech::itos(i);
        printf("finish %s \n", path.c_str());
    }
    
    std::ofstream fout("/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/timeResult.txt");
    fout<<result;
//	//time variables
//	clock_t start, finish;
//	double duration=0;
//	//convoy query
//	if(string(argv[1])=="query")
//	{
//		TrajectoryCluster trajectoryCluster;
//		string dataDir, convoyResultDir, tpcdResultDir, clusterResultDir, kconvoyResultDir;
//		dataDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+string(argv[2])+"/"+string(argv[3])+".txt";
//		convoyResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+string(argv[2])+"/"+string(argv[4])+"/"+"convoyResult.txt";
//		tpcdResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+string(argv[2])+"/"+string(argv[4])+"/"+"tpcdResult.txt";
//		clusterResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+string(argv[2])+"/"+string(argv[4])+"/"+"clusterResult.txt";
//		kconvoyResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+string(argv[2])+"/"+string(argv[4])+"/"+"kconvoyResult.txt";
//		string argvstr="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+string(argv[2])+"/"+"argv.txt";
//		char* argvs=(char*)argvstr.data();
//		int k=atoi(argv[5]);
//		double e=atof(argv[6]);
//		int m=atoi(argv[7]);
//		
//		fstream clInput(const_cast<const char *>(dataDir.c_str()));
//		ofstream convoyResult(const_cast<const char *>(convoyResultDir.c_str()));
//		ofstream tpcdResult(const_cast<const char *>(tpcdResultDir.c_str()));
//		ofstream clusterResult(const_cast<const char *>(clusterResultDir.c_str()));
//		ofstream kconvoyResult(const_cast<const char *>(kconvoyResultDir.c_str()));
//		ofstream oo;
//		oo.open(argvs, ios::app);
//		
//		oo<<string(argv[4])<<"|"<<k<<";"<<m<<";"<<e<<";";
//		start= clock();
//		string oneline;
//		//cout<<"data: "<<dataDir<<endl;
//		if(string(argv[8])=="all")
//		{
//			int dataSize=atoi(argv[9]);
//			trajectoryCluster.queryTrajectoryCluster(k, e, m, clInput, dataSize, convoyResult, tpcdResult, clusterResult, kconvoyResult);
//		}
//		//trajectory.getTlist(tpcdResult, queryResult, targetGroupID, targetGroup, Tlist, tpcdMap);
//		if(string(argv[8])=="interval")
//		{
//			int tstart=atoi(argv[9]), tend=atoi(argv[10]);
//			trajectoryCluster.queryTrajectoryCluster(k, e, m, clInput, convoyResult, tstart, tend);
//		}
//		finish= clock();
//		clInput.close();
//		convoyResult.close();
//		tpcdResult.close();
//		clusterResult.close();
//		kconvoyResult.close();
//		duration = (double)(finish-start)/CLOCKS_PER_SEC;
//		cout<<"Running time: "<<duration<<" s"<<endl;
//		cout<<"clustering is done"<<endl;  
//	}	
//  	//why-not
//	if(string(argv[1])=="explain")
//	{
//		AnswerWhyNot answerWhyNot;
//		string convoyResultDir, tpcdResultDir;
//		convoyResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+string(argv[2])+"/"+string(argv[3])+"/"+"convoyResult.txt";
//		tpcdResultDir="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+string(argv[2])+"/"+string(argv[3])+"/"+"tpcdResult.txt";
//		fstream queryResult(const_cast<const char *>(convoyResultDir.c_str()));
//		fstream tpcdResult(const_cast<const char *>(tpcdResultDir.c_str()));
//		cout<<convoyResultDir<<endl;
//		string missAnswer=string(argv[4]);
//		string targetGroupID=string(argv[5]);
//		cout<<"missAnswer "<<missAnswer<<endl;
//		cout<<"targetGroup "<<targetGroupID<<endl;
//		string argvstr="/Users/dolan/workspace/GitClone/RExDBSCAN/datesetreconvey/"+string(argv[2])+"/"+"argv.txt";
//		char* argvs=(char*)argvstr.data();
//		ofstream oo;
//		oo.open(argvs, ios::app);
//		oo<<missAnswer<<"|";
//		start = clock();
//		vector<string> targetGroup;
//		vector<string> Tlist;
//		boost::unordered_map<string, boost::unordered_map<string, vector<string> > > tpcdMap;
//		answerWhyNot.getTlist(tpcdResult, queryResult, targetGroupID, targetGroup, Tlist, tpcdMap);
//		int i=0;		
//		for(i=0; i<Tlist.size()-1; i++)
//		{
//			oo<<Tlist[i]<<";";
//		}
//		oo<<Tlist[i]<<endl;
//		/*
//		int acNum=0;
//		answerWhyNot.modifyTheOrder(Tlist, queryResult, targetGroupID, missAnswer, acNum);
//		int kMax=0;
//		answerWhyNot.modifyParaK(tpcdMap, targetGroup, Tlist, missAnswer, kMax);
//		cout<<"k'max="<<kMax<<endl;
//		double eMin=0;
//		answerWhyNot.modifyParaE(queryResult, targetGroupID, missAnswer, eMin);
//		vector<string> aEM;
//		answerWhyNot.modifyParaEM(queryResult, targetGroupID, missAnswer, aEM);
//		vector<string> aEMS;
//		answerWhyNot.modifyParaEMS(queryResult, targetGroupID, missAnswer, aEMS);
//		finish = clock();*/
//		queryResult.close();
//		tpcdResult.close();
//		duration = (double)(finish-start)/CLOCKS_PER_SEC;
//		cout<<"Running time: "<<duration<<" s"<<endl;
//		cout<<"explain is done"<<endl;  
//	}
	return 0;            //返回
}

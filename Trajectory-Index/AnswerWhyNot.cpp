#include "AnswerWhyNot.h"
#include <fstream>
#include <iosfwd>
#include <math.h>
#include <string>
#include <queue>
#include <boost/unordered_map.hpp> 
#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;
using namespace boost;
using namespace std;
#include <iostream>

//judge if one string is contained in one vector
int containStr(string &strc,vector<string> &strcSet)
{
   	int cid=0;
	//cout<<"strc: "<<strc<<endl;
   	for(int i=0;i<strcSet.size();i++)
   	{
      		if(strcSet[i]==strc)
      		{
         		cid=1;
         		break;
      		}
   	}
   	return cid;
}
//judge if one vector is a subsec of another string
int containVec(string groupStr, vector<string> &targetGroup)
{
  	int res=0;
	vector<string> group;	
	//cout<<groupStr<<endl;
	split(group, groupStr, is_any_of(";"));
	//cout<<"group: "<<group.size()<<endl;
	int existNum=0;
	for(int i=0; i<targetGroup.size(); i++)
	{
		if(containStr(targetGroup[i], group)==1)
		{
			//cout<<"comming"<<endl;
			existNum++;
		}
		else
		{
			break;
		}
	}
	//cout<<existNum<<"---existNum"<<endl;
	if(existNum==targetGroup.size())
	{
		res=1;
	}
	return res;
}

//explain
//global varibale
//get targetGroup and Tlist
int AnswerWhyNot::getTlist(fstream &tpcdResult, fstream &queryResult, string &targetGroupID, vector<string> &targetGroup, vector<string> &Tlist, boost::unordered_map<string, boost::unordered_map<string, vector<string> > > &tpcdMap)
{
	int i=0, j=0;
	//get tpcdMap;
	
	string oneline;
	string tkey, pkey;
	vector<string> tmp;
	vector<string> pcd;
	/*
	while(getline(tpcdResult,oneline))
	{
		split(tmp,oneline,is_any_of("|"));
		//split(resultPoints,tmp[0],is_any_of(";"));
		string tkey=tmp[0];
		boost::unordered_map<string, vector<string> > pcdMap;
		for(i=1; i<tmp.size(); i++)
		{
			split(pcd, tmp[i], is_any_of(";"));
			string pkey=pcd[0];
			vector<string> cd;
			for(j=1; j<pcd.size(); j++)
			{
				cd.push_back(pcd[j]);
			}			
			pcdMap.insert(pair<string, vector<string> > (pkey,cd));
		}
		tpcdMap.insert(pair<string, boost::unordered_map<string, vector<string> > > (tkey, pcdMap));	
	}*/
	
	//get targetGroup and Tlist
	boost::unordered_map<string, string> gtMap;
	boost::unordered_map<string, string>:: iterator git;
	string gkey;
	
	vector<string> tmpp;
	//cout<<"test"<<endl;
	while(getline(queryResult, oneline))
	{
		//cout<<oneline<<endl;
		split(tmp, oneline, is_any_of("|"));
		gkey=tmp[0];
		if(targetGroupID==gkey)
		{
			split(tmpp, tmp[1], is_any_of(";"));
			for(i=0;i<tmpp.size();i++)
			{
				//cout<<"tmpp: "<<tmpp[i]<<endl;
				targetGroup.push_back(tmpp[i]);
			}
			Tlist.push_back(gkey);
			//vector<string> tInterval;
			//split(tInterval, tmp[2], is_any_of(","));
			//Tlist.push_back(tInterval);	
		}else
		{
			gtMap.insert(pair<string, string> (gkey,tmp[1]+"|"+tmp[2]));
		}	
	}
	
	for(git=gtMap.begin(); git!=gtMap.end(); ++git)
	{
		//cout<<git->first<<" | "<<git->second<<endl;
		split(tmpp, git->second, is_any_of("|"));
		//cout<<tmpp[0]<<endl;
		if(containVec(tmpp[0], targetGroup)==1)
		{
			
			//cout<<"est"<<git->first<<endl;
			Tlist.push_back(git->first);
		}		
	}
	cout<<Tlist.size()<<endl;
    cout<<"test"<<endl;
	return 0;
}
//adjusting the order
int AnswerWhyNot::modifyTheOrder(vector<vector<string> >& Tlist, fstream &queryResult, string &targetGroup, string &missAnswer, int &acNum)
{
	queue<string> sWindow;
	sWindow.push("4");
	sWindow.push("1");
        sWindow.push("1");
	sWindow.push("1");
	cout<<sWindow.front()<<" ";
        sWindow.pop();
	sWindow.push("10");
	while(!sWindow.empty())
	{
		cout<<sWindow.front()<<endl;
		sWindow.pop();
	}
	int i=0, j=0;
	for(i=0; i<Tlist.size(); i++)
	{
        for(j=tech::stoi(Tlist[i][0]); j<=tech::stoi(Tlist[i][1]); j++)
		{
			
		}

	}

	return 0;
}

//modifying the parameter k
int AnswerWhyNot::modifyParaK(boost::unordered_map<string, boost::unordered_map<string, vector<string> > > &tpcdMap, vector<string> &targetGroup, vector<vector<string> > &Tlist, string &missAnswer, int &kMax)
{
	kMax=0;
	int i=0,j=0;
	boost::unordered_map<string, boost::unordered_map<string, vector<string> > >::iterator tpcdit;
	boost::unordered_map<string, vector<string> >::iterator pcdit;
	/*for(tpcdit=tpcdMap.begin(); tpcdit!=tpcdMap.end(); ++tpcdit)
	{
		cout<<tpcdit->first<<":"<<endl;
		for(pcdit=tpcdit->second.begin(); pcdit!=tpcdit->second.end(); ++pcdit)
		{
			cout<<pcdit->first<<": ";
			for(i=0; i<pcdit->second.size(); i++)
			{
				cout<<pcdit->second[i]<<",";
			}
			cout<<endl;
		}

	}*/
	int tNum=0;
	string cid1, cid2;	
	for(i=0; i<Tlist.size(); i++)
	{
		int tNum=0;
		for(j=tech::stoi(Tlist[i][0]); j<=tech::stoi(Tlist[i][1]); j++)
		{
            tpcdit=tpcdMap.find(tech::itos(j));
			pcdit=tpcdit->second.find(targetGroup[0]);
			cid1=pcdit->second[0];
			pcdit=tpcdit->second.find(missAnswer);
			cid2=pcdit->second[0];
			if(cid1==cid2)
			{
				tNum++;
			}
			else
			{
				if(tNum>kMax)
				{
					kMax=tNum;
					tNum=0;
				} 
			}
		}
		if(tNum>kMax)
		{
			kMax=tNum;
			tNum=0;
		}  
	}
	return 0;
	
}
//modifying the parameter E
int AnswerWhyNot::modifyParaE(fstream &queryResult, string &targetGroup, string &missAnswer, double &eMin)
{

	return 0;
}
//modifying the parameter M
int AnswerWhyNot::modifyParaM(fstream &queryResult, string &targetGroup, string &missAnswer, int &mMax)
{

	return 0;
}
//modifying the parameters E and M
int AnswerWhyNot::modifyParaEM(fstream &queryResult, string &targetGroup, string &missAnswer, vector<string> &aEM)
{
	

	return 0;
}
//modifying the parameters E and M (improving)
int AnswerWhyNot::modifyParaEMS(fstream &queryResult, string &targetGroup, string &missAnswer, vector<string> &aEMS)
{


	return 0;
}

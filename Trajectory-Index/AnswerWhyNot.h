#ifndef ANSWERWHYNOT_H
#define ANSWERWHYNOT_H

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
class AnswerWhyNot
{
private:



public:
	AnswerWhyNot(){};
	int getTlist(fstream &tpcdResult, fstream &queryResult, string &targetGroupID, vector<string> &targetGroup, vector<string> &Tlist, boost::unordered_map<string, boost::unordered_map<string, vector<string> > > &tpcdMap);
	int modifyTheOrder(vector<vector<string> > &Tlist, fstream &queryResult, string &targetGroup, string &missAnswer, int &acNum);
	int modifyParaK(boost::unordered_map<string, boost::unordered_map<string, vector<string> > > &tpcdMap, vector<string> &targetGroup, vector<vector<string> > &Tlist, string &missAnswer, int &kMax);
	int modifyParaE(fstream &queryResult, string &targetGroup, string &missAnswer, double &eMin);
	int modifyParaM(fstream &queryResult, string &targetGroup, string &missAnswer, int &MMax);
	int modifyParaEM(fstream &queryResult, string &targetGroup, string &missAnswer, vector<string> &aEM);
	int modifyParaEMS(fstream &queryResult, string &targetGroup, string &missAnswer, vector<string> &aEMS);
	~AnswerWhyNot(){};

};







#endif

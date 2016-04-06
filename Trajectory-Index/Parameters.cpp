#include "Parameters.h"
#include <boost/algorithm/string.hpp>

//judge if one int is contained in one vector
bool tech::ifContainInt(long &cid,vector<long> &cidSet)
{
   	bool res=false;
   	for(int i=0;i<cidSet.size();i++)
   	{
      		if(cidSet[i]==cid)
      		{
         		res=true;
         		break;
      		}
   	}
   	return res;
}
//int to string
string tech::itos(int i)
{
    stringstream s;
    s << i;
    return s.str();
}

//float to string

string tech::ftos(float f)
{
    stringstream s;
    s << f;
    return s.str();
}

string tech::dtos(double d)
{
    stringstream s;
    s << d;
    return s.str();
}

//string to double

double tech::stod(string s)
{
    return atof(s.c_str());
}

//string to int
int tech::stoi(string s)
{
    return atoi(s.c_str());
}


std::vector<std::string> tech::split(const  std::string& s, const std::string& delim)
{
    std::vector<std::string> elems;
    boost::split(elems,s,boost::is_any_of(delim.c_str()));
    return elems;
}

std::vector<std::string> tech::readFileToVector(std::string filePath)
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
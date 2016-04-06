#ifndef PARAMETERS_H
#define PARAMETERS_H

#include<vector>
#include<string>
#include<iostream>
#include <fstream>
#include<sstream>
#include <cstdlib>
#include<boost/unordered_map.hpp>  

using namespace std;

namespace tech {
    extern bool ifContainInt(long &cid,vector<long> &cidSet);
    //int to string
    extern string itos(int i);
    //float to string
    extern string ftos(float f);
    
    extern string dtos(double d);
    //string to double
    extern double stod(string s);
    //string to int
    extern int stoi(string s);
    
    extern std::vector<std::string> split(const  std::string& s, const std::string& delim);
    
    extern std::vector<std::string> readFileToVector(std::string filePath);
}
#endif

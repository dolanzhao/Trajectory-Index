///*
// Copyright (C) 2010 by The Regents of the University of California
// 
// Redistribution of this file is permitted under
// the terms of the BSD license.
// 
// Date: 20/06/2011
// Author: Sattam Alsubaiee <salsubai (at) ics.uci.edu>
// */
//
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include "rstartree.h"
//
//
//bool parseLine(string &line, Rectangle &mbr)
//{
//	istringstream coordinatesStream(line);
//	float coord;
//	vector<float> coords;
//	while(coordinatesStream >> coord)
//	{
//		coords.push_back(coord);
//	}
//	if(coords.size() == 2)
//	{
//		mbr.min.x = coords[0];
//		mbr.min.y = coords[1];
//		mbr.max.x = mbr.min.x;
//		mbr.max.y = mbr.min.y;
//	}
//	else 
//	{
//		mbr.min.x = coords[0];
//		mbr.min.y = coords[1];
//		mbr.max.x = coords[2];
//		mbr.max.y = coords[3];
//	}
//	return true;
//}
//
//void printResults(vector<Object> &objects, unsigned queryNumber)
//{
//	cout << "printing the answer for query #" << queryNumber << endl;
//	for(unsigned i = 0; i < objects.size(); ++i)
//	{
//		cout << objects[i].id << endl;
//	}
//}
//
//int main(int argc, char **argv)
//{		
//	string datasetFile = "../data/data.txt";
//	string queryWorkloadFile = "../data/queries.txt";
//		
//	Storage st;
//    RTree rtree(&st);
//	
//	// creating an R*-tree with a branch factor of 32, and a fill factor of 0.7
//    rtree.create(32, 0.7);
//	
//	ifstream data(datasetFile.c_str());
//	if (!data)
//	{
//		cerr << "cannot open dataset file" << endl;
//		return -1;
//	}
//	ifstream queries(queryWorkloadFile.c_str());
//	if (!queries)
//	{
//		cerr << "cannot open query workload file" << endl;
//		return -1;
//	}
//	
//    string line;
//    unsigned count = 0;
//
//	// inserting objects in the R*-tree
//	while (getline(data, line))
//	{	
//		Rectangle mbr;
//		Object obj;
//		if(parseLine(line, mbr))
//		{
//			obj.mbr = mbr;
//			obj.id = count;
//			rtree.insertData(obj);
//			// delete the object
//			// rtree.deleteData(obj);
//		}
//		if ((count % 1000) == 0)
//			cerr << count << endl;
//		++count;
//	}
//
//	cout << "starting the queries" << endl;
//    unsigned numOfQueries = 0;
//
//	while (getline(queries, line))
//	{
//		vector<Object> objects;
//		Rectangle range;
//		parseLine(line, range);
//		     
//		rtree.rangeQuery(objects, range);
//		
//		cout << "number of objects that satisfy the query are: " << objects.size() << endl;
//		++numOfQueries;
//	}
//}
//

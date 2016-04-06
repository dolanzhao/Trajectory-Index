#ifndef RStarTree_H
#define RStarTree_H
/*
 Copyright (C) 2010 by The Regents of the University of California
 
 Redistribution of this file is permitted under
 the terms of the BSD license.
 
 Date: 11/01/2009
 Author: Sattam Alsubaiee <salsubai (at) ics.uci.edu>
*/

#include "Storage.h"
#include "Util.h"
#include <vector>
#include <stack>
#include <map>
#include <boost/tr1/unordered_set.hpp>

typedef unsigned char byte;

const uintptr_t BRANCH_FACTOR = 32;
const double REINSERT_FACTOR = 0.3;
const double SPLIT_FACTOR = 0.4;
const uintptr_t NEAR_MINIMUM_OVERLAP_FACTOR = 32;
    
class Node: public Buffer
{
public:
    // the node's level in the tree, level 0 is the leaf level
    uintptr_t level;
    // number of entries in the node
    uintptr_t numChildren;
    
    Rectangle mbr;
    
    // node's entries
    Object objects[BRANCH_FACTOR];

    // check whether the node is a leaf node or not
    bool isLeaf() const
    {
        return level == 0;
    };
};

class RTree
{
protected:
    Storage *storage;
    size_t nodeSize;
	uintptr_t branchFactor;
    double fillFactor;
    
    // the implementation of the r*-tree insert function
    void insertData(const Object &obj, uintptr_t desiredLevel,
        byte *overflowArray);
        
    // the implementation of the r*-tree delete function
    bool deleteData(const Object &obj, stack<uintptr_t> &path, uintptr_t id);
        
    // r*-tree reinsert function
    void reinsert(Node *node, stack<Node *> path, uintptr_t desiredLevel,
        uintptr_t position, byte *overflowArray);
        
    // r*-tree split function
    void split(Node *node, Object &a, Object &b);
    
    // delete an object from a node
    void deleteObject(Node *node, uintptr_t id);
    
    // condense the tree after a deletetion
    void condenseTree(Node* node, stack<uintptr_t> &path, stack<uintptr_t> &needReinsertion);
    
    // adjust the MBR for a node after r*-tree split
    void adjustNode(Node *node, Object &a, vector <EntryValue> &entries, uintptr_t startIndex, uintptr_t endIndex);
    
    // adjust the MBR for a node after r*-tree split
    void adjustNode(Node *node);
        
    // the implementation of range query function for both r-tree and r*-tree 
    void rangeQuery(vector<Object> &objects, const Rectangle &range, 
        uintptr_t id);

    bool retrieve(unordered_set<uintptr_t> &ids, uintptr_t oid, uintptr_t id);
public:
    uintptr_t nodeNum;
    RTree(){};
    
    RTree(Storage *storage);

    // create r*-tree
    void create(uintptr_t bf, double ff);
    
    // r*-tree insert function
    void insertData(const Object &obj);
    
    // r*-tree delete function
    void deleteData(const Object &obj);
    
    // range query function for both r-tree and r*-tree
    void rangeQuery(vector<Object> &objects, const Rectangle &range);
    
    // top k nearest neighbour function for both r-tree and r*-tree
    void kNNQuery(multimap<double, Object> &objects, 
        const Point &point, uintptr_t k);
        
    void retrieve(unordered_set<uintptr_t> &ids, uintptr_t oid);
    //empty r*tree
    void cleanRTree(const RTree &rtree);
    ~RTree(){};
};



#endif


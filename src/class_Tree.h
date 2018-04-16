#ifndef DEF_TREE
#define DEF_TREE

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>    //for hash function

//class
#include "class_Node.h"



class Tree
{
    public:

    //Builders & Destroyers
    Tree(int f);
    ~Tree();

    //Attributes
    std::vector<Node> N;                            //Search tree (Dynamic Array of nodes)
    std::unordered_map<std::string, int> N_address; //hash function to know the address of corresponding node

    //Methods

};




#endif

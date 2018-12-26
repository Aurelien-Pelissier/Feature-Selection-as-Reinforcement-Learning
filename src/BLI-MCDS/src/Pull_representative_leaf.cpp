#include <iostream>         //cin & cout
#include <algorithm>        //min, max, fill, find...
#include <fstream>          //files write and read
#include <string>           //string
#include <vector>           //vector
#include <iomanip>          //set precision & set width
#include <bitset>           //for binary numbers manipulation
#include <unordered_map>    //for hash function
#include <chrono>           //to record simulation time
#include <windows.h>        //get key
#include <map>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>  //for binary vectors (couldn't use std::bitset because the size had to be defined at compilation time)

//including functions
#include "functions.h"
#include "print.h"

//including classes
#include "class_Node.h"        //Contains all the parameters of a node
#include "class_Tree.h"        //Pair of a Node vector and an associated hash function to quickly find nodes address - in O(1)


double  Pull_representative_leaf(Tree &T, Node &node, int &bt)
// Return the evaluation of the representative leaf of arm fi from root node
{

    int fi, node_address, child_node_address;

    node_address = node.address_f[bt];
    T.N[node_address].Tt_F ++;

    while ( (!T.N[node_address].is_leaf)&&(!T.N[node_address].allowed_features.empty())  )
    {
        fi = T.N[node_address].repres_child;
        child_node_address = T.N[node_address].address_f[fi];
        if (child_node_address < 0) {cout << endl << "   Error, child address is not defined for fi = " << fi << " at address "; print_features(T.N[node_address].sub_F); cout << endl; exit(1);}
        node_address = child_node_address;
        T.N[node_address].Tt_F ++;
    }


    return node_address;  //at the end of the while loop, node_address is the address of the representative leaf

}

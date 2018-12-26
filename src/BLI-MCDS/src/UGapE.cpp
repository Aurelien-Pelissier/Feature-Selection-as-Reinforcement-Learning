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
#include "class_Params.h"      //Contains all the important simulation parameters
#include "class_Variables.h"   //Contain the global variable of simulations (stopping condition)
#include "class_Node.h"        //Contains all the parameters of a node
#include "class_Tree.h"        //Pair of a Node vector and an associated hash function to quickly find nodes address - in O(1)

int UGapE(Tree &T, Node &node, const Params &params, Variables &variables)
// Return the arm corresponding to the UGapE policy from root node

{

    int bt, ct;


    //check if at least 2 children has a different representative leaf
    bool check_rep = false;
    int fi = *node.allowed_features.begin();

    for (const auto &fii : node.allowed_features)  //fii represent s'
    {
        if (T.N[node.address_f[fii]].repres_leaf_address != T.N[node.address_f[fi]].repres_leaf_address)
        {
            check_rep = true;
            break;
        }
    }

    if (check_rep == false)
    {
        print_Node(T,node);
        cout << endl << "  All arms has the same representative leaf, this should not happen in this experiment" << endl;
        variables.STOP = true;
        bt = *node.allowed_features.begin();
        variables.best_child = bt;
        T.N[node.address_f[bt]].is_root = true;
        exit(1);
        return 0; //return the first arm as the best arm
    }



    //most promissing child (bt) (original UGapE)

    /*
    map <double,int> child_Bs;
    map <double,int> child_LU;
    for (const auto &fi : node.allowed_features) //fi represent s
    {
        for (const auto &fii : node.allowed_features)  //fii represent s'
        {
            if (fii != fi)
            {
                if (T.N[node.address_f[fii]].repres_leaf_address != T.N[node.address_f[fi]].repres_leaf_address)
                {
                    child_LU.insert({T.N[node.address_f[fii]].U_F - T.N[node.address_f[fi]].L_F,fii});
                }
            }
        }

        child_Bs.insert({child_LU.rbegin()->first,fi});
        child_LU.clear();

    }
    bt = child_Bs.begin()->second;  //argmin
    */


    //most promissing child (bt) (updated UGapE

    map <double,int> child_L;
    for (const auto &fi : node.allowed_features)
    {
        child_L.insert({T.N[node.address_f[fi]].L_F,fi});
    }

    bt = child_L.rbegin()->second; //argmax





    //second most promising child (ct)
    map <double,int> child_U;
    for (const auto &fi : node.allowed_features)
    {
        if (fi != bt)
        {
            if  (T.N[node.address_f[fi]].repres_leaf_address != T.N[node.address_f[bt]].repres_leaf_address)
            {
                child_U.insert({T.N[node.address_f[fi]].U_F,fi});
            }
        }
    }

    ct = child_U.rbegin()->second; //argmax


    if (T.N[node.address_f[ct]].U_F - T.N[node.address_f[bt]].L_F <= params.eps) //if stopping condition fulfilled
    {
        cout << endl << "  Best arm found !" << endl;
        variables.STOP = true;

        variables.best_child = bt;
        if (T.N[node.address_f[bt]].L_F - T.N[node.address_f[params.f]].L_F <= params.eps) //if stopping node also fulfill the condition, we chose it as a priority to avoid over fitting
        {
            variables.best_child = params.f;
        }

        T.N[node.address_f[bt]].is_root = true;
    }


    //return the one with the one with largest confidence interval
    map <double,int> child_I;
    child_I.insert({T.N[node.address_f[bt]].U_F - T.N[node.address_f[bt]].L_F, bt});
    child_I.insert({T.N[node.address_f[ct]].U_F - T.N[node.address_f[ct]].L_F, ct});

    return child_I.rbegin()->second;

}

#include <iostream>         //cin & cout
#include <algorithm>        //min, max, fill, find...
#include <fstream>          //files write and read
#include <string>           //string
#include <vector>           //vector
#include <iomanip>          //set precision & set width
#include <bitset>           //for binary numbers manipulation
#include <unordered_map>    //for hash function
#include <unordered_set>
#include <chrono>           //to record simulation time
#include <windows.h>        //get key
#include <map>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>  //for binary vectors (couldn't use std::bitset because the size had to be defined at compilation time)
#include <boost/math/special_functions/binomial.hpp>

//including functions
#include "functions.h"
#include "print.h"

//including classes
#include "class_Params.h"   //Contains all the important simulation parameters
#include "class_Node.h"     //Contains all the parameters of a node
#include "class_Tree.h"     //Pair of a Node vector and an associated hash function to quickly find nodes address - in O(1)



void   Build_initial_graph(Tree &T, vector<pair<double,int>> &gRAVE, const Params &params, Variables &variables)
//prepare the initial tree for MCTS
{

    //Computing gRAVE for initial tree
    cout << "     Computing initial gRAVE (" << params.f*20 << " evaluations) ..." << endl;
    double V;
    boost::dynamic_bitset<> F(params.f+1); //initialize feature subset to empty (filled with zeros) + stopping feature
    for (int it=0; it<params.f*20; it++)
    {
        V = Random_phase(F,params);
        Update_gRAVE(gRAVE, F, V);
        F.reset();
    }

    vector<int> BestRAVE;
    map <double, int>  RAVE;
    double RAVE_Score;
    for (int fi=0; fi<params.f; fi++)
    {
        RAVE.insert({gRAVE[fi].first, fi});
    }

    for (int i=0; i<params.branch_i; i++)
    {
        BestRAVE.push_back(RAVE.rbegin()->second);
        RAVE.erase(RAVE.rbegin()->first);
    }
    BestRAVE.push_back(params.f); //always put stopping feature in BestRAVE



    //Defining the initial tree
    cout << "     Adding initial nodes ..." << endl;

    F.reset();
    T.N[0].is_root = true;

    Add_children_DAG(T, F, -1, -1, params, variables, BestRAVE, 0);

    cout << "     " << T.N.size() << " Nodes created " << endl;
    cout << "     with " << variables.n_leaf_temp << " (temp) & " << variables.n_leaf_term << " (term) leafs." << endl << endl;

}



void   Complete_current_graph(Tree &T, vector<pair<double,int>> &gRAVE, const Params &params, Variables &variables, const int &root_address)
//prepare the initial tree for MCTS
{


    //Computing gRAVE for initial tree
    cout << "     Computing more lRAVE (" << params.f*20 << " evaluations) ..." << endl;
    double V;
    boost::dynamic_bitset<> F = T.N[root_address].sub_F;
    for (int it=0; it<params.f*20; it++)
    {
        V = Random_phase(F,params);
        Update_lRAVE(T.N[root_address], F, V);
        F = T.N[root_address].sub_F;
    }

    vector<int> BestRAVE;
    map <double, int>  RAVE;
    double RAVE_Score;
    for (int fi=0; fi<params.f; fi++)
    {
        if (!F[fi])
        {
            RAVE.insert({T.N[root_address].lRAVE_f[fi].first, fi});
        }
    }


    for (int i=0; i<params.branch_i; i++)
    {
        if (RAVE.empty()) break;
        BestRAVE.push_back(RAVE.rbegin()->second);
        RAVE.erase(RAVE.rbegin()->first);
    }
    BestRAVE.push_back(params.f); //always put stopping feature in BestRAVE



    //Defining the initial tree
    cout << "     Completing the current graph ..." << endl;


    Add_children_DAG(T, F, -1, -1, params, variables, BestRAVE, root_address);


}





void    Add_children_DAG(Tree &T, boost::dynamic_bitset<> &F, int parent_node_address, int fc, const Params &params, Variables &variables, const vector<int> &BestRAVE, const int &root_address)
//link the parent and children with the lattice pc policy
//fc is the corresponding feature between parent and child
{

    //cout << F.count() << endl;

    int node_address = Add_Node_And_Get_Adress(T, F, params);

    if (!T.N[node_address].is_root) //the root node has no parent and no fc
    {
        T.N[node_address].parent_address.insert(parent_node_address);
        T.N[parent_node_address].allowed_features.insert(fc);
        T.N[parent_node_address].address_f[fc] = node_address;
    }

    if (F[params.f] == 1) //if we reach stopping feature
    {
        if (!T.N[node_address].is_leaf) variables.n_leaf_term++;
        T.N[node_address].is_leaf = true;
        return;
    }

    if (F.count() == params.depth_i + T.N[root_address].F_size) //if we reach maximum depth
    {

        if (T.N[node_address].was_leaf) return;

        //add extra children (best RAVE)
        int fbest = BestRAVE[0];

        for (int i=0; i<BestRAVE.size(); i++)
        {
            if (!F[BestRAVE[i]])
            {
                fbest = BestRAVE[i];
                break;
            }
        }

        if (!F[fbest])
        {
            F[fbest] = 1;
            Add_children_DAG(T, F, node_address, fbest, params, variables, BestRAVE, root_address);
            F[fbest] = 0;
        }

        //add extra children (stopping feature)
        F[params.f] = 1;
        Add_children_DAG(T, F, node_address, params.f, params, variables, BestRAVE, root_address);
        F[params.f] = 0;

        return;
    }

    if (F.count() > params.depth_i + T.N[root_address].F_size) //if are higher than the maximum depth
    {

        if (!T.N[node_address].is_leaf) variables.n_leaf_temp++;
        T.N[node_address].is_leaf = true;
        return;
    }


    //add children with the best RAVE score


    bool check_leaf = true;
    int fi;
    for (int i=0; i<BestRAVE.size(); i++)
    {
        fi = BestRAVE[i];
        if (!F[fi])
        {
            F[fi] = 1;
            Add_children_DAG(T, F, node_address, fi, params, variables, BestRAVE, root_address);
            F[fi] = 0;

            check_leaf = false;
        }
    }

    if (check_leaf)
    {
        if (!T.N[node_address].is_leaf) variables.n_leaf_temp++;
        T.N[node_address].is_leaf = true;
    }
}


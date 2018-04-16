///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////                            Feature Selection as a One-Player Game                         //////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    This code is an implementation of the algorithm described in (https://hal.inria.fr/inria-00484049/document)    //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The feature selection algorithm is run on the training set "dataset.dat.                                          //
// The reading function can be modified in dataset.cpp                                                               //
// The algorithm is based on bandit reinforcement learning, and is an improved version of UCT                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Created : 2018-03-12                                                                                              //
// Updated : 2018-05-01                                                                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include <iostream>         //cin & cout
#include <algorithm>        //min, max, fill, find...
#include <fstream>          //files write and read
#include <string>           //string
#include <vector>           //vector
#include <iomanip>          //set precision & set width
#include <bitset>           //for binary numbers manipulation
#include <unordered_map>    //for hash function
#include <chrono>           //to record simulation time
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>  //for binary vectors (couldn't use std::bitset because the size had to be defined at compilation time)

//including functions
#include "mainf.h"
#include "dataset.h"
#include "update.h"
#include "print.h"

//including classes
#include "class_Params.h"   //Contains all the important simulation parameters
#include "class_Node.h"     //Contains all the parameters of a node
#include "class_Tree.h"     //Pair of a Node vector and an associated hash function to quickly find nodes address - in O(1)



void Run_feature_selection(Params &params)
{
    if (params.Nt<1000) {cout << endl << "   Error: Number if iteration should be at least 1000" << endl << endl; exit(1);} //or the output files will be messed up
    cout << fixed << setprecision(4);

    //variables
    int f = params.L[0].size() - 1;                //Number of features
    boost::dynamic_bitset<> F(f+1);                //F[f+1] Feature subset with F[fi]=0 if feature not used, + the stopping feature
    Tree T(f+1);                                   //Tree T is a Dynamic array of node + with a hash function to find its element faster, f+1 for the stopping feature
    vector < pair <double, int> > gRAVE(f*2+2);    //Global RAVE score, pair of <RAVE_score, Nb of calc involved>, all are initialized to zero.
                                                   //(the first f index are for each features, and the f+1 remaining are for the stopping feature at each feature subset size)

    //output variables
    double V = 0;
    int depth =0 ;
    vector <double> reward_V(1,V);                 //Reward vector
    vector <int> Depth(1,depth);                   //Depth of exploration vector
    ofstream output_file_1 ("Output_Tree.txt");    //Output file for the tree
    ofstream output_file_2 ("Output_Reward.txt");  //Output file for the reward
    ofstream output_file_3 ("Results.txt", ios_base::app); //file which append simulation result after each simulations







    cout << "    n = " << params.L.size() << " examples" << endl;
    cout << "    f = " << params.L[0].size()-1 << " features" << endl << endl << endl << endl;
    cout << "  Starting the computation ..." << endl;
    cout << "    Estimated time is " << setprecision(0) << (double)params.L.size()*(double)params.L.size()*(double)params.L[0].size()/(double)params.r*(double)params.Nt / 334000000000.0 * 400.0 << "s " << endl;










    auto t_start = std::chrono::high_resolution_clock::now(); //record the starting time

//======================================================= Running the simulations =======================================================//

    for (int it =0; it<params.Nt; it++)
    {
        if (it%100 == 0) cout << endl << "it = " << it << "/" << params.Nt << endl;

        if (it == 31)
        {
            cout << "debug" << endl;
        }

        //reinitialize the feature subset to empty and clear the path;
        F = F.reset();    //start from empty feature subset after each iteration
        depth = 0;
        for (int i=0;i<T.N.size();i++) {T.N[i].already_updated = 0;} //reset the values to zero for next update

        //Explore the tree (until it chose final feature
        V = iterate(T, gRAVE, F, params, depth);
        reward_V.push_back(V);
        Depth.push_back(depth);
    }


//============================================= Terminate the simulation and print results =============================================//

    auto t_finish = std::chrono::high_resolution_clock::now(); //record the finishing time
    std::chrono::duration<double> t_elapsed = t_finish - t_start; //get the elapsed time

    print_results(T, gRAVE, reward_V, Depth, output_file_1, output_file_2, output_file_3, params, t_elapsed);

}

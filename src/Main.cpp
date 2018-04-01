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
// Created : 2018-03-26                                                                                              //
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
using namespace std;

//including functions
#include "mainf.h"
#include "dataset.h"
#include "update.h"
#include "print.h"

//including classes
#include "class_Params.h"   //Contains all the important simulation parameters
#include "class_Node.h"     //Contains all the parameters of a node
#include "class_Tree.h"     //Pair of a Node vector and an associated hash function to quickly find nodes address - in O(1)



int main()
{
    //Important simulations parameter:
    int N_it = 100000;                                //Number of iterations of the simulation
    bool MA = 0;                                   //Many-Armed behavior, put 0 for Discrete and 1 for Continuous
    double q = 0.98;                                //Random expansion parameter, used to control the average depth in the random phase, |q|<1, high q -> deep exploration
    int k = 5;                                     //Number of nearest neighbors involved in the reward function calculation
    double ce = 2;                                  //UCB exploration control parameter (used in both discrete and continuous heuristic)
    double c = 100;                                 //Continuous heuristic exploration parameter 1
    double cl = 100;                                //Continuous heuristic exploration parameter 2
    double b = 0.8;                                 //Discrete heuristic exploration parameter, |b|<1
    string myfile = "dataset.dat";                 //Training set file
    ofstream output_file_1 ("Output_Tree.txt");    //Output file for the tree
    ofstream output_file_2 ("Output_Reward.txt");  //Output file for the reward

    vector < vector<float> > L = read_dataset(myfile);  //training set matrix L[n][f+1] read from set from the file
    Params params = Params(MA,q,k,ce,c,cl,b,L);


    //variables
    int f = L[0].size() - 1;                     //Number of features
    double V;
    vector <double> reward_V(1,V);               //Reward vector
    vector<bool> F(f+1,0);                       //F[f+1] Feature subset with F[fi]=0 if feature not used, + the stopping feature
    Tree T(f+1);                                 //Tree T is a Dynamic array of node + with a hash function to find its element faster, f+1 for the stopping feature
    vector < pair <double, int> > gRAVE(f*2+1);  //Global RAVE score, pair of <RAVE_score, Nb of calc involved>, all are initialized to zero.
                                                 //(the first f index are for each features, and the f+1 remaining are for the stopping feature at each feature subset size)

    for (int it =0; it<N_it; it++)
    {
        if (it%100 == 0) cout << endl << "it = " << it << endl;

        if (it == 1691)
        {
            cout << "debug" << endl;
        }

        //reinitialize the feature subset to empty and clear the path;
        fill(F.begin(), F.end(), 0);    //start from empty feature subset after each iteration

        //Explore the tree (until it chose final feature
        V = iterate(T, gRAVE, F, params);
        reward_V.push_back(V);
    }


    print_results(T, gRAVE, reward_V, output_file_1, output_file_2);
    return 0;
}

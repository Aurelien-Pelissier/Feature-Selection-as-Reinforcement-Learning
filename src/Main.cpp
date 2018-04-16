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


#include <string>
#include <vector>
using namespace std;

//including functions
#include "mainf.h"
#include "dataset.h"

//including classes
#include "class_Params.h"   //Contains all the important simulation parameters



int main()
{
    //just checking if 32bit or 64bit
    //vector<float> vector_float;
    //cout << endl << "   max_size_float  = " << vector_float.max_size() << " = 2^" << log(vector_float.max_size())/log(2) << endl << endl << endl;
    //cin.get();

//=====================================================================================================================================//
//================================================== Important simulation parameters ==================================================//
//=====================================================================================================================================//

    int Nt = 300000;      //Number of iterations of the simulation
    bool MA = 0;         //Many-Armed behavior, put 0 for Discrete and 1 for Continuous
    double q = 0.98;     //Random expansion parameter, used to control the average depth in the random phase, |q|<1, high q -> deep exploration
    int k = 5;           //Number of nearest neighbors involved in the reward function calculation
    int r = 12;          //Ratio of aggressive subsample size / Training set size
    double ce = 2;       //UCB exploration control parameter (used in both discrete and continuous heuristic)
    double c = 20;       //Continuous heuristic exploration parameter (RAVE score weight)
    double cl = 1000;     //l-RAVE/g-RAVE weight
    double b = 0.2;      //Discrete heuristic exploration parameter, |b|<1




//============================================================= Training set ==========================================================//

    cout << endl << "   Reading the dataset ..." << endl << endl;

    string data = "Madelon.data";            //Training set file
    string labels = "Madelon.labels";        //Training set file
    //vector < vector<float> > L = concatenate(read_matrix(data), read_matrix(labels));  //training set matrix L[n][f+1] read from set from the file

    vector < vector<float> > L = linear_dataset(400,15); //it generates a training set automatically with a hypothtical linear function to check if the algorithm can solve easy problems
    //print_dataset(L);





//=====================================================================================================================================//
//=================================================t====== Running the simulation ======================================================//
//=====================================================================================================================================//

    Params params = Params(Nt,MA,q,k,r,ce,c,cl,b,L);
    Run_feature_selection(params);
    return 0;
}

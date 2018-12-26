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
// Last Updated : 2018-10-14                                                                                         //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

//including functions
#include "mainf.h"
#include "dataset.h"
#include "select_features.h"

//including classes
#include "class_Params.h"   //Contains all the important simulation parameters



int main()
{

        cout << endl;
        cout << " |-----------------------------------------------------------------------------------------------|" << endl;
        cout << " |---------------------------------- FEATURE SELECTION - FUSE -----------------------------------|" << endl;
        cout << " |-----------------------------------------------------------------------------------------------|" << endl << endl;




//=====================================================================================================================================//
//================================================== Important simulation parameters ==================================================//
//=====================================================================================================================================//

    int Nt = 20000;        //Number of iterations of the simulation
    bool MA = 0;             //Many-Armed behavior, put 0 for Discrete and 1 for Continuous
    double q = 0.9;         //Random expansion parameter, used to control the average depth in the random phase, |q|<1, high q -> deep exploration
    int k = 5;               //Number of nearest neighbors involved in the reward function calculation
    int m = 50;              //Size of aggressive subsample
        double r2 = 1;           //Ratio of reward training set size / Training set size (not used anymore)
    double ce = 2;           //UCB exploration control parameter (used in both discrete and continuous heuristic)
    double c = 20;           //Continuous heuristic exploration parameter (RAVE score weight)
    double cl = 200;         //l-RAVE/g-RAVE weight
    double b = 0.26;         //Discrete heuristic exploration parameter, |b|<1

    int rseed = 72;          //seed of the random generator

    bool Reward_policy = 1;  //0 for Accuracy (ACC) and 1 for Area Under Curve (AUC)
    bool KNN_policy = 0;     //0 for naive kNN search, 1 for KD-Tree kNN search


    bool with_data = 0;       //1 for reading the training set, 0 for tests with theoretical training set
    bool sub = 0;             //True if we want to reduce the training set size
         int n_sub = 500;     //desired size of the new sub training set
    bool pFS = 0;             //True if we want to perform a previous feature selection
        unordered_set<int> Features = {};
                                      //Pre-selection of features because FUSE calculation may become too heavy when there is too many features
                                      //for example, This feature subset contain the best 150 ranked after their Relief score






//============================================================= Training set ==========================================================//

    cout << endl << "  Reading the dataset ..." << endl << endl;



     vector<vector<float>> L;

    if (with_data)
    {
        string data = "Madelon.data";            //Training set file
        string labels = "Madelon.labels";        //Training set file
        L = concatenate(read_matrix(data), read_matrix(labels));  //training set matrix L[n][f+1] read from set from the file
    }

    else
    {
        L = linear_dataset(300, 30, 10);
    }



//=====================================================================================================================================//
//======================================================= Running the simulation ======================================================//
//=====================================================================================================================================//

    Params params = Params(pFS,KNN_policy,Reward_policy,Nt,MA,q,k,m,r2,ce,c,cl,b,L,Features,rseed);
    Run_feature_selection(params);
    return 0;
}

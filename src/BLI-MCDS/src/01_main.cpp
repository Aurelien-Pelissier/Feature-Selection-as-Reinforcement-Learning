///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////                            BLI - MCDS (Monte Carlo Lattice Search)                        //////////////
//////////////                 (delta - epsilon) PAC BAI problem in fixed confidence setting             //////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Last Update : 2018-12-26                                                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

//including functions
#include "functions.h"
#include "print.h"
#include "realdata/dataset.h"

//including classes
#include "class_Params.h"   //Contains all the important simulation parameters

int main()
{

    cout << endl;
    cout << " |-----------------------------------------------------------------------------------------------|" << endl;
    cout << " |---------------------------------- Feature_Selection_BLI-MCDS ---------------------------------|" << endl;
    cout << " |-----------------------------------------------------------------------------------------------|" << endl << endl;




//=====================================================================================================================================//
//================================================== Important simulation parameters ==================================================//
//=====================================================================================================================================//

    //main simulation parameter
    double eps = 0.005;          //accuracy parameter
    double delta = 0.1;          //risk parameter
    double b = 0.3;              //expansion parameter 0 < b < 1
    bool interval_policy = 1;    //0 for theoretical, 1 for empirical
    int branch_i = 7;            //initial branching factor of the lattice
    int depth_i = 2;             //initial depth of the lattice

    int rseed = 41;              //seed for the random generator of intermediate oracle


    //real data parameter
    double q = 0.9;          //Random expansion parameter, used to control the average depth in the random phase, |q|<1, high q -> deep exploration
    int k = 5;               //Number of nearest neighbors involved in the reward function calculation
    int m = 50;              //Size of aggressive subsample
    double cl = 200;         //l-RAVE/g-RAVE weight
    bool with_data = 0;      //1 for reading the training set, 0 for tests with artificial linear training set
    bool Reward_policy = 1;  //0 for Accuracy (ACC) and 1 for Area Under Curve (AUC)


    //prepare the dataset
    vector<vector<float>> L;
    if (with_data)
    {
        string data = "realdata/Madelon.data";            //Training set file
        string labels = "realdata/Madelon.labels";        //Training set file
        L = concatenate(read_matrix(data), read_matrix(labels));  //training set matrix L[n][f+1] read from set from the file
    }
    else  L = linear_dataset(300, 30, 10);



    Params params = Params(eps,delta,b,interval_policy ,branch_i,depth_i, rseed, q, k, m, cl, L, Reward_policy);
    Run_BLI_MCLS(params);
    return 0;

}

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>   //pair
#include <map>
#include <cmath>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>

//include classes
#include "class_Node.h"
#include "class_Params.h"
#include "class_Tree.h"

/* Class Node:
     vector<bool> sub_F;                   //Features subset
     int F_size;                           //Number of features in the subset
     int T_F;                              //Nb of time the node has been visited
     unordered_set<int> allowed_features;  //Allowed features for Discrete heuristic
     vector<int> t_f;                      //Number of time feature has been selected
     vector<double> mu_f;                  //Average reward for each feature
     vector<double> sg_f;                  //Variance of each feature
     vector<pair<double, int>> lRAVE_f;    //Local RAVE score of each feature
*/




//================================================================================================================================================================//
//====================================================================== Discrete Heuristic ======================================================================//
//================================================================================================================================================================//

int Discrete_UCB(Node &node, const vector <pair<double,int>> &gRAVE, const Tree &T, const Params &params)
// Return the feature corresponding to the highest UCB score
{
    int d = node.F_size;  //feature subset size
    int f = node.sub_F.size()-1; //number of features
    int nrand = 100;
    double T_Fr = (double)node.T_F - (double)nrand;
    if (T_Fr <= 0) T_Fr = 1.0;



    //----------------------------------------------------------------------------------------------------------------------------------------------------------//
    //------------------------------------------- Exploring a new child node whenever floor(T_F^b) is incremented ----------------------------------------------//
    //----------------------------------------------------------------------------------------------------------------------------------------------------------//







   if (node.T_F < nrand) // we perform random exploration fort the first 50 visits
    {
        return -1; //-1 indicate that we don't to want chose any feature
    }







    //if ( ( ( (floor(pow((double)node.T_F+(double)1,params.b)) - floor(pow((double)node.T_F,params.b))) != 0) && (node.allowed_features.size() < (f-d+1)) )  )
    if ( ( ( (floor(pow(T_Fr+1.0,params.b)) - floor(pow(T_Fr,params.b))) != 0) && (node.allowed_features.size() < (f-d+1)) )  )
    {    // if  (                   floor(T_F^b) incremented                       &&                  allowed feature subset not already full  )


        if (node.allowed_features.find(f) == node.allowed_features.end()) //allowed feature set is empty
        {
            node.allowed_features.insert(f);
            return f;
        }


        // Adding the child node with the highest RAVE score to allowed features
        double beta;

        map <double, int>  RAVE;  //map is a SORTED array of element, so the maximum value is found in O(1), storing the RAVE score with its corresponding feature
        double RAVE_Score;
        for (int fi=0; fi<f; fi++)
        {
            if ( (!node.sub_F[fi]) && (node.allowed_features.find(fi) == node.allowed_features.end())  )
            //    if feature is not already selected         &&       fi is not already in allowed features
            {
                beta = params.cl/(params.cl + (double)node.lRAVE_f[fi].second);
                RAVE_Score = (1-beta)*node.lRAVE_f[fi].first + beta*gRAVE[fi].first;
                RAVE.insert({RAVE_Score, fi});
            }
        }
        node.allowed_features.insert(RAVE.rbegin()->second);  //adding (to allowed feature set) the feature corresponding to the maximum calculated RAVE score
                                                                 // (it is the one at the end of the map)
    }


    else //if [TF^b] not incremented, and if stopping feature not added, we force the prgogram to add it
    {
        if (node.allowed_features.find(f) == node.allowed_features.end()) //allowed feature set is empty
        {
            node.allowed_features.insert(f);
            return f;
        }
    }






    //------------------------------------------------------------------------------------------------------------------------------------------------//
    //------------------------------------------------- Computing the UCB scores of allowed features -------------------------------------------------//
    //------------------------------------------------------------------------------------------------------------------------------------------------//


    map <double, int>  UCB; //map is a SORTED array of element, so the maximum value is found in O(1), storing the score with its corresponding feature
    double UCB_Score;
    for ( auto &fi : node.allowed_features ) //computing UCB for each feature
    {
        UCB_Score = node.mu_f[fi] + sqrt(    params.ce*log((double)node.T_F)/(double)node.t_f[fi]  *  min( (double)0.25 ,  (double)pow(node.sg_f[fi],2) + sqrt(2*log((double)node.T_F)/(double)node.t_f[fi]) )    );
        UCB.insert({UCB_Score, fi});
    }

    return UCB.rbegin()->second; //return the feature corresponding to the maximum calculated score, (it is the end of the map)
}


















//================================================================================================================================================================//
//===================================================================== Continuous Heuristic =====================================================================//
//================================================================================================================================================================//

int Continuous_UCB(Node &node, const vector <pair<double,int>> &gRAVE, const Tree &T, const Params &params)
// Return the feature corresponding to the highest UCB score
{
}

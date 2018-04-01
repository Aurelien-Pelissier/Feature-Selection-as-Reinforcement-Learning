#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>   //pair
#include <map>
using namespace std;

#include "class_Node.h"
#include "class_Params.h"

/* Class Node:
     vector<bool> sub_F;                   //Features subset
     int F_size;                           //Number of features in the subset
     int T_F;                              //Nb of time the node has been visited
     unordered_set<int> allowed_features;  //Allowed features for Discrete heuristic
     vector<int> t_f;                      //Number of time feature has been selected
     vector<double> mu_f;                   //Average reward for each feature
     vector<double> sg_f;                   //Variance of each feature
     vector<pair<double, int>> lRAVE_f;     //Local RAVE score of each feature
*/


bool choose_first(const pair<double, int> &lhs, //function for comparing the first member of a pair
                   const pair<double, int> &rhs) {
    return lhs.first < rhs.first ;
}


int Discrete_UCB(Node &node, const vector <pair<double,int>> &gRAVE, const Params &params)
// Return the feature corresponding to the highest UCB score
{
    int d = node.F_size;  //feature subset size
    int f = node.sub_F.size()-1; //number of features

    //"exploring a new child node whenever floor(T_F^b) is incremented"
    if (   ( (floor(pow((double)node.T_F+(double)1,params.b)) - floor(pow((double)node.T_F,params.b))) != 0) && (node.allowed_features.size() < (f-d+1))   )
    {               //if floor(T_F^b) incremented                       &&                  allowed feature subset not already full
        // Adding the child node with the highest RAVE score to allowed features
        double beta;
        double gRAVE_f;


        map <double, int>  RAVE;  //map is a SORTED array of element, so the maximum value is found in O(1), storing the RAVE score with its corresponding feature
        double RAVE_Score;
        for (int fi=0; fi<f+1; fi++)
        {
            if ( (!node.sub_F[fi]) && (node.allowed_features.find(fi) == node.allowed_features.end())  )
            //    if feature is not already selected         &&       fi is not already in allowed features
            {
                if (fi == f) {gRAVE_f = gRAVE[f+d].first;} //stopping feature
                else {gRAVE_f = gRAVE[fi].first;}
                beta = params.cl/(params.cl + (double)node.lRAVE_f[fi].second);

                RAVE_Score = (1-beta)*node.lRAVE_f[fi].first + beta*gRAVE_f;
                RAVE.insert({RAVE_Score, fi});
            }
        }
        node.allowed_features.insert(RAVE.rbegin()->second);  //adding (to allowed feature set) the feature corresponding to the maximum calculated RAVE score
                                                              // (it is the one at the end of the map)
    }



    // Computing the UCB score of allowed features
    map <double, int>  UCB; //map is a SORTED array of element, so the maximum value is found in O(1), storing the score with its corresponding feature
    double UCB_Score;
    for ( auto &fi : node.allowed_features ) //computing UCB for each feature
    {
        UCB_Score = node.mu_f[fi] + sqrt(    params.ce*log((double)node.T_F)/(double)node.t_f[fi]  *  min( (double)0.25 ,  (double)pow(node.sg_f[fi],2) + sqrt(2*log((double)node.T_F)/(double)node.t_f[fi]) )    );
        UCB.insert({UCB_Score, fi});
    }

    return UCB.rbegin()->second; //return the feature corresponding to the maximum calculated score, (it is the end of the map)

}






int Continuous_UCB(Node &node, const vector <pair<double,int>> &gRAVE, const Params &params)
// Return the feature corresponding to the highest UCB score
{
    int d = node.F_size;  //feature subset size
    int f = node.sub_F.size()-1; //number of features

    //defining useful parameters
    double beta;
    double alpha;
    double gRAVE_f;

    // Computing the UCB score of allowed features
    map <double, int>  UCB;  //map is a SORTED array of element, so the maximum value is found in O(1), storing the score with its corresponding feature
    double UCB_Score;
    for (int fi=0; fi<f+1; fi++) //for each feature
    {
        if (!node.sub_F[fi]) //choosing a feature which is not already selected
        {
            if (fi == f) {gRAVE_f = gRAVE[f+d].first;} //stopping feature
            else {gRAVE_f = gRAVE[fi].first;}
            alpha = params.c/(params.c + (double)node.t_f[fi]);
            beta = params.cl/(params.cl + (double)node.lRAVE_f[fi].second);

            if (alpha == 1) //need to split in 2 different cases to avoid UCP_Score to be NAN
            {
                UCB_Score = alpha * ((1-beta)*node.lRAVE_f[fi].first + beta*gRAVE_f);
            }
            else
            {
                UCB_Score = alpha * ((1-beta)*node.lRAVE_f[fi].first + beta*gRAVE_f)
                + (1-alpha)*( node.mu_f[fi]  +  sqrt(    params.ce*log((double)node.T_F)/(double)node.t_f[fi]  *  min( (double)0.25 ,  (double)pow(node.sg_f[fi],2) + sqrt(2*log((double)node.T_F)/(double)node.t_f[fi]) )    ));
            }

            UCB.insert({UCB_Score, fi});
            //cout << fi << "     " << UCB_Score << endl;
        }
    }
    //cout << UCB.rbegin()->second << "     " << UCB.rbegin()->first << endl;
    return UCB.rbegin()->second;  //return the feature corresponding to the maximum calculated score (it is the end of the map)
}

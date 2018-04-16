#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>   //pair
#include <map>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>

//include classes
#include "class_Node.h"
#include "class_Params.h"
#include "class_Tree.h"

/* Class Node:
    vector<bool> sub_F;                   //Features subset
    int nF;                               //Number associated to feature subset
    int F_size;                           //Number of features in the subset
    int T_F;                              //Nb of time the node has been visited
    double av_F                           //Average of the node
    double sg_f                           //Variance of the node
    unordered_set<int> allowed_features;  //Allowed features (only for Discrete heuristic)
    vector<int> address_f;                //Node address of the child nodes, (= -1 if never visited)
    vector<pair<double, int>> lRAVE_f;    //Local RAVE score of each feature
    double Node_Score;                    //Score of the node (it is the average at the moment)
    int N_final;                          //Number of time this node has been chosen final
*/




//================================================================================================================================================================//
//====================================================================== Discrete Heuristic ======================================================================//
//================================================================================================================================================================//

int Discrete_UCB(Node &node, const vector <pair<double,int>> &gRAVE, const Tree &T, const Params &params)
// Return the feature corresponding to the highest UCB score
{
    int d = node.F_size;  //feature subset size
    int f = node.sub_F.size()-1; //number of features






    //----------------------------------------------------------------------------------------------------------------------------------------------------------//
    //------------------------------------------- Exploring a new child node whenever floor(T_F^b) is incremented ----------------------------------------------//
    //----------------------------------------------------------------------------------------------------------------------------------------------------------//

    if ( ( ( (floor(pow((double)node.T_F+(double)1,params.b)) - floor(pow((double)node.T_F,params.b))) != 0) && (node.allowed_features.size() < (f-d+1)) )  )
    {    // if  (                   floor(T_F^b) incremented                       &&                  allowed feature subset not already full  )



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
                if (fi == f) {gRAVE_f = gRAVE[f+d+1].first;} //stopping feature
                else {gRAVE_f = gRAVE[fi].first;}
                beta = params.cl/(params.cl + (double)node.lRAVE_f[fi].second);

                RAVE_Score = (1-beta)*node.lRAVE_f[fi].first + beta*gRAVE_f;
                RAVE.insert({RAVE_Score, fi});
            }
        }
        node.allowed_features.insert(RAVE.rbegin()->second);  //adding (to allowed feature set) the feature corresponding to the maximum calculated RAVE score
    }                                                         // (it is the one at the end of the map)


    else //if [TF^b] not incremented, and if the feature set is empty, we perform random exploration
    {
        if (node.allowed_features.empty()) //allowed feature set is empty
        {
            return -1; //-1 indicate that we don't to want chose any feature
        }
    }





    //------------------------------------------------------------------------------------------------------------------------------------------------//
    //------------------------------------------------- Computing the UCB scores of allowed features -------------------------------------------------//
    //------------------------------------------------------------------------------------------------------------------------------------------------//

    map <double, int>  UCB; //map is a SORTED array of element, so the maximum value is found in O(1), storing the score with its corresponding feature
    double UCB_Score;

    int T_F;
    int t_f;
    double mu_f;
    double sg_f;

    for ( auto &fi : node.allowed_features ) //computing UCB for each allowed feature
    {
        T_F = node.T_F;

        if (fi==f) //stopping feature
        {
            t_f = node.fs[0];
            mu_f = node.fs[1];
            sg_f = node.fs[2];
        }
        else
        {

            if (node.address_f[fi] == -1) //if feature never selected before, UCB score is infinite (the child node will then be added to the tree)
            {
                return fi;  //return the new feature
            }

            t_f = T.N[node.address_f[fi]].T_F;
            mu_f = T.N[node.address_f[fi]].av_F;
            sg_f = T.N[node.address_f[fi]].sg_F;
        }

        UCB_Score = mu_f + sqrt(    params.ce*log((double)T_F)/(double)t_f  *  min( (double)0.25 ,  (double)pow(sg_f,2) + sqrt(2*log((double)T_F)/(double)t_f) )    );
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
    int d = node.F_size;  //feature subset size
    int f = node.sub_F.size()-1; //number of features

    //defining useful parameters
    double beta;
    double alpha;
    double gRAVE_f;

    int T_F;
    int t_f;
    double mu_f;
    double sg_f;


    //------------------------------------------------------------------------------------------------------------------------------------------------//
    //------------------------------------------------- Computing the UCB scores of the child nodes --------------------------------------------------//
    //------------------------------------------------------------------------------------------------------------------------------------------------//

    map <double, int>  UCB;  //map is a SORTED array of element, so the maximum value is found in O(1), UCB is storing the score with its corresponding feature
    double UCB_Score;


    for (int fi=0; fi<f+1; fi++) //for each feature
    {
        if (!node.sub_F[fi]) //choosing a feature which is not already selected
        {


            //------------------------------ Taking the values from child nodes ------------------------------//

            if (fi == f) //stopping feature
            {
                t_f = node.fs[0];
                mu_f = node.fs[1];
                sg_f = node.fs[2];
                gRAVE_f = gRAVE[f+d+1].first;

            }
            else
            {
                if (node.address_f[fi] == -1) //if feature never selected before, all the parameters are zero
                {
                    t_f = 0;
                    mu_f = 0;
                    sg_f = 0;
                }
                else
                {
                    t_f = T.N[node.address_f[fi]].T_F;
                    mu_f = T.N[node.address_f[fi]].av_F;
                    sg_f = T.N[node.address_f[fi]].sg_F;
                }
                gRAVE_f = gRAVE[fi].first;
            }
            T_F = node.T_F;

            alpha = params.c/(params.c + (double)t_f);
            beta = params.cl/(params.cl + (double)node.lRAVE_f[fi].second);



        //--------------------------------------- Computing UCB scores ---------------------------------------//

            if (alpha == 1) //we need to split in 2 different cases to avoid UCB_Score to be NAN
            {
                UCB_Score = alpha * ((1-beta)*node.lRAVE_f[fi].first + beta*gRAVE_f);
            }
            else
            {
                UCB_Score = alpha * ((1-beta)*node.lRAVE_f[fi].first + beta*gRAVE_f)
                + (1-alpha)*( mu_f  +  sqrt(    params.ce*log((double)T_F)/(double)t_f  *  min( (double)0.25 ,  (double)pow(sg_f,2) + sqrt(2*log((double)T_F)/(double)t_f) )    ));
            }

            UCB.insert({UCB_Score, fi});
            //cout << fi << "     " << UCB_Score << endl;
        }
    }
    //cout << UCB.rbegin()->second << "     " << UCB.rbegin()->first << endl;
    return UCB.rbegin()->second;  //return the feature corresponding to the maximum calculated score (it is the end of the map)
}

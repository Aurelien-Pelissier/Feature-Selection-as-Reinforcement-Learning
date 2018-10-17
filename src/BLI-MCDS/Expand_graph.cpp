///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////                           UGapE - MCLS (Monte Carlo Lattice Search)                       //////////////
//////////////                 (delta - epsilon) PAC BAI problem in fixed confidence setting             //////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Created : 2018-07-24                                                                                              //
// Updated : 2018-08-01                                                                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_set>
#include <map>
using namespace std;

//including functions
#include "functions.h"
#include "print.h"

//including classes
#include "class_Params.h"   //Contains all the important simulation parameters
#include "class_Variables.h"


int    Expand_graph(Tree &T, const Params &params, Variables &variables, vector<pair<double,int>> &gRAVE)
{
    int leaf_address;
    leaf_address = Expand_DAG(T, params, variables, gRAVE);

    return leaf_address;
}




int     Expand_DAG(Tree &T, const Params &params, Variables &variables, vector<pair<double,int>> &gRAVE) ///need to be improved, the factor 1/d is not necessarily good
// add one node to the lattice
{

    //--------------------------------------------------- Finding the node to be expanded ---------------------------------------------------//

    map <double,int> Index;
    int f = params.f;
    int d;
    double index_i;
    bool check_under_root;

    for (int i=0; i<T.N.size(); i++)
    {

        check_under_root = true; //check if the node is under the current root
        for (const auto & fi : variables.selected_features)
        {
            if (!T.N[i].sub_F[fi])
            {
                check_under_root = false;
            }
        }

        if (check_under_root)
        {
            d = T.N[i].F_size;
            if ( (T.N[i].allowed_features.size() < (f - d) ) && (!T.N[i].sub_F[params.f]) )  //if children are not all already added
            {
                if (d <= 50)  //another condition that we would like to put (ex - limit the depth)
                {
                    index_i = 1/((double) d + 1) * T.N[i].T_F / (T.N[i].allowed_features.size() + 1);
                    Index.insert({index_i,i});
                }
            }
        }
    }

    int expand_address = Index.rbegin()->second; //(The leaf status is updated later when Add_family is called)



    //--------------------------------------------------- Choose the child to add ---------------------------------------------------//

    int leaf_address;
    boost::dynamic_bitset<> F = T.N[expand_address].sub_F;


    Check_for_stopping_feature(T,expand_address,params, variables);


    map <double, int>  RAVE;
    double RAVE_Score;
    double beta;

    for (int fi=0; fi<f; fi++)
    {
        if ( (!F[fi]) && (T.N[expand_address].allowed_features.find(fi) == T.N[expand_address].allowed_features.end()) )  //if feature not already added
        {
            beta = params.cl/(params.cl + (double)T.N[expand_address].lRAVE_f[fi].second);
            RAVE_Score = (1-beta)*T.N[expand_address].lRAVE_f[fi].first + beta*gRAVE[fi].first;
            RAVE.insert({RAVE_Score, fi});
        }
    }
    int f_add = RAVE.rbegin()->second;

    F[f_add] = 1;
    leaf_address = Add_Node_And_Get_Adress(T, F, params);
    T.N[leaf_address].is_leaf = true;
    variables.n_leaf_temp++;
    Add_Familly(T,leaf_address,params,variables);
    F[f_add] = 0;

    if (params.f <= 20 )
    {
        cout << "    (" << setprecision(3) << setw(5) << Index.rbegin()->first << ")   ";
        print_features(T.N[expand_address].sub_F);
        cout << "  ->  ";
        print_features(T.N[leaf_address].sub_F);
    }


    return leaf_address;
}

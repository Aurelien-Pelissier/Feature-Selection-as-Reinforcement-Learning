#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

//libraries
#include <boost/dynamic_bitset.hpp>
#include "kdtree/alglibmisc.h"
#include "kdtree/stdafx.h"

//include functions
#include "quickselect.h"

//include classes
#include "class_Params.h"

using namespace std;
using namespace alglib;










multimap <int, float>      naive_kNN(const vector<vector<float>> &L, const vector<vector<float>> &V, const boost::dynamic_bitset<> &F, const Params &params)

//    return sF, sorted vector of pair containing example score with its label

{



    //==================================================================================================================================================//
    //--------------------------------------------- Find the k nearest neighbors for each element in V -------------------------------------------------//
    //--------------------------------------------------------- Complexity is O(m*n*(d+1)) -------------------------------------------------------------//
    //==================================================================================================================================================//

    multimap <int, float> sF;      //Ordered multimap of {sf,V[mi][f]}

    int n = params.n;
    int d = V[0].size()-1;
    int m = params.m;
    int k = params.k;


    float df;        			   //Squared distance based on features in F
    vector<pair<float, int>> dF;   //dF is a vector of pair containing {distance, index}, the initial index needs to be saved because the quickselect function modify the array
    vector<int> n_index(k);        //indexes of the k nearest neighboors
    int sf;        			       //Number of positive among k

    int mi, ni, fi, ki;


    for (mi=0;mi<m;mi++)
    {
        //calculate the squared distance between points - complexity O(n*f)
        for (ni=0;ni<n;ni++)
        {
            df=0;
            for (fi=0;fi<d;fi++)
            {
                df = df + pow(L[ni][fi]-V[mi][fi],2);
            }
            if (df != 0) dF.push_back({df,ni}); //we dont want the point itself to be considered as a neighboor
        }

        if (dF.empty())
        {
            cout << endl << " Warning: All the examples are at distance zero in the training set for feature [";
            for (int fi=0;fi<params.f;fi++) {if (F[fi]==1){cout << " " << fi << " ";}} cout << "]"<< endl;
            cout << "    This happened because all values of one feature are equal in the training set" << endl << "     The reward is returned 0, it is a bad feature anyway"<< endl << endl;
            sF.clear();
            return sF;
        }

        if (dF.size() < k)
        {
            cout << endl << " Warning: Less than k = " << k << " examples are at nonzero distance for feature [";
            for (int fi=0;fi<params.f;fi++) {if (F[fi]==1){cout << " " << fi << " ";}} cout << "]"<< endl;
            cout << "    This happened because all values of one feature are equal in the training set" << endl << "    The reward is returned 0, it is a bad feature anyway" << endl << endl;
            sF.clear();
            return sF;
        }


        //chose the k min distances - complexity O(n)in average thanks to quickselect
        n_index = quick_select(k, dF, 0);
        sf = 0;
        for (ki=0;ki<k;ki++)
        {
             if (L[n_index[ki]][d]>0)
             {
                    sf++; //count the positives ones
             }
        }
        sF.insert({sf,V[mi][d]}); //complexity O(log(mi))
        dF.clear();

    }

    return sF;
}





multimap <int, float>  KD_tree_kNN(const real_2d_array &La, const vector<vector<float>> &V, const boost::dynamic_bitset<> &F, const Params &params)
// perform nearest neighbors with KD-tree, ALGLIB library is used --> http://www.alglib.net/other/nearestneighbors.php

//    If use_kdt, precomputed tree is used
//    return sF, sorted vector of pair containing example score with its label
{
    multimap <int, float> sF;      //Ordered multimap of {sf,V[mi][f]}

    int n = params.n;
    int d = V[0].size()-1;
    int m = V.size();
    int k = params.k;
    int sf;        			       //Number of positive among k


    //initialize parameters for the kd-tree
    ae_int_t nx = d;
    ae_int_t ny = 1;
    ae_int_t normtype = 2;
    std::string s;

    real_1d_array x;
    real_2d_array r0 = "[[]]";
    double _x[d];


    //Build the KD-Tree (complexity d*n*log(n))
    kdtree kdt;
    kdtreebuild(La, nx, ny, normtype, kdt);
    alglib::kdtreeserialize(kdt, s);


    //perform predictions on m querries (complexity m*k*log(n)*1.7^d)   --> http://www.alglib.net/other/nearestneighbors.php
    for (int mi=0;mi<m;mi++)
    {

        for (int i=0;i<d;i++) {_x[i] = (double) V[mi][i];}
        x.setcontent(d,_x);

        kdtreequeryknn(kdt, x, k, false); //false to avoid considering the point itself as NN
        kdtreequeryresultsxy(kdt, r0);

        if (r0.rows() < k)
        {
            cout << endl << " Warning: Less than k = " << k << " examples are at nonzero distance for feature [";
            for (int fi=0;fi<params.f;fi++) {if (F[fi]==1){cout << " " << fi << " ";}} cout << "]"<< endl;
            cout << "    This happened because all values of one feature are equal in the training set" << endl << "    The reward is returned 0, it is a bad feature anyway" << endl << endl;
            sF.clear();
            return sF;
        }

        sf = 0;
        for (int ki=0;ki<k;ki++)
        {
            if (r0[ki][d] > 0) sf++;
        }

        sF.insert({sf,V[mi][d]});
    }


    return sF;
}


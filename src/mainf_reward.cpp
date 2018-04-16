#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>

//include classes
#include "class_Params.h"


// Return the reward (double) for inputs:
//      - training set L[n][f+1] with f the number of features and +1 for the CV label (x1 x2 x3 ... xf y)
//      - feature subset F[f] with F[f]=0 if feature not used
//        The reward is calculated with nearest neighbor methods
//        THE STOPPING FEATURE IS NOT CONSIDERED


double reward(const boost::dynamic_bitset<> &F, const Params &params)  //& to pass the reference so the vectors are not copied (gain time), const to make sure that we don't modify it
{

    //important variables:
    int f = F.size()-1;       //Number of features (-1 to remove the stopping feature)
    int n = params.L.size();  //Size of the training set
    int m = n/params.r;       //Size of the aggressive subsample
    int k = params.k;            //Number of nearest neighbors


    if (params.L[0].size() != F.size())
    {
        cout << "Error - feature size does not correspond with training set" << endl;
        cout << "Feature subset f = " << F.size()-1 << " and Training set f+1 =" << params.L[0].size() << endl;
        exit(1);
    }


    if (   (F.count() == 0) || ( (F.count() == 1) && (F[f] == 1) )    ) //if empty feature subset, it's to avoid empty map in the reward calculation
    {
          return 0;
    }


    //declaring variables:
    vector< vector<float> > V(m,vector<float>(f+1));   //V[m][f+1] Aggressive subsample of L
    int sf;        			  //Number of positive among k
    multimap <int, float> sF; //Ordered multimap of {sf,mi}
    vector<float> dF(n);      //Squared distance based on features in F
    double reward_V;          //Reward value

    int positive, total, N_ones;      //Variables for the reward function
    vector<int> n_index(k);   //Variable for nearest neighbor
    vector<int> check(n);     //Variable for aggressive sub-sampling
    int ni0;                  //Variable for aggressive sub-sampling
    int ni, mi, mj, fi, ki;   //Increment numbers








    //==================================================================================================================================================//
	//------------------------------------------------------ Compute an aggressive subsample V  --------------------------------------------------------//
	//------------------------------------- Subsample of the training set of size (m=n/r) with uniform selection ---------------------------------------//
	//==================================================================================================================================================//

	mi = 0;
    while (mi<m)
    {
        ni = floor(rand()% n); //select a random example from the training set
        if (check[ni]==0){  //to avoid adding the same feature 2 times
            for (fi=0;fi<f+1;fi++){
                V[mi][fi] = params.L[ni][fi];
            }
            mi++;
        }
        check[ni]=1;
    }









    //==================================================================================================================================================//
    //--------------------------------------------- Find the k nearest neighbors for each element in V -------------------------------------------------//
    //--------------------------------------------------------- Complexity is O(m*n*(f+k)) -------------------------------------------------------------------//
    //==================================================================================================================================================//

    for (mi=0;mi<m;mi++)
    {
        //calculate the squared distance between points - complexity O(n*f)
        for (ni=0;ni<n;ni++){
            dF[ni]=0;
            for (fi=0;fi<f;fi++){
                if (F[fi]){  //calculate the distance only for used features
                    dF[ni] = dF[ni] + pow(params.L[ni][fi]-V[mi][fi],2);
                }
            }
            if (dF[ni] == 0.0) ni0 = ni; //finding the index of its twin in L
        }


        //chose the k min distances - complexity O(k*n)
        sf = 0;
        dF[ni0] = *max_element(dF.begin(), dF.end()); //correspond to max(dF) - we don't want the point itself to be considered as a neighbor
        for (ki=0;ki<k;ki++)
        {
             //find the index of the minimum distance
             n_index[ki] = distance(dF.begin(), min_element(dF.begin(), dF.end())); //correspond to argmin(dF)
             if (params.L[n_index[ki]][f]>0){
                    sf++; //count the positives ones
             }

             dF[n_index[ki]] = dF[ni0]; //we put the minimum to a high value to get the second minimum on next iteration
        }

        sF.insert({sf,V[mi][f]}); //complexity O(log(mi))
    }








    //==================================================================================================================================================//
    //------------------------------------------------------------ Computing the reward V --------------------------------------------------------------//
    //------------------------------------------------ Complexity O(m) thanks to sorted sF multimap ----------------------------------------------------//
    //==================================================================================================================================================//

    positive = 0;
    N_ones = 0;
    multimap <int, float>::reverse_iterator itS = sF.rbegin();
	while (itS != sF.rend())
	{
		if (itS->second == 1) {N_ones++;}
		else {positive += N_ones;}
		itS++;
	}


    if ((N_ones == 0)||(N_ones == m))
    {
        cout << endl << "Error: aggressive subsample is too small and the reward cannot be calculated" << endl << endl;
        exit(1);
    }

    total = N_ones*(m-N_ones);
    reward_V = (double)positive/(double)total;
    return reward_V;

}

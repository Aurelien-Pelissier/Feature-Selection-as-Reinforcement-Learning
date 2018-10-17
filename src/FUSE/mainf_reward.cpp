#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <random>

//include libraries
#include <boost/dynamic_bitset.hpp>
#include "kdtree/alglibmisc.h"
#include "kdtree/stdafx.h"

//include functions
#include "quickselect.h"
#include "select_features.h"
#include "kNN.h"
#include "dataset.h"

//include classes
#include "class_Params.h"

using namespace std;
using namespace alglib;




int rand_int(int a, int b, int rseed)
//reuturn an integer unifomly selected between a and b
{
  static std::mt19937 generator(rseed); //static is important to avoid the generator returning always the same number
  std::uniform_int_distribution<int> distribution (a,b);
  return distribution(generator);
}




vector<vector<float>>    subsample(const vector<vector<float>> &L, const int &n, const int &d, const int &m, const int &rseed)
// Compute a Subsample of the training set L of size m with uniform selection
//     - n is training set size
//     - d is number of features (without labels)
//     - we pass them as argument to avoid calculating them each time

// Complexity O(md) in average
{

    if (m == n)
    {
        return L;
    }
    else if (m>n)
    {
        cout << " Error: The subsample size m = " << m << " should be smaller than the training set size n = " << n << endl << endl;
        exit(1);
    }

    vector< vector<float> > V;   //V[m][f] Aggressive subsample of L
	vector<int> check(n);  //Variable to check if the feature has been added, we dont want to add 2 times the same element

	int mi = 0;
    while (mi<m)
    {
        int ni = rand_int(0, n-1, rseed); //select a random example from the training set
        if (check[ni]==0){     //to avoid adding the same feature 2 times
            V.push_back(vector<float>());
            for (int fi=0;fi<d+1;fi++){
                V[mi].push_back(L[ni][fi]);
            }
            mi++;
        }
        check[ni]=1;
    }
    return V;
}







double reward(const boost::dynamic_bitset<> &F, const Params &params) //& to pass the reference so the vectors are not copied (gain time), const to make sure that we don't modify it
// Return the reward (double) for inputs:
//      - training set L[n][f+1] with f the number of features and +1 for the CV label (x1 x2 x3 ... xf y)
//      - feature subset F[f] with F[f]=0 if feature not used
//        The reward is calculated with nearest neighbor methods
//        THE STOPPING FEATURE IS NOT CONSIDERED

{



    //==================================================================================================================================================//
	//--------------------------------------------------------- Check for particular cases  ------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------------------------//
	//==================================================================================================================================================//


    int f = params.f;          //Number of features (-1 to remove the stopping feature)

    if (f+1 != F.size())
    {
        cout << " Error - feature size does not correspond with training set" << endl;
        cout << "     Feature subset f = " << F.size()-1 << " and Training set f+1 =" << params.L[0].size() << endl;
        exit(1);
    }


    if (   (F.count() == 0) || ( (F.count() == 1) && (F[f] == 1) )    ) //if empty feature subset, directly return 0 --> it's to avoid weird behaviors in map
    {
          return 0;
    }






    //==================================================================================================================================================//
	//------------------------------------------------ Reduce the training set to selected features  ---------------------------------------------------//
	//----------------------------------------------- (To avoid heavy computation) - Complexity O(nd) --------------------------------------------------//
	//==================================================================================================================================================//


    int ni, mi, fi, ki;   //Increment numbers
    vector<vector<float>> L = select_features(params.L, params.n, params.f, F);

    // important variables:
    int n = params.n;  //size of the subtraining example
    int d = F.count();           //Number of features in the feature subset (d for depth)
    int m = params.m;          //Size of the aggressive subsample
    int k = params.k;            //Number of nearest neighbors


    if (F[f] == 1) d--;          //Make sure that the last bit is not counted (it is the stopping feature)




    //==================================================================================================================================================//
	//--------------------------------------------- Compute an aggressive subsample V - Complexity O(m) ------------------------------------------------//
	//------------------------------------- Subsample of the training set of size (m=n/r) with uniform selection ---------------------------------------//
	//==================================================================================================================================================//


	vector<vector<float>> V = subsample(L,params.n, d,m, params.rseed);



    //==================================================================================================================================================//
    //--------------------------------------------- Find the k nearest neighbors for each element in V -------------------------------------------------//
    //--------------------------------------------------------- Complexity is O(m*n*(d+1)) -------------------------------------------------------------//
    //==================================================================================================================================================//

    multimap <int, float> sF;

    if (!params.KdTree_policy)
    {
        sF = naive_kNN(L, V, F, params);
    }
    else
    {
        //create a real_2d_array for kdtree library
        double _L[n*(d+1)];
        for (int i=0;i<n*(d+1);i++)
        {
            _L[i] = (double) L[i/(d+1)][i%(d+1)];
        }
        real_2d_array La; //convert the dataset into "real_2d_array" for kdTree
        La.setcontent(n,d+1,_L);

        sF = KD_tree_kNN(La, V, F, params);
    }






    //==================================================================================================================================================//
    //------------------------------------------------------------ Computing the reward V --------------------------------------------------------------//
    //------------------------------------------------ Complexity O(m) thanks to sorted sF multimap ----------------------------------------------------//
    //==================================================================================================================================================//

    double reward_V;                 //Reward value

    if (sF.empty())
    {
        return 0;
    }

    if (params.Reward_policy)  //AUC
    {
        int positive, total, N_ones;     //Variables for the reward function

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
            print_dataset(V);
            cout << endl << " Error: aggressive subsample is too small and the reward cannot be calculated" << endl;
            cout << "    This happened because m = n/r = " << m << " is too small" << endl << "    and all the examples has the same label"<< endl << endl;
            exit(1);
        }

        total = N_ones*(m-N_ones);
        reward_V = (double)positive/(double)total;

    }


    else   //ACC
    {
        int positive =0;
        multimap <int, float>::reverse_iterator itS = sF.rbegin();
        while (itS != sF.rend())
        {
            if ( (itS->first < k/2) && (itS->second <= 0))
            {
                positive++;
            }
            else if ( (itS->first >= k/2) && (itS->second > 0))
            {
                positive++;
            }
            itS++;
        }

        reward_V = (double)positive/(double)m;
    }

    return reward_V;

}






double reward_full(const boost::dynamic_bitset<> &F, const Params &params) //& to pass the reference so the vectors are not copied (gain time), const to make sure that we don't modify it
// Return the reward (double) for inputs:
//      - training set L[n][f+1] with f the number of features and +1 for the CV label (x1 x2 x3 ... xf y)
//      - feature subset F[f] with F[f]=0 if feature not used
//        The reward is calculated with nearest neighbor methods
//        THE STOPPING FEATURE IS NOT CONSIDERED

{



    //==================================================================================================================================================//
	//--------------------------------------------------------- Check for particular cases  ------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------------------------//
	//==================================================================================================================================================//


    int f = params.f;          //Number of features (-1 to remove the stopping feature)

    if (f+1 != F.size())
    {
        cout << " Error - feature size does not correspond with training set" << endl;
        cout << "     Feature subset f = " << F.size()-1 << " and Training set f+1 =" << params.L[0].size() << endl;
        exit(1);
    }


    if (   (F.count() == 0) || ( (F.count() == 1) && (F[f] == 1) )    ) //if empty feature subset, directly return 0 --> it's to avoid weird behaviors in map
    {
          return 0;
    }






    //==================================================================================================================================================//
	//------------------------------------------------ Reduce the training set to selected features  ---------------------------------------------------//
	//----------------------------------------------- (To avoid heavy computation) - Complexity O(nd) --------------------------------------------------//
	//==================================================================================================================================================//


    int ni, mi, fi, ki;   //Increment numbers
    vector<vector<float>> L = select_features(params.L, params.n, params.f, F);

    // important variables:
    int n = params.n;  //size of the subtraining example
    int d = F.count();           //Number of features in the feature subset (d for depth)
    int k = params.k;            //Number of nearest neighbors


    if (F[f] == 1) d--;          //Make sure that the last bit is not counted (it is the stopping feature)



    //==================================================================================================================================================//
    //--------------------------------------------- Find the k nearest neighbors for each element in V -------------------------------------------------//
    //--------------------------------------------------------- Complexity is O(m*n*(d+1)) -------------------------------------------------------------//
    //==================================================================================================================================================//

    multimap <int, float> sF;

    //create a real_2d_array for kdtree library
    double _L[n*(d+1)];
    for (int i=0;i<n*(d+1);i++)
    {
        _L[i] = (double) L[i/(d+1)][i%(d+1)];
    }
    real_2d_array La; //convert the dataset into "real_2d_array" for kdTree
    La.setcontent(n,d+1,_L);

    sF = KD_tree_kNN(La, L, F, params);






    //==================================================================================================================================================//
    //------------------------------------------------------------ Computing the reward V --------------------------------------------------------------//
    //------------------------------------------------ Complexity O(m) thanks to sorted sF multimap ----------------------------------------------------//
    //==================================================================================================================================================//

    double reward_V;                 //Reward value

    if (sF.empty())
    {
        return 0;
    }

    if (params.Reward_policy)  //AUC
    {
        int positive, total, N_ones;     //Variables for the reward function

        positive = 0;
        N_ones = 0;

        multimap <int, float>::reverse_iterator itS = sF.rbegin();
        while (itS != sF.rend())
        {
            if (itS->second == 1) {N_ones++;}
            else {positive += N_ones;}
            itS++;
        }
        total = N_ones*(n-N_ones);
        reward_V = (double)positive/(double)total;

    }


    else   //ACC
    {
        int positive =0;
        multimap <int, float>::reverse_iterator itS = sF.rbegin();
        while (itS != sF.rend())
        {
            if ( (itS->first < k/2) && (itS->second <= 0))
            {
                positive++;
            }
            else if ( (itS->first >= k/2) && (itS->second > 0))
            {
                positive++;
            }
            itS++;
        }

        reward_V = (double)positive/(double)n;
    }

    return reward_V;

}

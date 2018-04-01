#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;



// return the reward (double) for inputs:
//      - training set L[n][f+1] with f the number of features and +1 for the CV label (x1 x2 x3 ... xf y)
//      - feature subset F[f] with F[f]=0 if feature not used
//        The reward is calculated with nearest neighbor methods


double reward(const vector< vector<float> > &L, const vector<bool> &F)  //& to pass the reference so the vectors are not copied (gain time), const to make sure that we don't modify it
{
    if (L[0].size() != F.size())
      {
        cout << "Error - feature size does not correspond with training set" << endl;
        cout << "Feature subset f = " << F.size()-1 << " and Training set f+1 =" << L[0].size() << endl;
        exit(1);
      }


  // declaring variables:
   int f = F.size()-1;       //Number of features (-1 to remove the stopping feature)
   int n = L.size();         //Size of the training set
   int m = n/2;              //Size of the aggressive subsample
   vector< vector<float> > V(m,vector<float>(f+1));   //V[m][f+1] Aggressive subsample of L
   vector<int> sf(m);        //Number of positive among k
   vector<float> dF(n);      //Squared distance based on features in F
   double reward_V;          //Reward value

   int k = 5;                //Number of nearest neighbors
   int ppos = 0;             //Number of positive examples in the training set
   int positive, total;      //Variables for the reward function
   vector<int> n_index(k);   //Variable for nearest neighbor
   vector<int> check(n);     //Variable for aggressive sub-sampling
   int ni0;                  //Variable for aggressive sub-sampling
   int ni, mi, mj, fi, ki;   //Increment numbers


	//Compute an aggressive subsample V = small subset of the training set (around 1/100) with uniform selection
	mi = 0;
	ppos = 0;
    while (mi<m)
    {
        ni = floor(rand()% n); //select a random example from the training set
        if (check[ni]==0){  //to avoid adding the same feature 2 times
            for (fi=0;fi<f+1;fi++){
                V[mi][fi] = L[ni][fi];
            }
            if (V[mi][f] == 1) ppos = ppos+1;
            mi++;
        }
        check[ni]=1;
    }



    //Find the k nearest neighbors for each element in V - complexity O(m*n*f), can be improved
    for (mi=0;mi<m;mi++)
    {
        //calculate the squared distance between points
        for (ni=0;ni<n;ni++){
            dF[ni]=0;
            for (fi=0;fi<f;fi++){
                if (F[fi]){  //calculate the distance only for used features
                    dF[ni] = dF[ni] + pow(L[ni][fi]-V[mi][fi],2);
                }
            }
            if (dF[ni] == 0.0) ni0 = ni; //finding the index of its twin in L
        }


        //chose the k min distances
        dF[ni0] = *max_element(dF.begin(), dF.end()); //correspond to max(dF) - we don't want the point itself to be considered as a neighbor
        for (ki=0;ki<k;ki++)
        {
             //find the index of the minimum distance
             n_index[ki] = distance(dF.begin(), min_element(dF.begin(), dF.end())); //correspond to argmin(dF)

             if (L[n_index[ki]][f]>0){
                    sf[mi]++; //count the positives ones
             }

             dF[n_index[ki]] = *max_element(dF.begin(), dF.end()); //correspond to max(dF) - we put the minimum to a high value to get the second minimum on next iteration
        }

    }


    //calculate the reward - complexity O(m^2), need to be improved
    positive = 0;
    total = 0;
    for (mi=0;mi<m;mi++){
        for (mj=0;mj<m;mj++){
            if (V[mi][f]<V[mj][f]){
                if (sf[mi]<sf[mj]){
                    positive ++;
                }
                total ++;
            }
        }
    }

    reward_V = (double)positive/(double)total;
    return reward_V;

}

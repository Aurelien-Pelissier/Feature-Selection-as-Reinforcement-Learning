

#include <iomanip>
#include <iostream>         //cin & cout
#include <vector>           //vector
#include <unordered_set>
using namespace std;



//libraries
#include <boost/dynamic_bitset.hpp>





vector<vector<float>>          select_features(const vector<vector<float>> &L, const int &n, const int &f, const unordered_set<int> &Features)
//This function build a new traing set V from the training set L containing only the features in Features
//     - n is training set size
//     - f is number of features (without labels)
//     - we pass them as argument to avoid calculating them each time
//@Complexity is O(nd)  -> with d = size(Features)
{
    int n_er;                //number of time the example has been erazed, it is necessary because the indexes in the vector are shifted each time we erase
    vector<vector<float>> V;

    for (int ni=0; ni<n; ni++)
    {
        V.push_back(vector<float>());
        for (const auto& fi: Features)
        {
           V[ni].push_back(L[ni][fi]);
        }
    }

    return V;
}



vector<vector<float>>          select_features(const vector<vector<float>> &L, const int &n, const int &f, const boost::dynamic_bitset<> &F)
//This function build a new traing set V from the training set L containing only the features fi such as F[fi]=1
//     - n is training set size
//     - f is number of features (without labels)
//     - we pass them as argument to avoid calculating them each time
//@Complexity is O(nd)  -> with d = size(Features)
{
    int n_er;                //number of time the example has been erazed, it is necessary because the indexes in the vector are shifted each time we erase
    vector<vector<float>> V;

    unordered_set<int> Features;
    for (int fi=0; fi<f; fi++)
    {
        if (F[fi]==1) Features.insert(fi);
    }


    for (int ni=0; ni<n; ni++)
    {
        V.push_back(vector<float>());
        for (const auto& fi: Features)
        {
           V[ni].push_back(L[ni][fi]);
        }
        V[ni].push_back(L[ni][f]);  //Addind the labels
    }

    return V;
}

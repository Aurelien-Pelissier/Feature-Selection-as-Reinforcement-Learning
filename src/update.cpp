#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

//including class
#include "class_Node.h"
#include "class_Tree.h"




void  Update_gRAVE(vector < pair <double, int> > &gRAVE, const vector<bool> &F, const double &reward_V)
// update gRAVE score for each feature of feature subset F, by adding the reward_V the the score
{
    int f = F.size()-1;  //number of features (without the stopping feature)
    int d = count_if (F.begin(), F.end(), [](bool i) { return (i==1); }); //number of features in the subset, without the stopping feature

    // (when updated, gRAVE should be updated only if F is final) --> update = not necessary the case for random exploration
    if (!F[f])
    {
        d--; //we dont want the stopping feature to be considered in the feature subset size
    }

    for (int fi=0; fi<f; fi++){
        if(F[fi]){
            gRAVE[fi].first = (gRAVE[fi].first*gRAVE[fi].second + reward_V)/(gRAVE[fi].second +1 );
            gRAVE[fi].second ++;
        }
    }

    // Updating the gRAVE score of stopping feature
    gRAVE[f+d].first = (gRAVE[f+d].first*gRAVE[f+d].second + reward_V)/(gRAVE[f+d].second +1 );
    gRAVE[f+d].second ++;

}




int  Update_Tree_And_Get_Adress(Tree &T, const vector<bool> &F)
//Returning the address of the node corresponding to feature F
//Adding the nodeF to the search Tree:
    //-> if the node F is not in the tree, it is added at the end
    //-> if the node F is already in the tree, it does nothing

{
    //finding the index of the feature using hash function (hash function because complexity = O(1))
        int last_index = T.N_adress.size();
        T.N_adress.insert({F,last_index}); //this will ad the new node if doesn't exist, or do nothing otherwise
        unordered_map<vector<bool>,int>::const_iterator got = T.N_adress.find(F); //getting the iterator corresponding to the feature number
        int node_adress = got->second; //getting the address from the iterator

    if (node_adress == T.N.size())
    {
        T.N.push_back(Node(F));
    }

    return node_adress;

}





void   Update_Node(Node &node, const int fi, const vector<bool> &Ft, const double &reward_V)
// Updating the node with inputs:
//  - the node to be updated
//  - the computed reward
//  - the feature that have been selected in the node fi
//  - the final feature subset Ft (used only for lRAVE score calculation), it's actually not necessary final in the case of random iteration

{
    double xN1 = reward_V;

    //Parent node parameters
    int TF = node.T_F;              //number of time node has been visited
    double MUN = node.Node_av;         //previous average reward of the node
    double MUN1 = (MUN*(double)TF + xN1)/((double)TF+1);  //new average reward of the node
    double Score_1 = MUN1;          //score of the node (considered to be the av at the moment)

    //Child nodes parameters
    int N = node.t_f[fi];
    double muN = node.mu_f[fi];
    double sgN = node.sg_f[fi];
    double muN1 = (muN*(double)N + xN1)/((double)N + 1);
    double SSN = (xN1 - muN1)*(xN1 - muN);  //for variance calculation, see details at (http://jonisalonen.com/2013/deriving-welfords-method-for-computing-variance/)
    double sgN1 = sqrt( (SSN + (double)N*pow(sgN,2))/((double)N+1) );


    //Updating the node values
    node.T_F = TF + 1;
    node.Node_av = MUN1;
    node.Node_Score = Score_1;
    if (fi==Ft.size()-1) node.N_final++;
    node.t_f[fi] = N+1;
    node.mu_f[fi] = muN1;
    node.sg_f[fi] = sgN1;



    //Updating lRAVE scores
    double lRAVE;
    int tl;
    for (int i=0; i<Ft.size(); i++)
    {
        if (Ft[i]^node.sub_F[i]) //^ is the XOR operation, in our case it correspond to subtracting the vectors : Ft - sub_F, note that sub_F is always included in Ft
        {
            lRAVE = node.lRAVE_f[i].first;
            tl = node.lRAVE_f[i].second;

            node.lRAVE_f[i].first = (lRAVE*(double)tl + xN1)/((double)tl+1);
            node.lRAVE_f[i].second = tl + 1;
        }
    }

}




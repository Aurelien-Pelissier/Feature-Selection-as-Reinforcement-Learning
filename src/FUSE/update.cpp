#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

//using boost library
#include <boost/dynamic_bitset.hpp>

//functions
#include "update.h"
#include "mainf.h"

//including class
#include "class_Node.h"
#include "class_Tree.h"
#include "class_Params.h"




void  Update_gRAVE(vector < pair <double, int> > &gRAVE, const boost::dynamic_bitset<> &F, const double &reward_V)
// update gRAVE score for each feature of feature subset F, by adding the reward_V the the score
{
    int f = F.size()-1;  //number of features (without the stopping feature)
    int d = F.count()-1;   //number of features in the subset, without the stopping feature

    //F is should be final when we update gRAVE
    if (!F[f])
    {
        cout << "Error, the feature subsest should be final when updating gRAVE" << endl;
        exit(1);
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
















//========================================================================================================================================================================================//

int  Update_Tree_And_Get_Adress(Tree &T, const boost::dynamic_bitset<> &F, const Params &params)
//Returning the address of the node corresponding to feature F
//Adding the node F to the search Tree:
    //-> if the node F is not in the tree, it is added at the end
    //-> if the node F is already in the tree, it does nothing

{
    //finding the index of the feature using hash function (hash function because complexity = O(1))
        string Fl;
        to_string(F, Fl);
        int last_index = T.N_address.size();
        T.N_address.insert({Fl,last_index}); //this will ad the new node if doesn't exist, or do nothing otherwise (looking at the first argument)
        unordered_map<string,int>::const_iterator got = T.N_address.find(Fl); //getting the iterator corresponding to the feature number
        int node_address = got->second; //getting the address from the iterator

    if (node_address == T.N.size())
    {
        T.N.push_back(Node(F,node_address));
        T.N[node_address].mu_f[params.f] = reward_full(F, params);
    }

    return node_address;
}






//========================================================================================================================================================================================//

void   Update_Node(Node &node, const int fi, const boost::dynamic_bitset<> &Ft, const double &reward_V)
// Updating the node with inputs:
//  - the node to be updated
//  - the computed reward
//  - the feature that have been selected in the node fi (if -1 it mean that random exploration has been preformed)
//  - the final feature subset Ft (used only for lRAVE score calculation), it's actually not necessary final in the case of random iteration

// the allowed features and the child-node address are not updated here, since they have already been updated in iterate

{
    double xN1 = reward_V;


    //Updating the Parent node values
    Update_av(node.T_F, node.Node_av, xN1);


    if (fi==-1)  {Update_av_var(node.t_f.back(), node.mu_f.back(), node.sg_f.back(), xN1); }
    else    {Update_av_var(node.t_f[fi], node.mu_f[fi], node.sg_f[fi], xN1);}
    if (fi == Ft.count()-1) {node.N_final++;}




    //Updating lRAVE scores
    double lRAVE;
    int tl;
    for (size_t i=0; i<Ft.size(); ++i)
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

void  Update_av(int &N_IT, double &AV, const double &xN1)
{
        double N = (double)N_IT;
        double muN = AV;                        //previous average
        double muN1 = (muN*N + xN1)/(N+1);    //new average

        N_IT = (int)N+1;
        AV = muN1;
}


void  Update_av_var(int &N_IT, double &AV, double &SG, const double &xN1)
// update the average AV and the variance SG with:
//     - the number of iteration already involved N
//     - the value to be added xN1
{
        double N = (double)N_IT;
        double muN = AV;                        //previous average
        double muN1 = (muN*N + xN1)/(N+1);    //new average
        double sgN = SG;
        double SSN = (xN1 - muN1)*(xN1 - muN);  //for variance calculation, see details at (http://jonisalonen.com/2013/deriving-welfords-method-for-computing-variance/)
        double sgN1 = sqrt( (SSN + N*pow(sgN,2))/(N+1) );

        N_IT = (int)N+1;
        AV = muN1;
        SG = sgN1;
}






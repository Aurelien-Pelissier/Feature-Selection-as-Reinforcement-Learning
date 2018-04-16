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

//including class
#include "class_Node.h"
#include "class_Tree.h"





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

int  Update_Tree_And_Get_Adress(Tree &T, const boost::dynamic_bitset<> &F)
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
        Add_Familly(T,F,node_address);
    }

    return node_address;
}










//========================================================================================================================================================================================//

void  Add_Familly(Tree &T, boost::dynamic_bitset<> F, const int &node_address)

   //------------------------------------------------------------------------------------------//
   //------------------------- Checking for parents and children nodes ------------------------//
   //------------------------------------------------------------------------------------------//

// If they exist, the address are updated
// here F is send as a copy and not as a pointer to the address, so we can modify it without affecting the global F
{

    string Fp; //parent node string
    string Fc; //child node string
    int parent_node_address;
    int child_node_address;
    unordered_map<string,int>::const_iterator got;

    //indicate to my parents and my child nodes that I exist
    for (int fi=0; fi<F.size(); fi++)
    {
        if (F[fi]) //if this is a 1, replace it with a 0 to find a parent
        {
            F[fi] = 0;
            to_string(F, Fp);
            got = T.N_address.find(Fp);
            if (got != T.N_address.end()) //this parent is int the tree, lets tell him that we exist
            {
                parent_node_address = got->second;
                T.N[parent_node_address].address_f[fi] = node_address;
                T.N[parent_node_address].allowed_features.insert(fi);
            }
            F[fi] = 1; //put the value back to 1 for next loops
        }
        else // if this is a 0, replace it with a 1 to find a child
        {
            F[fi] = 1;
            to_string(F, Fc);
            got = T.N_address.find(Fc);
            if (got != T.N_address.end()) //if my child exist in the tree
            {
                child_node_address = got->second;
                T.N[node_address].address_f[fi] = child_node_address;
                T.N[node_address].allowed_features.insert(fi);
            }
            F[fi] = 0; //put the value back to 1 for next loops
        }
    }


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


    //Updating the Parent node values
    Update_av_var(node.T_F, node.av_F, node.sg_F, xN1);
    node.Score = node.av_F;  //score of the node (considered to be the av at the moment)

    if (fi==Ft.size()-1)  //updating stopping feature values
    {
        Update_av_var(node.fs[0], node.fs[1], node.fs[2], xN1);
    }


    if (fi==-1)   //updating random exploration information
    {
        Update_av_var(node.fr[0], node.fr[1], node.fr[2], xN1);
    }

}



void  Update_av_var(double &N_IT, double &AV, double &SG, const double &xN1)
// update the average AV and the variance SG with:
//     - the number of iteration already involved N
//     - the value to be added xN1
{
        double N = N_IT;
        double muN = AV;                        //previous average
        double muN1 = (muN*N + xN1)/(N+1);    //new average
        double sgN = SG;
        double SSN = (xN1 - muN1)*(xN1 - muN);  //for variance calculation, see details at (http://jonisalonen.com/2013/deriving-welfords-method-for-computing-variance/)
        double sgN1 = sqrt( (SSN + N*pow(sgN,2))/(N+1) );

        N_IT = N+1;
        AV = muN1;
        SG = sgN1;
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

        N_IT = N+1;
        AV = muN1;
        SG = sgN1;
}













void   Update_parents(Tree &T, Node &node, int &fi, const boost::dynamic_bitset<> &Ft, double &reward_V)
// This recursive function will update itself and all its ancestors (which already exist)
// it should be called when Ft is final (it is used only for l-RAVE update)
{

    //Updating myself first
    Update_Node(node, fi, Ft, reward_V);
    node.already_updated = true;
    fi = 0; // fi is useful ONLY for the leaf node. (its only to check if its final or random exploration), we set it to zero after the leaf node has been updated



    //Now update my parents
    if (node.sub_F.count() == 0) return; //if we arrived to the ultimate ancestor (empty Feature set), we can move on

    boost::dynamic_bitset<> F = node.sub_F;
    string Fp;
    int parent_node_address;
    unordered_map<string,int>::const_iterator got;

    for (int i=0; i<F.size(); i++)
    {
        if (F[i]) //if this is a 1, replace it with a 0 to find a parent
        {
            F[i] = 0;
            to_string(F, Fp);
            got = T.N_address.find(Fp);
            if (got != T.N_address.end()) //we found it, lets update him
            {
                parent_node_address = got->second;
                if (T.N[parent_node_address].already_updated == false)
                {
                    Update_parents(T, T.N[parent_node_address], fi, Ft, reward_V);
                }
            }
            F[i] = 1; //put the value back to 1 for next loops
        }
    }

    return;

}

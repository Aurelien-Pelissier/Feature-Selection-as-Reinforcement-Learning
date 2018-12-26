#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <map>
#include <cmath>


//using boost library
#include <boost/dynamic_bitset.hpp>

//including functions
#include "functions.h"
#include "print.h"

//including classes
#include "class_Params.h"      //Contains all the important simulation parameters
#include "class_Node.h"        //Contains all the parameters of a node
#include "class_Tree.h"        //Pair of a Node vector and an associated hash function to quickly find nodes address - in O(1)

using namespace std;



void   Backpropagate(Tree &T, Node &node, const int &root_address, const boost::dynamic_bitset<> &Ft, const double &reward_V, const Params &params, const Variables variables)
//backpropagate the statistic from the leaf node to all of the ancestors
{
    Check_Ancestors(T, node); //important because backpropagation has to be done in an organized way to updated correctly
    Update(T, T.N[root_address], Ft, reward_V, params, variables);
}



void   Check_Ancestors(Tree &T, Node &node)
// recursive function which check all the ancestors to be updated
{

    //tag myself
    node.tobe_updated = true;

    if (node.is_root == true) return;


    //Now tag my parents
    for (const auto& parent_node_address : node.parent_address)
    {
        if (T.N[parent_node_address].tobe_updated == false)
        {
            Check_Ancestors(T, T.N[parent_node_address]);
        }
    }

    return;

}



void   Update(Tree &T, Node &node, const boost::dynamic_bitset<> &Ft, const double &reward_V, const Params &params, const Variables variables)
//recursively backpropagate the statistic from the leaf node to all of the ancestors
{

    if (node.is_leaf == true) //if leaf is reached, this is the end
    {
        Update_Leaf(T, node, Ft, reward_V, params, variables);
        return;
    }

    //Update all the childs
    for ( const auto &fi : node.allowed_features )
    {
        if (T.N[node.address_f[fi]].tobe_updated == true)
        {
            if (T.N[node.address_f[fi]].updated == false)
            {
                Update(T, T.N[node.address_f[fi]], Ft, reward_V, params, variables);
            }
        }
    }

    //We update the node AFTER all of its child has been updated
    Update_Node(T, node, Ft, reward_V);
}



void   Update_Node(Tree &T, Node &node, const boost::dynamic_bitset<> &Ft, const double &reward_V)
{
    node.updated = true;
    node.T_F ++;
    map <double,int> child_U;
    map <double,int> child_L;

    if (node.allowed_features.empty())
    {
        node.U_F = 0;
        node.L_F = 0;
        return;
    }

    for (const auto &fi : node.allowed_features)
    {
        child_U.insert({T.N[node.address_f[fi]].U_F,fi});
        child_L.insert({T.N[node.address_f[fi]].L_F,fi});
    }

    node.repres_child = child_U.rbegin()->second;
    node.U_F = child_U.rbegin()->first;
    node.L_F = child_L.rbegin()->first;
    node.repres_leaf_address = T.N[node.address_f[node.repres_child]].repres_leaf_address;

    Update_lRAVE(node, Ft, reward_V);
}



void   Update_Leaf(Tree &T, Node &node, const boost::dynamic_bitset<> &Ft, const double &reward_V, const Params &params, const Variables variables)
// Updating the leaf with computed reward
{
    node.updated = true;  //mark this node as updated

    //Update average
    Update_av(node.T_F, node.av_F, reward_V);


    if (node.sub_F[params.f])
    {
        node.U_F = node.av_F;
        node.L_F = node.av_F;
    }

    else
    {
        int n_leaf = variables.n_leaf_temp + variables.n_leaf_term;
        double explo;

        if (!params.interval)
        {
            explo = log(n_leaf)/params.delta + log(log(node.T_F)+1);
        }
        else
        {
            explo = log(log(exp(1)*node.T_F)/params.delta);
        }

        node.U_F = node.av_F + sqrt(explo/2/node.T_F);
        node.L_F = node.av_F - sqrt(explo/2/node.T_F);

        Update_lRAVE(node, Ft, reward_V);
    }

}


void  Update_lRAVE(Node &node, const boost::dynamic_bitset<> &Ft, const double &reward_V)
//Updating lRAVE scores
{
    int f = node.sub_F.size();
    double lRAVE;
    int tl;
    for (size_t i=0; i<f; ++i)
    {
        if (Ft[i]^node.sub_F[i]) //^ is the XOR operation, in our case it correspond to subtracting the vectors : Ft - sub_F, note that sub_F is always included in Ft
        {
            lRAVE = node.lRAVE_f[i].first;
            tl = node.lRAVE_f[i].second;

            node.lRAVE_f[i].first = (lRAVE*(double)tl + reward_V)/((double)tl+1);
            node.lRAVE_f[i].second = tl + 1;
        }
    }
}


void  Update_av(double &N_IT, double &AV, const double &xN1)
// update the average AV and the variance SG with:
//     - the number of iteration already involved N
//     - the value to be added xN1
{
        double N = N_IT;
        double muN = AV;                        //previous average
        double muN1 = (muN*N + xN1)/(N+1);    //new average

        N_IT = N+1;
        AV = muN1;
}



#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>

//using boost library
#include <boost/dynamic_bitset.hpp>

//functions
#include "functions.h"
#include "print.h"
#include "realdata/reward.h"

//including class
#include "class_Node.h"
#include "class_Tree.h"
#include "class_Params.h"
#include "class_Variables.h"

using namespace std;



int  Add_Node_And_Get_Adress(Tree &T, const boost::dynamic_bitset<> &F, const Params &params)
//Returning the address of the node corresponding to feature F
//Adding the node F to the search Tree:
    //-> if the node F is not in the tree, it is added at the end
    //-> if the node F is already in the tree, it does nothing but return the address

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
        //print_features(F);
        T.N.push_back(Node(F,node_address));
        //cout << "ok" << endl;
    }

    if (F[params.f])
    {
        T.N[node_address].av_F = reward_full(F, params);
    }

    return node_address;
}





//========================================================================================================================================================================================//

void  Add_Familly(Tree &T, const int &new_node_address, const Params &params, Variables &variables)

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

    boost::dynamic_bitset<> F = T.N[new_node_address].sub_F;

    //indicate to my parent and my child nodes that I exist
    for (int fi=0; fi<F.size(); fi++)
    {
        if (F[fi]) //if this is a 1, replace it with a 0 to find a parent
        {
            F[fi] = 0;
            to_string(F, Fp);
            got = T.N_address.find(Fp);
            if (got != T.N_address.end()) //this parent is in the lattice, lets tell him that we exist
            {
                parent_node_address = got->second;

                if (T.N[parent_node_address].is_leaf) variables.n_leaf_temp--; //this parent is no longer a leaf
                T.N[parent_node_address].is_leaf = false;
                T.N[parent_node_address].was_leaf = true;

                T.N[parent_node_address].address_f[fi] = new_node_address;
                T.N[parent_node_address].allowed_features.insert(fi);
                T.N[new_node_address].parent_address.insert(parent_node_address);

                Check_for_stopping_feature(T,parent_node_address,params, variables);

            }
            F[fi] = 1; //put the value back to 1 for next loops
        }

        else // if this is a 0, replace it with a 1 to find a child
        {
            F[fi] = 1;
            to_string(F, Fc);
            got = T.N_address.find(Fc);
            if (got != T.N_address.end()) //this child is in the lattice
            {
                child_node_address = got->second;

                if (T.N[new_node_address].is_leaf) variables.n_leaf_temp--; //the added node is no longer a leaf
                T.N[new_node_address].is_leaf = false;

                T.N[new_node_address].address_f[fi] = child_node_address;
                T.N[new_node_address].allowed_features.insert(fi);
                T.N[child_node_address].parent_address.insert(new_node_address);
                Update_node_bounds(T, T.N[new_node_address]);

                Check_for_stopping_feature(T,new_node_address,params, variables);

            }
            F[fi] = 0; //put the value back to 1 for next loops
        }
    }
}


void  Check_for_stopping_feature(Tree &T, const int &expand_address, const Params &params, Variables  &variables)
{
    boost::dynamic_bitset<> F = T.N[expand_address].sub_F;
    int f = params.f;

    if (T.N[expand_address].allowed_features.find(f) == T.N[expand_address].allowed_features.end()) //always add stopping leaf if not added yet
    {
        F[f] = 1;
        int leaf_address = Add_Node_And_Get_Adress(T, F, params);
        if (!T.N[leaf_address].is_leaf) variables.n_leaf_term++;
        T.N[leaf_address].is_leaf = true;

        if (T.N[expand_address].is_leaf)  variables.n_leaf_temp--;
        T.N[expand_address].is_leaf = false;
        T.N[expand_address].was_leaf = true;
        T.N[expand_address].address_f[f] = leaf_address;
        T.N[expand_address].allowed_features.insert(f);
        T.N[leaf_address].parent_address.insert(expand_address);


        if (params.f <= 20 )
        {
            cout << "    (stopf)   ";
            print_features(T.N[expand_address].sub_F);
            cout << "  ->  ";
            print_features(T.N[leaf_address].sub_F);
            cout << endl;
        }
        F[f] = 0;
    }
}



void  Update_node_bounds(Tree &T, Node &node)
//adding the information of the child nodes to the parent node
{
    map <double,int> child_U;
    map <double,int> child_L;

    for (const auto &fi : node.allowed_features)
    {
        child_U.insert({T.N[node.address_f[fi]].U_F,fi});
        child_L.insert({T.N[node.address_f[fi]].L_F,fi});
    }

    node.repres_child = child_U.rbegin()->second;
    node.U_F = child_U.rbegin()->first;
    node.L_F = child_L.rbegin()->first;
}






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

}

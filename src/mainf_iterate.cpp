#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

//including functions
#include "mainf.h"
#include "update.h"

//including class
#include "class_Node.h"
#include "class_Params.h"
#include "class_Tree.h"



double iterate(Tree &T, vector <pair<double,int>> &gRAVE, vector<bool> &F, const Params &params)
// exploring the tree with UCB until:
    // -> It arrive to a new node and perform a random exploration
    // -> It select the stopping feature

// The reward is then calculated and all the nodes score are updated with back-propagation.
{

    double reward_V;
    int fi; //selected feature
    int parent_node_adress;

    if (F.back()) // if Feature subset final
    {
        reward_V = reward(params.L,F);
        Update_gRAVE(gRAVE, F, reward_V);
    }

    else
    {
        parent_node_adress = Update_Tree_And_Get_Adress(T, F); //not modified when recursively called

        if (T.N[parent_node_adress].T_F != 0) //if node already visited
        // ============================= UCB exploration ============================= //
        {
            if (params.MA == 0)
            // --------------------- Discrete heuristic --------------------- //
            {
                fi = Discrete_UCB(T.N[parent_node_adress], gRAVE, params);
            }
            else
            // -------------------- Continuous heuristic -------------------- //
            {
                fi = Continuous_UCB(T.N[parent_node_adress], gRAVE, params);
            }
            // -------------------------------------------------------------- //
            F[fi] = 1;
            reward_V = iterate (T,gRAVE,F,params);
        }

        else //if node never visited before
        // =========================== Random exploration =========================== //
        {
            fi = iterate_random(T,F,params);  //the feature fi is the FIRST randomly selected child node, we are not interested in the others.
            T.N[parent_node_adress].allowed_features.insert(fi); //adding the new feature to allowed_feature_set (useful only for discrete UCB)

            reward_V = reward(params.L,F);
            Update_gRAVE(gRAVE, F, reward_V);
            //cout << "random" << endl;
        }
        // ========================================================================== //
        Update_Node(T.N[parent_node_adress], fi, F, reward_V);
    }
    return reward_V;

}

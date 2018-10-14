#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>

//including functions
#include "mainf.h"
#include "update.h"
#include "print.h"

//including class
#include "class_Node.h"
#include "class_Params.h"
#include "class_Tree.h"




/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// iterate is a RECURSIVE function, which mean that it is calling itself many times, until it return something.                                            //
/// In our case, the function will call itself until the features subset is final or until it arrived to a new node and perform random exploration.         //
/// The function will then back-propagate and update the nodes until it finally arrive to main again.                                                       //
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// - The variables fi and parent_node_address are static so they are the TRANSFERED to each recursive call                                                 //
///   so the informations about the path taken by the exploration is kept in the memory and back-propagation can occur                                      //
/// - The feature subset F is transmitted by ADDRESS so it is updated after each recursive call, at the end F is always final and is used to update l-RAVE  //
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double iterate(Tree &T, vector <pair<double,int>> &gRAVE, boost::dynamic_bitset<> &F, const Params &params, int &depth)
// exploring the tree with UCB until:
    // -> It arrive to a new node and perform a random exploration
    // -> It select the stopping feature

// The reward is then calculated and all the nodes score are updated with back-propagation.

{

    double reward_V;
    int fi = 0; //selected feature
    int parent_node_address = 0;
    static int parent_node_address_rec = 0;

    if (F[F.size()-1]) // if Feature subset final
    {
        depth = F.count()-1;
        reward_V = T.N[parent_node_address_rec].mu_f[params.f];
        Update_gRAVE(gRAVE, F, reward_V);
    }

    else
    {
        parent_node_address = Update_Tree_And_Get_Adress(T, F, params);
        parent_node_address_rec = parent_node_address;

        if (T.N[parent_node_address].T_F != 0) //if node already visited
        // ============================= UCB exploration ============================= //
        {
            if (params.MA == 0)
            // --------------------- Discrete heuristic --------------------- //
            {
                fi = Discrete_UCB(T.N[parent_node_address], gRAVE, T, params);
            }
            else
            // -------------------- Continuous heuristic -------------------- //
            {
                fi = Continuous_UCB(T.N[parent_node_address], gRAVE, T, params);
            }
            // -------------------------------------------------------------- //

            if (fi==-1) //it means that no feature has been selected and that we are going to perform random exploration
            {
                depth = F.count();
                reward_V = iterate_random(T,F,params);
                Update_gRAVE(gRAVE, F, reward_V);
            }
            else //add the feature to the feature set
            {
                F[fi] = 1;
                reward_V = iterate (T,gRAVE,F,params,depth);
            }
        }


        else //if node never visited before
        // =========================== Random exploration =========================== //
        {
            depth = F.count();
            reward_V = iterate_random(T,F,params);
            Update_gRAVE(gRAVE, F, reward_V);
            fi = -1; //indicate that random exploration has been perform and thus no feature selected
        }
        // ========================================================================== //
        Update_Node(T.N[parent_node_address], fi, F, reward_V);
    }
    return reward_V;

}

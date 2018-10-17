#include <iostream>         //cin & cout
#include <algorithm>        //min, max, fill, find...
#include <fstream>          //files write and read
#include <string>           //string
#include <vector>           //vector
#include <iomanip>          //set precision & set width
#include <bitset>           //for binary numbers manipulation
#include <unordered_map>    //for hash function
#include <chrono>           //to record simulation time
#include <windows.h>        //get key
#include <map>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>  //for binary vectors (couldn't use std::bitset because the size had to be defined at compilation time)
#include <boost/math/special_functions/binomial.hpp>

//including functions
#include "functions.h"
#include "print.h"

//including classes
#include "class_Params.h"      //Contains all the important simulation parameters
#include "class_Variables.h"   //Contain the global variable of simulations (stopping condition)
#include "class_Node.h"        //Contains all the parameters of a node
#include "class_Tree.h"        //Pair of a Node vector and an associated hash function to quickly find nodes address - in O(1)



void Run_BLI_MCLS(const Params &params)
{

    cout << "     n = " << params.L.size() << " examples" << endl;
    cout << "     f = " << params.L[0].size()-1 << " features" << endl << endl << endl;

    vector < pair <double, int> > gRAVE(params.f);    //Global RAVE score, pair of <RAVE_score, Nb of calc involved>, all are initialized to zero.


    //==================================================== Preparing the Initial Lattice ====================================================//

    cout << "  Building the initial DAG (branching " << params.branch_i << " and depth " << params.depth_i << ") ..." << endl;

    Tree T(params.f + 1);
    Variables variables = Variables();
    Build_initial_graph(T, gRAVE, params, variables);

    int root_address = 0;
    int best_arm, new_root_address;

    while (true)
    {
        variables.STOP = false;
        if (root_address != 0)
        {
            int leaf_term_old = variables.n_leaf_term;
            int leaf_temp_old = variables.n_leaf_temp;

            cout << "  Computing the new DAG (branching " << params.branch_i << " and depth " << params.depth_i << ") ..." << endl;
            Complete_current_graph(T, gRAVE, params, variables, root_address);
            cout << "       " << variables.n_leaf_temp - leaf_temp_old << " (temp) & " << variables.n_leaf_term - leaf_term_old << " (term) leaves has been added" << endl;

            leaf_term_old = variables.n_leaf_term;
            leaf_temp_old = variables.n_leaf_temp;



            //reset the number of leaves
            variables.n_leaf_term = 0;
            variables.n_leaf_temp = 0;
            for (int i=0;i<T.N.size();i++)
            {
                if ( (T.N[i].sub_F[best_arm]) && (T.N[i].is_leaf) )
                {
                    if (T.N[i].sub_F[params.f]) variables.n_leaf_term++;
                    else variables.n_leaf_temp++;
                }
            }

            variables.n_leaf_term_tot += (leaf_term_old - variables.n_leaf_term);
            variables.n_leaf_temp_tot += (leaf_temp_old - variables.n_leaf_temp);
            cout << "     Changing the root to the best child " << endl;
            cout << "       " << leaf_temp_old - variables.n_leaf_temp << " (temp) & " << leaf_term_old - variables.n_leaf_term << " (term) leaves has been removed" << endl << endl << endl;

        }
        best_arm = Run_BAI_MCLS(T, root_address, params, variables, gRAVE);
        new_root_address = T.N[root_address].address_f[best_arm];
        root_address = new_root_address;



        if (T.N[root_address].sub_F[params.f]) //if root is final
        {
            variables.n_leaf_term_tot += variables.n_leaf_term;
            variables.n_leaf_temp_tot += variables.n_leaf_temp;

            cout << endl << endl << "  " << T.N[root_address].F_size -1 << " features has been selected after " << variables.it << " iterations with reward " << variables.reward.back() << endl;
            cout << "  Best leaf with with precision eps = " << params.eps << " is ";
            print_features(T.N[root_address].sub_F);
            cout << endl << "  Containing features = [";
            for (int i=0;i<variables.selected_features.size();i++) {cout << " " << variables.selected_features[i] << " ";} cout << "]"<< endl;
            cout << "  after [";
            for (int i=0;i<variables.itt.size();i++) {cout << " " << variables.itt[i] << " ";} cout << "] iterations" << endl;
            cout << "  with reward [";
            for (int i=0;i<variables.reward.size();i++) {cout << " " << setprecision(4) << variables.reward[i] << " ";} cout << "]" << endl;
            cout << "  In the total DAG, there is " << variables.n_leaf_temp_tot << " (temp) & " << variables.n_leaf_term_tot << " (term) leaves"  << endl << endl;
            break;
        }
    }

    print_results (T, params, variables);

}





int Run_BAI_MCLS(Tree &T, const int &root_address, const Params &params, Variables &variables, vector<pair<double,int>> &gRAVE)
{

    int t=0,  n_exp=0;
    int fi, leaf_address;
    double reward_V;

    T.N[root_address].is_root = true;
    double max_tree_size = pow(2,params.f + 1);
    boost::dynamic_bitset<> F;


    //===================================================== Running BAI MCLS algorithm ======================================================//

    cout << "  Starting Best Arm Identification with UGapE policy ..." << endl;

    while (!variables.STOP)
    {

        /*
        if (t == 81182)
        {
            cout << "debug" << endl;
        }
        if (t > 80000)
        {
            cout << t << endl;
        }
        */

        if (t%5000 == 0) {cout << endl << "    it = " << setw(3) << std::setfill('0') << (t / 1000) << " " << setw(3) << std::setfill('0') << (t % 1000);
                          cout << "       N_leaf = " << variables.n_leaf_temp << " (temp) & " << variables.n_leaf_term << " (term)"  << "       N_nodes = " << T.N.size() << " / 2^" << params.f + 1 << endl;}
        if (t%20000== 0) {cout << "       You can pause the search any time by pressing 'j'" << endl << endl;}


        //reset the parameters
        for (int i=0;i<T.N.size();i++) {T.N[i].updated = 0; T.N[i].tobe_updated = 0;}
        T.N[root_address].Tt_F++;

        //check for expansion and pull the added leaf
        if  ( ( (floor(pow(t+1.0,params.b)) - floor(pow(t,params.b))) != 0) && ( (double)T.N.size() < max_tree_size) )
        {
            leaf_address = Expand_graph(T, params, variables, gRAVE);
            reward_V = Evaluation(T, F, leaf_address,params);
            Backpropagate(T, T.N[leaf_address], root_address, F, reward_V, params, variables);
            n_exp++; cout << "    Expansion " << n_exp << endl;
        }
        else //perform the search with UgapE policy
        {
            fi = UGapE(T, T.N[root_address], params, variables);
            leaf_address = Pull_representative_leaf(T, T.N[root_address], fi);
            reward_V = Evaluation(T, F, leaf_address,params);
            Backpropagate(T, T.N[leaf_address], root_address, F, reward_V, params, variables);
        }

        t++;

        if(GetKeyState('J') & 0x8000) //check if high-order bit is set
        {
            cout << endl;
            print_Node(T,T.N[root_address]);
            cout << "  Press any key to continue" << endl;
            string Answer;
            getline( cin, Answer );
        }
    }

    variables.it += t;
    variables.itt.push_back(t);
    if (variables.best_child != params.f) variables.selected_features.push_back(variables.best_child);
    if (variables.best_child != params.f) variables.reward.push_back(T.N[T.N[T.N[root_address].address_f[variables.best_child]].address_f[params.f]].av_F);

    cout << endl;
    cout << "============================================================================================" << endl;
    cout << endl;
    print_Node(T,T.N[root_address]);
    cout << "  Stopping condition fulfilled after " << t << " iterations with " << variables.n_leaf_temp << " (temp) & " << variables.n_leaf_term << " (term) leafs and " << n_exp << " expansions" << endl;
    cout << "  Best arm with precision eps = " << params.eps << " and confidence 1-delta = " << 1 - params.delta << " is ";
    if (variables.best_child == params.f) cout << "fs";
    else cout << variables.best_child;
    cout << endl;

    cout << "  Current feature set is = [";
    for (int i=0;i<variables.selected_features.size();i++) {cout << " " << variables.selected_features[i] << " ";} cout << "]"<< endl;
    cout << "  after [";
    for (int i=0;i<variables.itt.size();i++) {cout << " " << variables.itt[i] << " ";} cout << "] iterations" << endl;
    cout << "  with reward [";
    for (int i=0;i<variables.reward.size();i++) {cout << " " << setprecision(4) << variables.reward[i] << " ";} cout << "]" << endl;
    cout << endl << endl;


    return variables.best_child;


    /*
    boost::dynamic_bitset<> F(params.branch);
    F[14]=1;
    string Fl;
    to_string(F, Fl);
    unordered_map<string,int>::const_iterator got = T.N_address.find(Fl); //getting the iterator corresponding to the feature number
    int node_address = got->second; //getting the address from the iterator

    print_Node(T,T.N[node_address]);
    */

    //for (int i=0;i<T.N.size();i++) print_Node(T,T.N[i]);

}

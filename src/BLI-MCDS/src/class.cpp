#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>


//libraries
#include <boost/dynamic_bitset.hpp>

//class
#include "class_Node.h"
#include "class_Params.h"
#include "class_Results.h"
#include "class_Variables.h"
#include "class_Tree.h"


using namespace std;




//--------------------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------ Builder and destroyer of the classes ------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------------------------------------//


//Params
Params::~Params(){}
Params::Params(const double &m_eps, const double &m_delta, const double &m_b, const bool &m_interval, const int &m_branch_i, const int &m_depth_i, const int &m_rseed,
               const double &m_q, const int &m_k, const int &m_m, const double &m_cl, const std::vector< std::vector<float> > &m_L, const bool &m_Reward_policy):
    eps(m_eps), delta(m_delta), b(m_b), interval(m_interval), branch_i(m_branch_i), depth_i(m_depth_i), rseed(m_rseed),
    Reward_policy(m_Reward_policy),q(m_q),k(m_k),m(m_m),cl(m_cl),L(m_L),n(m_L.size()),f(m_L[0].size()-1) {}



//BAI variables
Variables::~Variables(){}
Variables::Variables():
    STOP(false), best_child(-1), n_leaf_term(0), n_leaf_temp(0), n_leaf_temp_tot(0), n_leaf_term_tot(0), n_update(0), it(0){}


//BAI results
Results::~Results(){}
Results::Results(){}



//Node
Node::~Node(){}
Node::Node(const boost::dynamic_bitset<> F, int node_address):
    address(node_address), sub_F(F), F_size(F.count()), is_leaf(false), was_leaf(false), is_root(false), T_F(0),Tt_F(0),av_F(0),U_F(10),L_F(-10), repres_leaf_address(node_address),address_f(F.size()+1,-1),
    updated(0),tobe_updated(0),lRAVE_f(F.size()),repres_child(F.size()-1){}



//Tree
Tree::~Tree(){}
Tree::Tree(int f)
{
    boost::dynamic_bitset<> F(f,0);
    N.push_back(Node(F,0));            //Add the first node to the tree (no feature selected)
    string Fl;
    to_string(F, Fl);
    N_address.insert({Fl,0});         //Add the first address to the tree
}





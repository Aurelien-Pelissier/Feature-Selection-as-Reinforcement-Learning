#include "class_Node.h"
#include "class_Params.h"
#include "class_Tree.h"
#include "class_Variables.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>




//--------------------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------ Builder and destroyer of the classes ------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------------------------------------//


//Params
Params::~Params(){}
Params::Params(const bool &m_pFS, const bool &m_KdTree_policy, const bool &m_Reward_policy, const int &m_Nt, const bool &m_MA, const double &m_q, const int &m_k, const int &m_m, const double &m_r2, const double &m_ce,
                const double &m_c, const double &m_cl, const double &m_b, const vector< vector<float> > &m_L, const unordered_set<int> &m_Features, const int &m_rseed) :
    pFS(m_pFS),KdTree_policy(m_KdTree_policy),Reward_policy(m_Reward_policy),Nt(m_Nt),MA(m_MA),q(m_q),k(m_k),m(m_m),r2(m_r2),ce(m_ce),c(m_c),cl(m_cl),b(m_b),L(m_L),n(m_L.size()),f(m_L[0].size()-1),Features(m_Features),rseed(m_rseed){}


//variables
Variables::~Variables(){}
Variables::Variables():
    time(0), reward_V(1,0), Depth(1,0), reward_V2(1,0), Depth2(1,0){}


//Node
Node::~Node(){}
Node::Node(boost::dynamic_bitset<> F, int node_address) :
    address(node_address), sub_F(F), F_size(F.count()), T_F(0), Node_av(0), N_final(0), t_f(F.size()+1), mu_f(F.size()+1), sg_f(F.size()+1), lRAVE_f(F.size()){}



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





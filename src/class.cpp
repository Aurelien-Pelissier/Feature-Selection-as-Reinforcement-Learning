#include "class_Node.h"
#include "class_Params.h"
#include "class_Tree.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>




//--------------------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------ Builder and destroyer of the classes ------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------------------------------------//


//Params
Params::~Params(){}
Params::Params(const int &m_Nt, const bool &m_MA, const double &m_q, const int &m_k, const int &m_r, const double &m_ce, const double &m_c, const double &m_cl, const double &m_b, const vector< vector<float> > &m_L) :
    Nt(m_Nt),MA(m_MA),q(m_q),k(m_k),r(m_r),ce(m_ce),c(m_c),cl(m_cl),b(m_b),L(m_L){}



//Node
Node::~Node(){}
Node::Node(boost::dynamic_bitset<> F, int node_address) :
    address(node_address), sub_F(F), F_size(F.count()), T_F(0),av_F(0),sg_F(0),Score(0),fs(3),fr(3),address_f(F.size()-1,-1),lRAVE_f(F.size()),already_updated(0){}



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





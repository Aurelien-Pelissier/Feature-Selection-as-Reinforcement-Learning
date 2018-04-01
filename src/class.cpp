#include "class_Node.h"
#include "class_Rave.h"
#include "class_Params.h"
#include "class_Tree.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

//Builder and destroyer of the classes


//Params
Params::~Params(){}
Params::Params(const bool &m_MA, const double &m_q, const int &m_k, const double &m_ce, const double &m_c, const double &m_cl, const double &m_b, const vector< vector<float> > &m_L) : MA(m_MA),q(m_q),k(m_k),ce(m_ce),c(m_c),cl(m_cl),b(m_b),L(m_L){}


//Node
Node::~Node(){}
Node::Node(vector<bool> F) : sub_F(F), T_F(0),t_f(F.size()),mu_f(F.size()),sg_f(F.size()),lRAVE_f(F.size()),Node_av(0),Node_Score(0),N_final(0)
{
    nF = 0;
    for (int k=0; k<F.size(); k++)
    {   nF += F[k]*pow(2,F.size()-k-1);   }

    F_size = count_if (F.begin(), F.end(), [](bool i) { return (i==1); });
}



//Tree
Tree::~Tree(){}
Tree::Tree(int f)
{
    vector<bool> F(f,0);
    N.push_back(Node(F));            //Add the first node to the tree (no feature selected)
    N_adress.insert({F,0});         //Add the first address to the tree
}

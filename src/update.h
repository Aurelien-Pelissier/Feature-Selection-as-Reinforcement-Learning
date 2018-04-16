#ifndef INCLUDED_UPDATEH
#define INCLUDED_UPDATEH

#include <vector>
#include <unordered_map>

//using boost library
#include <boost/dynamic_bitset.hpp>

//including classes
#include "class_Node.h"
#include "class_Path.h"
#include "class_Tree.h"


//Update
void                                  Update_gRAVE(std::vector <std::pair<double,int>> &gRAVE, const boost::dynamic_bitset<> &F, const double &reward_V);
int                                   Update_Tree_And_Get_Adress(Tree &T, const boost::dynamic_bitset<> &F);
void                                  Add_Familly(Tree &T, const boost::dynamic_bitset<> F, const int &node_address);
void                                  Update_Node(Node &node, const int fi, const boost::dynamic_bitset<> &Ft, const double &reward_V);
void                                  Update_av_var(double &N_IT, double &AV, double &SG, const double &xN1);
void                                  Update_av_var(int &N_IT, double &AV, double &SG, const double &xN1);

void                                  Update_parents(Tree &T, Node &node, int &fi, const boost::dynamic_bitset<> &Ft, double &reward_V);

#endif

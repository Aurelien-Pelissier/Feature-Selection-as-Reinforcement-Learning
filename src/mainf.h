#ifndef INCLUDED_MAINH
#define INCLUDED_MAINH


#include <vector>
#include <unordered_map>

//using boost library
#include <boost/dynamic_bitset.hpp>

//including classes
#include "class_Node.h"
#include "class_Params.h"
#include "class_Tree.h"



//main
void                                  Run_feature_selection(Params &params);
double                                reward(const boost::dynamic_bitset<> &F, const Params &params);
double                                iterate_random(Tree &T, boost::dynamic_bitset<> &F, const Params &params);
double                                iterate(Tree &T, std::vector <std::pair<double,int>> &gRAVE, boost::dynamic_bitset<> &F, const Params &params, int &depth);
int                                   Discrete_UCB(Node &node, const std::vector <std::pair<double,int>> &gRAVE, const Tree &T, const Params &params);
int                                   Continuous_UCB(Node &node, const std::vector <std::pair<double,int>> &gRAVE, const Tree &T, const Params &params);


#endif

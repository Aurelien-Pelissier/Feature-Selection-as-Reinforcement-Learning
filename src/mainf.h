#ifndef INCLUDED_MAINH
#define INCLUDED_MAINH


#include <vector>
#include <unordered_map>

//including classes
#include "class_Node.h"
#include "class_Params.h"
#include "class_Tree.h"



//main
double                                reward(const std::vector< std::vector<float> > &L, const std::vector<bool> &F);
int                                   iterate_random(Tree &T, std::vector<bool> &F, const Params &params);
double                                iterate(Tree &T, std::vector <std::pair<double,int>> &gRAVE, std::vector<bool> &F, const Params &params);
int                                   Discrete_UCB(Node &node, const std::vector <std::pair<double,int>> &gRAVE, const Params &params);
int                                   Continuous_UCB(Node &node, const std::vector <std::pair<double,int>> &gRAVE, const Params &params);


#endif

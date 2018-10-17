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
#include "class_Variables.h"


//main
void                                  Run_feature_selection(Params &params);
void                                  Run_FUSE(Tree &T, std::vector <std::pair<double,int>> &gRAVE, Params &params, boost::dynamic_bitset<> &F, Variables &variables, int it);
double                                reward(const boost::dynamic_bitset<> &F, const Params &params);
double                                reward_full(const boost::dynamic_bitset<> &F, const Params &params);
int                                   rand_int(int a, int b, int rseed);
std::vector<std::vector<float>>       subsample(const std::vector<std::vector<float>> &L, const int &n, const int &f, const int &m, const int &rseed);
double                                iterate_random(Tree &T, boost::dynamic_bitset<> &F, const Params &params);
double                                iterate(Tree &T, std::vector <std::pair<double,int>> &gRAVE, boost::dynamic_bitset<> &F, const Params &params, int &depth);
int                                   Discrete_UCB(Node &node, const std::vector <std::pair<double,int>> &gRAVE, const Tree &T, const Params &params);
int                                   Continuous_UCB(Node &node, const std::vector <std::pair<double,int>> &gRAVE, const Tree &T, const Params &params);
bool                                  get_answer();


#endif

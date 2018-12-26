#ifndef INCLUDED_REWARDH
#define INCLUDED_REWARDH


#include <vector>
#include <unordered_map>

//using boost library
#include <boost/dynamic_bitset.hpp>

//including classes
#include "class_Node.h"
#include "class_Params.h"
#include "class_Tree.h"




int                                     rand_int(int a, int b, const int &rseed);
std::vector<std::vector<float>>         subsample(const std::vector<std::vector<float>> &L, const int &n, const int &d, const int &m, const int &rseed);
double                                  reward(const boost::dynamic_bitset<> &F, const Params &params);
double                                  reward_full(const boost::dynamic_bitset<> &F, const Params &params);

#endif

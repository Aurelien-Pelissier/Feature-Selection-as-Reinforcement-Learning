#ifndef INCLUDED_REWARDH
#define INCLUDED_REWARDH

#include <map>
#include <vector>

//using boost library
#include <boost/dynamic_bitset.hpp>
#include "kdtree/alglibmisc.h"
#include "kdtree/stdafx.h"

//including classes
#include "class_Params.h"



//main
std::multimap <int, float>             naive_kNN(const std::vector<std::vector<float>> &L, const std::vector<std::vector<float>> &V, const boost::dynamic_bitset<> &F, const Params &params);
std::multimap <int, float>             KD_tree_kNN(const alglib::real_2d_array &La, const std::vector<std::vector<float>> &V, const boost::dynamic_bitset<> &F, const Params &params);

#endif

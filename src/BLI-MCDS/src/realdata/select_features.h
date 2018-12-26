#ifndef INCLUDED_SFH
#define INCLUDED_SFH


#include <vector>
#include <unordered_set>

//libraries
#include <boost/dynamic_bitset.hpp>



//main
std::vector<std::vector<float>>                  select_features(const std::vector<std::vector<float>> &L, const int &n, const int &f, const std::unordered_set<int> &Features);
std::vector<std::vector<float>>                  select_features(const std::vector<std::vector<float>> &L, const int &n, const int &f, const boost::dynamic_bitset<> &F);

#endif

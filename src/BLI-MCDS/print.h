#ifndef INCLUDED_PRINTH
#define INCLUDED_PRINTH

#include <string>
#include <vector>
#include <fstream>
#include <chrono>

//using boost library
#include <boost/dynamic_bitset.hpp>

//including class
#include "class_Node.h"
#include "class_Tree.h"
#include "class_Params.h"
#include "class_Variables.h"


//functions used to print
void                                    print_features(const boost::dynamic_bitset<> &F);
void                                    print_Node(const Tree &T, const Node &node);
void                                    print_score(const std::vector<double> &score, int f);

void                                    print_results(const Tree &T, const Params &params, const Variables &variables);

#endif

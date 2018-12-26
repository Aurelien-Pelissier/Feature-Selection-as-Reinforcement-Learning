#ifndef INCLUDED_PRINTH
#define INCLUDED_PRINTH

#include <string>
#include <vector>
#include <fstream>
#include <chrono>

//using boost library
#include <boost/dynamic_bitset.hpp>

//including classes
#include "class_Node.h"
#include "class_Tree.h"
#include "class_Params.h"
#include "class_Variables.h"



//functions used to print
void                                  print_results(const Tree &T, const std::vector <std::pair<double,int>> &gRAVE, const Variables &variables, const Params &params);
void                                  print_params(const Params &params, const Tree &T, const int &time);
void                                  print_features(const boost::dynamic_bitset<> &F);
void                                  print_Tree(const Tree &T);
void                                  print_gRAVE(const std::vector <std::pair<double,int>> &gRAVE);
void                                  print_Node(const Tree &T, const Node &node);
void                                  print_other_nodes(const Tree &T, boost::dynamic_bitset<> &F, const std::vector < std::pair <double, int> > &gRAVE);
void                                  print_text(std::string current_F);

Node                                  Find_Best_Node_Final(const Tree &T);
Node                                  Find_Best_Node_Path(const Tree &T, const Params &params);
Node                                  Find_Best_Node_Path_Av(const Tree &T, const Params &params);



#endif

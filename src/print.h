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


//functions used to print
void                                  print_results(const Tree &T, const std::vector <std::pair<double,int>> &gRAVE, const std::vector <double> &reward_V, const  vector <int> &Depth,
                                            std::ofstream &output_file_1, std::ofstream &output_file_2, std::ofstream &output_file_3,
                                            Params &params, std::chrono::duration<double> &t_elapsed);

void                                  print_params(const Params &params);
void                                  print_features(const boost::dynamic_bitset<> &F);
void                                  print_Tree(const Tree &T);
void                                  print_gRAVE(const std::vector <std::pair<double,int>> &gRAVE);
void                                  print_params(const Params &params, const Tree &T);
void                                  print_Node(const Tree &T, const Node &node);

Node                                  Find_Best_Node_Score(const Tree &T);
Node                                  Find_Best_Node_Final(const Tree &T);
Node                                  Find_Best_Node_Path(const Tree &T);



#endif

#ifndef INCLUDED_PRINTH
#define INCLUDED_PRINTH

#include <string>
#include <vector>
#include <fstream>

//including classes
#include "class_Node.h"
#include "class_Rave.h"


//functions used to print
void                                  print_results(const Tree &T, const std::vector <std::pair<double,int>> &gRAVE, const std::vector <double> &reward_V, std::ofstream &output_file_1, std::ofstream &output_file_2);
void                                  print_dataset(const std::vector< std::vector<float> > &L);
void                                  print_features(const std::vector<bool> &F);
void                                  print_tree_spaced(const Tree &T);
void                                  print_tree(const Tree &T);
void                                  print_gRAVE(const std::vector <std::pair<double,int>> &gRAVE);
void                                  print_Node(const Node &node);
Node                                  Find_Best_Node_Score(const Tree &T);
Node                                  Find_Best_Node_Final(const Tree &T);
Node                                  Find_Best_Node_Path(const Tree &T);



#endif

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
void            Run_BLI_MCLS(const Params &params);
int             Run_BAI_MCLS(Tree &T, const int &root_address, const Params &params, Variables &variables, std::vector<std::pair<double,int>> &gRAVE);

//Build initial Tree
void            Build_initial_graph(Tree &T, std::vector < std::pair <double, int> > &gRAVE, const Params &params, Variables &variables);
void            Complete_current_graph(Tree &T, std::vector<std::pair<double,int>> &gRAVE, const Params &params, Variables &variables, const int &root_address);
void            Add_children_DAG(Tree &T, boost::dynamic_bitset<> &F, int parent_node_address, int fc, const Params &params, Variables &variables, const std::vector<int> &BestRAVE, const int &root_address);

//Expansion
int             Expand_graph(Tree &T, const Params &params, Variables &variables, std::vector<std::pair<double,int>> &gRAVE);
int             Expand_DAG(Tree &T, const Params &params, Variables &variables, std::vector<std::pair<double,int>> &gRAVE);

//Arm pull
int             UGapE(Tree &T, Node &node, const Params &params, Variables &variables);
double          Pull_representative_leaf(Tree &T, Node &node, int &bt);
double          Evaluation(const Tree &T, boost::dynamic_bitset<> &F, const int &leaf_address, const Params &params);
double          Random_phase(boost::dynamic_bitset<> &F, const Params &params);

//Back propagation
void            Backpropagate(Tree &T, Node &node, const int &root_address, const boost::dynamic_bitset<> &Ft, const double &reward_V, const Params &params, const Variables variables);
void            Check_Ancestors(Tree &T, Node &node);
void            Update(Tree &T, Node &node, const boost::dynamic_bitset<> &Ft, const double &reward_V, const Params &params, const Variables variables);
void            Update_Leaf(Tree &T, Node &node, const boost::dynamic_bitset<> &Ft, const double &reward_V, const Params &params, const Variables variables);
void            Update_Node(Tree &T, Node &node, const boost::dynamic_bitset<> &Ft, const double &reward_V);
void            Update_lRAVE(Node &node, const boost::dynamic_bitset<> &Ft, const double &reward_V);
void            Update_av(double &N_IT, double &AV, const double &xN1);

//Update tools
int             Add_Node_And_Get_Adress(Tree &T, const boost::dynamic_bitset<> &F, const Params &params);
void            Add_Familly(Tree &T, const int &node_address, const Params &params, Variables &variables);
void            Check_for_stopping_feature(Tree &T, const int &expand_address, const Params &params, Variables  &variables);
void            Update_node_bounds(Tree &T, Node &node);
void            Update_gRAVE(std::vector < std::pair <double, int> > &gRAVE, const boost::dynamic_bitset<> &F, const double &reward_V);



#endif

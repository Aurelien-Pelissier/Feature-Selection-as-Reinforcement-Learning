#ifndef DEF_NODES
#define DEF_NODES

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

//using boost library
#include <boost/dynamic_bitset.hpp>



class Node
{
    public:

    //Builders & Destroyers
    Node(boost::dynamic_bitset<> F, int node_address);
    ~Node();

    //Attributes
    int address;                                    //Node address in Tree T
    boost::dynamic_bitset<> sub_F;                  //Features subset (binary)
    int F_size;                                     //Number of features in the subset
    int T_F;                                        //Nb of time the node has been visited
    std::unordered_set<int> allowed_features;       //Allowed features (only for Discrete heuristic)
    std::vector<int> t_f;                           //Number of visits of child nodes (+ fr and fs)
    std::vector<double> mu_f;                       //Average of child nodes (+ fr and fs)
    std::vector<double> sg_f;                       //variance of child nodes (+ fr and fs)
    std::vector<std::pair<double, int>> lRAVE_f;    //Local RAVE score of each feature
    double Node_av;                       //Average of all child node
    int N_final;                          //Number of time this node has been chosen final

};




#endif

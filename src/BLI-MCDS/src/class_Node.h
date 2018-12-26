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
    Node(const boost::dynamic_bitset<> F, int node_address);
    ~Node();

    //Attributes
    bool is_root;                                   //boolean that states if the node is root or not
    bool is_leaf;                                   //boolean that states if the node is leaf or not
    bool was_leaf;                                  //boolean that states if the node was leaf or not

    int address;                                    //Node address in Tree T
    boost::dynamic_bitset<> sub_F;                  //Features subset (binary)
    int F_size;                                     //Number of features in the subset
    double T_F;                                     //Nb of time the node has been visited (with exp backpropagation)
    int Tt_F;                                       //Nb of time node has been visited (through UCB phase))
    double av_F;                                    //Average of the node (only for leaf nodes)
    double U_F;                                     //Upper confidence bound
    double L_F;                                     //Lower confidence bound
    int repres_child;                               //representative child of the node
    int repres_leaf_address;                        //Tree address of the representative leaf

    std::unordered_set<int> allowed_features;       //Allowed features (only for Discrete heuristic)
    std::unordered_set<int> parent_address;         //Allowed features (only for Discrete heuristic)
    std::vector<int> address_f;                     //Address of the child nodes
    std::vector<std::pair<double, int>> lRAVE_f;    //Local RAVE score of each feature

    bool updated;        //just a tool for the back-propagation
    bool tobe_updated;   //just a tool for the back-propagation

};


#endif

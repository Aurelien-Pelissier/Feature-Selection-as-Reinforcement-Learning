#ifndef DEF_NODES
#define DEF_NODES

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>



class Node
{
    public:

    //Builders & Destroyers
    Node(std::vector<bool> F);
    ~Node();

    //Attributes
    std::vector<bool> sub_F;              //Features subset
    int nF;                               //number associated to feature subset
    int F_size;                           //number of features in the subset
    int T_F;                              //Nb of time the node has been visited
    std::unordered_set<int> allowed_features;  //Allowed features (only for Discrete heuristic)
    std::vector<int> t_f;                 //Number of time feature has been selected
    std::vector<double> mu_f;              //Average reward for each feature
    std::vector<double> sg_f;              //Variance of each feature
    std::vector<std::pair<double, int>> lRAVE_f;   //Local RAVE score of each feature
    double Node_av;                       //Average of all child node
    double Node_Score;                    //Average when choosing the node in parent node
    int N_final;                          //Number of time this node has been chosen final

    //Methods

};




#endif

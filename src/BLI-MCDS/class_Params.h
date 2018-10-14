#ifndef DEF_PARAMS
#define DEF_PARAMS

#include <vector>

#include "class_Tree.h"


class Params
{
    public:

    //Builders & Destroyers
    Params(const double &m_eps, const double &m_delta, const double &m_b, const bool &m_interval, const int &m_branch_i, const int &m_depth_, const int &m_rseed,
           const double &m_q, const int &m_k, const int &m_m, const double &m_cl, const std::vector< std::vector<float> > &m_L, const bool &m_Reward_policy);
    ~Params();

    //Attributes
    double delta;      //risk
    double eps;        //precision
    double b;          //expansion parameter
    int branch_i;      //initial branching factor of the tree
    int depth_i;         //initial depth of the tree
    bool interval;     //exploration interval definition
    int rseed;         //seed for the random generator of bernouilli distribution

    double q;                    //Random expansion parameter, used to control the average depth in the random phase, |q|<1, high q -> deep exploration
    int k;                       //Number of nearest neighbors involved in the reward function calculation
    int m;                       //Number of element in the subsample for reward calculation
    double cl;                   //l-RAVE/g-RAVE weight
    std::vector< std::vector<float> > L;  //Training set matrix L[n][f+1], n is the number of examples, and f the number of features
    bool Reward_policy;
    int n;
    int f;        //branching factor of the tree

};


#endif

#ifndef DEF_PARAMS
#define DEF_PARAMS

#include <vector>
#include <unordered_set>


class Params
{
    public:

    //Builders & Destroyers
    Params(const bool &m_pFS, const bool &m_KdTree_policy, const bool &m_Reward_policy, const int &m_Nt, const bool &m_MA, const double &m_q, const int &m_k, const int &m_m, const double &m_r2, const double &m_ce, const double &m_c,
            const double &m_cl, const double &m_b, const std::vector< std::vector<float> > &m_L, const std::unordered_set<int> &m_Features, const int &m_rseed);
    ~Params();

    //Attributes
    int Nt;                      //Number of iterations of the simulation
    bool MA;                     //Many-Armed behavior, put 0 for Discrete and 1 for Continuous
    double q;                    //Random expansion parameter, used to control the average depth in the random phase, |q|<1, high q -> deep exploration
    int k;                       //Number of nearest neighbors involved in the reward function calculation
    int m;                       //Number of element in the subsample for reward calculation
    double r2;                      //Ratio of reward training set size / Training set size;
    double ce;                   //UCB exploration control parameter (used in both discrete and continuous heuristic)
    double c;                    //Continuous heuristic exploration parameter 1
    double cl;                   //l-RAVE/g-RAVE weight
    double b;                    //Discrete heuristic exploration parameter, |b|<1
    std::vector< std::vector<float> > L;  //Training set matrix L[n][f+1], n is the number of examples, and f the number of features
    std::unordered_set<int> Features;
    bool pFS;
    bool Reward_policy;
    bool KdTree_policy;
    int rseed;

    int n;
    int f;

};




#endif

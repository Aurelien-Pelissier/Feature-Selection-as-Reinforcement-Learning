#ifndef DEF_PARAMS
#define DEF_PARAMS

#include <vector>


class Params
{
    public:

    //Builders & Destroyers
    Params(const int &m_Nt, const bool &m_MA, const double &m_q, const int &m_k, const int &m_r, const double &m_ce, const double &m_c, const double &m_cl, const double &m_b, const std::vector< std::vector<float> > &m_L);
    ~Params();

    //Attributes
    int Nt;                      //Number of iterations of the simulation
    bool MA;                     //Many-Armed behavior, put 0 for Discrete and 1 for Continuous
    double q;                    //Random expansion parameter, used to control the average depth in the random phase, |q|<1, high q -> deep exploration
    int k;                       //Number of nearest neighbors involved in the reward function calculation
    int r;                       //Ratio of aggressive subsample size / Training set size
    double ce;                   //UCB exploration control parameter (used in both discrete and continuous heuristic)
    double c;                    //Continuous heuristic exploration parameter 1
    double cl;                   //Continuous heuristic exploration parameter 2
    double b;                    //Discrete heuristic exploration parameter, |b|<1
    std::vector< std::vector<float> > L;  //Training set matrix L[n][f+1], n is the number of examples, and f the number of features

};




#endif

#ifndef DEF_RESULTS
#define DEF_RESULTS


#include "class_Tree.h"


class Results
{
    public:

    //Builders & Destroyers
    Results();
    ~Results();

    //Attributes
    std::vector<int> best_arm;
    std::vector<int> iterations;
    std::vector<int> updates;
};



#endif

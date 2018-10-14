#ifndef DEF_VARIABLES
#define DEF_VARIABLES

#include <vector>

#include "class_Tree.h"


class Variables
{
    public:

    //Builders & Destroyers
    Variables();
    ~Variables();

    //Attributes
    int time;
    std::vector <double> reward_V;    //contains result after each it
    std::vector <int> Depth;

    std::vector <double> reward_V2;   //keep track of current best feature set
    std::vector <int> Depth2;
};



#endif

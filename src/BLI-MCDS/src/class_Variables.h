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
    bool STOP;         //when stopping condition is fulfilled
    int best_child;    //store the recommended best child
    int n_leaf_temp;   //current number of temporary leaf in the current tree
    int n_leaf_term;   //current number of terminal leaf in the current tree
    int n_leaf_temp_tot;   //current number of temporary leaf in the total tree
    int n_leaf_term_tot;   //current number of terminal leaf in the total tree
    int n_update;      //number of updates during the search
    int it;            // number of total iterations
    std::vector<double> reward;
    std::vector<int> itt;
    std::vector<int> selected_features;
};



#endif

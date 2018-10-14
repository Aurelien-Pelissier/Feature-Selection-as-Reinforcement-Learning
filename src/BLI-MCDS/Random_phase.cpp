#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>

//including functions
#include "functions.h"
#include "realdata/reward.h"

//including class
#include "class_Node.h"





double  Random_phase(boost::dynamic_bitset<> &F, const Params &params)
 //Perform a random exploration of the tree, the depth of the exploration is randomly determined by 1-q^|F|
 //(It is possible that no exploration is performed)

{

    int i;
    int zero_i;

    int f = F.size()-1;   //number of features (without the stopping feature)
    int F_size = F.count(); //size of the feature subset (equivalent to the number of 1)


    while (  ((double) rand() / (RAND_MAX)) < pow(params.q,F_size)   )
    {
        if (F_size==f) break; //if all feature are already selected, no need to go further


        //chose a random feature that is not already in the feature subset, and put its value to one (and not the stopping feature)
        i = rand()% (f - F_size);


        zero_i = 0;
        for(int fi = 0; fi < f; fi++ ){
            if ((zero_i == i)&&(!F[fi])){ //= feature fi has been chosen
                F[fi] = 1; //putting its value to one
                break;
            }
            if (!F[fi]){
                zero_i++;
            }
        }
        F_size++;

    }

    F[F.size()-1] = 1; //selecting the stopping feature at the end of the exploration

    return reward(F, params);

}

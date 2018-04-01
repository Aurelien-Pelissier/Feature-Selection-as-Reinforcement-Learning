#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

//including functions
#include "mainf.h"

//including class
#include "class_Node.h"





int  iterate_random(Tree &T, vector<bool> &F, const Params &params)
// Perform a random exploration of the true and return the FIRST randomly selected feature
// The depth of the exploration is randomly determined by 1-q^|F|
// If no exploration is done, the stopping feature is returned

{

    int fi1;
    int i;
    int zero_i;
    bool it_first = true;

    int f = F.size()-1;   //number of features (without the stopping feature)
    int F_size = count_if (F.begin(), F.end(), [](bool i) { return (i==1); }); //size of the feature subset


    while (  ((double) rand() / (RAND_MAX)) < pow(params.q,F_size)   )
    {
        if (F_size==f) break; //if all feature are already selected, no need to go further


        //chose a random feature that is not already in the feature subset, and put its value to one (and not the stopping feature)
        i = rand()% (f - F_size);


        zero_i = 0;
        for(int fi = 0; fi < f; fi++ ){
            if ((zero_i == i)&&(!F[fi])){ //= feature fi has been chosen
                F[fi] = 1; //putting its value to one
                if (it_first) fi1=fi;
                break;
            }
            if (!F[fi]){
                zero_i++;
            }
        }
        F_size++;
        it_first = false;

    }
    if (it_first) fi1=f; //choosing the stopping feature if the tree isn't expanded

    return fi1;

}

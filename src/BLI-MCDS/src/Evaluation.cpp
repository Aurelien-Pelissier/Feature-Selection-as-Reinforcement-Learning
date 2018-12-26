#include <iostream>         //cin & cout
#include <algorithm>        //min, max, fill, find...
#include <fstream>          //files write and read
#include <string>           //string
#include <vector>           //vector
#include <iomanip>          //set precision & set width
#include <bitset>           //for binary numbers manipulation
#include <unordered_map>    //for hash function
#include <chrono>           //to record simulation time
#include <windows.h>        //get key
#include <map>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>  //for binary vectors (couldn't use std::bitset because the size had to be defined at compilation time)

//functions
#include "functions.h"

//class
#include "class_Tree.h"
#include "class_Params.h"


double  Evaluation(const Tree &T, boost::dynamic_bitset<> &F, const int &leaf_address, const Params &params)
// Evaluate the feature subset F

{

    if (leaf_address == -1) return 0;  //if there is no node at this address , the address is -1 and reward 0 is returned

    if (false) return 0.5;


    F = T.N[leaf_address].sub_F;
    if (F[params.f]) //if F is final, the reward is known and the full oracle is called
    {
        return T.N[leaf_address].av_F;
    }
    else  //else we call the intermediate oracle
    {
        return Random_phase(F, params);
    }





}

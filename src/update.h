#ifndef INCLUDED_UPDATEH
#define INCLUDED_UPDATEH

#include <vector>
#include <unordered_map>

//including classes
#include "class_Node.h"
#include "class_Rave.h"
#include "class_Path.h"
#include "class_Tree.h"


//Update
void                                  Update_gRAVE(std::vector <std::pair<double,int>> &gRAVE, const std::vector<bool> &F, const double &reward_V);
int                                   Update_Tree_And_Get_Adress(Tree &T, const std::vector<bool> &F);
void                                  Update_Node(Node &node, const int fi, const std::vector<bool> &Ft, const double &reward_V);

#endif

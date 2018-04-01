#ifndef INCLUDED_DATASETH
#define INCLUDED_DATASETH


#include <string>
#include <vector>



//functions used to handle dataset
std::vector< std::vector<float> >     read_dataset(const std::string &myfile);
std::vector< std::vector<float> >     random_dataset(int n, int f);


#endif

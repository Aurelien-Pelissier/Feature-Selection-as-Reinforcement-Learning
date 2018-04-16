#ifndef INCLUDED_DATASETH
#define INCLUDED_DATASETH


#include <string>
#include <vector>



//functions used to handle dataset
std::vector< std::vector<float> >     read_matrix(const std::string &myfile);
std::vector< std::vector<float> >     concatenate(const std::vector< std::vector<float> > &Data, const std::vector< std::vector<float> > &Labels);
std::vector< std::vector<float> >     linear_dataset(int n, int f);
std::vector< std::vector<float> >     random_dataset(int n, int f);
void                                  print_dataset(const std::vector< std::vector<float> > &L);


#endif

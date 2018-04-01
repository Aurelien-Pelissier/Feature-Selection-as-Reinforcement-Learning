#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;




vector< vector<float> > read_dataset(const string &myfile)
// return the training set array L[n][f+1] read from myfile:
{
    vector< vector<float> > L;
    fstream in(myfile);
    string line;
    int i = 0;

    while (getline(in, line))
    {
        float value;
        stringstream ss(line);

        L.push_back(vector<float>());

        while (ss >> value)
        {
            L[i].push_back(value);
        }
        ++i;
    }

    //putting the outcome (2nd position) on the last column
    int y;
    for(int i = 0; i < L.size(); i++ ){
        y = L[i][1];
        for(int j = 1; j < L[0].size()-1; j++ ){
            L[i][j] = L[i][j+1];
        }
        L[i][L[0].size()-1] = y;
   }


    return L;
}



vector< vector<float> > random_dataset(int n, int f)
// fill the training set L[n][f+1] with random values:
    //features are random numbers between 0 and 9
    //The classification is 0 or 1
{
    vector< vector<float> > L;

    for(int i=0;i<n;i++){
        L.push_back(vector<float>());
        for(int j=0;j<f;j++){
            L[i].push_back(rand()% 10);
        }
        L[i].push_back(rand()% 2);
    }

    return L;
}

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;


#include "dataset.h"




//=============================================================================================================================//
//=================================================== Reading matrix from file ================================================//
//=============================================================================================================================//

vector< vector<float> > read_matrix(const string &myfile)
// return a float array L[n_rows][n_column] read from myfile, the delimiter is space:
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
    in.close();
    return L;
}








//=============================================================================================================================//
//======================================= Concatenate the Data-set and Label-set together =====================================//
//=============================================================================================================================//

vector< vector<float> > concatenate(const vector< vector<float> > &Data, const vector< vector<float> > &Labels)
//Return the training set L[n][f+1]
{
    vector < vector<float> > L;
    int n = Data.size();
    int f = Data[0].size();


    if (n != Labels.size())
    {
        cout << "  Concatenate Error: the two matrix should have the same number of examples" << endl;
        cout << "     Data have size " << n << "and labels " << Labels.size() << endl << endl;
        exit(1);
    }


    for(int i=0;i<n;i++){
        L.push_back(vector<float>());
        for(int j=0;j<f;j++){
            L[i].push_back(Data[i][j]);
        }
        L[i].push_back(Labels[i][0]);
    }

    return L;
}







//=============================================================================================================================//
//=============================================== Merge two training set together =============================================//
//=============================================================================================================================//

void   merge_matrix(vector< vector<float> > &L, const vector< vector<float> > &L2)
//append the matrix L2 to the matrix L (L is modified)
{

    int n = L.size();
    int n2 = L2.size();
    int f = L[0].size();
    if (f != L2[0].size())
    {
        cout << "  Merge Error: the two matrix should have the same number of features" << endl;
        cout << "     first matrix has " << f << " features and second one has " << L2[0].size() << " features" << endl << endl;
        exit(1);
    }

    for(int i=n; i<(n2+n); i++){
        L.push_back(vector<float>());
        for(int j=0;j<f;j++){
            L[i].push_back(L2[i-n][j]);
        }
    }

}





//=============================================================================================================================//
//====================================================== Linear dataset =======================================================//
//=============================================================================================================================//

vector< vector<float> > linear_dataset(int n, int f, int nr)
{
    srand(10);
    // fill the training with 3 relevant feature, nr redundant features, and set the remaining with random values:
    // return the training set L[n][f+1] with n the number of training examples and f the number of features

    if (f<=nr)
    {
        cout << "Please enter a number of feature higher than nr" << endl;
        exit(1);
    }

    if (nr>30)
    {
        cout << "Please enter a number of redundant feature lower than 30" << endl;
        exit(1);
    }

    vector< vector<float> > L;
    float a = 0.2f;
    float b = -0.8f;
    float c = 0.6f;


    float x,y,z;
    vector<float> fr(27);

    for (int i=0;i<n;i++)
    {
        L.push_back(vector<float>());

        //good features
        x = ((float) rand() / (RAND_MAX));
        y = ((float) rand() / (RAND_MAX));
        z = ((float) rand() / (RAND_MAX));
        L[i].push_back(x);
        L[i].push_back(y);
        L[i].push_back(z);

        //redundant features
        fr[0]  = 3.f*x;
        fr[1]  = 2.f*y;
        fr[2]  = x*z;
        fr[3]  = fabs(x-z);
        fr[4]  = pow(x,z);
        fr[5]  = 2.f*fr[4];
        fr[6]  = fr[4] + 3.f*y;
        fr[7]  = fr[1];
        fr[8]  = 3.f*y;
        fr[9]  = x + y + z;
        fr[10] = fr[0] + 5.0f;
        fr[11] = x*y*z/10.0f;
        fr[12] = fr[3]*fr[5];
        fr[13] = fr[8]/2.3f;
        fr[14] = pow(fr[6],2);
        fr[15] = pow(fr[13],z);
        fr[16] = fabs(x-fr[3]);
        fr[17] = fabs(x-fr[4]);
        fr[18] = x + 10.0f;
        fr[19] = y - 12.0f;
        fr[20] = 2.0f*fr[0] - fr[1];
        fr[21] = fr[20] + fr[4];
        fr[22] = fr[20] + fr[2];
        fr[23] = fr[16];
        fr[24] = fr[16]/fr[8];
        fr[25] = fr[16]/fr[10];
        fr[26] = fr[16]*fr[12]/fr[3];

        for (int j=0;j<nr-3;j++){
        L[i].push_back(fr[j]);
        }

        //random features
        for(int j=nr;j<f;j++){
            L[i].push_back(((float) rand() / (RAND_MAX)));
        }

        //classification result
        if (a*x + b*y + c*z >= 0.f) {L[i].push_back(1.f);}
        else {L[i].push_back(0.f);}
    }

    return L;
}






//=============================================================================================================================//
//==================================================== Fully Random dataset====================================================//
//=============================================================================================================================//


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










//=============================================================================================================================//
//======================================================= Print Dataset =======================================================//
//=============================================================================================================================//


void print_dataset(const vector< vector<float> > &L)
// Prints out matrix elements
{
    cout << endl << "Training set:" << endl;
    cout << "  n = " << L.size() << " examples" << endl;
    cout << "  f = " << L[0].size()-1 << " features" << endl << endl;
    cout << fixed;
    for(int i = 0; i < L.size(); i++ ){
        for(int j = 0; j < L[0].size()-1; j++ ){
            cout << setprecision(1) << L[i][j] << " ";
        }
        cout << setprecision(0) << "   " << L[i][L[0].size()-1]<< endl;
   }
   cout << endl;
   cout << "============================================================================================";
   cout << endl;
}





void   save_dataset(const vector<vector<float>> &L, const string &filename)
//save the training set L in 2 files:
//    - filename.data containing all the examples
//    - filename.labels containing the labels
{
    int n = L.size(); // number of training examples
    int f = L[0].size()-1; //number of features (without the labels)
    string data = filename + string(".data");
    string labels = filename + string(".labels");
    ofstream data_file (data);
    ofstream labels_file (labels);


    freopen(data.c_str(), "w", stdout );  //write the datafile
    for (int ni=0; ni<n; ni++)
    {
        for (int fi=0; fi<f; fi++)
        {
            cout << L[ni][fi] << " ";
        }
        cout << endl;
    }
    data_file.close();


    freopen(labels.c_str(), "w", stdout );  //write the labels file
    for (int ni=0; ni<n; ni++)
    {
        cout << L[ni][f] << endl;
    }
    labels_file.close();


    freopen( "CON", "w", stdout );
}

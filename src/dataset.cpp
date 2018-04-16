#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;





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
//======================================== Concatenate the Data-set and Label-set together ======================================//
//=============================================================================================================================//

vector< vector<float> > concatenate(const vector< vector<float> > &Data, const vector< vector<float> > &Labels)
//Return the training set L[n][f+1]
{
    vector < vector<float> > L;
    int n = Data.size();
    int f = Data[0].size();

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
//====================================================== Linear dataset =======================================================//
//=============================================================================================================================//

vector< vector<float> > linear_dataset(int n, int f)
{
    // fill the training with 3 relevant feature, some redundant features, and set the remaining with random values:
    // return the training set L[n][f+1] with n the number of training examples and f the number of features

    if (f<=8)
    {
        cout << "Please enter a number of feature higher than 7" << endl;
        exit(1);
    }
    vector< vector<float> > L;
    float a = 0.1f;
    float b = -0.8f;
    float c = 0.6f;


    float x,y,z,fr1,fr2,fr3,fr4,fr5;

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
        fr1 = 3.f*x;
        fr2 = 2.f*y;
        fr3 = x*z;
        fr4 = fabs(x-z);
        fr5 = pow(x,z);
        L[i].push_back(fr1);
        L[i].push_back(fr2);
        L[i].push_back(fr3);
        L[i].push_back(fr4);
        L[i].push_back(fr5);

        //random features
        for(int j=8;j<f;j++){
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


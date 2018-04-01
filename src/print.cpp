#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
#include <bitset>
#include <fstream>
#include <unordered_map>
using namespace std;

//including classes
#include "class_Node.h"
#include "class_Tree.h"





void print_dataset(const vector< vector<float> > &L)
// Prints out matrix elements
{
    cout << "Training set" << endl;
    cout << fixed;
    for(int i = 0; i < L.size(); i++ ){
        for(int j = 0; j < L[0].size()-1; j++ ){
            cout << setprecision(0) << L[i][j] << " ";
        }
        cout << setprecision(0) << "   " << L[i][L[0].size()-1]<< endl;
   }
   cout << endl;
}



void print_features(const vector<bool> &F)
// Prints out array elements of the feature subset
{
    cout << "Feature Subset (without fs)" << endl;
    for(int i = 0; i < F.size()-1; i++ ){
    cout << F[i] << " ";}
    cout << "   Y";
    cout << endl << endl;
}


void print_tree(const Tree &T)
// Prints out array elements of the search tree
{
    cout << endl << "Search Tree T:" << endl << endl;
    cout << "Address   Features                             T_F    N_final" << endl;
    for(int i = 0; i < T.N.size(); i++ ){
        cout << " " << setw(7) << std::setfill('0') << i << "    ";
        cout << bitset<int(31)> (T.N[i].nF) << " ";
        cout << "     " << setw(4) << std::setfill('0') << T.N[i].T_F;
        cout << "     " << T.N[i].N_final << endl;
    }
    cout << endl;

}


void print_tree_spaced(const Tree &T)
// Prints out array elements of the search tree (spaced using vector)
{
    cout << endl << "Search Tree T:" << endl << endl;
    cout << "Address  Features                                                          T_F" << endl;
    for(int i = 0; i < T.N.size(); i++ ){
        cout << setw(7) << std::setfill('0') << i << "    ";
        for(int j = 0; j < T.N[i].sub_F.size(); j++ ){
            cout << T.N[i].sub_F[j] << " ";
        }
        cout << "     " << T.N[i].T_F << endl << endl;
    }
    cout << endl << endl;
}



void print_gRAVE(const vector < pair <double, int> > &gRAVE)
{
    cout << fixed;
    cout << endl << "gRAVE score:" << endl << endl;
    cout << "feature     score      Ng" << endl;
    for(int i = 0; i < gRAVE.size(); i++ ){
        if (i == gRAVE.size()/2)
        {
            cout << endl << "stopping features :" << endl;
        }
        cout << "   " << setw(2) << std::setfill('0') << i << "       " << setprecision(4) << gRAVE[i].first << "     " << gRAVE[i].second << endl;
    }
    cout << endl << endl << endl;


}




void print_Node(const Node &node)
// Print Node
{
    cout << "Node with feature subset:" << endl << " " << bitset<int(32)> (node.nF) << endl;
    cout << " Feature subset size = " << node.F_size << endl;
    cout << " Node Average = " << node.Node_av << endl;
    cout << " Node Score = " << node.Node_Score << endl;
    cout << " Node Final = " << node.N_final << endl;
    cout << " T_F = " << node.T_F << endl << endl;
    cout << "Child Nodes:" << endl;
    cout << "feature    t_f     mu_f     sg_f      lRAVE_f    tl_f" << endl;
    for(int i = 0; i < node.t_f.size()-1; i++ )
    {
        cout << "   " << setw(2) << std::setfill('0') << i << "      " << setw(4) << std::setfill('0') << node.t_f[i] << "    " <<  node.mu_f[i] << "   " << node.sg_f[i] << "    " << node.lRAVE_f[i].first << "     " << node.lRAVE_f[i].second  << endl;
    }
    cout << endl;
    cout << "   " << "fs" << "      " << setw(4) << std::setfill('0') << node.t_f[node.t_f.size()-1] << "    " <<  node.mu_f[node.t_f.size()-1];
    cout << "   " << node.sg_f[node.t_f.size()-1] << "    " << node.lRAVE_f[node.t_f.size()-1].first << "     " << node.lRAVE_f[node.t_f.size()-1].second  << endl;
    cout << endl << endl <<endl;
}




Node Find_Best_Node_Score(const Tree &T)
 //Find Node with highest score
{
    cout << "Node with best average " << endl << endl;
    int indexb = 0;
    double maxScore = 0;
    for (int i=0; i<T.N.size(); i++)
    {
        if (T.N[i].Node_Score > maxScore)
        {
            maxScore = T.N[i].Node_Score;
            indexb = i;
        }
    }
    return T.N[indexb];

}



Node Find_Best_Node_Final(const Tree &T)
 //Find the most selected final node
{
    cout << "Most selected final " << endl << endl;
    int indexb = 0;
    double maxScore = 0;
    for (int i=0; i<T.N.size(); i++)
    {
        if (T.N[i].N_final > maxScore)
        {
            maxScore = T.N[i].N_final;
            indexb = i;
        }
    }
    return T.N[indexb];

}



Node Find_Best_Node_Path(const Tree &T)
 //Find the node at the end of the most visited path (has to be final)
{
    vector<bool>F = T.N[0].sub_F;
    int node_adress = 0;
    int fi;

    cout << "Most visited path:" << endl << " [";
    while (!F.back())
    {
        fi =  distance(T.N[node_adress].t_f.begin(), max_element(T.N[node_adress].t_f.begin(), T.N[node_adress].t_f.end())); //correspond to argmax, choosing the most visited child node
        cout << " " << fi << "-" << T.N[node_adress].t_f[fi] << " ";
        if (fi==F.size()-1) break;
        F[fi] = 1;

        unordered_map<vector<bool>,int>::const_iterator got = T.N_adress.find(F);
        if ( got == T.N_adress.end() ) break;
        else node_adress = got->second;  //getting the address of the new feature subset


    }
    cout << "]" << endl << endl;
    return T.N[node_adress];

}





void print_results(const Tree &T, const vector <pair<double,int>> &gRAVE, const vector <double> &reward_V, ofstream &output_file_1, ofstream &output_file_2)
// Prints out the results of the simulations
{

    print_tree(T);
    print_gRAVE(gRAVE);
    print_Node(T.N[0]);
    print_Node(Find_Best_Node_Score(T));
    print_Node(Find_Best_Node_Final(T));
    print_Node(Find_Best_Node_Path(T));


    if (output_file_1.is_open())
    {
        freopen( "Output_Tree.txt", "w", stdout );
        print_tree(T);
        print_gRAVE(gRAVE);
        print_Node(T.N[0]);
        print_Node(Find_Best_Node_Score(T));
        print_Node(Find_Best_Node_Final(T));
        print_Node(Find_Best_Node_Path(T));
        output_file_1.close();
    }
    else cout << "Unable to open Output_Tree.txt" << endl;



    if (output_file_2.is_open())
    {
        freopen( "Output_Reward.txt", "w", stdout );

        cout << "Computed reward for each iterations" << endl << endl;
        for (int i=0; i<reward_V.size(); i++)
        {
            cout << setw(5) << std::setfill('0') << i << "     " << setprecision(4) << reward_V[i] << endl;
        }

        output_file_2.close();
    }
    else cout << "Unable to open Output_Reward.txt" << endl;
}

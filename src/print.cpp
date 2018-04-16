#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <map>
#include <chrono>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>

//including functions
#include "dataset.h"

//including classes
#include "class_Node.h"
#include "class_Tree.h"
#include "class_Params.h"





void print_params(const Params &params, const Tree &T)
// Prints out matrix elements
{
    double cl2 = params.cl;
    if (params.cl >= 10*params.Nt) cl2 = 1.0/0.0; //inf

    cout << fixed;

    cout << "  Simulation parameters:" << endl;
    cout << "      Number of Iterations                  Nt = " << params.Nt << endl;
    cout << "      Random exploration:                   q = " << setprecision(2) << params.q << endl;
    cout << "      Number of nearest neighbors:          k = " << setw(2) << std::setfill('0') << params.k << endl;
    cout << "      Training_set/Aggressive_set ratio     r = " << setw(2) << std::setfill('0') << params.r << endl;
    cout << "      UCB exploration parameter:            ce = " << setprecision(2) << params.ce << endl;
    cout << "      g-RAVE/l-RAVE weight:                 cl = " << setprecision(2) << cl2 << endl;
    cout << "      Exploration heuristic:                ";
    if (params.MA == 0)
    {
        cout << "DISCRETE" << endl;
        cout << "             Allowed feature parameter:       b = " << setprecision(2)<< params.b << endl;
    }
    else
    {
        cout << "CONTINUOUS" << endl;
        cout << "             RAVE weight                      c = " << setprecision(2)<< params.c << endl;
    }
    cout << endl;
    cout << "  Final Tree length:     |T| = " << T.N.size() << endl;
    cout << endl;
    cout << "============================================================================================";
    cout << endl;
}



void print_features(const boost::dynamic_bitset<> &F)
// Prints out array elements of the feature subset
{
    cout << "Feature Subset (with fs)" << endl;
    for(int i = 0; i < F.size(); i++ ){
    cout << F[i] << " ";}
    cout << endl << endl;
}


void print_Tree(const Tree &T)
// Prints out array elements of the search tree
{
    cout << endl << "Search Tree T:" << endl << endl;
    cout << "Address   Features                             T_F    N_final" << endl;
    string Fset;
    for(int i = 0; i < T.N.size(); i++ ){
        cout << " " << setw(8) << std::setfill('0') << i << "    ";
        to_string(T.N[i].sub_F,Fset);
        reverse(Fset.begin(), Fset.end());
        cout << Fset << " ";
        cout << "     " << setw(8) << std::setfill('0') << T.N[i].T_F;
        cout << "     " << (int)T.N[i].fs[0] << endl;
    }
    cout << endl;
    cout << "============================================================================================";
    cout << endl;

}



void print_gRAVE(const vector < pair <double, int> > &gRAVE)
{
    cout << fixed;
    cout << endl << "gRAVE score:" << endl << endl;
    cout << "feature     score      Ng" << endl;
    for(int i = 0; i < gRAVE.size()-1; i++ ){
        if (i == gRAVE.size()/2-1)
        {
            cout << endl << "stopping features :" << endl;
        }
        cout << "   " << setw(2) << std::setfill('0') << i << "       " << setprecision(4) << gRAVE[i].first << "     " << gRAVE[i].second << endl;
    }
    cout << endl;
    cout << "============================================================================================";
    cout << endl << endl;


}




void print_Node(const Tree &T, const Node &node)
// Print Node
{
    cout << "Node T[" << node.address << "] with feature subset:" << endl;
    string Fset;   to_string(node.sub_F,Fset);  reverse(Fset.begin(), Fset.end());
    cout << " " << Fset << endl;
    cout << " Feature subset size = " << node.F_size << endl;
    cout << " Node Average = " << node.av_F << endl;
    cout << " Node Variance = " << node.sg_F<< endl;
    cout << " Node Score = " << node.Score << endl;
    cout << " Node Final = " << (int)node.fs[0] << endl;
    cout << " T_F = " << node.T_F << endl << endl;
    cout << "Child Nodes:" << endl;
    cout << "feature     t_f       mu_f     sg_f      lRAVE_f    tl_f" << endl;
    for(int i = 0; i < node.sub_F.size()-1; i++ )
    {
        if (node.address_f[i] != -1)
        {
            Node chnode = T.N[node.address_f[i]];
            cout << "   " << setw(3) << std::setfill('0') << i << "      " << setw(6) << std::setfill('0') << chnode.T_F << "    " << setprecision(4) <<  chnode.av_F;
            cout << "   " << chnode.sg_F << "    " << node.lRAVE_f[i].first << "     " << node.lRAVE_f[i].second  << endl;
        }
    }
    cout << endl;
    cout << "   " << "fs " << "      " << setw(6) << std::setfill('0') << (int)node.fs[0] << "    " <<  node.fs[1];
    cout << "   " << node.fs[2] << "    " << node.lRAVE_f[node.lRAVE_f.size()-1].first << "     " << node.lRAVE_f[node.lRAVE_f.size()-1].second  << endl;
    cout << endl;
    cout << "   " << "fr " << "      " << setw(6) << std::setfill('0') << (int)node.fr[0] << "    " <<  node.fr[1];
    cout << "   " << node.fr[2]  << endl;
    cout << endl;
    cout << "============================================================================================";
    cout << endl << endl << endl;
}




Node Find_Best_Node_Score(const Tree &T)
 //Find Node with highest score
{
    cout << "Node with best average " << endl << endl;
    int indexb = 0;
    double maxScore = 0;
    for (int i=0; i<T.N.size(); i++)
    {
        if (T.N[i].Score > maxScore)
        {
            maxScore = T.N[i].Score;
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
    double maxFinal = 0;
    for (int i=0; i<T.N.size(); i++)
    {
        if (T.N[i].fs[0] > maxFinal)
        {
            maxFinal = T.N[i].fs[0];
            indexb = i;
        }
    }
    return T.N[indexb];

}



Node Find_Best_Node_Path(const Tree &T)
 //Find the node at the end of the most visited path (has to be final)
{
    boost::dynamic_bitset<> F = T.N[0].sub_F;
    int f = F.size()-1;
    int node_address = 0;
    int child_address;
    map <int, int>  TF;
    int fi;
    int tf;

    cout << "Most visited path:" << endl << " [";
    while (!F[f])
    {
        TF.clear();
        TF.insert({(int)T.N[node_address].fs[0], f});
        int tfi = 0;
        for (int i=0; i<f ;i++)
        {
            child_address = T.N[node_address].address_f[i];
            if (child_address != -1)
            {
                tfi = T.N[child_address].T_F;
                TF.insert({tfi, i});
            }
        }

        //choosing the most visited child node
        tf = TF.rbegin()->first;
        fi = TF.rbegin()->second;
        if (fi!=f)
        {
            node_address = T.N[node_address].address_f[fi];
            cout << " " << fi << "-" << tf << " ";
        }
        else
        {
            cout << " " << "fs" << "-" << tf << " ";
            break;
        }
        F[fi] = 1;
    }
    cout << "]" << endl << endl;
    return T.N[node_address];

}









void print_results(const Tree &T, const vector <pair<double,int>> &gRAVE, const vector <double> &reward_V, const  vector <int> &Depth,
                   ofstream &output_file_1, ofstream &output_file_2, ofstream &output_file_3, Params &params, std::chrono::duration<double> &t_elapsed)
// Prints out the results of the simulations in console and different files
{






    //----------------------------------------- Print on console -----------------------------------------//

    print_gRAVE(gRAVE);
    print_Node(T, T.N[0]);

    Node N_best_score = Find_Best_Node_Score(T);
    print_Node(T, N_best_score);

    Node N_best_final = Find_Best_Node_Final(T);
    print_Node(T, N_best_final);

    Node N_best_path = Find_Best_Node_Path(T);
    print_Node(T, N_best_path);




    //--------------------------------------- Print simulation file ---------------------------------------//

    if (output_file_1.is_open())
    {
        freopen( "Output_Tree.txt", "w", stdout );
        cout << endl;
        cout << "===============================================================================================" << endl;
        cout << "====================================== FEATURE SELECTION ======================================" << endl;
        cout << "===============================================================================================" << endl << endl;
        print_params(params,T);
        print_dataset(params.L);
        print_gRAVE(gRAVE);
        print_Node(T, T.N[0]);

        Node N_best_score = Find_Best_Node_Score(T);
        print_Node(T, N_best_score);

        Node N_best_final = Find_Best_Node_Final(T);
        print_Node(T, N_best_final);

        Node N_best_path = Find_Best_Node_Path(T);
        print_Node(T, N_best_path);

        print_Tree(T);
        output_file_1.close();
    }
    else cout << "Unable to open Output_Tree.txt" << endl;




    //--------------------------------------- Print Reward file ---------------------------------------//

    if (output_file_2.is_open())
    {
        freopen( "Output_Reward.txt", "w", stdout );

        cout << "Computed reward for each iterations     &    depth of the search" << endl << endl;
        for (int i=0; i<reward_V.size(); i++)
        {
            cout << setw(5) << std::setfill('0') << i << "     " << setprecision(4) << reward_V[i] << "     " << Depth[i] << endl;
        }

        output_file_2.close();
    }
    else cout << "Unable to open Output_Reward.txt" << endl;





    //--------------------------------------- Append the results ---------------------------------------//

    if (output_file_3.is_open())
    {
        FILE* pFile = freopen( "Results.txt", "a", stdout );
        fseek (pFile, 0, SEEK_END);

        if (ftell(pFile) == 0) // if file is empty,
        {
            cout << endl;
            cout << "==========================================================================================================================================================" << endl;
            cout << "=====================================================   FEATURE SELECTION   -   SIMULATION RESULTS   =====================================================" << endl;
            cout << "==========================================================================================================================================================" << endl << endl;

            cout << "Parameters: " << endl;
            cout << "   f = number of features," << endl;
            cout << "   n = number of example in the training set," << endl;
            cout << "   Nt = number of iteration in the simulation," << endl;
            cout << "   t = computation time [in s]," << endl;
            cout << "   Tree = size of the tree at the end of computation" << endl;
            cout << "   reward = computed reward for the last 1000 iteration," << endl;
            cout << "   depth = computed depth search for the last 1000 iterations," << endl;
            cout << "   N_final = number of time best node has been chosen final," << endl << endl;

            cout << "   --> For the other parameters, please refer to the implementation details." << endl << endl << endl;


            cout << "==========================================================================================================================================================" << endl;
            cout << "|| ---- Main parameters ---- ||  -------------  Simulation results   ------------- || --------------  Other parameters of the simulation -------------- ||" << endl;
            cout << "==========================================================================================================================================================" << endl;
            cout << "f        n          Nt          t[s]     Tree        reward     depth       N_final     q        k      r      ce       cl            MA    b        c" << endl;
        }

        double reward = 0; //average of the 1000 last calculated reward
        double depth = 0; //average of the 1000 last calculated depth
        for (int i=0;i<1000;i++)
        {
            reward = reward + reward_V[reward_V.size()-i-1];
            depth = depth + Depth[Depth.size()-i-1];
        }
        reward = reward/1000.0;
        depth = (double)depth/1000.0;

        cout << setw(4) << std::setfill('0') << params.L[0].size() << "     ";
        cout << setw(6) << std::setfill('0') << params.L.size() << "     ";
        cout << setw(7) << std::setfill('0') << params.Nt << "     ";
        cout << setw(4) << std::setfill('0') << (int)t_elapsed.count() << "     ";
        cout << setw(7) << std::setfill('0') << T.N.size() << "     ";
        cout << setprecision(4) << reward << "     ";
        cout << setfill ('0') << setw(7) << depth << "     ";
        cout << setw(7) << std::setfill('0') << (int)N_best_path.fs[0] << "     ";
        cout << setprecision(2) << params.q << "     ";
        cout << setw(2) << std::setfill('0') << params.k << "     ";
        cout << setw(2) << std::setfill('0') << params.r << "     ";
        cout << params.ce << "     ";
        cout << setfill ('0') << setw(9) << fixed << params.cl << "     ";
        cout << params.MA << "     ";
        cout << params.b << "     ";
        cout << params.c;
        cout << endl;

        output_file_3.close();
    }
    else cout << "Unable to open Results.txt" << endl;





    // Back to console - printing other nodes

    freopen( "CON", "w", stdout );
    cout << endl << " Do you want to print another node ?" << endl;
    cout << "     if the entered string is smaller than the number of features," << endl;
    cout << "     it will be filled with zero" << endl;
    cout << "     press ENTER to exit, do not use space" << endl << endl << endl << endl;
    cout << " Enter the node feature subset:" << endl << "     ";

    string Faddress;
    while (getline( cin, Faddress ) && !Faddress.empty())
    {
        cout << endl;

        if (Faddress.size() > T.N[0].sub_F.size())
        {
            cout << endl;
            cout << "     -|-" << endl;
            cout << "     -|-" << endl;
            cout << "     -|-" << endl;
            cout << "     -|-" << endl;
            cout << endl << "Error: the number of feature should be up to " << T.N[0].sub_F.size() << " and you entered " << Faddress.size() << " features" << endl << endl;
            cout << "============================================================================================" << endl << endl << endl;
            cout << " Do you want to print another node ?" << endl;
            cout << "     if the entered string is smaller than the number of features," << endl;
            cout << "     it will be filled with zero" << endl;
            cout << "     press ENTER to exit, do not use space" << endl << endl << endl << endl;
            cout << " Enter the node feature subset:" << endl << "     ";
        }
        else
        {
            if (Faddress.size() < T.N[0].sub_F.size())
            {
                while (Faddress.size() < T.N[0].sub_F.size())
                {
                    Faddress.push_back('0');
                }
            }

            reverse(Faddress.begin(), Faddress.end()); //reversing because the hash function has been set reversely

            unordered_map<string,int>::const_iterator got = T.N_address.find(Faddress); //getting the iterator corresponding to the feature number

            if ( got == T.N_address.end() )
            {
                cout << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << endl << "This feature set has never been visited" << endl << endl;
                cout << "============================================================================================" << endl << endl << endl;
                cout << " Do you want to print another node ?" << endl;
                cout << "     if the entered string is smaller than the number of features," << endl;
                cout << "     it will be filled with zero" << endl;
                cout << "     press ENTER to exit, do not use space" << endl << endl << endl << endl;
                cout << " Enter the node feature subset:" << endl << "     ";
            }
            else
            {
                int node_address = got->second; //getting the address from the iterator
                print_Node(T, T.N[node_address]);
                cout << " Do you want to print another node ?" << endl;
                cout << "     if the entered string is smaller than the number of features," << endl;
                cout << "     it will be filled with zero" << endl;
                cout << "     press ENTER to exit, do not use space" << endl << endl << endl << endl;
                cout << " Enter the node feature subset:" << endl << "     ";
            }

        }

    }












}

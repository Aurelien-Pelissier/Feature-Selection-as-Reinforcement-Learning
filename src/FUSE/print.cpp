#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <map>
#include <set>
#include <chrono>
using namespace std;

//libraries
#include <boost/dynamic_bitset.hpp>

//including functions
#include "print.h"
#include "dataset.h"

//including classes
#include "class_Node.h"
#include "class_Tree.h"
#include "class_Params.h"





void print_params(const Params &params, const Tree &T, const int &time)
// Prints out matrix elements
{

    cout << fixed;

    cout << "  Training set:" << endl << endl;
    cout << "      n = " << params.L.size() << " examples" << endl;
    cout << "      f = " << params.L[0].size()-1 << " features" << endl << endl << endl << endl;

    cout << "  Simulation parameters:" << endl;
    cout << "      Number of Iterations                  Nt = " << params.Nt << endl;
    cout << "      Random exploration:                   q = " << setprecision(2) << params.q << endl;
    cout << "      Number of nearest neighbors:          k = " << setw(2) << std::setfill('0') << params.k << endl;
    cout << "      Aggressive_set size:                  m = " << setw(2) << std::setfill('0') << params.m << endl;
    cout << "      UCB exploration parameter:            ce = " << setprecision(2) << params.ce << endl;
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
    cout << "  Computation time:       t  = " << time << "s" << endl;
    cout << endl;
    cout << "============================================================================================";
    cout << endl << endl << endl;
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
        cout << "     " << (int)T.N[i].N_final << endl;
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
    int fs = node.sub_F.size()-1;

    cout << " Node T[" << node.address << "] with feature subset:" << endl;
    string Fset;   to_string(node.sub_F,Fset);  reverse(Fset.begin(), Fset.end());
    cout << "  " << Fset << endl << endl;
    cout << "  Features = [";
    for (int fi=0;fi<node.sub_F.size();fi++) {if (node.sub_F[fi]==1){cout << " " << fi << " ";}} cout << "]"<< endl;
    cout << "  Feature subset size = " << node.F_size << endl;
    cout << setprecision(4) << "  Node Average = " << node.Node_av << endl;
    cout << "  Node Final = " << node.t_f[fs] << endl;
    cout << "  T_F = " << node.T_F << endl << endl;
    cout << " Child Nodes:" << endl;
    cout << " feature     t_f       mu_f     sg_f     lRAVE_f    tl_f" << endl;

    for ( auto &fi : node.allowed_features ) //computing UCB for each feature
    {
        if (fi != fs)
        {
            cout << "   " << setw(3) << std::setfill('0') << fi << "      " << setw(6) << std::setfill('0') << node.t_f[fi] << "    " << setprecision(4) <<  node.mu_f[fi];
            cout << "   " << node.sg_f[fi] << "    " << node.lRAVE_f[fi].first << "     " << node.lRAVE_f[fi].second  << endl;
        }
    }
    cout << endl;
    cout << "   " << "fs " << "      " << setw(6) << std::setfill('0') << node.t_f[fs] << "    " <<  node.mu_f[fs];
    cout << "   " << node.sg_f[fs] << endl;
    cout << endl;
    cout << "   " << "fr " << "      " << setw(6) << std::setfill('0') << node.t_f.back() << "    " <<  node.mu_f.back();
    cout << "   " << node.sg_f.back() << endl;
    cout << endl;
    cout << "============================================================================================";
    cout << endl << endl << endl;
}





Node Find_Best_Node_Final(const Tree &T)
 //Find the most selected final node
{
    cout << " Most selected final " << endl << endl;
    int indexb = 0;
    double maxFinal = 0;
    int fs = T.N[0].sub_F.size()-1;
    for (int i=0; i<T.N.size(); i++)
    {
        if (T.N[i].t_f[fs] > maxFinal)
        {
            maxFinal = T.N[i].t_f[fs];
            indexb = i;
        }
    }

    return T.N[indexb];

}



Node Find_Best_Node_Path(const Tree &T, const Params &params)
 //Find the node at the end of the most visited path (has to be final)
{
    boost::dynamic_bitset<> F = T.N[0].sub_F;
    vector<int> selected_features;
    int f = F.size()-1;
    int node_address = 0;
    int child_address;
    map <int, int>  TF;
    int fi;
    int tf;

    cout << " Most visited path:" << endl << " [";
    while (!F[f])
    {

        fi =  distance(T.N[node_address].t_f.begin(), max_element(T.N[node_address].t_f.begin(), T.N[node_address].t_f.end())); //correspond to argmax, choosing the most visited child node

        if (fi==f)
        {
            cout << " " << "fs" << "-" << T.N[node_address].t_f[fi] << " ";
            break;
        }

        else if (fi==f+1)
        {
            break;
        }

        else
        {
            cout << " " << fi << "-" << T.N[node_address].t_f[fi] << " ";
        }

        F[fi] = 1;

        string Fl;
        to_string(F, Fl);
        unordered_map<std::string, int>::const_iterator got = T.N_address.find(Fl);
        if ( got == T.N_address.end() ) break;
        else node_address = got->second;  //getting the address of the new feature subset
    }

    cout << "]" << endl << endl;





    if (params.pFS)
    {
        set<int> Features_ordered;
        for (const auto& fi: params.Features)
        {
            Features_ordered.insert(fi);
        }
        set<int>::iterator it;
        int fii, fi;
        cout << "  Correspond to:             = [";
        for (size_t i = 0, maxi = selected_features.size(); i != maxi; i++)
        {
            fii=0;
            fi = selected_features[i];
            for (it = Features_ordered.begin(); it != Features_ordered.end(); it++, fii++)
            {
                if (fii==fi)
                {
                    cout << " " << *it << " ";
                    break;
                }
            }
        }
        cout << "]"<< endl;
    }






    return T.N[node_address];

}




Node Find_Best_Node_Path_Av(const Tree &T, const Params &params)
 //Find the node at the end of the most visited path (has to be final)
{
    boost::dynamic_bitset<> F = T.N[0].sub_F;
    vector<int> selected_features;
    int f = F.size()-1;
    int node_address = 0;
    int child_address;
    map <int, int>  TF;
    int fi;
    int tf;

    cout << " Most visited path with av:" << endl << " [";
    while (!F[f])
    {

        fi =  distance(T.N[node_address].mu_f.begin(), max_element(T.N[node_address].mu_f.begin(), T.N[node_address].mu_f.end())); //correspond to argmax, choosing the most visited child node

        if (fi==f)
        {
            cout << " " << "fs" << "-" << T.N[node_address].mu_f[fi] << " ";
            break;
        }

        else if (fi==f+1)
        {
            break;
        }

        else
        {
            cout << " " << fi << "-" << T.N[node_address].mu_f[fi] << " ";
        }

        F[fi] = 1;

        string Fl;
        to_string(F, Fl);
        unordered_map<std::string, int>::const_iterator got = T.N_address.find(Fl);
        if ( got == T.N_address.end() ) break;
        else node_address = got->second;  //getting the address of the new feature subset
    }

    cout << "]  ->  " << setprecision(4) << T.N[node_address].mu_f[f] << endl << endl;





    if (params.pFS)
    {
        set<int> Features_ordered;
        for (const auto& fi: params.Features)
        {
            Features_ordered.insert(fi);
        }
        set<int>::iterator it;
        int fii, fi;
        cout << "  Correspond to:             = [";
        for (size_t i = 0, maxi = selected_features.size(); i != maxi; i++)
        {
            fii=0;
            fi = selected_features[i];
            for (it = Features_ordered.begin(); it != Features_ordered.end(); it++, fii++)
            {
                if (fii==fi)
                {
                    cout << " " << *it << " ";
                    break;
                }
            }
        }
        cout << "]"<< endl;
    }




    return T.N[node_address];

}









void print_results(const Tree &T, const vector <pair<double,int>> &gRAVE, const Variables &variables, const Params &params)
// Prints out the results of the simulations in console and different files
{

    ofstream output_file_1 ("Output_Tree.txt");    //Output file for the tree
    ofstream output_file_2 ("Output_Reward.txt");  //Output file for the reward
    ofstream output_file_3 ("Results.txt", ios_base::app); //file which append simulation result after each simulations
    ofstream output_file_4 ("Output_Reward2.txt");  //Output file for the reward2




    //----------------------------------------- Print on console -----------------------------------------//

    print_gRAVE(gRAVE);
    print_Node(T, T.N[0]);

    Node N_best_final = Find_Best_Node_Final(T);
    print_Node(T, N_best_final);

    Node N_best_path = Find_Best_Node_Path(T, params);
    print_Node(T, N_best_path);

    cout << "  Final Tree length:     |T| = " << T.N.size() << endl;
    cout << "  Computation time:       t  = " << time << "s" << endl << endl << endl;
    cout << "============================================================================================";

    cout << endl << endl << endl << " Saving result in txt files ..." << endl << endl;




    //--------------------------------------- Print simulation file ---------------------------------------//

    freopen( "CON", "w", stdout );
    cout << "    Saving relevant outputs in \'Output_Tree.txt\' ..." << endl;
    if (output_file_1.is_open())
    {
        freopen( "Output_Tree.txt", "w", stdout );
        cout << endl;
        cout << "===============================================================================================" << endl;
        cout << "====================================== FEATURE SELECTION ======================================" << endl;
        cout << "===============================================================================================" << endl << endl;
        print_params(params,T, variables.time);

        Node N_best_path = Find_Best_Node_Path(T, params);
        print_Node(T, N_best_path);

        Node N_best_final = Find_Best_Node_Final(T);
        print_Node(T, N_best_final);

        print_Node(T, T.N[0]);

        print_gRAVE(gRAVE);
        print_dataset(params.L);
        print_Tree(T);
        output_file_1.close();
    }
    else cout << "Unable to open Output_Tree.txt" << endl;




    //--------------------------------------- Print Reward files ---------------------------------------//

    freopen( "CON", "w", stdout );
    cout << "    Saving Reward in \'Output_Reward.txt\' ..." << endl;
    if (output_file_2.is_open())
    {
        freopen( "Output_Reward.txt", "w", stdout );

        cout << "Computed reward for each iterations     &    depth of the search" << endl << endl;
        for (int i=0; i<variables.reward_V.size(); i++)
        {
            cout << setw(5) << std::setfill('0') << i << "     " << setprecision(4) << variables.reward_V[i] << "     " << variables.Depth[i] << endl;
        }

        output_file_2.close();
    }
    else cout << "Unable to open Output_Reward.txt" << endl;



    freopen( "CON", "w", stdout );
    cout << "    Saving Reward in \'Output_Reward2.txt\' ..." << endl;
    if (output_file_4.is_open())
    {
        freopen( "Output_Reward2.txt", "w", stdout );

        cout << "Computed reward for each iterations     &    depth of the search" << endl << endl;
        for (int i=0; i<variables.reward_V2.size(); i++)
        {
            cout << setw(5) << std::setfill('0') << i << "     " << setprecision(4) << variables.reward_V2[i] << "     " << variables.Depth2[i] << endl;
        }

        output_file_4.close();
    }
    else cout << "Unable to open Output_Reward2.txt" << endl;





    //--------------------------------------- Append the results ---------------------------------------//

    freopen( "CON", "w", stdout );
    cout << "    Adding results to \'Results.txt\' ..." << endl << endl << endl;
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


            cout << "============================================================================================================================================================" << endl;
            cout << "|| ---- Main parameters ---- ||  --------------  Simulation results   -------------- || --------------  Other parameters of the simulation -------------- ||" << endl;
            cout << "============================================================================================================================================================" << endl;
            cout << "f        n          Nt          t[s]       Tree        reward     depth       N_final     q        k      m      ce       cl       MA    b        c" << endl;
        }

        double reward = 0; //average of the 1000 last calculated reward
        double depth = 0; //average of the 1000 last calculated depth
        for (int i=0;i<1000;i++)
        {
            reward = reward + variables.reward_V[variables.reward_V.size()-i-1];
            depth = depth + variables.Depth[variables.Depth.size()-i-1];
        }
        reward = reward/1000.0;
        depth = (double)depth/1000.0;

        cout << setw(4) << std::setfill('0') << params.L[0].size()-1 << "     ";
        cout << setw(6) << std::setfill('0') << params.L.size() << "     ";
        cout << setw(7) << std::setfill('0') << params.Nt << "     ";
        cout << setw(6) << std::setfill('0') << time << "     ";
        cout << setw(7) << std::setfill('0') << T.N.size() << "     ";
        cout << setprecision(4) << reward << "     ";
        cout << setfill ('0') << setw(7) << depth << "     ";
        cout << setw(7) << std::setfill('0') << (int)N_best_path.N_final << "     ";
        cout << setprecision(2) << params.q << "     ";
        cout << setw(2) << std::setfill('0') << params.k << "     ";
        cout << setw(3) << std::setfill('0') << params.m << "     ";
        cout << setprecision(0) << params.ce << "     ";
        cout << setprecision(0) << params.cl << "     ";
        cout << params.MA << "     ";
        cout << setprecision(2) << params.b << "     ";
        cout << setprecision(0) << params.c;
        cout << endl;

        output_file_3.close();
    }
    else cout << "Unable to open Results.txt" << endl;




    // Back to console - printing other nodes

    freopen( "CON", "w", stdout );
    //print_other_nodes(T, N_best_path.sub_F, gRAVE);


}







void  print_other_nodes(const Tree &T, boost::dynamic_bitset<> &F, const vector < pair <double, int> > &gRAVE)
// Prints out other nodes specified by the used
{

    string Faddress;
    string current_F;
    to_string(F, current_F);
    reverse(current_F.begin(), current_F.end());

    cout << endl << endl << endl;
    print_text(current_F);

    while (getline( cin, Faddress ) && !Faddress.empty())
    {

        cout << endl;
        if ( (Faddress[0] == 'g') || (Faddress[1] == 'r') )
        {
            print_gRAVE(gRAVE);
        }

        else if (Faddress[0] == 'p')
        {
            Faddress.erase(0,1);
            int fi = atoi(Faddress.c_str());
            if (F[fi])
            {
                F[fi]=0;
                string Fp;
                to_string(F, Fp);
                unordered_map<string,int>::const_iterator got = T.N_address.find(Fp);
                reverse(Fp.begin(), Fp.end());
                cout << "     You asked for feature set " << Fp << endl << endl << endl;

                if ( got == T.N_address.end() )
                {
                    F[fi]=1;
                    cout << endl;
                    cout << "     -|-" << endl;
                    cout << "     -|-" << endl;
                    cout << "     -|-" << endl;
                    cout << "     -|-" << endl;
                    cout << endl << " This feature set has never been visited" << endl << endl;
                    cout << "============================================================================================" << endl << endl << endl;
                }
                else
                {
                    current_F = Fp;
                    int node_address = got->second; //getting the address from the iterator
                    print_Node(T, T.N[node_address]);
                }
            }

            else
            {
                cout << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << endl << " Error: this cannot be a parent" << endl;
                cout << "============================================================================================" << endl << endl << endl;
            }




        }

        else if (Faddress[0] == 'c')
        {
            Faddress.erase(0,1);
            int fi = atoi(Faddress.c_str());
            if (!F[fi])
            {
                F[fi]=1;
                string Fc;
                to_string(F, Fc);
                unordered_map<string,int>::const_iterator got = T.N_address.find(Fc);
                reverse(Fc.begin(), Fc.end());
                cout << "     You asked feature set " << Fc << endl << endl << endl;
                if ( got == T.N_address.end() )
                {
                    F[fi]=0;
                    cout << endl;
                    cout << "     -|-" << endl;
                    cout << "     -|-" << endl;
                    cout << "     -|-" << endl;
                    cout << "     -|-" << endl;
                    cout << endl << " This feature set has never been visited" << endl << endl;
                    cout << "============================================================================================" << endl << endl << endl;
                }
                else
                {
                    current_F = Fc;
                    int node_address = got->second; //getting the address from the iterator
                    print_Node(T, T.N[node_address]);
                }

            }

            else
            {
                cout << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << endl << " Error: this cannot be a child" << endl;
                cout << "============================================================================================" << endl << endl << endl;
            }




        }




        else
        {

            if (Faddress.size() > T.N[0].sub_F.size())
            {
                cout << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << "     -|-" << endl;
                cout << endl << " Error: the number of feature should be up to " << T.N[0].sub_F.size() << " and you entered " << Faddress.size() << " features" << endl << endl;
                cout << "============================================================================================" << endl << endl << endl;

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
                    cout << endl << " This feature set has never been visited" << endl << endl;
                    cout << "============================================================================================" << endl << endl << endl;


                }
                else
                {
                    current_F = Faddress;
                    reverse(current_F.begin(), current_F.end());
                    int node_address = got->second; //getting the address from the iterator
                    print_Node(T, T.N[node_address]);
                    boost::dynamic_bitset<> F2(Faddress);
                    F = F2;

                }

            }

        }
        print_text(current_F);
    }




}



void print_text(string current_F)
{
    cout << " Last printed node is:  " << current_F << endl;
    cout << "             Features:  ["; for (int i=0; i<current_F.size(); i++) { if (current_F[i] == '1') cout << " " << i << " ";} cout << "]" << endl << endl;
    cout << " Do you want to print another node ?" << endl;
    cout << "     - Enter cfc to access the child node with feature fc (ex: c21)" << endl;
    cout << "     - Enter pfp to access the parent node with feature fp (ex: p17)" << endl;
    cout << "     - Enter a new feature subset to access a different node (ex: 001001)" << endl;
    cout << "       (if the entered string is smaller than the number " << endl;
    cout << "       of features,it will be filled with zero)" << endl << endl;
    cout << "     You can also print gRAVE scores with \'grave\'" << endl << endl;
    cout << "     Press ENTER to exit, do not use space" << endl << endl << endl << endl;
    cout << " Enter the node feature subset:" << endl << "     ";

}

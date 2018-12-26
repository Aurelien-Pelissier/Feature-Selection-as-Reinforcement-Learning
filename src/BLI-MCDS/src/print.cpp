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

//including classes
#include "class_Node.h"
#include "class_Tree.h"
#include "class_Params.h"




void print_features(const boost::dynamic_bitset<> &F)
// Prints out array elements of the feature subset
{
    for(int i = 0; i < F.size(); i++ ){cout << F[i];}
}



void print_Node(const Tree &T, const Node &node)
// Print Node
{
    int f = node.sub_F.size()-1;

    cout << " Node T[" << node.address << "] with feature subset:" << endl;
    string Fset;   to_string(node.sub_F,Fset);  reverse(Fset.begin(), Fset.end());
    cout << "  " << Fset << endl << endl;
    cout << "  Features = [";
    for (int fi=0;fi<node.sub_F.size();fi++) {if (node.sub_F[fi]==1){cout << " " << fi << " ";}} cout << "]"<< endl;
    cout << "  Feature subset size = " << node.F_size << endl;
    cout << "  Is root ? " << node.is_root << endl;
    cout << "  Is leaf ? " << node.is_leaf << endl;
    cout << "  Was leaf ? " << node.was_leaf << endl;
    cout << "  av_F = " << setprecision(4) << node.av_F << endl << endl;

    cout << "  T_F = " << (int)node.T_F << endl;
    cout << "  Tt_F = " << node.Tt_F << endl;
    cout << "  R_child = " << node.repres_child << endl;
    cout << "  R_leaf = "; print_features(T.N[node.repres_leaf_address].sub_F); cout << endl << endl;

    cout << " Child Nodes:" << endl;
    cout << " feature       tt_f        t_f         u_f         l_f        rep_leaf_av    rep_leaf_Tf    rep_leaf" << endl;
    for(int fi = 0; fi < f+1; fi++ )
    {
        if (node.address_f[fi] != -1)
        {
            Node chnode = T.N[node.address_f[fi]];
            cout << "   ";
            if (fi!=f) cout << setw(4) << std::setfill('0') << fi;
            else cout << " fs ";
            cout << "       " << setw(8) << std::setfill('0') << chnode.Tt_F;
            cout << "    " << setw(8) << std::setfill('0') << (int)chnode.T_F << "    " << setprecision(4) << setw(8) <<  chnode.U_F << "    " << setw(8) << chnode.L_F << "       ";
            cout << setw(6) << T.N[chnode.repres_leaf_address].av_F << "         " << setw(6) << (int)T.N[chnode.repres_leaf_address].T_F << "      ";
            if (f <= 20 ) print_features(T.N[chnode.repres_leaf_address].sub_F);
            else cout << "too long";
            cout << endl;
        }
    }
    cout << endl;
    cout << "============================================================================================";
    cout << endl << endl << endl;
}





void print_score(const vector<double> &score, int f)
{
    cout << endl << "  Score associated to each features (%):" << endl << endl;
    for (int fi=0; fi<f; fi++)
    {
        cout << "    " << setw(3) << std::setfill('0') << fi << "    " << setprecision(4) << score[fi]*100 << endl;
    }
    cout << endl;

}




void  print_results(const Tree &T, const Params &params, const Variables &variables)
// Prints out the results of the simulations in console and different files
{
    ofstream output_file_3 ("Results.txt", ios_base::app); //file which append simulation result after each simulations



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
            cout << "   delta = confidence," << endl;
            cout << "   epsilon = precision," << endl;
            cout << "   rseed = seed of the random generator," << endl;
            cout << "   it = number of iterations," << endl;
            cout << "   reward = evaluation of the selected feature set," << endl;
            cout << "   features = selected features" << endl;
            cout << "   itt = number of iterations for each greedy search," << endl;

            cout << "   --> For the other parameters, please refer to the implementation details." << endl << endl << endl;


            cout << "============================================================================================================================================================" << endl;
            cout << "|| ----------------- Main results   ----------------- || -------------------------------------------  Other results ------------------------------------------ ||" << endl;
            cout << "============================================================================================================================================================" << endl;
            cout << "delta        epsilon         rseed        it             reward          features                            itt                            reward                                       " << endl;
        }

        cout << setw(4) << std::setfill('0') << setprecision(4) << params.delta << "         ";
        cout << setw(6) << std::setfill('0') << setprecision(4) << params.eps << "          ";
        cout << setw(3) << std::setfill('0') << params.rseed << "          ";
        cout << setw(7) << std::setfill('0') << variables.it << "        ";
        cout << setprecision(4) << variables.reward.back() << "          [";

        for (int i=0;i<variables.selected_features.size();i++) {cout << " " << variables.selected_features[i] << " ";} cout << "]      [";
        for (int i=0;i<variables.itt.size();i++) {cout << " " << variables.itt[i] << " ";} cout << "]      [";
        for (int i=0;i<variables.reward.size();i++) {cout << " " << variables.reward[i] << " ";} cout << "]";
        cout << endl;

        output_file_3.close();
    }
    else cout << "Unable to open Results.txt" << endl;




    // Back to console - printing other nodes

    freopen( "CON", "w", stdout );
    //print_other_nodes(T, N_best_path.sub_F, gRAVE);


}

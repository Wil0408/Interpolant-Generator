// **************************************************************************
//  File       [main.cpp]
//  Author     [Wei-Sheng Hsieh]
//  Synopsis   [111-Ric final project SAT interpolant generator]
// **************************************************************************
#include <iostream>
#include <fstream>
#include "resol_proof.h"

using namespace std;

int main(int argc, char* argv[]) {
    fstream fin;
    fstream fout;
    fin.open(argv[1], ios::in);
    if(!fin) {
        cout << endl;
        cout << argv[1] << " can not be opened! " << endl;
    }
    fout.open(argv[2], ios::out);

    int var_num;    // total number of variables
    int clause_A_num;    // clause number of CNF A
    int clause_B_num;    // clause number of CNF B

    fin >> var_num >> clause_A_num >> clause_B_num;
    int root_clause_num = clause_A_num + clause_B_num;
    vector<resol_clause> clause_list;   // clause list
    vector<variable> var_list(var_num);     // variable list
    for(int i=0; i<var_num; i++) {
        var_list[i].var_Idx = i+1;
    }

    int c_Idx;
    while(fin >> c_Idx) {
        bool first_zero = false;
        bool pred_1 = false;
        bool pred_2 = false;
        int var;
        resol_clause clause;
        clause.clause_Idx = c_Idx;
        clause.is_A = (c_Idx <= clause_A_num ) ? 0 : (c_Idx <= root_clause_num) ? 1 : -1;
        while(fin >> var) {
            if(var == 0) {
                if(!first_zero) first_zero = true;
                else break;
            }
            else {
                if(!first_zero) {
                    clause.literal_list.push_back(var);
                    int v = abs(var);
                    if(c_Idx <= clause_A_num && !var_list[v-1].in_clauseA) var_list[v-1].in_clauseA = true;
                    if(c_Idx > clause_A_num && c_Idx <= root_clause_num && !var_list[v-1].in_clauseB) var_list[v-1].in_clauseB = true;
                }
                else {
                    if(!pred_1 && !pred_2) {
                        clause.pred_1 = var;
                        pred_1 = true;
                    }
                    else if(pred_1 && !pred_2) {
                        clause.pred_2 = var;
                        pred_2 = true;
                    }
                    else {
                        cout << " more than two predecessors! " << endl;
                        exit(-1);
                    }
                }
            }
        }
        clause_list.push_back(clause);
    }

    print_input_info(var_num, clause_A_num, clause_B_num, clause_list, var_list);

    vector<interpolant> itp(clause_list.size());
    itp_gen(itp, clause_list, var_list, clause_list.size());
    print_itp_info(itp);
    write_itp(itp, fout);
    
    return 0;
}
#ifndef _RESOL_PROOF_H_
#define _RESOL_PROOF_H_

#include <vector>
#include <fstream>
using namespace std;
using lit = int;

class resol_clause {    
    public:
        resol_clause() {
            pred_1 = -1;
            pred_2 = -1;
            is_A = -1;
        }
        ~resol_clause() {;}
        vector<lit> literal_list;   // literals in clasues
        int pred_1;
        int pred_2;
        int clause_Idx; 
        int is_A;   // -1 for neither A nor B, 0 for A, 1 for B
};

class variable {
    public:
        variable() {
            in_clauseA = false;
            in_clauseB = false;
        }
        ~variable() {;}
        bool in_clauseA;
        bool in_clauseB;
        int var_Idx;
};

class interpolant {
    public:
        interpolant() {
            gate_type = false;
            is_root = false;
            is_const_true = false;
            pred_1 = -1;
            pred_2 = -1;
        }
        ~interpolant() {;}
        bool gate_type; // 0 for and, 1 for or
        bool is_root;   // 0 for resol clause, 1 for root clause
        bool is_const_true; // true for this gate is const true
        int pred_1;
        int pred_2;
        vector<lit> literal_list;   // the literal list int root clause
};

void itp_gen(vector<interpolant> &itp, const vector<resol_clause> &clause_list, const vector<variable> &var_list, int clause_Idx);

int find_pivot(const vector<lit> &list_A, const vector<lit> &list_B, const vector<lit> &resol_list);

void print_input_info(int var_num, int clause_A_num, int clause_B_num, const vector<resol_clause> &clause_list, const vector<variable> &var_list);

void print_itp_info(const vector<interpolant> &itp);

void write_itp(const vector<interpolant> &itp, fstream &fout);


#endif
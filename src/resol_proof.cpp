#include "resol_proof.h"
#include <iostream>

void itp_gen(vector<interpolant> &itp, const vector<resol_clause> &clause_list, const vector<variable> &var_list, int clause_Idx) {
    
    if(clause_list[clause_Idx-1].is_A == -1) {  // resolution clause
        resol_clause c1 = clause_list[clause_list[clause_Idx-1].pred_1-1];
        resol_clause c2 = clause_list[clause_list[clause_Idx-1].pred_2-1];
        int pivot_var = find_pivot(c1.literal_list, c2.literal_list, clause_list[clause_Idx-1].literal_list);
        // cout << "clause" << clause_Idx << ": " << pivot_var << endl;

        if(var_list[pivot_var-1].in_clauseA && !var_list[pivot_var-1].in_clauseB) {   // pivot variable is local to A
            itp[clause_Idx-1].gate_type = true; // or gate
        }
        else {
            itp[clause_Idx-1].gate_type = false; // and gate
        }

        int pred_1 = clause_list[clause_Idx-1].pred_1;
        int pred_2 = clause_list[clause_Idx-1].pred_2;
        itp[clause_Idx-1].pred_1 = pred_1;
        itp[clause_Idx-1].pred_2 = pred_2;
        itp_gen(itp, clause_list, var_list, pred_1);
        itp_gen(itp, clause_list, var_list, pred_2);
    }
    else {  // root clause
        if(clause_list[clause_Idx-1].is_A == 0) { // clause belongs to A
            itp[clause_Idx-1].gate_type = true;
            itp[clause_Idx-1].is_root = true;
            for(int i=0; i<clause_list[clause_Idx-1].literal_list.size(); i++) {
                int v = abs(clause_list[clause_Idx-1].literal_list[i]);
                if(var_list[v-1].in_clauseA && var_list[v-1].in_clauseB) {  // disjunction of global literals
                    itp[clause_Idx-1].literal_list.push_back(clause_list[clause_Idx-1].literal_list[i]);
                }
            }
        }
        else {
            itp[clause_Idx-1].is_root = true;
            itp[clause_Idx-1].is_const_true = true;
        }
    }
    return;
}

int find_pivot(const vector<lit> &list_A, const vector<lit> &list_B, const vector<lit> &resol_list) {
    int pivot = 0;
    vector<lit> record;
    for(int i=0; i<list_A.size(); i++) {
        for(int j=0; j<list_B.size(); j++) {
            if(list_A[i]+list_B[j] == 0) {
                record.push_back(list_A[i]);
            }
        }
    }
    if(record.size() == 1) {
        pivot = abs(record[0]);
        return pivot;
    }
    else {
        for(int i=0; i<record.size(); i++) {
            bool flag = false;
            for(int j=0; j<resol_list.size(); j++) {
                if(record[i] == resol_list[j]) flag = true; 
            }
            if(!flag) {
                pivot = abs(record[i]);
                return pivot;
            }
        }
    }
    return pivot;
}

void print_input_info(int var_num, int clause_A_num, int clause_B_num, const vector<resol_clause> &clause_list, const vector<variable> &var_list) {
    cout << "===================== input resolution proof info =======================" << endl;
    cout << "var_num: " << var_num << endl;
    cout << "clause_A_num: " << clause_A_num << endl;
    cout << "clause_B_num: " << clause_B_num << endl;
    cout << endl;
    for(int i=0; i<clause_list.size(); i++) {
        cout << "clause" << clause_list[i].clause_Idx << endl;
        cout << "literal list: ";
        for(int j=0; j<clause_list[i].literal_list.size(); j++) {
            cout << clause_list[i].literal_list[j] << " ";
        }
        cout << endl;
        cout << "pred_1: " << clause_list[i].pred_1 << endl;
        cout << "pred_2: " << clause_list[i].pred_2 << endl;
        switch(clause_list[i].is_A) {
            case 0:
                cout << "belong clause A" << endl;
                break;
            case 1:
                cout << "belong clause B" << endl;
                break;
            default:
                cout << "resolution clause" << endl;
                break;
        }
    }
    cout << endl;
    cout << "=================== variable list ===================" << endl;
    for(int i=0; i<var_list.size(); i++) {
        cout << "var" << i+1 << endl;
        if(var_list[i].in_clauseA && var_list[i].in_clauseB) cout << "global var" << endl;
        if(var_list[i].in_clauseA && !var_list[i].in_clauseB) cout << "local to clause A" << endl;
        if(!var_list[i].in_clauseA && var_list[i].in_clauseB) cout << "local to clause B" << endl;
        if(!var_list[i].in_clauseA && !var_list[i].in_clauseB) cout << "var is neither in A nor in B" << endl;
    }
    cout << endl;
    return;
}

void print_itp_info(const vector<interpolant> &itp) {
    cout << "========================= interpolant info =============================" << endl;
    for(int i=0; i<itp.size(); i++) {
        cout << i+1 << " ";
        if(itp[i].is_root) {
            if(itp[i].is_const_true) {
                cout << "true ";
            }
            else {
                if(itp[i].literal_list.size() == 0) {
                    cout << "false ";
                }
                else {
                    cout << "OR ";   // disjunction
                    for(int j=0; j<itp[i].literal_list.size(); j++) {
                        cout << itp[i].literal_list[j] << " ";
                    }
                }
            }
            cout << "0 0" << endl;
        }
        else {
            if(itp[i].gate_type) {  // or gate
                cout << "OR 0 ";
                cout << itp[i].pred_1 << " " << itp[i].pred_2 << " 0" << endl;
            }
            else {  // and gate
                cout << "AND 0 ";
                cout << itp[i].pred_1 << " " << itp[i].pred_2 << " 0" << endl;
            }
        }
    }
}

void write_itp(const vector<interpolant> &itp, fstream &fout) {
    for(int i=0; i<itp.size(); i++) {
        fout << i+1 << " ";
        if(itp[i].is_root) {
            if(itp[i].is_const_true) {
                fout << "true ";
            }
            else {
                if(itp[i].literal_list.size() == 0) {
                    fout << "false ";
                }
                else {
                    fout << "OR ";   // disjunction
                    for(int j=0; j<itp[i].literal_list.size(); j++) {
                        fout << itp[i].literal_list[j] << " ";
                    }
                }
            }
            fout << "0 0" << endl;
        }
        else {
            if(itp[i].gate_type) {  // or gate
                fout << "OR 0 ";
                fout << itp[i].pred_1 << " " << itp[i].pred_2 << " 0" << endl;
            }
            else {  // and gate
                fout << "AND 0 ";
                fout << itp[i].pred_1 << " " << itp[i].pred_2 << " 0" << endl;
            }
        }
    }
}
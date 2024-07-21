#include "../SATSolver/SATsolver.h"
#include <iostream>
using namespace std;

/*

Given a set V, a list of subsets S and a number k find out if there exists {i_1,i_2,...i_k} where each i_j is distinct 
and |i_j|<|S| and S[i_1] U S[i_2] U S[i_3] ... S[i_k] = V where U represents set union
It is guaranteed all entries of V are unique (i.e. V is not a multiset)
(I think) this problem is NP-Complete
Either solve this in polynomial time OR use the SAT solver

*/

int get_index_element(int element, const vector<int>& vec){
    for(int i=0;i<vec.size();i++){
        if(element==vec[i]) return i;
    }
    return 0;
}

void convert_boolean(const vector<int>& set,const vector<vector<int>>& setofSubset, vector<vector<int>>& subset_bool){
    for(int i=0;i<setofSubset.size();i++){
        for(int j=0;j<setofSubset[i].size();j++ ){
            int element = setofSubset[i][j];
            int x = get_index_element(element,set);
            subset_bool[i][x] = 1;
        }
    }
    return;
}

void generate_k_subsets(const vector<int>& set, int k, int start, vector<int>& one_tuple, vector<vector<int>>& setOfktuples) {
    if (one_tuple.size() == k) {
        setOfktuples.push_back(one_tuple);
        return;
    }

    for (int i = start; i < set.size(); ++i) {
        one_tuple.push_back(set[i]);
        generate_k_subsets(set, k, i + 1, one_tuple, setOfktuples);
        one_tuple.pop_back();
    }
}

void generate_k_tuples(int k, int num_subset, vector<vector<int>>& setOfKtuples) {
    vector<int> temp;
    vector<int> set;
    for(int i=0;i<num_subset;i++){
        set.push_back(i);
    }
    generate_k_subsets(set, k, 0, temp, setOfKtuples);
    return ;
}

int main () {
    int lengthOfSet;
    //cout<<"Enter length of set : \n";
    cin>>lengthOfSet;
    vector<int> V;
    //cout<<"Enter set : \n";
    for (int i=0; i<lengthOfSet; i++) {
        int a;
        cin>>a;
        V.push_back(a);
    }
    int numSubSets;
    //cout<<"Enter number of subsets : \n";
    cin>>numSubSets;
    vector<vector<int>> S;
    for (int i=0; i<numSubSets; i++) {
        int lengthOfSubset;
        //cout<<"Enter length of subset "<<i<<" : \n";
        cin>>lengthOfSubset;
        //cout<<"Enter subset "<<i<<" : \n";
        vector<int> t;
        for (int j=0; j<lengthOfSubset; j++) {
            int x;
            cin>>x;
            t.push_back(x);
        }
        S.push_back(t);
    }

    //cout<<"Enter k : \n";
    int k;
    cin>>k;
    int cardinality = lengthOfSet;
    vector<vector<int>> subset_bool(numSubSets,vector<int>(cardinality,0));
    convert_boolean(V,S,subset_bool);
    // for(int i=0;i<subset_bool.size();i++){
    //     for(int j=0;j<subset_bool[i].size();j++){
    //         cout<<subset_bool[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    // cout<<"Done"<<endl;

    // Generating all k-element subset vectors on which we test the problem being unsat or not

    vector<vector<int>> k_iterate_subset;
    generate_k_tuples(k,numSubSets,k_iterate_subset);

    // for(int i=0;i<k_iterate_subset.size();i++){
    //     for(int j=0;j<k_iterate_subset[i].size();j++){
    //         cout<<k_iterate_subset[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    // cout<<"Done"<<endl;

    //Now for each tuple i generate clauses and check, if anyone is unsat then print possible or impossible
    //Approach1
    // for(int i=0;i<k_iterate_subset.size();i++){
    //     vector<vector<int>> clause_generator;
    //     for(int j=0;j<k_iterate_subset[i].size();j++){
    //         clause_generator.push_back(subset_bool[k_iterate_subset[i][j]]);
    //     }
    //     vector<vector<int>> clauses;
    //     for(int p=0;p<lengthOfSet;p++){
    //         vector<int> clause;
    //         for(int q=0;q<clause_generator.size();q++){
    //             if(clause_generator[q][p]){
    //                 clause.push_back(q+1);
    //             }
    //         }
    //         if(clause.size()>0){
    //             clauses.push_back(clause);   
    //         }
    //     }
    //     vector<int> all_negation;
    //     for(int a=0;a<subset_bool.size();a++){
    //         all_negation.push_back(-1*(a+1));
    //     }
    //     clauses.push_back(all_negation);

    //     //testing clause generator and clauses
    //     // for(int x=0;x<clause_generator.size();x++){
    //     //     for(int y=0;y<clause_generator[x].size();y++){
    //     //         cout<<clause_generator[x][y]<<" ";
    //     //     }
    //     //     cout<<endl;
    //     // }
    //     // cout<<"Done"<<endl;

    //     // for(int x=0;x<clauses.size();x++){
    //     //     for(int y=0;y<clauses[x].size();y++){
    //     //         cout<<clauses[x][y]<<" ";
    //     //     }
    //     //     cout<<endl;
    //     // }
    //     // cout<<"Done"<<endl;

    //     //Now checking if UNSAT or not, if UNSAT return possible
    //     cdcl solver;
    //     solver.setup(clauses,subset_bool.size()+1);
    //     bool result = solver.solve();
    //     if(!result){
    //         cout<<"Possible"<<endl;
    //         return 0;
    //     }

    // }
    // cout<<"Impossible"<<endl;
    // return 0;

    //Approach2
    for(int i=0;i<k_iterate_subset.size();i++){
         vector<vector<int>> clause_generator;
         for(int j=0;j<k_iterate_subset[i].size();j++){
             clause_generator.push_back(subset_bool[k_iterate_subset[i][j]]);
        }
        vector<vector<int>> clauses;
        for(int i=0;i<clause_generator.size();i++){
            for(int j=0;j<clause_generator[i].size();j++){
                if(clause_generator[i][j]){
                    vector<int> clause;
                    clause.push_back(j+1);
                    clauses.push_back(clause);
                }
            }
        }
        vector<int> all_negation;
        for(int a=0;a<cardinality;a++){
             all_negation.push_back(-1*(a+1));
         }
         clauses.push_back(all_negation);

        // for(int x=0;x<clauses.size();x++){
        //     for(int y=0;y<clauses[x].size();y++){
        //         cout<<clauses[x][y]<<" ";
        //     }
        //     cout<<endl;
        // }
        // cout<<"Done"<<endl;

        //Now checking if UNSAT or not, if UNSAT return possible
        cdcl solver;
        solver.setup(clauses,lengthOfSet+1);
        bool result = solver.solve();
        if(!result){
            cout<<"Possible"<<endl;
            return 0;
        }

    }
    cout<<"Impossible"<<endl;
    return 0;
    
}
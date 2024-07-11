#include <bits/stdc++.h>
using namespace std;

/*

The Naive SAT Solver algorithm is far too slow, thus there is a better algorithm for SAT solving called DPLL
Davis–Putnam–Logemann–Loveland (DPLL) Algorithm:
* First perform unit propogation to get rid of unit clauses (clauses that have only one literal)
* Then, decide the value of some variable and check if the formula becomes SAT (by for example, recursively calling DPLL)
* If it does, return the assignment
* If not, then change the value of that variable 

*/

#define UNDEFINED -1
#define TRUE 1
#define FALSE 0

class dpll {
    private:
    uint numVariables;
    uint numClauses;
    vector<vector<int>> clauses;
    bool unitProp (vector<int> partialModel);
    bool doPll (vector<int> partialModel);
    
    public:
    vector<int> finalModel;
    void getInput ();
    bool solve();

};



void dpll::getInput () {

    char c = cin.get(); // CNF comments
    while (c == 'c') {
        while (c != '\n') {
            c = cin.get();
        }
        c = cin.get();
    }
    string t;
    cin>>t;

    cin>>numVariables;
    cin>>numClauses;
    for (uint i=0; i<numVariables+1; i++) {
        finalModel.push_back(UNDEFINED);
    }
    for (uint i=0; i<numClauses; i++) {
        vector<int> a;
        int c;
        cin>>c;
        while (c != 0) {
            a.push_back(c);
            cin>>c;
        }
        // a.value = UNDEFINED;
        // a.size = a.elements.size();
        clauses.push_back(a);
    }
}

bool isUnitClause(vector<vector<int>> clauses,uint numClauses, uint numVariables, vector<int> partialModel){
    bool unitclause=false;
    // Iterating through all clauses
    for(uint i=0;i<numClauses;i++){
        uint count_false=0;
        uint count_undecided=0;
        //Checking if a particular clause is a unit clause
        for(uint j=0;j<clauses[i].size();j++){
            if(clauses[i][j]<0){
                if(partialModel[-clauses[i][j]-1]==UNDEFINED){
                    count_undecided+=1;
                }
                if(partialModel[-clauses[i][j]-1]==TRUE){
                    count_false+=1;
                }
            }
            else{
                if(partialModel[clauses[i][j]-1]==UNDEFINED){
                    count_undecided+=1;
                }
                if(partialModel[clauses[i][j]-1]==FALSE){
                    count_false+=1;
                }

            }

        }
        if(count_false==clauses[i].size()-1 && count_undecided==1){
            unitclause=true;
            return unitclause;
        }

    }
    return unitclause;
}

void determine_undecided_assign(vector<vector<int>> clauses,uint numClauses, uint numVariables, vector<int> partialModel, uint& clause_no, uint& partial_undecided){
    bool unitclause=false;
    for(uint i=0;i<numClauses;i++){
        uint count_false=0;
        uint count_undecided=0;
        uint index_undecided=0;
        for(uint j=0;j<clauses[i].size();j++){
            if(clauses[i][j]<0){
                if(partialModel[-clauses[i][j]-1]==UNDEFINED){
                    count_undecided+=1;
                    index_undecided=-clauses[i][j]-1;
                }
                if(partialModel[-clauses[i][j]-1]==TRUE){
                    count_false+=1;
                }
            }
            else{
                if(partialModel[clauses[i][j]-1]==UNDEFINED){
                    count_undecided+=1;
                    index_undecided=clauses[i][j]-1;
                }
                if(partialModel[clauses[i][j]-1]==FALSE){
                    count_false+=1;
                }

            }

        }
        if(count_false==clauses[i].size()-1 && count_undecided==1){
            unitclause=true;
        }
        if(unitclause){
            clause_no = i;
            partial_undecided=index_undecided;
            return;
        }

    }
    return;
}

bool someClauseFalse(vector<vector<int>> clauses,uint numClauses, uint numVariables, vector<int> partialModel){
    for(uint i=0;i<numClauses;i++){
        bool clause_value=false;
        //Iterating through one clause
        for(uint j=0;j<clauses[i].size();j++){
            if(clauses[i][j]<0){
                if(partialModel[-clauses[i][j]-1]==UNDEFINED){
                    clause_value=true;
                    break;
                }
                else if(partialModel[-clauses[i][j]-1]==FALSE){
                    clause_value=true;
                    break;
                }

            }
            else{
                if(partialModel[clauses[i][j]-1]==UNDEFINED){
                    clause_value=true;
                    break;
                }
                else if(partialModel[clauses[i][j]-1]==TRUE){
                    clause_value=true;
                    break;
                }

            }

        }
        if(!clause_value) return true;

    }
    return false;
}

bool dpll::unitProp (vector<int> partialModel) {
//Implement unit propogation!
while (true) {
    //cout<<"before entered"<<endl;
    //for(uint i=0;i<partialModel.size();i++){
    //    cout<<partialModel[i]<<" ";
    //}
    //cout<<endl;
    if (isUnitClause(clauses,numClauses,numVariables,partialModel)) {
        //cout<<"entered"<<endl;
        uint clause_no = 0;
        uint partial_undecided=0;
        determine_undecided_assign(clauses,numClauses,numVariables,partialModel,clause_no,partial_undecided);
        if(clauses[clause_no][partial_undecided]<0){
            partialModel[partial_undecided]=FALSE;
        }
        else{
            partialModel[partial_undecided]=TRUE;
        }

    }
    //cout<<"after entered"<<endl;
    //for(uint i=0;i<partialModel.size();i++){
    //    cout<<partialModel[i]<<" ";
    //}
    //cout<<endl;
    if (someClauseFalse(clauses,numClauses, numVariables,partialModel)) {
        return false;
    }
    if (!isUnitClause(clauses,numClauses,numVariables,partialModel)) {
        break;
    }

}
return true;

}

bool check_partial_undecided(vector<int> partialModel){
    for(uint i=0;i<partialModel.size();i++){
        //cout<<partialModel[i]<<" ";
        if(partialModel[i]==UNDEFINED){
            return true;
        }
    }
    return false;
}

uint index_undecided_partial(vector<int> partialModel){
    for(uint j=0;j<partialModel.size();j++){
        if(partialModel[j]==UNDEFINED){
            return j;
        }
    }
}

bool formulaIsSat(vector<vector<int>> clauses,uint numClauses, uint numVariables, vector<int> partialModel){
    for(uint i=0;i<numClauses;i++){
        bool this_clause=false;
        for(uint j=0;j<clauses[i].size();j++){
            if(clauses[i][j]<0){
                if(partialModel[-clauses[i][j]-1]==UNDEFINED){
                    this_clause=false;
                    break;
                }
                else if(partialModel[-clauses[i][j]-1]==FALSE){
                    this_clause=true;
                }
            }
            else{
                if(partialModel[clauses[i][j]-1]==UNDEFINED){
                this_clause=false;
                break;
                }
                else if(partialModel[clauses[i][j]-1]==TRUE){
                this_clause=true;
                }   
            }
        }
        if(!this_clause){
            return false;
        }
    }
    return true;
}

bool dpll::doPll (vector<int> partialModel) {

//Implement the dpll algorithm
unitProp(partialModel);
if (formulaIsSat(clauses,numClauses,numVariables,partialModel)) {
    finalModel = partialModel;
    return true;
}
else {
    //cout<<"entered"<<endl;
    if (check_partial_undecided(partialModel)) {
        uint x = index_undecided_partial(partialModel);
        //cout<<"update"<<" "<<x<<endl;
        vector<int> posMod = partialModel;
        posMod[x] = 1;
        vector<int> negMod = partialModel;
        negMod[x] = 0;
        return (doPll(posMod) || doPll(negMod));
    }
    else {
        return false;
    }
}


}

bool dpll::solve() {
    vector<int> m(numVariables,UNDEFINED);
    return doPll(m);
}

int main () {
    
    dpll d;
    d.getInput();

    if (d.solve()) {
        cout<<"TRUE\n";
        for (int i=0; i<d.finalModel.size(); i++) {
            cout<<i+1<<" : "<<d.finalModel[i]<<endl;
        }
    }
    else {
        cout<<"UNSAT\n";
    }
}
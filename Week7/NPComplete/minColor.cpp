#include "../SATSolver/SATsolver.h"
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
/*

Given a graph G, determine the minimum number of colors needed to be used to color the graph given the constraint that no two adjacent edges
can have the same color.
Graph:
It consists of two objects
1]int numNodes : The number of vertices in the graph
2]vector<pair<int,int>> adjacencyList : The adjacency list of the graph, adjacencyList[i] = {j,k} implies an edge exists between j and k
Constraints:
1]Each vertex must have exactly one color
2]If there exists an edge between vertex i and vertex j, then color(i) != color(j)
To Find : 
minimum color k that satisfies above constraint
(I think) the above problem is NP-Complete, either find a polynomial algo to solve it OR
Use the SAT solver you built as an oracle to solve it

*/

void clause_generator(vector<vector<int>>& clauses,const vector<pair<int,int>>& adjacencyList, int numNodes, int numEdges, int no_color){
    //First each clause must have atleast one color
    //Then a clause must have atmost one color, which with earlier condition means exactly one color
    for(int i=0;i<numNodes;i++){
        vector<int> clause_all;
        for(int j=0;j<no_color;j++){
            clause_all.push_back(i*no_color+j+1);
        }
        clauses.push_back(clause_all);
        for(int p=0;p<no_color;p++){
            for(int q=p+1;q<no_color;q++){
                vector<int> clause_negate = {-clause_all[p],-clause_all[q]};
                clauses.push_back(clause_negate);
            }
        }
    }
    
    //For edges, no endpoints of an edge can have same color
    for(auto edge : adjacencyList){
        for(int r=0; r<no_color;r++){
            vector<int> clause_edge = {-((edge.first-1)*no_color+r+1),-((edge.second-1)*no_color+r+1),};
            clauses.push_back(clause_edge);
        }
    }
    return;

}

bool k_color_check(int numNodes, int numEdges, const vector<pair<int,int>>& adjacencyList, int no_color ){
    vector<vector<int>> clauses;
    clause_generator(clauses,adjacencyList,numNodes,numEdges, no_color);
    cdcl minColorSolver;
    minColorSolver.setup(clauses,numNodes*no_color+1);
    bool satisfied = minColorSolver.solve();
    return satisfied;
}


int main() {
    int numNodes;
    cin>>numNodes;
    vector<pair<int,int>> adjacencyList;
    int numEdges;
    cin>>numEdges;
    for (int i=0; i<numEdges; i++) {
        int a,b;
        cin>>a>>b;
        adjacencyList.push_back(make_pair(a,b));
    }
    if(numEdges==0){
        cout<<"The value of k is "<<1<<endl;
        return 0;
    }
    else if(numEdges==1){
        cout<<"The value of k is "<<2<<endl;
        return 0;
    }
    else if(numEdges==2){
        cout<<"The value of k is "<<2<<endl;
        return 0;
    }
    else{
        int no_color=2;
        while(true){
            //cout<<no_color<<endl;
            if(!k_color_check(numNodes,numEdges, adjacencyList,no_color)) no_color++;
            else break;
        }
        cout<<"The value of k is "<<no_color<<endl;
    }
}
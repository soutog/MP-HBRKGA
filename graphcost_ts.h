#ifndef GRAPH_COST_TS_H_
#define GRAPH_COST_TS_H_
#include "instance.h"
#include "ilcplex/cplex.h"
#include <vector>
#include <iostream>

using namespace std;

class GraphCostTS{
                
    private:
        int  buildNetwork  (CPXENVptr env, CPXNETptr net);
        void free_and_null (char **ptr);   
        void update_ub(vector<int> Solution);

        //Data
        Instance * p;
        int commodity;
        int artificial_ware_arc_zero_index;
        int artificial_fac_arc_zero_index;

        //CPLEX Sol                                  
        int      solstat;
        double   objval;
        double   *x;
        double   *pi;
        double   *slack;
        double   *dj;

        //Grafo
        int      NARCS;
        int      NNODES;
        double * supply;
        int * from_node;
        int * to_node;
        double * obj;
        double * ub;
        double * lb;

        CPXENVptr env;
        CPXNETptr net;
        int       status;
      
    public:
        GraphCostTS(Instance *p, int l);
        ~GraphCostTS();
        void solve(vector<int> Solution);
        pair<double,vector<int>> getsol();

};

#endif
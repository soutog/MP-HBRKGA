#ifndef HEURISTIC_H_
#define HEURISTIC_H_

#include <vector>
#include <queue>
#include <algorithm>
#include <random>

#include "instance.h"
#include "checkfeas.h"
#include "stdlib.h"
#include "graphcost_ts.h"
#include "time.h"

class Heuristic{
	private:
		Instance *p;
		CheckFeas *is_feasible;

		
		vector<int> S;
		vector<int> cap_fab_abe;
		vector<int> cap_dep_abe;
		vector<vector<double>> hcapfab;
		vector<vector<double>> hcapdep;
		double cost;
		void construtivo_fab(double alpha);
		void construtivo_dep(double alpha);
		int aleatorio(int lower,int upper);
		void CostBenCalculate();

	public : 
	    Heuristic(Instance *p_,vector<GraphCostTS *> _g);
		~Heuristic();
		void run(double alpha);
		vector<int>& get_Solution(){return S;}
		double  get_hcapfab(int l,int i){return hcapfab[l][i];}
		double  get_hcapdep(int l,int j){return hcapdep[l][j];}
		vector<int>& get_cap_fab_abe(){return cap_fab_abe;}
		vector<int>& get_cap_dep_abe(){return cap_dep_abe;}
		double get_cost(){return cost;}
};

#endif

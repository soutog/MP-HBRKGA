#include <iostream>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <algorithm>
#include <vector>
#include <ctime>


#include "instance.h"
#include "checkfeas.h"
#include "heuristic.h"
#include "localsearch.h"
#include "graphcost_ts.h"
#include "./BRKGA/AG.h"
#include "ExactMPTSCFL.h"

using namespace std;



void usage(char *argv[]){
	cout << "Usage:" <<endl;
	cout << "\t"<<argv[0]<<" <input_instance_name> <tempo_limite> <iter> <gen> <alpha>"<<endl;
	cout << "\t"<<"<input_instance_name>: nome do arquivo de entrada"<<endl;
	cout << "\t"<<"<tempo_limite>: tempo maximo de execucao do algoritmo"<<endl;
	cout << "\t"<<"<iter>: numero de iteracoes" << endl;
	cout << "\t"<<"<gen>: gerações de execucao do algoritmo"<<endl;
	cout << "\t"<<"<alpha>: entre 0 e 1 "<<endl;
}

int main(int argc, char *argv[]){
	if(argc!=6){
		usage(argv);
	}else{
		srand(clock());
		const char* datafile = argv[1];
		//double alpha = atof(argv[2]);
		int timelimit = atoi(argv[2]); //Tempo Limite em seg
		int iter = atoi(argv[3]);
		unsigned gen = atoi(argv[4]);
		double alpha = atof(argv[5]);

		double random =  rand();

		Instance *p = new Instance();
		p->load_file(datafile);
		p->calculaMinMaxFabDep();

		//ExactMPTSCFL model(p);
		//model.ConstroiModelo();
		vector<GraphCostTS *> gcosts(p->ncommodities);
		for (int i=0; i < p->ncommodities; ++i){
			GraphCostTS *g = new GraphCostTS(p,i);
			gcosts[i] = g;
		}
		
		vector<int> Solution;
		// clock_t hI = clock();
		
		// cout << fixed << constructive.get_cost() << endl;
		// Solution =  constructive.get_Solution();
		// clock_t hF = clock();
		// double time = (double)(hF - hI) / CLOCKS_PER_SEC;
		// cout << time << endl;

		// CheckFeas is_feasible(p,gcosts);
		// vector<int> Solution = {1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,0,1};
		// cout << Solution.size() << endl;
		// int cost_constructive = is_feasible.check(Solution);
		// cout << cost_constructive << endl;
		// LocalSearch ls(p,gcosts);
		// ls.solveRVND(Solution);
		// Solution = ls.get_Solution();
		// int cost_ls = is_feasible.check(Solution);
		// cout << cost_constructive << endl;
		// clock_t hI = clock();
		//instance->print_structures();
		// CheckFeas is_feasible(instance,g);
		// vector<int> Solution = {1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,0,1};
		// cout << Solution.size();
		// model.solveLB(Solution);
		//vector<double> sol{0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1};
		// Heuristic genSol(instance,g);
		
		// genSol.run(alpha);
		// vector<int> Solution = genSol.get_Solution();   
		
		// vector<int> Solution = {1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,0,1};
		// //vector<int> Solution = {1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1};
		// cout<<"Solution:"<<endl;
		//  for(int i=0; i<Solution.size();++i){
		// 	if (i == I){ cout << "fab "  << endl; }
		// 	else if (i == I+1){ cout << "dep "  << endl;}
		// 	if(Solution[i]!=0) cout<<i<<" ";
		// }
		// cout<<endl; 
		// //int cost_constructive =  genSol.get_cost();
		// //cout << "Durante: " << genSol.get_cost() << endl;
		// cout << "Depois: " << is_feasible.check(Solution) << endl;
		//  int cost_constructive = is_feasible.check(Solution);
 		// clock_t hF = clock();

		// cout << "Tempo de Execucao: " << ((double)hF - hI) / CLOCKS_PER_SEC << endl;
		// LocalSearch ls(instance,g);

		//  ls.solve(Solution);
		// Solution = ls.get_Solution();
		
		// for(int i=0; i<Solution.size();++i){
		// 	cout << Solution[i] << ",";
		// }
		// cout<<endl;
		// cout << "Construtivo: " << cost_constructive << endl;
		// cout << "Durante: " << ls.get_cost() << endl;
		// cout << "Depois: " << is_feasible.check(Solution) << endl; 
 
		// clock_t hF = clock();

		// cout << "Tempo de Execucao: " << ((double)hF - hI) / CLOCKS_PER_SEC << endl;
		
		
		int64_t best_cost = 99999999; //p->get_nvertices() + 1;

		std::vector<double> costs(iter);
		std::vector<double> time(iter);

		double avg = 0.0;
		double std_dev = 0.0;
		//avg = 0.0;
		double avg_time = 0.0;
		//std_dev = 0.0;
		double std_dev_time = 0.0;
		for (int64_t i = 0; i < iter; ++i)
		{
			AG Meta((double)alpha,gcosts,p, p->nfactories+p->nwarehouses, 50, 0.2, 0.3, 0.7, 1, 1, random , 0, 100, 2, gen);
			//AG(double alpha,GraphCost &g, Instance &_instance, unsigned _n, unsigned _p, double _pe, double _pm, double _rhoe, unsigned _K, unsigned _MAXT, long unsigned _rngSeed, unsigned _generation, unsigned _X_INTVL, unsigned _X_NUMBER, unsigned _MAX_GENS)
			ExactMPTSCFL model(p);
			model.ConstroiModelo();
			clock_t hI = clock();
			tie(Solution,costs[i]) = Meta.solve();
			clock_t hF = clock();
			double timebound = (double)(hF - hI) / CLOCKS_PER_SEC;
			// cout << "LB" << endl;
			
			tie(Solution,costs[i]) = model.solveLB(Solution,int(6 * timebound),costs[i]);

			hF = clock();
			time[i] = (double)(hF - hI) / CLOCKS_PER_SEC;
			//cout << time[i] << endl;
			//cout << std::fixed << costs[i] << endl;
			avg += costs[i];
			avg_time += time[i];
			if (costs[i] < best_cost)
			{
				best_cost = costs[i];
			}
		}

		avg = avg / iter;
		avg_time = avg_time / iter;
		for (int64_t i = 0; i < iter; ++i)
		{
			std_dev += (costs[i] - avg) * (costs[i] - avg);
			std_dev_time += (time[i] - avg_time) * (time[i] - avg_time);
		}

		std_dev = std::sqrt(std_dev / iter);
		std_dev_time = std::sqrt(std_dev_time / iter);

		std::cout << datafile << " " << std::fixed << best_cost << " " << avg << " " << std_dev << " " << avg_time << " " << std_dev_time << std::endl;
		for (int i=0; i<p->ncommodities; ++i){
			delete (gcosts[i]);
		}
		delete (p);
	}
	
	return 0;
}

#include <iostream>
#include "SampleDecoder.h"

#include "MTRand.h"
#include "BRKGA.h"

#include "../heuristic.h"
#include "../checkfeas.h"
#include "../instance.h"
#include "../localsearch.h"
#include "../graphcost_ts.h"

class AG{
	private:

	unsigned n;		// size of chromosomes
	unsigned p;	// size of population
	double pe;		// fraction of population to be the elite-set
	double pm;		// fraction of population to be replaced by mutants
	double rhoe;	// probability that offspring inherit an allele from elite parent
	unsigned K;		// number of independent populations
	unsigned MAXT;	// number of threads for parallel decoding
	unsigned timelimit;
	SampleDecoder* decoder;			// initialize the decoder
	Instance * instance;
	vector<GraphCostTS*> g;
	CheckFeas * checador;
	long unsigned rngSeed;	// seed to the random number generator
	MTRand * rng;				// initialize the random number generator
	bool weight, conex;
	// initialize the BRKGA-based heuristic
	BRKGA< SampleDecoder, MTRand > * algorithm;

	unsigned generation;		// current generation
	unsigned X_INTVL;	// exchange best individuals at every 100 generations
	unsigned X_NUMBER ;	// exchange top 2 best
	unsigned MAX_GENS;	// run for 1000 gens

	public:

	AG(double alpha,vector<GraphCostTS*> _g, Instance *_instance, unsigned _n, unsigned _p, double _pe, double _pm, double _rhoe, unsigned _K, unsigned _MAXT, long unsigned _rngSeed, unsigned _generation, unsigned _X_INTVL, unsigned _X_NUMBER, unsigned _MAX_GENS):instance(_instance){
	g=_g;
	n=_n;
	p=_p;
	pe=_pe;
	pm=_pm;
	rhoe=_rhoe;
	K=_K;
	MAXT=_MAXT;
	rngSeed=_rngSeed;
	generation=_generation;
	X_INTVL=_X_INTVL;
	X_NUMBER=_X_NUMBER;
	MAX_GENS=_MAX_GENS;

	checador = new CheckFeas(_instance,g);
	LocalSearch ls(_instance,g);
	Heuristic construtivo(_instance,g);
	rng = new MTRand(rngSeed);
	decoder = new SampleDecoder(_instance, checador, ls,construtivo,_rngSeed);
	algorithm = new BRKGA<SampleDecoder, MTRand>(alpha,g,instance, construtivo, n, p, pe, pm, rhoe, (*decoder), (*rng), K, MAXT);
	};

	~AG(){
		delete(decoder);
		delete(algorithm);
		delete(rng);
	};

	std::tuple <std::vector<int>,double>  solve(){

		// do {
			algorithm->evolve(MAX_GENS);	// evolve the population for one generation
			// ++generation;
			//  if((++generation) % X_INTVL == 0) {
			// 	algorithm->exchangeElite(X_NUMBER);	// exchange top individuals

			// }
		// 	cout<<"Geracao "<<generation<<": "<<algorithm->getBestFitness()<<endl;
		// } while (generation < MAX_GENS);

		/* std::cout << "Melhor Sol BRKGA = "	<< algorithm->getBestFitness() << std::endl;
		int cont=0;
		vector<double> bestsol = algorithm->getBestChromosome();
		vector<int> solution(I+J); */
		/* for(unsigned i=0; i < n; ++i)
		{
			if(bestsol[i]>0.5){
				solution[i] = 1;
			}
			else{
				solution[i] = 0;
			}

		}
		ls->solve(solution);
		int objval = ls->get_cost();
		
		cout<<endl; */
		//std::cout << "Melhor Sol BRKGA = "	<< algorithm->getBestFitness() << std::endl;
		vector<int> Solution(n);
		vector<double> bestchromo = algorithm->getBestChromosome();
		for(unsigned i=0; i < n; ++i)
			{
				if(bestchromo[i]>=0.5){
					Solution[i] = 1;
				}
				else{
					Solution[i] = 0;
				}

			}
		double cost = checador->check(Solution);
		// clock_t ti = clock(); 
		// LocalSearch ls(instance,g);
		// ls.solveRVND(Solution);
		// Solution = ls.get_Solution();
		// double cost = ls.get_cost();
		// cout << cost << endl;
		// cout << "Durante: " << ls.get_cost() << endl;
		return make_pair(Solution,cost);
		//return make_pair(Solution,(int)algorithm->getBestFitness());
		//return int( algorithm->getBestFitness());
		/* return objval; */
	};

	/*Solution getBest(){
		int cont=0;
		vector<double> bestsol = algorithm->getBestChromosome();
		Solution BS;
		for(auto it = bestsol.begin(); it!= bestsol.end(); it++){
			BS.add_param(*it*(par_dom[cont].second-par_dom[cont].first) + par_dom[cont].first);
			cont++;
		}
		BS.set_cost(-algorithm->getBestFitness());
		return BS;
	}*/

};

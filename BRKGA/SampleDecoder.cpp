#include "SampleDecoder.h"

#include <cmath>
#include <ctime>

#define Infinity 10E6

SampleDecoder::SampleDecoder(Instance *_instance, CheckFeas* _checador,LocalSearch &_ls,Heuristic &_construtivo,double seed): instance(_instance),ls(_ls),construtivo(_construtivo){
	checador = _checador;
	rng = new MTRand(seed); 
	ineficiency = 0;
	BestFitness = 0;
	idindividu.reserve(0.3*80);
}

SampleDecoder::~SampleDecoder() {
	delete(rng);

}
void SampleDecoder::Recodifica(std::vector<int> &Solution,std::vector<double> &chromosome) {

}
bool sortByBrkgaScore(const pair<int,double> &a, const pair<int,double> &b){ 
    return (a.second > b.second); 
} 
int SampleDecoder::get_RandomIndex(){
	 int random = rand()%idindividu.size();
	 int idretorno = idindividu[random];

	 idindividu.erase(idindividu.begin()+random);

	 return idretorno;
}
double SampleDecoder::decode(std::vector<double> &chromosome,double bestcost,unsigned gen,unsigned MaxGen,unsigned i)
{
	unsigned n = chromosome.size();
	double objval;
	vector <int> Solution(instance->nfactories+instance->nwarehouses);
	double capacidadeFab=0,capacidadeDep=0;
	for(unsigned i=0; i < n; ++i)
	{
		if(chromosome[i]>=0.5){
			Solution[i] = 1;
		}
		else{
			Solution[i] = 0;
		}

	}

	objval = checador->check(Solution);
	for(unsigned i=0; i < n; ++i){
			if(Solution[i]>0){
				chromosome[i] = rng->randExc(0.5) +0.5;
				
			}
			else{
				chromosome[i] = rng->randExc(0.5);
			}

	}
	
	//cout << bestcost << endl;
	return objval;
}

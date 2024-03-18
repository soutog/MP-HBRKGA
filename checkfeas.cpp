#include "checkfeas.h"
#include <ctime>

CheckFeas::CheckFeas(Instance *_p,vector<GraphCostTS*> _g):g(_g)
{
	p =_p;
}


double CheckFeas::CalculaCustoAbertura(std::vector<int> &solution)
{
    double custo_abertura = 0;
	//Calculo do custo de abertura de fabricas
    for (int i=0; i<p->nfactories; i++){
		if (solution[i] > 0){
			custo_abertura = custo_abertura + p->fixedcost_factory[i];
		}	
	}
	
	//Calculo do custo de abertura de depositos
	for (int j=p->nfactories; j<p->nfactories+p->nwarehouses; j++){
		if (solution[j] > 0){
			custo_abertura = custo_abertura +  p->fixedcost_warehouse[j-p->nfactories];
		}				
	}
	return custo_abertura;

}

bool CheckFeas::CalculaCapacidade(std::vector<int> &solution)
{
    int capacidade = 0;
	for(int l =0;l < p->ncommodities;l++){
		capacidade = 0;
		for (int i=0; i<p->nfactories; i++){
		if (solution[i] > 0){
			capacidade +=  p->factory_capacity[i][l];
			}	
		}
		if (capacidade < p->t_customer_demand_commodities[l]) return false;
	}
	
	for(int l =0;l < p->ncommodities;l++){
		capacidade = 0;
		for (int j=0; j < p->nwarehouses; j++){
			if (solution[p->nfactories+j] > 0){
				capacidade +=  p->warehouse_capacity[j][l];
			}				
		}
		if (capacidade < p->t_customer_demand_commodities[l]) return false;
	}
	return true;

}
double CheckFeas::CalculateFlow(std::vector<int> &solution){
	double total_cost = 0;
	double cost = 0;
	vector<int> sbar(p->nfactories+p->nwarehouses);
	vector <int> solutionfinal(p->nfactories+p->nwarehouses,0);
	for(int i=0; i < p->ncommodities;++i){
		g[i]->solve(solution);
		tie(cost,sbar) = g[i]->getsol();
		for(int j=0; j < p->nfactories+p->nwarehouses;++j){
			solutionfinal[j] = solutionfinal[j] || sbar[j]; 
		} 
		total_cost += cost;
	}
	solution = solutionfinal;
	for(int i=0; i < p->nfactories+p->nwarehouses;++i){
		solution[i] = solutionfinal[i]; 
	} 
	return total_cost;
}

double CheckFeas::check(std::vector<int> & solution)
{
   double obj;
   double flow_obj;
   vector <int> SolutionFinal;
   bool recebe_programa = CalculaCapacidade(solution);
   
   if(recebe_programa){
	   
        obj = CalculateFlow(solution);
        //cout << obj << endl;
		obj += CalculaCustoAbertura(solution);
        //cout << obj << endl;
        return obj;
    }
    obj = 500000000;
	
    return obj;
}

CheckFeas::~CheckFeas(){


}
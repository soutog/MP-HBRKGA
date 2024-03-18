#include "heuristic.h"


Heuristic::Heuristic(Instance *p_,vector<GraphCostTS *> _g)
{
	p = p_;
    S.resize(p->nfactories+p->nwarehouses);
    cap_fab_abe.resize(p->nfactories);
    cap_dep_abe.resize(p->nwarehouses);
	hcapfab.resize(p->ncommodities,vector<double>(p->nfactories));
	hcapdep.resize(p->ncommodities,vector<double>(p->nwarehouses));
	CostBenCalculate();
    srand(clock());
    is_feasible = new CheckFeas(p,_g);
}
Heuristic::~Heuristic(){
	delete(is_feasible);
}

int Heuristic::aleatorio(int lower,int upper){
   
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(lower,upper-1);
    
    cout <<distribution(generator) << endl;
    return distribution(generator);
}

void Heuristic::run(double alpha)
{
    
    construtivo_fab(alpha);
    construtivo_dep(alpha);
    // cout << "solution" << endl;
	// for (int s: S){
	// 	cout << s << ", ";
	// }
	// cout << endl;
	// cout << S.size() << endl;
    //cost = is_feasible->check(S);
}
void Heuristic::CostBenCalculate(){
	//Calculo do melhor custo beneficio de abertura da  fabrica
	for (int i=0;i<p->nfactories;i++){
		for (int l =0; l < p->ncommodities; l++){
			int flow_cost = 0;
			for (int j = 0; j < p->nwarehouses; j++){
				flow_cost = flow_cost + p->flowcost_fw[l][i][j];
			}
			hcapfab[l][i] = double((p->fixedcost_factory[i] + flow_cost))/double(p->factory_capacity[i][l]);
	    }
		
	}


	//Calculo do melhor custo beneficio de abertura de deposito
	for (int j=0; j<p->nwarehouses;j++){
		    int flow_cost = 0;
			for (int l=0; l < p->ncommodities;l++){
				int flow_cost = 0;
				for (int k = 0; k < p->ncustomers; k++){
					flow_cost = flow_cost + p->flowcost_wc[l][j][k];
				}
				hcapdep[l][j] = double((p->fixedcost_warehouse[j] + flow_cost))/double(p->warehouse_capacity[j][l]);
	    	}
			
	}

	
}


void Heuristic::construtivo_fab(double alpha){

	double capacidade_total_fab = 0;  // inicialmente todas as fabricas estao fechadas
	int c;
	int lcr[p->nfactories];
	int random;
	double hmin, hmax;
	double epsilon = 1.0E-5;
    
    
    
	for (int l = 0; l < p->ncommodities; l++){
		capacidade_total_fab =0;
		// zera capacidade da fabrica
		std::fill(cap_fab_abe.begin(), cap_fab_abe.end(), 0);
		
		//Abre os depositos com base no melhor custo beneficio
		while (capacidade_total_fab <= p->t_customer_demand_commodities[l]){
			//Emite lista de candidatos restrita
			int count = 0;
			for(int i=0;i<p->nfactories;i++){
				if(cap_fab_abe[i] == 0){ // fabrica esta fechada pegar o valor e colocar no custo min e max -> adaptativo!! uma nova lista
					if (count == 0){
						hmin = hcapfab[l][i];
						hmax = hcapfab[l][i];
						count ++;
					}
					if(hcapfab[l][i] < hmin){ // menor dos custos das fabricas fechadas
						hmin = hcapfab[l][i]; 
					}
					if(hcapfab[l][i] > hmax){ // maior dos custos das fabricas fechadas
						hmax = hcapfab[l][i]; 
					}
				}
			}

			c = 0;

			for (int i=0;i<p->nfactories;i++){
				if(cap_fab_abe[i] == 0){	// fabrica esta fechada pegar o valor e colocar no custo min e max -> adaptativo!! uma nova lista
					if(((hmin - hcapfab[l][i]) < epsilon) && ((hcapfab[l][i] - (hmax - alpha*(hmax - hmin)) < epsilon))){
					lcr[c] = i;
					c++;				
					}
				}
			}
			
			//random = aleatorio(0,l); // escolhe uma valor aleatorio na lista de candidatos restrita
			random = rand()%c; 
			S[lcr[random]] = 1;
			cap_fab_abe[lcr[random]] =  p->factory_capacity[lcr[random]][l];
			capacidade_total_fab += p->factory_capacity[lcr[random]][l]; 
			
		}
	}
    
}

void Heuristic::construtivo_dep(double alpha){
	
	int c;
	int lcr[p->nwarehouses];
	int random;
	double hmin, hmax;
	double epsilon = 1.0E-5;
  
    for (int l = 0; l < p->ncommodities; l++){
		double capacidade_total_dep =0;
		// zera capacidade do deposito
		std::fill(cap_dep_abe.begin(), cap_dep_abe.end(), 0);
		//Abre os depositos com base no melhor custo beneficio
		while (capacidade_total_dep <= p->t_customer_demand_commodities[l]){
		int count = 0;
		
		//Emite lista de candidatos restrita
			for(int j=0;j<p->nwarehouses;j++){ 
				if(cap_dep_abe[j] == 0){ // adaptativo!! uma nova lista 
					if (count == 0){
						hmin = hcapdep[l][j];
						hmax = hcapdep[l][j];
						count ++;
					}	
					if(hcapdep[l][j] < hmin){
						hmin = hcapdep[l][j]; 
					}
					if(hcapdep[l][j] > hmax){
						hmax = hcapdep[l][j]; 
					}
				}
			}

			c = 0;
		
			for (int i=0;i<p->nwarehouses;i++){
				if(cap_dep_abe[i] == 0){	// deposito já esta aberto (retirar da lista de candidatos - lcr) -> adaptativo!! uma nova lista - nao calcula mais nada sobre ela
					if(((hmin - hcapdep[l][i]) < epsilon) && ((hcapdep[l][i] - (hmax - alpha*(hmax - hmin)) < epsilon))){
					lcr[c] = i;
					c++;				
					}
				}
			}			
			
			//random = aleatorio(0,l);
			random = (rand() % c);

			S[p->nfactories+lcr[random]] = 1;
			cap_dep_abe[lcr[random]] =  p->warehouse_capacity[lcr[random]][l];
			capacidade_total_dep += p->warehouse_capacity[lcr[random]][l]; 
		
		}
	}
        
}

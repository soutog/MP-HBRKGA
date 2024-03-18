#include "localsearch.h"
#include <numeric>


LocalSearch::LocalSearch(Instance *p_,vector <GraphCostTS *> _g):p(p_)
{
	S.resize(p->nfactories+p->nwarehouses);
    srand(clock());
    is_feasible = new CheckFeas(p,_g);
	capFab.resize(p->ncommodities);
	capDep.resize(p->ncommodities);
	capacityCalc.resize(p->ncommodities);
	//functionvectorRVND.emplace_back(&LocalSearch::NeighborhoodSwap);
	functionvectorRVND.emplace_back(&LocalSearch::NeighborhoodClose);
	functionvectorRVND.emplace_back(&LocalSearch::NeighborhoodOpen);
	//functionvectorRVNDFI.emplace_back(&LocalSearch::NeighborhoodSwapFI);
	functionvectorRVNDFI.emplace_back(&LocalSearch::NeighborhoodCloseFI);
	functionvectorRVNDFI.emplace_back(&LocalSearch::NeighborhoodOpenFI);
}
LocalSearch::~LocalSearch(){
	delete (is_feasible);
}
void LocalSearch::sumElements(vector<int> &solution){
	numFabAbe= std::accumulate(solution.begin(),solution.begin()+p->nfactories,0);
	numDepAbe= std::accumulate(solution.begin()+(p->nfactories+1),solution.end(),0);
}

void LocalSearch::CalculaCapacidade(std::vector<int> &solution)
{	
	std::fill(capFab.begin(),capFab.end(),0.0);
    std::fill(capDep.begin(),capDep.end(),0.0);
	for (int l=0; l < p->ncommodities;l++){
		for (int i=0; i<p->nfactories; i++){
			if (solution[i] > 0){
				capFab[l] +=  p->t_factory_capacity[l][i];
			}	
		}
	}
	for (int l=0; l < p->ncommodities;l++){

		for (int j=0; j<p->nwarehouses; j++){
			if (solution[p->nfactories+j] > 0){
				capDep[l] +=  p->t_warehouse_capacity[l][j];
			}				
		}
	}

}

bool LocalSearch::verifyCapacity(vector<int> &solutionCurrent,unsigned i,unsigned j,unsigned neighborhood){
	std::fill(capacityCalc.begin(),capacityCalc.end(),0.0);
	if(neighborhood == 1){
		if(i >= p->nfactories){
			 for (int l = 0; l <p->ncommodities;++l ){
				 capacityCalc[l] = capDep[l] - p->t_warehouse_capacity[l][i - p->nfactories] +p->t_warehouse_capacity[l][j - p->nfactories];
				 if(capacityCalc[l] < p->t_customer_demand_commodities[l]){
					 return false;
				 }
			 }
			 
			 return true;
		}
		for (int l = 0; l <p->ncommodities;++l ){
			capacityCalc[l] = capFab[l] - p->t_factory_capacity[l][i] +p->t_factory_capacity[l][j];
			if(capacityCalc[l] < p->t_customer_demand_commodities[l]){
				return false;
			}
		}
		
		return true;
		
	}
	if(neighborhood == 2){
		if(i >= p->nfactories){
			for (int l = 0; l <p->ncommodities;++l ){
				 capacityCalc[l] = capDep[l] - p->t_warehouse_capacity[l][i - p->nfactories];
				 if(capacityCalc[l] < p->t_customer_demand_commodities[l]){
						return false;
				 }
			}
		
			return true;
		}
		for (int l = 0; l <p->ncommodities;++l ){
				 capacityCalc[l] = capFab[l] - p->t_factory_capacity[l][i] ;
				 if(capacityCalc[l] < p->t_customer_demand_commodities[l]){
						return false;
				 }
		}
		
		return true;
		
	}
	if(i >= p->nfactories){
		for (int l = 0; l <p->ncommodities;++l ){
				 capacityCalc[l] = capDep[l] + p->t_warehouse_capacity[l][i - p->nfactories];
				 if(capacityCalc[l] < p->t_customer_demand_commodities[l]){
						return false;
				 }
		}
		
		return true;
	}
	for (int l = 0; l <p->ncommodities;++l ){
				 capacityCalc[l] = capFab[l] + p->t_factory_capacity[l][i] ;
				 if(capacityCalc[l] < p->t_customer_demand_commodities[l]){
						return false;
				 }
	}
	
	return true;

}

void LocalSearch::solveRVND(vector<int> &solution)
{	
	
	vector <int> Stemp(solution);
	CalculaCapacidade(solution);
	
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int neighborhood = 0;
	int neighborhood_max = functionvectorRVND.size();
	random_shuffle( functionvectorRVND.begin(), functionvectorRVND.end() );
	
	while(neighborhood < neighborhood_max){
		(this->*functionvectorRVND[neighborhood])(Stemp);
		if(cost < costtemp){
			costtemp = cost;
			if(neighborhood > 0 ){
				neighborhood = 0;
			}
		}else{
			neighborhood++;
		}
	}

}
void LocalSearch::NeighborhoodSwap(vector<int> &solution)
{
	vector <int> Stemp(solution);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int i,j, bstI=0,bstJ=0,bstVal = cost;
	while(improved)
	{
		improved = false;
		//Swap de fabricas abertas e fechadas
		clock_t ti = clock();
		for(i=0;i<p->nfactories;++i)
		{
			if(Stemp[i]>0){
				for(j = 0;j<p->nfactories;++j)
				{
						if(Stemp[j]==0){
							Stemp[i] = 0;
							Stemp[j] = 1;
							bool capverified = verifyCapacity(Stemp,i,j,1);
							if(capverified){
								ti = clock();
								costtemp = is_feasible->check(Stemp);
								
								if (costtemp < bstVal){
									// cout << "improved swap fabrica" << endl;
									// cout << "Cost" << cost << endl;
									// cout << "Cost p->nfactoriesmproved" << costtemp << endl;
									improved = true;
									bstVal = costtemp;
									bstI = i;
									bstJ = j;
								}
							}
							costtemp=cost;
							Stemp[i] = 1;
							Stemp[j] = 0;

						}
				}
			}
		}
		
		for(i=0;i< p->nwarehouses;++i){
			if(Stemp[p->nfactories+i]>0){
				for(j = 0;j< p->nwarehouses;++j)
				{
						if(Stemp[p->nfactories+j]==0){
							Stemp[p->nfactories+i] = 0;
							Stemp[p->nfactories+j] = 1;
							bool capDepVerify = verifyCapacity(Stemp,p->nfactories+i,p->nfactories+j,1);
								
							if(capDepVerify){
								costtemp = is_feasible->check(Stemp);
								if (costtemp < bstVal){
									/* cout << "improved swap deposito" << endl;
									cout << "Cost" << cost << endl;
									cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
									improved = true;
									bstVal = costtemp;
									bstI = p->nfactories+i;
									bstJ = p->nfactories+j;
								}
							}
							Stemp[p->nfactories+i] = 1;
							Stemp[p->nfactories+j] = 0;
							costtemp = cost;
						}
				}
			}
			 
		}
		if(improved){
			if(bstI >= p->nfactories){
					Stemp[bstI] = 0;
					Stemp[bstJ] = 1;
					solution[bstI] = 0;
					solution[bstJ] = 1;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capDep[l] = capDep[l] - p->t_warehouse_capacity[l][bstI-p->nfactories]+p->t_warehouse_capacity[l][bstJ-p->nfactories];
			 		}
			}else{
					Stemp[bstI] = 0;
					Stemp[bstJ] = 1;
					solution[bstI] = 0;
					solution[bstJ] = 1;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capFab[l] = capFab[l] - p->t_factory_capacity[l][bstI] + p->t_factory_capacity[l][bstJ];
					}
			}
			cost = bstVal;
		}

	}
	S = solution;


}

void LocalSearch::NeighborhoodClose(vector<int> &solution)
{
	vector <int> Stemp(solution);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int i,j, bstI=0,bstJ=0,bstVal = cost;
	while(improved)
	{
		improved = false;
		//Fechamento de fabricas
		for(i = 0;i<p->nfactories;++i)
		{
			if(numFabAbe > p->piFab){
				if(Stemp[i]>0){
					Stemp[i]=0;
					if(verifyCapacity(Stemp,i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved fechamento fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
							improved = true;
							bstI = i;
							bstJ = -1;	
						}
					}
					Stemp[i]=1;
					costtemp = cost;
				}
			}else{
			
				break;
			}
		}
		//Fechamento de depositos
		for(i = 0;i< p->nwarehouses;++i)
		{
			if(numDepAbe > p->piDep){
				if(Stemp[i+p->nfactories]>0){
					Stemp[i+p->nfactories]=0;
					if(verifyCapacity(Stemp,p->nfactories+i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
						/* 	cout << "improved fechamento deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i+p->nfactories;
							bstJ = -1;
						}
					}
					Stemp[i+p->nfactories]=1;
					costtemp = cost;
				}
			}else{
				break;
			}
		}
		if(improved){
			//cout << "veio aki";
			if(bstI >= p->nfactories){
					Stemp[bstI] = 0;
					solution[bstI] = 0;
					numDepAbe--;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capDep[l] = capDep[l] - p->t_warehouse_capacity[l][bstI - p->nfactories];
			 		}
			}
			else{
					Stemp[bstI] = 0;
					solution[bstI] = 0;
					numFabAbe--;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capFab[l] = capFab[l] - p->t_factory_capacity[l][bstI];
					}
			}
			cost = bstVal;
		}
	}
	S = solution;
}




void LocalSearch::NeighborhoodOpen(vector<int> &solution)
{
	vector <int> Stemp(solution);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int i,j, bstI=0,bstJ=0,bstVal = cost;
	
	while(improved)
	{
		improved = false;
		//Abertura de fabricas
		for(i = 0;i< p->nfactories;++i)
		{
			if(numFabAbe < p->betaFab){
				if(Stemp[i]==0){
					Stemp[i]=1;
					if(verifyCapacity(Stemp,i,j,3)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved abertura fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i;
							bstJ = -1;	
						}
					}
					Stemp[i]=0;
					costtemp = cost;
				}
			}
			else{
				break;
			}
		}
		for(i = 0;i< p->nwarehouses;++i)
		{
			if(numDepAbe < p->betaDep){
				if(Stemp[i+p->nfactories]==0){
					Stemp[i+p->nfactories]=1;
					if(verifyCapacity(Stemp,p->nfactories+i,j,3)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved abertura deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i+p->nfactories;
							bstJ = -1;
							
						}
					}
					Stemp[i+p->nfactories]=0;
					costtemp = cost;
				}
			}else{
				break;
			}
		}
		if(improved){
			//cout << "veio aki";
			if(bstI >= p->nfactories){
					Stemp[bstI] = 1;
					solution[bstI] = 1;
					numDepAbe++;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capDep[l] = capDep[l] + p->t_warehouse_capacity[l][bstI - p->nfactories];
			 		}
			}
			else{
					Stemp[bstI] = 1;
					solution[bstI] = 1;
					numFabAbe++;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capFab[l] = capFab[l] - p->t_factory_capacity[l][bstI];
					}
			}
			cost = bstVal;
		}
	}
	S = solution;

}



void LocalSearch::solve(vector<int> &solution)
{
	vector <int> Stemp(solution);
	S.resize(p->nfactories+p->nwarehouses);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int i,j, bstI=0,bstJ=0,bstVal = cost;
	
	while(improved)
	{
		improved = false;
		//Swap de fabricas abertas e fechadas
		for(i=0;i< p->nfactories;++i)
		{
			if(Stemp[i]>0){
				for(j = 0;j< p->nfactories;++j)
				{
						if(Stemp[j]==0){
							Stemp[i] = 0;
							Stemp[j] = 1;
							if(verifyCapacity(Stemp,i,j,1)){
								costtemp = is_feasible->check(Stemp);
								if (costtemp < bstVal){
									/* cout << "improved swap fabrica" << endl;
									cout << "Cost" << cost << endl;
									cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
									improved = true;
									bstVal = costtemp;
									bstI = i;
									bstJ = j;
								}
							}
							costtemp=cost;
							Stemp[i] = 1;
							Stemp[j] = 0;

						}
				}
			}
		}
		//Fechamento de fabricas
		for(i = 0;i< p->nfactories;++i)
		{
			if(numFabAbe > p->piFab){
				if(Stemp[i]>0){
					Stemp[i]=0;
					if(verifyCapacity(Stemp,i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved fechamento fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
							improved = true;
							bstI = i;
							bstJ = -1;	
						}
					}
					Stemp[i]=1;
					costtemp = cost;
				}
			}else{
			
				break;
			}
		}
		//Abertura de fabricas
		for(i = 0;i< p->nfactories;++i)
		{
			if(numFabAbe < p->betaFab){
				if(Stemp[i]==0){
					Stemp[i]=1;
					if(verifyCapacity(Stemp,i,j,3)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved abertura fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i;
							bstJ = -1;	
						}
					}
					Stemp[i]=0;
					costtemp = cost;
				}
			}
			else{
				break;
			}
		}
		//Swap de depositos abertos e fechados
		for(i=0;i< p->nwarehouses;++i){
			if(Stemp[p->nfactories+i]>0){
				for(j = 0;j< p->nwarehouses;++j)
				{
						if(Stemp[p->nfactories+j]==0){
							Stemp[p->nfactories+i] = 0;
							Stemp[p->nfactories+j] = 1;
							if(verifyCapacity(Stemp,p->nfactories+i,p->nfactories+j,1)){
								costtemp = is_feasible->check(Stemp);
								if (costtemp < bstVal){
									/* cout << "improved swap deposito" << endl;
									cout << "Cost" << cost << endl;
									cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
									improved = true;
									bstVal = costtemp;
									bstI = p->nfactories+i;
									bstJ = p->nfactories+j;
								}
							}
							Stemp[p->nfactories+i] = 1;
							Stemp[p->nfactories+j] = 0;
							costtemp = cost;
						}
				}
			}
			 
		}
		//Fechamento de depositos
		for(i = 0;i< p->nwarehouses;++i)
		{
			if(numDepAbe > p->piDep){
				if(Stemp[i+p->nfactories]>0){
					Stemp[i+p->nfactories]=0;
					if(verifyCapacity(Stemp,p->nfactories+i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
						/* 	cout << "improved fechamento deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i+p->nfactories;
							bstJ = -1;
						}
					}
					Stemp[i+p->nfactories]=1;
					costtemp = cost;
				}
			}else{
				break;
			}
		}
		
		for(i = 0;i< p->nwarehouses;++i)
		{
			if(numDepAbe < p->betaDep){
				if(Stemp[i+p->nfactories]==0){
					Stemp[i+p->nfactories]=1;
					if(verifyCapacity(Stemp,p->nfactories+i,j,3)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved abertura deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i+p->nfactories;
							bstJ = -1;
							
						}
					}
					Stemp[i+p->nfactories]=0;
					costtemp = cost;
				}
			}else{
				break;
			}
		}

		if(improved){
			//cout << "veio aki";
			if(bstI > p->nfactories){
				if(bstJ== -1 && solution[bstI] == 0){
					Stemp[bstI] = 1;
					solution[bstI] = 1;
					numDepAbe++;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capDep[l] = capDep[l] + p->t_warehouse_capacity[l][bstI-p->nfactories];
			 		}
				}else if(bstJ== -1){
					Stemp[bstI] = 0;
					solution[bstI] = 0;
					numDepAbe--;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capDep[l] = capDep[l] - p->t_warehouse_capacity[l][bstI-p->nfactories];
			 		}
				}else{
					Stemp[bstI] = 0;
					Stemp[bstJ] = 1;
					solution[bstI] = 0;
					solution[bstJ] = 1;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capDep[l] = capDep[l] - p->t_warehouse_capacity[l][bstI-p->nfactories] + p->t_warehouse_capacity[l][bstJ-p->nfactories];
			 		}
				}
			}else{
				if(bstJ== -1 && solution[bstI] == 0){
					Stemp[bstI] = 1;
					solution[bstI] = 1;
					numFabAbe++;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capFab[l] = capFab[l] + p->t_factory_capacity[l][bstI];
					}
				}else if(bstJ== -1){
					Stemp[bstI] = 0;
					solution[bstI] = 0;
					numFabAbe--;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capFab[l] = capFab[l] - p->t_factory_capacity[l][bstI];
					}
				}else{
					Stemp[bstI] = 0;
					Stemp[bstJ] = 1;
					solution[bstI] = 0;
					solution[bstJ] = 1;
					for (int l = 0; l <p->ncommodities;++l ){
				 		capFab[l] = capFab[l] - p->t_factory_capacity[l][bstI] + p->t_factory_capacity[l][bstJ];
					}
				}
			}
			cost = bstVal;
		}
	}
    S = solution;

    
}

void LocalSearch::solveFI(vector<int> &solution)
{
	vector <int> Stemp(solution);
	sumElements(solution);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	int costtemp = cost;
	bool improved = true;
	int i,j;
	
	
	while(improved)
	{
		improved = false;
		//Swap de fabricas abertas e fechadas
		for(i=0;i< p->nfactories;++i)
		{
			if(Stemp[i]>0){
				for(j = 0;j< p->nfactories;++j)
				{
					if(i != j){
						if(Stemp[j]==0){
							Stemp[i] = 0;
							Stemp[j] = 1;
							if(verifyCapacity(Stemp,i,j,1)){
								costtemp = is_feasible->check(Stemp);
								if (costtemp < cost){
									cout << "improved swap fabrica" << endl;
									cout << "Cost" << cost << endl;
									cout << "Cost p->nfactoriesmproved" << costtemp << endl;
									improved = true;
									cost = costtemp;
									solution[i] = 0;
									solution[j] = 1;
									//goto SolutionEnd;
								}else{
									costtemp=cost;
									Stemp[i] = 1;
									Stemp[j] = 0;
								}
							}else{
								costtemp=cost;
								Stemp[i] = 1;
								Stemp[j] = 0;
							}

						}
					}
				}
			}
		}
		//Fechamento de fabricas
		for(i = 0;i< p->nfactories;++i)
		{
			if(numFabAbe > p->piFab){
				if(Stemp[i]>0){
					Stemp[i]=0;
					if(verifyCapacity(Stemp,i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < cost){
							cout << "improved fechamento fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i] = 0;
							numFabAbe--;
						}
						else{
							Stemp[i]=1;
							costtemp = cost;
						}
					}else{
						Stemp[i]=1;
						costtemp = cost;
					}
				}
			}else{
			
				break;
			}
		}
		//Abertura de fabricas
		for(i = 0;i< p->nfactories;++i)
		{
			if(numFabAbe < p->betaFab){
				if(Stemp[i]==0){
					Stemp[i]=1;
					if(verifyCapacity(Stemp,i,j,3)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < cost){
							cout << "improved abertura fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i] = 1;
							numFabAbe++;
							//goto SolutionEnd;	
						}
						else{
							Stemp[i]=0;
							costtemp = cost;
						}
					}else{
							Stemp[i]=0;
							costtemp = cost;
					}
				}
			}
			else{
				break;
			}
		}
		//Swap de depositos abertos e fechados
		for(i=0;i< p->nwarehouses;++i){
			if(Stemp[p->nfactories+i]>0){
				for(j = 0;j< p->nwarehouses;++j)
				{
					if(Stemp[p->nfactories+j]==0){
						Stemp[p->nfactories+i] = 0;
						Stemp[p->nfactories+j] = 1;
						if(verifyCapacity(Stemp,p->nfactories+i,p->nfactories+j,1)){
							costtemp = is_feasible->check(Stemp);
							if (costtemp < cost){
								cout << "improved swap deposito" << endl;
								cout << "Cost" << cost << endl;
								cout << "Cost p->nfactoriesmproved" << costtemp << endl;
								improved = true;
								cost=costtemp;
								solution[p->nfactories+i] = 0;
								solution[p->nfactories+j] = 1;
								//goto SolutionEnd;
							}else{
								costtemp=cost;
								Stemp[p->nfactories+i] = 1;
								Stemp[p->nfactories+j] = 0;
							}
						}else{
								costtemp=cost;
								Stemp[p->nfactories+i] = 1;
								Stemp[p->nfactories+j] = 0;
						}
					
					}
				}
			}
			
		} 
		//Fechamento de depositos
		for(i = 0;i< p->nwarehouses;++i)
		{
			if(numDepAbe > p->piDep){
				if(Stemp[i+p->nfactories]>0){
					Stemp[i+p->nfactories]=0;
					if(verifyCapacity(Stemp,p->nfactories+i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < cost){
							cout << "improved fechamento deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i+p->nfactories] = 0;
							numDepAbe--;
							//goto SolutionEnd;	
						}
						else{
							Stemp[i+p->nfactories]=1;
							costtemp = cost;
						}
					}else{
							Stemp[i+p->nfactories]=1;
							costtemp = cost;
					}
				}
			}else{
				break;
			}
		}
		
		for(i = 0;i< p->nwarehouses;++i)
		{
			if(numDepAbe < p->betaDep){
				if(Stemp[i+p->nfactories]==0){
					Stemp[i+p->nfactories]=1;
					costtemp = is_feasible->check(Stemp);
					if(verifyCapacity(Stemp,p->nfactories+i,j,3)){
						if (costtemp < cost){
							cout << "improved abertura deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost p->nfactoriesmproved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i+p->nfactories] = 1;
							numDepAbe++;
							//goto SolutionEnd;	
						}
						else{
							Stemp[i+p->nfactories]=0;
							costtemp = cost;
						}
					}else{
							Stemp[i+p->nfactories]=0;
							costtemp = cost;
					}
				}
			}else{
				break;
			}
		}

	
	
	}
   S = solution;

    
}


void LocalSearch::solveRVNDFI(vector<int> &solution){
	vector <int> Stemp(solution);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int neighborhood = 0;
	int neighborhood_max = functionvectorRVNDFI.size();
	random_shuffle( functionvectorRVNDFI.begin(), functionvectorRVNDFI.end() );
	while(neighborhood < neighborhood_max){
		(this->*functionvectorRVNDFI[neighborhood])(Stemp);
		if(cost < costtemp){
			costtemp = cost;
			if(neighborhood > 0 ){
				neighborhood = 0;
			}
		}else{
			neighborhood++;
		}
	}
}

void LocalSearch::NeighborhoodSwapFI(vector<int>& solution){
	
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	vector <int> Stemp(solution);
	int i,j;
	
	while(improved)
	{
		improved = false;
		for(i=0;i< p->nwarehouses;++i){
			if(Stemp[p->nfactories+i]>0){
				for(j = 0;j< p->nwarehouses;++j)
				{
					if(Stemp[p->nfactories+j]==0){
						Stemp[p->nfactories+i] = 0;
						Stemp[p->nfactories+j] = 1;
						if(verifyCapacity(Stemp,p->nfactories+i,p->nfactories+j,1)){
							costtemp = is_feasible->check(Stemp);
							if (costtemp < cost){
								// cout << "improved swap deposito" << endl;
								// cout << "Cost" << cost << endl;
								// cout << "Cost p->nfactoriesmproved" << costtemp << endl;
								improved = true;
								cost=costtemp;
								solution[p->nfactories+i] = 0;
								solution[p->nfactories+j] = 1;
								//goto SolutionEnd;
							}else{
								costtemp=cost;
								Stemp[p->nfactories+i] = 1;
								Stemp[p->nfactories+j] = 0;
							}
						}else{
								costtemp=cost;
								Stemp[p->nfactories+i] = 1;
								Stemp[p->nfactories+j] = 0;
						}
					
					}
				}
			}
			
		} 
		for(i=0;i< p->nwarehouses;++i){
			if(Stemp[p->nfactories+i]>0){
				for(j = 0;j< p->nwarehouses;++j)
				{
					if(Stemp[p->nfactories+j]==0){
						Stemp[p->nfactories+i] = 0;
						Stemp[p->nfactories+j] = 1;
						if(verifyCapacity(Stemp,p->nfactories+i,p->nfactories+j,1)){
							costtemp = is_feasible->check(Stemp);
							if (costtemp < cost){
								// cout << "improved swap deposito" << endl;
								// cout << "Cost" << cost << endl;
								// cout << "Cost p->nfactoriesmproved" << costtemp << endl;
								improved = true;
								cost=costtemp;
								solution[p->nfactories+i] = 0;
								solution[p->nfactories+j] = 1;
								//goto SolutionEnd;
							}else{
								costtemp=cost;
								Stemp[p->nfactories+i] = 1;
								Stemp[p->nfactories+j] = 0;
							}
						}else{
								costtemp=cost;
								Stemp[p->nfactories+i] = 1;
								Stemp[p->nfactories+j] = 0;
						}
					
					}
				}
			}
			
		} 
			
	}
	S = solution;
}


void LocalSearch::NeighborhoodCloseFI(vector<int>& solution){

	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	vector <int> Stemp(solution);
	int i,j;
	
	while(improved)
	{
		improved = false;
		for(i = 0;i< p->nfactories;++i)
		{
			if(numFabAbe > p->piFab){
				if(Stemp[i]>0){
					Stemp[i]=0;
					if(verifyCapacity(Stemp,i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < cost){
							// cout << "improved fechamento fabrica" << endl;
							// cout << "Cost" << cost << endl;
							// cout << "Cost p->nfactoriesmproved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i] = 0;
							numFabAbe--;
						}
						else{
							Stemp[i]=1;
							costtemp = cost;
						}
					}else{
						Stemp[i]=1;
						costtemp = cost;
					}
				}
			}else{
			
				break;
			}
		}
	for(i = 0;i< p->nwarehouses;++i)
		{
			if(numDepAbe > p->piDep){
				if(Stemp[i+p->nfactories]>0){
					Stemp[i+p->nfactories]=0;
					if(verifyCapacity(Stemp,p->nfactories+i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < cost){
							// cout << "improved fechamento deposito" << endl;
							// cout << "Cost" << cost << endl;
							// cout << "Cost p->nfactoriesmproved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i+p->nfactories] = 0;
							numDepAbe--;
							//goto SolutionEnd;	
						}
						else{
							Stemp[i+p->nfactories]=1;
							costtemp = cost;
						}
					}else{
							Stemp[i+p->nfactories]=1;
							costtemp = cost;
					}
				}
			}else{
				break;
			}
		}
		

	}
	S = solution;

}

void LocalSearch::NeighborhoodOpenFI(vector<int>& solution){
	
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	vector <int> Stemp(solution);
	int i,j;
	
	while(improved)
	{
		improved = false;
		for(i = 0;i< p->nwarehouses;++i)
		{
			if(numDepAbe < p->betaDep){
				if(Stemp[i+p->nfactories]==0){
					Stemp[i+p->nfactories]=1;
					costtemp = is_feasible->check(Stemp);
					if(verifyCapacity(Stemp,p->nfactories+i,j,3)){
						if (costtemp < cost){
							// cout << "improved abertura deposito" << endl;
							// cout << "Cost" << cost << endl;
							// cout << "Cost p->nfactoriesmproved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i+p->nfactories] = 1;
							numDepAbe++;
							//goto SolutionEnd;	
						}
						else{
							Stemp[i+p->nfactories]=0;
							costtemp = cost;
						}
					}else{
							Stemp[i+p->nfactories]=0;
							costtemp = cost;
					}
				}
			}else{
				break;
			}
		}



		for(int i = 0;i< p->nwarehouses;++i)
		{
			if(numDepAbe < p->betaDep){
				if(Stemp[i+p->nfactories]==0){
					Stemp[i+p->nfactories]=1;
					costtemp = is_feasible->check(Stemp);
					if(verifyCapacity(Stemp,p->nfactories+i,j,3)){
						if (costtemp < cost){
							// cout << "improved abertura deposito" << endl;
							// cout << "Cost" << cost << endl;
							// cout << "Cost p->nfactoriesmproved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i+p->nfactories] = 1;
							numDepAbe++;
							//goto SolutionEnd;	
						}
						else{
							Stemp[i+p->nfactories]=0;
							costtemp = cost;
						}
					}else{
							Stemp[i+p->nfactories]=0;
							costtemp = cost;
					}
				}
			}else{
				break;
			}
		}

	}
	S = solution;
}
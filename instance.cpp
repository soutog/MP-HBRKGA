#include "instance.h"

double min(double a, double b)
{
	return (a<b) ? a : b;
}

void Instance::load_file(const char *const _file_name)
{
	file_name = _file_name;

	ifstream f_inst(file_name);
	if (!f_inst.is_open())
	{
		cout << "ERROR: File " << file_name << " not found!" << endl;
		exit(0);
	}

	TotalCapFac = 0;
	TotalCapWare = 0;

	f_inst >> nfactories;
	f_inst >> nwarehouses;
	f_inst >> ncustomers;
	f_inst >> ncommodities;
	

	customer_demand = new double * [ncustomers];
	for(int k = 0; k<ncustomers; ++k){
		customer_demand[k] =  new double [ncommodities];
		for(int l=0; l<ncommodities; ++l){
			f_inst >> customer_demand[k][l];
		}
	}

	fixedcost_factory = new double [nfactories];
	factory_capacity = new double * [nfactories];
	for(int i = 0; i<nfactories; ++i){
		factory_capacity[i] = new double [ncommodities];
		for(int l=0; l<ncommodities; ++l){
			f_inst >> factory_capacity[i][l];
			TotalCapFac += factory_capacity[i][l];
		}
		f_inst >> fixedcost_factory[i];
	}

	flowcost_fw = new double ** [ncommodities];
	for(int l=0; l<ncommodities; ++l){
		flowcost_fw[l] = new double * [nfactories];
		for(int i = 0; i<nfactories; ++i){
			flowcost_fw[l][i] = new double [nwarehouses];
			for(int j = 0; j<nwarehouses; ++j){
				f_inst >> flowcost_fw[l][i][j];
			}
		}
	}

	fixedcost_warehouse = new double [nwarehouses];
	warehouse_capacity = new double * [nwarehouses];
	for(int j = 0; j<nwarehouses; ++j){
		warehouse_capacity[j] = new double [ncommodities];
		for(int l=0; l<ncommodities; ++l){
			f_inst >> warehouse_capacity[j][l];
			TotalCapWare += warehouse_capacity[j][l];
		}
		f_inst >> fixedcost_warehouse[j];
	}

	flowcost_wc = new double ** [ncommodities];
	for(int l=0; l<ncommodities; ++l){
		flowcost_wc[l] = new double * [nwarehouses];
		for(int j = 0; j<nwarehouses; ++j){
			flowcost_wc[l][j] = new double [ncustomers];
			for(int k = 0; k<ncustomers; ++k){
				f_inst >> flowcost_wc[l][j][k];
			}
		}
	}	

	//Estrutura de Dados Auxiliar
	t_customer_demand_commodities = new double [ncommodities];
	t_customer_demand = new double * [ncommodities];
	t_factory_capacity = new double * [ncommodities];
	t_warehouse_capacity = new double * [ncommodities];

	for(int l=0; l<ncommodities; ++l)
	{
		t_customer_demand[l] = new double [ncustomers];
		for(int k = 0; k<ncustomers; ++k)
		{
			t_customer_demand_commodities[l] = t_customer_demand_commodities[l] + customer_demand[k][l];
			t_customer_demand[l][k] = customer_demand[k][l];
		}

		t_factory_capacity[l] = new double [nfactories];
		for(int i = 0; i<nfactories; ++i)
		{
			t_factory_capacity[l][i] = factory_capacity[i][l];
		}

		t_warehouse_capacity[l] = new double [nwarehouses];
		for(int j = 0; j<nwarehouses; ++j)
		{	
			t_warehouse_capacity[l][j] = warehouse_capacity[j][l];
		}

	}
	//ncommodities = 1;
	//print_instance();
}


Instance::~Instance()
{
	//cout << "Deleting Instance ... ";

	for(int k = 0; k<ncustomers; ++k){
		delete[] customer_demand[k];
	}
	delete[] customer_demand;

	for(int i = 0; i<nfactories; ++i){
		delete[] factory_capacity[i];
	}
	delete[] factory_capacity;

	for(int j = 0; j<nwarehouses; ++j){
		delete[] warehouse_capacity[j];
	}
	delete[] warehouse_capacity;


	delete[] fixedcost_factory;
	delete[] fixedcost_warehouse;
	

	for(int l=0; l<ncommodities; ++l){
		for(int i = 0; i<nfactories; ++i){
			delete[] flowcost_fw[l][i];
		}
		for(int j = 0; j<nwarehouses; ++j){
			delete[] flowcost_wc[l][j];
		}
		delete[] flowcost_fw[l];
		delete[] flowcost_wc[l]; 

		delete[] t_customer_demand[l];
		delete[] t_factory_capacity[l];
		delete[] t_warehouse_capacity[l];
	}

	delete[] flowcost_fw;
	delete[] flowcost_wc; 

	delete[] t_customer_demand;
	delete[] t_factory_capacity;
	delete[] t_warehouse_capacity;
	delete[] t_customer_demand_commodities;

	//cout << "done" << endl;
}


void Instance::print_instance(){
	cout<<"+=======================================+"<<endl;
	cout<<"+===============Instancia===============+"<<endl;
	cout<<"+=======================================+"<<endl;
	cout <<	nfactories << " " << nwarehouses << " " << ncustomers << " " << ncommodities << endl;
	for(int k = 0; k<ncustomers; ++k){
		for(int l=0; l<ncommodities; ++l){
			cout << customer_demand[k][l] << " ";
		}
		cout << endl;
	}

	for(int i = 0; i<nfactories; ++i){
		for(int l=0; l<ncommodities; ++l){
			cout << factory_capacity[i][l] << " ";
		}
		cout << fixedcost_factory[i] << endl;
	}

	for(int l=0; l<ncommodities; ++l){
		for(int i = 0; i<nfactories; ++i){
			for(int j = 0; j<nwarehouses; ++j){
				cout << flowcost_fw[l][i][j] << " ";
			}
			cout << endl;
		}
	}

	for(int j = 0; j<nwarehouses; ++j){
		for(int l=0; l<ncommodities; ++l){
			cout << warehouse_capacity[j][l] << " ";
		}
		cout << fixedcost_warehouse[j] <<endl;
	}

	for(int l=0; l<ncommodities; ++l){
		for(int j = 0; j<nwarehouses; ++j){
			for(int k = 0; k<ncustomers; ++k){
				cout << flowcost_wc[l][j][k] << " ";
			}
			cout << endl;
		}
	}	


	cout<<"+=======================================+"<<endl;
	cout<<"+===============Transpostas=============+"<<endl;
	cout<<"+=======================================+"<<endl;


	for(int l=0; l<ncommodities; ++l)
	{
		
		for(int k = 0; k<ncustomers; ++k)
		{
			cout<< t_customer_demand[l][k] << " ";
		}
		cout<<endl;

	}

	for(int l=0; l<ncommodities; ++l)
	{
		

		for(int i = 0; i<nfactories; ++i)
		{
			cout<< t_factory_capacity[l][i] << " ";
		}
		cout<<endl;
	
	}

	for(int l=0; l<ncommodities; ++l)
	{
		

		for(int j = 0; j<nwarehouses; ++j)
		{	
			cout<< t_warehouse_capacity[l][j] << " ";
		}
		
		cout<<endl;

	}


}

void Instance::calculaMinMaxFabDep()
{
	vector<int> betaFabVec(ncommodities,0);
	vector<int> piFabVec(ncommodities,0);
	vector<int> betaDepVec(ncommodities,0);
	vector<int> piDepVec(ncommodities,0);
	for(int l =0; l < ncommodities;++l){
		std::vector<int> fab(nfactories);
		for(int i =0; i < nfactories; ++i)
		{
			fab[i] = factory_capacity[l][i];
		}
		std::sort(fab.begin(),fab.end());
		std::vector<int> dep(nwarehouses);
		for(int i =0; i < nwarehouses; ++i)
		{
			dep[i] = warehouse_capacity[l][i];
		}
		std::sort(dep.begin(),dep.end());
		int capacidade = 0;
		for(int i = 0; i < nfactories; ++i){
			if(capacidade < t_customer_demand_commodities[l]){
				betaFabVec[l]++;
				capacidade +=fab[i];
			}else break;
		}
		capacidade = 0;
		for(int i = nfactories-1; i >= 0; --i){
			if(capacidade < t_customer_demand_commodities[l]){
				piFabVec[l]++;
				capacidade +=fab[i];
			}else break;
		}
		
		capacidade = 0;
		for(int i = 0; i < nwarehouses; ++i){
			if(capacidade < t_customer_demand_commodities[l]){
				betaDepVec[l]++;
				capacidade +=dep[i];
			}else break;
		}
		capacidade = 0;
		for(int i = nwarehouses-1; i >= 0; --i){
			if(capacidade < t_customer_demand_commodities[l]){
				piDepVec[l]++;
				capacidade +=dep[i];
			}else break;
		}	
	}
	
	betaFab = *max_element(betaFabVec.begin(),betaFabVec.end());
	piFab = *min_element(piFabVec.begin(),piFabVec.end());

	betaDep = *max_element(betaDepVec.begin(),betaDepVec.end());
	piDep = *min_element(piDepVec.begin(),piDepVec.end());


}

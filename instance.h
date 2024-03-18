#ifndef INSTANCE_H
#define INSTANCE_H
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <exception>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

enum status {OPTIMALFOUND, SOLUTIONFOUND, INFEASIBLE, NOTFOUND};

class Instance{
	
public:
	int nfactories;
	int nwarehouses;
	int ncustomers;
	int ncommodities;

	double ** customer_demand;
	
	double ** warehouse_capacity;
	double ** factory_capacity;

	double * t_customer_demand_commodities;
	double ** t_customer_demand;
	double ** t_warehouse_capacity;
	double ** t_factory_capacity;

	double * fixedcost_factory;
	double * fixedcost_warehouse;

	double *** flowcost_fw;
	double *** flowcost_wc;

	double TotalCapFac;
	double TotalCapWare;

	const char* file_name;
	int betaFab;
	int piFab;
	int betaDep;
	int piDep;

	void load_file(const char *const _file_name);
	void print_instance();
	void calculaMinMaxFabDep();


	~Instance();
	
};
#endif
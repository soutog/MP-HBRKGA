
#include <iostream>
#include <cmath>
#include "ExactMPTSCFL.h"




using namespace std;



ExactMPTSCFL::ExactMPTSCFL(Instance *instance):instance(instance){}

ExactMPTSCFL::~ExactMPTSCFL(){ env.end();}

void ExactMPTSCFL::addConstraintFluxoDepositoCliente(IloEnv &env,IloModel &model,IloNumVar3Matrix &s,Instance *instance){
    for(int k= 0; k<instance->ncustomers;++k){
        for(int l=0; l < instance->ncommodities; l++){
            IloExpr summation(env);
            for(int j=0;j < instance->nwarehouses;++j){ 
                summation +=s[j][k][l];
            }
            summation -=instance->customer_demand[k][l];
            IloRange constr_fluxo_deposito_cliente(env, 0, summation, IloInfinity);
            stringstream name;
            name << "FluxoDepositoClientepelaCommoditie(" << k << ")(" << l <<")";
            constr_fluxo_deposito_cliente.setName(name.str().c_str());
            model.add(constr_fluxo_deposito_cliente);

        }
        
    }

}
void ExactMPTSCFL::addConstraintFluxoFabricaDeposito(IloEnv &env,IloModel &model,IloNumVar3Matrix &x,IloNumVar3Matrix &s){
    for(int j= 0; j<instance->nwarehouses;++j){
        for(int l=0; l < instance->ncommodities; l++){
            IloExpr summationFab(env);
            IloExpr summationDep(env);
            IloExpr constraint(env);
            for(int i=0;i <instance->nfactories;++i){ 
                summationFab +=x[i][j][l];
            }
            for(int k=0;k <instance->ncustomers;++k){ 
                summationDep +=s[j][k][l];
            }
            constraint = summationFab - summationDep;
            IloRange constr_fluxo_fabrica_deposito(env, 0, constraint, IloInfinity);
            stringstream name;
            name << "FluxoFabricaDepositopelaCommoditie(" << j << ")(" << l <<")";
            constr_fluxo_fabrica_deposito.setName(name.str().c_str());
            model.add(constr_fluxo_fabrica_deposito);
        }
    }
}
void ExactMPTSCFL::addConstraintLimiteCapacidadeFabrica(IloEnv &env,IloModel &model,IloIntVarArray &y,IloNumVar3Matrix &x,Instance *instance){
    for(int i= 0; i<instance->nfactories;++i){
        for(int l=0; l < instance->ncommodities; l++){
            IloExpr summation(env);
            for(int j=0;j < instance->nwarehouses;++j){ 
                summation +=x[i][j][l];
            }
            summation -= instance->factory_capacity[i][l] * y[i];
            IloRange constr_limite_capacidade_fabrica(env, -IloInfinity, summation, 0);
            stringstream name;
            name << "LimiteCapacidadeFabricapelaCommoditie(" << i << ")("<< l <<")";
            constr_limite_capacidade_fabrica.setName(name.str().c_str());
            model.add(constr_limite_capacidade_fabrica);
        }
    }
}
void ExactMPTSCFL::addConstraintLimiteCapacidadeDeposito(IloEnv &env,IloModel &model,IloIntVarArray &z,IloNumVar3Matrix &s,Instance *instance){
     for(int j= 0; j<instance->nwarehouses;++j){
        for(int l=0; l < instance->ncommodities; l++){
            IloExpr summation(env);
            for(int k=0;k <instance->ncustomers;++k){ 
                summation +=s[j][k][l];
            }
            summation -=instance->warehouse_capacity[j][l] * z[j];
            IloRange constr_limite_capacidade_deposito(env, -IloInfinity, summation, 0);
            stringstream name;
            name << "LimiteCapacidadeDeposito(" << j << ")";
            constr_limite_capacidade_deposito.setName(name.str().c_str());
            model.add(constr_limite_capacidade_deposito);
        }
    }
}

void ExactMPTSCFL::addConstraintLimiteFabricaAbertaSuperior(IloEnv &env,IloModel &model,IloIntVarArray &y,Instance *instance){
        IloExpr summation(env);
        for(int i= 0; i<instance->nfactories;++i){
            summation +=y[i];       
        }
        summation -= instance->betaFab;
        IloRange constr_limite_fabrica_aberta_superior(env, -IloInfinity, summation, 0);
        stringstream name;
        name << "LimiteFabricaAbertaSuperior";
        constr_limite_fabrica_aberta_superior.setName(name.str().c_str());
        model.add(constr_limite_fabrica_aberta_superior);
}
void ExactMPTSCFL::addConstraintLimiteDepositoAbertoSuperior(IloEnv &env,IloModel &model,IloIntVarArray &z,Instance *instance){
        IloExpr summation(env);
        for(int j= 0; j<instance->nwarehouses;++j){
            summation +=z[j];       
        }
        summation -= instance->betaDep;
        IloRange constr_limite_deposito_aberto_superior(env, -IloInfinity, summation, 0);
        stringstream name;
        name << "LimiteDepositoAbertoSuperior";
        constr_limite_deposito_aberto_superior.setName(name.str().c_str());
        model.add(constr_limite_deposito_aberto_superior);
}
void ExactMPTSCFL::addConstraintLimiteFabricaAbertaInferior(IloEnv &env,IloModel &model,IloIntVarArray &y,Instance *instance){
        IloExpr summation(env);
        for(int i= 0; i<instance->nfactories;++i){
            summation +=y[i];       
        }
        summation -= instance->piFab;
        IloRange constr_limite_fabrica_aberta_inferior(env, 0, summation, IloInfinity);
        stringstream name;
        name << "LimiteFabricaAbertaInferior";
        constr_limite_fabrica_aberta_inferior.setName(name.str().c_str());
        model.add(constr_limite_fabrica_aberta_inferior);
}
void ExactMPTSCFL::addConstraintLimiteDepositoAbertoInferior(IloEnv &env,IloModel &model,IloIntVarArray &z,Instance *instance){
        IloExpr summation(env);
        for(int j= 0; j<instance->nwarehouses;++j){
            summation +=z[j];       
        }
        summation -= instance->piDep;
        IloRange constr_limite_deposito_aberto_inferior(env, 0, summation, IloInfinity);
        stringstream name;
        name << "LimiteDepositoAbertoInferior";
        constr_limite_deposito_aberto_inferior.setName(name.str().c_str());
        model.add(constr_limite_deposito_aberto_inferior);
}

void ExactMPTSCFL::addObjective(IloEnv &env,IloModel &model,IloNumVar3Matrix &x,IloNumVar3Matrix &s,IloIntVarArray &z,IloIntVarArray &y){

    IloExpr summation(env);
    for(int i=0;i<instance->nfactories;++i){
        summation +=y[i]*instance->fixedcost_factory[i];
        for(int j=0;j<instance->nwarehouses;++j){
            for(int l=0;l<instance->ncommodities;++l){
                summation +=x[i][j][l]*instance->flowcost_fw[l][i][j];
            }
        }
    }
    for (int j=0;j<instance->nwarehouses;++j){
        summation+=z[j]*instance->fixedcost_warehouse[j];
        for(int k=0;k<instance->ncustomers;++k){
            for(int l=0;l<instance->ncommodities;++l){
                summation +=s[j][k][l]*instance->flowcost_wc[l][j][k];
            }
        }
    }    

    model.add(IloMinimize(env,summation));




}

void ExactMPTSCFL::setMIPstart(vector<int> &Solution, IloCplex &cplex) {  	
    
    
    int solvaly[instance->nfactories]; 	
    int solvalz[instance->nwarehouses];  	  	
    for (int i=0;i<instance->nfactories;++i){ 		 		
        solvaly[i] = Solution[i]; 	
    }
    
    for(int j=0;j<instance->nwarehouses;++j) 	{ 		
        solvalz[j] = Solution[instance->nfactories+j]; 	
    }
    
    IloNumVarArray startVar(env); 	
    IloNumArray startVal(env);  	
    for (int i=0;i< instance->nfactories;++i) 	{ 		
        startVar.add(y[i]); 		
        startVal.add(solvaly[i]); 	
    }  	
    for (int j=0;j<instance->nwarehouses;++j){ 		
        startVar.add(z[j]); 		
        startVal.add(solvalz[j]); 	
    }  	
    cplex.addMIPStart(startVar, startVal);
} 

void ExactMPTSCFL::addConstraintLocalBranching(IloEnv &env,IloModel &model,IloIntVarArray &y,IloIntVarArray &z,std::vector<int> &solution){
    double deltaFab = 0.30 * instance->nfactories;
    double deltaDep = 0.30 * instance->nwarehouses;
    IloExpr summationFab(env);
    IloExpr summationDep(env);

    for(int i=0;i<instance->nfactories;++i){
        if (solution[i] == 1){
            summationFab += (1 - y[i]);
        }else{
            summationFab += y[i];
        }
    }
    
     for(int j=0;j<instance->nwarehouses;++j){
        if (solution[instance->nfactories+j] == 1){
            summationDep += (1 -z[j]);
        }else{
            summationDep += z[j];
        }
    }
    //LB FAB
    summationFab -= deltaFab; 
    IloRange constr_localbranch_fab(env, -IloInfinity, summationFab, 0);
    stringstream lbfab;
    lbfab << "constr_localbranch_fab";
    constr_localbranch_fab.setName(lbfab.str().c_str());
    model.add(constr_localbranch_fab);    
    //LB DEP
    summationDep -= deltaDep; 
    IloRange constr_localbranch_dep(env, -IloInfinity, summationDep, 0);
    stringstream lbdep;
    lbdep << "constr_localbranch_dep";
    constr_localbranch_dep.setName(lbdep.str().c_str());
    model.add(constr_localbranch_dep);  
}
    

void ExactMPTSCFL::ConstroiModelo(){
    
    env = IloEnv();

    try
    {
    
    model = IloModel(env);
    
    // cout << "I = " << instance->nfactories << endl;
    // cout << "J = " << instance->nwarehouses << endl;
    // cout << "K = " << instance->ncustomers << endl;
    // cout << "L = " << instance->ncommodities << endl;

    IloNumVar3Matrix x(env,instance->nfactories);
    IloNumVar3Matrix s(env,instance->nwarehouses);
    y = IloIntVarArray(env,instance->nfactories,0,1);
    z = IloIntVarArray(env,instance->nwarehouses,0,1);
    
    for (int i = 0; i < instance->nfactories; ++i){
        x[i] = IloNumVarMatrix (env,instance->nwarehouses);
        stringstream vary;
        vary << "y(" << i << ")";
        y[i].setName(vary.str().c_str());
        model.add(y[i]);
        for(int j = 0; j < instance->nwarehouses; ++j){
            x[i][j] = IloNumVarArray (env,instance->ncommodities,0,IloInfinity,ILOFLOAT);
            for(int l=0; l < instance->ncommodities;l++){
                stringstream varx;
                varx << "x(" << i << ")(" << j << ")(" << l << ")";
                x[i][j][l].setName(varx.str().c_str());
                model.add(x[i][j][l]);
            } 
            
        }
    
    }
    
    for (int j = 0; j < instance->nwarehouses; ++j){
        s[j] = IloNumVarMatrix(env,instance->ncustomers);
        stringstream varz;
        varz << "z(" << j << ")";
        z[j].setName(varz.str().c_str());
        model.add(z[j]);
        for (int k=0; k < instance->ncustomers;++k){
            s[j][k] = IloNumVarArray(env,instance->ncommodities,0,IloInfinity,ILOFLOAT);
            for(int l=0; l < instance->ncommodities;l++){
                stringstream vars;
                vars << "s(" << j << ")(" << k << ")(" << l << ")";
                s[j][k][l].setName(vars.str().c_str());
                model.add(s[j][k][l]);
            }  
        }
    }
    
    
    addConstraintLimiteFabricaAbertaSuperior(env,model,y,instance);
    addConstraintLimiteDepositoAbertoSuperior(env,model,z,instance);
    addConstraintLimiteFabricaAbertaInferior(env,model,y,instance);
    addConstraintLimiteDepositoAbertoInferior(env,model,z,instance);
    addConstraintFluxoDepositoCliente(env,model,s,instance);
    addConstraintFluxoFabricaDeposito(env,model,x,s);
    addConstraintLimiteCapacidadeFabrica(env,model,y,x,instance);
    addConstraintLimiteCapacidadeDeposito(env,model,z,s,instance);
    addObjective(env,model,x,s,z,y);

    //IloCplex cplex(model);
    //cplex.exportModel("FlowModel.lp");
    //cplex.setParam();
    /* cplex.setParam(IloCplex::Param::Preprocessing::Linear, 0);
	cplex.setParam(IloCplex::Param::Preprocessing::Aggregator, 0);
	cplex.setParam(IloCplex::Param::Preprocessing::Presolve, 0); */
    //cplex.setParam(IloCplex::Param::Preprocessing::Symmetry, 0);

	/* cplex.setParam(	IloCplex::Param::Advance, 0); //AdvInd = 1 ou 2

	cplex.setParam(IloCplex::Param::Threads, 1);

	//cplex.setParam(IloCplex::Param::Preprocessing::Pre, IloFalse);
    

	cplex.setParam(IloCplex::Param::MIP::Strategy::HeuristicFreq, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::Cliques, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::Covers, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::Disjunctive, -1);
    cplex.setParam(IloCplex::Param::MIP::Cuts::Implied, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::FlowCovers, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::PathCut, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::Gomory, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::GUBCovers, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::MIRCut, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::ZeroHalfCut, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::MCFCut, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::LiftProj, -1); 
 */
    //cplex.solve();
 

   }
   catch (IloException& e) {
      cerr << "Concert exception caught " << e << endl;
   }
   catch (...) {
      cerr << "Unknown exception caught" << endl;
   }
    
    
    //cout << " Max =" << cplex.getObjValue() << endl ; 


    


}
tuple <std::vector<int>,double> ExactMPTSCFL::solveLB(vector<int> &Solution,int timebound,double upperbound){
    try{
        ofstream logFileAddress("outputcplex.log", ios::out | ios::app);
        //addConstraintLocalBranching(env,model,y,z,solution);
        IloCplex cplex(model);
        cplex.setOut(logFileAddress);
        
        cplex.setParam(IloCplex::Param::TimeLimit,timebound);
        cplex.setParam(IloCplex::Param::Emphasis::MIP,1);
        cplex.setParam(IloCplex::Param::Simplex::Limits::UpperObj,upperbound+1);
        cplex.setParam(IloCplex::Param::Threads, 1);
        setMIPstart(Solution,cplex);
        addConstraintLocalBranching(env,model,y,z,Solution);
        //cplex.exportModel("MPTSCFLP.lp");
        cplex.solve();
        vector <int> Solution(instance->nfactories+instance->nwarehouses);
        for(int i=0;i<instance->nfactories;++i){
            if(cplex.getValue(y[i]) > 0.5){
                Solution[i]=1;
            }else{
                Solution[i]=0;
            }
        }
        for(int j=0;j<instance->nwarehouses;++j){
            if(cplex.getValue(z[j]) > 0.5){
                Solution[instance->nfactories+j]=1;
            }else{
                Solution[instance->nfactories+j]=0;
            }
        }
        // double mipgap = cplex.getMIPRelativeGap();
        return make_pair(Solution,cplex.getObjValue());
        
        //logFileAddress.close();
    }catch (IloException& e) {
      cerr << "Concert exception caught " << e << endl;
    }
    catch (...) {
      cerr << "Unknown exception caught" << endl;
    }
}
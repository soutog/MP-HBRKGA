#pragma once
#include "instance.h"
#include "ilcplex/ilocplex.h"
#include <vector>


typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumVarMatrix> IloNumVar3Matrix;

class ExactMPTSCFL {
    public:
        Instance *instance;
        ExactMPTSCFL(Instance *instance);
        ~ExactMPTSCFL();
        void ConstroiModelo();
        std::tuple <std::vector<int>,double> solveLB(std::vector<int>& Solution,int timebound,double upperbound);
        void setMIPstart(vector<int> &Solution, IloCplex &cplex);
    private:
        IloEnv env;
        IloModel model;
        IloIntVarArray y;
        IloIntVarArray z;
        void addConstraintLocalBranching(IloEnv &env,IloModel &model,IloIntVarArray &y,IloIntVarArray &z,std::vector<int> &solution);
        void addConstraintFluxoDepositoCliente(IloEnv &env,IloModel &model,IloNumVar3Matrix &s,Instance *instance);
        void addObjective(IloEnv &env,IloModel &model,IloNumVar3Matrix &x,IloNumVar3Matrix &s,IloIntVarArray &z,IloIntVarArray &y);
        void addConstraintFluxoFabricaDeposito(IloEnv &env,IloModel &model,IloNumVar3Matrix &x,IloNumVar3Matrix &s);
        void addConstraintLimiteCapacidadeFabrica(IloEnv &env,IloModel &model,IloIntVarArray &y,IloNumVar3Matrix &x,Instance *instance);
        void addConstraintLimiteCapacidadeDeposito(IloEnv &env,IloModel &model,IloIntVarArray &z,IloNumVar3Matrix &s,Instance *instance);
        void addConstraintLimiteFabricaAbertaSuperior(IloEnv &env,IloModel &model,IloIntVarArray &y,Instance *instance);
        void addConstraintLimiteDepositoAbertoSuperior(IloEnv &env,IloModel &model,IloIntVarArray &z,Instance *instance);
        void addConstraintLimiteFabricaAbertaInferior(IloEnv &env,IloModel &model,IloIntVarArray &y,Instance *instance);
        void addConstraintLimiteDepositoAbertoInferior(IloEnv &env,IloModel &model,IloIntVarArray &z,Instance *instance);
};
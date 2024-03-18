#ifndef CHECK_FEAS_H_
#define CHECK_FEAS_H_

#include "instance.h"
#include "graphcost_ts.h"
#include <vector>
#include <queue>
#include <tuple>



class CheckFeas {
 private:
    Instance *p;
    vector<GraphCostTS *> g;
    
    double CalculaCustoAbertura(std::vector<int> &solution);
    bool CalculaCapacidade(std::vector<int> &solution);
    double CalculateFlow(std::vector<int> &solution);

 public:
    CheckFeas(Instance *_p,vector<GraphCostTS*> _g);

    ~CheckFeas();

    double check(std::vector<int> &solution);
};

#endif
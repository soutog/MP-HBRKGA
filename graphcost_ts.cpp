#include "graphcost_ts.h"


GraphCostTS::GraphCostTS(Instance * _p, int l)
{
  commodity = l;
  p = _p;
  x     = NULL;
  pi    = NULL;
  slack = NULL;
  dj    = NULL;

  env = CPXopenCPLEX (&status);

  if ( env == NULL ) {
    char  errmsg[CPXMESSAGEBUFSIZE];
    fprintf (stderr, "Could not open CPLEX environment.\n");
    CPXgeterrorstring (env, status, errmsg);
    fprintf (stderr, "%s", errmsg);
    exit(-17);
  }

  status = CPXsetintparam (env, CPXPARAM_ScreenOutput, CPX_OFF);
  if ( status ) {
    fprintf (stderr, "Failure to turn on screen indicator, error %d.\n", status);
    exit(-17);
  }

  net = CPXNETcreateprob (env, &status, "minCostmaxFlow");
  if ( net == NULL ) {
    fprintf (stderr, "Failed to create network object.\n");
    exit(-17);
  }

  NNODES = p->nfactories + 2*p->nwarehouses + p->ncustomers + 1;
  NARCS =  p->nfactories * p->nwarehouses + p->nwarehouses * p->ncustomers + p->nfactories + p->nwarehouses;

  double totalSuply = 0;

  supply = new double [NNODES];
  from_node = new int [NARCS];
  to_node = new int [NARCS];
  obj = new double [NARCS];
  ub = new double [NARCS];
  lb = new double [NARCS];

  int aux=0;
  for (int i = 0; i <  p->nfactories; ++i)
  {
    supply[aux] = 0;
    aux++;
  }

 
  for (int j = 0; j < p->nwarehouses; ++j)
  {
    supply[aux] = 0;
    aux++;
  }

  for (int j = 0; j < p->nwarehouses; ++j)
  {
    supply[aux] = 0;
    aux++;
  }

  for (int k = 0; k < p->ncustomers; ++k)
  {
    supply[aux] = -p->t_customer_demand[commodity][k];
    totalSuply += p->t_customer_demand[commodity][k];
    aux++;
  }

  supply[aux] = totalSuply;
  aux++;

  if(NNODES != aux){
    cout<<"Deu ruim - NNODE!!!"<<endl;
    exit(-17);
  } 

  artificial_ware_arc_zero_index = p->nfactories * p->nwarehouses + p->nwarehouses * p->ncustomers;
  artificial_fac_arc_zero_index = p->nfactories * p->nwarehouses + p->nwarehouses * p->ncustomers + p->nwarehouses;

  aux = 0;
  for (int i = 0; i < p->nfactories; ++i)
  {
    
    from_node[artificial_fac_arc_zero_index+i] = NNODES-1;
    to_node[artificial_fac_arc_zero_index+i] = i;
    obj[artificial_fac_arc_zero_index+i] = 0;    
    lb[artificial_fac_arc_zero_index+i] = 0;
    ub[artificial_fac_arc_zero_index+i] = p->t_factory_capacity[commodity][i];
    
    for (int j = 0; j < p->nwarehouses; ++j)
    {
      from_node[aux] = i;
      to_node[aux] = p->nfactories+j;
      obj[aux] = p->flowcost_fw[commodity][i][j];
      lb[aux] = 0;
      ub[aux] = min(p->t_factory_capacity[commodity][i],p->t_warehouse_capacity[commodity][j]);
      aux++;
    }
    
  }

  for (int j = 0; j < p->nwarehouses; ++j)
  {
    
    from_node[artificial_ware_arc_zero_index+j] = p->nfactories+j;
    to_node[artificial_ware_arc_zero_index+j] = (p->nfactories+p->nwarehouses)+j;
    obj[artificial_ware_arc_zero_index+j] = 0;    
    lb[artificial_ware_arc_zero_index+j] = 0;
    ub[artificial_ware_arc_zero_index+j] = p->t_warehouse_capacity[commodity][j];
    
    for (int k = 0; k < p->ncustomers; ++k)
    {
      from_node[aux] = (p->nfactories+p->nwarehouses)+j;
      to_node[aux] = (p->nfactories+2*p->nwarehouses)+k;
      obj[aux] = p->flowcost_wc[commodity][j][k];
      lb[aux] = 0;
      ub[aux] = min(p->t_warehouse_capacity[commodity][j],p->t_customer_demand[commodity][k]);
      aux++;
    }
    
  }

   if(NARCS != aux+p->nwarehouses+p->nfactories){
    cout<<"Deu ruim - NARCS!!!"<<endl;
    cout<<NARCS<<" "<<aux+p->nwarehouses+p->nfactories<<endl;
    exit(-17);
  } 


  buildNetwork(env, net);

  /*   
  cout<<"Supply"<<endl;
  for(int n = 0; n<NNODES; ++n)
  {
    cout<<supply[n]<<" ";
  }
  cout<<endl;
  cout<<"From-Node"<<endl;

  for(int a = 0; a<NARCS; ++a)
  {
    cout<<from_node[a]<<" ";
  }
  cout<<endl;
  cout<<"To-Node"<<endl;

  for(int a = 0; a<NARCS; ++a)
  {
    cout<<to_node[a]<<" ";
  }
  cout<<endl;
  cout<<"Obj"<<endl;
  for(int a = 0; a<NARCS; ++a)
  {
    cout<<obj[a]<<" ";
  }
  cout<<endl;
  cout<<"LB"<<endl;
  for(int a = 0; a<NARCS; ++a)
  {
    cout<<lb[a]<<" ";
  }
  cout<<endl;
  cout<<"UB"<<endl;
  for(int a = 0; a<NARCS; ++a)
  {
    cout<<ub[a]<<" ";
  }
  cout<<endl;
  */
}

GraphCostTS::~GraphCostTS()
{
  //cout<<"Deleting Graphcost ... ";
  /* Free memory for solution data */

  free_and_null ((char **) &x);
  free_and_null ((char **) &dj);
  free_and_null ((char **) &pi);
  free_and_null ((char **) &slack);
  free_and_null ((char **) &supply);
  free_and_null ((char **) &from_node);
  free_and_null ((char **) &to_node);
  free_and_null ((char **) &obj);
  free_and_null ((char **) &ub);
  free_and_null ((char **) &lb);

  /* Free up the problem as allocated by CPXNETcreateprob, if necessary */

  if ( net != NULL ) {
    status = CPXNETfreeprob (env, &net);
    if ( status ) {
      fprintf (stderr, "CPXNETfreeprob failed, error code %d.\n", status);
    }
  }

  /* Free up the CPLEX environment, if necessary */

  if ( env != NULL ) {
    status = CPXcloseCPLEX (&env);

    if ( status ) {
      char  errmsg[CPXMESSAGEBUFSIZE];
      fprintf (stderr, "Could not close CPLEX environment.\n");
      CPXgeterrorstring (env, status, errmsg);
      fprintf (stderr, "%s", errmsg);
    }
  }
  //cout<<"done"<<endl;
}

int GraphCostTS::buildNetwork (CPXENVptr env, CPXNETptr net)
{
  int status = 0;

  if ( CPXNETgetnumnodes (env, net) > 0 ) {
    status = CPXNETdelnodes (env, net, 0,CPXNETgetnumnodes (env, net)-1);
    if ( status ) return (status); 
  }

  status = CPXNETchgobjsen (env, net, CPX_MIN);
  if ( status ) return (status);

  status = CPXNETaddnodes (env, net, NNODES, supply, NULL);
  if ( status ) return (status);


  return (status);

}

void GraphCostTS::update_ub(vector<int> Solution)
{
  vector<int> openfactories(Solution.begin(),Solution.begin()+p->nfactories);
  vector<int> openwarehouses(Solution.begin()+p->nfactories,Solution.end());
  for (int i = 0; i < p->nfactories; ++i)
  {
    ub[artificial_fac_arc_zero_index+i] = p->t_factory_capacity[commodity][i]*openfactories[i];
  }

  for (int j = 0; j < p->nwarehouses; ++j)
  {
    ub[artificial_ware_arc_zero_index+j] = p->t_warehouse_capacity[commodity][j]*openwarehouses[j];
  }
}

void GraphCostTS::solve(vector<int> Solution)
{
  update_ub(Solution);

  if ( CPXNETgetnumarcs (env, net) > 0 ) {
    status = CPXNETdelarcs (env, net, 0, CPXNETgetnumarcs (env, net)-1);
    if ( status )
    {
      fprintf (stderr, "Failed to delete arcs.\n");
      exit(-17);
    }
  }
  
  status = CPXNETaddarcs (env, net, NARCS, from_node, to_node, lb, ub, obj, NULL);
  if ( status ) {
    fprintf (stderr, "Failed to add Arcs.\n");
    exit(-17);
  }
  
  status = CPXNETprimopt (env, net);
  if ( status ) {
    fprintf (stderr, "Failed to optimize network.\n");
    exit(-17);
  }


  int narcs  = CPXNETgetnumarcs  (env, net);
  int nnodes = CPXNETgetnumnodes (env, net);
  //deleta valores antes de usa-los para evitar leak
  free_and_null ((char **) &x);
  free_and_null ((char **) &dj);
  free_and_null ((char **) &pi);
  free_and_null ((char **) &slack);
  //aloca novos endereços de memoria 
  x     = (double *) malloc (narcs  * sizeof (double));
  dj    = (double *) malloc (narcs  * sizeof (double));
  pi    = (double *) malloc (nnodes * sizeof (double));
  slack = (double *) malloc (nnodes * sizeof (double));

  if ( x     == NULL ||
      dj    == NULL ||
      pi    == NULL ||
      slack == NULL   ) {
    fprintf (stderr, "Failed to allocate arrays.\n");
    exit(-17);
  }

  status = CPXNETsolution (env, net, &solstat, &objval, x, pi, slack, dj);
  if ( status ) {
    fprintf (stderr, "Failed to obtain solution.\n");
    exit(-17);
  }
  
 /* 
  printf ("\nSolution status = %d\n", solstat);
  printf ("Solution value  = %f\n\n", objval);
 

  for (int i = 0; i < nnodes; i++) {
    printf ("Node %2d:  Slack = %10f  Pi = %10f\n", i, slack[i], pi[i]);
  }

  for (int j = 0; j < narcs; j++) {
    printf ("Arc  %2d:  Value = %10f  Reduced cost = %10f\n", j, x[j], dj[j]);
  }*/
  

  // status = CPXNETwriteprob (env, net, "minCostmaxFlow.net", NULL);
  // if ( status ) {
  //   fprintf (stderr, "Failed to write network to disk.\n");
  //   exit(-17);
  // }
  
}

void GraphCostTS::free_and_null (char **ptr)
{
  if ( *ptr != NULL ){
    delete (*ptr);
    *ptr = NULL;
  }
} 

pair<double,vector<int>> GraphCostTS:: getsol()
{
  
  vector <int> openfactories(p->nfactories,0);
  vector <int> openwarehouses(p->nwarehouses,0);
  for (int i = 0; i < p->nfactories; ++i)
  {
    if(x[artificial_fac_arc_zero_index+i]>0)
    {
      openfactories[i] = 1;
    }
  }

  for (int j = 0; j < p->nwarehouses; ++j)
  {
    if(x[artificial_ware_arc_zero_index+j]>0)
    {
      openwarehouses[j] = 1;
    }
  }

  vector <int> Solution(openfactories.begin(),openfactories.end());
  Solution.insert(Solution.end(),openwarehouses.begin(),openwarehouses.end());

  pair<double,vector<int>> sol = make_pair(objval,Solution);

  return sol;
}
        

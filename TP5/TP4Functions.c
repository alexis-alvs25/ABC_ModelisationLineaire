#include "TP4Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include<stdio.h>
#include<ilcplex/cplex.h>
#define MAX_COST 10

// On peut lire une instance de 1 dimension car le fscanf ne lira que les premieere valeur, on conseillera de faire un calloc lors de l'allocation afin d'éviter que des valeurs parasite vienne tou casser
int read_TP2_instance(FILE*fin,dataSet* dsptr)
{
	int rval = 0;

	//capacites b et g
	int b;
	int g;
	//Nombre d'objets
	int n;
	rval = fscanf(fin,"%d,%d,%d\n",&n,&b,&g);
	dsptr->b = b;
	dsptr->g = g;

	dsptr->n = n;
	dsptr->c = (double*)malloc(sizeof(double)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);
	dsptr->f = (int*)malloc(sizeof(int)*n);


	int i;
	for( i = 0 ; i < n ; i++)
		rval = fscanf(fin,"%lf,%d,%d\n",&(dsptr->c[i]),&(dsptr->a[i]),&(dsptr->f[i]));

	fprintf(stderr,"\nInstance file read, we have capacites [b,g] = [%d,%d] and there is %d items of values/weights:\n",
			b,g,n);
	fprintf(stderr,"i\tci\tai\tfi\n");
	fprintf(stderr,"--------------------------\n");


	for( i = 0 ; i < n ; i++)
		fprintf(stderr,"%d\t%lf\t%d\t%d\n",i,dsptr->c[i],dsptr->a[i],dsptr->f[i]);
	fprintf(stderr,"\n");

	return rval;
}

void create_instance(char * filePath,int seed, int b, int g, int n)
{
	FILE * f = fopen(filePath, "w");
	srand(seed);
	if(b == -1)
		b = rand()%1000;
	if(g == -1)
		g = rand()%1000;
	if(n == -1)
		n = rand()%100;
	fprintf(f,"%d,%d;%d\n",n,b,g);

	for(int i = 0 ; i < n ; i++)
	{
		int a = rand()%b+1;
		int d = rand()%g+1;
		int c = rand()%MAX_COST;
		fprintf(f,"%d,%d,%d\n",c,a,d);
	}
	fclose(f);

}

int read_TP1_instance(FILE*fin,dataSet* dsptr)
{
	int rval = 0;

	//capacite b
	int b;
	//Nombre d'objets
	int n;
	rval = fscanf(fin,"%d,%d\n",&n,&b);
	dsptr->b = b;
	dsptr->n = n;
	dsptr->c = (double*)malloc(sizeof(double)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);


	int i;
	for( i = 0 ; i < n ; i++)
		rval = fscanf(fin,"%lf,%d\n",&(dsptr->c[i]),&(dsptr->a[i]));

	//fprintf(stderr,"\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",b,n);
	/*for( i = 0 ; i < n ; i++)
		fprintf(stderr,"%d,%d\n",dsptr->c[i],dsptr->a[i]);
	//fprintf(stderr,"\n");*/


	return rval;
}

dataSet * create_instance2d(int b, int g,int n)
{
	if(b == -1)
		b = (rand()%100)+1;
	if(n == -1)
		n = (rand()%100)+1;
	if(g == -1)
		n = (rand()%100)+1;
	int * a = malloc(n * sizeof(int));
	double * c = malloc(n * sizeof(int));
	int * f = malloc(n * sizeof(int));

	for(int i = 0 ; i < n ; i++)
	{
		a[i] = rand()%b+1;
		c[i] = ((double)rand()/RAND_MAX)  *MAX_COST;
		f[i] = rand()%MAX_COST;

	}
	dataSet * dpstr = malloc(sizeof(dataSet));
	dpstr->b = b;
	dpstr->n = n;
	dpstr->a = a;
	dpstr->c = c;
	dpstr->f = f;
	return dpstr;

}

dataSet * create_instance1d(int b, int n)
{
	if(b == -1)
		b = (rand()%100)+1;
	if(n == -1)
		n = (rand()%100)+1;
	int * a = malloc(n * sizeof(int));
	double * c = malloc(n * sizeof(double));

	for(int i = 0 ; i < n ; i++)
	{
		a[i] = rand()%b+1;
		c[i] = ((double)rand()/RAND_MAX)  *MAX_COST;
	}
	dataSet * dpstr = malloc(sizeof(dataSet));
	dpstr->f = NULL;
	dpstr->b = b;
	dpstr->n = n;
	dpstr->a = a;
	dpstr->c = c;
	return dpstr;

}

int solve_2DKP(dataSet* dsptr)
{
	int rval = 0;

	IP_problem* ip_prob_ptr = &(dsptr->master);
	ip_prob_ptr->env = NULL;
	ip_prob_ptr->lp = NULL;
	ip_prob_ptr->env = CPXopenCPLEX (&rval);
	if(rval) fprintf(stderr,"ERROR WHILE CALLING CPXopenCPLEX\n");
	if ( ip_prob_ptr->env == NULL ) 
	{
		char  errmsg[1024];
		fprintf (stderr, "Could not open CPLEX environment.\n");
		CPXgeterrorstring (ip_prob_ptr->env, rval, errmsg);
		fprintf (stderr, "%s", errmsg);
		exit(0);	
	}

	//We create the MIP problem
	ip_prob_ptr->lp = CPXcreateprob (ip_prob_ptr->env, &rval, "TP4");
	if(rval) fprintf(stderr,"ERROR WHILE CALLING CPXcreateprob\n");

	rval = CPXsetintparam (ip_prob_ptr->env, CPX_PARAM_DATACHECK, CPX_ON); 
	rval = CPXsetintparam (ip_prob_ptr->env, CPX_PARAM_SCRIND, CPX_ON);

	int n = dsptr->n;
	int nv = n;
	int* a = dsptr->a;
	int* f = dsptr->f;
	int* c = dsptr->c;
	int b = dsptr->b;
	int g = dsptr->g;

	//We fill our arrays
	//Memory
	ip_prob_ptr->nv = nv;
        ip_prob_ptr->x = (double*)malloc(sizeof(double)*nv);
        ip_prob_ptr->cost = (double*)malloc(sizeof(double)*nv);
        ip_prob_ptr->c_type = (char*)malloc(sizeof(char)*nv);
        ip_prob_ptr->up_bound = (double*)malloc(sizeof(double)*nv);
        ip_prob_ptr->low_bound = (double*)malloc(sizeof(double)*nv);
	ip_prob_ptr->var_name = (char**)malloc(sizeof(char*)*nv);

	int j,id = 0;
	//Structures keeping the index of each variable
	int*id_x_j = (int*)malloc(sizeof(int)*n);

	//variables xi definition
	for( j = 0 ; j < n ; j++)
	{
		//We keep the id
		id_x_j[j] = id;

		//We generate the variable attributes
		ip_prob_ptr->x[id] = 0;
		ip_prob_ptr->cost[id] = c[j];
		ip_prob_ptr->c_type[id] = 'B';
		ip_prob_ptr->up_bound[id] = 1;
		ip_prob_ptr->low_bound[id] = 0;
		ip_prob_ptr->var_name[id] = (char*)malloc(sizeof(char)*1024);
	        snprintf(       ip_prob_ptr->var_name[id],
        	                1024,
                	        "x_j%d",
                        	j);
		id++;
	}


	rval = CPXnewcols( ip_prob_ptr->env, ip_prob_ptr->lp, 
			nv, 
			ip_prob_ptr->cost, 
			ip_prob_ptr->low_bound,
			ip_prob_ptr->up_bound,
			ip_prob_ptr->c_type,
			ip_prob_ptr->var_name);
	if(rval)
		fprintf(stderr,"CPXnewcols returned errcode %d\n",rval);



	//Constraints part
        ip_prob_ptr->rhs = (double*)malloc(sizeof(double));
        ip_prob_ptr->sense = (char*)malloc(sizeof(char));
        ip_prob_ptr->rmatbeg = (int*)malloc(sizeof(int));
	ip_prob_ptr->nz = n;


        ip_prob_ptr->rmatind = (int*)malloc(sizeof(int)*nv);
        ip_prob_ptr->rmatval = (double*)malloc(sizeof(double)*nv);
	ip_prob_ptr->const_name = (char**)malloc(sizeof(char*));
	ip_prob_ptr->const_name[0] = (char*)malloc(sizeof(char)*1024);

	//We fill what we can 
	ip_prob_ptr->rmatbeg[0] = 0;

	//We generate and add each constraint to the model

	//capacity constraint #1
	ip_prob_ptr->rhs[0] = b;
	ip_prob_ptr->sense[0] = 'L';
	//Constraint name
	snprintf(       ip_prob_ptr->const_name[0],
			1024,
			"capacity_1"
			);
	id=0;
	//variables x_j coefficients
	for( j = 0 ; j < n ; j++)
	{
		ip_prob_ptr->rmatind[id] = id_x_j[j];
		ip_prob_ptr->rmatval[id] =  a[j];
		id++;
	}
	rval = CPXaddrows( ip_prob_ptr->env, ip_prob_ptr->lp, 
			0,//No new column
			1,//One new row
			n,//Number of nonzero coefficients
			ip_prob_ptr->rhs, 
			ip_prob_ptr->sense, 
			ip_prob_ptr->rmatbeg, 
			ip_prob_ptr->rmatind, 
			ip_prob_ptr->rmatval,
			NULL,//No new column
			ip_prob_ptr->const_name );
	if(rval)
		fprintf(stderr,"CPXaddrows returned errcode %d\n",rval);

	//capacity constraint 2#
	ip_prob_ptr->rhs[0] = g;
	ip_prob_ptr->sense[0] = 'L';
	//Constraint name
	snprintf(       ip_prob_ptr->const_name[0],
			1024,
			"capacity_2"
			);
	id=0;
	//variables x_j coefficients
	for( j = 0 ; j < n ; j++)
	{
		ip_prob_ptr->rmatind[id] = id_x_j[j];
		ip_prob_ptr->rmatval[id] =  f[j];
		id++;
	}
	rval = CPXaddrows( ip_prob_ptr->env, ip_prob_ptr->lp, 
			0,//No new column
			1,//One new row
			n,//Number of nonzero coefficients
			ip_prob_ptr->rhs, 
			ip_prob_ptr->sense, 
			ip_prob_ptr->rmatbeg, 
			ip_prob_ptr->rmatind, 
			ip_prob_ptr->rmatval,
			NULL,//No new column
			ip_prob_ptr->const_name );
	if(rval)
		fprintf(stderr,"CPXaddrows returned errcode %d\n",rval);

	//We switch to maximization
	rval = CPXchgobjsen( ip_prob_ptr->env, ip_prob_ptr->lp, CPX_MAX );


	//We write the problem for debugging purposes, can be commented afterwards
	rval = CPXwriteprob (ip_prob_ptr->env, ip_prob_ptr->lp, "multiDKnapsack.lp", NULL);
	if(rval)
		fprintf(stderr,"CPXwriteprob returned errcode %d\n",rval);

	//We solve the model
	rval = CPXmipopt (ip_prob_ptr->env, ip_prob_ptr->lp);
	if(rval)
		fprintf(stderr,"CPXmipopt returned errcode %d\n",rval);

	rval = CPXsolwrite( ip_prob_ptr->env, ip_prob_ptr->lp, "multiDKnapsack.sol" );
	if(rval)
		fprintf(stderr,"CPXsolwrite returned errcode %d\n",rval);

	//We get the objective value
	rval = CPXgetobjval( ip_prob_ptr->env, ip_prob_ptr->lp, &(ip_prob_ptr->objval) );
	if(rval)
		fprintf(stderr,"CPXgetobjval returned errcode %d\n",rval);

	//We get the best solution found 
	rval = CPXgetobjval( ip_prob_ptr->env, ip_prob_ptr->lp, &(ip_prob_ptr->objval) );
	rval = CPXgetx( ip_prob_ptr->env, ip_prob_ptr->lp, ip_prob_ptr->x, 0, nv-1 );
	if(rval)
		fprintf(stderr,"CPXgetx returned errcode %d\n",rval);


	/**************** FILL HERE ***************/



	return rval;
}

int solve_1DKP(dataSet * dsptr)
{
	int rval = 0;

	IP_problem* ip_prob_ptr = &(dsptr->master);
	ip_prob_ptr->env = NULL;
	ip_prob_ptr->lp = NULL;
	ip_prob_ptr->env = CPXopenCPLEX (&rval);
	if(rval) fprintf(stderr,"ERROR WHILE CALLING CPXopenCPLEX\n");
	if ( ip_prob_ptr->env == NULL ) 
	{
		char  errmsg[1024];
		fprintf (stderr, "Could not open CPLEX environment.\n");
		CPXgeterrorstring (ip_prob_ptr->env, rval, errmsg);
		fprintf (stderr, "%s", errmsg);
		exit(0);	
	}

	//We create the MIP problem
	ip_prob_ptr->lp = CPXcreateprob (ip_prob_ptr->env, &rval, "TP4");
	if(rval) fprintf(stderr,"ERROR WHILE CALLING CPXcreateprob\n");

	rval = CPXsetintparam (ip_prob_ptr->env, CPX_PARAM_DATACHECK, CPX_ON); 
	rval = CPXsetintparam (ip_prob_ptr->env, CPX_PARAM_SCRIND, CPX_ON);

	int n = dsptr->n;
	int nv = n;
	int* a = dsptr->a;
	int* c = dsptr->c;
	int b = dsptr->b;

	//We fill our arrays
	//Memory
	ip_prob_ptr->nv = nv;
        ip_prob_ptr->x = (double*)malloc(sizeof(double)*nv);
        ip_prob_ptr->cost = (double*)malloc(sizeof(double)*nv);
        ip_prob_ptr->c_type = (char*)malloc(sizeof(char)*nv);
        ip_prob_ptr->up_bound = (double*)malloc(sizeof(double)*nv);
        ip_prob_ptr->low_bound = (double*)malloc(sizeof(double)*nv);
	ip_prob_ptr->var_name = (char**)malloc(sizeof(char*)*nv);

	int j,id = 0;
	//Structures keeping the index of each variable
	int*id_x_j = (int*)malloc(sizeof(int)*n);

	//variables xi definition
	for( j = 0 ; j < n ; j++)
	{
		//We keep the id
		id_x_j[j] = id;

		//We generate the variable attributes
		ip_prob_ptr->x[id] = 0;
		ip_prob_ptr->cost[id] = c[j];
		ip_prob_ptr->c_type[id] = 'B';
		ip_prob_ptr->up_bound[id] = 1;
		ip_prob_ptr->low_bound[id] = 0;
		ip_prob_ptr->var_name[id] = (char*)malloc(sizeof(char)*1024);
	        snprintf(       ip_prob_ptr->var_name[id],
        	                1024,
                	        "x_j%d",
                        	j);
		id++;
	}


	rval = CPXnewcols( ip_prob_ptr->env, ip_prob_ptr->lp, 
			nv, 
			ip_prob_ptr->cost, 
			ip_prob_ptr->low_bound,
			ip_prob_ptr->up_bound,
			ip_prob_ptr->c_type,
			ip_prob_ptr->var_name);
	if(rval)
		fprintf(stderr,"CPXnewcols returned errcode %d\n",rval);



	//Constraints part
        ip_prob_ptr->rhs = (double*)malloc(sizeof(double));
        ip_prob_ptr->sense = (char*)malloc(sizeof(char));
        ip_prob_ptr->rmatbeg = (int*)malloc(sizeof(int));
	ip_prob_ptr->nz = n;


        ip_prob_ptr->rmatind = (int*)malloc(sizeof(int)*nv);
        ip_prob_ptr->rmatval = (double*)malloc(sizeof(double)*nv);
	ip_prob_ptr->const_name = (char**)malloc(sizeof(char*));
	ip_prob_ptr->const_name[0] = (char*)malloc(sizeof(char)*1024);

	//We fill what we can 
	ip_prob_ptr->rmatbeg[0] = 0;

	//We generate and add each constraint to the model

	//capacity constraint #1
	ip_prob_ptr->rhs[0] = b;
	ip_prob_ptr->sense[0] = 'L';
	//Constraint name
	snprintf(       ip_prob_ptr->const_name[0],
			1024,
			"capacity_1"
			);
	id=0;
	//variables x_j coefficients
	for( j = 0 ; j < n ; j++)
	{
		ip_prob_ptr->rmatind[id] = id_x_j[j];
		ip_prob_ptr->rmatval[id] =  a[j];
		id++;
	}
	rval = CPXaddrows( ip_prob_ptr->env, ip_prob_ptr->lp, 
			0,//No new column
			1,//One new row
			n,//Number of nonzero coefficients
			ip_prob_ptr->rhs, 
			ip_prob_ptr->sense, 
			ip_prob_ptr->rmatbeg, 
			ip_prob_ptr->rmatind, 
			ip_prob_ptr->rmatval,
			NULL,//No new column
			ip_prob_ptr->const_name );
	if(rval)
		fprintf(stderr,"CPXaddrows returned errcode %d\n",rval);

	//We switch to maximization
	rval = CPXchgobjsen( ip_prob_ptr->env, ip_prob_ptr->lp, CPX_MAX );


	//We write the problem for debugging purposes, can be commented afterwards
	rval = CPXwriteprob (ip_prob_ptr->env, ip_prob_ptr->lp, "multiDKnapsack.lp", NULL);
	if(rval)
		fprintf(stderr,"CPXwriteprob returned errcode %d\n",rval);

	//We solve the model
	rval = CPXmipopt (ip_prob_ptr->env, ip_prob_ptr->lp);
	if(rval)
		fprintf(stderr,"CPXmipopt returned errcode %d\n",rval);

	rval = CPXsolwrite( ip_prob_ptr->env, ip_prob_ptr->lp, "multiDKnapsack.sol" );
	if(rval)
		fprintf(stderr,"CPXsolwrite returned errcode %d\n",rval);

	//We get the objective value
	rval = CPXgetobjval( ip_prob_ptr->env, ip_prob_ptr->lp, &(ip_prob_ptr->objval) );
	if(rval)
		fprintf(stderr,"CPXgetobjval returned errcode %d\n",rval);

	//We get the best solution found 
	rval = CPXgetobjval( ip_prob_ptr->env, ip_prob_ptr->lp, &(ip_prob_ptr->objval) );
	rval = CPXgetx( ip_prob_ptr->env, ip_prob_ptr->lp, ip_prob_ptr->x, 0, nv-1 );
	if(rval)
		fprintf(stderr,"CPXgetx returned errcode %d\n",rval);


	/**************** FILL HERE ***************/



	return rval;
}

void free_dataSet(dataSet * set)
{
	free(set->a);
	free(set->c);
	if(set->f)
		free(set->f);
	free(set);
}


void benchmark(int n, int b,int g, int iteration,int seed)
{
	
	FILE * f_lr_1D = fopen("./benchmark/1d.csv","w");
	FILE * f_lr_2D = fopen("./benchmark/2d.csv","w"); 
	int i = 0;
	dataSet * data;
	srand(seed);
	double * time_1D = calloc(100 , sizeof(double)); //n fixé
	double * time_2D = calloc(100 , sizeof(double)); //b fixé
	int * number_1D = calloc(100 , sizeof(int));
	int * number_2D = calloc(100 , sizeof(int));

	for(i = 1 ; i <= iteration ; i++)
	{
		data = create_instance1d(b,-1);
		clock_t begin = clock();
		solve_1DKP(data);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		time_1D[data->n] += time_spent;
		number_1D[data->n] += 1;
		//fprintf(f_greedy_b,"%d,%d,%lf\n",data->n,data->b,time_spent);
		free_dataSet(data);
	}

	for(int j = 0 ; j < 100 ; j++)
	{
		if(number_1D[j] != 0)
			fprintf(f_lr_1D,"%d,%lf\n",j+1,time_1D[j]/(double)number_1D[j]);
		time_1D[j] = 0;
		number_1D[j] = 0;
	}

	for(i = 1 ; i <= iteration ; i++)
	{
		data = create_instance2d(b,g,-1);
		clock_t begin = clock();
		solve_2DKP(data);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		//printf("%d,%d,%d\n",data->b,data->n,i);
		//fprintf(f_greedy_n,"%d,%d,%lf\n",data->n,data->b,time_spent);
		time_2D[data->n] += time_spent;
		number_2D[data->n] +=1;
		free_dataSet(data);
	}

	for(int j = 0 ; j < 100 ; j++)
	{
		if(number_2D[j] != 0)
			fprintf(f_lr_2D,"%d,%lf\n",j+1,time_2D[j]/(double)number_2D[j]);
		time_2D[j] = 0;
		number_2D[j] = 0;
	}

	free(number_1D);
	free(number_2D);
	free(time_1D);
	free(time_2D);
	fclose(f_lr_1D);
	fclose(f_lr_2D);

}


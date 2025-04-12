#include "TP5Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <float.h>


#define MAX_COST 10
#define MAX(a,b) ((a) > (b) ? (a) : (b))

void print_final_solution(int_solution sol)
{
	printf("Value : %lf \nWith variable :\n",sol->value);
	for(int i = 0 ; i < sol->size - 1; i++)
	{
		printf("x_%d = %d,",i,sol->xbar[i]);
	}
	printf("x_%d = %d\n",sol->size - 1,sol->xbar[sol->size - 1]);
}

int read_TP5_instance(FILE*fin,dataSet* dsptr)
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

void free_solution(solution sol)
{
	free(sol->xbar);
	free(sol);	
}

solution create_solution(int val, dataSet * dsptr)
{
	solution sol = malloc(sizeof(struct solution));
	sol->value = val;
	sol->xbar =  (&(dsptr->master))->x;
	return sol;
}


int isfeasible(solution sol, dataSet * dpstr)
{
	int sum = 0;
	for(int i = 0 ; i < dpstr->n ; i++)
	{
		sum += sol->xbar[i] *  dpstr->f[i];
		if(dpstr->g - sum < 0)
		{
			return 0;
		}
	}
	return 1;
}

double zxk(solution sol, dataSet * dpstr)
{
	double z = 0;
	for(int i = 0 ; i < dpstr->n ; i++)
	{
		z += sol->xbar[i] * dpstr->c[i];
	}
	return z;
}

int_solution knapsack2d(dataSet* dpstr,double * step,int size,double epsilon)
{
	fprintf(stderr,"debug1\n");
	int * xbar = malloc(dpstr->n * sizeof(int));
	double zbar = DBL_MAX;
	double zx = 0;
	int k = 0;
	double lambda = 0;
	dataSet * tmpdata = malloc(sizeof(dataSet));
	double * tmpcost = malloc(dpstr->n * sizeof(double));
	int ** xbark = malloc(size * sizeof(int *));
	double * zbark = malloc(size * sizeof(double));

	while(step[k] >epsilon && zbar -zx > epsilon)
	{
		if(k >= size)
		{
			exit(-1);
		}
		tmpdata->n = dpstr->n;
		tmpdata->b = dpstr->b;
		tmpdata->a = dpstr->a;
		fprintf(stderr,"%d, %d\n",dpstr->n,dpstr->b);
		for(int i = 0 ; i < dpstr->n ;i++)
		{
			tmpcost[i] = (double) dpstr->c[i] - lambda * (double)dpstr->f[i]; 
			fprintf(stderr,"%d, %lf, %lf\n",dpstr->a[i],tmpcost[i],dpstr->c[i]);
		}
		tmpdata->c = tmpcost;
		int val = solve_1DKP(tmpdata);
		solution sol = create_solution(val,tmpdata);
		xbark[k] = malloc(dpstr->n * sizeof(double));
		for(int i = 0 ; i < dpstr->n ; i++)
		{
			xbark[k][i] = sol->xbar[i];
		}
		zbark[k] = sol->value;
		if(isfeasible(sol,dpstr) && zxk(sol,dpstr) > zx)
		{
			xbar = xbark[k];
		}
		if(zbark[k] < zbar)
		{
			zbar = zbark[k];
		}
		
		double gamma = (double)dpstr->g;
		for(int i = 0 ; i < dpstr->n ; i++)
		{
			gamma -= xbark[k][i]*dpstr->f[i];
		}

        lambda = lambda - step[k] * gamma > 0 ? lambda - step[k] * gamma > 0 : 0;
		k++;
		//free_solution(sol);
	}
	//free(tmpdata);
	//free(tmpcost);
	
	int val = 0;
	for(int i = 0 ; i < dpstr->n ; i++)
	{
		val += dpstr->c[i] * xbark[k-1][i];
	}

	int_solution sol = malloc(sizeof(struct int_solution));
	sol->value = val;
	sol->xbar = xbar;
	sol->size = dpstr->n;
	fprintf(stderr,"debug2\n");
	return sol;
}



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

double calcul_gap(dataSet * dpstr, int ** xbark, int size, double rval)
{
	double max = 0;
	for(int i = 0 ; i < size ; i++)
	{
		double val = 0;
		for(int j = 0 ; j < dpstr->n ; j++)
		{
			val += (double)xbark[i][j] * dpstr->c[i];
		}
		max = val > max ? val : max;
	}
	return ((rval-max)/max)*100;
}

int_solution knapsack2d(dataSet* dpstr,double * step,int size,double epsilon)
{
	int * xbar = malloc(dpstr->n * sizeof(int));
	double zbar = DBL_MAX;
	double zx = 0;
	int k = 0;
	double lambda = 0;
	dataSet * tmpdata = malloc(sizeof(dataSet));
	double * tmpcost = malloc(dpstr->n * sizeof(double));
	int ** xbark = malloc(size * sizeof(int *));
	double * zbark = malloc(size * sizeof(double));
	double * zk = malloc(size * sizeof(double));

	while(step[k] >epsilon && zbar -zx > epsilon)
	{
		
		if(k >= size)
		{
			exit(-1);
		}
		tmpdata->n = dpstr->n;
		tmpdata->b = dpstr->b;
		tmpdata->a = dpstr->a;
		for(int i = 0 ; i < dpstr->n ;i++)
		{
			tmpcost[i] = (double) dpstr->c[i] - lambda * (double)dpstr->f[i]; 
		}
		tmpdata->c = tmpcost;
		int val = solve_1DKP(tmpdata);
		solution sol = create_solution(val,tmpdata);
		xbark[k] = malloc(dpstr->n * sizeof(double));
		for(int i = 0 ; i < dpstr->n ; i++)
		{
			xbark[k][i] = (int)sol->xbar[i];
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
		zk[k] = zx;
		
		double gamma = (double)dpstr->g;
		for(int i = 0 ; i < dpstr->n ; i++)
		{
			gamma -= xbark[k][i]*dpstr->f[i];
		}
        lambda = lambda - step[k] * gamma > 0 ? lambda - step[k] * gamma > 0 : 0;
		k++;
		free_solution(sol);
	}
	free(tmpdata);
	free(tmpcost);
	
	int val = 0;
	for(int i = 0 ; i < dpstr->n ; i++)
	{
		val += dpstr->c[i] * xbark[k-1][i];
	}

	int_solution sol = malloc(sizeof(struct int_solution));
	sol->value = val;
	sol->xbar = xbar;
	sol->size = dpstr->n;
	sol->gap = calcul_gap(dpstr,xbark,k,val);
	sol->zbark = zbark;
	sol->kmax = k;
	sol->zbar = zk;
	return sol;
}


void benchmark_TP5(int n, int b,int g, int iteration,int seed, int maxk,double * step)
{
	
	FILE * f_time = fopen("./benchmark/time.csv","w");
	FILE * f_gap = fopen("./benchmark/gap.csv","w"); 
	FILE * f_z = fopen("./benchmark/z.csv","w"); 
	FILE * f_zxk = fopen("./benchmark/zxk.csv","w"); 
	int i = 0;
	dataSet * data;
	srand(seed);
	double * time = calloc(100 , sizeof(double)); //n fixé
	double * gap = calloc(100 , sizeof(double));
	double * z = calloc(maxk , sizeof(double));
	double * zxk = calloc(maxk , sizeof(double));
	int * number_n = calloc(100 , sizeof(int));
	int * number_k = calloc(maxk , sizeof(int));

	for(i = 1 ; i <= iteration ; i++)
	{
		data = create_instance2d(b,g,-1);
		clock_t begin = clock();
		int_solution sol = knapsack2d(data,step,maxk,0.001);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		time[data->n] += time_spent;
		gap[data->n] += sol->gap;
		number_n[data->n] += 1;
		for(int j = 0 ; j < sol->kmax ; j++)
		{
		fprintf(stderr, "%lf %lf\n",sol->zbar[j],sol->zbark[j]);
			number_k[j]++;
			z[j]+= sol->zbar[j];
			zxk[j] += sol->zbark[j];
		}
		free_dataSet(data);
	}

	for(int j = 0 ; j < 100 ; j++)
	{
		if(number_n[j] != 0)
		{
			fprintf(f_time,"%d,%lf\n",j+1,time[j]/(double)number_n[j]);
			fprintf(f_gap,"%d,%lf\n",j+1,gap[j]/(double)number_n[j]);
		}
	}

	for(int j = 0 ; j < maxk ; j++)
	{
		if(number_k[j] != 0)
		{
			fprintf(f_z,"%d,%lf\n",j+1,z[j]/(double)number_k[j]);
			fprintf(f_zxk,"%d,%lf\n",j+1,zxk[j]/(double)number_k[j]);
		}
	}

	free(number_k);
	free(number_n);
	free(time);
	free(gap);
	free(z);
	free(zxk);
	fclose(f_time);
	fclose(f_gap);
	fclose(f_z);
	fclose(f_zxk);

}
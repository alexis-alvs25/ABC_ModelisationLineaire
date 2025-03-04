#include "TP5Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_COST 10

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
	dsptr->c = (int*)malloc(sizeof(int)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);
	dsptr->f = (int*)malloc(sizeof(int)*n);


	int i;
	for( i = 0 ; i < n ; i++)
		rval = fscanf(fin,"%d,%d,%d\n",&(dsptr->c[i]),&(dsptr->a[i]),&(dsptr->f[i]));

	fprintf(stderr,"\nInstance file read, we have capacites [b,g] = [%d,%d] and there is %d items of values/weights:\n",
			b,g,n);
	fprintf(stderr,"i\tci\tai\tfi\n");
	fprintf(stderr,"--------------------------\n");


	for( i = 0 ; i < n ; i++)
		fprintf(stderr,"%d\t%d\t%d\t%d\n",i,dsptr->c[i],dsptr->a[i],dsptr->f[i]);
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



void triSelec(dataSet* dsptr, double * utility, int * order)
{
		for (int i = 0; i < dsptr->n - 1; i++) {
        int max = i;
        for (int j = i + 1; j < dsptr->n; j++) {
            if (utility[j] > utility[max])
                max = j;
        }
       if (max != i) {
            double tempUt = utility[max];
            utility[max] = utility[i];
            utility[i] = tempUt;
			order[i] = max;
        }
    }
}

solution * create_solution(double * xbar, dataSet * dsptr, int * order)
{
	solution * sol = malloc(sizeof(solution));
	sol->xbar = xbar;
	sol->value = 0;
	for(int i = 0 ; i < dsptr->n ; i++)
	{
		sol->value += dsptr->c[i] * xbar[i];
	}
	sol->size = dsptr->n;
	sol->order = order;
	return sol;
}

void print_solution(solution * sol)
{
	printf("Value : %lf\nWith variable : ",sol->value);
	for(int i = 0 ; i < sol->size - 1; i++)
	{
		printf("x_%d = %lf,",i,sol->xbar[i]);
	}
	printf("x_%d = %lf\n",sol->size - 1,sol->xbar[sol->size - 1]);
}


void free_dataSet(dataSet * set)
{
	free(set->a);
	free(set->c);
}

void free_solution(solution * sol)
{
	free(sol->xbar);
	free(sol->order);
	free(sol);
}

void print_solution_csv(solution * sol, const char * path)
{
	FILE * f = fopen(path,"w");
		fprintf(f,"objective,%lf;\n",sol->value);
		fprintf(f,"x");
		for(int i = 0 ; i < sol->size ; i++)
		{
			fprintf(f,",%lf",sol->xbar[i]);
		}
		fprintf(f,";\n");
		fclose(f);

}

solution knapsack2d(dataSet* dpstr,double step,double epsilon)
{

}

/*void benchmark(int n, int b, int iteration)
{
	char basei[50] = "./benchmark/instance";
	char index[20];
	char base[50];
	char * lr = "_lr_";
	char * greed = "__greedy_";
	int i = 1;
	dataSet data;
	int seed = 42;
	for(i = 1 ; i < iteration/2 ; i++)
	{
		strcpy(base,basei);
		sprintf(index, "%d", i);
		
		strcat(base,index);
		create_instance(base, ++seed,b,-1);
		FILE * fin = fopen(base,"r");

		read_TP1_instance(fin,&data);
		fclose(fin);
		solution * sol = KP_greedy(&data);

		strcpy(base,basei);
		strcat(base,greed);
		strcat(base,index);

		print_solution_csv(sol,base);
		free_solution(sol);
		sol = KP_LP(&data);
		print_solution(sol);

		strcpy(base,basei);
		strcat(base,lr);
		strcat(base,index);

		print_solution_csv(sol,base);
		free_solution(sol);
		free_dataSet(&data);
	}
	for(; i <= iteration; i++)
	{
		strcpy(base,basei);
		sprintf(index, "%d", i);	
		strcat(base,index);
		create_instance(base, ++seed,-1,n);
		FILE * fin = fopen(base,"r");
		read_TP1_instance(fin,&data);
		fclose(fin);
		solution * sol = KP_greedy(&data);
		strcpy(base,basei);
		strcat(base,greed);
		strcat(base,index);

		print_solution_csv(sol,base);
		free_solution(sol);
		sol = KP_LP(&data);
		print_solution(sol);

		strcpy(base,basei);
		strcat(base,lr);
		strcat(base,index);
		print_solution_csv(sol,base);
		free_solution(sol);
		free_dataSet(&data);
	}

}*/


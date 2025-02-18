#include "TP1Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_COST 10

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
	dsptr->c = (int*)malloc(sizeof(int)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);


	int i;
	for( i = 0 ; i < n ; i++)
		rval = fscanf(fin,"%d,%d\n",&(dsptr->c[i]),&(dsptr->a[i]));

	//fprintf(stderr,"\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",b,n);
	/*for( i = 0 ; i < n ; i++)
		fprintf(stderr,"%d,%d\n",dsptr->c[i],dsptr->a[i]);
	//fprintf(stderr,"\n");*/


	return rval;
}

void create_instance(char * filePath,int seed, int b, int n)
{
	FILE * f = fopen(filePath, "w");
	srand(seed);
	if(b == -1)
		b = rand()%1001;
	if(n == -1)
		n = rand()%100;
	fprintf(f,"%d,%d\n",n,b);

	for(int i = 0 ; i < n ; i++)
	{
		int a = rand()%b+1;
		int c = rand()%MAX_COST;
		fprintf(f,"%d,%d\n",c,a);
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

solution * KP_LP(dataSet* dsptr)
{
	double * utility = malloc(dsptr->n * sizeof(double));
	for(int i = 0 ; i < dsptr->n ; i++)
	{
		utility[i] = (double)dsptr->c[i] / (double)dsptr->a[i];
	}

	int * order = malloc(dsptr->n * sizeof(int));
	for(int i = 0 ; i < dsptr->n ; i++)
	{
		order[i] = i;
	}

	triSelec(dsptr, utility,order);



	double * xbar = malloc(dsptr->n * sizeof(double));
	double bbar = (double)dsptr->b;

	printf("%d %d %d \n",order[0],dsptr->c[order[0]],dsptr->a[order[0]]);
	for(int j = 0 ; j < dsptr->n ; j++)
	{
		if(bbar == 0)
		{
			return create_solution(xbar, dsptr,order);
		}
		xbar[order[j]] = bbar / (double)dsptr->a[order[j]] > 1.0 ? 1.0 : bbar / (double)dsptr->a[order[j]];


		bbar = bbar - xbar[order[j]]*(double)dsptr->a[order[j]];
	}
	free(utility);
	return create_solution(xbar, dsptr,order);
}

solution * KP_greedy(dataSet* dsptr)
{
	double * utility = malloc(dsptr->n * sizeof(double));
	for(int i = 0 ; i < dsptr->n ; i++)
	{
		utility[i] = (double)dsptr->c[i] / (double)dsptr->a[i];
	}

	int * order = malloc(dsptr->n * sizeof(int));
	for(int i = 0 ; i < dsptr->n ; i++)
	{
		order[i] = i;
	}

	triSelec(dsptr, utility,order);



	double * xbar = malloc(dsptr->n * sizeof(double));
	double bbar = (double)dsptr->b;

	for(int j = 0 ; j < dsptr->n ; j++)
	{

		if(bbar <= 0)
		{
			return create_solution(xbar, dsptr,order);
		}
		if(bbar >= dsptr->a[j])
		{
			xbar[j] = 1;
			bbar -= dsptr->a[j];
		}
	}
	free(utility);
	return create_solution(xbar, dsptr,order);
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

void benchmark(int n, int b, int iteration)
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

}
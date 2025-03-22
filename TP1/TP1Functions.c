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

void create_instance_file(char * filePath,int seed, int b, int n)
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

dataSet * create_instance(int b, int n)
{
	
	if(b == -1)
		b = rand()%1001;
	if(n == -1)
		n = rand()%100;
	int * a = malloc(n * sizeof(int));
	int * c = malloc(n * sizeof(int));

	for(int i = 0 ; i < n ; i++)
	{
		a[i] = rand()%b+1;
		c[i] = rand()%MAX_COST;
	}
	dataSet * dpstr = malloc(sizeof(dataSet));
	dpstr->b = b;
	dpstr->n = n;
	dpstr->a = a;
	dpstr->c = c;
	return dpstr;

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




void benchmark(int n, int b, int iteration,int seed)
{
	
	FILE * f_lr_n = fopen("./benchmark/lr_n.csv","w"); //n fixé
	FILE * f_lr_b = fopen("./benchmark/lr_b.csv","w"); //b fixé
	FILE * f_greedy_n = fopen("./benchmark/greedy_n.csv","w");
	FILE * f_greedy_b = fopen("./benchmark/greedy_b.csv","w");
	int i = 0;
	dataSet * data;
	srand(seed);
	double * time_n = calloc(1000 , sizeof(double)); //n fixé
	double * time_b = calloc(100 , sizeof(double)); //b fixé
	int * number_n = calloc(1000 , sizeof(int));
	int * number_b = calloc(100 , sizeof(int));

	for(i = 1 ; i < iteration ; i++)
	{
		data = create_instance(b,-1);
		clock_t begin = clock();
		solution * sol = KP_greedy(data);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		time_b[data->n] += time_spent;
		number_b[data->n] += 1;
		//fprintf(f_greedy_b,"%d,%d,%lf\n",data->n,data->b,time_spent);
		free_dataSet(data);
		free_solution(sol);
	}

	for(int j = 0 ; j < 100 ; j++)
	{
		if(number_b[j] != 0)
			fprintf(f_greedy_b,"%d,%lf\n",j,time_b[j]/(double)number_b[j]);
		time_b[j] = 0;
		number_b[j] = 0;
	}

	for(i = 1 ; i < iteration ; i++)
	{
		data = create_instance(-1,n);
		clock_t begin = clock();
		solution * sol = KP_greedy(data);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		//fprintf(f_greedy_n,"%d,%d,%lf\n",data->n,data->b,time_spent);
		time_n[data->b] += time_spent;
		number_n[data->b] +=1;
		free_dataSet(data);
		free_solution(sol);
	}

	for(int j = 0 ; j < 1000 ; j++)
	{
		if(number_n[j] != 0)
			fprintf(f_greedy_n,"%d,%lf\n",j,time_n[j]/(double)number_n[j]);
		time_n[j] = 0;
		number_n[j] = 0;
	}

	for(i = 1 ; i < iteration ; i++)
	{
		data = create_instance(b,-1);
		clock_t begin = clock();
		solution * sol = KP_LP(data);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		time_b[data->n] += time_spent;
		number_b[data->n] += 1;
		//fprintf(f_lr_b,"%d,%d,%lf\n",data->n,data->b,time_spent);
		free_dataSet(data);
		free_solution(sol);
	}

	for(int j = 0 ; j < 100 ; j++)
	{
		if(number_b[j] != 0)
			fprintf(f_lr_b,"%d,%lf\n",j,time_b[j]/(double)number_b[j]);
	}

	for(i = 1 ; i < iteration ; i++)
	{
		data = create_instance(-1,n);
		clock_t begin = clock();
		solution * sol = KP_LP(data);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		time_b[data->b] += time_spent;
		number_b[data->b] +=1;
		//fprintf(f_lr_n,"%d,%d,%lf\n",data->n,data->b,time_spent);
		free_dataSet(data);
		free_solution(sol);
	}

	for(int j = 0 ; j < 1000 ; j++)
	{
		if(number_n[j] != 0)
			fprintf(f_lr_n,"%d,%lf\n",j,time_n[j]/(double)number_n[j]);
	}
	free(number_b);
	free(number_n);
	free(time_b);
	free(time_n);
	fclose(f_lr_b);
	fclose(f_lr_n);
	fclose(f_greedy_b);
	fclose(f_greedy_n);

}
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

solution create_solution(int * xbar, dataSet * dsptr)
{
	solution sol = malloc(sizeof(solution));
	sol->xbar = xbar;
	sol->value = 0;
	for(int i = 0 ; i < dsptr->n ; i++)
	{
		sol->value += dsptr->c[i] * xbar[i];
	}
	sol->size = dsptr->n;
	sol->xbark = NULL;
	sol->zbark = NULL;
	return sol;
}

void print_solution(solution sol)
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

void free_solution(solution sol)
{
	free(sol->xbar);
	if(sol->xbark != NULL)
	{
		free(sol->xbark);
	}
	if(sol->zbark != NULL)
	{
		free(sol->zbark);
	}
	free(sol);
	
}

void print_solution_csv(solution sol, const char * path)
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

solution KP_dynamic(dataSet * data) {
	
	int i;
	int * Z = (int *)calloc((data->b + 1), sizeof(int));
	int * Z_temp = (int *)calloc((data->b + 1), sizeof(int));
	int * D = (int *)calloc((data->b + 1), sizeof(int));

	for (int k = 0; k < data->n; k++) {
		for (i = data->b; i >= data->a[k]; i--) {
			Z_temp[i] = Z[i];
		}
		for (i = data->a[k]; i <= data->b; i++) {
			if (Z_temp[i - data->a[k]] + data->c[k] > Z_temp[i]) {
				D[i] = k + 1;
				Z[i] = MAX(Z_temp[i], Z_temp[i - data->a[k]] + data->c[k]);
			}
		}
	}

	int * x = (int *)calloc(data->n, sizeof(int));

	i = data->b;
	
	while (i > 0) {
		while (Z[i] == Z[i - 1]) {
			i--;
		}
		x[D[i] - 1] = 1;
		i -= data->a[D[i] - 1];
	}

	// Affichage des résultats
	printf("Valeur maximale : %d", Z[data->b]);
	printf("\nTableau Z = ");
	for (int j = 0; j <= data->b; j++) {
		printf("%d ", Z[j]);
	}
	printf("\nTableau D = ");
	for (int j = 0; j <= data->b; j++) {
		printf("%d ", D[j]);
	}

	printf("\n");
    printf("Objets sélectionnés : ");
    for (int j = 0; j < data->n; j++) {
        if (x[j]) {
            printf("%d ", j + 1);
        }
    }
    printf("\n");

	free(Z);
	free(Z_temp);
	free(D);
	return create_solution(x,data);
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

solution knapsack2d(dataSet* dpstr,double * step,int size,double epsilon)
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
		solution sol = KP_dynamic(tmpdata);
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
		free_solution(sol);
	}
	free(tmpdata);
	free(tmpcost);
	
	solution sol = create_solution(xbark[k-1],dpstr);
	sol->zbark = zbark;
	sol-> xbark = xbark;
	return sol;
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


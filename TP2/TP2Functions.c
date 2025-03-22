//TP2Functions.c
#include "TP2Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_COST 1000

int read_TP2_instance(FILE*fin,dataSet* dsptr)
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

	fprintf(stderr,"\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",b,n);
	for( i = 0 ; i < n ; i++)
		fprintf(stderr,"%d,%d\n",dsptr->c[i],dsptr->a[i]);
	fprintf(stderr,"\n");


	return rval;
}

void create_instance_file(char * filePath,int seed)
{
	FILE * f = fopen(filePath, "w");
	srand(seed);
	int b = rand()%1000 + 1;				//capacite maximale
	int n = rand()%100 + 1;					//nombre d'objets
	fprintf(f,"%d,%d\n",n,b);

	for(int i = 0 ; i < n ; i++)
	{
		int a = rand()%b+1;					//poids
		int c = rand()%MAX_COST;			//valeur
		fprintf(f,"%d,%d\n",c,a);	
	}
	fclose(f);
}

dataSet * create_instance(int b, int n) {
	
	if(b == -1)
		b = rand()%1000 + 1;
	if(n == -1)
		n = rand()%100 + 1;
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

int compare_ratios(const void * p1, const void * p2) {
	double ratio1 = *(int *)p1 / (double)*(int *)(p1 + sizeof(int));
	double ratio2 = *(int *)p2 / (double)*(int *)(p2 + sizeof(int));
	if (ratio1 > ratio2) return -1; 												// ordre décroissant
	if (ratio1 < ratio2) return 1;
	return 0;
}

void sort_items(dataSet * data) {													//tri des objets par ordre décroissant de leur ratio valeur/poids
	int *items = (int *)malloc(data->n * 2 * sizeof(int));
	for (int i = 0; i < data->n; i++) {
		items[2 * i] = data->c[i];
		items[2 * i + 1] = data->a[i];
	}
	qsort(items, data->n, 2 * sizeof(int), compare_ratios);
	for (int i = 0; i < data->n; i++) {
		data->c[i] = items[2 * i];
		data->a[i] = items[2 * i + 1];
	}
	free(items);
}

void print_items(dataSet *data) {
    printf("Objects (value, weight, ratio) :\n");
    for (int i = 0; i < data->n; i++) {
        double ratio = (double)data->c[i] / data->a[i];
        printf("x%d  :  value = %d,   weight = %d,   ratio = %.2f\n", i + 1, data->c[i], data->a[i], ratio);
    }
    printf("\n");
}

void print_solution(solution * sol, int n) {
	printf("Max Value: %d\n", sol->max_value);
	printf("Selected Items: ");
	for (int i = 0; i < n; i++) {
		if (sol->selected_items[i]) {
			printf("x%d ", i + 1);
		} 
	}
	printf("\n");
}

solution * create_solution(int n) {
    solution * sol = malloc(sizeof(solution));
    sol->selected_items = calloc(n, sizeof(int));
    sol->max_value = 0;  // initialisation par défaut
    return sol;
}

solution * KP_dynamic(dataSet * data) {

    solution * sol = create_solution(data->n);

    int *Z = calloc(data->b + 1, sizeof(int));        // Z(y)
    int *Z_temp = calloc(data->b + 1, sizeof(int));    // Z'(y)
    int *D = calloc(data->b + 1, sizeof(int));         // D(y)

    for (int k = 0; k < data->n; k++) {
        // Copier Z dans Z_temp
        for (int y = 0; y <= data->b; y++) {
            Z_temp[y] = Z[y];
        }
        for (int y = data->a[k]; y <= data->b; y++) {
            int candidate = Z_temp[y - data->a[k]] + data->c[k];
            if (candidate > Z_temp[y]) {
                Z[y] = candidate;
                D[y] = k + 1;
            }
        }
    }

    int y = data->b;
    while (y > 0) {
        while (y > 0 && D[y] == 0) {
            y--;
        }
        if (y <= 0)
            break;
        int item = D[y] - 1;
        sol->selected_items[item] = 1;
        y -= data->a[item];
    }

    sol->max_value = Z[data->b];

    free(Z);
    free(Z_temp);
    free(D);

    return sol;
}

void benchmark(int n, int b, int iteration, int seed) {
	FILE * f_dynamic_n = fopen("./benchmark/dynamic_n.csv","w");
	FILE * f_dynamic_b = fopen("./benchmark/dynamic_b.csv","w");
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
		solution * sol = KP_dynamic(data);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		time_b[data->n] += time_spent;
		number_b[data->n] += 1;
		//fprintf(f_dynamic_b,"%d,%d,%lf\n",data->b,data->n,time_spent);
		free_dataSet(data);
		free_solution(sol);
	}
	for(int j = 0 ; j < 100 ; j++)
	{
		if(number_b[j] != 0)
			fprintf(f_dynamic_b,"%d,%lf\n",j+1,time_b[j]/(double)number_b[j]);
		time_b[j] = 0;
		number_b[j] = 0;
	}

	for(i = 1 ; i < iteration ; i++)
	{
		data = create_instance(-1,n);
		clock_t begin = clock();
		solution * sol = KP_dynamic(data);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		time_n[data->b] += time_spent;
		number_n[data->b] += 1;		
		//fprintf(f_dynamic_n,"%d,%d,%lf\n",data->n,data->b,time_spent);
		free_dataSet(data);
		free_solution(sol);
	}
	for(int j = 0 ; j < 100 ; j++)
	{
		if(number_n[j] != 0)
			fprintf(f_dynamic_n,"%d,%lf\n",j+1,time_n[j]/(double)number_n[j]);
		time_n[j] = 0;
		number_n[j] = 0;
	}

	fclose(f_dynamic_b);
	fclose(f_dynamic_n);
	free(time_b);
	free(time_n);
	free(number_b);
	free(number_n);
}

void free_dataSet(dataSet * set) {
	free(set->c);
	free(set->a);
	free(set);
}

void free_solution(solution * sol) {
	free(sol->selected_items);
	free(sol);
}
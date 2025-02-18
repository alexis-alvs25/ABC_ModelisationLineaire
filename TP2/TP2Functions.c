#include "TP2Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>

#define MAX_COST 1000
#define MAX(a,b) ((a) > (b) ? (a) : (b))

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

void create_instance(char * filePath,int seed)
{
	FILE * f = fopen(filePath, "w");
	srand(seed);
	int b = rand()%1001;				//capacite maximale
	int n = rand()%100;					//nombre d'objets
	fprintf(f,"%d,%d\n",n,b);

	for(int i = 0 ; i < n ; i++)
	{
		int a = rand()%b+1;					//poids
		int c = rand()%MAX_COST;			//valeur
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
    printf("Objets (valeur, poids, ratio) :\n");
    for (int i = 0; i < data->n; i++) {
        double ratio = (double)data->c[i] / data->a[i];
        printf("Objet %d : valeur = %d, poids = %d, ratio = %.2f\n", i + 1, data->c[i], data->a[i], ratio);
    }
    printf("\n");
}

void KP_dynamic(dataSet * data) {
	
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
	free(x);
}
 

 
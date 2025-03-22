//
// Created by Bastien Fayolle on 28/01/2025.
//

#include "TP3Functions.h"
#include "../TP1/TP1Functions.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_COST 100
#define MAX(a,b) ((a) > (b) ? (a) : (b))
dataSet  preprocessing(dataSet *data)
{

    dataSet newData;
    newData.b=data->b;
    solution* sol_greed  = KP_greedy(data);
    solution * ideal = KP_LP(data);
    int j,p;
    float optval;
	if(DISPLAY){
    printf("L'algorithme glouton donne : %f\n, la relaxation linéaire : %f\n", sol_greed->value, ideal->value);
    }
    p=0;
    int numberOfObject = data->n;

    while(data->b>sumWeight(p, data->a)){
		p++;
    }
    if(DISPLAY){

    printf("b : %d, (bernard) pivot : %d\nsum: %d\n",data->b, p,sumWeight(1,data->a));
    }

    int* fixed0=calloc(data->n, sizeof(int)); // array of fixed 0, 1 if the n value of the pb is fixed at 0, 0 otherwise
    int* fixed1=calloc(data->n,sizeof(int)); // array of fixed 1, 1 if the n value of the pb is 1, 0 otherwise


    for(j=0; j<data->n; j++){
        optval=fabs((float)data->c[j]-(((float)data->c[p]/(float)data->a[p])*(float)data->a[j]));
        recalcSol(sol_greed,data);
        if(optval>=ideal->value -sol_greed->value ){
          if(j<=p-1){
            sol_greed->xbar[j]=1;
            fixed0[j]=0;
            fixed1[j]=1;
            data->b-=data->a[j];
            numberOfObject--;
          }
          else{
            sol_greed->xbar[j]=0;
            numberOfObject--;
            fixed0[j]=1;
            fixed1[j]=0;
            data->b-=data->a[j];
          }
        }
        else{
          fixed0[j]=0;
          fixed1[j]=0;
          }
    }
    /*
    for(j=0; j<data->n; j++){
      printf("lists %d : %d %d\n",j,fixed0[j],fixed1[j]);
    }
     */
    int k=0;
    data->b=newData.b;
    newData.n = numberOfObject;
    if(DISPLAY){
    printf("datab : %d\n\n",data->b);
    }
    for(j=0; j<data->n; j++){
      if(fixed1[j]){
        if(DISPLAY){
        printf("%d %d\n",j , data->a[j]);
          }
        newData.b-=data->a[j];
        }
    }
    newData.c = (int*)malloc(sizeof(int)*numberOfObject);
    if(newData.c==NULL){
      printf("Erreur d'allocation de c\n");
      exit(-1);
    }
    for(j=0; j<data->n; j++){
      if(!fixed0[j]&&!fixed1[j]){
        newData.c[k]=data->c[j];
        k++;
        }
    }
    newData.a = (int*)malloc(sizeof(int)*numberOfObject);

    if(newData.a==NULL){
      printf("Erreur d'allocation\n");
      exit(-1);
    }
    k=0;
    for(j=0; j<data->n; j++){
      if(!fixed0[j]&&!fixed1[j]){
        newData.a[k]=data->a[j];
        k++;
        }
    }
    return newData;
}



int sumWeight(int n, int* a){
	int sum = 0;
    for(int i=0; i<=n; i++){
      sum += a[i];
    }
    return sum;
}

void recalcSol(solution * sol, dataSet *data){
  double z = 0;
  for(int i=0; i<data->n; i++){
      z+=sol->xbar[i]*data->c[i];
  }
  sol->value = z;
}


void benchmarkTP3( int iteration,int seed,int objects){
  	char filename[50];
    sprintf(filename,"./benchmark/preprocessingn=%d.csv",objects);
	FILE * f_prep = fopen(filename, "w");
    int i=0;
    int n,b;
    dataSet  * data;
    srand(seed);
    for(int j=0;j<iteration;j++){
    	data=create_instance(-1,objects);
		if(DISPLAY){
        	printf("\n-------\nb : %d, n: %d\n\n",data->b, data->n);
        	print_items(data);
        }
        n=data->n;
        b=data->b;
        clock_t begin = clock();
        *data=preprocessing(data);
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        // writing n, b, and time in file
        if(DISPLAY){
	        printf("n = %d, Ratio : %f, number of processed: %d \n", n,(float)(n-data->n)/(float)n, n-data->n);
    		for(i=0;i<data->n;i++){
    			printf("%dx%d ",data->c[i],i);
                        }
    		printf("\nsc : ");
    		for(i=0;i<data->n-1;i++){
    			printf("%dx%d + ",data->a[i],i);
                      }
    		printf("%dx%d\n---END--\n\n",data->a[data->n-1],data->n-1);
        }
        fprintf(f_prep," %d,%d,%f,%f\n",n,b,time_spent,(float)(n-data->n)/(float)n);

    }
    fclose(f_prep);
}


// TP2

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


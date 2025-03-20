//
// Created by Bastien Fayolle on 28/01/2025.
//

#include "TP3Functions.h"
#include "../TP1/TP1Functions.h"
#include <stdio.h>
#include <stdlib.h>
dataSet  preprocessing(dataSet *data)
{

    dataSet newData;
    newData.b=data->b;
    solution* sol_greed  = KP_greedy(data);
    solution * ideal = KP_LP(data);
    int j,p;
    float optval;

    printf("L'algorithme glouton donne : %f\n, la relaxation linéaire : %f\n", sol_greed->value, ideal->value);
    p=0;
    int numberOfObject = data->n;

    while(data->b>sumWeight(p, data->a)){
		p++;
    }
    printf("b : %d, (bernard) pivot : %d\nsum: %d\n",data->b, p,sumWeight(1,data->a));

    int* fixed0=calloc(data->n, sizeof(int)); // array of fixed 0, 1 if the n value of the pb is fixed at 0, 0 otherwise
    int* fixed1=calloc(data->n,sizeof(int)); // array of fixed 1, 1 if the n value of the pb is 1, 0 otherwise


    for(j=0; j<data->n; j++){
        optval=fabs((float)data->c[j]-(((float)data->c[p]/(float)data->a[p])*(float)data->a[j]));
        recalcSol(sol_greed,data);
        printf("optval : %f, compared to %f \n",optval, fabs(ideal->value -sol_greed->value ));
        if(optval>=ideal->value -sol_greed->value ){
          if(j<=p-1){
            printf("1\n");
            sol_greed->xbar[j]=1;
            fixed0[j]=0;
            fixed1[j]=1;
            data->b-=data->a[j];
            numberOfObject--;
          }
          else{
            printf("2\n");
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
    for(j=0; j<data->n; j++){
      printf("lists %d : %d %d\n",j,fixed0[j],fixed1[j]);
    }
    int k=0;
    newData.n = numberOfObject;
    printf("datab : %d\n\n",data->b);
    for(j=0; j<data->n; j++){
      if(fixed1[j]){
        printf("%d %d\n",j , data->a[j]);
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
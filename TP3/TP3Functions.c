//
// Created by Bastien Fayolle on 28/01/2025.
//

#include "TP3Functions.h"
#include "TP2Functions.h"
#include "TP1Functions.h"
#include <stdio.h>
#include <stdlib.h>

solution * withPreprocessing(dataSet *data)
{
    solution* xtilde  = KP_greedy(data);
    solution * ideal = KP_LP(data);
 	double * xbar = malloc(data->n * sizeof(double));

    int j,p,sum, optval ;
    while(p<sumWeight(data->n, data->a)){
		p++;
    }
    for(j=0; j<data->n; j++){
		printf("%d",j);
        optval=abs(data->c[j]-((data->c[p]/data->a[p])*data->a[j]));
        if(optval>=(ideal->value - xtilde->value)){
          if(j<=p-1){
            xbar[j]

          }
	}

}


int sumWeight(int n, int* a){
	int sum = 0;
    for(int i=0; i<n; i++){
      sum += a[i];
    }
    return sum;
}
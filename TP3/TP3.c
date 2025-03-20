//
// Created by Bastien Fayolle on 04/03/2025.
//

#include "TP3Functions.h"

int main(){
  dataSet dt;
  FILE * fin = fopen("dataset.txt","r");
    read_TP1_instance(fin, &dt);
    printf("Dataset : %d\n\n", dt.n);
    dataSet dta = preprocessing(&dt);
    printf("Dataset : n : %d, b : %d\n\n", dta.n,dta.b);
    for(int i=0;i<dta.n;i++){
      printf("%dx%d ",dta.c[i],i);
    }
    printf("\nsc : ");
    for(int i=0;i<dta.n-1;i++){
      printf("%dx%d + ",dta.a[i],i);
    }
    printf("%dx%d\n",dta.a[dta.n-1],dta.n-1);



}
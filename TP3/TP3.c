//
// Created by Bastien Fayolle on 04/03/2025.
//

#include "TP3Functions.h"
#define MAX_COST 100

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

 /* printf("Avant le tri :\n");
  print_items(&dt);

  sort_items(&dt);

  printf("Après le tri :\n");
  print_items(&dt);
*/
  //execute your solution methods on the instance you just read
//   KP_dynamic(&dt);

  free(dt.c);
  free(dt.a);
/*
  printf("DTA : \n");


  printf("Avant le tri :\n");
  print_items(&dta);

  sort_items(&dta);

  printf("Après le tri :\n");
  print_items(&dta);

  //execute your solution methods on the instance you just read
  //KP_dynamic(&dta);
*/
  free(dta.c);
  free(dta.a);
  for(int l=0;l<=200;l+=10){
    benchmarkTP3(20,42,l);
  }



}
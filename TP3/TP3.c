//
// Created by Bastien Fayolle on 04/03/2025.
//

#include "TP3Functions.h"

int main(){
  dataSet dt;
  FILE * fin = fopen("dataset.txt","r");
    read_TP1_instance(fin, &dt);
    printf("Dataset : %d\n\n", dt.n);
    solution* s = withPreprocessing(&dt);

}
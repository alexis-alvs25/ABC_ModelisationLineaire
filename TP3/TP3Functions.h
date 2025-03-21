//
// Created by Bastien Fayolle on 28/01/2025.
//

#ifndef TP3FUNCTIONS_H
#define TP3FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include "../TP1/TP1Functions.h"
#define DISPLAY 1


/**
* @brief Compute the objectiv value of the given x solution vector (for exact solution), for the given data set
* @param x the solution vector
* @param data the data set (the problem instance)
* @return the objectiv value , Z
*/
int objectivValue(int *x, dataSet *data);


/**
* @brief Compute the objectiv value of the given x solution vector (for linear relaxation), for the given data set
* @param x the solution vector of a relaxed problem
* @param data the data set (the problem instance)
* @return the objectiv value , Z
*/
double objectivRelaxed(double *x, dataSet *data);


dataSet  preprocessing(dataSet *data);
int sumWeight(int n, int * a);

/**
* calcSol : compute the objectiv value for the givens solution and dataset
*/
void recalcSol(solution *sol, dataSet *data);

void benchmarkTP3(int iteration, int seed,int n);

// TP2
int compare_ratios(const void *p1, const void *p2);
void sort_items(dataSet *data);
void print_items(dataSet *data);
void KP_dynamic(dataSet * data);

#endif //TP3FUNCTIONS_H

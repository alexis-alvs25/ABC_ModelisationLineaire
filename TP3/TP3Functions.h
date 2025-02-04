//
// Created by Bastien Fayolle on 28/01/2025.
//

#ifndef TP3FUNCTIONS_H
#define TP3FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>


typedef struct dataSet
{
    //Attributes of the instance
    //Nombre d'objets
    int n;
    //Capacite b
    int b;
    //Tableau d'entiers de taille n contenant la valeur de chacun des objets
    int*c;
    //Tableau d'entiers de taille n contenant le poids de chacun des objets
    int*a;


} dataSet;

typedef struct solution {
   double value;
   double * xbar;
   int size;
   int * order;
    } solution;
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


solution * withPreprocessing(dataSet *data);
solution *KP_greedy(dataSet* dsptr);
solution * KP_LP(dataSet* dsptr);
int sumWeight(int n, int * a);


#endif //TP3FUNCTIONS_H

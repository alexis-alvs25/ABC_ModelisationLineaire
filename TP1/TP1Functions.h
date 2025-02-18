#ifndef TP1FUNCTIONS_H
#define TP1FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <assert.h>




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

int read_TP1_instance(FILE*fin,dataSet* dsptr);
solution * KP_greedy(dataSet* dsptr);
solution * KP_LP(dataSet* dsptr);
void print_solution(solution * sol);
void create_instance(char * filePath,int seed, int b,int n);
void free_dataSet(dataSet * set);
void free_solution(solution * sol);
void benchmark(int n, int b, int iteration);

#endif
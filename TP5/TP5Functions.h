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
	//Capacite g 
	int g;

	//Tableau d'entiers de taille n contenant la valeur de chacun des objets
	int*c;
	//Tableau d'entiers de taille n contenant le poids de chacun des objets
	int*a;
	//Tableau d'entiers de taille n contenant le poids #2 de chacun des objets
	int*f;

} dataSet;

typedef struct solution {
	double value;
	double * xbar;
	int size;
	int * order;
} solution;

int read_TP5_instance(FILE*fin,dataSet* dsptr);
void print_solution(solution * sol);
solution knapsack2d(dataSet* dpstr,double step,double epsilon);
void create_instance(char * filePath,int seed, int b, int g, int n);
void free_dataSet(dataSet * set);
void free_solution(solution * sol);
//void benchmark(int n, int b, int iteration);



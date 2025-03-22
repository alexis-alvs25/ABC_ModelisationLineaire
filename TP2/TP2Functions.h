//TP2Functions.h
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

typedef struct {
	int max_value;
	int * selected_items;
} solution;

int read_TP2_instance(FILE*fin,dataSet* dsptr);
void create_instance_file(char * filePath,int seed);
int compare_ratios(const void *p1, const void *p2);
void sort_items(dataSet *data);
void print_items(dataSet *data);
void print_solution(solution * sol, int n);
solution * KP_dynamic(dataSet * data);
solution * create_solution(int n);
void free_dataSet(dataSet * set);
void free_solution(solution * sol);
dataSet * create_instance(int b, int n);
void benchmark(int n, int b, int iteration,int seed);
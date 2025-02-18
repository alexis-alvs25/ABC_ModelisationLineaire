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

int read_TP2_instance(FILE*fin,dataSet* dsptr);
void create_instance(char * filePath,int seed);
int compare_ratios(const void *p1, const void *p2);
void sort_items(dataSet *data);
void print_items(dataSet *data);
void KP_dynamic(dataSet * data);
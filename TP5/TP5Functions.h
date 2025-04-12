#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <ilcplex/cplex.h>
#include "./TP4Functions.h"

typedef struct solution {
	double value;
	double * xbar;
} * solution;

typedef struct int_solution {
	double value;
	int * xbar;
	int size;
	double gap;
	int kmax;
	double * zbar;
	double * zbark;
} * int_solution;

void benchmark_TP5(int n, int b,int g, int iteration,int seed, int maxk,double * step);
int read_TP5_instance(FILE*fin,dataSet* dsptr);
void print_final_solution(int_solution sol);
int_solution knapsack2d(dataSet* dpstr,double * step, int k,double epsilon);
void create_instance(char * filePath,int seed, int b, int g, int n);
void free_dataSet(dataSet * set);
void free_solution(solution sol);
//void benchmark(int n, int b, int iteration);



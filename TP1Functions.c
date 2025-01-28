#include "TP1Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>

#define MAX_COST 1000

int read_TP1_instance(FILE*fin,dataSet* dsptr)
{
	int rval = 0;

	//capacite b
	int b;
	//Nombre d'objets
	int n;
	rval = fscanf(fin,"%d,%d\n",&n,&b);
	dsptr->b = b;
	dsptr->n = n;
	dsptr->c = (int*)malloc(sizeof(int)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);


	int i;
	for( i = 0 ; i < n ; i++)
		rval = fscanf(fin,"%d,%d\n",&(dsptr->c[i]),&(dsptr->a[i]));

	fprintf(stderr,"\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",b,n);
	for( i = 0 ; i < n ; i++)
		fprintf(stderr,"%d,%d\n",dsptr->c[i],dsptr->a[i]);
	fprintf(stderr,"\n");


	return rval;
}

void create_instance(char * filePath,int seed)
{
	FILE * f = fopen(filePath, "w");
	srand(seed);
	int b = rand()%1001;
	int n = rand()%100;
	fprintf(f,"%d,%d\n",n,b);

	for(int i = 0 ; i < n ; i++)
	{
		int a = rand()%b+1;
		int c = rand()%MAX_COST;
		fprintf(f,"%d,%d\n",c,a);
	}
	fclose(f);

}

int KP_greedy(dataSet* dsptr)
{
	int rval = 0;


	return rval;
}

void triSelec(dataSet* dsptr, double * utility)
{
		for (int i = 0; i < dsptr->n - 1; i++) {
        int max = i;
        for (int j = i + 1; j < dsptr->n; j++) {
            if (utility[j] > utility[max])
                max = j;
        }
       if (max != i) {
            double tempUt = utility[max];
			int tempA = dsptr->a[max];
			int tempC = dsptr->c[max];
            utility[max] = utility[i];
			dsptr->a[max] = dsptr->a[i];
			dsptr->c[max] = dsptr->c[i];
            utility[i] = tempUt;
			dsptr->a[i] = tempA;
			dsptr->c[i] = tempC;
        }
    }
}

int KP_LP(dataSet* dsptr)
{
	int rval = 0;
	double * utility = malloc(dsptr->n * sizeof(double));
	for(int i = 0 ; i < dsptr->n ; i++)
	{
		utility[i] = dsptr->c[i] / dsptr->a[i];
	}

	triSelec(dsptr, utility);

	for(int i = 0 ; i < dsptr->n ; i++)
	{
		printf("%lf\n",utility[i]);
	}

	double * xbar = malloc(dsptr->n * sizeof(double));
	int rval = 0;
	int b=dsptr->b;
	int j;
	int temp;
	for(j=0; j<dsptr->n; j++){
		if(b==0) {
			return rval;
		};
		temp=b/dsptr->a[j];
		x[j]=1<temp?1:temp;
		b=b-x[j]*dsptr->a[j];
	};


	return rval;
}


//TP2.c
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "TP2Functions.h"


int main(int argc, char **argv)
{

	//Create a random instance
	//create_instance_file("instance1.csv", time(NULL));
	

	int rval =0;	
	//File instance name
	//-F option
    char instance_file[1024];
    snprintf(instance_file, 1024, "%s", "instance1.csv");

	char c;
    while ((c=getopt (argc, argv,"F:h")) != EOF) {
		
        switch(c) {
            case 'F':
				snprintf(instance_file, 1024, "%s", optarg);
				printf("Instance file: %s\n", instance_file);				//print the instance file name
			break;

			case 'h':
				fprintf(stderr,"Usage: ./TP2 [options]\nOptions:\n\n");
				fprintf(stderr,"******** INSTANCE DATA ********\n");
				fprintf(stderr,"\t-F Instance file name to load............................(default %s).\n",instance_file);
			break;
			
            default:
				exit(0);
		}
	}

 	dataSet * data = malloc(sizeof(dataSet));

	//Open the instance file
	FILE* fin = fopen(instance_file,"r");
	read_TP2_instance(fin,data);
	fclose(fin);

	//Exemple du TP2
/* 	FILE* fin1 = fopen("exempleTP2.csv","r");
	read_TP2_instance(fin1,data);
	fclose(fin1);  */

	//execute your solution methods on the instance you just read
	sort_items(data);
	print_items(data);
	solution * sol = KP_dynamic(data);
	print_solution(sol, data->n);
	free_dataSet(data);
	free_solution(sol);

	/* benchmark(100,1000,1000000,42); */

	return rval;
}


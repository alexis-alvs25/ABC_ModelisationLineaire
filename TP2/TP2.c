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
	create_instance("instance1.csv", time(NULL));
	

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

 	dataSet data;
	//Exemple du TP2
	
	data.n = 4;
	data.b = 11;
	data.a = (int *)malloc(data.n * sizeof(int));
	data.c = (int *)malloc(data.n * sizeof(int));

	data.a[0] = 5; data.c[0] = 12;
	data.a[1] = 4; data.c[1] = 8;
	data.a[2] = 1; data.c[2] = 2;
	data.a[3] = 3; data.c[3] = 5;


	//Open the instance file
/* 	FILE* fin = fopen(instance_file,"r");
	read_TP2_instance(fin,&data);
	fclose(fin);
*/
    printf("Avant le tri :\n");
    print_items(&data);

    sort_items(&data);

    printf("Après le tri :\n");
    print_items(&data);

	//execute your solution methods on the instance you just read
	KP_dynamic(&data);

    free(data.c);
    free(data.a);

	return rval;
}


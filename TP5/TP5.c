#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "TP5Functions.h"
#define MAXSTEP 2000


int main(int argc, char **argv)
{	
	//File instance name
	//-F option
        char instance_file[1024];
        snprintf(       instance_file,
                        1024,
                        "%s",
                        "instance1.csv");

	char c;
        while ((c=getopt (argc, argv,"F:h")) != EOF)
	{
		switch(c)
		{
                        case 'F':
				snprintf(       instance_file,
						1024,
						"%s",
						optarg);
				break;

			case 'h':
				fprintf(stderr,"Usage: ./TP1 [options]\nOptions:\n\n");
				fprintf(stderr,"******** INSTANCE DATA ********\n");
				fprintf(stderr,"\t-F Instance file name to load............................(default %s).\n",instance_file);
				break;
			default:
				exit(0);
		}
	}
	//dataSet * data = create_instance2d(100, 100,10);
	double step[MAXSTEP];
	for(int i = 1 ; i <= MAXSTEP ; i++)
	{
		step[i-1] = 1.0/(double)i;
	}


fprintf(stderr,"debug2\n");
	//execute your solution methods on the instance you just read
	/*int_solution sol = knapsack2d(data,step,MAXSTEP,0.001);
	print_final_solution(sol);*/
	benchmark_TP5(-1, 1000,1000, 10000,42, MAXSTEP,step);

	/*free(sol->xbar);
	free(sol);
	free_dataSet(data);*/
	
	//benchmark(100,100,5);
	return 0;
}


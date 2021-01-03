#include <stdio.h>
#include <stdlib.h>
#include <math.h>	
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "lib_Astar.h"


//gcc csvreader.c -o csvread -Ofast -lm && time ./csvread preprocessing/parsedfiles/nodes_clean.csv preprocessing/parsedfiles/ways_clean.csv
//DEBUGGING:  gcc csvreader.c -o csvread -lm -fsanitize=address -static-libasan -g -Wall && time ./csvread preprocessing/parsedfiles/nodes_clean.csv preprocessing/parsedfiles/ways_clean.csv

int main(int argc, char *argv[]){
	unsigned long n_nodes = 3472620UL; //change: cataluna 3472620; spain 23895681
	unsigned long waynode;
	bool oneway;
	int n;

	ssize_t line_chars = 79857;
	size_t line_bytes  = line_chars;
	size_t field_bytes = 184;


	char *filename_nodes = argv[1];
	char *filename_ways = argv[2];
	char *line_buff, *field, *linec;

	nodetype *nodes;
	unsigned long *ways;

	FILE *fp;

	int lc = 0;
	

	if((line_buff = (char *)		malloc(sizeof(char) * line_bytes	)) == NULL){
		perror("Unable to allocate buffer");
		exit(1);
    }

   	if((field     = (char *)		malloc(sizeof(char) * field_bytes	)) == NULL){
		perror("Unable to allocate buffer");
		exit(1);
    }

   	if((nodes    = (nodetype *)		malloc(sizeof(nodetype) * n_nodes	)) == NULL){
		perror("Unable to allocate buffer");
		exit(1);
    }

   	if((ways     = (unsigned long *)malloc(sizeof(unsigned long) * 5306 )) == NULL){
		perror("Unable to allocate buffer");
		exit(1);
    }



	
	nodes = (nodetype *)malloc(sizeof(nodetype) * n_nodes);
	
	ways = (unsigned long *)malloc(sizeof(unsigned long));

/*
	cat cataluna.csv | grep ^node | wc -l
	cat cataluna.csv | grep ^way  | wc -l
*/


//====== LOAD NODES ==============================================================

	fp = fopen(filename_nodes, "r");
	if (!fp){
		fprintf(stderr, "Error opening file '%s'\n", filename_nodes);
		return EXIT_FAILURE;
	}

	while ( (line_chars = getline(&line_buff, &line_bytes, fp)) != -1) {
		linec = line_buff;
		lc++;

		//printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", lc, line_chars, line_bytes, line_buff);
		field = strsep(&linec, "|");
			nodes[lc-1].id = strtoul(field, NULL, 10);


		field = strsep(&linec, "|");
			nodes[lc-1].name = (char *) malloc(sizeof(char) * (strlen(field) + 1));
			strcpy(nodes[lc-1].name, field);

		field = strsep(&linec, "|");
			nodes[lc-1].lat = strtod(field, NULL);


		field = strsep(&linec, "|");
			nodes[lc-1].lon = strtod(field, NULL);


		if (lc % 100 == 0){
			printf("loaded %d nodes.\r", lc);
		}

	}
	fclose(fp);
	printf("loaded %d nodes.\n", lc);



//====== LOAD WAYS ==============================================================

	fp = fopen(filename_ways, "r");
	if (!fp){
		fprintf(stderr, "Error opening file '%s'\n", filename_ways);
		return EXIT_FAILURE;
	}
	
	printf("\nStarting to read ways...\n");
	lc = 0;
	while (( line_chars = getline(&line_buff, &line_bytes, fp)) != -1) {
		linec = line_buff;
		lc++;
		//printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", lc, line_chars, line_bytes, line_buff);
		printf("line[%06d]: chars=%06zd, buf size=%06zu\n", lc, line_chars, line_bytes);
		//printf("%p \n",(void*)&fp);

		field = strsep(&linec, "|");
		field = strsep(&linec, "|");

		if (strlen(field)) {oneway = true;} else {oneway = false;}
/*
		while( (field = strsep(&linec, "|")) != NULL) {
			waynode = strtoul(field, NULL, 10);
			n = 0;
			if (binSearchNode(waynode, nodes, n_nodes) != ULONG_MAX) {
					ways[n] = waynode;
					n++;
			}
		}
*/		


		if (lc % 1000 == 0){
			//printf("loaded %d ways.\r", lc);
		}

	}
	fclose(fp);
	//printf("loaded %d ways.\n", lc);
	
	free(field);
	free(line_buff);

}

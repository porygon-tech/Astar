#include <stdio.h>
#include <stdlib.h>
#include <math.h>	
#include <string.h>
#include "lib_Astar.h"


//gcc csvreader.c -o csvread -lm && time ./csvread preprocessing/parsedfiles/nodes_clean.csv preprocessing/parsedfiles/ways_clean.csv

int main(int argc, char *argv[]){
	unsigned long n_nodes = 23895681UL;
	ssize_t line_chars = 79857;
	size_t line_bytes  = line_chars * 4;
	size_t field_bytes = 184;


	char *filename_nodes = argv[1];
	char *filename_ways = argv[2];
	char *line_buff, *field;
	FILE *fp;

	int lc = 0;
	

	if((line_buff = (char *)malloc(sizeof(char) * line_bytes )) == NULL){
		perror("Unable to allocate buffer");
		exit(1);
    }

   	if((field    = (char *)malloc(sizeof(char) * field_bytes)) == NULL){
		perror("Unable to allocate buffer");
		exit(1);
    }

	nodetype *nodes;
	nodes = (nodetype *)malloc(sizeof(nodetype) * n_nodes);


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

	while (( line_chars = getline(&line_buff, &line_bytes, fp)) != -1) {
		lc++;

		//printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", lc, line_chars, line_bytes, line_buff);
		field = strsep(&line_buff, "|");
			nodes[lc-1].id = strtoul(field, NULL, 10);


		field = strsep(&line_buff, "|");
			nodes[lc-1].name = (char *) malloc(sizeof(char) * (strlen(field) + 1));
			strcpy(nodes[lc-1].name, field);

		field = strsep(&line_buff, "|");
			nodes[lc-1].lat = atof(field);


		field = strsep(&line_buff, "|");
			nodes[lc-1].lon = atof(field);


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

	while (( line_chars = getline(&line_buff, &line_bytes, fp)) != -1) {
		lc++;

		//printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", lc, line_chars, line_bytes, line_buff);
		field = strsep(&line_buff, "|");



		if (lc % 1000 == 0){
			printf("loaded %d ways.\r", lc);
		}

	}
	fclose(fp);
	printf("loaded %d ways.\n", lc);
	


}

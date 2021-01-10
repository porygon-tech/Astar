#include <stdio.h>
#include <stdlib.h>
#include <math.h>	
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "lib_Astar.h"


//DEBUGGING:  gcc csvreader.c -o csvread -lm -fsanitize=address -static-libasan -g -Wall && time ./csvread preprocessing/parsedfiles/nodes_clean.csv preprocessing/parsedfiles/ways_clean.csv bin_out.bin
//gcc csvreader.c -o csvread -lm -fsanitize=address -static-libasan -g -Wall && time ./csvread preprocessing/parsedfiles/spain_nodes_clean.csv preprocessing/parsedfiles/spain_ways_clean.csv bin_out.bin
//gcc csvreader.c -o csvread -O3 -lm && ./csvread preprocessing/parsedfiles/nodes_clean.csv preprocessing/parsedfiles/ways_clean.csv bin_out.bin

int main(int argc, char *argv[]){
	unsigned long n_nodes = 23895681UL; //change: cataluna 3472620UL; spain 23895681UL
	unsigned long waynode_id, A_id, B_id;
	bool oneway;
	unsigned short len_way;

	ssize_t line_chars = 79857;
	size_t line_bytes  = line_chars;
	size_t field_bytes = 184;


	char *filename_nodes = argv[1];
	char *filename_ways = argv[2];
	char *filename_bin = argv[3];
	char *line_buff, *field, *linec;

	nodetype *nodes;
	unsigned long *way_nodes;

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

   	if((way_nodes     = (unsigned long *)malloc(sizeof(unsigned long) * 5306 )) == NULL){
		perror("Unable to allocate buffer");
		exit(1);
    }


/*
	cat cataluna.csv | grep ^node | wc -l
	cat cataluna.csv | grep ^way  | wc -l
*/


//====== LOAD NODES ==============================================================

	fp = fopen(filename_nodes, "r");

	printf("Reading nodes from %s\n", filename_nodes);
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
	printf("loaded %d nodes.\n\n", lc);



// INIT NODE LIST VARS
	for (int i = 0; i < n_nodes; ++i){
		nodes[i].nsucc = 0;
		nodes[i].successors = (unsigned long *) malloc(sizeof(unsigned long) * 16);
	}



//====== LOAD WAYS ==============================================================

	fp = fopen(filename_ways, "r");

	printf("Reading ways from %s\n", filename_ways);
	if (!fp){
		fprintf(stderr, "Error opening file '%s'\n", filename_ways);
		return EXIT_FAILURE;
	}
	
	
	lc = 0;
	while (( line_chars = getline(&line_buff, &line_bytes, fp)) != -1) {
		linec = line_buff;
		lc++;
		//printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", lc, line_chars, line_bytes, line_buff);
		//printf("line[%06d]: chars=%06zd, buf size=%06zu\n"			, lc, line_chars, line_bytes);
		//printf("%p \n",(void*)&fp);

		field = strsep(&linec, "|");
		field = strsep(&linec, "|");
			if (strlen(field)) {oneway = true;} else {oneway = false;}

		len_way = 0;
		while( (field = strsep(&linec, "|")) != NULL) {
			waynode_id = strtoul(field, NULL, 10);
			
			if (binSearchNode(waynode_id, nodes, n_nodes) != ULONG_MAX) {
					way_nodes[len_way] = waynode_id;
					len_way++;

			}
			
		}
		//printf("line[%06d]: %d\n", lc, len_way);

		if (len_way > 1) {

			for (int i = 0; i < len_way - 1; ++i) {
				A_id = binSearchNode(way_nodes[i],   nodes, n_nodes);	// current node's index in nodelist
				B_id = binSearchNode(way_nodes[i+1], nodes, n_nodes);	//    next node's index in nodelist
				//printf("%d, %d, %d\n", A_id, nodes[A_id].nsucc, B_id);
					
					nodes[A_id].successors[nodes[A_id].nsucc] = B_id;
					nodes[A_id].nsucc++;


				if (!oneway) {
					
					nodes[B_id].successors[nodes[B_id].nsucc] = A_id;
					nodes[B_id].nsucc++;

				}
			}
		}	

		if (lc % 100 == 0) {
			printf("loaded %d ways.\r", lc);
		}

	}

	fclose(fp);
	printf("loaded %d ways.\n\n", lc);

/*
unsigned long nsucc_sum = 0;

	for (int i = 0; i < n_nodes; ++i){
		//printf("%d: %d\n", i, nodes[i].nsucc);
		nsucc_sum = nsucc_sum + nodes[i].nsucc;
	}
printf("\naverage nsucc = %lu/%lu\n", nsucc_sum, n_nodes);
*/





/* HAVERSINE WORKING EXAMPLE
double d = haversine(240949599, 30558454, nodes, n_nodes);
	printf("dist: %.1f km (%.1f mi.)\n", d, d / 1.609344);
*/

//====== WRITE BINARY FILE ==============================================================


	FILE *fin;
	
	// Computing the total number of successors
	unsigned long ntotnsucc=0UL;
	for(int i=0; i<n_nodes; i++) ntotnsucc += nodes[i].nsucc;
	
	// Setting the name of the binary file
	// strcpy(strrchr(filename_bin, '.' ), ".bin");
	printf("Writing graph to %s\n...", filename_bin);

	if ((fin = fopen (filename_bin, "wb")) == NULL)
		ExitError("the output binary data file cannot be opened", 31);
		
	// Global data −−− header
	if( fwrite(&n_nodes, sizeof(unsigned long), 1, fin) + fwrite(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 )
		ExitError("when initializing the output binary data file", 32);
		
	// Writing all nodes
	if( fwrite(nodes, sizeof(nodetype), n_nodes, fin) != n_nodes )
		ExitError("when writing nodes to the output binary data file", 32);
		
	// Writing successors in blocks
	for(int i=0; i < n_nodes; i++) if(nodes[i].nsucc) {
		if( fwrite(nodes[i].successors, sizeof(unsigned long), nodes[i].nsucc, fin) != nodes[i].nsucc )
		ExitError("when writing edges to the output binary data file", 32);
	}
	fclose(fin);
	printf("Done\n");

	for (int i = 0; i < n_nodes; ++i){
		free(nodes[i].name);
		free(nodes[i].successors);
	}

	free(line_buff);
	free(field);
	free(nodes);
	free(way_nodes);
}

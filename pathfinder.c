#include "lib_Astar.h"

//gcc pathfinder.c -o pathfind -Ofast -lm -fsanitize=address -static-libasan -g -Wall && time ./pathfind bin_out.bin

int main (int argc, char* argv[]){
	FILE *fin;
	unsigned long n_nodes;
	unsigned long ntotnsucc;
	unsigned long *allsuccessors;
	nodetype *nodes;
	

//====== LOAD BINARY FILE ==============================================================
        
	if ((fin = fopen (argv[1], "r")) == NULL)
           	ExitError("The data files does not exist or cannot be opened", 11);
		
	//Global data --- header
	if (fread(&n_nodes, sizeof(unsigned long), 1, fin) +
	    fread(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2)
			ExitError("When reading the header of the binary data file", 12);
			
	//Getting memory for all data
	if ((nodes = (nodetype *) malloc(n_nodes*sizeof(nodetype))) == NULL)
		ExitError("When allocating memory for the nodes vector", 13); 
		
	if ((allsuccessors = (unsigned long *) malloc(ntotnsucc*sizeof(unsigned long))) == NULL)
		ExitError("When allocating memory for the edges vector", 15);
		
	//Reading all data from file	
	if(fread(nodes, sizeof(nodetype), n_nodes, fin) != n_nodes)
		ExitError("When reading nodes from the binary file", 17);
	if (fread(allsuccessors, sizeof(unsigned long), ntotnsucc, fin) != ntotnsucc)
	   ExitError("When reading successors from the binary file", 18);

	fclose(fin);

	//Setting pointers to successors
	for (int i=0; i<n_nodes; i++) if(nodes[i].nsucc){
		nodes[i].successors = allsuccessors; 
		allsuccessors += nodes[i].nsucc;
	}
        
//====== A-STAR QUERY ==============================================================

//ejemplo para demostrar que se cargan bien los datos
	double d = haversine(240949599, 195977239, nodes, n_nodes);
	printf("dist: %.1f km (%.1f mi.)\n", d, d / 1.609344);
	
// Choose the starting and destination nodes
        unsigned long node_start;
        unsigned long node_goal; 
        
        printf("Enter the ID of the starting node (ex. Santa Maria del Mar: 240949599): ");
        if(scanf("%lu", &node_start) != 1) ExitError("Invalid start node", 19);
        
        printf("Enter the ID of the goal node (ex. Giralda: 195977239): ");
        if(scanf("%lu", &node_goal) != 1) ExitError("Invalid goal node", 20);
        
        AStar_alg (node_start, node_goal, nodes, n_nodes);
        
        free(nodes);
        free(allsuccessors);
}











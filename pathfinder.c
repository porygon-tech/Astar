#include "lib_Astar.h"

//gcc pathfinder.c -o pathfind -Ofast -lm -fsanitize=address -static-libasan -g -Wall && time ./pathfind bin_out.bin

int main (int argc, char *argv[]){
	FILE *fin;
	unsigned long n_nodes, ntotnsucc;
	unsigned long *allsuccessors;
	nodetype *nodes;
	

//====== LOAD BINARY FILE ==============================================================

	if(( fin = fopen (argv[1], "rb")) == NULL)
		ExitError("The data files does not exist or cannot be opened", 11);
		
	//Global data --- header
	if(fread(&n_nodes, sizeof(unsigned long), 1, fin) +
	   fread(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2)
			ExitError("When reading the header of the binary data file", 12);
			
	//Getting memory for all data
	if((nodes = (nodetype *) malloc(n_nodes*sizeof(nodetype))) == NULL)
		ExitError("When allocating memory for the nodes vector", 13); 
		
	if((allsuccessors = (unsigned long *) malloc(ntotnsucc*sizeof(unsigned long))) == NULL)
		ExitError("When allocating memory for the edges vector", 15);
		
	//Reading all data from file
/*
	if(fread(nodes, sizeof(nodetype), n_nodes, fin) != n_nodes)
		ExitError("When reading nodes from the binary file", 17);
	
	if(fread(allsuccessors, sizeof(unsigned long), ntotnsucc, fin) != ntotnsucc)
		ExitError("When reading successors from the binary file", 18);
*/
	fread(nodes, sizeof(nodetype), n_nodes, fin);
	fread(allsuccessors, sizeof(unsigned long), ntotnsucc, fin);


	fclose(fin);
	
	//Setting pointers to successors
	for(int i=0; i<n_nodes; i++) if(nodes[i].nsucc) {
		nodes[i].successors = allsuccessors; 
		allsuccessors += nodes[i].nsucc;
	}


//====== A-STAR QUERY ==============================================================
//ejemplo para demostrar que se cargan bien los datos
	double d = haversine(240949599, 30558454, nodes, n_nodes);
	printf("dist: %.1f km (%.1f mi.)\n", d, d / 1.609344);
	
// Choose the starting and destination nodes
        unsigned long node_start;
        unsigned long node_goal; 
        
        printf("Enter the ID of the starting node (ex. Santa Maria del Mar: 240949599): ");
        if(scanf("%lu", &node_start) != 1) ExitError("Invalid start node", 19);
        
        printf("Enter the ID of the goal node (ex. Giralda: 195977239, cat: 30558454): ");
        if(scanf("%lu", &node_goal) != 1) ExitError("Invalid goal node", 20);
        
        AStar_alg (240949599, 30558454, nodes, n_nodes);
        /*
        // Search the index of the nodes
         unsigned long start_index = binSearchNode(node_start, nodes, n_nodes);
         unsigned long goal_index = binSearchNode(node_goal, nodes, n_nodes);
   
      // To check the nodes are the same as the introduced ones
         printf("\nThe start node is: id %lu with lat %.4f and long %.4f;\n", nodes[start_index].id, nodes[start_index].lat, nodes[start_index].lon);
         printf("and the goal node is: id %lu with lat %.4f and long %.4f.\n", nodes[goal_index].id, nodes[goal_index].lat, nodes[goal_index].lon);
    
     // Initialize the status
        AStarStatus *status = NULL;
        status = (AStarStatus *) malloc(sizeof(AStarStatus)*n_nodes);
        
        status[start_index].g = 0;
        status[start_index].h = haversine(node_start, node_goal, nodes, n_nodes);
        status[start_index].whq = 1; //OPEN
   
   
        printf("Dijkstra distance from the start node is %.4f and the heuristic distance is %.4f km", status[start_index].g, status[start_index].h);
        
        // Initialize OPEN list
        open_node *OPEN_LIST = NULL;
    
        insert_info (&OPEN_LIST, status[start_index].h, start_index); //g=0; f=g+h; f=h
        
        AStar_alg (start_index, goal_index, nodes, n_nodes, status, OPEN_LIST);
        
        
        // Check if the choosen nodes are in the dataset (FALTA)!!!!
        if(scanf("%lu", &node_start) != 1) ExitError("when reading the starting node", 19);
        while ( binSearchNode(node_start, nodes, n_nodes) = -1) {
           printf("Could not find the "%lu" node");     
        
        
        
        
        free(nodes);
        free(allsuccessors);
        */
}











#include <stdio.h>
#include <stdlib.h>
#include <math.h>	
#include <string.h>
#include <stdbool.h>
#include <limits.h>

const int R = 6371;
const float DEG_TO_RAD = (3.1415926536 / 180);

typedef struct {
	unsigned long id; 		// Node identifier(accessed with node.id)
	char *name;
	double lat, lon;		// Node position (accessed with node.lat, node.lon)
	unsigned short nsucc;   	// Number of node successors; i. e. length of successors
	unsigned long *successors;     // List of successors
} nodetype;


void ExitError( const char *miss, int errcode) {
     fprintf (stderr, " \nERROR: %s. \nStopping... \n\n", miss); exit(errcode);
}

unsigned long binSearchNode (unsigned long id_s, nodetype *nodes, unsigned long listlength) {
	register unsigned long id, iStart = 0UL, half, iEnd = listlength;
	while (iStart < iEnd) {
		//half = iStart + ((iEnd - iStart - 1)>>1);
		half = iStart + round(0.5 * (iEnd - iStart - 1));
		id = nodes[half].id;

		//printf("looking for %lu at %lu, (%lu, %lu): Found %lu\n", id_s, half, iStart, iEnd, id);
		
		if (id > id_s) {
			iEnd = half;
			//puts("vvv");
		} else if (id < id_s) {
			iStart = half + 1;
			//puts("^^^");
		} else {
			//printf("FOUND %lu at %lu \n\n\n", id_s, half);
			return(half);

		}
	}
	//printf("COULD NOT FIND %lu\n\n\n", id_s);
	return(ULONG_MAX);
}


float haversine(unsigned long a_id, unsigned long b_id, nodetype *nodelist, unsigned long n_nodes){
	/*Haversine distance calculator
	IN: two points a,b on earth surface in form of their coordinate values (a_LAT, a_LON, b_LAT, b_LON)
	OUT: haversine distance */

	nodetype a = nodelist[binSearchNode(a_id, nodelist, n_nodes)];
	nodetype b = nodelist[binSearchNode(b_id, nodelist, n_nodes)];

	float a_LAT = a.lat;
	float a_LON = a.lon;
	float b_LAT = b.lat;
	float b_LON = b.lon;


	double dx, dy, dz;
	a_LAT *= DEG_TO_RAD, a_LON *= DEG_TO_RAD, b_LAT *= DEG_TO_RAD, b_LON *= DEG_TO_RAD, 
 	
	dz = sin(a_LAT) - sin(b_LAT);
	dx = cos(a_LAT) * sin(a_LON) - cos(b_LAT) * sin(b_LON);
	dy = cos(a_LAT) * cos(a_LON) - cos(b_LAT) * cos(b_LON);

	return asin(sqrt(dx * dx + dy * dy + dz * dz) *0.5) * 2 * R;
}

//Memory models and queues
typedef char Queue;
enum whichQueue {NONE, OPEN, CLOSED}; 

typedef struct {
   double g, h;            
   unsigned long parent;   
   Queue whq;              
} AStarStatus;

typedef struct open_node{
   double f;                
   unsigned long index;     
   struct open_node* next;  
} open_node;


//====== Astar function ==============================================================

void Astar_alg (unsigned long node_start, unsigned long node_goal, nodetype *nodes, unsigned long n_nodes) {
   
   // Search the index of the nodes
    unsigned long start_index = binSearchNode(node_start, nodes, n_nodes);
    unsigned long goal_index = binSearchNode(node_goal, nodes, n_nodes);
   
   // To check the nodes are the same as the introduced ones
    printf("\nThe start node is: id %lu with lat %.4f and long %.4f;\n", nodes[start_index].id, nodes[start_index].lat, nodes[start_index].lon);
    printf("and the goal node is: id %lu with lat %.4f and long %.4f.\n", nodes[goal_index].id, nodes[goal_index].lat, nodes[goal_index].lon);
    
   // Initialize the status
    AStarStatus* status = NULL;
    
    
    if ((status = (AStarStatus*) malloc(n_nodes*sizeof(AStarStatus))) == NULL)
       ExitError("when allocate memory for the status", 21);
    
    
    unsigned long i;
   // Store for all nodes queue NONE and g to infinity
   
    for(i=0; i<n_nodes; i++){
       status[i].whq = 0;
       status[i].g = INFINITY;
    }
   
   /* Initialization of node_start:
      dijkstra distance g() = 0
      heuristic distance h() = haversine
      and OPEN (1) queue*/
   
    status[start_index].g = 0;
    status[start_index].h = haversine(node_start, node_goal, nodes, n_nodes);
    status[start_index].whq = 1; //OPEN
   
   
    printf("Dijkstra distance from the start node is %.4f and the heuristic distance is %.4f km", status[start_index].g, status[start_index].h);
   
   // Initialize OPEN list
    struct open_node* OPEN = NULL;
   /*
    if ((OPEN = (open_node*) malloc(sizeof(open_node))) == NULL)
    ExitError("when allocate memory for the OPEN list", 24);
   */
    OPEN -> index = start_index;
    OPEN -> f = status[start_index].g + status[start_index].h;
    OPEN -> next = NULL;
    
    
    unsigned long current_index;   // node_current index
    double successor_current_cost; // cost of the successors
    unsigned long successor_index; // node_successor index
    int count_exp = 0;
    unsigned long expanded;
    double w;
  
    
    while (OPEN != NULL) {
      current_index = OPEN -> index;
      count_exp += 1;
      
     //  If node is the goal node break;
      if (current_index == goal_index) break;
      
     // Generate each state of the successors that come after the current node
      for (expanded = 0; expanded < nodes[current_index].nsucc; expanded++){
      
        // Set the cost of the current successor 
         successor_index = (nodes[current_index].successors)[expanded];
         w = haversine(nodes[current_index].id, node_goal, nodes, n_nodes);
         successor_current_cost = status[current_index].g + w;
         
        // If node successor is in the OPEN list
         if (status[successor_index].whq == 1){
             if (status[successor_index].g <= successor_current_cost) continue;
             else {
                open_node* TEMPORARY = OPEN;
                open_node* PREVIOUS = NULL;
               
                while((TEMPORARY != NULL) && (TEMPORARY -> index != successor_index)){
                   PREVIOUS = TEMPORARY;
                   TEMPORARY = TEMPORARY -> next;
                }
               
                PREVIOUS -> next = TEMPORARY -> next;
                free(TEMPORARY);
             }
        // If node successor is in the CLOSE list  
         } else if (status[successor_index].whq == 2 ) continue;
      
         else status[successor_index].h = haversine(nodes[successor_index].id, node_goal, nodes, n_nodes);
      
         status[successor_index].g = successor_current_cost;
         status[successor_index].parent = current_index;
      
         (status + successor_index) -> whq = 1; //OPEN
         open_node* TEMPORARY = OPEN;
         open_node* new = NULL;
         
         if ((new = (open_node*) malloc(sizeof(open_node))) == NULL)
            ExitError("when allocate memory for a new node in the OPEN list", 23);
            
         new -> index = successor_index;
         new -> f = status[successor_index].g + status[successor_index].h;
         new -> next = NULL;
         
         while ((TEMPORARY -> next != NULL) && ((TEMPORARY -> next) -> f)){
            TEMPORARY = TEMPORARY -> next;
         }
         
         if ((TEMPORARY -> next) -> f > new -> f){
            new -> next = TEMPORARY -> next;
            TEMPORARY -> next = new;
         }
      
      }
      
      status[current_index].whq = 2; //CLOSE
      
      struct open_node* COPY = NULL;
      COPY = OPEN -> next; 
      free(OPEN);
      OPEN = COPY;  
     
    }
    
    if (OPEN == NULL) ExitError("OPEN list is empty", 24);
    
    
    
    struct open_node* COPY = NULL;
    while (OPEN != NULL){
       COPY = OPEN -> next; 
       free(OPEN);
       OPEN = COPY; 
    }
    
   // We count the number of nodes 
    current_index = goal_index;
    unsigned long length_path = 0;
    while(current_index != start_index){
       length_path += 1;
       current_index = status[current_index].parent;
    }
    
   // We store the path 
    unsigned long *path = NULL;
    if ((path = (unsigned long*) malloc(length_path*sizeof(unsigned long))) == NULL)
       ExitError("when allocate memory for the path", 25);
       
    path += length_path -1;
    current_index = goal_index;
    
    while (current_index != start_index){
       *path = current_index;
       current_index = status[current_index].parent;
       path -=1;
    }
    
    if (current_index == goal_index) *path = current_index;
    
    // And we write it into a csv (FALTA)
    
     
    printf("The solution length is %.4f km.\n", status[goal_index].g);
    printf("The number of expanded nodes is: %u.\n", count_exp);
    
    free(path); 
    free(status);
    
   
    
} 








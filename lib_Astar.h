#include <stdio.h>
#include <stdlib.h>
#include <math.h>	
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

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

typedef struct dynamic_node {
   double f;                
   unsigned long index;     
   struct dynamic_node *next;  
} open_node;

void insert_info (open_node **n, double f, unsigned long index){
    open_node *nnew;
    nnew = malloc(sizeof(open_node));
    
    nnew -> f = f;
    nnew -> index = index;
    nnew -> next = *n;
    *n = nnew;
}

unsigned long min_index (open_node *n){
   open_node *ACTUAL = n;
   if (ACTUAL == NULL) return ULONG_MAX;
   open_node *MINI = NULL;
   double num_min = 999999999999;
   
   while (ACTUAL != NULL) {
      if (ACTUAL -> f < num_min) {
         MINI = ACTUAL;
         num_min = ACTUAL -> f;
      }
      
      ACTUAL = ACTUAL -> next;
   }
   return MINI -> index;
}

int iter_by_index (open_node **n, unsigned long index){
    open_node *ACTUAL = *n;
    if(ACTUAL == NULL) return -1;
    
    open_node *TEMPORARY = NULL;
    if (index == ACTUAL -> index){
       open_node *COPY = NULL;
       
       COPY = (*n) -> next;
       free(*n);
       *n = COPY;
       return 1;
    }
    
    while (ACTUAL -> next -> index != index) {
          ACTUAL = ACTUAL -> next;
          if (ACTUAL == NULL) return -1;      
    }
    
    TEMPORARY = ACTUAL -> next;
    ACTUAL -> next = TEMPORARY -> next;
    free(TEMPORARY);
    return 1;
}


//====== AStar function ==============================================================

void AStar_alg (unsigned long node_start, unsigned long node_goal, nodetype *nodes, unsigned long n_nodes)
{
   
   // Search the index of the nodes
    unsigned long start_index = binSearchNode(node_start, nodes, n_nodes);
    unsigned long goal_index = binSearchNode(node_goal, nodes, n_nodes);
   
   // To check the nodes are the same as the introduced ones
    printf("\nThe start node is: id %lu with lat %.4f and long %.4f;\n", nodes[start_index].id, nodes[start_index].lat, nodes[start_index].lon);
    printf("and the goal node is: id %lu with lat %.4f and long %.4f.\n", nodes[goal_index].id, nodes[goal_index].lat, nodes[goal_index].lon);
    
   // Initialize the status
    AStarStatus *status = NULL;
    status = (AStarStatus *) malloc(n_nodes*sizeof(AStarStatus));
   
   /*Initialization of node_start:
      dijkstra distance g() = 0
      heuristic distance h() = haversine
      and queue 1 */
   
    status[start_index].g = 0;
    status[start_index].h = haversine(node_start, node_goal, nodes, n_nodes);
    status[start_index].whq = 1; //OPEN
    
   // Check status
    printf("Dijkstra distance from the start node is %.4f and the heuristic distance is %.4f km.\n",
            status[start_index].g, status[start_index].h);
   
   // Initialize OPEN list
    open_node *OPEN_LIST = NULL;
    insert_info(&OPEN_LIST, (status[start_index].g + status[start_index].h), start_index); //g=0; g+h=h
    
    // printf("OPEN LIST: (%ld) index, (%f) f, (%ld) next\n.", OPEN_LIST -> index, OPEN_LIST -> f, OPEN_LIST -> next);
   
   // Vars   
    unsigned long current_index;   // node_current index
    double successor_current_cost; // cost of the successors
    int count_expanded = 0;
    double w;
    
   // Start loop timer;
    clock_t start, end;
    start = clock();
    
    while ((current_index = min_index(OPEN_LIST)) != ULONG_MAX) {
      
      count_expanded += 1;
      // printf("Current index: %ld.\nAnd number of expanded nodes: %d.\n", current_index, count_expanded);
      
      if (current_index == goal_index) {
         printf("The goal node has been found.\n");
         break;
      }
      
      int i;
      for (i=0; i < nodes[current_index].nsucc; i++){
      
          unsigned long successor_index = nodes[current_index].successors[i];
          w = haversine(nodes[current_index].id, nodes[goal_index].id, nodes, n_nodes);
          successor_current_cost = status[current_index].g + w;
          
          if (status[successor_index].whq == 1){
          
             if (status[successor_index].g <= successor_current_cost) continue;
             
          } else if (status[successor_index].whq == 2){

             if (status[successor_index].g <= successor_current_cost) continue;
             
             status[successor_index].whq = 1;
             insert_info (&OPEN_LIST, (successor_current_cost + status[successor_index].h), successor_index);
       
          } else {
             status[successor_index].h = haversine(nodes[successor_index].id, nodes[goal_index].id, nodes, n_nodes);
             status[successor_index].whq = 1;
             insert_info (&OPEN_LIST, (successor_current_cost + status[successor_index].h), successor_index);
             
          }
          
          status[successor_index].g = successor_current_cost;
          status[successor_index].parent = current_index;
      }
      
      status[current_index].whq = 2;
      
     // iter_by_index
     if ((iter_by_index(&OPEN_LIST, current_index)) != 1) printf("Delete failed.\n");      

    }
    // Finish loop timer
    end = clock();
    printf("A* has taken %.4fs.\n", ((double) (end-start))/CLOCKS_PER_SEC);
    
    if (current_index != goal_index) printf("OPEN list is empty.\n");
    
    // Write the path
    
    unsigned long *path;
    unsigned long neigh;
    
    neigh = current_index;
    path = (unsigned long *) malloc(n_nodes*sizeof(unsigned long));
    path[0] = neigh;
    unsigned long nn = 0;
    while(neigh != start_index){
       nn++;
       neigh = status[neigh].parent;
       path[nn] = neigh;
    }
    
    unsigned long length_path = nn+1;
    
    printf("The found path has %ld nodes (including the start and goal node).\n", length_path);
    
    FILE *res = fopen("final_path.csv", "w");
    fprintf(res, "Node id | Latitud | Longitud\n");
    int i;
    for (i=length_path-1; i >= 0 ; i--){
       fprintf(res, "%lu | %.4f | %.4f \n",
               nodes[path[i]].id, nodes[path[i]].lat, nodes[path[i]].lon);
    }
    fclose(res);
    
    free(path); 
    free(status); 
 
} 








#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>	

const int R = 6371;
const float DEG_TO_RAD = (3.1415926536 / 180);


// Exit Error
void ExitError( const char *miss, int errcode) {
     fprintf (stderr, " \nERROR: %s. \nStopping... \n\n", miss); exit(errcode);
}

// Structure to represent the nodes
typedef struct {
	unsigned long id; 		// Node identifier(accessed with node.id)
	char *name;
	double lat, lon;		// Node position (accessed with node.lat, node.lon)
	unsigned short nsucc;	// Number of node successors; i. e. length of successors
	unsigned long *successors;
} nodetype;


float haversine(float a_LAT,float a_LON,float b_LAT,float b_LON){
	/*Haversine distance calculator
	IN: two points a,b on earth surface in form of their coordinate values (a_LAT, a_LON, b_LAT, b_LON)
	OUT: haversine distance */

	double dx, dy, dz;
	a_LAT *= DEG_TO_RAD, a_LON *= DEG_TO_RAD, b_LAT *= DEG_TO_RAD, b_LON *= DEG_TO_RAD, 
 	
	dz = sin(a_LAT) - sin(b_LAT);
	dx = cos(a_LAT) * sin(a_LON) - cos(b_LAT) * sin(b_LON);
	dy = cos(a_LAT) * cos(a_LON) - cos(b_LAT) * cos(b_LON);

	return asin(sqrt(dx * dx + dy * dy + dz * dz) *0.5) * 2 * R;
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

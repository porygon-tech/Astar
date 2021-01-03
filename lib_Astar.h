#include <stdio.h>
#include <stdlib.h>
#include <math.h>	

const int R = 6371;
const float DEG_TO_RAD = (3.1415926536 / 180);

typedef struct {
	unsigned long id; 		// Node identificator (accessed with node.id)
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


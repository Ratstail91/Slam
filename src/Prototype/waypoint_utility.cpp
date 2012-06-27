/* File Name: waypoint_utility.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#include <math.h>
#include "waypoint_utility.h"

//-------------------------
//Preprocessor directives
//-------------------------

#define sqr(x) ((x)*(x))

//-------------------------
//Public access members
//-------------------------

const int WaypointUtility::count = 6;

const int WaypointUtility::location[6][2] = {
	144,13,
	595,23,
	133,382,
	643,398,
	102,545,
	676,560
};

const int WaypointUtility::solution[6][6] = {
	0,1,2,1,2,1,
	0,1,0,3,0,3,
	0,0,2,3,4,3,
	1,1,2,3,2,5,
	2,2,2,2,4,5,
	3,3,3,3,4,5
};

float WaypointUtility::Distance(float x1, float y1, float x2, float y2) {
	return sqrt( sqr(x1 - x2) + sqr(y1 - y2) );
}

int WaypointUtility::Nearest(float x, float y) {
	//find the nearest node
	int nearest = 0;
	float dist = Distance(x, y, (float)location[0][0], (float)location[0][1]);

	for (int n = 1; n < count; n++) {
		float newdist = Distance(x, y, (float)location[n][0], (float)location[n][1]);

		if (dist > newdist) {
			dist = newdist;
			nearest = n;
		}
	}

	return nearest;
}

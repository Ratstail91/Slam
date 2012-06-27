/* File Name: waypoint_utility.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#ifndef KR_WAYPOINTUTILITY_H_
#define KR_WAYPOINTUTILITY_H_

class WaypointUtility {
public:
	/* Public access members */
	static const int count;
	static const int location[6][2];
	static const int solution[6][6];

	static float Distance(float x1, float y1, float x2, float y2);
	static int Nearest(float x, float y);
};

#endif

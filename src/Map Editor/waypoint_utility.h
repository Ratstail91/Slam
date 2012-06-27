/* File Name: waypoint_utility.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#ifndef KR_WAYPOINTUTILITY_H_
#define KR_WAYPOINTUTILITY_H_

#include <string>
using std::string;

class WaypointUtility {
public:
	/* Public access members */
	WaypointUtility();

	void Load(string sFileName, string sPathDir);
	void Clear();

	float Distance(float x1, float y1, float x2, float y2);
	int Nearest(float x, float y);

	/* Accessors and mutators */
	int* GetNode	(int);
	int GetNext		(float sx, float sy, float dx, float dy);
	int GetNextX	(float sx, float sy, float dx, float dy); //slower
	int GetNextY	(float sx, float sy, float dx, float dy); //slower

	int GetNodeX	(int);
	int GetNodeY	(int);

	int** GetNodeList();
	int** GetSolutionTable();

private:
	/* Private access members */
	void GenTables();
	void ClrTables();

	int m_iCount;
	int** m_aiLocation;
	int** m_aiSolution;
};

#endif

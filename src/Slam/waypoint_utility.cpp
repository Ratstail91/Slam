/* File Name: waypoint_utility.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#include <exception>
#include <math.h>
#include <fstream>
#include "waypoint_utility.h"
using namespace std;

//-------------------------
//Preprocessor directives
//-------------------------

#define sqr(x) ((x)*(x))
#define dsqr(x1, y1, x2, y2) (sqr((x1) - (x2)) + sqr((y1) - (y2))) //for optimisation in Distance() and Nearest()

//-------------------------
//Public access members
//-------------------------

WaypointUtility::WaypointUtility() {
	m_iCount = 0;
	m_aiLocation = NULL;
	m_aiSolution = NULL;
}

void WaypointUtility::Load(string sFileName, string sPathDir) {
	ifstream is( (sPathDir + sFileName).c_str() );

	if (!is.is_open())
		throw(exception("Failed to open the path file for reading"));

	ClrTables();

	//get the count
	is >> m_iCount;

	GenTables();

	//get the node locations
	for (int i = 0; i < m_iCount; i++) {
		is >> m_aiLocation[i][0];
		is >> m_aiLocation[i][1];
	}

	//get the solution table
	for (int i = 0; i < m_iCount; i++) {
		for (int j = 0; j < m_iCount; j++) {
			is >> m_aiSolution[i][j];
		}
	}

	is.close();
}

void WaypointUtility::Clear() {
	ClrTables();
}

float WaypointUtility::Distance(float x1, float y1, float x2, float y2) {
	return sqrt( dsqr(x1, y1, x2, y2) );
}

int WaypointUtility::Nearest(float x, float y) {
	if (m_iCount == 0) return -1;

	int nearest = 0;

	for (int i = 0; i < m_iCount; i++) {
		if (Distance(x, y, (float)m_aiLocation[nearest][0], (float)m_aiLocation[nearest][0]) > Distance(x, y, (float)m_aiLocation[i][0], (float)m_aiLocation[i][0])) {
			nearest = i;
		}
	}

	return nearest;
}

//-------------------------
//Accessors and mutators
//-------------------------

int* WaypointUtility::GetNode(int i) {
	if (m_aiLocation == NULL) return NULL;

	return m_aiLocation[i];
}

int WaypointUtility::GetNext(float sx, float sy, float dx, float dy) {
	if (m_aiSolution == NULL) return -1;

	int sn = Nearest(sx, sy);
	int dn = Nearest(dx, dy);

	return m_aiSolution[sn][dn];
}

int WaypointUtility::GetNextX(float sx, float sy, float dx, float dy) {
	if (m_aiSolution == NULL) return -1;

	int sn = Nearest(sx, sy);
	int dn = Nearest(dx, dy);

	return m_aiLocation[m_aiSolution[sn][dn]][0];
}

int WaypointUtility::GetNextY(float sx, float sy, float dx, float dy) {
	if (m_aiSolution == NULL) return -1;

	int sn = Nearest(sx, sy);
	int dn = Nearest(dx, dy);

	return m_aiLocation[m_aiSolution[sn][dn]][1];
}

int WaypointUtility::GetNodeX(int i) {
	if (m_aiLocation == NULL) return -1;

	return m_aiLocation[i][0];
}

int WaypointUtility::GetNodeY(int i) {
	if (m_aiLocation == NULL) return -1;

	return m_aiLocation[i][1];
}

int** WaypointUtility::GetNodeList() {
	return m_aiLocation;
}

int** WaypointUtility::GetSolutionTable() {
	return m_aiSolution;
}

//-------------------------
//Private access members
//-------------------------

void WaypointUtility::GenTables() {
	if (m_iCount <= 0 || m_aiLocation != NULL || m_aiSolution != NULL)
		throw(exception("Failed to generate the waypoint tables"));

	m_aiLocation = new int*[m_iCount];

	for (int i = 0; i < m_iCount; i++)
		m_aiLocation[i] = new int[2];

	m_aiSolution = new int*[m_iCount];

	for (int i = 0; i < m_iCount; i++)
		m_aiSolution[i] = new int[m_iCount];
}

void WaypointUtility::ClrTables() {
	if (m_iCount <= 0 || m_aiLocation == NULL || m_aiSolution == NULL)
		return;

	for (int i = 0; i < m_iCount; i++)
		delete[] m_aiLocation[i];

	delete[] m_aiLocation;

	for (int i = 0; i < m_iCount; i++)
		delete[] m_aiSolution[i];

	delete[] m_aiSolution;
}

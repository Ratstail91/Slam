/* File Name: waypoint_tool.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#ifndef KR_WAYPOINTTOOL_H_
#define KR_WAYPOINTTOOL_H_

#include <vector>
#include <string>
#include "SDL.h"
using std::string;

class WaypointTool {
public:
	/* Internal structure definitions */
	struct node { int x, y; };
	typedef std::vector<node*> nodeList;

	struct path { node *a, *b; };
	typedef std::vector<path*> pathList;

	/* Public access members */
	WaypointTool();

	void Load(string sFileName, string sPathDir);
	void Save(string sFileName, string sPathDir);
	void Clear();

	void Draw(SDL_Surface* pDest, int iCamX = 0, int iCamY = 0);

	void Bake(string sFileName, string sPathDir);

	float Distance(float x1, float y1, float x2, float y2);
	node* Nearest(float x, float y);

	/* Accessors and mutators */
	nodeList* GetNodeList();
	pathList* GetPathList();

	void AddNode		(int iMouseX, int iMouseY, int iCamX = 0, int iCamY = 0);
	void DelNearestNode	(int iMouseX, int iMouseY, int iCamX = 0, int iCamY = 0);
	void DelNode		(node* n);

	void AddPath		(node* a, node* b);
	path* GetPath		(node* a, node* b);
	void DelPath		(node* a, node* b);
	void DelPath		(path* p);
	void TogPath		(node* a, node* b);

	void SetSelection	(int iMouseX, int iMouseY, int iCamX = 0, int iCamY = 0);

private:
	/* Private access members */
	int NodeIndex(node*);

	nodeList m_vNodes;
	pathList m_vPaths;

	node* m_pSelection;

	/* Dijkstra members */
	void CreateSolutionTable();
	void GenTables();
	void ClrTables();
	int Backtrack(int i);

	void Dijkstra(int);

	int** m_aiSolution;
	int* m_aiDist;
	int* m_aiPrev;
};

#endif

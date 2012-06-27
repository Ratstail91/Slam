/* File Name: waypoint_tool.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#include <exception>
#include <math.h>
#include <fstream>
#include "SDL_gfxPrimitives.h"
#include "waypoint_tool.h"
using namespace std;

//-------------------------
//Preprocessor directives
//-------------------------

#define sqr(x) ((x)*(x))
#define dsqr(x1, y1, x2, y2) (sqr((x1) - (x2)) + sqr((y1) - (y2))) //for optimisation in Distance() and Nearest()
#define INFINITY 65535

//-------------------------
//Public access members
//-------------------------

WaypointTool::WaypointTool() {
	m_pSelection = NULL;

	m_aiSolution = NULL;
	m_aiDist = NULL;
	m_aiPrev = NULL;
}

void WaypointTool::Load(string sFileName, string sPathDir) {
	//empty the lists
	Clear();

	//open the file
	ifstream is( (sPathDir + sFileName).c_str() );

	if (!is.is_open())
		throw(exception("Failed to open way file for reading"));

	int size;

	//load the size
	is >> size;

	//load the nodes
	node n;
	for (int i = 0; i < size; i++) {
		is >> n.x;
		is >> n.y;

		m_vNodes.push_back(new node(n));
	}

	//load the size
	is >> size;
	
	//load the paths
	path p;
	int a, b;
	for (int i = 0; i < size; i++) {
		is >> a;
		is >> b;

		p.a = m_vNodes[a];
		p.b = m_vNodes[b];

		m_vPaths.push_back(new path(p));
	}

	//close
	is.close();
}

void WaypointTool::Save(string sFileName, string sPathDir) {
	ofstream os( (sPathDir + sFileName).c_str() );

	if (!os.is_open())
		throw(exception("Failed to open way file for writing"));

	//size
	os << m_vNodes.size() << endl;

	//nodes
	for (nodeList::iterator it = m_vNodes.begin(); it != m_vNodes.end(); it++) {
		os << (*it)->x << " " << (*it)->y << endl;
	}

	//size
	os << m_vPaths.size() << endl;

	//paths
	for (pathList::iterator it = m_vPaths.begin(); it != m_vPaths.end(); it++) {
		os << NodeIndex((*it)->a) << " " << NodeIndex((*it)->b) << endl;
	}

	os.close();
}

void WaypointTool::Clear() {
	ClrTables();
	m_vNodes.clear();
	m_vPaths.clear();
}

void WaypointTool::Draw(SDL_Surface* pDest, int iCamX, int iCamY) {
	for (nodeList::iterator it = m_vNodes.begin(); it != m_vNodes.end(); it++) {
		filledCircleRGBA(pDest, (*it)->x + iCamX, (*it)->y + iCamY, 5, 255, 0, 0, 255);
	}

	for (pathList::iterator it = m_vPaths.begin(); it != m_vPaths.end(); it++) {
		lineRGBA(pDest, (*it)->a->x + iCamX, (*it)->a->y + iCamY, (*it)->b->x + iCamX, (*it)->b->y + iCamY, 0, 0, 255, 255);
	}

	if (m_pSelection != NULL) {
		circleRGBA(pDest, m_pSelection->x + iCamX, m_pSelection->y + iCamY, 7, 255, 255, 0, 255);
	}
}

void WaypointTool::Bake(string sFileName, string sPathDir) {
	//open the file
	ofstream os( (sPathDir + sFileName).c_str() );

	if (!os.is_open())
		throw(exception("Failed to open path file for writing"));

	//output the size
	os << m_vNodes.size() << endl;

	//output the node pairs
	for (unsigned int i = 0; i < m_vNodes.size(); i++)
		os << m_vNodes[i]->x << " " << m_vNodes[i]->y << endl;

	//output the solution graph
	GenTables();
	CreateSolutionTable();

	for (unsigned int i = 0; i < m_vNodes.size(); i++) {
		for (unsigned int j = 0; j < m_vNodes.size(); j++) {
			//output solution[src][dest]

			os << m_aiSolution[i][j] << " ";
		}
		os << endl;
	}
	ClrTables();

	//close the file
	os.close();
}

float WaypointTool::Distance(float x1, float y1, float x2, float y2) {
	return sqrt( dsqr(x1, y1, x2, y2) );
}

WaypointTool::node* WaypointTool::Nearest(float x, float y) {
	if (m_vNodes.size() == 0) return NULL;

	nodeList::iterator n = m_vNodes.begin();

	for (nodeList::iterator it = m_vNodes.begin(); it != m_vNodes.end(); it++) {
		if (dsqr(x, y, (*n)->x, (*n)->y) > dsqr(x, y, (*it)->x, (*it)->y)) {
			n = it;
		}
	}

	return *n;
}

//-------------------------
//Accessors and mutators
//-------------------------

WaypointTool::nodeList* WaypointTool::GetNodeList() {
	return &m_vNodes;
}

WaypointTool::pathList* WaypointTool::GetPathList() {
	return &m_vPaths;
}

void WaypointTool::AddNode(int iMouseX, int iMouseY, int iCamX, int iCamY) {
	node n;

	n.x = iMouseX-iCamX;
	n.y = iMouseY-iCamY;

	m_vNodes.push_back(new node(n));
}

void WaypointTool::DelNearestNode(int iMouseX, int iMouseY, int iCamX, int iCamY) {
	//get the nearest node
	node* n = Nearest((float)(iMouseX-iCamX), (float)(iMouseY-iCamY));

	//delete this node
	DelNode(n);
}

void WaypointTool::DelNode(node* n) {
	if (n == NULL) return;

	//remove all paths connected to this node
	for (pathList::iterator it = m_vPaths.begin(); it != m_vPaths.end(); /* Empty */ ) {
		if ((*it)->a == n || (*it)->b == n) { //find a path
			DelPath(*it); //delete the path, invalidate the iterator
			it = m_vPaths.begin(); //reset the iterator
			continue;
		}

		it++;
	}

	//delete the node
	for (nodeList::iterator it = m_vNodes.begin(); it != m_vNodes.end(); it++) {
		if (*it == n) {
			m_vNodes.erase(it);
			delete n;
			return;
		}
	}
}

void WaypointTool::AddPath(node* a, node* b) {
	if (a == NULL || b == NULL) return;

	if (a == b) return; //no path to itself

	//check if there is a path here
	for (pathList::iterator it = m_vPaths.begin(); it != m_vPaths.end(); it++) {
		if (//matching paths
			((*it)->a == a && (*it)->b == b) ||
			((*it)->a == b && (*it)->b == a)
			)
		{
			return; //path already exists
		}
	}

	path p;

	p.a = a;
	p.b = b;

	m_vPaths.push_back(new path(p));
}

WaypointTool::path* WaypointTool::GetPath(node* a, node* b) {
	for (pathList::iterator it = m_vPaths.begin(); it != m_vPaths.end(); it++) {
		if (//matching paths
			((*it)->a == a && (*it)->b == b) ||
			((*it)->a == b && (*it)->b == a)
			)
		{
			return *it;
		}
	}

	return NULL;
}

void WaypointTool::DelPath(node* a, node* b) {
	//find the path
	for (pathList::iterator it = m_vPaths.begin(); it != m_vPaths.end(); it++) {
		if (//matching paths
			((*it)->a == a && (*it)->b == b) ||
			((*it)->a == b && (*it)->b == a)
			)
		{
			DelPath(*it);
			return;
		}
	}
}

void WaypointTool::DelPath(path* p) {
	if (p == NULL) return;

	for (pathList::iterator it = m_vPaths.begin(); it != m_vPaths.end(); it++) {
		if (*it == p) {
			m_vPaths.erase(it);
			delete p;
			return;
		}
	}
}

void WaypointTool::TogPath(node* a, node* b) {
	if (a == NULL || b == NULL) return;

	for (pathList::iterator it = m_vPaths.begin(); it != m_vPaths.end(); it++) {
		if (//matching paths
			((*it)->a == a && (*it)->b == b) ||
			((*it)->a == b && (*it)->b == a)
			)
		{
			DelPath(*it);
			return;
		}
	}

	AddPath(a, b);
}

void WaypointTool::SetSelection(int iMouseX, int iMouseY, int iCamX, int iCamY) {
	node* n = Nearest((float)(iMouseX-iCamX), (float)(iMouseY-iCamY));

	if (n == NULL) return;

	if (m_pSelection == NULL) {
		m_pSelection = n;
		return;
	}

	if (m_pSelection == n) {
		m_pSelection = NULL;
		return;
	}

	TogPath(m_pSelection, n);
	m_pSelection = NULL;
}

//-------------------------
//Private access members
//-------------------------

int WaypointTool::NodeIndex(node* n) {
	for (unsigned int i = 0; i < m_vNodes.size(); i++) {
		if (m_vNodes[i] == n) return i;
	}

	return -1;
}

//-------------------------
//Dijkstra members
//-------------------------

void WaypointTool::CreateSolutionTable() {
	//init
	for (unsigned int i = 0; i < m_vNodes.size(); i++) {
		for (unsigned int j = 0; j < m_vNodes.size(); j++) {
			m_aiSolution[i][j] = -1;
		}
	}

	//for each source node
	for (unsigned int i = 0; i < m_vNodes.size(); i++) {
		Dijkstra(i);

		//for each dest node
		for (unsigned int j = 0; j < m_vNodes.size(); j++) {
			if (i == j) {
				m_aiSolution[i][j] = j; //don't move
			}
			else if (m_aiDist[i] == INFINITY) {
				m_aiSolution[i][j] = -1; //no connection
			}
			else {
				m_aiSolution[i][j] = Backtrack(j);
			}
		}
	}
}

void WaypointTool::GenTables() {
	if (m_aiSolution != NULL || m_aiDist != NULL || m_aiPrev != NULL)
		throw(exception("Failed to generate the waypoint tables"));

	m_aiSolution = new int*[m_vNodes.size()];
	m_aiDist = new int[m_vNodes.size()];
	m_aiPrev = new int[m_vNodes.size()];

	for (unsigned int i = 0; i < m_vNodes.size(); i++) {
		m_aiSolution[i] = new int[m_vNodes.size()];
	}
}

void WaypointTool::ClrTables() {
	if (m_aiSolution != NULL) {
		for (unsigned int i = 0; i < m_vNodes.size(); i++)
			delete[] m_aiSolution[i];

		delete[] m_aiSolution;

		m_aiSolution = NULL;
	}

	if (m_aiDist != NULL) {
		delete[] m_aiDist;

		m_aiDist = NULL;
	}

	if (m_aiPrev != NULL) {
		delete[] m_aiPrev;

		m_aiPrev = NULL;
	}
}

int WaypointTool::Backtrack(int i) {
	if (m_aiPrev[i] == -1)
		return -1;

	if (m_aiPrev[m_aiPrev[i]] != -1)
		return Backtrack(m_aiPrev[i]);

	return i;
}

void SetOpen(bool* b, int size) {
	for (int i = 0; i < size; i++) {
		b[i] = true;
	}
}

bool IsOpen(bool* b, int size) {
	for (int i = 0; i < size; i++) {
		if (b[i]) return true;
	}
	return false;
}

void WaypointTool::Dijkstra(int source) {
	//initial setup
	for (unsigned int i = 0; i < m_vNodes.size(); i++) {
		m_aiDist[i] = INFINITY;
		m_aiPrev[i] = -1; //undefined
	}

	m_aiDist[source] = 0; //from source to source

	bool* Q = new bool[m_vNodes.size()]; //suboptimal open method, only choice

	SetOpen(Q, m_vNodes.size());

	while(IsOpen(Q, m_vNodes.size())) {
//		cout << "\tloop head" << endl;

		//u in Q with the smallest dist[u]
		int u = -1;

		for (unsigned int i = 0; i < m_vNodes.size(); i++) {
			if (Q[i] && (u == -1 || m_aiDist[i] < m_aiDist[u])) u = i;
		}

		//remove u from Q
		Q[u] = false;

//		//debugging...
//		for (unsigned int i = 0; i < m_vNodes.size(); i++) {
//			cout << "Node " << i << ": Q=" << Q[i] << " dist=" << m_aiDist[i] << endl;
//		}

		//error check
		if (m_aiDist[u] == INFINITY) break;

		//for each neighbour v
		for (nodeList::iterator it = m_vNodes.begin(); it != m_vNodes.end(); it++) {
			if (m_vNodes[u] != *it && GetPath(*it, m_vNodes[u]) != NULL) { //not matching && got a path
				int v = NodeIndex(*it);
				//calc alt
				int alt = m_aiDist[u] + (int)Distance( (float)m_vNodes[u]->x, (float)m_vNodes[u]->y, (float)m_vNodes[v]->x, (float)m_vNodes[v]->y);

				//reassign
				if (alt < m_aiDist[v]) {
					m_aiDist[v] = alt;
					m_aiPrev[v] = u;
				}
			}
		}
	}

	delete[] Q;
}
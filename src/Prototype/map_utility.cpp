/* File Name: map_utility.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: A stipped down version of Sketch's map utility.
*/
#include <exception>
#include <fstream>
#include "map_utility.h"
using namespace std;

//-------------------------
//Public access members
//-------------------------

MapUtility::MapUtility() {
	m_pArray = NULL;
}

MapUtility::~MapUtility() {
	Clear();
}

void MapUtility::Load(const char* szFileName) {
	ifstream is(szFileName);

	if (!is.is_open())
		throw(exception("Failed to open file for reading"));

	ClrArray();

	is >> m_iTileXCount;
	is >> m_iTileYCount;
	is >> m_iTileW;
	is >> m_iTileH;

	GenArray();

	for (int j = 0; j < m_iTileYCount; j++) {
		for (int i = 0; i < m_iTileXCount; i++) {
			is >> m_pArray[i][j];
		}
	}

	is.close();
}

void MapUtility::Save(const char* szFileName) {
	if (m_pArray == NULL)
		throw(exception("Failed to save region"));

	ofstream os(szFileName);

	if (!os.is_open())
		throw(exception("Failed to open file for writing"));

	os << m_iTileXCount << ' ';
	os << m_iTileYCount << ' ';
	os << m_iTileW << ' ';
	os << m_iTileH << ' ';
	os << endl;

	for (int j = 0; j < m_iTileYCount; j++) {
		for (int i = 0; i < m_iTileXCount; i++) {
			os << m_pArray[i][j] << ' ';
		}
		os << endl;
	}
}

void MapUtility::New(int x, int y, int w, int h) {
	ClrArray();

	m_iTileXCount	= x;
	m_iTileYCount	= y;
	m_iTileW		= w;
	m_iTileH		= h;

	GenArray();

	for (int j = 0; j < m_iTileYCount; j++) {
		for (int i = 0; i < m_iTileXCount; i++) {
				m_pArray[i][j] = 0;
		}
	}
}

void MapUtility::Clear() {
	ClrArray();
}

//-------------------------
//Accessors and mutators
//-------------------------

int MapUtility::GetT(int x, int y) {
	if (m_pArray == NULL)
		throw(exception("Failed to find the array"));

	if (x < 0 || y < 0 || x >= m_iTileXCount || y >= m_iTileYCount)
		return -1;

	return m_pArray[x][y];
}

int MapUtility::GetX() {
	return m_iTileXCount;
}

int MapUtility::GetY() {
	return m_iTileYCount;
}

int MapUtility::GetW() {
	return m_iTileW;
}

int MapUtility::GetH() {
	return m_iTileH;
}

int** MapUtility::GetArray() {
	return m_pArray;
}

//-------------------------
//Private access members
//-------------------------

void MapUtility::GenArray() {
	if (m_pArray != NULL)
		throw(exception("Failed to generate the array"));

	//create X
	m_pArray = new int*[m_iTileXCount];

	for (int i = 0; i < m_iTileXCount; i++) {

		//create Y
		m_pArray[i] = new int[m_iTileYCount];
	}
}

void MapUtility::ClrArray() {
	if (m_pArray == NULL)
		return;

	for (int i = 0; i < m_iTileXCount; i++) {
		delete[] m_pArray[i];
	}

	delete[] m_pArray;

	m_pArray = NULL;
}

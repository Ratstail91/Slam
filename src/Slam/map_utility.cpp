/* File Name: map_utility.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: A modular map utility.
*/
#include <exception>
#include <fstream>
#include "map_utility.h"
using namespace std;

//-------------------------
//Public access members
//-------------------------

MapUtility::MapUtility() {
	m_iTileX = 0;
	m_iTileY = 0;
	m_iTileW = 0;
	m_iTileH = 0;

	m_pArray = NULL;
	m_pTileset = NULL;
}

MapUtility::~MapUtility() {
	ClrArray();
}

void MapUtility::Load(string sFileName, string sMapDir, string sTilesetDir) {
	//open the map file
	ifstream is((sMapDir + sFileName).c_str());

	if (!is.is_open())
		throw(exception("Failed to open map file for reading"));

	//get the metadata
	getline(is, m_sTilesetName);

	ClrArray();

	is >> m_iTileX;
	is >> m_iTileY;
	is >> m_iTileW;
	is >> m_iTileH;

	GenArray();

	//load the array
	for (int j = 0; j < m_iTileY; j++) {
		for (int i = 0; i < m_iTileX; i++) {
			is >> m_pArray[i][j];
		}
	}

	//close the map file
	is.close();

	//load the tileset
	SDL_FreeSurface(m_pTileset);
	m_pTileset = SDL_LoadBMP((sTilesetDir + m_sTilesetName).c_str());

	if (m_pTileset == NULL) {
		throw(exception("Failed to open tileset file for reading"));
	}
}

void MapUtility::Clear() {
	ClrArray();
}

void MapUtility::DrawMap(SDL_Surface* pDest, int iCamX, int iCamY) {
	//error checking
	if (m_pTileset == NULL || m_pArray == NULL) return;

	//setup the format
	SDL_Rect sclip;
	SDL_Rect dclip;

	sclip.w = m_iTileW;
	sclip.h = m_iTileH;

	//for each tile
	for (int j = 0; j < m_iTileY; j++) {
		for (int i = 0; i < m_iTileX; i++) {
			//set the source tile
			sclip.x = m_pArray[i][j] * m_iTileW % m_pTileset->w;
			sclip.y = m_pArray[i][j] * m_iTileW / m_pTileset->w * m_iTileH;

			//set the graphical location
			dclip.x = i * m_iTileW + iCamX;
			dclip.y = j * m_iTileH + iCamY;

			//draw
			SDL_BlitSurface(m_pTileset, &sclip, pDest, &dclip);
		}
	}
}

//-------------------------
//Accessors and mutators
//-------------------------

int MapUtility::GetTileX() {
	return m_iTileX;
}

int MapUtility::GetTileY() {
	return m_iTileY;
}

int MapUtility::GetTileW() {
	return m_iTileW;
}

int MapUtility::GetTileH() {
	return m_iTileH;
}

int MapUtility::GetTile(int x, int y) {
	if (x < 0 || y < 0 || x >= m_iTileX || y >= m_iTileY) return 0;
	return m_pArray[x][y];
}

int** MapUtility::GetTileArray() {
	return m_pArray;
}

SDL_Surface* MapUtility::GetTileset() {
	return m_pTileset;
}

string MapUtility::GetTilesetName() {
	return m_sTilesetName;
}

//-------------------------
//Private access members
//-------------------------

void MapUtility::GenArray() {
	if (m_pArray != NULL)
		throw(exception("Failed to generate the array"));

	//create X
	m_pArray = new int*[m_iTileX];

	for (int i = 0; i < m_iTileX; i++) {

		//create Y
		m_pArray[i] = new int[m_iTileY];
	}
}

void MapUtility::ClrArray() {
	if (m_pArray == NULL) return;

	for (int i = 0; i < m_iTileX; i++) {
		delete[] m_pArray[i];
	}

	delete[] m_pArray;

	m_pArray = NULL;
}

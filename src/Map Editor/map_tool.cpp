/* File Name: map_tool.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: A modular map utility.
*/
#include <exception>
#include <fstream>
#include "map_tool.h"
using namespace std;

//-------------------------
//Public access members
//-------------------------

MapTool::MapTool() {
	m_iTileX = 0;
	m_iTileY = 0;
	m_iTileW = 0;
	m_iTileH = 0;

	m_pArray = NULL;
	m_pTileset = NULL;
	m_iSelection = 0;
}

MapTool::~MapTool() {
	Clear();
}

void MapTool::Load(string sFileName, string sMapDir, string sTilesetDir) {
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

void MapTool::Save(string sFileName, string sMapDir) {
	//check the map data exists
	if (m_pArray == NULL)
		throw(exception("Failed to save map"));

	//open the map file
	ofstream os( (sMapDir + sFileName).c_str() );

	if (!os.is_open())
		throw(exception("Failed to open map file for writing"));

	//save the metadata
	os << m_sTilesetName << endl;
	os << m_iTileX << ' ';
	os << m_iTileY << ' ';
	os << m_iTileW << ' ';
	os << m_iTileH << ' ';
	os << endl;

	//save the array
	for (int j = 0; j < m_iTileY; j++) {
		for (int i = 0; i < m_iTileX; i++) {
			os << m_pArray[i][j] << ' ';
		}
		os << endl;
	}

	//close the map file
	os.close();
}

void MapTool::New(string sTilesetName, string sTilesetDir, int x, int y, int w, int h) {
	//set the metadata
	m_sTilesetName = sTilesetName;

	ClrArray();

	m_iTileX = x;
	m_iTileY = y;
	m_iTileW = w;
	m_iTileH = h;

	GenArray();

	//zero the array
	for (int j = 0; j < m_iTileY; j++) {
		for (int i = 0; i < m_iTileX; i++) {
			m_pArray[i][j] = 0;
		}
	}

	//load the new tileset
	SDL_FreeSurface(m_pTileset);
	m_pTileset = SDL_LoadBMP( (sTilesetDir + m_sTilesetName).c_str() );

	if (m_pTileset == NULL)
		throw(exception("Failed to open tileset file for reading"));
}

void MapTool::Clear() {
	ClrArray();
}

void MapTool::DrawMap(SDL_Surface* pDest, int iCamX, int iCamY) {
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

void MapTool::DrawTileset(SDL_Surface* pDest, int iCamX, int iCamY) {
	SDL_Rect dclip;

	dclip.x = iCamX;
	dclip.y = iCamY;

	SDL_BlitSurface(m_pTileset, NULL, pDest, &dclip);
}

//-------------------------
//Accessors and mutators
//-------------------------

int MapTool::GetTileX() {
	return m_iTileX;
}

int MapTool::GetTileY() {
	return m_iTileY;
}

int MapTool::GetTileW() {
	return m_iTileW;
}

int MapTool::GetTileH() {
	return m_iTileH;
}

int MapTool::GetTile(int x, int y) {
	return m_pArray[x][y];
}

int** MapTool::GetTileArray() {
	return m_pArray;
}

SDL_Surface* MapTool::GetTileset() {
	return m_pTileset;
}

string MapTool::GetTilesetName() {
	return m_sTilesetName;
}

int MapTool::SetSelection(int iMouseX, int iMouseY, int iCamX, int iCamY) {
	if (m_pTileset == NULL || m_pArray == NULL) return -1;

	if (iMouseX < iCamX || iMouseX >= m_pTileset->w + iCamX) return -1;
	if (iMouseY < iCamY || iMouseY >= m_pTileset->h + iCamY) return -1;

	return m_iSelection = ((iMouseY-iCamY) / m_iTileH) * (m_pTileset->w / m_iTileW) + ((iMouseX-iCamX) / m_iTileW);
}

int MapTool::SetTile(int iMouseX, int iMouseY, int iCamX, int iCamY) {
	if (m_pTileset == NULL || m_pArray == NULL) return -1;

	if (iMouseX < iCamX || iMouseX >= (m_iTileX * m_iTileW) + iCamX) return -1;
	if (iMouseY < iCamY || iMouseY >= (m_iTileY * m_iTileH) + iCamY) return -1;

	return m_pArray[(iMouseX-iCamX)/m_iTileW][(iMouseY-iCamY)/m_iTileH] = m_iSelection;
}

//-------------------------
//Private access members
//-------------------------

void MapTool::GenArray() {
	if (m_pArray != NULL)
		throw(exception("Failed to generate the array"));

	//create X
	m_pArray = new int*[m_iTileX];

	for (int i = 0; i < m_iTileX; i++) {

		//create Y
		m_pArray[i] = new int[m_iTileY];
	}
}

void MapTool::ClrArray() {
	if (m_pArray == NULL) return;

	for (int i = 0; i < m_iTileX; i++) {
		delete[] m_pArray[i];
	}

	delete[] m_pArray;

	m_pArray = NULL;
}

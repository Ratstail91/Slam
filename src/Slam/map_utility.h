/* File Name: map_utility.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: A modular map utility.
*/
#ifndef KR_MAPUTILITY_H_
#define KR_MAPUTILITY_H_

#include <string>
#include "SDL.h"
using std::string;

class MapUtility {
public:
	/* Public access members */
	MapUtility();
	~MapUtility();

	void Load(string sFileName, string sMapDir, string sTilesetDir);
	void Clear();

	void DrawMap(SDL_Surface* pDest, int iCamX = 0, int iCamY = 0);

	/* Accessors and mutators */
	int GetTileX			();
	int GetTileY			();
	int GetTileW			();
	int GetTileH			();
	int GetTile				(int x, int y);
	int** GetTileArray		();
	SDL_Surface* GetTileset	();
	string GetTilesetName	();

private:
	/* Private access members */
	void GenArray();
	void ClrArray();

	int m_iTileX;
	int m_iTileY;
	int m_iTileW;
	int m_iTileH;
	int** m_pArray;
	SDL_Surface* m_pTileset;
	string m_sTilesetName;
};

#endif

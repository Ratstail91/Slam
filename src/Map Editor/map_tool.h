/* File Name: map_tool.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: A modular map utility.
*/
#ifndef KR_MAPTOOL_H_
#define KR_MAPTOOL_H_

#include <string>
#include "SDL.h"
using std::string;

class MapTool {
public:
	/* Public access members */
	MapTool();
	~MapTool();

	void Load			(string sFileName, string sMapDir, string sTilesetDir);
	void Save			(string sFileName, string sMapDir);
	void New			(string sTilesetName, string sTilesetDir, int x, int y, int w, int h);
	void Clear			();

	void DrawMap		(SDL_Surface* pDest, int iCamX = 0, int iCamY = 0);
	void DrawTileset	(SDL_Surface* pDest, int iCamX = 0, int iCamY = 0);

	/* Accessors and mutators */
	int GetTileX			();
	int GetTileY			();
	int GetTileW			();
	int GetTileH			();
	int GetTile				(int x, int y);
	int** GetTileArray		();
	SDL_Surface* GetTileset	();
	string GetTilesetName	();

	int SetSelection		(int iMouseX, int iMouseY, int iCamX = 0, int iCamY = 0);
	int SetTile				(int iMouseX, int iMouseY, int iCamX = 0, int iCamY = 0);

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

	int m_iSelection;
};

#endif

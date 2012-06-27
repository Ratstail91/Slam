/* File Name: main.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#include <iostream>
#include "SDL.h"
#include "simple_callbacks.h"
using namespace std;

#include "map_tool.h"
#include "waypoint_tool.h"

//codebase stuff
#include "fps_utility.h"
#include "camera_2d_utility.h"
#include "config_utility.h"

//-------------------------
//Preprocessor directives
//-------------------------

#define CASE break; case
#define CAM_SPEED 0.28f
//#define NODE_SPEED 0.14f

//-------------------------
//Forward declarations
//-------------------------

//framework
void Init	();
void Head	();
void Render	();

//events
void Quit				(SDL_Event const* const) { scQuit(); };//dummy quit
void MouseMotion		(SDL_Event const* const);
void MouseButtonDown	(SDL_Event const* const);
void KeyDown			(SDL_Event const* const);
void KeyUp				(SDL_Event const* const);

//utilities
void SetMode(int);
void TogMode();
void ShowMode();

void LoadMap();
void SaveMap();
void NewMap();
void BakePaths();

//-------------------------
//Globals
//-------------------------

enum EditMode {
	_BEGIN = 0, //bounding

	MAP_EDITOR,
	TILE_SELECT,
	WAYPOINT_EDITOR,

	_END //bounding
};

int g_eEditMode = MAP_EDITOR;

Camera2DUtility g_camUtil;
ConfigUtility g_cfgUtil;

SDL_Surface* g_pScreen = NULL;
MapTool g_mapTool;
WaypointTool g_wayTool;

//-------------------------
//mainline
//-------------------------

int SDL_main(int, char**) {
	scSetInit(Init);
	scSetQuit(SDL_Quit);

	scSetHead(Head);
	scSetRender(Render);

	scSetEvent(Quit,			SDL_QUIT);
	scSetEvent(MouseMotion,		SDL_MOUSEMOTION);
	scSetEvent(MouseButtonDown,	SDL_MOUSEBUTTONDOWN);
	scSetEvent(KeyDown,			SDL_KEYDOWN);
	scSetEvent(KeyUp,			SDL_KEYUP);

	scProc();

	return 0;
}

//-------------------------
//definitions
//-------------------------

//framework

void Init() {
	if (SDL_Init(SDL_INIT_VIDEO))
		cerr << "Failed to init SDL" << endl;

	g_pScreen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);

	if (g_pScreen == NULL)
		cerr << "Failed to init the screen" << endl;

	//camera
	g_camUtil.SetScreenSize( g_pScreen->w, g_pScreen->h );
	g_camUtil.SetMode( Camera2DUtility::NORMAL );

	//assets
	try {
		g_cfgUtil.Load("config.cfg");

		NewMap();
	}
	catch(exception &e) {
		cerr << "Asset error: " << e.what() << endl;
		scQuit();
	}

	//edit mode
	ShowMode();
}

void Head() {
	FPSUtility::CalcDeltaTime();

	g_camUtil.Update( FPSUtility::GetDeltaTime() );
}

void Render() {
	//blank
	SDL_FillRect(g_pScreen, NULL, SDL_MapRGB(g_pScreen->format, 195, 195, 195));

	switch(g_eEditMode) {
		CASE MAP_EDITOR:
			{
				g_mapTool.DrawMap(g_pScreen, g_camUtil.GetCamX(), g_camUtil.GetCamY());

				Uint8* kb = SDL_GetKeyState(NULL);
				if (kb[SDLK_LSHIFT] || kb[SDLK_RSHIFT])
					g_wayTool.Draw(g_pScreen, g_camUtil.GetCamX(), g_camUtil.GetCamY());
			}

		CASE TILE_SELECT:
			g_mapTool.DrawTileset(g_pScreen, g_camUtil.GetCamX(), g_camUtil.GetCamY());

		CASE WAYPOINT_EDITOR:
			g_mapTool.DrawMap(g_pScreen, g_camUtil.GetCamX(), g_camUtil.GetCamY());
			g_wayTool.Draw(g_pScreen, g_camUtil.GetCamX(), g_camUtil.GetCamY());
			break;
	}

	SDL_Flip(g_pScreen);
}

//events
void MouseMotion(SDL_Event const* const event) {
	if (event->motion.state & SDL_BUTTON_LMASK) {
		switch(g_eEditMode) {
			CASE MAP_EDITOR:
				g_mapTool.SetTile(event->motion.x, event->motion.y, g_camUtil.GetCamX(), g_camUtil.GetCamY());

			CASE TILE_SELECT:
				break;

			CASE WAYPOINT_EDITOR:
				break;
		}
	}
}

void MouseButtonDown(SDL_Event const* const event) {
	if (event->button.button == SDL_BUTTON_LEFT) {
		switch(g_eEditMode) {
			CASE MAP_EDITOR:
				g_mapTool.SetTile(event->button.x, event->button.y, g_camUtil.GetCamX(), g_camUtil.GetCamY());

			CASE TILE_SELECT:
				g_mapTool.SetSelection(event->button.x, event->button.y, g_camUtil.GetCamX(), g_camUtil.GetCamY());

			CASE WAYPOINT_EDITOR:
				{
					Uint8* kb = SDL_GetKeyState(NULL);
					if (kb[SDLK_LSHIFT] || kb[SDLK_RSHIFT])
						g_wayTool.SetSelection(event->button.x, event->button.y, g_camUtil.GetCamX(), g_camUtil.GetCamY());
					else
						g_wayTool.AddNode(event->button.x, event->button.y, g_camUtil.GetCamX(), g_camUtil.GetCamY());
				}
		}

		return;
	}

	if (event->button.button == SDL_BUTTON_RIGHT) {
		switch(g_eEditMode) {
			CASE MAP_EDITOR:
				//

			CASE TILE_SELECT:
				//

			CASE WAYPOINT_EDITOR:
				{
					Uint8* kb = SDL_GetKeyState(NULL);
					if (kb[SDLK_LSHIFT] || kb[SDLK_RSHIFT])
						;//nothing should happen when the right button is clicked with shift down
					else
						g_wayTool.DelNearestNode(event->button.x, event->button.y, g_camUtil.GetCamX(), g_camUtil.GetCamY());
				}
		}

		return;
	}
}

void KeyDown(SDL_Event const* const event) {
	switch(event->key.keysym.sym) {
		CASE SDLK_ESCAPE: scQuit();

		//camera controls
		CASE SDLK_UP:		g_camUtil.ShiftMotionY(-CAM_SPEED);
		CASE SDLK_DOWN:		g_camUtil.ShiftMotionY( CAM_SPEED);
		CASE SDLK_LEFT:		g_camUtil.ShiftMotionX(-CAM_SPEED);
		CASE SDLK_RIGHT:	g_camUtil.ShiftMotionX( CAM_SPEED);

		//hotkeys
		CASE SDLK_F1:		BakePaths();
		CASE SDLK_s:		SaveMap();
		CASE SDLK_l:		LoadMap();
		CASE SDLK_n:		NewMap();
		CASE SDLK_SPACE:	g_camUtil.SetOriginPosition(0, 0);

		//mode control
		CASE SDLK_TAB:		TogMode();
		CASE SDLK_1:		SetMode(MAP_EDITOR);
		CASE SDLK_2:		SetMode(TILE_SELECT);
		CASE SDLK_3:		SetMode(WAYPOINT_EDITOR);
	}
}

void KeyUp(SDL_Event const* const event) {
	switch(event->key.keysym.sym) {
		//camera controls
		CASE SDLK_UP:		g_camUtil.ShiftMotionY( CAM_SPEED);
		CASE SDLK_DOWN:		g_camUtil.ShiftMotionY(-CAM_SPEED);
		CASE SDLK_LEFT:		g_camUtil.ShiftMotionX( CAM_SPEED);
		CASE SDLK_RIGHT:	g_camUtil.ShiftMotionX(-CAM_SPEED);
	};
}

//utilities
void SetMode(int eEditMode) {
	g_eEditMode = eEditMode;

	if (g_eEditMode >= _END || g_eEditMode <= _BEGIN) {
		g_eEditMode = _BEGIN;
		g_eEditMode++;
	}

	ShowMode();
}

void TogMode() {
	SetMode(g_eEditMode+1);
}

void ShowMode() {
	cout << "Edit Mode: " << g_eEditMode << endl;
}

void LoadMap() {
	//load the map file and path file
	try {
		g_mapTool.Load(g_cfgUtil["map.load"], g_cfgUtil["dir.map"], g_cfgUtil["dir.tileset"]);

		g_wayTool.Load(g_cfgUtil["path.load"], g_cfgUtil["dir.map"]);
	}
	catch(exception& e) {
		cerr << "Load Map Error: " << e.what() << endl;
	}
}

void SaveMap() {
	//save the map file and path file
	try {
		g_mapTool.Save(g_cfgUtil["map.save"], g_cfgUtil["dir.map"]);

		g_wayTool.Save(g_cfgUtil["path.save"], g_cfgUtil["dir.map"]);
	}
	catch(exception& e) {
		cerr << "Save Map Error: " << e.what() << endl;
	}
}

void NewMap() {
	//new map and waypoints
	try {
		g_mapTool.New(
				g_cfgUtil["map.new.tileset"],
				g_cfgUtil["dir.tileset"],
				g_cfgUtil.Integer("map.new.x"),
				g_cfgUtil.Integer("map.new.y"),
				g_cfgUtil.Integer("map.new.w"),
				g_cfgUtil.Integer("map.new.h")
			);

		g_wayTool.Clear();
	}
	catch(exception& e) {
		cerr << "New Map Error: " << e.what() << endl;
		throw(e);
	}
}

void BakePaths() {
	try {
		g_wayTool.Bake(g_cfgUtil["path.bake"],g_cfgUtil["dir.map"]);
	}
	catch(exception& e) {
		cerr << "Path Bake Error: " << e.what() << endl;
	}
}

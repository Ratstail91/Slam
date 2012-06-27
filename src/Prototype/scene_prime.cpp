/* File Name: scene_prime.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#include <exception>
#include <iostream>
#include "scene_prime.h"
using namespace std;

#include "SDL_gfxPrimitives.h"

#include "waypoint_utility.h"

//-------------------------
//Preprocessor directives
//-------------------------

#pragma warning( disable : 4244 )

#define CASE break; case
#define DEFAULT break; default
#define SPEED 0.14f

//-------------------------
//Public access members
//-------------------------

ScenePrime::ScenePrime() {
	//setup the framework
	m_cam.SetMode(CameraUtility::CENTER);
	m_cam.SetScreenSize(GetScreen()->w, GetScreen()->h);

	//load the assets
	try {
		m_red.Load("player_red.bmp");
		m_blu.Load("player_blue.bmp");

		m_map.Load("map1.txt");
	}
	catch(exception& e) {
		cerr << "Asset error: " << e.what() << endl;
		Return(SCENE_EXIT);
	}

	//initial setup
	m_red.SetBBox(3 - m_red.GetW()/2, 6 - m_red.GetH()/2, 28 - m_red.GetW()/2, 28 - m_red.GetH()/2);
	m_blu.SetBBox(3 - m_blu.GetW()/2, 6 - m_blu.GetH()/2, 28 - m_blu.GetW()/2, 28 - m_blu.GetH()/2);
//	m_blu.SetBBox(3, 6, 28, 28);
}

ScenePrime::~ScenePrime() {
	m_red.Unload();
	m_blu.Unload();

	m_map.Clear();
}

//-------------------------
//Frame loop members
//-------------------------

void ScenePrime::BeginLoop() {
	FPSUtility::CalcFrameRate();
}

void ScenePrime::EndLoop() {
	//
}

void ScenePrime::Update() {
	//waypoint pathfinding
	int current = WaypointUtility::Nearest(m_blu.GetRealX(), m_blu.GetRealY());
	int dest = WaypointUtility::Nearest(m_red.GetRealX(), m_red.GetRealY());
	int next = WaypointUtility::solution[current][dest];

	cout << current << ", " << dest << ", " << next << endl;

	m_iCurrentPos = current;
	m_iDestPos = dest;

//	m_blu.SetRealPosition(WaypointUtility::location[dest][0], WaypointUtility::location[dest][1]);

	//updates
	FPSUtility::CalcDeltaTime();

	float fPrevX = m_red.GetRealX();
	float fPrevY = m_red.GetRealY();

	m_red.Update(FPSUtility::GetDeltaTime());
	m_blu.Update(FPSUtility::GetDeltaTime());

	//collisions
	BBox wallBox(0, 0, m_map.GetW()-1, m_map.GetH()-1);

	//for each SURROUNDING tile, check the collision
	for (int i = (int)m_red.GetWorldBBox().x / m_map.GetW(); i <= (int)m_red.GetWorldBBox().w / m_map.GetW(); i++) {
		for (int j = (int)m_red.GetWorldBBox().y / m_map.GetH(); j <= (int)m_red.GetWorldBBox().h / m_map.GetH(); j++) {
			if (m_map.GetT(i, j) == 9 && wallBox.CheckWorldBBox( i*m_map.GetW(), j*m_map.GetH(), m_red.GetWorldBBox()) ) {
				m_red.SetRealPosition(fPrevX, fPrevY);
			}
		}
	}

	//camera
	m_cam.SetRealX(m_red.GetRealX() + m_red.GetW()/2);
	m_cam.SetRealY(m_red.GetRealY() + m_red.GetH()/2);
}

void ScenePrime::Draw(SDL_Surface* const pScreen) {
	SDL_FillRect(pScreen, NULL, SDL_MapRGB(pScreen->format, 255, 255, 255));

	//draw the grid
	for (int i = 0; i <= m_map.GetX(); i++) vlineRGBA(pScreen, m_cam.GetCamX()+i*m_map.GetW(), m_cam.GetCamY(), m_cam.GetCamY()+m_map.GetY()*m_map.GetH(), 0, 0, 0, 255);
	for (int j = 0; j <= m_map.GetY(); j++) hlineRGBA(pScreen, m_cam.GetCamX(), m_cam.GetCamX()+m_map.GetX()*m_map.GetW(), m_cam.GetCamY()+j*m_map.GetH(), 0, 0, 0, 255);

	//draw the map
	for (int i = 0; i < m_map.GetX(); i++) {
		for (int j = 0; j < m_map.GetY(); j++) {
			if (m_map.GetT(i, j) == 9) { //9, for some reason
				boxRGBA(pScreen, m_cam.GetCamX() + i * m_map.GetW(), m_cam.GetCamY() + j * m_map.GetH(), m_cam.GetCamX() + (i+1) * m_map.GetW(), m_cam.GetCamY() + (j+1) * m_map.GetH(), 0, 0, 0, 255);
			}
		}
	}

	//draw the collision boxes
	rectangleRGBA(pScreen, m_cam.GetCamX() + m_red.GetWorldBBox().x, m_cam.GetCamY() + m_red.GetWorldBBox().y, m_cam.GetCamX() + m_red.GetWorldBBox().w, m_cam.GetCamY() + m_red.GetWorldBBox().h, 255, 0, 255, 255);
	rectangleRGBA(pScreen, m_cam.GetCamX() + m_blu.GetWorldBBox().x, m_cam.GetCamY() + m_blu.GetWorldBBox().y, m_cam.GetCamX() + m_blu.GetWorldBBox().w, m_cam.GetCamY() + m_blu.GetWorldBBox().h, 255, 0, 255, 255);

	//draw the players
	m_red.DrawTo(pScreen, m_cam.GetCamX(), m_cam.GetCamY());
	m_blu.DrawTo(pScreen, m_cam.GetCamX(), m_cam.GetCamY());

	//draw the waypoint nodes
	for (int i = 0; i < WaypointUtility::count; i++) {
		filledCircleRGBA(pScreen, WaypointUtility::location[i][0] + m_cam.GetCamX(), WaypointUtility::location[i][1] + m_cam.GetCamY(), 5, 255, 0, 0, 255);
	}
}

//-------------------------
//Input loop members
//-------------------------

void ScenePrime::MouseMotion(SDL_MouseMotionEvent const& rMotion) {
	//
}

void ScenePrime::MouseButtonDown(SDL_MouseButtonEvent const& rButton) {
	switch(rButton.button) {
		case SDL_BUTTON_LEFT:
			m_red.SetRealX( rButton.x - m_cam.GetCamX() );
			m_red.SetRealY( rButton.y - m_cam.GetCamY() );
			break;
		case SDL_BUTTON_RIGHT:
			m_blu.SetRealX( rButton.x - m_cam.GetCamX() );
			m_blu.SetRealY( rButton.y - m_cam.GetCamY() );
			break;
	}
}

void ScenePrime::MouseButtonUp(SDL_MouseButtonEvent const& rButton) {
	//
}

void ScenePrime::KeyDown(SDL_KeyboardEvent const& rKey) {
	switch(rKey.keysym.sym) {
		CASE SDLK_ESCAPE: Quit();

		//player movement
		CASE SDLK_UP:		m_red.ShiftMotionY(-SPEED);
		CASE SDLK_DOWN:		m_red.ShiftMotionY( SPEED);
		CASE SDLK_LEFT:		m_red.ShiftMotionX(-SPEED);
		CASE SDLK_RIGHT:	m_red.ShiftMotionX( SPEED);

		//debugging..
		CASE SDLK_SPACE:
			m_blu.SetRealPosition(
				WaypointUtility::location[WaypointUtility::solution[m_iCurrentPos][m_iDestPos]][0],
				WaypointUtility::location[WaypointUtility::solution[m_iCurrentPos][m_iDestPos]][1]
			);

/*		//complex player movement
		CASE SDLK_UP:
			if (m_pKeyboard[SDLK_DOWN])
				m_red.SetMotionY(0);
			else if (m_pKeyboard[SDLK_LEFT] || m_pKeyboard[SDLK_RIGHT])
				m_red.SetMotionY(-SPEED/2);
			else
				m_red.SetMotionY(-SPEED);

		CASE SDLK_DOWN:
			if (m_pKeyboard[SDLK_UP])
				m_red.SetMotionY(0);
			else if (m_pKeyboard[SDLK_LEFT] || m_pKeyboard[SDLK_RIGHT])
				m_red.SetMotionY(SPEED/2);
			else
				m_red.SetMotionY(SPEED);

		CASE SDLK_LEFT:
			if (m_pKeyboard[SDLK_RIGHT])
				m_red.SetMotionX(0);
			else if (m_pKeyboard[SDLK_UP] || m_pKeyboard[SDLK_DOWN])
				m_red.SetMotionX(-SPEED/2);
			else
				m_red.SetMotionX(-SPEED);

		CASE SDLK_RIGHT:
			if (m_pKeyboard[SDLK_LEFT])
				m_red.SetMotionX(0);
			else if (m_pKeyboard[SDLK_UP] || m_pKeyboard[SDLK_DOWN])
				m_red.SetMotionX(SPEED/2);
			else
				m_red.SetMotionX(SPEED);
//*/
		//hotkeys
		CASE SDLK_f: cout << "FPS: " << FPSUtility::GetFrameRate() << endl;
	};
}

void ScenePrime::KeyUp(SDL_KeyboardEvent const& rKey) {
	switch(rKey.keysym.sym) {
		//player movement
		CASE SDLK_UP:		m_red.ShiftMotionY( SPEED);
		CASE SDLK_DOWN:		m_red.ShiftMotionY(-SPEED);
		CASE SDLK_LEFT:		m_red.ShiftMotionX( SPEED);
		CASE SDLK_RIGHT:	m_red.ShiftMotionX(-SPEED);
	};
}

//-------------------------
//Members
//-------------------------

//...


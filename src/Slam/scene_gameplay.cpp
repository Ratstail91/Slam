/* File Name: scene_gameplay.cpp
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#define _USE_MATH_DEFINES
#include <exception>
#include <iostream>
#include <math.h>
#include "scene_gameplay.h"
using namespace std;

//-------------------------
//Preprocessor directives
//-------------------------

#if KR_BASESCENE_H_ != 2012062701 //27/6/2012, revision 1
#error BaseScene version is incompatible with this scene
#endif

//-------------------------
//Public access members
//-------------------------

SceneGameplay::SceneGameplay() {
	//setup the environment
	SDL_WM_SetCaption("Slam Gameplay", NULL);
	SetScreen(800, 600, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE|SDL_DOUBLEBUF);

	m_cd = ClientData::GetSingletonPtr();

	//setup the camera: no targeting system yet
	m_camera.SetScreenSize(GetScreen()->w, GetScreen()->h);
	m_camera.SetMode( Camera2DUtility::CENTER );

	//load the map: this should probably be in SceneSync, but meh.
	m_map.Load("map.map", m_cd->config["dir.map"], m_cd->config["dir.tileset"]);
	m_way.Load("map.pth", m_cd->config["dir.map"]);

	//these pointers will speedup access, and (hopefully) simplify the code
	m_pPacket = reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() );
	m_pAvatar = &m_cd->vUserAvatars[m_cd->index];
}

SceneGameplay::~SceneGameplay() {
	m_map.Clear();
	m_way.Clear();

	m_cd->vUserAvatars.clear();
	m_cd->vAIAvatars.clear();
}

//-------------------------
//Frame loop members
//-------------------------

void SceneGameplay::Head() {
	FPSUtility::CalcFrameRate();
}

void SceneGameplay::Tail() {
	//
}

void SceneGameplay::Update() {
	//update the AI
	if (m_cd->host) {
		RunAIAlgorithms();
	}

	//update the from network (AI bounces back)
	while(m_cd->network.Receive()) ServerCommands();

	//chache the player's position for the collision
	float fPrevX = m_pAvatar->GetOriginX();
	float fPrevY = m_pAvatar->GetOriginY();

	//delta for every entity in the world
	FPSUtility::CalcDeltaTime();

	//update the avatars
	for (vector<UserAvatar>::iterator it = m_cd->vUserAvatars.begin(); it != m_cd->vUserAvatars.end(); it++) {
		it->Update(FPSUtility::GetDeltaTime());
	}

	/* This following code is hard to follow so pay attention.
	 * The first line creates a collision box to use on the walls.
	 * The for loops iterate ove only the walls closest to the entity,
	 *  since checking them all will weigh the game down.
	 * Finally, if the player collides with a wall, set the players Origin and Image
	 *  positions back to the cached position.
	 * This data must also be synced.
	 * 
	 * THIS NEEDS TO BE FOLDED INTO ANOTHER FUNCTION
	*/

	BBox2D box(0.0f, 0.0f, m_map.GetTileW()-1.0f, m_map.GetTileH()-1.0f); //32*32

	//for each SURROUNDING TILE
	for (int i = (int)m_pAvatar->GetWorldBBox().x / m_map.GetTileW(); i <= (int)m_pAvatar->GetWorldBBox().w / m_map.GetTileW(); i++) {
		for (int j = (int)m_pAvatar->GetWorldBBox().y / m_map.GetTileH(); j <= (int)m_pAvatar->GetWorldBBox().h / m_map.GetTileH(); j++) {

			//if solid && collision
			if (m_map.GetTile(i, j) > 0 && box.CheckWorldBBox(float(i*m_map.GetTileW()), float(j*m_map.GetTileH()), m_pAvatar->GetWorldBBox())) {

				//reset the player's position to the cached position, and stop them from moving
				m_pAvatar->SetOriginPosition(fPrevX, fPrevY);
				m_pAvatar->StopMotion();
				m_pAvatar->SetImagePosition((int)fPrevX, (int)fPrevY); //prevent the dance

				SyncAvatar(m_pAvatar, m_cd->index);
			}
		}
	}

	//update the camera
	m_camera.SetOriginPosition(m_pAvatar->GetOriginX(), m_pAvatar->GetOriginY());
}

void SceneGameplay::Render(SDL_Surface* const pScreen) {
	SDL_FillRect(pScreen, NULL, SDL_MapRGB(pScreen->format, 255, 255, 255));

	//draw the map
	m_map.DrawMap(pScreen, m_camera.GetCamX(), m_camera.GetCamY());

	//draw the avatars
	for (vector<UserAvatar>::iterator it = m_cd->vUserAvatars.begin(); it != m_cd->vUserAvatars.end(); it++) {
		it->DrawTo(pScreen, m_camera.GetCamX(), m_camera.GetCamY());
	}

	//draw the bullets
	//...

	//draw the pickups
	//...
}

//-------------------------
//Input loop members
//-------------------------

void SceneGameplay::MouseMotion(SDL_MouseMotionEvent const& rMotion) {
	SetAngle(m_pAvatar, m_cd->index, rMotion.x - m_camera.GetCamX(), rMotion.y - m_camera.GetCamY());
}

void SceneGameplay::MouseButtonDown(SDL_MouseButtonEvent const& rButton) {
	//
}

void SceneGameplay::MouseButtonUp(SDL_MouseButtonEvent const& rButton) {
	//
}

void SceneGameplay::KeyDown(SDL_KeyboardEvent const& rKey) {
	switch(rKey.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
			break;

		//player movement
		case SDLK_w:	ShiftUp(); break;
		case SDLK_s:	ShiftDown(); break;
		case SDLK_a:	ShiftLeft(); break;
		case SDLK_d:	ShiftRight(); break;

		//hotkeys
		case SDLK_f: cout << "FPS: " << FPSUtility::GetFrameRate() << endl;
	};
}

void SceneGameplay::KeyUp(SDL_KeyboardEvent const& rKey) {
	switch(rKey.keysym.sym) {
		//player movement
		//these ifs are needed to prevent the moonwalk
		case SDLK_w:
			if (m_pAvatar->GetMotionY() < 0)
				ShiftDown();
			break;

		case SDLK_s:
			if (m_pAvatar->GetMotionY() > 0)
				ShiftUp();
			break;

		case SDLK_a:
			if (m_pAvatar->GetMotionX() < 0)
				ShiftRight();
			break;

		case SDLK_d:
			if (m_pAvatar->GetMotionX() > 0)
				ShiftLeft();
			break;
	};
}

//-------------------------
//Networking
//-------------------------

void SceneGameplay::ServerCommands() {
	switch(m_pPacket->type) {
		case UPDATE_PLAYER:
			//block your own avatar
			if (m_pPacket->update.index == m_cd->index)
				break;

			//update an avatar's position in the world
			m_cd->vUserAvatars[m_pPacket->update.index].SetOriginPosition(
				m_pPacket->update.originX, m_pPacket->update.originY);
			m_cd->vUserAvatars[m_pPacket->update.index].SetMotion(
				m_pPacket->update.motionX, m_pPacket->update.motionY);
			m_cd->vUserAvatars[m_pPacket->update.index].SetAngle(
				m_pPacket->update.angle);

			break;
			//bullet
			//pickup
	};
}

void SceneGameplay::ClientCommands() {
	//
}

//-------------------------
//Movement controllers
//These will also send update packets to the network
//-------------------------

#define SPEED 0.14f

void SceneGameplay::ShiftUp() {
	m_pAvatar->ShiftMotionY(-SPEED);

	SyncAvatar(m_pAvatar, m_cd->index);
}

void SceneGameplay::ShiftDown() {
	m_pAvatar->ShiftMotionY( SPEED);

	SyncAvatar(m_pAvatar, m_cd->index);
}

void SceneGameplay::ShiftLeft() {
	m_pAvatar->ShiftMotionX(-SPEED);

	SyncAvatar(m_pAvatar, m_cd->index);
}

void SceneGameplay::ShiftRight() {
	m_pAvatar->ShiftMotionX( SPEED);

	SyncAvatar(m_pAvatar, m_cd->index);
}

//-------------------------
//Avatar ccontrollers
//-------------------------

void SceneGameplay::SetAngle(UserAvatar* pAvatar, int index, int x, int y) {
	pAvatar->SetAngle( int(
		atan2(double( x - pAvatar->GetOriginX() ), double( y - pAvatar->GetOriginY() ) ) * 180 / M_PI + 180
		) );

	SyncAvatar(pAvatar, index);
}

void SceneGameplay::SyncAvatar(UserAvatar* pAvatar, int index) {
	//send the avatar's data to the network

	//fill the packet
	Packet p;

	p.type = UPDATE_PLAYER;
	p.update.index = index;
	p.update.originX = pAvatar->GetOriginX();
	p.update.originY = pAvatar->GetOriginY();
	p.update.motionX = pAvatar->GetMotionX();
	p.update.motionY = pAvatar->GetMotionY();
	p.update.angle = pAvatar->GetAngle();

	m_cd->network.Send(m_cd->server, &p, sizeof(Packet));
}

//-------------------------
//AI algorithms
//-------------------------

void SceneGameplay::RunAIAlgorithms() {
	//
}

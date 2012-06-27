/* File Name: scene_mgr.cpp
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#include <exception>
#include "scene_mgr.h"
#include "client_data.h"

//-------------------------
//Preprocessor directives
//-------------------------

#if KR_BASESCENEMGR_H_ != 2012062701 //27/6/2012, revision 1
#error BaseSceneMgr version is incompatible with SceneMgr
#endif

//-------------------------
//Scene headers
//-------------------------

#include "scene_splash.h"
#include "scene_lobby.h"
#include "scene_sync.h"
#include "scene_gameplay.h"

//-------------------------
//Public access members
//-------------------------

//SceneMgr::SceneMgr() {
//	//
//}

//SceneMgr::~SceneMgr() {
//	//
//}

void SceneMgr::Init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw(std::exception("Failed to initialise SDL"));
	}

	//must set the screen in each scene

	//initialise the client data
	ClientData* cd = ClientData::GetSingletonPtr();

	cd->config.Load("config.cfg");
	cd->network.Init(cd->config.Integer("net.client.port"), sizeof(Packet));

	//IPs
	int err = 0;

	err |= SDLNet_ResolveHost(&cd->local, "127.0.0.1", cd->config.Integer("net.server.port"));
	err |= SDLNet_ResolveHost(&cd->broadcast, "255.255.255.255", cd->config.Integer("net.server.port"));

	if (err) throw(std::exception("Failed to set the default IP addresses"));
}

//void SceneMgr::Proc() {
//	//
//}

void SceneMgr::Quit() {
	//scene system
	BaseSceneMgr::Quit();

	//client data
	ClientData* cd = ClientData::GetSingletonPtr();
	cd->network.Quit();
}

//-------------------------
//Protected access members
//-------------------------

void SceneMgr::LoadScene() {
	delete GetScene();

	switch(GetNext()) {
		case BaseScene::SCENE_FIRST: //this is always the first scene in the game

		case SCENE_SPLASH:
			SetScene( new SceneSplash() );
			break;

		case SCENE_LOBBY:
			SetScene( new SceneLobby() );
			break;

		case SCENE_SYNC:
			SetScene( new SceneSync() );
			break;

		case SCENE_GAMEPLAY:
			SetScene( new SceneGameplay() );
			break;
	}
}

//-------------------------
//SDL_main()
//-------------------------

#include <iostream>
using namespace std;

int SDL_main(int, char**) {
	try {
		SceneMgr app;

		app.Init();
		app.Proc();
		app.Quit();
	}
	catch(exception& e) {
		cerr << "Existance Error: " << e.what() << endl;
	}

	return 0;
}

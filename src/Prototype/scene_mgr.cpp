/* File Name: scene_mgr.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#include "scene_mgr.h"

//-------------------------
//Scene headers
//-------------------------

#include "scene_prime.h"

//-------------------------
//Public access members
//-------------------------

SceneMgr::SceneMgr() {
	//
}

SceneMgr::~SceneMgr() {
	//
}

//void SceneMgr::Init() {
//	//
//}

//void SceneMgr::Loop() {
//	//
//}

//void SceneMgr::Quit() {
//	//
//}

//-------------------------
//Protected access members
//-------------------------

void SceneMgr::LoadScene() {
	delete GetScene();

	switch(GetNext()) {
		case SCENE_FIRST:
		case SCENE_PRIME:
			SetScene( new ScenePrime() );
	}
}

//-------------------------
//SDL_main()
//-------------------------

#include <iostream>
using namespace std;

int SDL_main(int, char**) {
	SceneMgr app;

	app.Init();
	app.Loop();
	app.Quit();

	return 0;
}

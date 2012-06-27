/* File Name: scene_splash.cpp
 * Author: 
 * Date: 
 * Copyright: 
 * Description: Display a logo for a moment.
*/
#include <iostream>
#include "scene_splash.h"
#include "client_data.h"
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

SceneSplash::SceneSplash() {
	ClientData* cd = ClientData::GetSingletonPtr();

	m_logo.LoadImage((cd->config["dir.logo"]+"logo.bmp").c_str());

	SDL_WM_SetCaption("Loading...", NULL);
	SetScreen(m_logo.GetImageW(), m_logo.GetImageH(), SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE|SDL_DOUBLEBUF);

	m_startTick = clock();
}

SceneSplash::~SceneSplash() {
	m_logo.UnloadImage();
}

//-------------------------
//Frame loop members
//-------------------------

void SceneSplash::Update() {
	if (clock() - m_startTick > CLOCKS_PER_SEC * 2) {
		SetNextScene(SCENE_LOBBY);
	}
}

void SceneSplash::Render(SDL_Surface* const pScreen) {
	m_logo.DrawTo(pScreen);
}

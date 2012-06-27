/* File Name: scene_sync.cpp
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#include <iostream>
#include "scene_sync.h"
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

SceneSync::SceneSync() {
	//setup the environment
	SDL_WM_SetCaption("Loading...", NULL);

	m_cd = ClientData::GetSingletonPtr();
	m_pPacket = reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() );

	//load the logo
	m_logo.LoadImage( (m_cd->config["dir.logo"]+"logo.bmp").c_str() );
	SetScreen(m_logo.GetSheetW(), m_logo.GetSheetH(), SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE|SDL_DOUBLEBUF);

	//error check
	if (reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() )->type != GAME_SYNC) {
		throw(exception("Packet is of the incorrect type entering SceneSync")); //yeah, that makes perfect grammatical sense
	}

	//set the count
	m_uiLoaded = 0;
	m_uiClientsReady = 0;
	m_uiPlayerCount = reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() )->begin.playerCount;

	//reserve the space for the avatars and AIs
	for (unsigned int i = 0; i < m_uiPlayerCount; i++) {
		m_cd->vUserAvatars.push_back(UserAvatar());
	}

	if (m_cd->host) {
		for (int i = 0; i < m_cd->config.Integer("host.bot.count"); i++) {
			m_cd->vAIAvatars.push_back(AIAvatar());
		}
	}

	//packet barrage: send each packet to the server, which then sends to each client, including the host
	if (m_cd->host) //but only if this is the host
	for (vector<Packet>::iterator it = m_cd->vPacketList.begin(); it != m_cd->vPacketList.end(); it++) {
		cout << "Sending barrage packet" << endl;

		m_cd->network.Send(m_cd->server, &(*it), sizeof(Packet));
	}

	//clear the packets
	m_cd->vPacketList.clear();
}

SceneSync::~SceneSync() {
	m_logo.UnloadImage();
}

//-------------------------
//Frame loop members
//-------------------------

void SceneSync::Update() {
	while(m_cd->network.Receive()) ServerCommands();
}

void SceneSync::Render(SDL_Surface* const pScreen) {
	m_logo.DrawTo(pScreen);
}

//-------------------------
//Networking
//-------------------------

void SceneSync::ServerCommands() {
	switch(reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() )->type) {
		case CREATE_PLAYER:
			CreatePlayer();
			break;

		case GAME_READY:
			GameReady();
			break;

		case GAME_GO:
			SetNextScene(SCENE_GAMEPLAY);
			break;

		default:
			cout << "Unknown network actvity" << endl;
	};
}

void SceneSync::CreatePlayer() {
	cout << "CREATE_PLAYER" << endl;

	//load each element of the avatars
	m_cd->vUserAvatars[m_pPacket->create.index].LoadImage((m_cd->config["dir.sprite"] + m_cd->config[m_pPacket->create.szChar]).c_str() );

	m_uiLoaded++;

	if (m_uiLoaded == m_uiPlayerCount) {
		//everything has been loaded
		cout << "LOADED!" << endl;

		//fill the packet
		Packet p;
		p.type = GAME_READY;

		m_cd->network.Send(m_cd->server, &p, sizeof(Packet));
	}
}

void SceneSync::GameReady() {
	if (!m_cd->host) return;
	m_uiClientsReady++;

	if (m_uiClientsReady = m_uiPlayerCount) {
		//fill the packet
		Packet p;

		p.type = GAME_GO;

		m_cd->network.Send(m_cd->server, &p, sizeof(Packet));
	}
}

/* File Name: scene_sync.h
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#ifndef KR_SCENESYNC_H_
#define KR_SCENESYNC_H_

#include "SDL.h"
#include "base_scene.h"
#include "scene_list.h"

#include "image.h"
#include "client_data.h"

class SceneSync : public BaseScene {
public:
	/* Public access members */
	SceneSync();
	virtual ~SceneSync();

protected:
	/* Frame loop members */
	virtual void Update	();
	virtual void Render	(SDL_Surface* const);

	/* Networking */
	void ServerCommands();
	void CreatePlayer();
	void GameReady();

	/* Members */
	ClientData* m_cd;
	const Packet* m_pPacket;
	unsigned char m_uiPlayerCount;
	unsigned char m_uiLoaded;
	unsigned char m_uiClientsReady;
	Image m_logo; //probably a waste...
};

#endif

/* File Name: scene_lobby.h
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#ifndef KR_SCENELOBBY_H_
#define KR_SCENELOBBY_H_

#include "SDL.h"
#include "base_scene.h"
#include "scene_list.h"

#include <vector>
#include <string>

#include "client_data.h"

class SceneLobby : public BaseScene {
public:
	/* Public access members */
	SceneLobby();
	virtual ~SceneLobby();

protected:
	/* Frame loop members */
	virtual void Update	();
	virtual void Render	(SDL_Surface* const);

	/* Input loop members */
	virtual void KeyDown			(SDL_KeyboardEvent const&);
	virtual void KeyUp				(SDL_KeyboardEvent const&);

	/* Members */
	void Push(std::string);
	void PushInstructions();
	void ClientCommands();
	void ServerCommands();

	/* Server Command List */
	void scPing();
	void scConfirmHost();
	void scConfirmJoin();
	void scPushPacket();

	/* Utilities */
	void ResolveIP(char* buffer, IPaddress add);

	ClientData* m_cd;

	std::vector<std::string> m_vLines;
	std::string m_sInput;
};

#endif

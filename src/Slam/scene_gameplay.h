/* File Name: scene_gameplay.h
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#ifndef KR_SCENEGAMEPLAY_H_
#define KR_SCENEGAMEPLAY_H_

//prerequisites
#include "SDL.h"
#include "base_scene.h"
#include "scene_list.h"

#include <vector>

//engine stuff
#include "camera_2d_utility.h"
#include "fps_utility.h"

//game stuff

//avatar systems
#include "avatar.h"
#include "user_avatar.h"
#include "ai_avatar.h"

//map systems
#include "map_utility.h"
#include "waypoint_utility.h"

//singletons do kinda suck
#include "client_data.h"

class SceneGameplay : public BaseScene {
public:
	/* Public access members */
	SceneGameplay();
	virtual ~SceneGameplay();

protected:
	/* Frame loop members */
	virtual void Head	();
	virtual void Tail	();
	virtual void Update	();
	virtual void Render	(SDL_Surface* const);

	/* Input loop members */
	virtual void MouseMotion		(SDL_MouseMotionEvent const&);
	virtual void MouseButtonDown	(SDL_MouseButtonEvent const&);
	virtual void MouseButtonUp		(SDL_MouseButtonEvent const&);
	virtual void KeyDown			(SDL_KeyboardEvent const&);
	virtual void KeyUp				(SDL_KeyboardEvent const&);

	/* Networking */
	void ServerCommands();
	void ClientCommands();

	/* Movement controllers */
	void ShiftUp	();
	void ShiftDown	();
	void ShiftLeft	();
	void ShiftRight	();

	/* Avatar ccontrollers */
//	void CheckCollision(...
	void SetAngle	(UserAvatar* pAvatar, int index, int x, int y);
	void SyncAvatar	(UserAvatar* pAvatar, int index);

	/* AI algorithms */
	void RunAIAlgorithms();

	/* Members */
	ClientData* m_cd;
	Camera2DUtility m_camera;

	MapUtility m_map;
	WaypointUtility m_way;

	/* These are for speed and convenience */
	const Packet* m_pPacket;
	UserAvatar* m_pAvatar;
};

#endif

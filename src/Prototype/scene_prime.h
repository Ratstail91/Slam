/* File Name: scene_prime.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#ifndef KR_SCENEPRIME_H_
#define KR_SCENEPRIME_H_

#include "SDL.h"
#include "base_scene.h"
#include "fps_utility.h"
#include "camera_utility.h"

#include "entity.h"
#include "entity_rot.h"
#include "map_utility.h"

class ScenePrime : public BaseScene {
public:
	/* Public access members */
	ScenePrime();
	virtual ~ScenePrime();

protected:
	/* Frame loop members */
	virtual void BeginLoop	();
	virtual void EndLoop	();
	virtual void Update		();
	virtual void Draw		(SDL_Surface* const);

	/* Input loop members */
	virtual void MouseMotion		(SDL_MouseMotionEvent const&);
	virtual void MouseButtonDown	(SDL_MouseButtonEvent const&);
	virtual void MouseButtonUp		(SDL_MouseButtonEvent const&);
	virtual void KeyDown			(SDL_KeyboardEvent const&);
	virtual void KeyUp				(SDL_KeyboardEvent const&);

	/* Members */
	CameraUtility m_cam;

	EntityRot m_red;
	EntityRot m_blu;

	MapUtility m_map;

	int m_iCurrentPos;
	int m_iDestPos;
};

#endif

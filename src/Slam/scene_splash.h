/* File Name: scene_splash.h
 * Author: 
 * Date: 
 * Copyright: 
 * Description: Display a logo for a moment.
*/
#ifndef KR_SCENESPLASH_H_
#define KR_SCENESPLASH_H_

#include "SDL.h"
#include "base_scene.h"
#include "scene_list.h"

#include "image.h"
#include <time.h>

class SceneSplash : public BaseScene {
public:
	/* Public access members */
	SceneSplash();
	virtual ~SceneSplash();

protected:
	/* Frame loop members */
	virtual void Update();
	virtual void Render(SDL_Surface* const);

	/* Members */
	Image m_logo;
	clock_t m_startTick;
};

#endif

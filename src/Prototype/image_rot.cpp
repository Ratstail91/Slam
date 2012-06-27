/* File Name: image_rot.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#include <exception>
#include "SDL_rotozoom.h"
#include "image_rot.h"

//-------------------------
//Public access members
//-------------------------

void ImageRot::DrawTo(SDL_Surface* const pDest, int iCamX, int iCamY) {
	if (GetSurface() == NULL) {
		throw(std::exception("No bitmap loaded"));
	}

	SDL_Surface* pSurface = rotozoomSurface(GetSurface(), m_iAngle, 1, 0);

	if (pSurface == NULL) {
		throw(std::exception("Failed to rotozoom surface"));
	}

	SDL_Rect sclip;
	SDL_Rect dclip;

	sclip.x = 0;
	sclip.y = 0;
	sclip.w = pSurface->w;
	sclip.h = pSurface->h;
	dclip.x = GetX() - pSurface->w/2 + iCamX;
	dclip.y = GetY() - pSurface->h/2 + iCamY;

	SDL_BlitSurface(pSurface, &sclip, pDest, &dclip);

	SDL_FreeSurface(pSurface);
}

//-------------------------
//Accessors and mutators
//-------------------------

int ImageRot::SetAngle(int iAngle) {
	return m_iAngle = iAngle % 360;
}

int ImageRot::IncAngle(int iAngle) {
	return m_iAngle = (m_iAngle + iAngle) % 360;
}

int ImageRot::GetAngle() {
	return m_iAngle;
}

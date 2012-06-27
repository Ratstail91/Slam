/* File Name: image_rot.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#ifndef KR_IMAGEROT_H_
#define KR_IMAGEROT_H_

#include "image.h"

class ImageRot : public Image {
public:
	/* Public access members */
	ImageRot() : Image() {}
	ImageRot(const char* szFileName, int x=0, int y=0, int w=0, int h=0) : Image(szFileName, x, y, w, h) {}
	ImageRot(SDL_Surface* pSurface,  int x=0, int y=0, int w=0, int h=0) : Image(pSurface, x, y, w, h) {}
	virtual ~ImageRot() {}

	virtual void DrawTo(SDL_Surface* const, int iCamX=0, int iCamY=0);

	/* Accessors and mutators */
	int SetAngle(int);
	int IncAngle(int);
	int GetAngle();

private:
	/* Private access members */
	int m_iAngle;
};

#endif

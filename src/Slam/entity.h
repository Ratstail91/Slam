/* File Name: entity.h
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#ifndef KR_ENTITY_H_
#define KR_ENTITY_H_

#include "image.h"
#include "origin_2d.h"
#include "bbox_2d.h"

class Entity : public Image, public Origin2D, public BBox2D {
public:
	/* Public access members */
	Entity();
	virtual ~Entity();

	virtual void LoadImage(const char* szFileName);

	virtual void Update(int iDelta);

	virtual Rect GetWorldBBox	();
	virtual int CheckWorldBBox	(Rect box);

protected:
	/* Protected access members */
	Image::SetImageX;
	Image::SetImageY;
	Image::GetImageX;
	Image::GetImageY;
};

#endif

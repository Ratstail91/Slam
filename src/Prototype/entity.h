/* File Name: entity.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#ifndef KR_ENTITY_H_
#define KR_ENTITY_H_

#include "image.h"
#include "origin.h"
#include "bbox.h"

class Entity : public Image, public Origin, public BBox {
public:
	/* Public access members */
	Entity();
	Entity(const char* szFileName);
	virtual ~Entity();

	virtual void Load(const char* szFileName);

	virtual void Update(int iDelta);

	virtual Rect GetWorldBBox	();
	virtual int CheckWorldBBox	(Rect box);

protected:
	/* Protected access members */
	Image::SetX;
	Image::SetY;
	Image::GetX;
	Image::GetY;
};

#endif

/* File Name: entity_rot.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#ifndef KR_ENTITYROT_H_
#define KR_ENTITYROT_H_

#include "image_rot.h"
#include "origin.h"
#include "bbox.h"

class EntityRot : public ImageRot, public Origin, public BBox {
public:
	/* Public access members */
	EntityRot();
	EntityRot(const char* szFileName);
	virtual ~EntityRot();

	virtual void Load(const char* szFileName);

	virtual void Update(int iDelta);

	virtual Rect GetWorldBBox	();
	virtual int CheckWorldBBox	(Rect box);

protected:
	/* Protected access members */
	ImageRot::SetX;
	ImageRot::SetY;
	ImageRot::GetX;
	ImageRot::GetY;
};

#endif

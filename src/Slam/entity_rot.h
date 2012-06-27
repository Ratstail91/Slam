/* File Name: entity_rot.h
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#ifndef KR_ENTITYROT_H_
#define KR_ENTITYROT_H_

#include "image_rot.h"
#include "origin_2d.h"
#include "bbox_2d.h"

class EntityRot : public ImageRot, public Origin2D, public BBox2D {
public:
	/* Public access members */
	EntityRot();
	virtual ~EntityRot();

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

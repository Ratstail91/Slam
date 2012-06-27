/* File Name: entity_rot.cpp
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#include "entity_rot.h"

//-------------------------
//Public access members
//-------------------------

EntityRot::EntityRot() {
	//
}

EntityRot::~EntityRot() {
	//
}

void EntityRot::LoadImage(const char* szFileName) {
	Image::LoadImage(szFileName);

	BBox2D::SetBBox((float)-GetImageW()/2, (float)-GetImageH()/2, (float)GetImageW()/2-1, (float)GetImageH()/2-1);
}

void EntityRot::Update(int iDelta) {
	Origin2D::Update(iDelta);

	SetImageX( (int)GetOriginX() );
	SetImageY( (int)GetOriginY() );
}

BBox2D::Rect EntityRot::GetWorldBBox() {
	return BBox2D::GetWorldBBox(GetOriginX(), GetOriginY());
}

int EntityRot::CheckWorldBBox(BBox2D::Rect box) {
	return BBox2D::CheckWorldBBox(GetOriginX(), GetOriginY(), box);
}

/* File Name: entity_rot.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#include "entity_rot.h"

//-------------------------
//Public access members
//-------------------------

EntityRot::EntityRot() {
	//
}

EntityRot::EntityRot(const char* szFileName) {
	Load(szFileName);
}

EntityRot::~EntityRot() {
	//
}

void EntityRot::Load(const char* szFileName) {
	Image::Load(szFileName);

	BBox::SetBBox( (float)-GetW()/2, (float)-GetH()/2, (float)GetW()/2-1, (float)GetH()/2-1);
}

void EntityRot::Update(int iDelta) {
	Origin::Update(iDelta);

	SetX( (int)GetRealX() );
	SetY( (int)GetRealY() );
}

BBox::Rect EntityRot::GetWorldBBox() {
	return BBox::GetWorldBBox(GetRealX(), GetRealY());
}

int EntityRot::CheckWorldBBox(Rect box) {
	return BBox::CheckWorldBBox(GetRealX(), GetRealY(), box);
}

/* File Name: entity.cpp
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#include "entity.h"

//-------------------------
//Public access members
//-------------------------

Entity::Entity() {
	//
}

Entity::Entity(const char* szFileName) {
	Load(szFileName);
}

Entity::~Entity() {
	//
}

void Entity::Load(const char* szFileName) {
	Image::Load(szFileName);

	BBox::SetBBox( 0, 0, (float)GetW()-1, (float)GetH()-1 );
}

void Entity::Update(int iDelta) {
	Origin::Update(iDelta);

	SetX( (int)GetRealX() );
	SetY( (int)GetRealY() );
}

BBox::Rect Entity::GetWorldBBox() {
	return BBox::GetWorldBBox(GetRealX(), GetRealY());
}

int Entity::CheckWorldBBox(Rect box) {
	return BBox::CheckWorldBBox(GetRealX(), GetRealY(), box);
}

/* File Name: entity.cpp
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#include "entity.h"

//-------------------------
//Public access members
//-------------------------

Entity::Entity() {
	//
}

Entity::~Entity() {
	//
}

void Entity::LoadImage(const char* szFileName) {
	Image::LoadImage(szFileName);

	BBox2D::SetBBox( 0, 0, (float)GetImageW()-1, (float)GetImageH()-1 );
}

void Entity::Update(int iDelta) {
	Origin2D::Update(iDelta);

	SetImageX( (int)GetOriginX() );
	SetImageY( (int)GetOriginY() );
}

BBox2D::Rect Entity::GetWorldBBox() {
	return BBox2D::GetWorldBBox(GetOriginX(), GetOriginY());
}

int Entity::CheckWorldBBox(BBox2D::Rect box) {
	return BBox2D::CheckWorldBBox(GetOriginX(), GetOriginY(), box);
}

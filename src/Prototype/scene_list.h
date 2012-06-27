/* File Name: scene_list.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: 
*/
#ifndef KR_SCENELIST_H_
#define KR_SCENELIST_H_

enum SceneList {

#ifndef KR_BASESCENE_H_ //where these are normally declared
	//control
	SCENE_EXIT		= -1, //close the program
	SCENE_NULL		= 0, //do nothing
	SCENE_FIRST		= 1, //default
#else
	//reserved values
	_RESERVED_1		= -1,
	_RESERVED_2		= 0,
	_RESERVED_3		= 1,
#endif

	//program
	SCENE_PRIME
};

#endif

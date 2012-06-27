/* File Name: packet.h
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#ifndef KR_PACKET_H_
#define KR_PACKET_H_

typedef unsigned char ubyte;

enum {
	//generic
	PING,
	MESSAGE,

	//connection
	REQUEST_HOST,
	REQUEST_JOIN,
	CONFIRM_HOST,
	CONFIRM_JOIN,

	//game control
	GAME_SYNC,
	GAME_READY,
	GAME_GO,
	GAME_END,

	//host/client interaction
	CREATE_PLAYER,
	//bullet...
	UPDATE_PLAYER,
};

//-------------------------

typedef struct Payload {
	ubyte type;
	char szPayload[512 * 2];
}Payload;

typedef struct Begin {
	ubyte type;
	ubyte playerCount; //the amount of space to reserve: clients + bots
//	char szMap; //hard coded map for now
}Begin;

typedef struct PlayerCreate {
	ubyte type;
	ubyte index;
	char szName[512];
	char szChar[512];
}PlayerCreate;

typedef struct PlayerUpdate {
	ubyte type;
	ubyte index;
	float originX;
	float originY;
	float motionX;
	float motionY;
	int angle;
}PlayerUpdate;

//-------------------------

//the union
typedef union Packet {
	ubyte type;
	Payload ping;
	Payload message;
	PlayerCreate create;
	PlayerUpdate update;
	Begin begin;
}Packet;

#endif


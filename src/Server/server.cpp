/* File Name: server.cpp
 * Author: 
 * Date: 
 * Copyright: 
 * Description: server for the game slam.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <exception>
#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_net.h"
#include "udp_network.h"
#include "config_utility.h"
using namespace std;

#include "..\Slam\packet.h"

//forward declarations
void Init();
void Quit();
void Proc();

void ServerCommands();
void ClientCommands();

void Ping();
void Relay();
void RequestHost();
void RequestJoin();
void Lock();
void Unlock();
void StartGame();
void EndGame();

//global variables
bool g_bRunning = true;
bool g_bLockout = false; //restrict the commands to sync/gameplay

UDPNetwork g_network;
ConfigUtility g_config;

string g_sServerName;
vector<IPaddress> g_vClients;

//mainline
int SDL_main(int, char**) {
	try {
		Init();
		Proc();
		Quit();
	}
	catch(exception& e) {
		cerr << "Error: " << e.what() << endl;
	}

	return 0;
}

//definitions
void Init() {
	g_config.Load("config.cfg");

	g_network.Init(g_config.Integer("net.server.port"), sizeof(Packet));

	cout << "Slam Server: " << g_config["server.name"] << endl;
}

void Quit() {
	g_network.Quit();
}

void Proc() {
	while(g_bRunning) {
		//wait
		while(!g_network.Receive());

		if (g_bLockout) {
			switch( reinterpret_cast<const Packet*>( g_network.GetDataIn() )->type ) {
				//sync
				case CREATE_PLAYER:		Relay(); break;
				case GAME_READY:		Relay(); break; //RelayHost()?
				case GAME_GO:			Relay(); break;

				//ingame
				case UPDATE_PLAYER:		Relay(); break;

				//ending the game
				case GAME_END: EndGame();  break;
			}//switch
		}
		else { //lobby
			switch( reinterpret_cast<const Packet*>( g_network.GetDataIn() )->type ) {
				case PING:			Ping(); break;
				case MESSAGE:		Relay(); break;
				case REQUEST_HOST:	RequestHost(); break;
				case REQUEST_JOIN:	RequestJoin(); break;
				case GAME_SYNC:		StartGame(); break;
				default:			cout << "Unknown network activity" << endl;
			}//switch
		}//if-else
	}//while
}//Proc

void Ping() {
	cout << "Ping message: " << reinterpret_cast<const Packet*>( g_network.GetDataIn() )->ping.szPayload << endl;

	//fill the packet
	Packet p;
	p.type = PING;
	strcpy(p.ping.szPayload, g_config["server.name"].c_str());

	g_network.Send(g_network.GetPacketIn()->address, (&p), sizeof(Packet));
}

void Relay() {
	//relay the in packet to every known client
	for (vector<IPaddress>::iterator it = g_vClients.begin(); it != g_vClients.end(); it++) {
		g_network.Send(*it, reinterpret_cast<const void*>( g_network.GetDataIn() ), sizeof(Packet));
	}
}

void RequestHost() {
	if (g_vClients.size() > 0) return; //ignore more than one host

	//extract the data
	Packet p;

	p.create = reinterpret_cast<const Packet*>( g_network.GetDataIn() )->create;

	//add the host to the list
	p.create.index = 0;
	g_vClients.push_back( g_network.GetPacketIn()->address );

	//send the data to the host client
	p.type = CONFIRM_HOST;
	g_network.Send( g_vClients[0], &p, sizeof(Packet) );

	//output
	cout << "Host found" << endl;
	cout << "New player: " << p.create.szName << ", playing as " << p.create.szChar << endl;
}

void RequestJoin() {
	if (g_vClients.size() == 0) return; //ignore without a host

	//extract the data
	Packet p;

	p.create = reinterpret_cast<const Packet*>( g_network.GetDataIn() )->create;

	//add this client to the list
	p.create.index = g_vClients.size();
	g_vClients.push_back( g_network.GetPacketIn()->address );

	//send the data to the client
	p.type = CONFIRM_JOIN;
	g_network.Send( g_vClients.back(), &p, sizeof(Packet) );

	//send the data to the host
	//this packet is filtered using the "host" boolean in ClientData
	g_network.Send( g_vClients[0], &p, sizeof(Packet) );

	//output
	cout << "New player: " << p.create.szName << ", playing as " << p.create.szChar << endl;
}

void Lock() {
	cout << "Locking input" << endl;
	g_bLockout = true;

	//...
}

void Unlock() {
	//free every connection...
	cout << "Unlocking input" << endl;
	g_bLockout = false;

	//...
}

void StartGame() {
	Lock();

	Relay();
}

void EndGame() {
	Unlock();

	Relay();

	//clear the list of clients, etc.
}

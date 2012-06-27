/* File Name: client_data.h
 * Author: 
 * Date: 
 * Copyright: 
 * Description: This class holds the otherwise global objects.
*/
#ifndef KR_CLIENTDATA_H_
#define KR_CLIENTDATA_H_

#include <vector>

#include "config_utility.h"
#include "udp_network.h"

#include "packet.h"
#include "user_avatar.h"
#include "ai_avatar.h"

class ClientData {
	ClientData() {};

public:
	//just for the record, I hate singletons
	static ClientData* GetSingletonPtr() {
		static ClientData clientData;
		return &clientData;
	};

	//members
	ConfigUtility config;
	UDPNetwork network;

	//general IPs
	IPaddress local, broadcast, server;

	//game stuff (setup/reset in SceneLobby?)
	bool host;
	int index;

	//vector lists
	std::vector<Packet> vPacketList;
	std::vector<UserAvatar> vUserAvatars;
	std::vector<AIAvatar> vAIAvatars;
};

#endif

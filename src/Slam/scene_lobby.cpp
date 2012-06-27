/* File Name: scene_lobby.cpp
 * Author: 
 * Date: 
 * Copyright: 
 * Description: 
*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "SDL_gfxPrimitives.h"
#include "scene_lobby.h"
using namespace std;

//-------------------------
//Preprocessor directives
//-------------------------

#if KR_BASESCENE_H_ != 2012062701 //27/6/2012, revision 1
#error BaseScene version is incompatible with this scene
#endif

#define DIVIDER "<------------------------------------------------------------------------------>"

//-------------------------
//Public access members
//-------------------------

SceneLobby::SceneLobby() {
	//setup the environment
	SDL_WM_SetCaption("Slam Game Lobby", NULL);
	SetScreen(640, 480, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE|SDL_DOUBLEBUF);

	m_cd = ClientData::GetSingletonPtr();

	//reset the connection status
	m_cd->host = false;
	m_cd->index = -1;
	m_cd->vPacketList.clear();

	PushInstructions();
}

SceneLobby::~SceneLobby() {
	//
}

//-------------------------
//Frame loop members
//-------------------------

void SceneLobby::Update() {
	//I know an if here is messy and slow, but I need to leave the creation packets in the que,
	//to be processed in SceneGameplay, rather than having them leak out here //FIX THIS...
	if(m_cd->network.Receive()) ServerCommands();
}

void SceneLobby::Render(SDL_Surface* const pScreen) {
	/* This assumes 640*480 screen, giving 80 characters across the screen.
	 * Double-height lines gives 30 lines on the screen.
	 * 2 lines are taken by the header, and 2 lines are taken by the input field.
	 * This gives 26 lines to display text.
	*/
	SDL_FillRect(pScreen, NULL, SDL_MapRGB(pScreen->format, 0, 0, 0));

	static Uint32 white = -1;

	//header
	stringColor(pScreen, 0, 0, "Server List", white);
	stringColor(pScreen, 0, 16, DIVIDER, white);

	//list
	for (unsigned int i = 0; i < m_vLines.size(); i++) {
		stringColor(pScreen, 0, 32 + i*16, m_vLines[i].c_str(), white);
	}

	//input field
	stringColor(pScreen, 0, 28*16, DIVIDER, white);

	//pos allows the input feild to scroll when the input gets to long, 79 (instead of 80) allows the "_" chatacter to be appended
	int pos = ( m_sInput.size() < 79) ? 0 : m_sInput.size() - 79;
	stringColor(pScreen, 0, 29*16, (m_sInput + "_").c_str() + pos , white);
}

//-------------------------
//Input loop members
//-------------------------

void SceneLobby::KeyDown(SDL_KeyboardEvent const& rKey) {
	//no modifiers checked
	switch(rKey.keysym.sym) {
		//command keys
		case SDLK_RETURN:
			ClientCommands();
			break;

		case SDLK_ESCAPE:
			QuitEvent();
			break;

		case SDLK_BACKSPACE:
			if (m_sInput.size() > 0) {
				m_sInput.erase( m_sInput.end()-1 );
			}
			break;

	}

	//uppercase modifiers
	if ( //xor
		( (rKey.keysym.mod & KMOD_CAPS) || (rKey.keysym.mod & KMOD_SHIFT) ) &&
		!((rKey.keysym.mod & KMOD_CAPS) && (rKey.keysym.mod & KMOD_SHIFT) )
		) {
		switch(rKey.keysym.sym) {
			//numbers
			case SDLK_1: m_sInput += '!'; break;
			case SDLK_2: m_sInput += '@'; break;
			case SDLK_3: m_sInput += '#'; break;
			case SDLK_4: m_sInput += '$'; break;
			case SDLK_5: m_sInput += '%'; break;
			case SDLK_6: m_sInput += '^'; break;
			case SDLK_7: m_sInput += '&'; break;
			case SDLK_8: m_sInput += '*'; break;
			case SDLK_9: m_sInput += '('; break;
			case SDLK_0: m_sInput += ')'; break;

			//punctuation
			case SDLK_BACKQUOTE:	m_sInput += '~'; break;
			case SDLK_MINUS:		m_sInput += '_'; break;
			case SDLK_EQUALS:		m_sInput += '+'; break;
			case SDLK_TAB:			m_sInput = "host 127.0.0.1"; break; //debug...
			case SDLK_LEFTBRACKET:	m_sInput += '{'; break;
			case SDLK_RIGHTBRACKET:	m_sInput += '}'; break;
			case SDLK_BACKSLASH:	m_sInput += '|'; break;
			case SDLK_SEMICOLON:	m_sInput += ':'; break;
			case SDLK_QUOTE:		m_sInput += '"'; break;
			case SDLK_COMMA:		m_sInput += '<'; break;
			case SDLK_PERIOD:		m_sInput += '>'; break;
			case SDLK_SLASH:		m_sInput += '?'; break;

			case SDLK_RETURN: break; //derpy

			//text
			default:
				if (isalpha(rKey.keysym.sym)) {
					m_sInput += rKey.keysym.sym + ('A' - 'a');
				}

				if (isspace(rKey.keysym.sym)) {
					m_sInput += rKey.keysym.sym;
				}
		}
		return;
	}

	//no recognised modifiers
	switch(rKey.keysym.sym) {
		//punctuation
		case SDLK_BACKQUOTE:	m_sInput += '`'; break;
		case SDLK_MINUS:		m_sInput += '-'; break;
		case SDLK_EQUALS:		m_sInput += '='; break;
		case SDLK_TAB:			m_sInput = "join 127.0.0.1"; break; //debug...
		case SDLK_LEFTBRACKET:	m_sInput += '['; break;
		case SDLK_RIGHTBRACKET:	m_sInput += ']'; break;
		case SDLK_BACKSLASH:	m_sInput += '\\'; break;
		case SDLK_SEMICOLON:	m_sInput += ';'; break;
		case SDLK_QUOTE:		m_sInput += '\''; break;
		case SDLK_COMMA:		m_sInput += ','; break;
		case SDLK_PERIOD:		m_sInput += '.'; break;
		case SDLK_SLASH:		m_sInput += '/'; break;

		case SDLK_RETURN: break; //derpy

		//text & numbers
		default:
			if (isalnum(rKey.keysym.sym) || isspace(rKey.keysym.sym)) {
				m_sInput += rKey.keysym.sym;
				break;
			}
			break;
	};
}

void SceneLobby::KeyUp(SDL_KeyboardEvent const& rKey) {
	//
}

//-------------------------
//Members
//-------------------------

void SceneLobby::Push(string s) {
	while (s.size() > 80) {//break into lines
		string tmp = s.substr(0, 80);
		s = s.c_str()+80;
		m_vLines.push_back(tmp);
	}

	m_vLines.push_back(s);

	if (m_vLines.size() > 26) {
		m_vLines.erase(m_vLines.begin(), m_vLines.end()-26);
	}
}

void SceneLobby::PushInstructions() {
	Push( "To setup your character edit \"player data\" in \"config.cfg\"" );
	Push( "Type \"refresh\" to update the server list" );
	Push( "Type \"host [ip]\" to host the server" );
	Push( "Type \"join [ip]\" to join the server" );
	Push( "Type \"help\" to see these instructions again" );
}

void SceneLobby::ClientCommands() {
	if (m_cd->index != -1) {
		//the client is connected

		if (m_sInput == "begin" && m_cd->host) {
			//add the bots to the packet list
			for (int i = 0; i < m_cd->config.Integer("host.bot.count"); i++) {
				string s;
				char buf[16];

				//get the bot index, indexed from 1: messy
				_itoa(i+1, buf, 10);
				s = "host.bot[";
				s += buf;
				s += "]";

				//fill the packet
				Packet p;
				p.type = CREATE_PLAYER;
				p.create.index = m_cd->vPacketList.size(); //the server usually sets this, but we don't want the server sending messages to the bots
				strcpy(p.create.szName, s.c_str());
				strcpy(p.create.szChar, m_cd->config[s].c_str() );

				//push the packet onto the list
				m_cd->vPacketList.push_back( p );
			}

			//fill the packet
			Packet p;
			p.type = GAME_SYNC;
			p.begin.playerCount = m_cd->vPacketList.size(); //clients + bots

			//send the "begin" packet: this bounces of the server before returning to the host
			m_cd->network.Send(m_cd->server, reinterpret_cast<Packet*>( &p ), sizeof(Packet));

			m_sInput = "";
			return;
		}

		//assume this is a message

		//fill the packet
		Packet p;

		p.type = MESSAGE;
		strcpy(p.message.szPayload, (m_cd->config["player.name"] + string(": ") + m_sInput).c_str());
		m_cd->network.Send(m_cd->server, &p, sizeof(Packet));
		m_sInput = "";
		return;
	}//if-connected

	if (m_sInput == "refresh") {

		Packet p;
		p.type = PING;
		strcpy_s(p.ping.szPayload, m_cd->config["player.name"].c_str());

		m_cd->network.Send(m_cd->broadcast, &p, sizeof(p));

		m_sInput.clear();

		Push("Searching for servers...");
		return;
	}

	//-------------------------

	if ( !strncmp(m_sInput.c_str(),"host ", 5) ) {
		Push( "Sending host request..." );

		//extract the ip
		IPaddress add;
		if (SDLNet_ResolveHost(&add, m_sInput.c_str()+5, m_cd->config.Integer("net.server.port"))) {
			//error
			Push("Invalid IP address");
			m_sInput.clear();
			return;
		}

		//fill the packet
		Packet p;
		p.type = REQUEST_HOST;
		p.create.index = -1;
		strcpy(p.create.szName, m_cd->config["player.name"].c_str());
		strcpy(p.create.szChar, m_cd->config["player.char"].c_str());

		m_cd->network.Send(add, &p, sizeof(p));

		m_sInput.clear();
		return;
	}

	//-------------------------

	if ( !strncmp(m_sInput.c_str(),"join ", 5) ) {
		Push( "Sending join request..." );

		//extract the ip
		IPaddress add;
		if (SDLNet_ResolveHost(&add, m_sInput.c_str()+5, m_cd->config.Integer("net.server.port"))) {
			//error
			Push("Invalid IP address");
			m_sInput.clear();
			return;
		}

		//fill the packet
		Packet p;
		p.type = REQUEST_JOIN;
		p.create.index = -1;
		strcpy(p.create.szName, m_cd->config["player.name"].c_str());
		strcpy(p.create.szChar, m_cd->config["player.char"].c_str());

		m_cd->network.Send(add, &p, sizeof(p));

		m_sInput.clear();
		return;
	}

	//-------------------------

	if (m_sInput == "help") {
		PushInstructions();
		m_sInput.clear();
		return;
	}

	//-------------------------

	m_sInput = "";
}

void SceneLobby::ServerCommands() {
	switch( reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() )->type) {
		case PING:
			scPing();
			break;

		case MESSAGE:
			//push this message to the screen
			Push( reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() )->message.szPayload );
			break;

		case CONFIRM_HOST:
			scConfirmHost();
			scPushPacket();
			break;

		case CONFIRM_JOIN:
			if (!m_cd->host) {
				scConfirmJoin();
			}
			else {
				scPushPacket();
			}
			break;

		case GAME_SYNC:
			SetNextScene(SCENE_SYNC);
			break;

		default:
			Push("Unknown network activity");
	};
}

//-------------------------
//Server Command List
//-------------------------

void SceneLobby::scPing() {
	string s;
	char buf[16];

	s += "Server: ";
	s += reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() )->ping.szPayload;

	//pad/tab
	for (unsigned int i = s.size(); i < 40; i++) s += ' ';

	ResolveIP(buf, m_cd->network.GetPacketIn()->address);

	s += buf;
	Push(s);
}

void SceneLobby::scConfirmHost() {
	Push("Host confirmed");

	//extract the data
	const Packet* ptr = reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() );
	m_cd->host = true;
	m_cd->index = ptr->create.index;
	m_cd->server = m_cd->network.GetPacketIn()->address;

	Push("You can now speak with the others in this server");
	Push("Type \"begin\" to start the game");
}

void SceneLobby::scConfirmJoin() {
	Push("Join confirmed");

	//extract the data
	const Packet* ptr = reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() );
	m_cd->host = false;
	m_cd->index = ptr->create.index;
	m_cd->server = m_cd->network.GetPacketIn()->address;

	Push("You can now speak with the others in this server");
}

void SceneLobby::scPushPacket() {
	//push a "player create" packet onto the list
	const Packet* ptr = reinterpret_cast<const Packet*>( m_cd->network.GetDataIn() );
	m_cd->vPacketList.push_back(  *ptr );
	m_cd->vPacketList.back().type = CREATE_PLAYER;
	Push("    >scPushPacket()"); //debugging
}

//-------------------------
//Utilities
//-------------------------

void SceneLobby::ResolveIP(char* buffer, IPaddress add) {
	sprintf(buffer, "%d.%d.%d.%d",
		(add.host >> 0 ) & 0xff,
		(add.host >> 8 ) & 0xff,
		(add.host >> 16 ) & 0xff,
		(add.host >> 24 ) & 0xff
		);
}
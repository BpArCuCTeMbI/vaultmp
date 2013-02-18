#pragma once

#include <d3d9.h>

#include <iostream>
#include <deque>
#include <string>


using namespace std;

struct globalData
{
	bool chatting;
	bool disableKeyboardInput;
	bool disableMouseInput;
	bool gameReady;
	bool textureHookingDone;
	bool hideChatbox;
	bool lockChatbox;

	int lastChatTextTick;
};

extern globalData gData;

struct remotePlayers
{
	char name[64];
	bool player;
	double health;
	double pos[3];
	double rot[3];
	long int pid;
};

extern remotePlayers *playersData;
extern deque<string> chatQueue;
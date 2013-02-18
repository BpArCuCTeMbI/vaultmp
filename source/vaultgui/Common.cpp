#include <Windows.h>
#include <iostream>
#include <fstream>

#include "common.h"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"

#include <sstream>

#include "myDirectDevice.h"


using namespace std;

void ResetLog()
{
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
	{
		PathAppend( szPath, "\\vmp_gui.log" );
		ofstream o(szPath,ios::out);
		o.close();
	}
}

void SendToLog(char* str)
{
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
	{
		PathAppend( szPath, "\\vmp_gui.log" );
		ofstream o(szPath,ios::out|ios::app);
		o<<str<<endl;
		o.flush();
		o.close();
	}

}

char* ExceptionToString(PEXCEPTION_RECORD p)
{
	static char buffer[2056];
	buffer[0]=0;

	std::stringstream ss;
	ss << "Exception Address:"<<hex<<"0x"<<(int)p->ExceptionAddress<<endl;
	const std::string s = ss.str();

	strcpy(buffer,s.c_str());

	return buffer;
}

bool GetScreenPosition(D3DXVECTOR2& ScreenPosition, float& Distance, D3DXVECTOR3 PositionOffset = D3DXVECTOR3(0,0,0))
{
	D3DXMATRIX m;
	D3DXVECTOR4 s;
	D3DXVECTOR3 w;

	w = D3DXVECTOR3(0,0,0) + PositionOffset;

	//VIEW * PROJECTION
	m = D3DXMATRIX(GameData::lastView) * D3DXMATRIX(GameData::lastProjection);

	//get vectors from view * proj
	s.x = w.x * m._11 + w.y * m._21 + w.z * m._31 + m._41;
	s.y = w.x * m._12 + w.y * m._22 + w.z * m._32 + m._42;
	s.z = w.x * m._13 + w.y * m._23 + w.z * m._33 + m._43;
	s.w = w.x * m._14 + w.y * m._24 + w.z * m._34 + m._44;

	//get inverse of w
	float w_inv = 1.0f / s.w;
	//get center of screen
	float fWidth2 = GameData::windowWidth * 0.5f;
	float fHeight2 = GameData::windowHeight * 0.5f;

	//get screen coordinates
	ScreenPosition.x = (1.0f + (s.x * w_inv)) * fWidth2;
	ScreenPosition.y = GameData::windowHeight - ((1.0f + (s.y * w_inv)) * fHeight2);
	//get distance to object from camera position
	Distance = s.z;

	return (s.z > 0.0f);
}

void RealityKeyDown(LPVOID ths, DWORD size, LPVOID data)
{
	MyDirectDevice* dev=(MyDirectDevice*)ths;

	if(memcmp(dev->dataFormat,&c_dfDIMouse,sizeof(DIDATAFORMAT)))
	{
		DIMOUSESTATE *state=(DIMOUSESTATE*)data;
		#ifdef USE_CEGUI
		if(gData.chatting)
			CEGUI::System::getSingleton().injectMouseMove(state->lX,state->lY);
#endif

		if(gData.disableMouseInput)
		{
			state->lX=0;
			state->lY=0;
		}
	}

	if(memcmp(dev->dataFormat,&c_dfDIMouse2,sizeof(DIDATAFORMAT)))
	{
		DIMOUSESTATE2 *state=(DIMOUSESTATE2*)data;
		#ifdef USE_CEGUI
		if(gData.chatting)
			CEGUI::System::getSingleton().injectMouseMove(state->lX,state->lY);
#endif

		if(gData.disableMouseInput)
		{
			state->lX=0;
			state->lY=0;
		}
	}
	
	if(memcmp(dev->dataFormat,&c_dfDIKeyboard,sizeof(DIDATAFORMAT)))
	{
		//if (diKeys[DIK_ESCAPE] & 0x80) DoSomething();
		if(gData.disableKeyboardInput)
		{
			memset(data,0,size);
		}
	}
}

void SimulateKeyDown(LPVOID ths, DWORD size, LPVOID data)
{
	MyDirectDevice* dev=(MyDirectDevice*)ths;

	if(memcmp(dev->dataFormat,&c_dfDIMouse,sizeof(DIDATAFORMAT)))
	{
		/*DIMOUSESTATE *state=(DIMOUSESTATE*)data;
		state->lX=0;
		state->lY=0;*/
	}

	if(memcmp(dev->dataFormat,&c_dfDIMouse2,sizeof(DIDATAFORMAT)))
	{
		/*DIMOUSESTATE2 *state=(DIMOUSESTATE2*)data;
		state->lX=0;
		state->lY=0;*/
	}
	
	if(memcmp(dev->dataFormat,&c_dfDIKeyboard,sizeof(DIDATAFORMAT)))
	{
		//if (diKeys[DIK_ESCAPE] & 0x80) DoSomething();
		//memset(data,0,size);
		/*if(TaskManager::taskQueue.size())
		{
			Task* tmp=&TaskManager::taskQueue[0];
			if(tmp->type==TASK_KEYPRESS)
			{
				((char*)data)[(*((int*)tmp->data))]=0x80;
				tmp->done=true;
			}
			else if(tmp->type==TASK_SETANGLE)
			{
				float* f=(float*)tmp->data;
				float* f2=(float*)(playerPointer+0x24);
				f2[0]=f[0];
				f2[1]=f[1];
				f2[2]=f[2];
				tmp->done=true;
			}
			else
			{
				tmp->done=true;
			}
		}*/

		/*wnd->setPosition(UVector2(cegui_reldim(0.01f), cegui_reldim( 0.01f)));
		wnd->setSize(UVector2(cegui_reldim(0.35f), cegui_reldim( 0.30f)));*/

		if(!gData.lockChatbox)
		{
			char* d=(char*)data;
			if (d[DIK_NUMPAD9] & 0x80 && d[DIK_LSHIFT] & 0x80)
			{
				CEGUI::FrameWindow *w = ((CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().getWindow("Main Window"));
				w->setPosition(CEGUI::UVector2(cegui_reldim(1.0f-0.35f-0.01f), cegui_reldim( 0.01f)));
			}
			if (d[DIK_NUMPAD8] & 0x80 && d[DIK_LSHIFT] & 0x80)
			{
				CEGUI::FrameWindow *w = ((CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().getWindow("Main Window"));
				w->setPosition(CEGUI::UVector2(cegui_reldim((1.0f-0.35f)/2-0.01f), cegui_reldim( 0.01f)));
			}
			if (d[DIK_NUMPAD7] & 0x80 && d[DIK_LSHIFT] & 0x80)
			{
				CEGUI::FrameWindow *w = ((CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().getWindow("Main Window"));
				w->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim( 0.01f)));
			}

			if (d[DIK_NUMPAD6] & 0x80 && d[DIK_LSHIFT] & 0x80)
			{
				CEGUI::FrameWindow *w = ((CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().getWindow("Main Window"));
				w->setPosition(CEGUI::UVector2(cegui_reldim(1.0f-0.35f-0.01f), cegui_reldim( (1.0f-0.30f)/2-0.01f)));
			}
			if (d[DIK_NUMPAD5] & 0x80 && d[DIK_LSHIFT] & 0x80)
			{
				CEGUI::FrameWindow *w = ((CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().getWindow("Main Window"));
				w->setPosition(CEGUI::UVector2(cegui_reldim((1.0f-0.35f)/2-0.01f), cegui_reldim( (1.0f-0.30f)/2-0.01f)));
			}
			if (d[DIK_NUMPAD4] & 0x80 && d[DIK_LSHIFT] & 0x80)
			{
				CEGUI::FrameWindow *w = ((CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().getWindow("Main Window"));
				w->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim( (1.0f-0.30f)/2-0.01f)));
			}

			if (d[DIK_NUMPAD3] & 0x80 && d[DIK_LSHIFT] & 0x80)
			{
				CEGUI::FrameWindow *w = ((CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().getWindow("Main Window"));
				w->setPosition(CEGUI::UVector2(cegui_reldim(1.0f-0.35f-0.01f), cegui_reldim( 1.0f-0.30f-0.01f)));
			}
			if (d[DIK_NUMPAD2] & 0x80 && d[DIK_LSHIFT] & 0x80)
			{
				CEGUI::FrameWindow *w = ((CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().getWindow("Main Window"));
				w->setPosition(CEGUI::UVector2(cegui_reldim((1.0f-0.35f)/2-0.01f), cegui_reldim( 1.0f-0.30f-0.01f)));
			}
			if (d[DIK_NUMPAD1] & 0x80 && d[DIK_LSHIFT] & 0x80)
			{
				CEGUI::FrameWindow *w = ((CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().getWindow("Main Window"));
				w->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim( 1.0f-0.30f-0.01f)));
			}
		}
	}

	//TaskManager::Update();
}

void CreateDevice(LPVOID ths, BOOL isKeyboard)
{

}
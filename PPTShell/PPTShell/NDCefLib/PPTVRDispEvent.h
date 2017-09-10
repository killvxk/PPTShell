#pragma once
#include <windows.h>


#define GLOBAL_EVENT_DISP_EXIT TEXT("Global\\EVENT_VRDISP_EXIT")      //PPT�˳�����VR�˳��¼�
#define GLOBAL_EVENT_DISP_START TEXT("Global\\EVENT_VRDISP_START")    //VR��ʼ�����¼�
#define GLOBAL_EVENT_DISP_STOP TEXT("Global\\EVENT_VRDISP_STOP")      //VRֹͣ�����¼�

enum PPTVRDispEvent_Type{
	EVENT_VRDISP_EXIT = 1,
	EVENT_VRDISP_START,
	EVENT_VRDISP_STOP,
	EVENT_101PPT_EXIT,
};

class CPPTVRDispEvent
{
public:
	CPPTVRDispEvent();
	~CPPTVRDispEvent();

public:
	bool OpenDispEvent(PPTVRDispEvent_Type nType);
	bool CreateDispEvent(PPTVRDispEvent_Type nType);
	HANDLE GetDispEvent(PPTVRDispEvent_Type nType);
	void SetDispEvent(PPTVRDispEvent_Type nType);
	void ReSetDispEvent(PPTVRDispEvent_Type nType);
	void SetExit();
	void CreateThreadSetEvent();
	void PPTRun();
	static DWORD WINAPI PPTVRDispThreadProc(LPVOID lpParameter);
private:
	HANDLE m_hEventExit;
	HANDLE m_hEventStop;
	HANDLE m_hEventStart;
	HANDLE m_hStopThreadEvent; 
};

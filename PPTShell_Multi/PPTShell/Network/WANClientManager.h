//===========================================================================
// FileName:				WANClientManager.h
// 
// Desc:				
//============================================================================
#ifndef _WAN_CLIENT_MANAGER_H_
#define _WAN_CLIENT_MANAGER_H_

#include "Util/Singleton.h"
#include "WANThread.h"

class CWANClientManager
{
private:
	CWANClientManager();
	~CWANClientManager();

public:
	struct SHeartBeatInfo
	{
		bool ifReConnect;		//�Ƿ�����
		SHeartBeatInfo():ifReConnect(false)
		{}
		SHeartBeatInfo(bool bReconn):ifReConnect(bReconn)
		{
			
		}
	};
	BOOL					Initialize(char* szWanServerIP, int nWanServerPort, DWORD dwPPTID);
	BOOL					Destroy();
	BOOL					SendPacket(SOCKET sock, char* pData, int nDataSize);
	BOOL					SetRecvCloseNotifyFunc(ON_PACKET_RECV_FUNC pPacketProcessFunc, ON_SOCKET_CLOSE_FUNC pSocketCloseFunc);

	void					OnMainWanClientPacket(SOCKET sock, int type, char* packet);
	void					OnWanClientPacket(SOCKET sock, int type, char* packet);
	void					OnWanClientClosed(SOCKET sock);

	DECLARE_SINGLETON_CLASS(CWANClientManager);

protected:
	CWANClient*				FindWanClient(DWORD dwUID);
	BOOL					MainClientConnect();
	static DWORD WINAPI		MainClientConnectThread(LPARAM lParam);

	CWANClient*				FindWanClient(SOCKET sockFind);
	bool					SleepInterrupt(time_t tStart, unsigned long ulSleepMS, bool& bBreak);
	
	//���������߳� ������Ҫ�������������ӵ������Լ���������
	static DWORD WINAPI		HeartbeatThread(LPARAM lParam);
	void DealHeartbeat();
	void					AddToHeartBeatList(CWANClient* client, bool bIfReconnect);
protected:
	char							m_szWanServerIP[MAX_PATH];
	int								m_nWanServerPort;
	DWORD							m_dwPPTID;

	CWANClient*						m_pMainClient;		// main 
	map<SOCKET, CWANClient*>		m_mapClients;		// SOCKET <--> CWANClient*
	map<DWORD, CWANClient*>			m_mapUIDClients;	// UID	  <--> CWANClient*
	CRITICAL_SECTION				m_Lock;

	map<CWANClient*, SHeartBeatInfo> m_mapHeartbeatClients;
	HANDLE					m_hEventHeartbeatStart;
	HANDLE					m_hThreadHeartbeat;
	HANDLE					m_hThreadMainClientConn;

	ON_PACKET_RECV_FUNC				m_pPacketProcessFunc;
	ON_SOCKET_CLOSE_FUNC			m_pSocketCloseFunc;

	bool			m_bCloseApp;		// �����Ƿ��˳�
	CWANThread		m_cWanThread;		// �߳���,��������WAN�ͻ��������շ�
};

typedef Singleton<CWANClientManager> WANClientManager;

#endif
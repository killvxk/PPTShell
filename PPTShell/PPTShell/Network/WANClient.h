//===========================================================================
// FileName:				WANClient.h
// 
// Desc:				
//============================================================================
#ifndef _WAN_CLIENT_H_
#define _WAN_CLIENT_H_

#define MAX_CLIENT_PACEKT_SIZE		1024*9

class CWANThread;

//-------------------------------------------------------------------
// WAN Client
//
enum
{
	WAN_NONE	= 0,
	WAN_MOBILE,
	WAN_PPTSHELL,
};

enum
{
	PT_TRANSFER				= 0xFFFF,
	PT_REGISTER				= 0xFFFE,
	PT_CLOSE_MOBILE			= 0xFFFD,
	PT_HEARTBEAT_RQST		=0xfffb,			//������
	PT_HEARTBEAT_ACK		=0xfffc	,		//������ Ӧ��
	PT_DISCONNECT				= 	0xfffa		//�Ͽ�����
};


typedef struct PACKET_REGISTER : PACKET_HEAD
{
	DWORD		dwClientType;			// 0: none 1: mobile / 2: PPTShell
	DWORD		dwMobileID;				// for mobile ID
	DWORD		dwPPTShellID;			// for PPTShell

} PACKET_REGISTER, *PPACKET_REGISTER;

//
// queue packet
//
struct QUEUE_PACKET
{
	DWORD		dwSize;
	char		szData[MAX_CLIENT_PACEKT_SIZE];
};

//
// notify functions
//
typedef void (*ON_PACKET_RECV_FUNC)(SOCKET sock, int type, char* packet);
typedef void (*ON_SOCKET_CLOSE_FUNC)(SOCKET sock);

class CWANClient
{
public:
	CWANClient();
	~CWANClient();

	BOOL				Initialize();
	BOOL				Destroy();

	BOOL				Connect(char* szServerIP, int nServerPort);
	BOOL				SetRecvCloseNotifyFunc(ON_PACKET_RECV_FUNC pPacketProcessFunc, ON_SOCKET_CLOSE_FUNC pSocketCloseFunc);
	BOOL				OnDataRecvd();				// �Ѳ�ʹ��
	void				CloseSocket();

	BOOL				SendRegisterPacket(DWORD dwMobileID, DWORD dwPPTShellID);
	BOOL				SendPacket(char* pData, int nDataSize);		// �Ѳ�ʹ��

	// threads
	void				ProcessRecvEvent();			// �Ѳ�ʹ��
	static DWORD WINAPI RecvThread(LPARAM lParam);	// �Ѳ�ʹ��
	int					ProcessPacket();

	void				ProcessSendEvent();			// �Ѳ�ʹ��
	static DWORD WINAPI SendThread(LPARAM lParam);	// �Ѳ�ʹ��

	SOCKET				GetSocket();
	
	//����������
	BOOL				SendHeartBeat();
	bool				IfRcvHeartBeatAck();

	DWORD				GetUID() { return m_dUID; }

	// ������յ������ݣ�������OnDataRecvd��󲿷���ͬ
	BOOL				DealRecvData();
	void	SetWanThread(CWANThread* pWanThread){ m_pWanThread = pWanThread; }
protected:
	BOOL				SendData(char* pData, int nDataSize);

	int					ThreadExit();

protected:
	SOCKET							m_ClientSocket;
	int								m_nConnectType;

	HANDLE							m_hRecvEvent;		// �Ѳ�ʹ��
	HANDLE							m_hSendEvent;		// �Ѳ�ʹ��
	HANDLE							m_hRecvThread;		// �Ѳ�ʹ��
	HANDLE							m_hSendThread;		// �Ѳ�ʹ��
	
	int								m_nRecvdDataSize;
	char							m_szRecvBuffer[MAX_CLIENT_PACEKT_SIZE];

	CRITICAL_SECTION				m_Lock;
	deque<QUEUE_PACKET*>			m_SendQueue;		// �Ѳ�ʹ��

	DWORD m_dUID;
	
	bool m_bHeartbeatRqst;			//�Ƿ���������
	int m_iHeartbeatTryCnt;

	ON_PACKET_RECV_FUNC				m_pPacketProcessFunc;
	ON_SOCKET_CLOSE_FUNC			m_pSocketCloseFunc;
	int m_iThreadDestroyCnt;

	friend class CWANThread;
	CWANThread* m_pWanThread;		// �߳���ָ��
};

#endif
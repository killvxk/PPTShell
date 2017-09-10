//
// WANClient���߳��࣬���ﴦ������WANClient�����ݽ��ա����Ͳ��������ս����ݴ���WANClient���д���
// ����һ����ɶ˿ڴ����̺߳�һ�����ݷ����̡߳�
// �����̴߳���ͻ��˹رպ����ݽ��ղ�����
// �����߳̽����Ͷ��е��������η��ͳ�ȥ��
//
#pragma once
#include "PacketProcess/Packet.h"
#include "WANClient.h"

// ��ɶ˿�OV�ṹ��
typedef struct _IOCPOVERLAPPED
{
	OVERLAPPED	stOverlapped;	// �첽IO
	WSABUF		stWsaBuf;		// �����ڴ�
	unsigned	nOperType;		// ������1=���գ�2=���ͣ�3=�Ͽ�

	_IOCPOVERLAPPED()
	{
		nOperType = 0;
		ZeroMemory(&stOverlapped, sizeof(stOverlapped));
	}
}IOCPOVERLAPPED, *PIOCPOVERLAPPED;

// ���Ͷ������ݽṹ�����ӿͻ���ָ��
typedef struct _QUEUE_PACKET_EX :QUEUE_PACKET
{
	CWANClient* pClient;
}QUEUE_PACKET_EX, *PQUEUE_PACKET_EX;


class CWANThread
{
public:
	CWANThread(void);
	~CWANThread(void);

	// ��ʼ��
	BOOL Initialize();
	// ��ȡ��ɶ˿ھ��
	inline HANDLE GetIOCompletionPort(){ return m_hIOCompletionPort; }
	// �󶨿ͻ�������ɶ˿�
	BOOL AssociateIOCP(CWANClient* pClient);

	// ���ӽ�������
	void PostRecv(CWANClient* pClient);
	// ������������ݵ����Ͷ���
	BOOL PostSend(CWANClient* pClient, char* pData, int nDataSize);		

	void Destroy();		// ��������Դ

	//void SetExitFlag(bool bExit){m_bCloseApp = bExit; Destroy();}
	//inline void PushDeleteClient(CWANClient* pDeleteClient);
	void SetMainClient(CWANClient* pMainClient){m_pMainClient = pMainClient;}
protected:
	// ���ӽ�������
	inline void PostRecv(CWANClient* pClient, PIOCPOVERLAPPED pIOCPInfo);
	// ������յ�������
	inline void DoRecv(CWANClient* pClient, PIOCPOVERLAPPED pIOCPInfo, DWORD dwTransferred);
	//void DoAccept();
	//void DoSend();
	//void DoCloseClient(CWANClient* pClient);

	// ��ɶ˿ڽ����̺߳����ݷ����߳�
	static DWORD WINAPI RecvThread(LPARAM lParam);
	static DWORD WINAPI SendThread(LPARAM lParam);

	// �Ƴ��ͻ���
	inline void RemoveClient(CWANClient* pClient);

	bool							m_bCloseApp;				// �����˳�

	HANDLE							m_hIOCompletionPort;		// ��ɶ˿ھ��
	unsigned						m_nRecvThreadNum;			// �����߳���/��ɶ˿��߳���
	vector<HANDLE>					m_vtRecvThreadHandle;		// �����߳̾��
	map<CWANClient*, IOCPOVERLAPPED*>	m_mapIOCPRecvInfo;		// �ͻ���-����OV
	CRITICAL_SECTION				m_csIOCPInfo;				// m_mapIOCPRecvInfo���ٽ���

	HANDLE							m_hSendThread;				// ���ݷ����߳�
	HANDLE							m_hSendEvent;				// �����߳��¼�
	deque<PQUEUE_PACKET_EX>			m_SendQueue;				// ���Ͷ���
	CRITICAL_SECTION				m_csSendSection;			// m_csSendSection/���Ͷ����ٽ���
	//map<CWANClient*, IOCPOVERLAPPED*>	m_mapIOCPSendInfo;		// �ͻ���-����OV

	//vector<CWANClient*>				m_vtDeleteClients;			// ���潫��ɾ���Ŀͻ���ָ��
	CWANClient*						m_pMainClient;				// PPTShell��PPTServer�����ӵ�Client
};

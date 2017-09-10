//===========================================================================
// FileName:				PacketProcessor.h
//	
// Desc:					 process packets
//============================================================================
#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include "Util/Singleton.h"
#include "DUI/Shape.h"

struct SOCK_PACKET
{
	SOCKET			sock;
	int				type;		// connect type	
	char*			buffer;
};

class CUser;
class CPacketProcessor
{
private:
	CPacketProcessor();
	~CPacketProcessor();

public:
	void Initialize();
	void Destroy();
	void OnProcessPacket(SOCKET sock, int connectType, char* pPacket);
	void OnSocketClosed(SOCKET sock);

	static DWORD WINAPI ProcessPacketThread(LPARAM lParam);
	void ProcessPacket();
	void ProcessOnePacket(SOCK_PACKET* pPacket);

	DECLARE_SINGLETON_CLASS(CPacketProcessor);

protected:
	// control PPT
	void PPTControl(SOCK_PACKET* pSockPacket);
	void ImageControl(SOCK_PACKET* pSockPacket);
	void ThreeDControl(SOCK_PACKET* pSockPacket);
	void LaserControl(SOCK_PACKET* pSockPacket, DWORD pkgLen);
	void BrushControl(SOCK_PACKET* pSockPacket, DWORD pkgLen);

	void SyncSlideImgRequest(SOCK_PACKET* pSockPacket);
protected:
	void ProcessActionUser(SOCK_PACKET* pSockPacket);
	void ProcessActionControl(SOCK_PACKET* pSockPacket);
	void ProcessActionTransferFile(SOCK_PACKET* pSockPacket);
	void ProcessActionPushData(SOCK_PACKET* pSockPacket);
	void ProcessActionPullData(SOCK_PACKET* pSockPacket);
	void ProcessActionSyncData(SOCK_PACKET* pSockPacket);

	BOOL TransferFileCheckExists(SOCK_PACKET *pSockPacket);
protected:
	BOOL PlayEffect(int nEffectType);
	BOOL ExecCalculator();
	BOOL KillCalculator();
	BOOL ControlCalcTop();//�����ö���ʾ�Ѿ��򿪵ļ�����2015.12.21 cws
protected:
	HANDLE					m_hEvent;
	HANDLE					m_hProcessThread;	// �����߳̾�� 
	CLineShape*				m_pNewLineShape;//һ�ζ�����shape
	BOOL					m_bCloseApp;		// �����Ƿ��˳�
	CRITICAL_SECTION		m_cs;
	deque<SOCK_PACKET*>		m_ProcessDeque;	 
};

typedef Singleton<CPacketProcessor> PacketProcesser;

#endif
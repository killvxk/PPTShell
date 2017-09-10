//===========================================================================
// FileName:				PacketProcessor.cpp
//	
// Desc:					 process packets
//============================================================================
#include "stdafx.h"
#include "PPTShell.h"
#include "Util/Util.h"
#include "Util/Stream.h"
#include "Util/Singleton.h"
#include "PacketStream.h"
#include "PPTControl/PPTController.h"
#include "PPTControl/PPTControllerManager.h"
#include "PPTControl/PPTInfoSender.h"
#include "FileTransfer/FileTransferTask.h"
#include "FileTransfer/FileTransferManager.h"
#include "PacketProcessor.h"
#include "DUI/LaserPointerDlg.h"
#include "DUI/ImageCtrlDialogUI.h"
#include "NDCloud/NDCloudUser.h"
#include "gui/MainFrm.h" 
#include "DUI/MobilePenView.h" 
#include "DUI/MagicPen_Main.h"
#include "DUI/BlackBoarder.h"

#define DIRECTORY_MOBILE_RECV_FILE			_T("RecvFiles")
void GenReceiveFilePath(const tstring& strFileName, tstring &strFullName);

CPacketProcessor::CPacketProcessor()
{ 
	m_pNewLineShape = NULL;
}

CPacketProcessor::~CPacketProcessor()
{
	m_bCloseApp = TRUE;

	SetEvent(m_hEvent);
	if(m_hProcessThread != NULL)
		WaitForSingleObject(m_hProcessThread, 5000);

	DWORD dwExitCode;
	if(GetExitCodeThread(m_hProcessThread, &dwExitCode))
	{
		if(dwExitCode == STILL_ACTIVE)
			TerminateThread(m_hProcessThread, 0);
	}

	CloseHandle(m_hEvent); 
}

void CPacketProcessor::Initialize()
{
	m_bCloseApp = FALSE;
	m_hProcessThread = NULL;

	InitializeCriticalSection(&m_cs);
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hProcessThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ProcessPacketThread, this, 0, NULL);
}

void CPacketProcessor::Destroy()
{

}

void CPacketProcessor::OnProcessPacket(SOCKET sock, int connectType, char *pPacket)
{
	PACKET_HEAD* pHeader = (PACKET_HEAD*)pPacket;
	if( pHeader->dwSize > 0xffff )
		return;
	

	// add to process deque
	SOCK_PACKET* p = new SOCK_PACKET;

	p->sock = sock;
	p->type = connectType;
	p->buffer = new char[pHeader->dwSize+1];

	memset(p->buffer, 0, pHeader->dwSize+1);
	memcpy(p->buffer, pPacket, pHeader->dwSize);


	EnterCriticalSection(&m_cs);
	m_ProcessDeque.push_back(p);
	LeaveCriticalSection(&m_cs);

	SetEvent(m_hEvent);
}

void CPacketProcessor::OnSocketClosed(SOCKET sock)
{
	// notify leader
	CPPTShellApp* pApp = (CPPTShellApp*)AfxGetApp();
	CUserManager* pUserManager = UserManager::GetInstance();

	if( pApp == NULL || pUserManager == NULL )
		return;
	
	CUser* pUser = pUserManager->FindUser(sock);
	if( pUser == NULL )
		return;

	CUser* pLeader = pUserManager->GetLeader();
	if( pLeader == NULL || pUser == pLeader || !pLeader->IsOnline() )
		return;

	CPacketStream GuestLogoffStream;
	GuestLogoffStream.PackGuestLogoffResponse(pUser->GetUid());
	pApp->SendStreamPacket(pLeader->GetSocket(), &GuestLogoffStream, pLeader->GetConnectType());
	
	pUserManager->UserLogoff(pUser->GetUid());
}

void CPacketProcessor::ProcessPacket()
{
	while( TRUE )
	{
		DWORD ret = WaitForSingleObject(m_hEvent, INFINITE);
		if(m_bCloseApp)
			return;
		if( ret != 0 )
			continue;

		deque<SOCK_PACKET*> packetDeque;

		EnterCriticalSection(&m_cs);
		m_ProcessDeque.swap(packetDeque);
		LeaveCriticalSection(&m_cs);


		deque<SOCK_PACKET*>::iterator itr;
		for(itr = packetDeque.begin(); itr != packetDeque.end(); itr++)
		{
			SOCK_PACKET* packet = *itr;
			ProcessOnePacket(packet);

			if( packet->buffer != NULL )
			{
				delete packet->buffer;
				packet->buffer = NULL;
			}

			if( packet != NULL )
			{
				delete packet;
				packet = NULL;
			}
		}
		
		time_t tmStart = time(NULL);
		while(true)
		{
			if(m_bCloseApp)
				return;

			//180�����������Ϣ��תȥ����
			if (m_ProcessDeque.size()>0)
			{
				break;
			}

			//180���������û����Ϣ�͹���
			if (time(NULL)-tmStart>180)
			{
				break;
			}

			Sleep(1);
		}
	}
}

DWORD WINAPI CPacketProcessor::ProcessPacketThread(LPARAM lParam)
{
	CPacketProcessor* pThis = (CPacketProcessor*)lParam;
	if( pThis == NULL )
		return 0;

	pThis->ProcessPacket();

	return 1;
}

//
// process one packet
//
void CPacketProcessor::ProcessOnePacket(SOCK_PACKET* pPacket)
{
	PACKET_HEAD* pHeader = (PACKET_HEAD*)pPacket->buffer;
	if( pHeader == NULL )
		return;
 
	switch( pHeader->dwMajorType )
	{
	case ACTION_USER:
		ProcessActionUser(pPacket);
		break;

	case ACTION_CONTROL:
		ProcessActionControl(pPacket);
		break;

	case ACTION_PUSH_DATA:
		ProcessActionPushData(pPacket);
		break;

	case ACTION_PULL_DATA:
		ProcessActionPullData(pPacket);
		break;

	case ACTION_SYNC_DATA:
		ProcessActionSyncData(pPacket);
		break;

	case ACTION_TRANSFER_FILE:
		ProcessActionTransferFile(pPacket);
		break;
	case  MOBILE_HEARTBEAT:
		WRITE_LOG_LOCAL(_T("Receive a heartbeat package from ��%d��(1:ios, 2:android)"), pHeader->dwMinorType);
		break;
	default:
		WRITE_LOG_LOCAL(_T("Recieve a unrecognize package! majortype:%d"), pHeader->dwMajorType);
		break;
	}
}

//
// process user action
//
void CPacketProcessor::ProcessActionUser(SOCK_PACKET* pSockPacket)
{
	CPPTShellApp* pApp = (CPPTShellApp*)AfxGetApp();
	CUserManager* pUserManager = UserManager::GetInstance();

	if( pApp == NULL || pUserManager == NULL )
		return;

	PACKET_HEAD* pHeader = (PACKET_HEAD*)pSockPacket->buffer;
	if( pHeader == NULL )
		return;

	switch( pHeader->dwMinorType )
	{
	case USER_LOGON:
		{
			DWORD dwUID;
			tstring strUserName;
			BOOL bMobileLogin;

			CPacketStream LogonStream(pSockPacket->buffer);
			LogonStream.UnPackUserLogon(dwUID, strUserName, bMobileLogin);

			CUser* pUser = pUserManager->UserLogin(pSockPacket->sock, dwUID, pSockPacket->type, (TCHAR *)strUserName.c_str());
			if( pUser == NULL )
				return;

			WRITE_LOG_LOCAL(_T("UserLogin sock:%u, uid:%u, username:%s"), pSockPacket->sock, dwUID, strUserName.c_str());

			// send response to this user
			CPacketStream LogonResponseStream;
			LogonResponseStream.PackUserLogonResponse(pUser->IsLeader(), pUser->GetPermission());
			pApp->SendStreamPacket(pSockPacket->sock, &LogonResponseStream, pSockPacket->type);

			// send pc information to this user
			int nScreenWidth, nScreenHeight;
			GetScreenResolution(nScreenWidth, nScreenHeight);
			// ��ȡConfig.ini�ı����༶���ƣ����Ϊ�����ȡ�������
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
			tstring strNodeName = pMainFrame->GetConfigClassName();
			if(strNodeName.empty())
				strNodeName = GetPCName();

			CPacketStream PCInfoStream;
			PCInfoStream.PackPushPCInfo(strNodeName, nScreenWidth, nScreenHeight);
			pApp->SendStreamPacket(pSockPacket->sock, &PCInfoStream, pSockPacket->type);

			// send PPT basic information and outline and slide images
			CPPTInfoSender* pPPTInfoSender = PPTInfoSender::GetInstance();
			pPPTInfoSender->SendBasicInfo(pSockPacket->sock, pSockPacket->type);
//			pPPTInfoSender->SendPPTOutLine(pSockPacket->sock, pSockPacket->type);
//			pPPTInfoSender->SendSlideThumb(pSockPacket->sock, pSockPacket->type);
			pPPTInfoSender->SendPlayBegin(pSockPacket->sock, pSockPacket->type);
			pPPTInfoSender->SendPlaySlide(pSockPacket->sock, pSockPacket->type);

			pPPTInfoSender->StartExportThumbnails();		//ʵ����ֻ�е�һ�ε�½�Żᴥ����������ͼ
			
			CPPTController* pController = GetPPTController();
			if( pController != NULL )
			{
				BOOL bRet = pController->IsChanged(FALSE);
				if (bRet)
				{
					pPPTInfoSender->SendSyncDataPPTChanged(pSockPacket->sock, pSockPacket->type);
					WRITE_LOG_LOCAL(_T("user logon, find ppt is changed, send notice to user...."));
//					pController->Save();
// 					CStream* pTmpStream	= new CStream(50);
// 					pTmpStream->WriteDWORD(PPTC_SYNC_CHANGE);
// 					BroadcastPostedEvent(EVT_THREAD_CONTROL_PPT, (WPARAM)pTmpStream, 0, NULL);
				}

				if (!pController->IsPPTShowViewActive())
				{
					int iCurrSlideIdx = pController->GetActiveSlideIndex();
					pPPTInfoSender->SendPreviewSlideIdx(pSockPacket->sock, pSockPacket->type, iCurrSlideIdx);
				}
				if(pController->IfPPTSlideShowView())
				{
					// ������ڷ�ӳPPT��ͬ����Ƶ��Ϣ���ֻ���
					GetVideoListByThread();
				}
			}

			//�ֻ��Զ���¼
			if (bMobileLogin)
			{
				time_t tNow = time(NULL);
				CNDCloudUser *pCloudUser = NDCloudUser::GetInstance();
				if (pCloudUser && !pCloudUser->IsLogin() && (tNow-pCloudUser->GetLastMobileLoginRqstTime())>=60)
				{
					CPacketStream pkgStream;
					pkgStream.PackCommand(ACTION_USER, NDUSER_REQUEST_LOGON_INFO);
					pApp->SendStreamPacket(pSockPacket->sock, &pkgStream, pSockPacket->type);
					pCloudUser->SetMobileLoginRqstTime(tNow);
				}
			}

// 			CStream* pTmpStream	= new CStream(10);
// 			BroadcastPostedEvent(EVT_MOBILE_LOGIN, (WPARAM)pTmpStream, 0, NULL);

			// leader
			CUser* pLeader = pUserManager->GetLeader();
			if( pLeader == NULL )
				return;

			// send all the other user informations to leader
			if( pUser == pLeader )
			{
				map<DWORD, CUser*> mapUsers = pUserManager->GetUsers();
				map<DWORD, CUser*>::iterator itr;

				for(itr = mapUsers.begin(); itr != mapUsers.end(); itr++)
				{
					pUser = itr->second;
					if( pUser != NULL && pUser != pLeader && pUser->IsOnline() )
					{
						// guest logon packet
						CPacketStream GuestLogonStream;
						GuestLogonStream.PackGuestLogonResponse(pUser->GetUid(), pUser->GetPermission(), pUser->GetName());
						pApp->SendStreamPacket(pLeader->GetSocket(), &GuestLogonStream, pLeader->GetConnectType());
					}
				}
			}
			else
			{
				// send to leader
				if( !pLeader->IsOnline() )
					return;

				CPacketStream GuestLogonStream;
				GuestLogonStream.PackGuestLogonResponse(pUser->GetUid(), pUser->GetPermission(), pUser->GetName());
				pApp->SendStreamPacket(pLeader->GetSocket(), &GuestLogonStream, pLeader->GetConnectType());
			}

		}
		break;

	case USER_LOGOFF:
		{
			
		}
		break;

	case USER_MODIFY_NAME:
		{
			DWORD dwUID;
			tstring strName;

			CPacketStream ModifyNameStream(pSockPacket->buffer);
			ModifyNameStream.UnPackModifyName(dwUID, strName);

			CUser* pUser = pUserManager->FindUser(dwUID);
			if( pUser == NULL )
				return;

			pUser->SetName((TCHAR *)strName.c_str());

			// send user info to leader
			CUser* pLeader = pUserManager->GetLeader();
			if( pLeader != NULL && pLeader != pUser && pLeader->IsOnline() )
			{
				CPacketStream GuestModifyNameStream;
				GuestModifyNameStream.PackGuestModifyName(dwUID, strName);

				pApp->SendStreamPacket(pLeader->GetSocket(), &GuestModifyNameStream, pLeader->GetConnectType());
			}
		}
		break;

	case USER_MODIFY_PERMISSION:
		{ 
			DWORD dwUID;
			DWORD dwPermission;

			CPacketStream ModifyPermissionStream(pSockPacket->buffer);
			ModifyPermissionStream.UnPackModifyPermission(dwUID, dwPermission);

			CUser* pUser = pUserManager->FindUser(dwUID);
			if( pUser == NULL )
				return;

			pUser->SetPermission(dwPermission);

			// send user info 
			if( pUser->IsOnline() )
				pApp->SendStreamPacket(pUser->GetSocket(), &ModifyPermissionStream, pUser->GetConnectType());
		}
		break;
	case NDUSER_LOGON_INFO:
		{
			char *pData = (char *)pHeader+sizeof(PACKET_HEAD);
			CNDCloudUser *pCloudUser = NDCloudUser::GetInstance();
			if (pCloudUser==NULL || pCloudUser->IsLogin())
			{
				WRITE_LOG_LOCAL(_T("Reveive dumplicate NDUSER_LOGON_INFO!"));
				return;
			}

			int iLen = pHeader->dwSize - sizeof(PACKET_HEAD);
			CStream* pTmpStream	= new CStream(iLen+10);
			pTmpStream->Write(pData, iLen);
			BroadcastPostedEvent(EVT_MOBILE_LOGIN, (WPARAM)pTmpStream, 0, NULL);
		}
		break;
	}
}

//
// process control action
//
void CPacketProcessor::ProcessActionControl(SOCK_PACKET* pSockPacket)
{
	PACKET_HEAD* pHeader = (PACKET_HEAD*)pSockPacket->buffer;
	if( pHeader == NULL )
		return;

	CPPTShellApp* pApp = (CPPTShellApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
	if(pMainFrame!=NULL && pMainFrame->IsPPTOpening())
	{
		// ����Ӧ��
		CPacketStream streamError;
		streamError.PackControlResponse(pHeader->dwMinorType, FALSE, _T("PC�����ڴ�PPT���޷�����ָ��"));
		pApp->SendStreamPacket(pSockPacket->sock, &streamError, pSockPacket->type);

		WRITE_LOG_LOCAL(_T("Recv PPTControl Action When PPT Opening, cmd is %d-%d"), pHeader->dwMajorType, pHeader->dwMinorType);
		return;
	}

	switch( pHeader->dwMinorType )
	{
	case CONTROL_M2P_PPT:
		PPTControl(pSockPacket);
		break;

	case CONTROL_M2P_LASER:
		LaserControl(pSockPacket, pHeader->dwSize);
		break;
	case CONTROL_M2P_BRUSH:
		BrushControl(pSockPacket, pHeader->dwSize);
		break;
	case CONTROL_M2P_3D:
		ThreeDControl(pSockPacket);
		break;

	case CONTROL_M2P_IMAGE:
		ImageControl(pSockPacket);
		break;

#ifdef DEVELOP_VERSION
	case CONTROL_M2P_WHITE_BOARD:
		BlackBoardControl(pSockPacket);
		break;
#endif

	case CONTROL_M2P_MAGNIFIER:
		MagnifierControl(pSockPacket);
		break;
#ifdef DEVELOP_VERSION
	case CONTROL_M2P_SPOTLIGHT:
		SpotlightControl(pSockPacket);
		break;
#endif
	}
}

void CPacketProcessor::LaserControl(SOCK_PACKET* pSockPacket, DWORD pkgLen)
{
	CPPTController* pController = GetPPTController();
	if (pController == NULL)
	{
		return ;
	}

	CPacketStream laserControlStream(pSockPacket->buffer);
	laserControlStream.SkipHeader();

	DWORD laserType = laserControlStream.ReadDWORD();
	switch(laserType)
	{
	case LASER_MOVE:
		{
			int iPointCnt = (pkgLen-sizeof(PACKET_HEAD))/8;
			if (iPointCnt<=0)
			{
				WRITE_LOG_LOCAL(_T("CPacketProcessor::LaserControl, invalid message!"));
				return ;
			}
			for (int i=0; i<iPointCnt; i++)
			{
				float ptX = laserControlStream.ReadFloat();
				float ptY = laserControlStream.ReadFloat();
				
				
				pController->SlidePosToScreenPos(ptX, ptY);
				clock_t tclk = clock();
				logToOutput(_T("CPacketProcessor::LaserControl,posX:%f, posY:%f----clock:%ld"), ptX, ptY, tclk);
				LaserPointerDlg::GetInstance()->Show((int)ptX, (int)ptY);
				tclk = clock();
				logToOutput(_T("CPacketProcessor::LaserControl--------------show done----clock:%ld"), tclk);
				SleepRunNull(6);
			}
		}
		break;
	case LASER_END:
		LaserPointerDlg::GetInstance()->Hide();
		break;
	default:
		break;
	}

}

void CPacketProcessor::BrushControl(SOCK_PACKET* pSockPacket, DWORD pkgLen)
{
	CPacketStream brushControlStream(pSockPacket->buffer);
	brushControlStream.SkipHeader();
	
	CPPTController* pController = GetPPTController();
	if (pController==NULL)
	{
		WRITE_LOG_LOCAL(_T("CPacketProcessor::BrushControl, GetPPTController fail!"));
		return ;
	}
	
	DWORD brushType = brushControlStream.ReadDWORD();

	if(!pController->IfPPTSlideShowView())
	{
		return ;
	}
	
	switch(brushType)
	{
	case BRUSH_COLOR:
		{
			DWORD color = brushControlStream.ReadDWORD();
			DWORD red = color&0x00ff0000;
			DWORD green = color&0x0000ff00;
			DWORD blue = color&0x000000ff;
			DWORD colorDest = (blue<<16) | green | (red>>16);
			pController->SetPenColor(colorDest);
		}
		break;
	case BRUSH_STYLE:
		break;
	case BRUSH_WIDTH:
		break;
	case BRUSH_MOVE:
		{

//			WRITE_LOG_LOCAL("--------------new BRUSH_MOVE---------------");
			int iDataLen = pkgLen-sizeof(PACKET_HEAD)-4;
			int iPointCnt = iDataLen/8;
			if ( iDataLen%8 != 0)
			{
				WRITE_LOG_LOCAL(_T("CPacketProcessor::BrushControl, MOVE Data error!"));
				return ;
			}	

			float fLastX = -1;
			float fLastY = -1;
			bool bSetMouseDown = false;
				
			for (int i=0; i<iPointCnt; i++)
			{
				float fCurrX = brushControlStream.ReadFloat();
				float fCurrY = brushControlStream.ReadFloat(); 
				//WRITE_LOG_LOCAL("mobile send[%d]:%f,%f\n",i, fCurrX, fCurrY);

				if (fCurrX==-2 || fCurrY==-2)
				{ 
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); //�����ʱ����ȥ����ȥ����������һ������û�л��� 2016.03.04 cws
 
					MobilePenViewUI::GetInstance()->AddShapes(m_pNewLineShape);
					break;
				}

				if (fCurrX==-1 || fCurrY ==-1)
				{ 
					m_pNewLineShape = NULL;					
					m_pNewLineShape = new CLineShape;

					DWORD dwCurrentColor = RGB(234,55,68);
					dwCurrentColor  &= 0x00FFFFFF;
					if(dwCurrentColor == 0)
					{
						dwCurrentColor = 0x1;
					}
					dwCurrentColor = RGB(GetBValue(dwCurrentColor), GetGValue(dwCurrentColor), GetRValue(dwCurrentColor));
					m_pNewLineShape->SetColor(dwCurrentColor);
					m_pNewLineShape->SetWidth(5);

					continue;
				}
 
				pController->SlidePosToScreenPos(fCurrX, fCurrY); 
			
				POINT ptMouse;
				ptMouse.x = (int)fCurrX;
				ptMouse.y = (int)fCurrY;
				//WRITE_LOG_LOCAL("mobile input:%d,%d\n", ptMouse.x,ptMouse.y);
				m_pNewLineShape->AddPoint(ptMouse); 
			}
		}
		break;
	case BRUSH_UNDO:
		{

		}
		break;
	case BRUSH_CLEAR:
		{
			MobilePenViewUI::GetInstance()->ClearPanel();
			//pController->PenEraseDrawing();
		}
		break;
	case BRUSH_BEGIN:
		{ 
			CScreenInstrumentUI* pScreenIns = CScreenInstrumentUI::GetMainInstrument(); 
			if(pScreenIns != NULL)
				PostMessage(pScreenIns->GetHWND(), WM_SHOWMOBILEPEN, 0,0);
		}
		break;
	case BRUSH_END:
		{ 
			CScreenInstrumentUI* pScreenIns = CScreenInstrumentUI::GetMainInstrument(); 
			if(pScreenIns != NULL)
				PostMessage(pScreenIns->GetHWND(), WM_HIDEMOBILEPEN, 0,0);
		}
		break;
	case BRUSH_SAVE:
		{
			HWND hdPrompt = ::FindWindow(_T("#32770"), _T("Microsoft PowerPoint"));
			if(hdPrompt!=NULL)
			{
				HWND hdSave = ::FindWindowEx(hdPrompt, NULL, _T("Button"), _T("����(&K)"));
				PostMessage(hdSave, BM_CLICK, 0, 0);
			}
		}
		break;
	case  BRUSH_CANCEL_SAVE:
		{
			HWND hdPrompt = ::FindWindow(_T("#32770"), _T("Microsoft PowerPoint"));
			if (hdPrompt != NULL)
			{
				HWND hdSave = ::FindWindowEx(hdPrompt, NULL, _T("Button"), _T("����(&D)"));
				PostMessage(hdSave, BM_CLICK, 0, 0);
			}
		}
		break;
	default:
		break;
	}

}
void CPacketProcessor::MagnifierControl(SOCK_PACKET* pSockPacket)
{
	// PPTδ��ӳʱ��ʹ�÷Ŵ�
	CPPTController* pController = GetPPTController();
	if(pController!=NULL && !pController->IfPPTSlideShowView())
	{
		// δ��ӳʱ�յ��Ŵ�����
		return;
	}

	DWORD dwSize;
	DWORD dwCommand;

	CPacketStream stPacket(pSockPacket->buffer);
	stPacket.UnPackMagnifier(dwSize, dwCommand);

	// ֻ����һ���ֻ��˶˲���
	static SOCKET sockControl = INVALID_SOCKET;
	if(MagicPen_MainUI::GetInstance()->IsShowing() || MagicPen_MainUI::GetInstance()->IsMagnifierShowing())
	{
		// ����ֻ���δ���͹��������Ϊ��ǰ�ֻ����ǲ����ˣ����������ֻ��˵�����
		if(sockControl == INVALID_SOCKET)
			sockControl = pSockPacket->sock;
	}
	else
	{
		sockControl = INVALID_SOCKET;
	}
	if(sockControl != INVALID_SOCKET && sockControl != pSockPacket->sock)
	{
		// �������ֻ��˻�PC�����ڿ���ʱ���������β���
		CPacketStream packRespone;
		packRespone.PackControlResponse(dwCommand, FALSE, _T("����������ʹ�÷Ŵ󾵣��޷������β���"));
		CPPTShellApp* pApp = (CPPTShellApp*)AfxGetApp();
		pApp->SendStreamPacket(pSockPacket ->sock, &packRespone, pSockPacket->type);
		return;
	}

	try{
		switch(dwCommand)
		{
		case MAGNIFIER_OPEN:
			{
				// �򿪷Ŵ󾵣����Ŵ��Ѿ���ʱ�����ٷ�����Ϣ
				WRITE_LOG_LOCAL(_T("MAGNIFIER_OPEN"));
				sockControl = pSockPacket->sock;
				if(!MagicPen_MainUI::GetInstance()->IsShowing() && !MagicPen_MainUI::GetInstance()->IsMagnifierShowing())
				{
					CScreenInstrumentUI* pScreenIns = CScreenInstrumentUI::GetMainInstrument(); 
					if(pScreenIns != NULL)
						PostMessage(pScreenIns->GetHWND(), WM_SHOWMAGICPEN, 0,0);
					else
						WRITE_LOG_LOCAL(_T("Maginifier Open, GetScreenInstrument Failed"));
				}
			}
			break;
		case MAGNIFIER_TOGGLE:
			{
				// �رշŴ�
				WRITE_LOG_LOCAL(_T("MAGNIFIER_TOGGLE"));
				sockControl = INVALID_SOCKET;
				CScreenInstrumentUI* pScreenIns = CScreenInstrumentUI::GetMainInstrument(); 
				if(pScreenIns != NULL)
					PostMessage(pScreenIns->GetHWND(), WM_HIDEMAGICPEN, 0,0);
				else
					WRITE_LOG_LOCAL(_T("Maginifier Close, GetScreenInstrument Failed"));
			}
			break;

		case MAGNIFIER_MOVE:
			{
				// �϶��Ŵ󾵵ĵ㣬(-1,-1)�����ʼ�����֮꣬��Ķ����ƶ���/��ֹ��ĵ����꣬��ͼ�����½���(-2,-2)ǰһ����
				// PC�����ڲٿ�ʱ���Ա�������
				int nControlMode = MagicPen_MainUI::GetInstance()->GetControlMode();
				if(nControlMode == 1)
					break;

				int iDataLen = dwSize-sizeof(PACKET_HEAD)-4;
				int iPointCnt = iDataLen/8;
				if ( iDataLen%8 != 0)
				{
					WRITE_LOG_LOCAL(_T("CPacketProcessor::MagnifierControl, MOVE Data error!"));
					return ;
				}	

				float fLastX = -1;
				float fLastY = -1;
				bool bGetStartPoint = false;			// true - ��һ��������ʼ��
				static POINT ptMouse;					// ����������

				for (int i=0; i<iPointCnt; i++)
				{
					float fCurrX = stPacket.ReadFloat();
					float fCurrY = stPacket.ReadFloat(); 
					//WRITE_LOG_LOCAL(_T("mobile send[%d]:%f,%f"),i, fCurrX, fCurrY);

					if (fCurrX==-1 || fCurrY ==-1)
					{ 
						WRITE_LOG_LOCAL(_T("1 mobile send[%d]:%f,%f"),i, fCurrX, fCurrY);
						// ��һ��������ʼ��
						bGetStartPoint = true;
						continue;
					}
					else if (fCurrX==-2 || fCurrY==-2)
					{ 
						WRITE_LOG_LOCAL(_T("2 mobile send[%d]:%f,%f"),i, fCurrX, fCurrY);
						// �ͷŵ�
						POINT* ptPost = new POINT;
						ptPost->x = ptMouse.x;
						ptPost->y = ptMouse.y;
						CScreenInstrumentUI* pScreenIns = CScreenInstrumentUI::GetMainInstrument(); 
						if(pScreenIns != NULL)
							PostMessage(pScreenIns->GetHWND(), WM_MOVEMAGICPEN, 2, (LPARAM)ptPost);
						else
							WRITE_LOG_LOCAL(_T("Maginifier Move Finish, GetScreenInstrument Failed"));
						break;
					}

					// ��ʵ����
					pController->SlidePosToScreenPos(fCurrX, fCurrY); 
					ptMouse.x = (int)fCurrX;
					ptMouse.y = (int)fCurrY;

					POINT* ptPost = new POINT;
					//if(ptPost != NULL && !bGetStartPoint)
					//	continue;
					ptPost->x = ptMouse.x;
					ptPost->y = ptMouse.y;
					if(bGetStartPoint)
					{
						CScreenInstrumentUI* pScreenIns = CScreenInstrumentUI::GetMainInstrument(); 
						if(pScreenIns != NULL)
						{
							bGetStartPoint = false;
							PostMessage(pScreenIns->GetHWND(), WM_MOVEMAGICPEN, 0, (LPARAM)ptPost);
						}
						else
						{
							WRITE_LOG_LOCAL(_T("Maginifier Move Start, GetScreenInstrument Failed"));
						}
					}
					else
					{
						CScreenInstrumentUI* pScreenIns = CScreenInstrumentUI::GetMainInstrument(); 
						if(pScreenIns != NULL)
							PostMessage(pScreenIns->GetHWND(), WM_MOVEMAGICPEN, 1, (LPARAM)ptPost);
						else
							WRITE_LOG_LOCAL(_T("Maginifier Moving, GetScreenInstrument Failed"));
					}
				}
			}
			break;
		}

	}catch(...)
	{
		WRITE_LOG_LOCAL(_T("Catch Exception At MagnifierControl, Command=%d"), dwCommand);
	}
}
void CPacketProcessor::SpotlightControl(SOCK_PACKET* pSockPacket)
{
	// PPTδ��ӳʱ��ʹ�þ۹��
	CPPTController* pController = GetPPTController();
	if(pController!=NULL && !pController->IfPPTSlideShowView())
	{
		// δ��ӳʱ�յ��۹������
		return;
	}

	CPacketStream stPacket(pSockPacket->buffer);
	stPacket.SkipHeader();
	DWORD dwCommand;
	dwCommand = stPacket.ReadDWORD();

	CScreenInstrumentUI* pScreenIns = CScreenInstrumentUI::GetMainInstrument(); 
	if(pScreenIns == NULL)
		return;
	CSpotLightWindowUI * pSpotLightWnd = pScreenIns->GetSpotLightWindow();
	CSpotLightUI* pSpotLightUI = pSpotLightWnd->GetSpotLightUI();
	switch(dwCommand)
	{
	case SPOTLIGHT_OPEN:	// ��
		{
			OutputDebugString(_T("SPOTLIGHT_OPEN\n"));
			pSpotLightWnd->ShowSpotLightWindow(true);
		}
		break;

	case SPOTLIGHT_SWITCH:
		{
			// �л���״��1-CIRCLE��2-RECTANGLE
			OutputDebugString(_T("SPOTLIGHT_SWITCH\n"));
			DWORD dwType = stPacket.ReadDWORD();
			if(dwType == 1)
			{
				pSpotLightWnd->SetShapeType(SPOTLIGHT_SHAPE_CIRCLE);
			}
			else if(dwType == 2)
			{
				pSpotLightWnd->SetShapeType(SPOTLIGHT_SHAPE_RECTANGLE);
			}
		}
		break;

	case SPOTLIGHT_CHANGE:	// �ı��С
		{
			TCHAR szLog[100] = {0};

			if(pSpotLightUI->GetShape() == SPOTLIGHT_SHAPE_CIRCLE)
			{
				// �뾶���ٷֱ�
				float fPerRadius;
				fPerRadius = stPacket.ReadFloat();
				DWORD dwRadius = PerFloatToSlideLength(fPerRadius, 0);

				_stprintf_s(szLog, _T("SPOTLIGHT_CHANGE-CIRCLE, radius=%d, fRadius=%f\n"), dwRadius, fPerRadius);
				OutputDebugString(szLog);
				pSpotLightWnd->SetCircleRadius(dwRadius);
			}
			else
			{
				// �����п��
				float fPerWidth, fPerHeight;
				DWORD dwWidth, dwHeight;

				fPerWidth = stPacket.ReadFloat();
				fPerHeight  = stPacket.ReadFloat();
				dwWidth = PerFloatToSlideLength(fPerWidth, 0);
				dwHeight = PerFloatToSlideLength(fPerHeight, 1);

				_stprintf_s(szLog, _T("SPOTLIGHT_CHANGE-RECTANGLE, width=%d, height=%d, fWidth=%f, fHeight=%f\n"), dwWidth, dwHeight, fPerWidth, fPerHeight);
				OutputDebugString(szLog);
				pSpotLightWnd->SetRectanglePos(dwWidth/2, dwHeight/2);
			}
		}
		break;

	case SPOTLIGHT_MOVE:	// �ƶ�λ��
		{
			float fPercentX, fPercentY;
			DWORD dwXPos, dwYPos;
			TCHAR szLog[100];

			fPercentX = stPacket.ReadFloat();
			fPercentY = stPacket.ReadFloat();
			pController->SlidePosToScreenPos(fPercentX, fPercentY);
			dwXPos = (DWORD)fPercentX;
			dwYPos = (DWORD)fPercentY;

			_stprintf_s(szLog, _T("SPOTLIGHT_MOVE, x=%d, y=%d, fx=%f, fy=%f\n"), dwXPos, dwYPos, fPercentX, fPercentY);
			OutputDebugString(szLog);
			//pSpotLightWnd->MoveSpotLight(dwXPos, dwYPos);
			pSpotLightWnd->SetSpotLightPos(dwXPos, dwYPos);
		}
		break;

	case SPOTLIGHT_OPEN_LIGHT:	// ����
		OutputDebugString(_T("SPOTLIGHT_OPEN_LIGHT\n"));
		pSpotLightWnd->SetLightStatus(SPOTLIGHT_STATUS_CLOSE);
		break;

	case SPOTLIGHT_CLOSE_LIGHT:	// �ص�
		OutputDebugString(_T("SPOTLIGHT_CLOSE_LIGHT\n"));
		pSpotLightWnd->SetLightStatus(SPOTLIGHT_STATUS_ON);
		break;

	case SPOTLIGHT_TOGGLE:	// �رվ۹��
		OutputDebugString(_T("SPOTLIGHT_TOGGLE\n"));
		pSpotLightWnd->ShowSpotLightWindow(false);
		break;
	}

}
// ��������תΪ��PPTҳ��ĳ��ȡ�
// nLengthType��ʶ����PPT/��Ļ���Ǹ���ת�����ȡ�0�������ݿ����ת�����������֡������ݸ߶���ת����
// bIncludeBlank��ʶ�Ƿ������Ļ��ʾ�����һ�����2�߿հײ��֣�
// true-�����հײ��֣�����fPercent����Ļ�ĳ��ȣ�false-��ȥ�հײ��ֵĿ�/�ߣ�����fPercent��PPTҳ��ĳ���
int CPacketProcessor::PerFloatToSlideLength(float fPercent, int nLengthType, bool bIncludeBlank)
{
	int nLength;

	int nSlideWidth = 0;
	int nSlideHeight = 0;
	CPPTController* pController = GetPPTController();
	pController->GetSlideSize(nSlideWidth, nSlideHeight);
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHight = GetSystemMetrics(SM_CYSCREEN);

	// ����PPTҳ��ķŴ�ϵ��
	float fPerW, fPerH;
	fPerW = 1.0 * nScreenWidth / nSlideWidth;
	fPerH = 1.0 * nScreenHight / nSlideHeight;

	// �����ʵ�ʵ�PTTҳ���Ⱥ͸߶�
	int nActWidth, nActHeight; // ʵ�ʵ�PTTҳ���Ⱥ͸߶�
	if(fPerW < fPerH)
	{
		nActWidth = nScreenWidth;
		nActHeight = fPerW * nSlideHeight;
	}
	else
	{
		nActWidth = fPerH * nSlideWidth;
		nActHeight = nScreenHight;
	}

	// ���ݲ���ȷ��ת������
	int nReferLength = 0;
	if(nLengthType == 0)
	{
		if(bIncludeBlank)
			nReferLength = nScreenWidth;
		else
			nReferLength = nActWidth;
	}
	else
	{
		if(bIncludeBlank)
			nReferLength = nScreenHight;
		else
			nReferLength = nActHeight;
	}

	nLength = abs(fPercent) * nReferLength;
	return nLength;
}
void CPacketProcessor::BlackBoardControl(SOCK_PACKET* pSockPacket)
{
	// PPTδ��ӳʱ��ʹ�úڰ�
	CPPTController* pController = GetPPTController();
	if(pController!=NULL && !pController->IfPPTSlideShowView())
	{
		// δ��ӳʱ�յ��ڰ�����
		return;
	}

	CPacketStream stPacket(pSockPacket->buffer);
	stPacket.SkipHeader();
	DWORD dwCommand;
	dwCommand = stPacket.ReadDWORD();
	switch(dwCommand)
	{
	case BLACKBOARD_OPEN:
		{
			// �д������ڣ�Ҫ��PostMessage
			CScreenInstrumentUI* pScreenIns = CScreenInstrumentUI::GetMainInstrument();
			if(pScreenIns == NULL)
				WRITE_LOG_LOCAL(_T("BlackBoardControl Open BlackBoard, MainInstrument Instance is null"));
			PostMessage(pScreenIns->GetHWND(), WM_OPENBLACKBOARD, 0, 0);
		}
		break;
	case BLACKBOARD_CLOSE:
		{
			CScreenInstrumentUI* pScreenIns = CScreenInstrumentUI::GetMainInstrument();
			if(pScreenIns == NULL)
				WRITE_LOG_LOCAL(_T("BlackBoardControl Close BlackBoard, MainInstrument Instance is null"));
			PostMessage(pScreenIns->GetHWND(), WM_CLOSEBLACKBOARD, 0, 0);
		}
		break;
	}
}

//
// process file transfer action
//
void CPacketProcessor::ProcessActionTransferFile(SOCK_PACKET *pSockPacket)
{
	PACKET_HEAD* pHeader = (PACKET_HEAD*)pSockPacket->buffer;
	if( pHeader == NULL )
		return;

	switch( pHeader->dwMinorType )
	{
	case TRANSFER_FILE_START:
		{
			CPacketStream FileHeadStream(pSockPacket->buffer);

			tstring strFileName;
			DWORD dwFileSize = 0;
			DWORD dwFilecCrc = 0;
			DWORD dwFileType = 0;

			FileHeadStream.UnPackFileHead(strFileName, dwFileSize, dwFilecCrc, dwFileType);

			tstring strFilePath;
			GenReceiveFilePath(strFileName, strFilePath);
			//tstring strFilePath = GetLocalPath();
			//strFilePath += _T("\\");
			//strFilePath += DIRECTORY_MOBILE_RECV_FILE;

			//// create directory
			//CreateDirectory(strFilePath.c_str(), NULL);

			//strFilePath += _T("\\");
			//strFilePath += strFileName;

			// ����ļ�û�ڴ����б��У�Ҳ��ӽ�ȥ��ͼƬ�ļ��Ȳ�����CRCУ������
			if(m_mapTransferFile.count(strFileName) == 0)
				m_mapTransferFile[strFileName] = dwFilecCrc;
			WRITE_LOG_LOCAL(_T("TRANSFER_FILE_START filename=%s"), strFileName.c_str());

			FileTransferManager::GetInstance()->RecvFile((TCHAR*)strFilePath.c_str(), dwFileSize, dwFilecCrc, FILE_RECV_START);
	
		}
		break;

	case TRANSFER_FILE_END:
		{
			DWORD dwFileCrc;
			CPacketStream FileEndStream(pSockPacket->buffer);
			FileEndStream.UnPackFileEnd(dwFileCrc);

			// �Ƴ��Ѵ�������ļ���Ϣ
			tstring strFileName;
			for(map<tstring, DWORD>::iterator iter=m_mapTransferFile.begin(); iter!=m_mapTransferFile.end(); iter++)
			{
				if(iter->second == dwFileCrc)
				{
					strFileName = iter->first;
					m_mapTransferFile.erase(iter);
					break;
				}
			}

			// ���strFileName�ǿձ�ʾ�ļ��ѱ��Ƴ������б�����ͬ���ļ�����2���ļ���CRC��ͬʱ�����strFileNameΪ��
			if(strFileName.empty())
				WRITE_LOG_LOCAL(_T("TRANSFER_FILE_END, File Retransferred"), strFileName.c_str())
			else
				WRITE_LOG_LOCAL(_T("TRANSFER_FILE_END, filename=%s"), strFileName.c_str())

			FileTransferManager::GetInstance()->RecvFile(NULL, 0, dwFileCrc, FILE_RECV_END);
		}
		break;

	case TRANSFER_FILE_DATA:
		{
			char szFileData[MAX_FILE_DATA_LEN];

			DWORD dwFileCrc;
			DWORD dwDataSize;

			CPacketStream FileDataStream(pSockPacket->buffer);
			FileDataStream.UnPackFileData(dwFileCrc, szFileData, dwDataSize);

			FileTransferManager::GetInstance()->RecvFile(NULL, 0, dwFileCrc, FILE_RECV_DATA, szFileData, dwDataSize);

		}
		break;

	case TRANSFER_FILE_CANCEL:
		{
			DWORD dwFileCrc;
			CPacketStream FileCancelStream(pSockPacket->buffer);
			FileCancelStream.UnPackFileCancel(dwFileCrc);

			// �Ƴ�ȡ��������ļ���Ϣ
			tstring strFileName;
			for(map<tstring, DWORD>::iterator iter=m_mapTransferFile.begin(); iter!=m_mapTransferFile.end(); iter++)
			{
				if(iter->second == dwFileCrc)
				{
					strFileName = iter->first;
					m_mapTransferFile.erase(iter);
					break;
				}
			}

			FileTransferManager::GetInstance()->CancelFile(dwFileCrc);
			
		}
		break;

	case TRANSFER_FILE_CHECK_EXISTS:
		{
			TransferFileCheckExists(pSockPacket);
			break;
		}
	}
}

void CPacketProcessor::ProcessActionPushData(SOCK_PACKET *pSockPacket)
{
	 
}

void CPacketProcessor::ProcessActionPullData(SOCK_PACKET *pSockPacket)
{
	PACKET_HEAD* pHeader = (PACKET_HEAD*)pSockPacket->buffer;
	if( pHeader == NULL )
		return;

	switch( pHeader->dwMinorType )
	{
	case PULL_M2P_APKFILE:
		{
			// send PPTShell.apk to mobile
			tstring strFilePath = GetLocalPath() + _T("\\Bin\\PPTShell.apk");
			FileTransferManager::GetInstance()->SendFile((TCHAR*)strFilePath.c_str(), pSockPacket->sock, pSockPacket->type, FILE_TYPE_APK);

		}
		break;
	case PULL_M2P_PPTFILE:
		{
			CPPTController* pController = GetPPTController();
			if( pController != NULL )
			{
				tstring strFilePath = pController->GetFilePath();
				FileTransferManager::GetInstance()->SendFile((TCHAR*)strFilePath.c_str(), pSockPacket->sock, pSockPacket->type, FILE_TYPE_PPTFILE);
			}
		}
		break;
	}

}

void CPacketProcessor::ProcessActionSyncData(SOCK_PACKET* pSockPacket)
{
	PACKET_HEAD* pHeader = (PACKET_HEAD*)pSockPacket->buffer;
	if( pHeader == NULL )
		return;

	switch( pHeader->dwMinorType )
	{
	case SYNC_DATA_REQUEST_SLIDE_IMAGE:
		SyncSlideImgRequest(pSockPacket);
		break;
	case SYNC_SAVE_CHANGE:
		{
			CPPTController* pController = GetPPTController();
			CPPTInfoSender *pptSender = PPTInfoSender::GetInstance();
			if( pController != NULL && pController->IsChanged(FALSE) && pptSender!=NULL && !pptSender->IsSyncDataPPTChangedAck())
			{
				pController->Save();
				pptSender->SetAckSyncDataPPTChanged();
			}
		}
		break;
	case SYNC_NOSAVE_CHANGE:
		{
			CPPTInfoSender *pptSender = PPTInfoSender::GetInstance();
			if (pptSender != NULL)
			{
				pptSender->SetAckSyncDataPPTChanged();
			}
		}
		break;
	default:
		WRITE_LOG_LOCAL(_T("CPacketProcessor::ProcessActionSyncData, unknow minor type:%d"), pHeader->dwMinorType);
		break;

	}
}

void CPacketProcessor::SyncSlideImgRequest(SOCK_PACKET* pSockPacket)
{
	CPPTInfoSender *pptSender = PPTInfoSender::GetInstance();
	if (pptSender == NULL)
	{
		WRITE_LOG_LOCAL(_T("CPacketProcessor::SyncSlideImgRequest, CPPTInfoSender is null!"))
		return;
	}
	CPacketStream syncControlStream(pSockPacket->buffer);
	syncControlStream.SkipHeader();

	DWORD rqstCnt = syncControlStream.ReadDWORD();
	
	tstring strLog = _T("---------Rcv mobile request slide:");
	vector<int> vecSlideIdxs;
	for (int i=0; i<rqstCnt; i++)
	{
		int iPageIdx = syncControlStream.ReadDWORD();
		if (iPageIdx>0)
		{
			vecSlideIdxs.push_back(iPageIdx);
			strLog += iPageIdx; 
			strLog += _T("|");
		}
	}
	WRITE_LOG_LOCAL(_T("%s"), strLog.c_str());
	pptSender->SendSlideThumbList(pSockPacket->sock, pSockPacket->type, vecSlideIdxs);
}

//
// PPT control
//
void CPacketProcessor::PPTControl(SOCK_PACKET* pSockPacket)
{
	BOOL res = FALSE;

	CPPTController* pController = GetPPTController();
	if( pController == NULL )
		return;

	DWORD dwControlCommand = 0;
	CPacketStream PPTControlStream(pSockPacket->buffer);
	PPTControlStream.UnPackPPTControl(dwControlCommand);

	tstring strErrorDesc = _T("");

	switch(dwControlCommand)
	{
	case PPTCTL_PLAY:
		{
			DWORD dwSlideId = PPTControlStream.ReadDWORD();
			if( dwSlideId == 0 )
				res = pController->Play(TRUE);
			else
				res = pController->Play(FALSE);

			if( !res )
				strErrorDesc = _T("û��PPT���Բ���");
			
		}
		break;

	case PPTCTL_STOP:
		{
			if( !pController->IfPPTSlideShowView() )
				strErrorDesc = _T("PPTδ����");
			else
			{
				StopPPTByThread();
				res = TRUE;
			}
		}		
		break;

	case PPTCTL_PREV:
		{
			if (pController->IfPPTSlideShowView())
			{
				pController->TopShowSlideView();//�ö�����Ȼ���������ͻᵼ���޷���ҳ 2016.03.04 cws
				res = pController->Prev();
				if( !res )
					strErrorDesc = _T("PPTδ����");
			}
			else
			{
				int iSlideID = pController->GetActiveSlideIndex();
				iSlideID -= 1;
				res = pController->PresentationGoto(iSlideID);
				if( !res )
					strErrorDesc = _T("PPTԤ����תʧ��");
				else
					PPTInfoSender::GetInstance()->SendPreviewSlideIdxToAllUser(iSlideID);
			} 
		}
		break;

	case PPTCTL_NEXT:
		{
			if (pController->IfPPTSlideShowView())
			{
				pController->TopShowSlideView();//�ö�����Ȼ���������ͻᵼ���޷���ҳ 2016.03.04 cws
				res = pController->Next();
				if( !res )
				{
					strErrorDesc = _T("PPTδ����");
				}
				else
				{
					// �������ĻʱҪ���͵�ǰҳ�Ÿ��ֻ���,�����������������
					//if(pController->IfPPTSlideShowView())
					//static bool bSentBlackScreen = false;
					//if(!bSentBlackScreen)
					//{
					int nPage = pController->GetCurrentShowPosition();
					//WRITE_LOG_LOCAL(_T("PPTCTL_NEXT GetCurrentShowPosition=%d\n"), nPage);
					int nCount = CPPTController::GetShowViewCount();
					if(nPage > nCount)
					{
						OutputDebugString(_T("Send Black Screen Index\n"));
						//bSentBlackScreen = true;
						PPTInfoSender::GetInstance()->SendPlaySlideToAllUsers(nCount + 1);
					}
					//}
				}
			}
			else
			{
				int iSlideID = pController->GetActiveSlideIndex();
				iSlideID += 1;
				res = pController->PresentationGoto(iSlideID);
				if( !res )
					strErrorDesc = _T("PPTԤ����תʧ��");
				else
					PPTInfoSender::GetInstance()->SendPreviewSlideIdxToAllUser(iSlideID);
			} 
		}
		break;

	case PPTCTL_GOTO:
		{

			int iSlideID = PPTControlStream.ReadDWORD();

			if (pController->IfPPTSlideShowView())
			{
				pController->TopShowSlideView();//�ö�����Ȼ���������ͻᵼ���޷���ҳ 2016.03.04 cws
				res = pController->Goto(iSlideID);
				if( !res )
					strErrorDesc = _T("PPTδ����");
			}
			else
			{
				res = pController->PresentationGoto(iSlideID);
				if( !res )
					strErrorDesc = _T("PPTԤ����תʧ��");
				else
					PPTInfoSender::GetInstance()->SendPreviewSlideIdxToAllUser(iSlideID);
			} 
		}
		break;

	case PPTCTL_CLEAR_INK:
		break;

	case PPTCTL_BLACK_BACKGROUND:
		{
			PpSlideShowState state = (PpSlideShowState)pController->GetSlideShowViewState();
			res = pController->SetSlideShowViewState(ppSlideShowBlackScreen);
			if( !res )
				strErrorDesc = _T("PPTδ����");
		}
		break;

	case PPTCTL_WHITE_BACKGROUND:
		{
			res = pController->SetSlideShowViewState(ppSlideShowWhiteScreen);
			if( !res )
				strErrorDesc = _T("PPTδ����");
		}
		break;
		
	case PPTCTL_CANCEL_WHITE_BACKGROUND:
		{
			if(pController->GetPPTVersion() <= PPTVER_2007)
			{
				res = pController->SetSlideShowViewState(ppSlideShowRunning);
				if( !res )
					strErrorDesc = _T("PPTδ����");
			}
			else//OFFICE2013����������ppSlideShowRunning������Ч������ģ�ⰴ��������2016.01.05 CWS
			{
				res = TRUE;
				pController->TopShowSlideView();
				keybd_event('W', 0,0,0);
				keybd_event('W',0, KEYEVENTF_KEYUP, 0);
			}
		}
		break;
	case PPTCTL_CANCEL_BLACK_BACKGROUND:
		{
			// OFFICE2007����������ppSlideShowRunning������PPTҳ��������ͼ
			// OFFICE2013����������ppSlideShowRunning������Ч������ģ�ⰴ��������2016.01.05 CWS
			//res = pController->SetSlideShowViewState(ppSlideShowRunning);
			res = TRUE;
			pController->TopShowSlideView();
			keybd_event('B', 0,0,0);
			keybd_event('B',0, KEYEVENTF_KEYUP, 0);
		}
		break;

	case PPTCTL_HANDLCLAP:
		PlayEffect(0);
		break;

	case PPTCTL_CAIDAI:
		PlayEffect(1);
		break;

	case PPTCTL_CONFETTI:
		PlayEffect(2);
		break;

	case PPTCTL_CALC:
		{
			res = ExecCalculator();
			if( !res )
				strErrorDesc = _T("���м�����ʧ��");
		}
		break;

	case PPTCTL_CANCEL_CALC:
		{
			res = KillCalculator();
			// ǿ�Ʒ��سɹ����ֻ��ˡ�
			res = TRUE;
			strErrorDesc = _T("������δ����");
		}
		break;

	case PPTCTL_LASER_POINTER:
		break;

	case PPTCTL_PEN_POINTER:
		break;

	case PPTCTL_NORMAL_POINTER:
		break;

	case PPTCTL_PLAY_VIDEO:
		{
			int nVideoIndex = PPTControlStream.ReadInt();
			res = pController->PlayVideo(nVideoIndex);
			if( !res )
				strErrorDesc = _T("��Ƶ����ʧ��");

		}
		break;

	case PPTCTL_PAUSE_VIDEO:
		{
			int nSlideIdx = PPTControlStream.ReadInt();
			res = pController->PauseVideo(nSlideIdx);
			if( !res )
				strErrorDesc = _T("��Ƶ��ͣʧ��");
		}
		break;

	case PPTCTL_STOP_VIDEO:
		{
			int nSlideIdx = PPTControlStream.ReadInt();
			res = pController->StopVideo(nSlideIdx);
			if( !res )
				strErrorDesc = _T("��Ƶֹͣʧ��");
		}
		break;

	case PPTCTL_FLAME:
		break;
	case PPTCTL_OPEN:
		{
			if ( CPPTController::isPlayMode() )
			{
				strErrorDesc = _T("���ڲ���PPT���޷���");
				WRITE_LOG_LOCAL(_T("Recv PPTControl Action When PPT Playing, cmd is %d"), PPTCTL_OPEN);
			}
			else
			{

				string strUtf8FileName = PPTControlStream.ReadCString();
				tstring strFileName = Utf82Str(strUtf8FileName);

				// ���ļ��ظ���ʱ������ǰ����ļ�������
				if(m_mapTransferFile.count(strFileName) != 0)
				{
					DWORD dwCRC = CalcFileCRC(strFileName.c_str());
					if(m_mapTransferFile[strFileName] == dwCRC)
					{
						break;
					}
				}

				tstring strFilePath = _T("");
				GenReceiveFilePath(strFileName, strFilePath);

				WRITE_LOG_LOCAL(_T("PPTCTL_OPEN filename=%s"), strFileName.c_str());

				CStream* pStream	= new CStream(256);
				pStream->WriteDWORD(PPTC_OPEN_PPTFILE);
				pStream->WriteString(strFilePath);
				BroadcastPostedEvent(EVT_THREAD_CONTROL_PPT, (WPARAM)pStream, 0, NULL);
			}
		}
		break;
	case PPTCTL_BARRAGE:
		{
			
#ifdef _UNICODE
			tstring wstrDanmukuWord = PPTControlStream.ReadString();
			string astrDanmukuWord = string((char *)wstrDanmukuWord.c_str());
			tstring strDanmukuWord = Utf82Str(astrDanmukuWord);
#else
			tstring strDanmukuWord = PPTControlStream.ReadString();
#endif
			//strDanmukuWord = Utf82Str(strDanmukuWord);
			//wstring wstrDanmukuWord = Str2Unicode(strDanmukuWord);
			
			CStream* pStream = new CStream(256);
			pStream->WriteString(strDanmukuWord);
			BroadcastPostedEvent(EVT_DANMUKU, (WPARAM)pStream, 0, NULL);
			
		}
		break;
	case PPTCTL_YAOHAO:
		break;
	}

	CPacketStream ControlResponseStream;
	ControlResponseStream.PackControlResponse(dwControlCommand, res, strErrorDesc);
	
	CPPTShellApp* pApp = (CPPTShellApp*)AfxGetApp();
	pApp->SendStreamPacket(pSockPacket->sock, &ControlResponseStream, pSockPacket->type);

}

void GenReceiveFilePath(const tstring& strFileName, tstring &strFullName)
{
	strFullName = GetLocalPath();
	strFullName += _T("\\");
	strFullName += _T("Cache");
	CreateDirectory(strFullName.c_str(), NULL);

	strFullName += _T("\\");
	strFullName += DIRECTORY_MOBILE_RECV_FILE;
	CreateDirectory(strFullName.c_str(), NULL);

	strFullName += _T("\\");
	strFullName += strFileName;
}

//
// Image control
//
void CPacketProcessor::ImageControl(SOCK_PACKET *pSockPacket)
{
	BOOL res = FALSE;

	DWORD dwControlCommand = 0;
	CPacketStream ImageControlStream(pSockPacket->buffer);
	ImageControlStream.UnPackImageControl(dwControlCommand);

	CPPTController* pPPTController = GetPPTController();

	switch( dwControlCommand )
	{
	case IMGCTL_OPEN:
		{
			// image file name
			string strUtf8FileName = ImageControlStream.ReadCString();
			tstring strFileName = Utf82Str(strUtf8FileName);

			tstring strFilePath = _T("");
			GenReceiveFilePath(strFileName, strFilePath);

			ImageCtrlDialogUI::GetInstance()->OpenDialog(strFilePath.c_str());
		}
		break;

	case IMGCTL_ZOOMIN:
		{
			bool bClosed = ImageCtrlDialogUI::GetInstance()->IsWindowClosed();
			if(!bClosed)
			{
				float fScale = ImageControlStream.ReadFloat();
				ImageCtrlDialogUI::GetInstance()->ZoomIn(fScale);
			}	
		}
		break;

	case IMGCTL_ZOOMOUT:
		{
			bool bClosed = ImageCtrlDialogUI::GetInstance()->IsWindowClosed();
			if(!bClosed)
			{
				float fScale = ImageControlStream.ReadFloat();
				ImageCtrlDialogUI::GetInstance()->ZoomOut(fScale);
			}
		}
		break;

	case IMGCTL_NORMAL:
		{
			bool bClosed = ImageCtrlDialogUI::GetInstance()->IsWindowClosed();
			if(!bClosed)
				ImageCtrlDialogUI::GetInstance()->Normal();
		}
		break;

	case IMGCTL_CLOSE:
		// �����ش����ѹرպ󣬺�����������
		{
			bool bClosed = ImageCtrlDialogUI::GetInstance()->IsWindowClosed();
			if(!bClosed)
				ImageCtrlDialogUI::GetInstance()->CloseDialog();
		}
		break;
	}

}

//
// 3D Control
//
void CPacketProcessor::ThreeDControl(SOCK_PACKET *pSockPacket)
{
	BOOL res = FALSE;

	DWORD dwControlCommand = 0;
	CPacketStream ThreedControlStream(pSockPacket->buffer);
	ThreedControlStream.UnPack3DControl(dwControlCommand);

	switch( dwControlCommand )
	{
	case THREEDCTL_ZOOMIN:
		break;

	case THREEDCTL_ZOOMOUT:
		break;

	case THREEDCTL_MOVE:
		{
			// points of position
			int nPointCount = ThreedControlStream.ReadInt();
			for(int i = 0; i < nPointCount; i++)
			{
				
			}

		}
		break;

	case THREEDCTL_CLOSE:
		break;
	}
	
}

//
// util functions
//
BOOL CPacketProcessor::PlayEffect(int nEffectType)
{
	if( nEffectType == -1 )
		return FALSE;

	tstring sActionPath[] = { _T("4"), _T("5"), _T("6"), _T(""), _T(""), _T(""), _T(""), _T("")};
	TCHAR szExePath[MAX_PATH*2]; 
	TCHAR szImageDir[MAX_PATH*2];// ����Ŀ¼��ָ�����������Ქ�� 2015.12.18
 
	_stprintf_s(szImageDir, _T("%s\\bin"), GetLocalPath().c_str()); 
 
	_stprintf_s(szExePath, _T("%s\\bin\\GifPlayer.exe"), GetLocalPath().c_str());
	HINSTANCE hInstance = ShellExecute(NULL, _T("open"), szExePath, sActionPath[nEffectType].c_str(), szImageDir, SW_SHOW );
	if( (int)hInstance < 32 )
		return FALSE;


	return TRUE;
}

BOOL CPacketProcessor::ExecCalculator()
{
	//KillExeCheckParentPid(_T("CALC.EXE"), TRUE);//�Ȱ�ԭ�ȴ򿪵Ĺر� 2015.12.18 CWS  2016.01.15 ��
	if (! ControlCalcTop())
	{
		TCHAR szExePath[MAX_PATH*2]; 
		TCHAR szSystemPath[MAX_PATH*2];

		GetSystemDirectory(szSystemPath, MAX_PATH*2);
		_stprintf_s(szExePath, _T("%s\\calc.exe"), szSystemPath);

		HINSTANCE hInstance = ShellExecute(NULL, _T("open"), szExePath, _T(""), _T(""), SW_SHOW);
		if( (int)hInstance < 32 )
			return FALSE;
		ControlCalcTop();//�ٴ��ö�2016.06.15 cws
	}
 
	return TRUE;
}

BOOL CPacketProcessor::ControlCalcTop()
{ 
	HWND hWnd = FindWindow(_T("CalcFrame"), NULL); 
	if( !hWnd )
	{   
		hWnd = FindWindow(NULL, _T("������"));//win10ϵͳ ����ϵͳ�Ƚ������2016.01.07 cws 
	} 
	if (hWnd)
	{
		CRect rectCal;
		::GetWindowRect(hWnd,&rectCal);
		int calWidth = rectCal.Width();
		int calHeight = rectCal.Height();

		int nCx  = GetSystemMetrics(SM_CXSCREEN);
		int nCy = GetSystemMetrics(SM_CYSCREEN);
		::SetWindowPos(hWnd,HWND_TOPMOST ,(nCx - calWidth) /2, (nCy - calHeight) / 2, calWidth,calHeight,SWP_NOACTIVATE|SWP_SHOWWINDOW);		
		return TRUE; 
	}
	return FALSE;
}

BOOL CPacketProcessor::KillCalculator()
{
	HWND hWnd = FindWindow(_T("CalcFrame"), NULL);
	if (!hWnd)
	{
		hWnd = FindWindow(NULL, _T("������"));//win10ϵͳ ����ϵͳ�Ƚ������2016.01.07 cws 
	}
	if( hWnd )
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	else
	{ 
		return FALSE; 
	}

	return TRUE;
}
// ������	TransferFileCheckExists
// ������	pSockPacket		- ���յ�������֡
// ���أ�	TRUE			- ���ͻظ�֡�ɹ�
//			FALSE			- �����쳣
// ������	У�鱾���ļ��Ƿ��Ѵ��ڣ�����У�����ظ�֡��
BOOL CPacketProcessor::TransferFileCheckExists(SOCK_PACKET *pSockPacket)
{
	tstring strFileName;
	DWORD dwFileCRC = 0;
	BOOL bExists = FALSE;

	CPacketStream FileCheckExists(pSockPacket->buffer);
	FileCheckExists.UnPackFileCRC(strFileName, dwFileCRC);

	// ��鱾���ļ�
	tstring strFilePath;
	GenReceiveFilePath(strFileName, strFilePath);
	//tstring strFilePath = GetLocalPath();
	//strFilePath += _T("\\");
	//strFilePath += DIRECTORY_MOBILE_RECV_FILE;
	//CreateDirectory(strFilePath.c_str(), NULL);
	//strFilePath += _T("\\");
	//strFilePath += strFileName;

	bExists = FileTransferManager::GetInstance()->CheckFileIsExists(strFilePath, dwFileCRC);

	m_mapTransferFile[strFileName] = dwFileCRC;
	//if( !bExists)
	//{
	//	// ����ļ������жϣ��ᵼ�º���ԭ�ļ��޷��ϴ�
	//	// ���CRC�뱾���ļ���һ�£��������ļ�����δ������Ҳ�����ļ����޸ġ�����ļ��Ƿ��ڴ����б�
	//	map<tstring, DWORD>::iterator iter = m_mapTransferFile.find(strFileName);
	//	if(iter != m_mapTransferFile.end())
	//	{
	//		// �ļ��ڴ��䴫���б������CRC�Ƿ���ͬ
	//		if(dwFileCRC == iter->second)
	//			bExists = TRUE;
	//		else
	//			bExists = FALSE;
	//	}
	//	else
	//	{
	//	}
	//	// ���汾�δ�����ļ���Ϣ
	//	if(!bExists)
	//		m_mapTransferFile[strFileName] = dwFileCRC;
	//}

 	CPPTShellApp* pApp = (CPPTShellApp*)AfxGetApp();
	if( pApp == NULL )
		return FALSE;

	
	// ���ͼ����֡��bExists=1��ʾ�����ļ��Ѵ��ڣ�bExists=0��ʾ�����ļ�������
	CPacketStream FileCheckResult;
	FileCheckResult.PackFileCheckResult(strFileName, bExists);
	pApp->SendStreamPacket(pSockPacket->sock, &FileCheckResult, pSockPacket->type);

	return TRUE;
}

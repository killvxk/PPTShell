#include "StdAfx.h"
#include "SyncInternetTime.h"
#include <winbase.h>

CSyncInternetTime::CSyncInternetTime(void)
{
}

CSyncInternetTime::~CSyncInternetTime(void)
{
}



void CSyncInternetTime::SyncSystime() 
{ 

	// �õ��û�ѡ��ķ�����
	//	CString strServer("");
	//m_cmbServer.GetWindowText(strServer);
 
	//	"utcnist.colorado.edu"  
	//	"ntps1-0.uni-erlangen.de"
	//	"ntps1-1.uni-erlangen.de"
	//	"ntps1-2.uni-erlangen.de"
	//	"ntps1-0.cs.tu-berlin.de"
	//	"time.ien.it" 
	CString strServer="time.ien.it";
	//��ȡinternetʱ��
	CTime tm(1990, 1, 1, 0, 0, 0);
	if(GetInternetTime(&tm, strServer))
	{
		SyncSystemClock(tm);
	} 
}

BOOL CSyncInternetTime::GetInternetTime(CTime* pTime, CString strServer)
{

	CSocket sockClient;
	sockClient.Create();			//����socket 

	sockClient.Connect((LPCTSTR)strServer, 37); // strServer��ʱ���������ַ�� 37���˿ں�

	DWORD dwTime = 0;				//������ŷ����������ı�׼ʱ������
	unsigned char nTime[8];			//��ʱ��������
	memset(nTime, 0, sizeof(nTime));

	sockClient.Receive(nTime, sizeof(nTime));	//���շ�������������4���ֽڵ�����
	sockClient.Close();				//�ر�socket 

	dwTime += nTime[0] << 24;		//��������	
	dwTime += nTime[1] << 16;
	dwTime += nTime[2] << 8;
	dwTime += nTime[3];		

	if(0 == dwTime)	return FALSE;

	//for debug
	CString strSec("");
	strSec.Format("Receive value : %u\r\n", dwTime);

	//�������������������Դ�1900������������
	//ȡ�� 1900~1970 ��ʱ���(����������) ������dwSpan����
	COleDateTime t00( 1900, 1, 1, 0, 0, 0 ); // 1900.1.1 00:00:00 
	COleDateTime t70( 1970, 1, 1, 0, 0, 0 ); // 1970.1.1 00:00:00 

	COleDateTimeSpan ts70to00 = t70 - t00; 
	DWORD dwSpan = (DWORD)ts70to00.GetTotalSeconds(); 
	ASSERT( dwSpan == 2208988800L ); 

	//��ʱ���Ϊ����1970��ģ�������CTime����
	dwTime -= dwSpan;		
	//���������ӳ�����
	dwTime += 60;
	//���쵱ǰʱ���CTime����
	CTime timeNow = (CTime)dwTime;
	*pTime = timeNow;		

	return TRUE;

}

BOOL CSyncInternetTime::SyncSystemClock(CTime tmServer)
{

	//���������̵�Ȩ��
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize=sizeof OSVERSIONINFO;
	GetVersionEx(&osv);
	if(osv.dwPlatformId==VER_PLATFORM_WIN32_NT)
	{

		HANDLE hToken; 
		TOKEN_PRIVILEGES tkp; 

		// Get a token for this process. 

		if (!OpenProcessToken(GetCurrentProcess(), 
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
			return( FALSE ); 

		// Get the LUID for the shutdown privilege. 

		LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, 
			&tkp.Privileges[0].Luid); 

		tkp.PrivilegeCount = 1;  // one privilege to set    
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

		// Get the shutdown privilege for this process. 

		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
			(PTOKEN_PRIVILEGES)NULL, 0); 

		if (GetLastError() != ERROR_SUCCESS) 
			return FALSE; 

	}

	//����ϵͳʱ��
	SYSTEMTIME systm;
	systm.wYear = tmServer.GetYear();
	systm.wMonth = tmServer.GetMonth();
	systm.wDay = tmServer.GetDay();
	systm.wHour = tmServer.GetHour();
	systm.wMinute = tmServer.GetMinute();
	systm.wSecond = tmServer.GetSecond();
	systm.wMilliseconds = 0;


	if(0 != ::SetLocalTime(&systm))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
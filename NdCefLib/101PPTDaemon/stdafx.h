// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif	


#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <Tlhelp32.h>
#include <SetupAPI.h>
#include <atlstr.h>
#include "dbt.h"
#include "guiddef.h"

#include <memory>	
#include <vector>	
using namespace std;

void SvcDebugOut(LPSTR String, DWORD Status);
void SvcDebugOutFile(std::wstring nFilePath,LPSTR String, DWORD Status);
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

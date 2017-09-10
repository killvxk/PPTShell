#pragma once

#include "..\DuiLib\UIlib.h"

#ifndef NO_USING_DUILIB_NAMESPACE
using namespace DuiLib;
using namespace std;
#endif

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib.lib")
#   endif
#endif


#define UIBEGIN_MSG_MAP virtual void Notify(TNotifyUI& msg) { 
#define SUPER_HANDLER_MSG __super::Notify(msg);

//#define EVENT_HANDLER(event, func) if(msg) \
//{ \
//	func(msg);\
//	OutputDebugString(msg.sType + _T("\t\t") + msg.pSender->GetName() + _T("\r\n"));\
//}

#define EVENT_HANDLER(event, func) if( msg.sType == event && msg.pSender) \
{ \
	OutputDebugString(msg.sType + _T("\t\t") + msg.pSender->GetName() + _T("\r\n"));\
	func(msg);\
}

#define EVENT_ID_HANDLER(event, id, func) if(msg.sType == event && msg.pSender && _tcsicmp(msg.pSender->GetName(), id) == 0 ) \
{ \
	OutputDebugString(msg.sType + _T("\t\t") + msg.pSender->GetName() + _T("\r\n"));\
	func(msg);\
}

#define EVENT_ITEM_HANDLER_WITH_LIST_ID(event, id, func) if(msg.sType == event && msg.pSender && _tcsicmp(msg.pSender->GetParent()->GetParent()->GetName(), id) == 0 ) \
{ \
	OutputDebugString(msg.sType + _T("\t\t") + msg.pSender->GetParent()->GetParent()->GetName() + _T("\r\n"));\
	func(msg);\
}

#ifdef  _DEBUG
#define EVENT_PRINTER { \
	if(msg.pSender){\
		OutputDebugString(msg.sType + _T("\t\t") + msg.pSender->GetName() + _T("\r\n"));\
	}\
}
#else
#define EVENT_PRINTER
#endif



#define EVENT_BUTTON_CLICK(id,	func)		EVENT_ID_HANDLER(DUI_MSGTYPE_CLICK,			id, func)
#define EVENT_ITEM_CLICK(id,	func)		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMCLICK,		id, func)
#define EVENT_ITEM_RCLICK(id,	func)		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMRCLICK,	id, func)
#define EVENT_ITEM_LDBCLICK(id, func)		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMACTIVATE,	id, func)
#define EVENT_ITEM_SELECT(id,	func)		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMSELECT,	id, func)

#define UIEND_MSG_MAP return;} 


#define VERIFY_LPSZ(lpszVal, ret)  do { if ( (NULL == (lpszVal)) || (NULL == lpszVal[0]) ) return ret; } while ( 0 );
#define VERIFY_LPSZ_NOT_RET(lpszVal)  do { if ( (NULL == (lpszVal)) || (NULL == lpszVal[0]) ) return; } while ( 0 );
#define VERIFY_PTR(lpszVal, ret)  do { if ( (NULL == (lpszVal)) ) return ret; } while ( 0 );
#define VERIFY_PTR_NOT_RET(lpszVal)  do { if ( (NULL == (lpszVal)) ) return; } while ( 0 );


typedef enum
{
	UIFONT_GDI = 0,    /**< GDI */
	UIFONT_GDIPLUS,    /**< GDI+ */
	UIFONT_FREETYPE,   /**< FreeType */ 
	UIFONT_DEFAULT,    
} UITYPE_FONT;

#define DT_CALC_SINGLELINE		 DT_SINGLELINE| DT_LEFT_EX|DT_NOPREFIX  //���㵥��
#define DT_CALC_MULTILINE		 DT_WORDBREAK | DT_EDITCONTROL | DT_LEFT_EX | DT_NOPREFIX // �������

enum
{
	RES_TYPE_COURSE = 50001,
	RES_TYPE_VIDEO,
	RES_TYPE_PIC,
	RES_TYPE_FLASH,
	RES_TYPE_VOLUME,
	
	RES_TYP_TOTAL
};

#define	TOAST_TIMER_ID		10001

#define _STR_NODEJS_NO_PREPARE					_T("ϰ��ģ�����ڸ����У����Ժ�...")
#define _STR_PREVIEW_DIALOG_NOSUPPORT			_T("����ʧ�ܣ�����Դȱʧ")
#define _STR_PREVIEW_DIALOG_INSERT_OK			_T("����ɹ�")
#define _STR_PREVIEW_DIALOG_INSERT_FAIL			_T("����ʧ�ܣ�����Դȱʧ")
#define _STR_PREVIEW_DIALOG_UNDO_OK				_T("�����ɹ�")
#define _STR_PREVIEW_DIALOG_RESOURCE_NOT_FOUND	_T("����Դȱʧ")
#define _STR_PREVIEW_DIALOG_NOUSER				_T("���ȵ�¼�û�")
#define _STR_PREVIEW_DIALOG_DOING				_T("���ڲ����У����Ժ�...")

#define _STR_RESOURCE				_T("��Դ")
#define _STR_GROUPEXPLORER_NONE		_T("��ʱû��")

#define _STR_RIGHTBAR_ITEM_COURSE	_T("�μ�")
#define _STR_RIGHTBAR_ITEM_VEDIO	_T("��Ƶ")
#define _STR_RIGHTBAR_ITEM_IMAGE	_T("ͼƬ")
#define _STR_RIGHTBAR_ITEM_FLASH	_T("����")
#define _STR_RIGHTBAR_ITEM_VOLUME	_T("��Ƶ")
#define _STR_RIGHTBAR_ITEM_QUESTION		_T("ϰ��")
#define _STR_RIGHTBAR_ITEM_INTERACTION	_T("����ϰ��")
#define _STR_RIGHTBAR_ITEM_PPTMODEL		_T("PPTģ��")


#define _STR_RIGHTBAR_LOCAL_ITEM_SELECT_FOLDER				_T("����ѡ�л����½�һ�������ļ���")
#define _STR_RIGHTBAR_QUESTION_ITEM_PAGE_SAVE_FAILED		_T("����page.xml����ʧ��")
#define _STR_RIGHTBAR_QUESTION_ITEM_MAIN_SAVE_FAILED		_T("����main.xml����ʧ��")


#define _STR_LOCAL_ITEM_TIP_FILE_NO_EXIST			_T("�ļ��Ѿ�������")

#define _STR_PPTCTRLMGR_TIP_PPT_INIT_FAILED			_T("��ʼ��ʧ�ܣ��밲װ���޸�PowerPoint")
#define _STR_PPTCTRLMGR_TIP_PPT_NOT_OPEN			_T("���ȴ򿪻����½�һ��PPT��ʾ�ĸ�")
#define _STR_PPTCTRLMGR_TIP_INSERT_FAILED			_T("����ʧ��")
#define _STR_PPTCTRLMGR_TIP_INSERT_SUCESS			_T("����ɹ�")
#define _STR_PPTCTRLMGR_TIP_FINAL_PPT				_T("��ǰ�ĵ�Ϊ����״̬�ĵ�")
#define _STR_PPTCTRLMGR_TIP_MODIFY_PPT_EDITING		_T("�����л���PPT�ı༭״̬")
#define _STR_PPTCTRLMGR_TIP_OPEN_PPT_FAILED			_T("PPT��ʾ�ĸ��ʧ��")
#define _STR_PPTCTRLMGR_TIP_SLIDE_NO_SELECT			_T("����ѡ��һҳ�õ�Ƭ")

#define _STR_PPTCTRL_TIP_APP_EXCEPTION				_T("CPPTController�쳣��������")

#define _STR_PPT_DOC_IS_EMPTY						_T("PPT��ʾ�ĸ�����Ϊ��")
#define _STR_PACK_PPT_DOC_IS_EMPTY					_T("��Ǹ����ǰû��PPT�ĸ����ݿ��Ա���")
#define	_STR_POWER_POINT_NOT_ACTIVE					_T("�Ի����ʱ��PowerPoint �޷�ִ�иò��������ȹرնԻ���Ȼ�����")
#define	_STR_FILE_OPENED							_T("��ǰ�ĵ��Ѿ���")
#define	_STR_FILE_OPENEING							_T("���ڴ��ĵ������Ժ�...")
#define	_STR_FILE_OPEN_FAST							_T("��̫��Ƶ�������Ժ�...")
#define	_STR_FILE_OPEN_FAILED						_T("�ĵ�·��̫������ʧ��")
#define	_STR_FILE_DWONLOAD_FAILED					_T("�ļ�����ʧ��")
#define _STR_FILE_EXPORT_IMAGES						_T("��������3D��ʾ�ĸ壬���Ժ�...")

#define _STR_SELECT_FILE_BUFFER_SMALL				_T("ѡ���ļ�����, ������ѡ��")
#define _STRT_QUESTION_EXIST						_T("��Ǹ��ϰ��ҳ���ܲ�����Դ")
#define _STR_QUESTION_INSERTING						_T("ϰ�������...")
#define _STR_QUESTION_SAVING						_T("ϰ�Ᵽ����...")
#define _STR_MAGICPEN_FIRSTTIP						_T("�������϶�ѡ��Ŵ�����")
#define _STR_QUESTION_TYPE_EMPTY					_T("��ǰϰ�ⲻ֧�ֱ༭")
#define _STR_VR_PACKAGE_NOT_FIND					_T("û�ж�Ӧ���ƶ�����Դ")

#define _STR_NO_PERMISSION							_T("��ǰȨ���޷�ʹ��")
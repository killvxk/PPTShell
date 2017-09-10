#pragma once
#include "Util/Util.h"
#include "ThirdParty/json/json.h"
#include "ThirdParty/Tinyxml/tinyxml.h"
#include "Util/Singleton.h"

#define			INI_CONFIG_FILE_NAME		_T("Config.ini")
#define			CONFIG_FOLDER_NAME			_T("Setting")
#define			JSON_LOCAL_FILE_NAME		_T("LocalFile.dat")

#define			JK_LOCAL_FOLDER				_T("Folder")
#define			JK_LOCAL_FILES				_T("File")
#define			JK_NAME						_T("Name")

enum
{
	MODULE_TYPE_MOBILE_CONNECT,
	MODULE_TYPE_LOGIN,
	MODULE_TYPE_QUESITON,
	MODULE_TYPE_ND_ICRPLAY,
	MODULE_TYPE_MAX
};

#define			MODULE_MOBILE_CONNECT						_T("Module_MobileConnect")
#define			MODULE_LOGIN								_T("Module_Login")
#define			MODULE_QUESITON								_T("Module_Question")
#define			MODULE_COURSEWAREOBJESTS					_T("Module_CoursewareObjects")
#define			MODULE_ND_ICRPLAY							_T("Module_ND_ICRPlay")


class CConfig
{
public:
	CConfig(void);
	~CConfig(void);
public:
	BOOL		SaveString(tstring strSection, tstring strKey, tstring strContent);
	tstring		GetString(tstring strSection, tstring strKey);
	BOOL		SaveInt(tstring strSection, tstring strKey, int nValue);
	int			GetInt(tstring strSection, tstring strKey);

	BOOL		GetLocalFileJosn(map<tstring, list<LOCAL_RES_INFO>>& mapLocalFile);	//zcs 11-26
	BOOL		WriteLocalFileJosn(map<tstring, list<LOCAL_RES_INFO>>& mapLocalFile);

	tstring		GetVersion();
	
	//
	// ��ȡģ���Ƿ���ʾ
	//
	BOOL		GetModuleVisible(LPCTSTR szModule);

	//
	// ���õ�ǰ�������״̬
	//
	void		SetUpdatedStatus(int nUpdate);

	//
	// ��ȡ��ǰ�������״̬
	//
	DWORD		GetUpdatedStatus();

	//
	// ���ü����½��״̬
	//
	void		SetCheckUpdateStatus(int nUpdate);

	//
	// ��ȡ�����½��״̬
	//
	DWORD		GetCheckUpdateStatus();

	//
	// ���ü�����ʱ��
	//
	void		SetCheckUpdateCount();

	//
	// ������������ʱ��
	//
	void		SetStartAppCount();

	//
	// ��ȡ������ʱ��
	//
	DWORD		GetCheckUpdateCount();

	//
	// ��ȡ��������ʱ��
	//
	DWORD		GetStartAppCount();
protected:
	BOOL			Init();
	tstring			m_strConfigPath;
	tstring			m_strConfigName;
	tstring			m_strLocalFilePath;
	tstring			m_strVersionPath;

	tstring			m_strVerion;

	bool			m_bDocModuleVisible;
	TiXmlDocument	m_docModuleVisible;

	DWORD			m_dwCheckUpdateStatus;
	DWORD			m_dwUpdatedStatus;

	DWORD			m_dwCheckUpdateCount;
	DWORD			m_dwStartAppCount;
};

typedef Singleton<CConfig>		g_Config;

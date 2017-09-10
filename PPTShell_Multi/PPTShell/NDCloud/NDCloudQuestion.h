//-----------------------------------------------------------------------
// FileName:				NDCloudQuestion.h
//
// Desc:
//------------------------------------------------------------------------
#ifndef _NDCLOUD_QUESTION_H_
#define _NDCLOUD_QUESTION_H_

#include "Util/Singleton.h"


typedef struct tagQuestionResType
{
	TCHAR* szQuestionResType;			 // ����
	TCHAR* szQuestionResDescription;      // ����
	
}TagQuestionResType,*PTagQuestionResType;

enum tagQuestionJsonType{
	BASIC_QUESTION = 1,
	INTERACT_QUESTION
};
enum tagQuestionOptType{
	QUESTIONTYPE_NOEXIST = 1,
	QUESTIONTYPE_EXIST
};

typedef struct tagQuestionJsonResType
{
	tagQuestionJsonType nQuestionType;                       //�Ƿ�Ϊ����ϰ��
	tstring szQuestionResCode;			 //����
	tstring szQuestionResType;			 //����
	tstring szQuestionResLabel;			 //����

}TagQuestionJsonResType,*PTagQuestionJsonResType;

//static tagQuestionResType tagQuestionResTypeList[] = 
//{
//	{_T("��ѡ��"),_T("choice")},
//	{_T("��ѡ��"),_T("multiplechoice")},
//	{_T("ͶƱ��"),_T("vote")},
//	{_T("�����"),_T("textentry")},
//	{_T("������"),_T("order")},
//	{_T("������"),_T("match")},
//	{_T("�ж���"),_T("judge")},
//	{_T("�ʴ���"),_T("extendedtext")},
//	{_T("ƴͼ��"),_T("graphicgapmatch")},
//	{_T("����� "),_T("textentrymultiple")},
//	{_T("������"),_T("drawing")},
//	{_T("��д��"),_T("handwrite")},
//	{_T("��������"),_T("gapmatch")},
//	{_T("�ı�ѡ����"),_T("inlinechoice")},
//	{_T("������"),_T("data")},
//	{_T("�Ķ���"),_T("reading")},
//	{_T("�ۺ�ѧϰ��"),_T("comprehensivelearning")},
//	{_T("Ӧ����"),_T("application")},
//	{_T("������"),_T("calculation")},
//	{_T("�����"),_T("explain")},
//	{_T("�Ķ����"),_T("readingcomprehension")},
//	{_T("ʵ����̽����"),_T("experimentandinquiry")},
//	{_T("֤����"),_T("proof")},
//	{_T("���������������"),_T("specialcomplextext")},
//	{_T("�ƶ���"),_T("inference")},
//};

static tagQuestionResType tagQuestionResTypeList[] = 
{
	{_T("multiplechoice"),		_T("��ѡ��")},
	{_T("choice"),				_T("��ѡ��")},
	{_T("textentry"),			_T("�����")},
	{_T("graphicgapmatch"),		_T("ƴͼ��")},
	{_T("match"),				_T("������")},
	{_T("order"),				_T("������")},
	{_T("judge"),				_T("�ж���")},	
	{_T("drawing"),				_T("������")},
	{_T("handwrite"),			_T("��д��")},
	{_T("data"),				_T("������")},
	{_T("vote"),				_T("ͶƱ��")},
	{_T("linkup"),				_T("������")},
	{_T("extendedtext"),		_T("�ʴ���")},
	{_T("nd_fillblank"),		_T("ѡ�������")},
	{_T("nd_table"),			_T("��������")},
	{_T("nd_arithmetic"),		_T("��ʽ������")},
	{_T("nd_memorycard"),		_T("���俨Ƭ")},
	{_T("nd_guessword"),		_T("�´���Ϸ��")},
	{_T("nd_classified"),		_T("������")},
	{_T("nd_fraction"),			_T("��ʽ�Ӽ���")},
	{_T("nd_textselect"),		_T("�ı�ѡ����")},
	{_T("nd_magicbox"),			_T("ħ������")},
	{_T("nd_order"),			_T("������")},
	{_T("nd_wordpuzzle"),		_T("������Ϸ��")},
	{_T("nd_compare"),			_T("�ȴ�С��")},
	{_T("nd_pointsequencing"),	_T("��������")},
	{_T("nd_imagemark"),		_T("��ǩ��")},
	{_T("nd_probabilitycard"),	_T("�鿨��")},
	{_T("nd_catchball"),		_T("������")},
	{_T("nd_balance"),			_T("��ƽ")},
	{_T("nd_handwritequestion"),_T("��д��")}   //�������д������ 2016-01-13
	
};


static tagQuestionResType tagQuestionResTypeListEx[] = 
{
	{_T("choice"),				_T("$RE0201")},
	{_T("multiplechoice"),		_T("$RE0202")},
	{_T("textentry"),			_T("$RE0209")},
	{_T("extendedtext"),		_T("$RE0206")},
	{_T("graphicgapmatch"),		_T("$RE0207")},
	{_T("match"),				_T("$RE0205")},
	{_T("order"),				_T("$RE0204")},
	{_T("judge"),				_T("$RE0203")},	
	{_T("textentrymultiple"),	_T("$RE0216")},	
	{_T("drawing"),				_T("$RE0211")},
	{_T("handwrite"),			_T("$RE0210")},
	{_T("data"),				_T("$RE0208")},
	{_T("vote"),				_T("$RE0225")},
	{_T("gapmatch"),			_T("$RE0215")},
	{_T("inlinechoice"),		_T("$RE0217")},
	{_T("linkup"),				_T("$RE0401")},
	{_T("nd_order"),			_T("$RE0402")},
	{_T("nd_table"),			_T("$RE0403")},
	{_T("nd_wordpuzzle"),		_T("$RE0406")},
	{_T("nd_memorycard"),		_T("$RE0407")},
	{_T("nd_arithmetic"),		_T("$RE0408")},
	{_T("nd_compare"),			_T("$RE0409")},
	{_T("nd_guessword"),		_T("$RE0410")},
	{_T("nd_magicbox"),			_T("$RE0411")},
	{_T("nd_textselect"),		_T("$RE0414")},
	{_T("nd_classified"),		_T("$RE0415")},
	{_T("nd_fraction"),			_T("$RE0416")},
	{_T("nd_pointsequencing"),	_T("$RE0418")},
	{_T("nd_fillblank"),		_T("$RE0421")},
	{_T("nd_sequencefill"),		_T("$RE0423")},
	{_T("nd_imagemark"),		_T("$RE0424")},
	{_T("nd_highlightmark"),	_T("$RE0425")},
	{_T("nd_clock"),			_T("$RE0431")},
	{_T("nd_lego"),				_T("$RE0432")},
	{_T("nd_puzzle"),			_T("$RE0442")},
	{_T("nd_HandwriteQuestion"),_T("$RE0445")},
	{_T("nd_comicdialogue"),	_T("$RE0446")},


};

class CNDCloudQuestion
{
public:
	CNDCloudQuestion(tstring strGuid, DWORD dwUserId, CHttpDelegateBase& OnCompleteDelegate, CHttpDelegateBase& OnProgressDelegate,void* pUserData=NULL);
	~CNDCloudQuestion();

	bool PollPackState();
	bool Cancel();
	bool Pause(CHttpDelegateBase* pDelegate);
	bool Resume();

	bool OnPackStateObtained(void* param);
	bool OnPackStarted(void* param);

	bool OnQuestionDownloaded(void* param);
	bool OnQuestionDownloading(void* param);


protected:
	tstring					m_strGuid;
	DWORD					m_dwUserId;
	BOOL					m_bPolling;
	BOOL					m_bPolled;
	BOOL					m_bFinished;		
	BOOL					m_bPacking;
	void*					m_pUserData;
	int						m_nPendingCount;

	CHttpEventSource		m_OnProgress;
	CHttpEventSource		m_OnComplete;

	
	DWORD					m_dwDownloadId;
	
};

//
// CNDCloudQuestionManager
//
class CNDCloudQuestionManager
{
private:
	CNDCloudQuestionManager();
	~CNDCloudQuestionManager();

public:
	BOOL	Initialize();
	BOOL	Destroy();
	BOOL	DownloadQuestion(tstring strQuestionGuid, DWORD dwUserId, CHttpDelegateBase& OnCompleteDelegate, CHttpDelegateBase& OnProgressDelegate,void* pUserData=NULL);
	BOOL	CancelDownload(tstring strQuestionGuid);
	BOOL	PauseDownload(tstring strQuestionGuid, CHttpDelegateBase* pDelegate = NULL);
	BOOL	ResumeDownload(tstring strQuestionGuid);

	BOOL	AddQuestion(tstring strChapterGuid, tstring strQuestionType, 
						tstring strMaterialGuid, tstring strCategories, 
						bool bBasicQuestion,CHttpDelegateBase& delegate);

	DWORD	PackQuestion(tstring strQuestionGuid, DWORD dwUserId, CHttpDelegateBase& OnCompleteDelegate);

	tstring ComposeBasicQuestionEditUrl(tstring strTokenInfo, tstring strUserId, tstring strQuestionGuid, tstring strChapterGuid);
	tstring ComposeInteractQuestionEditUrl(tstring strTokenInfo, tstring strUserId, tstring strQuestionGuid, tstring strChapterGuid, tstring strQuestionType);

	tstring FindQuestionResType(tagQuestionOptType ntype,tstring nUrl);
	TCHAR*  GetQuestionResType(LPCTSTR szQuestionResDescription);
	TCHAR*  GetQuestionResDesc(LPCTSTR szQuestionResType);
	
	BOOL	ConvertQuestionToCourseFile(tstring strGuid, tstring strQuestionType, tstring strQuestionName, tstring strPath);

	BOOL    LoadJsonQuestionResType();
	DECLARE_SINGLETON_CLASS(CNDCloudQuestionManager);


protected:
	void PollPackState();
	static DWORD WINAPI PollPackStateThread(LPARAM lParam);
	
	bool OnQuestionAdded(void* param);
	bool TripleDESEncrypt(unsigned char* data,int datalen, unsigned char* outdata,int* outdatalen, const char* password);
 

protected:
	HANDLE									m_hThread;
	HANDLE									m_hEvent;
	CRITICAL_SECTION						m_Lock;

	// for add question
	bool                                    m_bLoadJson;
	bool									m_bBasicQuestion;
	tstring									m_strUserId;
	tstring									m_strChapterGuid;
	tstring									m_strQuestionType;
	CHttpEventSource						m_AddQuestionNotify;

	map<tstring, CNDCloudQuestion*>			m_mapQuestions;
	vector<TagQuestionJsonResType>			m_vQuestionsType;
	
};

typedef Singleton<CNDCloudQuestionManager>		NDCloudQuestionManager;

#endif
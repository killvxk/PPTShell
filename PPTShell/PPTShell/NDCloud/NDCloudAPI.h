//-----------------------------------------------------------------------
// FileName:				NDCloudAPI.h
//
// Desc:
//------------------------------------------------------------------------
#ifndef _ND_CLOUD_API_H_
#define _ND_CLOUD_API_H_

#include "Http/HttpDelegate.h"
#include "Http/HttpDownload.h"
#include "Http/HttpUpload.h"
#include "Util/Stream.h"
#include "CategoryTree.h"
#include "ChapterTree.h"
#include "NDCloudFile.h"


enum ResFileType
{
	CloudFileSearch = 0,
	CloudFileCourse,
	CloudFileVideo,
	CloudFileImage,
	CloudFileFlash,
	CloudFileVolume,
	CloudFileQuestion,
	CloudFileCoursewareObjects,
	CloudFilePPTTemplate,
	CloudFileNdpCourse,
	CloudFile3DResource,
	CloudFileNdpxCourse,
	CloudFileVRResource,
	CloudFileAssets,
	CloudFileTotal,
	CloudFileImageEx,

	LocalFileSearch = 50,
	LocalFileImport,
	LocalFileCourse,
	LocalFileVideo,
	LocalFileImage,
	LocalFileFlash,
	LocalFileVolume,
	LocalFileBasicExercises,
	LocalFileInteractiveExercises,
	LocalFileAnother,
	LocalFileTotal,

	DBankSearch = 100,
	DBankUpload,
	DBankCourse,
	DBankVideo,
	DBankImage,
	DBankFlash,
	DBankVolume,
	DBankQuestion,
	DBankCoursewareObjects,
	DBankPPTTemplate,
	DBankNdpCourse,
	DBank3DResource,
	DBankNdpxCourse,
	DBankAssets,
	DBankTotal,

	ResFileTotal,


	DBankLogin,
	DBankRegister,
	SearchALLRes,	//����ȫ����Դ
};


enum CourseFileType
{
	CourseFileXml,
	CourseFileThumb,
	CourseFilePPT,
};

//---------------------------------------------------------------------
// ������������ع���
//
//
BOOL		NDCloudInitialize(CHttpDownloadManager* pHttpManager);
BOOL		NDCloudDestroy();

DWORD		NDCloudDownload(tstring strUrl, CHttpDelegateBase& OnCompleteCB, void* pUserData = NULL);
DWORD		NDCloudDownload(tstring strUrl, CHttpDelegateBase& OnCompleteCB, CHttpDelegateBase& OnProgressCB, void* pUserData = NULL);
DWORD		NDCloudDownloadEx(tstring strUrl, tstring strHeader, tstring strMethod, string strPost, CHttpDelegateBase& OnCompleteCB, void* pUserData = NULL);

DWORD		NDCloudDownloadFile(tstring strUrl, tstring strGuid, tstring strName, int nCloudFileType, int nThumbSize, CHttpDelegateBase& OnCompleteCB, void* pUserData = NULL, tstring strMD5 = _T(""));
DWORD		NDCloudDownloadFile(tstring strUrl, tstring strGuid, tstring strName, int nCloudFileType, int nThumbSize, CHttpDelegateBase& OnCompleteCB, CHttpDelegateBase& OnProgressCB, void* pUserData = NULL, tstring strMD5 = _T(""));
DWORD		NDCloudDownloadCourseFile(tstring strUrl, tstring strGuid, tstring strPPTName, int nCourseFileType, CHttpDelegateBase& OnCompleteCB, void* pUserData = NULL, tstring strMD5 = _T(""));
DWORD		NDCloudDownloadCourseFile(tstring strUrl, tstring strGuid, tstring strPPTName, int nCourseFileType, CHttpDelegateBase& OnCompleteCB, CHttpDelegateBase& OnProgressCB, void* pUserData = NULL, tstring strMD5 = _T(""));

BOOL		NDCloudDownloadPriority(DWORD dwTaskId, int nPriority);
BOOL		NDCloudDownloadCancel(DWORD dwTaskId, CHttpDelegateBase* pOnCompleteDelegate = NULL, CHttpDelegateBase* pOnProgressDelegate = NULL);
BOOL		NDCloudDownloadPause(DWORD dwTaskId, CHttpDelegateBase* pOnPauseDelegate = NULL);
BOOL		NDCloudDownloadResume(DWORD dwTaskId);

//---------------------------------------------------------------------
// Ŀ¼�� �½�����ȫ�ֱ��������ӿ�
//
tstring		NDCloudGetBookGUID();
tstring		NDCloudGetChapterGUID();
BOOL		NDCloudSetChapterGUID(tstring strGUID, tstring strChapterName, bool bInit = false);
BOOL		NDCloudGetCategoryTree(CCategoryTree*& pCategory);
BOOL		NDCloudGetChapterTree(CChapterTree*& pChapter);
tstring		NDCloudPrevChapter(BOOL& bFirst);
tstring		NDCloudCurrentChapter();
tstring		NDCloudNextChapter(BOOL& bLast);
void		NDCloudSaveChapterTrees(CChapterTree* pChapter);
void		NDCloudSaveCategoryTrees(CCategoryTree* pCategory);
BOOL		NDCloudLoadTrees();
BOOL		NDCloudClearTrees();
BOOL		NDCloudSaveTrees();
BOOL		NDCloudCurrentPos(BOOL& bFirst, BOOL& bLast);


//---------------------------------------------------------------------
// URL��ַ�����ع���
//

//
// ����: ��ϲ�ѯѧ�Σ��꼶���γ̣������磬���²����Ϣ�ĵ�ַ
//
// ����:
// strGradeCode:	�꼶���� (����Сѧһ��͵ı���)
// strCourseCode:	�γ̱��� (�������Ŀεı���)
//
// ��ע:			�������������Ϊ�յĻ������ʾһ���Ի�ȡСѧһ�꼶�����ݣ�
//					���ⲻ��ֻ�ṩ�γ̱�������ṩ��ͱ���
//
tstring		NDCloudComposeUrlCategory(tstring strGradeCode = _T(""), tstring strCourseCode = _T(""));


//
// ����:			��ȡ�鱾��Ϣ (������GUID���)
//
// ����:			
// strSectionCode:	�꼶�� (Сѧ���и���)
// strGradeCode:	��� (һ�꼶 ���꼶)	
// strCourseCode:	ѧ�� (���� ��ѧ)
// strVersionCode:	�̲İ汾 
// strSubVersionCode: �̲��Ӱ汾
//
tstring		NDCloudComposeUrlBookInfo(tstring strSectionCode, tstring strGradeCode, tstring strCourseCode, tstring strEditionCode, tstring strSubEditionCode, int nLimitStart, int nLimitEnd);


//
// ����:			��ȡ�鱾�½���Ϣ  
//
// ����:			
// strBookGUID:		�鱾��GUID���
//
tstring		NDCloudComposeUrlChapterInfo(tstring strBookGUID);


//
// ����:			��ȡ�μ�����
//
// ����:
// strSearchKeyWord: �����ؼ��֣����Ϊ�ձ�ʾ�ܿμ���������Ϊ�������Ŀμ�����
//
tstring		NDCloudComposeUrlCourseCount(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T(""));

//
// ����:			��ȡ�μ���Ϣ
//
// ����:			
// strSearchKeyWord:�����ؼ���			
// nLimitStart:		�����μ���Ϣ��ʼ����
// nLimitEnd:		�����μ���Ϣ��������
//
tstring		NDCloudComposeUrlCourseInfo(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T(""), int nLimitStart = 0, int nLimitEnd = 0, DWORD dwUserId = 0);


tstring		NDCloudComposeUrlPictureCount(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T(""));
tstring		NDCloudComposeUrlPictureInfo(tstring strChapterGUID, tstring strSearchKeyWord, int nLimitStart, int nLimitEnd, DWORD dwUserId = 0);

tstring		NDCloudComposeUrlVolumeCount(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T(""));
tstring		NDCloudComposeUrlVolumeInfo(tstring strChapterGUID, tstring strSearchKeyWord, int nLimitStart, int nLimitEnd, DWORD dwUserId = 0);

tstring		NDCloudComposeUrlVideoCount(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T(""));
tstring		NDCloudComposeUrlVideoInfo(tstring strChapterGUID, tstring strSearchKeyWord, int nLimitStart, int nLimitEnd, DWORD dwUserId = 0);

tstring		NDCloudComposeUrlFlashCount(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T(""));
tstring		NDCloudComposeUrlFlashInfo(tstring strChapterGUID, tstring strSearchKeyWord, int nLimitStart, int nLimitEnd, DWORD dwUserId = 0);

tstring		NDCloudComposeUrlPPTTemplateCount(tstring strSectionCode = _T(""), tstring strCourseCode = _T(""), tstring strSearchKeyWord = _T(""));
tstring		NDCloudComposeUrlPPTTemplateInfo(tstring strSectionCode, tstring strCourseCode, tstring strSearchKeyWord, int nLimitStart, int nLimitEnd, DWORD dwUserId = 0);

tstring		NDCloudComposeUrl3DResourceCount(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T(""));
tstring		NDCloudComposeUrl3DResourceInfo(tstring strChapterGUID, tstring strSearchKeyWord, int nLimitStart, int nLimitEnd, DWORD dwUserId = 0);

tstring		NDCloudComposeUrlVRResourceCount(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T(""), tstring strCategory = _T(""));
tstring		NDCloudComposeUrlVRResourceInfo(tstring strChapterGUID, tstring strSearchKeyWord, tstring strCategory, int nLimitStart, int nLimitEnd, DWORD dwUserId = 0);

tstring		NDCloudComposeUrlQuestionCount(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T(""));
tstring		NDCloudComposeUrlQuestionInfo(tstring strChapterGUID, tstring strSearchKeyWord, int nLimitStart, int nLimitEnd, DWORD dwUserId = 0);
tstring		NDCloudComposeUrlPackQuestion(tstring strQuestionGUID, DWORD dwUserId);
tstring		NDCloudComposeUrlPackQuestionState(tstring strQuestionGUID, DWORD dwUserId);

tstring		NDCloudComposeUrlCoursewareObjectsCount(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T(""));
tstring		NDCloudComposeUrlCoursewareObjectsInfo(tstring strChapterGUID, tstring strSearchKeyWord, int nLimitStart, int nLimitEnd, DWORD dwUserId = 0);
tstring		NDCloudComposeUrlPackCoursewareObjects(tstring strGUID, DWORD dwUserId);
tstring		NDCloudComposeUrlPackCoursewareObjectsState(tstring strGUID, DWORD dwUserId);

tstring		NDCloudComposeUrlToolsCount(tstring strChapterGUID = _T(""), tstring strSearchKeyWord = _T("") );
tstring		NDCloudComposeUrlToolsInfo(tstring strChapterGUID, tstring strSearchKeyWord, int nLimitStart, int nLimitEnd);

tstring		NDCloudComposeUrlCagegoryInfo(tstring strSearchKeyWord, int nLimitStart, int nLimitEnd);
//---------------------------------------------------------------------
// Json�ַ�������������
//
int			NDCloudDecodeCount(IN char* pData, IN int nDataSize);

int			NDCloudDecodeExercisesCount(char* pData, int nDataSize);

//
// ����:			��ȡĿ¼��Ϣ
//
// ����:
// CourseTree:		����Ŀ¼��������ֱ�Ӳ������� 
//
// ��ע:			ʹ����������delete�����ͷ�pOutputStream 
//
BOOL		NDCloudDecodeCategory(IN char* pData, IN int nDataSize, OUT CCategoryTree*& pCourseTree);

BOOL		NDCloudDecodeCategory(IN char* pData, IN int nDataSize,IN tstring strSectionCode, IN tstring strGradeCode, IN tstring strCourseCode, IN tstring strEditionCode);

//
// ����:			��ȡ�鱾GUID
//
// ����:
//
//
BOOL		NDCloudDecodeBookGUID(IN char* pData, IN int nDataSize,tstring& strBookGUID);


//
// ����:			��ȡ�½���Ϣ
//
// ����:
//
//
BOOL		NDCloudDecodeChapterInfo(IN char* pData, IN int nDataSize, IN CChapterTree* pChapterTree);


//
// ����:			��ȡͼƬ�б���Ϣ
//
// ����:		
// ImageStream:		count|guid1|title1|url1|guid2|title2|url2...
//
BOOL		NDCloudDecodeImageList(IN char* pData, IN int nDataSize, OUT CStream* pImageStream);

//
// ����:			��ȡ��Ƶ�б���Ϣ
//
// ����:		
// VolumeStream:	count|title1|url1|title2|url2...
//
BOOL		NDCloudDecodeVolumeList(IN char* pData, IN int nDataSize, OUT CStream* pVolumeStream);


//
// ����:			��ȡ��Ƶ�б���Ϣ
//
// ����:		
// VideoStream:		count|title1|url1|title2|url2...
//
BOOL		NDCloudDecodeVideoList(IN char* pData, IN int nDataSize, OUT CStream* pVideoStream);


//
// ����:			��ȡ�����б���Ϣ
//
// ����:		
// VideoStream:		count|title1|url1|title2|url2.
//
BOOL		NDCloudDecodeFlashList(IN char* pData, IN int nDataSize, OUT CStream* pFlashStream);

//
// ����:			��ȡPPTģ���б���Ϣ
//
// ����:		
// VideoStream:		count|title1|guid1|pptUrl1|previews_count1|previews1|title2|guid2|pptUrl2|previews_count2|previews2|
//
BOOL		NDCloudDecodePPTTemplateList(IN char* pData, IN int nDataSize, OUT CStream* pPPTTemplateStream);


//
// ����:			��ȡ�μ��б���Ϣ
//
// ����:		
// VideoStream:		count|title1|guid1|pptUrl1|previews_count1|previews1|title2|guid2|pptUrl2|previews_count2|previews2|
//
BOOL		NDCloudDecodeCourseList(IN char* pData, IN int nDataSize, OUT CStream* pCourseStream);

//
// ����:			��ȡϰ���б���Ϣ
//
// ����:		
// VideoStream:		count|title1|guid1|dercription1|questiontype1|preview1|title2|guid2|dercription2|questiontype2|preview2
//
BOOL		NDCloudDecodeQuestionList(IN char* pData, IN int nDataSize, OUT CStream* pQuestionStream);

//
// ����:			��ȡ���������б���Ϣ
//
// ����:		
// 
//
BOOL		NDCloudDecodeCoursewareObjectsList(IN char* pData, IN int nDataSize, OUT CStream* pCoursewareObjectsStream);

//
// ����:			��ȡPPT����ͼ�б���Ϣ  
//
// ����:		
// VideoStream:		count|name1|ur1|name2|ur2|....
//
BOOL		NDCloudDecodePPTThumbnailList(IN char* pData, IN int nDataSize, OUT CStream* pThumbStream);

//
// ����:			��ȡ3D��Դ�б���Ϣ
//
// ����:		
// VideoStream:		count|title1|guid1|description1|PreviewUrl1|fileUrl1 
//
BOOL		NDCloudDecode3DResourceList(IN char* pData, IN int nDataSize, OUT CStream* p3DResourceStream);

//
// ����:			��ȡVR��Դ�б���Ϣ
//
// ����:		
// VideoStream:		count| guid | title | description | fileUrl |  PreviewUrl | apkPackageName | apkActivityName
//
BOOL		NDCloudDecodeVRResourceList(IN char* pData, IN int nDataSize, OUT CStream* pVRResourceStream, BOOL bIncludeEmptyVR = TRUE);

//
// ����:			��ȡѧ����Ϣ
//
// ����:		
// 
//
BOOL		NDCloudDecodeCategoryList(IN char* pData, IN int nDataSize, OUT CStream* pStream);

#endif
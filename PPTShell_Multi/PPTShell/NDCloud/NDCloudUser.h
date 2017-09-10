//-----------------------------------------------------------------------
// FileName:				NDCloudUser.h
//
// Desc:
//------------------------------------------------------------------------
#ifndef _ND_CLOUD_USER_H_
#define _ND_CLOUD_USER_H_

#include "Util/Singleton.h"
#include "Util/Stream.h"
#include "Util/RSA.h"
#include "Http/HttpDelegate.h"
#include "Http/HttpDownload.h"
#include "Http/HttpDownloadManager.h"
#include "json/forwards.h"
#include "json/value.h"

enum
{
	UCSTEP_NONE					= 0,
	UCSTEP_LOGIN,	
	UCSTEP_USERINFO,
	UCSTEP_CLASSROOM,
	UCSTEP_STUDENTINFO,
	UCSTEP_GROUPINFO,
	UCSTEP_DEVICEINFO,
};

enum
{
	BEARER_TOKEN				= 0,
	CHECK_BEARERTOKEN
};

typedef struct _GetBackPwdParam
{
	DWORD dwErrorCode;
	tstring strCode;
	tstring strAccount;
	tstring strEmail;
	tstring strPhone;
	tstring strMessage;
	tstring strTempPwd;
}GetBackPwdParam, *pGetBackPwdParam;

class CNDCloudUser
{
public:
	CNDCloudUser();
	~CNDCloudUser();

public:
	vector<Json::Value> GetGradeList();
	BOOL			Login(tstring strUserName, tstring strPassword, tstring strOrganize, CHttpDelegateBase& delegate);
	void				LogOut();//�˳���¼ 2015.11.20 cws
	BOOL			MobileLogin(CStream *loginStream, CHttpDelegateBase& delegate);
	BOOL			ThirdLogin(tstring strUserName, tstring strPassword, tstring strOrganize, CHttpDelegateBase& delegate);
	BOOL			TokenLogin(tstring strHost, tstring strUrl, tstring strMethod, tstring strMac, tstring strNonce, tstring strToken, CHttpDelegateBase& delegate);
	void				UndoMobileLogin();

	BOOL			Register(tstring strUserName, tstring strPassword, tstring strCheckCode, CHttpDelegateBase& delegate);
	BOOL			ChangePassword(tstring strOldPassword, tstring strNewPassword, CHttpDelegateBase& delegate);
	BOOL			QueryLoginNameExist(tstring strUserName, CHttpDelegateBase& delegate);
	BOOL			ObtainCheckCodeImage(CHttpDelegateBase& delegate);
	BOOL			SendRegisterShortMessage(tstring strPhoneNumber, CHttpDelegateBase& delegate);
	BOOL			SendActiveEmail(tstring strEmail, CHttpDelegateBase& delegate);
	string			MD5EncryptString(string content);
	BOOL			IsLogin();		//�ж��û��Ƿ��ѵ�¼

	// bearer_tokens_login
	BOOL			BearerTokensLogin(tstring strSessionId,CHttpDelegateBase& delegate);
	BOOL			GetBearerTokens(int nType);
	tstring			GetBearerToken();
	BOOL			CheckBearerTokens(int nType);

	// avatar
	BOOL			GetUserAvatar(DWORD dwSize, CHttpDelegateBase& delegate);
	BOOL			SetUserAvatar(tstring strFilePath, CHttpDelegateBase& delegate);
	BOOL			SetUserAvatarWithSession();
	BOOL			GetUserAvatarSession();

	// getback password
	BOOL			GetBackPwdConfirmAccount( tstring strUserName, tstring strCheckCode, CHttpDelegateBase& delegate );
	BOOL			GetBackPwdCheckEmail( tstring strEmail, tstring strEmailCode, CHttpDelegateBase& delegate );
	BOOL			GetBackPwdCheckPhone( tstring strPhoneNumber, tstring strPhoneCode, CHttpDelegateBase& delegate );
	BOOL			GetBackPwdResetPassword( tstring strAccount, tstring strPassword, tstring strTempPassword, CHttpDelegateBase& delegate );
	BOOL			SendGetBackPwdSMS( tstring strPhoneNumber, CHttpDelegateBase& delegate );
	BOOL			SendGetBackPwdEmail( tstring strEmail, CHttpDelegateBase& delegate );

	// cancel
	void			CancelLogin();
	void			CancelThirdLogin();
	void			CancelRegister();
	void			CancelQueryLoginNameExist();
	void			CancelObtainCheckCodeImage();
	void			CancelSendRegisterShortMessage();
	void			CancelSendActiveEmail();
	void			CancelOperaUserAvatar();

	// get authorization
	string			GetAuthorizationHeader(string strHost, string strUrl, string strMethod);

	int				GetCurStep();
	BOOL			IsSuccess();
	BOOL			IsLoginNameExist();
	DWORD			GetUserId();
	tstring			GetUserName();
	tstring			GetPassword();
	tstring			GetOriganize();
	tstring			GetErrorCode();
	tstring			GetErrorMessage();
	tstring			GetNickName();
	tstring			GetRealName();
	tstring			GetSex();
	tstring			GetSubjects();
	tstring			GetOrgNodeName();
	tstring			GetFullName();//����ѧУ�Ͱ༶����Ϣ  ʾ��"ѧУ�ڵ�|���ݽ���ѧԺ��������Сѧ|�꼶|С��|7��"	
	tstring			GetTeacherGrade(tstring sFullName);//��ʦ�İ༶��Ϣ 
	void			SetFreeMode(int nFreeMode);
	int				GetIsFreeMode() { return m_nIsFreeMode;}//�Ƿ��������ڿ�ģʽ
	void			SetCurGradeIndex(int nIndex) { m_nGradeIndex = nIndex;}
	int				GetCurGradeIndex() { return m_nGradeIndex;}
	BOOL			GetIsLoadAllTeacherInfo();// �Ƿ��Ѿ�������ȫ��ʦ����Ϣ 2016.02.17 cws
	tstring			GetLoadALLTeacherTip();//�Ƿ������ȫ��Ϣ��ʾ
	void			ReLoadTeachRoomInfo(tstring sClassID);//���ݰ༶ID��Ϣ���¼��ؽ�����Ϣ
	void			ReLoadTeachRoomInfo();//���ݰ༶������Ϣ���¼��ؽ�����Ϣ
	bool			CheckItemsSize(tstring strValue, tstring strItemName);//���json���Ƿ����Ӵ�

	BOOL			IcrOnStartInit(CStream* pStream);//�������ݳ�ʼ�� 2016.02.23 cws
	BOOL			IcrStart();
	CStream*		GetIcrStartInfo() {return m_pFullScreenStream;}

	
	bool			GetLoginComplete() {return m_bLoginComplete;}//��½���
	void			SetLoginComplete(bool bComplete) {m_bLoginComplete = bComplete;}
	void			SetFullName(tstring sNewFullName) { m_strFullName = sNewFullName;}
	CStream*		GetClassRooms();
	void			SetClassGuid(tstring strClassGuid);
	tstring			GetClassGuid();
	tstring			GetLoginResponse();

	tstring			GetStudentInfo();
	tstring			GetGroupInfo();
	tstring			GetDeviceInfo();
	tstring			GetTeacherPhotoPath(){ return m_strTeacherPhoto;}//��ʦͷ��
	void			GetTeacherPhoto();//��ʦͷ��ص�

	tstring			GetAccessToken();
	tstring			GetMacKey();

	int GetLoginType();
	void SetLoginType(int loginType);

	void SetMobileLoginRqstTime(time_t tmNow) { m_tLastMobileRqst = tmNow; }
	time_t GetLastMobileLoginRqstTime() { return m_tLastMobileRqst; }


	DECLARE_SINGLETON_CLASS(CNDCloudUser);

protected:
	bool			OnUserLoginResponse(void* param);
	bool			OnUserBearerLoginResponse(void* param);
	bool			OnUserInfoObtained(void* param);
	bool			OnClassRoomObtained(void* param);
	bool			OnStudentInfoObtained(void* param);
	bool			OnGroupInfoObtained(void* param);
	bool			OnDeviceInfoObtained(void* param);

	bool			OnCheckCodeImageObtained(void* param);
	bool			OnUserRegisterResponse(void* param);
	bool			OnUserChangePassResponse(void* param);
	bool			OnRegisterShortMessageSent(void* param);
	bool			OnLoginExistQueried(void* param);
	bool			OnActiveEmailSent(void* param);
	bool			OnGetBackPwdConfirmAccountResponse(void* param);
	bool			OnGetBackPwdResetResponse(void* param);
	bool			OnResetPasswordResponse(void* param);
	bool			OnMotifyResponse(void* param);
	
	bool			OnBearerTokensLoginResponse(void* param);
	bool			OnGetBearerTokensResponse(void* param);
	bool			OnCheckBearerTokensResponse(void* param);

	//avatar
	bool			OnGetUserAvatarSessionResponse(void* param);
	bool			OnSetUserAvatarResponse(void* param);
	//
	string			GenerateRandomString();
	BOOL			LoadHistoryData();
	BOOL			SaveHistoryData();

	bool			OnCreateUserPhoto(void * pParam);//��ͼƬ�������
	bool			OnGetUserPhoto(void * pParam);//�ӷ���˻�ȡͷ��
	bool			GetUserAvatarStart( void* pParam );
	void			setLoadTeacherInfoState(BOOL bSuccess);//���û�ȡ��ʦ��Ϣ�����״̬
	void			LoadTeachRoomInfo(tstring sOrgID, tstring sClassID);
protected:
	int							m_nCurStep;
	BOOL						m_bSuccess; 
	BOOL						m_bUserInfoSuccess;//��ȡ��ʦ��Ϣ
	BOOL						m_bClassRoomInfoSuccess;//��ȡ��ʦ�༶������Ϣ
	BOOL						m_bStudentInfoSuccess;//��ȡѧ����Ϣ
	BOOL						m_bDeviceInfoSuccess;//��ȡ�豸��Ϣ 
	BOOL						m_bGroupInfoSuccess;//��ȡѧ��������Ϣ 
	BOOL						m_bLoginNameExist;

	CHttpEventSource			m_OnCompleteLogin;
	CHttpEventSource			m_OnCompleteRegister;
	CHttpEventSource			m_OnCompleteChangePassword;
	CHttpEventSource			m_OnCompleteGetBackPwd;
	CHttpEventSource			m_OnCompleteCheckCode;
	CHttpEventSource			m_OnCompleteSendSms;
	CHttpEventSource			m_OnCompleteQueryLoginExist;
	CHttpEventSource			m_OnCompleteSendActiveEmail;
	CHttpEventSource			m_OnCompleteGetAvatar;
	CHttpEventSource			m_OnCompleteSetAvatar;

	tstring						m_strUserName;
	tstring						m_strPassword;
	tstring						m_strOriganize;

	// Login Response
	DWORD						m_dwUserId;
	tstring						m_strMacKey;
	tstring						m_strAccessToken;
	tstring						m_strLoginResponse;

	tstring						m_strErrorCode;
	tstring						m_strErrorMessage;

	// User Information
	tstring						m_strNickName;
	tstring						m_strRealName;
	tstring						m_strSex;
	tstring						m_strSubjects;
	tstring						m_strOrgNodeName;
	tstring						m_strFullName;//ѧУ��ȫ��
	tstring						m_strTeacherGrade;//��ʦ�İ༶��Ϣ
	int							m_nIsFreeMode;//�����ڿ�ģʽ 2016.02.16
	
	tstring						m_strTeacherPhoto;//��ʦͷ��2016.01.28 cws
	int							m_nGradeIndex;//�������Ĵ�����ÿ�����٣�����ʦ�༶������ֵ������ 2016.02.19 cws
	UINT64						m_ulOrgId;
	bool						m_bLoginComplete;//���е�½�������2016.01.27
	INT64						m_dwServerTime;
	INT64						m_dwTickCout;

	// Classroom Information
	CStream*					m_pClassroomStream;
	tstring						m_strClassGuid;

	// Student Information
	CStream*					m_pStudentInfoStream;
	CStream*					m_pGroupInfoStream;
	CStream*					m_pDeviceInfoStream;

	CStream*					m_pFullScreenStream;

	// RSA
	CRSA						m_RSA;

	// Cookie
	tstring						m_CheckCodeSession;

	// Bearer Token
	tstring						m_strBearerSessionId;
	tstring						m_strBearerToken;

	// backup user data to file
	map<DWORD, CStream*>		m_mapUserHistoryData;

	// avatar
	tstring						m_strAvatarSession;
	tstring						m_strAvatarFilePath;
	tstring						m_strAvatarServerPath;
	//

	int m_nLoginType;

	time_t m_tLastMobileRqst;


};

typedef Singleton<CNDCloudUser>		NDCloudUser;

#endif
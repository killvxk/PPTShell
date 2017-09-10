#pragma once

/**
ϰ������·������
*/
#define EXAM_MAX_PATH		1024

/**
ѧ��ID��󳤶�
*/
#define STUDENT_ID_MAX_LENGTH    65

/**
pptshell UI���ID����
*/
#define ICR_UICOMPONET_NAVIGATOR		1                 //PPTShell������UI���
#define ICR_UICOMPONET_TOOLBAR          2               //PPTShell������UI���

/**
�������ð�ťID����
*/
#define ICR_BUTTON_ID_ROSTER            1             //�����ᰴťID
#define ICR_BUTTON_ID_PUSHSCREEN        2             //������ťID
#define ICR_BUTTON_ID_EXAM              3             //������ϰ��ťID
#define ICR_BUTTON_ID_SUMMARY           4             // �����ܽᰴťID
#define ICR_BUTTON_ID_HOMEWORK          5             //������ҵ��ťID
#define ICR_BUTTON_ID_INTERACTIVE_TOOL  6             //�������߰�ťID
#define ICR_BUTTON_ID_VOTE              7            //����ťID
#define ICR_BUTTON_ID_BAIKE             8           //�ٿư�ťID

/**
��ť״̬
*/
#define ICR_UI_STATE_VISIBLE            1                   //��ʾ
#define ICR_UI_STATE_INVISIBLE          2                   //����ʾ
#define ICR_UI_STATE_ENABLE             3                   //����
#define ICR_UI_STATE_DISABLE            4                   //����

/**
��������
*/
#define ICR_SCREEN_QUALITY_LOW          1         //�ͣ���Ļѹ��1��
#define ICR_SCREEN_QUALITY_HIGH          2        //�ߣ���Ļδѹ��

/**
@name IcrRelease ��Icr��Ҫ�ͷ�pptshell������ڴ�ʱ����
@param pAddress �ڴ��ַ
*/
typedef void(__stdcall *LPFN_IcrReleaseCallbackPtr)(void* pAddress);

/**
@name IcrGetScreen ��Ҫ������Ļ����ʱ���ã���֤��UI�߳��е���
@param nQuality ��Ļ����ICR_SCREEN_QUALITY_*
@param pData ��Ļ���ݣ�jpg��ʽ������ʹ��ʱ����IcrRelease�ͷ��ڴ�
@param nSize ���ݴ�С
@return �ɹ���ʧ��
*/
typedef bool(__stdcall *LPFN_IcrGetScreenCallbackPtr)(int nQuality, unsigned char** pData, int& nSize);

/**
@name IcrSendExamEvent ����������ϰ�����¼�����֤��UI�߳��е���
@param szEventName �¼����ƣ�\0��β
@param szEventData �¼����ݣ�\0��β
@return �ɹ���ʧ��
*/
typedef bool(__stdcall *LPFN_IcrSendExamEventCallbackPtr)(const char* szEventName, const char* szEventData);

/**
@name IcrGetTopLevelWindowHandle Icr��Ҫ��ʾ����ʱ���ܻ����ȥ��ȡpptshell���㴰�ھ����Ϊ�����ھ��
@return pptshell���㴰�ھ��
*/
typedef HWND(__stdcall *LPFN_IcrGetTopLevelWindowHandleCallbackPtr)();

/**
@name IcrMessageBox Icr��Ҫ��ʾMessageBoxʱ���ã�����ͬwindows MessageBox
@param szText ��Ϣ����
@param szCaption ����
@param uType MB_OK �� MB_OKCANCEL �� MB_YESNO
@return IDOK �� IDCANCEL �� IDYES �� IDNO
*/
typedef int(__stdcall *LPFN_IcrMessageBoxCallbackPtr)(const char* szText, const char* szCaption, unsigned int uType);

/**
@name IcrShowToast Icr��Ҫ��ʾtoastʱ���ã�toast�Ƕ��������ʾ��Ϣ��Ȼ���Զ���Ϣ���첽ʵ�ֲ������������߳�
@param szText ��Ϣ���ݣ�ʵ�ֱ��뿽��һ����Ϣ����
@param nDuration ��ʾʱ������λ����
*/
typedef void(__stdcall *LPFN_IcrShowToastCallbackPtr)(const char* szText, int nDuration);

/**
@name IcrEnable Icr��⵽��ѧ����������Ϊ�ǻ������ó���ʱ���ø�֪pptshell������������ģʽ
*/
typedef void(__stdcall *LPFN_IcrEnableCallbackPtr)();

/**
@name IcrGetExamType ���������ϰ���жϵ�ǰ���ŵ�ϰ�������Ƿ�֧��ʱ����
@return ϰ�����ͣ�����ʹ��ʱ��IcrRelease�ص��ͷţ�NULL��ʾ����ϰ��
*/
typedef char*(__stdcall *LPFN_IcrGetExamTypeCallbackPtr)();

/**
@name IcrSetButtonState ����PPTShell��ť״̬�����û�����п��ư�ť����ʾ���ؽ�������״̬
@param nUIComponentId   UI���ID ICR_UICOMPONET_*
@param nButtonId    ��ťID��ICR_BUTTON_ID_*��0��ʾ���������UI����״̬
@param nState ״̬ICR_UI_STATE_*
*/
typedef void(__stdcall *LPFN_IcrSetButtonStateCallbackPtr)(int nUIComponentId, int nButtonId, int nState);

/**
@name IcrInit ��ʼ������������ʱ���ã�������UI�߳��е���
@return �ɹ���ʧ��
*/
typedef bool(__stdcall *LPFN_IcrInit)();

/**
@name LPFN_IcrUnInit �����˳�ʱ���ã�һ���������ڹر�ʱ���ã�
*/
typedef void(__stdcall *LPFN_IcrUnInit)();

/**
@name IcrSetCallback ���ûص�������IcrOnStart֮ǰ��������
*/
typedef void(__stdcall *LPFN_IcrSetCallback)(LPFN_IcrReleaseCallbackPtr releasePtr,
    LPFN_IcrGetScreenCallbackPtr getScreenPtr,
    LPFN_IcrSendExamEventCallbackPtr sendExamEventPtr,
    LPFN_IcrGetTopLevelWindowHandleCallbackPtr getTopLevelWindowHandlePtr,
    LPFN_IcrMessageBoxCallbackPtr messageBoxPtr,
    LPFN_IcrShowToastCallbackPtr showToastPtr,
    LPFN_IcrEnableCallbackPtr enableIcrPtr,
    LPFN_IcrGetExamTypeCallbackPtr getExamTypePtr,
    LPFN_IcrSetButtonStateCallbackPtr setButtonStatePtr);

/**
@name IcrOnStart ȫ������ʱ����
@param szParamJson start�������JSON����ʽ��
    exam: ����ϰ������·����ϰ���main.xml���ڵ��ļ���
    is_recovery: ��ʶ�Ƿ�ָ���true��false���������쳣�˳���������������ʦѡ��ָ���֮ǰ״̬ʱ������Ϊtrue
    slide_pos: �õ�Ƭλ�ã���������ʾʱ��Ҫ
    {
	"token":{"access_token": "��¼ʱ�򷵻ص�token", "mac_key":"��¼ʱ����key"},
    "teacher":{"id":"teacher-id", "name" : "nick-name"},
    "class":{"id":"class-id"},
    "chapter":{"subject":"��ĿID", "chapter":"�½�ID", "teaching_material":"�̲�ID"},
    "students":"json���ݲο� http://wiki.sdp.nd/index.php?title=Admin-API%E6%8E%A5%E5%8F%A3-%E8%8E%B7%E5%8F%96%E7%8F%AD%E7%BA%A7%E5%AD%A6%E7%94%9F%E5%88%97%E8%A1%A8",
    "groups":"json���ݲο� http://wiki.sdp.nd/index.php?title=Admin-API%E6%8E%A5%E5%8F%A3-%E8%8E%B7%E5%8F%96%E7%8F%AD%E7%BA%A7%E5%AD%A6%E7%94%9F%E5%88%86%E7%BB%84%E5%88%97%E8%A1%A8",
    "devices":"json���ݲο� http://wiki.sdp.nd/index.php?title=Admin-API%E6%8E%A5%E5%8F%A3-%E8%8E%B7%E5%8F%96%E7%8F%AD%E7%BA%A7%E5%8F%8A%E5%AD%A6%E7%94%9F%E8%AE%BE%E5%A4%87",
    "exam" : [{"path" : "exam-path-full-path1"}],
    "is_recovery" : true,
    "slide_pos" : {"left":0, "top":0, "width":100, "height":100}
	"vr": {"enable: true|false"}
    }
@return �ɹ���ʧ��
*/
typedef bool(__stdcall *LPFN_IcrOnStart)(const char* szParamJson);

/**
@name IcrOnClick �����������߲˵����ϰ�ťʱ���ã�������UI�߳��е���
@param nButtonID ��ťID
*/
typedef void(__stdcall *LPFN_IcrOnClick)(int nButtonID);

/**
@name IcrOnPageIndexChange ��ҳ�¼�֪ͨ��������UI�߳��е���
@param nCurrentPageIndex ��ǰҳ������
@param nNextPageIndex ��һ��ҳ������
@return true��ʾ����ҳ��false��ʾ������ҳ
*/
typedef bool(__stdcall *LPFN_IcrOnPageIndexChange)(int nCurrentPageIndex, int nNextPageIndex);

/**
@name IcrOnRecvExamEvent ����������ϰ���������H5�������¼���������UI�߳��е���
@param szEventName �¼�����
@param szEventData �¼�����
*/
typedef void(__stdcall *LPFN_IcrOnRecvExamEvent)(const char* szEventName, const char* szEventData);

/**
@name IcrGetOnlineStudents ��ȡ��������ѧ����������ϰmodule��Ҫ�༶ѧ����ʱ����ã�������UI�߳��е��ã������ʦ��¼ӵ�л�������Ϣ���ػ����ᣬ���򷵻���������¼��ѧ���б�
@param ppStudentsJson ѧ����ϢJSON��������ʹ�õ�IcrRelease�ͷ�
              JSON��ʽΪ��http://wiki.sdp.nd/index.php?title=%E4%BA%92%E5%8A%A8%E8%AF%BE%E5%A0%82/Native%E6%8E%A5%E5%8F%A3#.E8.8E.B7.E5.8F.96.E5.BD.93.E5.89.8D.E7.8F.AD.E7.BA.A7.E7.9A.84.E6.89.80.E6.9C.89.E5.AD.A6.E7.94.9F.E4.BF.A1.E6.81.AF
*/
typedef void(__stdcall *LPFN_IcrGetOnlineStudents)(char** ppStudentsJson);

/**
@name IcrInvokeNativeMethod JS����native������������UI�߳��е���
@param szMethodJson �������Ƽ�����JSON����utf-8��ʽ��JSON��ʽ���£�
    {
    "method_name":"dohandwrite",
    "param":{"type":"bool", "value":"true|false"},
    "param":{"type":"int", "value":"15"},
    "param":{"type":"double","value":"12.32"}
    "param":{"type":"string", "value":"string-value"}
    }
@return ����ֵJSON�����޷���ֵ���ؿմ���utf-8��ʽ������ʹ�õ���IcrRelease�ͷţ�JSON��ʽ���£�
    {
    "return":{"type":"bool|int|double|string", "value":"true|false|15|12.3|string-value"}
    }
*/
typedef char*(__stdcall *LPFN_IcrInvokeNativeMethod)(const char* szMethodJson);

/**
@name IcrSendCmd ��ѧ����������
@param szCmdJson ������ϢJSON����utf-8��ʽ��JSON��ʽ���£�
    {
    "category":"�������",
    "content": "��������JSON��",
    }

    categoryֵΪVRʱ��content JSON����ʽ���£�
    {    
    "package_name":"com.nd.vr.tianping",
    "activity_name":"TianPingActivity",
    "op":"play|stop"
    }
@return �ɹ���ʧ��
*/
typedef bool(__stdcall *LPFN_IcrSendCmd)(const char* szCmdJson);

/**
@name IcrRelease �ͷ��ڴ�
*/
typedef void(__stdcall *LPFN_IcrRelease)(void* pAddress);

/**
@name IcrStop �˳�ȫ������ʱ���ã�������UI�߳��е���
*/
typedef void(__stdcall *LPFN_IcrStop)();
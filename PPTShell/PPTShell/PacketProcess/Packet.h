//===========================================================================
// FileName:					Packet.h
//	
// Desc:					Packet defination
//===========================================================================
#ifndef _PACKET_H_
#define _PACKET_H_

typedef struct PACKET_HEAD
{
	DWORD		dwSize;
	DWORD		dwMajorType;
	DWORD		dwMinorType;

} PACKET_HEAD, *PPACKET_HEAD;


//------------------------------------------------------
// Major packet type
//
enum PACKET_TYPE
{
	ACTION_NONE,
	ACTION_USER,
	ACTION_CONTROL,
	ACTION_PUSH_DATA,
	ACTION_PULL_DATA,
	ACTION_SYNC_DATA,
	ACTION_TRANSFER_FILE,

	MOBILE_HEARTBEAT = 2048,		//�ֻ�������������
};


//------------------------------------------------------
// Minor packet type
// 

//
// User relative operation
//
enum
{
	USER_LOGON						= 1,
	USER_LOGOFF,
	USER_GUEST_LOGON,
	USER_GUEST_LOGOFF,
	USER_MODIFY_NAME,
	USER_GUEST_MODIFY_NAME,
	USER_MODIFY_PERMISSION,
	USER_GUEST_MODIFY_PERMISSION,
	USER_MODIFY_LEADER,
	NDUSER_REQUEST_LOGON_INFO,			//P2M		���ֻ������¼��Ϣ
	NDUSER_LOGON_INFO,								//M2P		�ֻ���PCͬ������ƽ̨�û���¼��Ϣ
};


//
// Control PPT/3D/Image/WhiteBoard
//
enum 
{
	CONTROL_M2P_PPT					= 1,
	CONTROL_M2P_LASER,			// �����
	CONTROL_M2P_BRUSH,
	CONTROL_M2P_3D,
	CONTROL_M2P_IMAGE,			// ͼƬ�촫
	CONTROL_M2P_WHITE_BOARD,	// �װ�/�ڰ�
	CONTROL_P2M_MOBILE,
	CONTROL_P2M_RESPONSE,		// PC�˸��ƶ��˵�Ӧ��
	CONTROL_M2P_MAGNIFIER,		// �Ŵ�
	CONTROL_M2P_SPOTLIGHT,		// �۹��
};

//
// Transfer file
//
enum 
{
	TRANSFER_FILE_START				= 1,	// ��ʼ�����ļ�
	TRANSFER_FILE_END,				// ���������ļ�
	TRANSFER_FILE_DATA,				// ������ļ�����
	TRANSFER_FILE_CANCEL,			// ȡ�������ļ�

	TRANSFER_FILE_CHECK_EXISTS,		// �����ļ�CRC���
	TRANSFER_FILE_CHECK_RESULT,		// �ļ�CRC�����
};

//
// Push informations 
//
enum 
{
	PUSH_P2M_PPTINFO				= 1,
	PUSH_P2M_OUTLINE,
	PUSH_P2M_PCINFO,
	PUSH_P2M_PLAY_BEGIN,
	PUSH_P2M_PLAY_SLIDE,
	PUSH_P2M_PLAY_END,
	PUSH_P2M_USER_STATUS,
	PUSH_P2M_VIDEO_LIST,
	PUSH_P2M_UPDATE_PPTIMAGE,			//����ppt����ͼ����
	PUSH_P2M_PREVIEW_INDEX,				//Ԥ��״̬�µ�ǰҳ
	PUSH_P2M_PPTCTL,					// PC��PPT״̬�仯��֪ͨ�ֻ���
	PUSH_P2M_MAGNIFIER,					// �Ŵ󾵣�֪ͨ�ֻ���
	PUSH_P2M_SPOTLIGHT,					// �۹�ƣ�֪ͨ�ֻ���
};

//����ppt����ͼ����
enum EPPTImageUpdate
{	
	UPDATE_START						= 1,			 //��ʼ��������
	UPDATE_PAGECNT,		 //�������£���ҳ��
	UPDATE_NEW,				//����ͼ���£���Ϣ�壺��IDNEW��4���ֽ�������ͼid����IDOLD:4���ֽھ�����ͼid��
	UPDATE_END,				//������������
};

//
// Pull informations 
//
enum
{
	PULL_M2P_APKFILE				= 1,
	PULL_M2P_PPTFILE,
};

//
// Sync informations		������Ϣ
//
enum
{
	SYNC_DATA_REQUEST_SLIDE_IMAGE		= 1,			//M2P	�ֻ���PC����PPT����ͼ����Ϣ�壺PAGECNT|PAGEIDX����
	SYNC_PPT_HAVE_CHANGE,										//P2M	PPT�ĵ������仯  
	SYNC_SAVE_CHANGE,												//M2P	����PPT�ĵ����ֻ����յ�SYNC_PPT_HAVE_CHANGE��Ϣ����Ӧ��
	SYNC_NOSAVE_CHANGE,											//M2P �������޸ģ�Ҳ���ǲ�ͬ��
};


//------------------------------------------------------
// More detail type
//

//
// PPT Control
// 
enum PPTControlType
{
	PPTCTL_NONE = 0,
	PPTCTL_PLAY,
	PPTCTL_STOP,
	PPTCTL_PREV,
	PPTCTL_NEXT,
	PPTCTL_GOTO,
	PPTCTL_CLEAR_INK,
	PPTCTL_BLACK_BACKGROUND,
	PPTCTL_CANCEL_BLACK_BACKGROUND,
	PPTCTL_WHITE_BACKGROUND,
	PPTCTL_CANCEL_WHITE_BACKGROUND,
	PPTCTL_HANDLCLAP,
	PPTCTL_CAIDAI,
	PPTCTL_CONFETTI,
	PPTCTL_CALC,
	PPTCTL_SHOW,
	PPTCTL_LASER_POINTER,
	PPTCTL_PEN_POINTER,
	PPTCTL_NORMAL_POINTER,
	PPTCTL_CANCEL_CALC,
	PPTCTL_PLAY_VIDEO,
	PPTCTL_PAUSE_VIDEO,
	PPTCTL_STOP_VIDEO,
	PPTCTL_FLAME,
	PPTCTL_OPEN,
	PPTCTL_BARRAGE,
	PPTCTL_YAOHAO,	// ҡ��
};

//
// Image Control
//
enum ImageControlType
{
	IMGCTL_NONE,
	IMGCTL_OPEN,
	IMGCTL_ZOOMIN,
	IMGCTL_ZOOMOUT,
	IMGCTL_NORMAL,
	IMGCTL_CLOSE,

};

//
// 3D Control
//
enum ThreeDControlType
{
	THREEDCTL_NONE,
	THREEDCTL_ZOOMIN,
	THREEDCTL_ZOOMOUT,
	THREEDCTL_MOVE,
	THREEDCTL_NORMAL,
	THREEDCTL_CLOSE,
};

//BRUSH
enum EBrushControlType
{
	NONE,
	BRUSH_COLOR,
	BRUSH_STYLE,
	BRUSH_WIDTH,
	BRUSH_MOVE,
	BRUSH_UNDO,
	BRUSH_CLEAR,
	BRUSH_BEGIN,
	BRUSH_END,
	BRUSH_SAVE,
	BRUSH_CANCEL_SAVE,
};

//�������� �Ŵ�
typedef enum {
	MAGNIFIER_NONE,
	MAGNIFIER_OPEN,			// �򿪷Ŵ�
	MAGNIFIER_MOVE,			// ��һ���㣨��1����1�����һ������2����2��
	MAGNIFIER_TOGGLE,		// �رշŴ�
	MAGNIFIER_P2M_RECT,		// PC�˸��ֻ��˵ķŴ����򣬺�����Ͻǡ����½�����ٷֱȣ�Left,Top,Right,Bottom
}PPTShellMagnifier;

//�������� �۹��
typedef enum {
	SPOTLIGHT_NONE,
	SPOTLIGHT_OPEN,			//�򿪾۹��
	SPOTLIGHT_SWITCH,		//�л��۹�Ƶ���̬��1:Բ�� 2:���Σ�
	SPOTLIGHT_CHANGE,		//�ı��С Բ���뾶��float��,���δ����(float)
	SPOTLIGHT_MOVE,			//�۹���ƶ� �����ĵ�
	SPOTLIGHT_OPEN_LIGHT,   //����
	SPOTLIGHT_CLOSE_LIGHT,  //�ص�
	SPOTLIGHT_TOGGLE		//�رվ۹��
}PPTShellSpotlight;
//�������� �ڰ�
typedef enum{
	BLACKBOARD_OPEN = 1,
	BLACKBOARD_CLOSE,
}BlackBoard;
// �������� PPT״̬
enum PPTStateSync
{
	PPTSTATE_P2M_CLOSE = 1,

};

enum ELaserControlType
{
	LASER_CTRL_NONE,
	LASER_MOVE,
	LASER_END,
};

#endif
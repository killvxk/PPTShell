//===========================================================================
// FileName:					Packet.h
//	
// Desc:					Packet defination
//===========================================================================
#ifndef _PACKET_H_
#define _PACKET_H_

#define MAX_PACKET_SIZE		(9*1024)

#define ACTION_DISCONNECT	0xfffa		//�Ͽ�����
#define PT_HEARTBEAT_RQST		0xfffb			//������
#define PT_HEARTBEAT_ACK			0xfffc			//������ Ӧ��
#define PT_CLOSE_MOBILE		0xfffd
#define PT_REGISTER			0xfffe
#define PT_TRANSFER			0xffff

enum
{
	CTYPE_NONE	= 0,
	CTYPE_MOBILE,
	CTYPE_PPTSHELL,
};


typedef struct PACKET_HEAD
{
	DWORD		dwSize;
	DWORD		dwMajorType;
	DWORD		dwMinorType;

} PACKET_HEAD, *PPACKET_HEAD;



typedef struct PACKET_REGISTER : PACKET_HEAD
{
	DWORD		dwClientType;				// 0: none 1: mobile / 2: PPTShell
	DWORD		dwMobileID;					// for mobile ID
	DWORD		dwPPTShellID;				// for PPTShell

} PACKET_REGISTER, *PPACKET_REGISTER;


#endif
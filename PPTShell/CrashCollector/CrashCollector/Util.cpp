#include "StdAfx.h"
#include "Util.h"

#include <winsock2.h> //��ͷ�ļ�������Socket��̵Ĺ���  
#include <stdio.h>    //��ͷ�ļ��������������������  
#include <stdlib.h>   //��ͷ�ļ�������һЩͨ�ú���  
#include <httpext.h>   //��ͷ�ļ�֧��HTTP����  
#include <windef.h>   //��ͷ�ļ�������Windows���������ݻ�����̬  
#include <Nb30.h>   //��ͷ�ļ�������netbios�����еĺ���   
#pragma comment(lib,"ws2_32.lib")    //����ws2_32.lib��.ֻҪ�������õ�Winsock API ��������Ҫ�õ� Ws2_32.lib  
#pragma comment(lib,"netapi32.lib")   //����Netapi32.lib�⣬MAC��ȡ���õ���NetApi32.DLL�Ĺ���  

//��Win32ƽ̨�ϵ�Winsock��̶�Ҫ�������еĻ������裺�������->���Winsock�汾->����Winsock��->  
//��ʼ��->�����׽���->�����׽���ѡ��->�ر��׽���->ж��Winsock�⣬�ͷ�������Դ��  
void CheckIP(void)    //����checkIP����������ȡ������ip��ַ  
{  
	WSADATA wsaData;  
	char name[155];      //�������ڴ�Ż���������ı���  
	char *ip;            //����IP��ַ����  
	PHOSTENT hostinfo;   

	//����MAKEWORD()���Winsocl�汾����ȷֵ����������ļ���Winscok��  
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 )   
	{   //����Winsock�⣬���WSAStartup()��������ֵΪ0��˵�����سɹ���������Լ�������ִ��  
		if( gethostname ( name, sizeof(name)) == 0)   
		{ //����ɹ����������������������name����ָ���Ļ�������  
			if((hostinfo = gethostbyname(name)) != NULL)   
			{ //���ǻ�ȡ�������������������ɹ��Ļ���������һ��ָ�룬ָ��hostinfo,hostinfoΪPHOSTENT�͵ı�����  
				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);   
				//inet_addr()�����ѵ�ַ��ת��ΪIP��ַ  
				//����inet_ntoa()����,��hostinfo�ṹ�����е�h_addr_listת��Ϊ��׼��IP��ַ(��202.197.11.12.)  
				printf(" IP��ַ: %s/n",ip);        //���IP��ַ  
			}   
		}   
		WSACleanup( );         //ж��Winsock�⣬���ͷ�������Դ  
	}   
}  

//��Win32ƽ̨�ϵ�Winsock��̶�Ҫ�������еĻ������裺�������->���Winsock�汾->����Winsock��->  
//��ʼ��->�����׽���->�����׽���ѡ��->�ر��׽���->ж��Winsock�⣬�ͷ�������Դ��  
void getIP(char * IpAddress)    //����checkIP����������ȡ������ip��ַ  
{  
	WSADATA wsaData;  
	char name[155];      //�������ڴ�Ż���������ı���  
	char *ip;            //����IP��ַ����  
	PHOSTENT hostinfo;   

	//����MAKEWORD()���Winsocl�汾����ȷֵ����������ļ���Winscok��  
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 )   
	{   //����Winsock�⣬���WSAStartup()��������ֵΪ0��˵�����سɹ���������Լ�������ִ��  
		if( gethostname ( name, sizeof(name)) == 0)   
		{ //����ɹ����������������������name����ָ���Ļ�������  
			if((hostinfo = gethostbyname(name)) != NULL)   
			{ //���ǻ�ȡ�������������������ɹ��Ļ���������һ��ָ�룬ָ��hostinfo,hostinfoΪPHOSTENT�͵ı�����  
				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);   
				//inet_addr()�����ѵ�ַ��ת��ΪIP��ַ  
				//����inet_ntoa()����,��hostinfo�ṹ�����е�h_addr_listת��Ϊ��׼��IP��ַ(��202.197.11.12.)  
				strcpy(IpAddress, ip);
			}   
		}   
		WSACleanup( );         //ж��Winsock�⣬���ͷ�������Դ  
	}   
}  


//ͨ��WindowsNT/Win2000�����õ�NetApi32.DLL�Ĺ�����ʵ�ֵġ�����ͨ������NCBENUM����,��ȡ������  
//��Ŀ��ÿ���������ڲ����,Ȼ���ÿ��������ŷ���NCBASTAT�����ȡ��MAC��ַ��  
int getMAC(char * mac)     //��NetAPI����ȡ����MAC��ַ  
{       
	NCB ncb;     //����һ��NCB(������ƿ�)���͵Ľṹ�����ncb  
	typedef struct _ASTAT_     //�Զ���һ���ṹ��_ASTAT_  
	{  
		ADAPTER_STATUS   adapt;   
		NAME_BUFFER   NameBuff   [30];       
	}ASTAT, *PASTAT;  
	ASTAT Adapter;     

	typedef struct _LANA_ENUM     //�Զ���һ���ṹ��_ASTAT_  
	{  
		UCHAR length;   
		UCHAR lana[MAX_LANA];     //�������MAC��ַ   
	}LANA_ENUM;       
	LANA_ENUM lana_enum;     

	//   ȡ��������Ϣ�б�       
	UCHAR uRetCode;       
	memset(&ncb, 0, sizeof(ncb));     //���ѿ����ڴ�ռ�ncb ��ֵ����Ϊֵ 0  
	memset(&lana_enum, 0, sizeof(lana_enum));     //���һ���ṹ���͵ı���lana_enum����ֵΪ0  
	//�Խṹ�����ncb��ֵ  
	ncb.ncb_command = NCBENUM;     //ͳ��ϵͳ������������  
	ncb.ncb_buffer = (unsigned char *)&lana_enum; //ncb_buffer��Աָ����LANA_ENUM�ṹ���Ļ�����  
	ncb.ncb_length = sizeof(LANA_ENUM);     
	//����������NCBENUM����Ի�ȡ��ǰ������������Ϣ�����ж��ٸ�������ÿ�������ı�ţ�MAC��ַ��   
	uRetCode = Netbios(&ncb); //����netbois(ncb)��ȡ�������к�      
	if(uRetCode != NRC_GOODRET)       
		return uRetCode;       

	//��ÿһ�������������������Ϊ�����ţ���ȡ��MAC��ַ     
	for(int lana=0; lana<lana_enum.length; lana++)       
	{  
		ncb.ncb_command = NCBRESET;   //����������NCBRESET������г�ʼ��  
		ncb.ncb_lana_num = lana_enum.lana[lana];   
		uRetCode = Netbios(&ncb);     
	}   
	if(uRetCode != NRC_GOODRET)  
		return uRetCode;       

	//   ׼��ȡ�ýӿڿ���״̬��ȡ��MAC��ַ  
	memset(&ncb, 0, sizeof(ncb));   
	ncb.ncb_command = NCBASTAT;    //����������NCBSTAT�����ȡ������Ϣ  
	ncb.ncb_lana_num = lana_enum.lana[0];     //ָ�������ţ��������ָ����һ��������ͨ��Ϊ��������   
	strcpy((char*)ncb.ncb_callname, "*");     //Զ��ϵͳ����ֵΪ*  
	ncb.ncb_buffer = (unsigned char *)&Adapter; //ָ�����ص���Ϣ��ŵı���  
	ncb.ncb_length = sizeof(Adapter);  
	//���ŷ���NCBASTAT�����Ի�ȡ��������Ϣ  
	uRetCode = Netbios(&ncb);   
	//   ȡ����������Ϣ����������������������Ļ������ر�׼��ð�ŷָ���ʽ��     
	if(uRetCode != NRC_GOODRET)     
		return uRetCode;     
	//������MAC��ַ��ʽת��Ϊ���õ�16������ʽ,������ַ���mac��   
	sprintf(mac,"%02X-%02X-%02X-%02X-%02X-%02X",       
		Adapter.adapt.adapter_address[0],       
		Adapter.adapt.adapter_address[1],       
		Adapter.adapt.adapter_address[2],       
		Adapter.adapt.adapter_address[3],       
		Adapter.adapt.adapter_address[4],       
		Adapter.adapt.adapter_address[5]   
	);   
	return 0;     
}  

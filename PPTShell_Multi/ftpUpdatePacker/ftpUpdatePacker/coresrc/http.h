/************************Http: Get��Post�ķ�װ��***************/
#pragma once
#include   "wininet.h"  
#include   "afxinet.h"

/*********************�ඨ��***********************************/
class   CHttpClient      
{  
public:  
	void   addParam(CString   name,CString   value);  
	CString   doPost(CString   href);  
	CString   doGet(CString   href);  
	CHttpClient();  
	virtual   ~CHttpClient();  
private:  
	CString   CONTENT;  
	int   CL;  
	CStringList   values;  
	CStringList   names;  
};  
#include "StdAfx.h"
#include "WebCrawler.h"
#include "RegExpContext.h"
#include "ThirdParty/ATLRegExp/WinHttpClient.h"		// http���ӵİ�װ��
#include "Util/Util.h"

CWebCrawler::CWebCrawler(void)
{
}

CWebCrawler::~CWebCrawler(void)
{
}

bool CWebCrawler::ProcessHttpRequest(CStringW strUrl, bool isUtf8File)
{
	//-------------------------------------------
	//�������ܣ���ȡURL��ַ��Ȼ��������񣬷���Htmlͷ������
	//����˵���� 
	//ע����� 
	//-------------------------------------------

	// ��ȡ��Ҫ�����URL��ַ
	CStringW url;
	url = strUrl;

	// ����Ƿ���Ҫ���httpЭ��ͷ
	if ( url.Find(L"http://") == -1 )
	{
		url.Insert(0, L"http://");
	}

	// ����Http�ͻ��ˣ���������
	WinHttpClient client( url.GetBuffer());
	// Send HTTP request, a GET request by default.
	if (isUtf8File)
	{
		client.SendHttpRequest(L"GET", false, true);
	}
	else
	{
		client.SendHttpRequest();
	}


	// �ռ����������ص�Httpͷ��Html����
	httpResponseHeader = client.GetResponseHeader();
	// The response content.
	httpResponseContent = client.GetResponseContent();

	//// ��httpͷ��Html������ʾ��Tab��ǩҳ���ı����С�
	//_tabHtmlHead._edtHtmlHead.SetWindowText( httpResponseHeader.c_str() );
	//_tabHtmlBody._edtHtmlBody.SetWindowText( httpResponseContent.c_str() );

	return true;
}

void CWebCrawler::FindTextInHtml(CStringW& strFindText, std::vector<wstring>& vecMatch)
{
	//-------------------------------------------
	//�������ܣ���Html����������������Ҫ���ı�
	//����˵���� 
	//ע����� 
	//-------------------------------------------

	// ��ȡ��Ҫ�������ı�
	CStringW searchText;
	searchText = strFindText;
	CStringW htmlContent;
	htmlContent = httpResponseContent.c_str();

	// �ж��û�ѡ������������ʽ
	CRegExpContext::REGEXP_RULE index = CRegExpContext::REGEXP_RULE::USE_BOOST_REGEXP;
	CRegExpContext RegExpContext(index);

	// ������ʽ����
	vecMatch.clear();
	std::wstring regExp = searchText.GetBuffer();
	std::wstring searchContent = htmlContent.GetBuffer();
	RegExpContext.RegExpMatch(regExp, searchContent, vecMatch);

	//// ��ʾƥ��������ʽ��Ŀ�ĸ���
	//size_t matchNum = vecMatch.size();
	//CString temp; temp.Format(L"%d", matchNum );
	//_edtMatchNumber.SetWindowText(temp);

	//// ��������ƥ������Ȼ����Richedit�����ƥ������
	//// ����������ɫ
	//for ( std::vector<wstring>::iterator it = vecMatch.begin();
	//	it != vecMatch.end();
	//	++it )
	//{
	//	CString test( (*it).c_str() );
	//	SearchInHtmlContent(test);
	//}

}

void CWebCrawler::FindTextInHtml(wstring& strHtmlContent, CStringW& strFindText, std::vector<wstring>& vecMatch)
{
	// ��ȡ��Ҫ�������ı�
	CStringW searchText;
	searchText = strFindText;

	// �ж��û�ѡ������������ʽ
	CRegExpContext::REGEXP_RULE index = CRegExpContext::REGEXP_RULE::USE_BOOST_REGEXP;
	CRegExpContext RegExpContext(index);

	// ������ʽ����
	vecMatch.clear();
	std::wstring regExp = searchText.GetBuffer();
	//std::wstring searchContent = strHtmlContent.GetBuffer();
	std::wstring searchContent = strHtmlContent;
	RegExpContext.RegExpMatch(regExp, searchContent, vecMatch);
}

//void CWebCrawler::FindName(CStringW strFindText)
//{
//	FindTextInHtml(strFindText, vecMatchName);
//}
//void CWebCrawler::FindPreviewsAddress(CStringW strFindText)
//{
//	FindTextInHtml(strFindText, vecMatchPreviewsAddress);
//}
//void CWebCrawler::FindObjAddress(CStringW strFindText)
//{
//	FindTextInHtml(strFindText, vecMatchObjAddress);
//}


wstring CWebCrawler::Getmatch(std::vector<wstring>& vecMatch)
{
	// TODO: �ڴ���������������
	//CString temp;
	//for ( vector<wstring>::iterator it = vecMatch.begin();
	//	it != vecMatch.end();
	//	++it)
	//{
	//	temp += (*it).c_str();
	//	temp += L"\r\n";
	//}
	//_tabHtmlGet._edtHtmlGet.SetWindowText(temp);

	wstring temp;
	for ( vector<wstring>::iterator it = vecMatch.begin();
		it != vecMatch.end();
		++it)
	{
		temp += (*it);
		temp += L"\r\n";
	}

	return temp;
}

void CWebCrawler::ResponseContent2Uni()
{
	
	//string httpResponseContentA = Un2Str(httpResponseContent);
	string httpResponseContentA = UnicodeToAnsi(httpResponseContent);

	int wcsLen = MultiByteToWideChar(CP_UTF8, NULL, httpResponseContentA.c_str(), httpResponseContentA.length(), NULL, 0);
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	MultiByteToWideChar(CP_UTF8, NULL, httpResponseContentA.c_str(), httpResponseContentA.length(), wszString, wcsLen);
	wszString[wcsLen] = '\0';

	httpResponseContent = wszString;
	delete wszString;


}
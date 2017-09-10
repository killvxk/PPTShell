#pragma once

class CWebCrawler
{
public:
	CWebCrawler(void);
	~CWebCrawler(void);

	//����
	bool ProcessHttpRequest(CStringW strUrl, bool isUtf8File = false);
	//����
	void FindTextInHtml(CStringW& strFindText, std::vector<wstring>& vecMatch);
	void FindTextInHtml(wstring& strHtmlContent, CStringW& strFindText, std::vector<wstring>& vecMatch);
	//void FindName(CStringW strFindText);
	//void FindPreviewsAddress(CStringW strFindText);
	//void FindObjAddress(CStringW strFindText);

	//��ȡ
	wstring Getmatch(std::vector<wstring>& vecMatch);

	void ResponseContent2Uni();

public:
	// �ռ����������ص�Httpͷ��Html����
	wstring httpResponseHeader;
	// The response content.
	wstring httpResponseContent;

	//std::vector<wstring> vecMatchName;
	//std::vector<wstring> vecMatchPreviewsAddress;
	//std::vector<wstring> vecMatchObjAddress;
};

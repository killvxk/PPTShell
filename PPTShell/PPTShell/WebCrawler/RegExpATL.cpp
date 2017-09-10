//-------------------------------------------
//�������ܣ�����ATL��������ʽ
//����˵���� 
//ע����� 
//-------------------------------------------

#include "StdAfx.h"
#include "RegExpATL.h"
//#include <atlrx.h>
#include "ThirdParty/ATLRegExp/atlrx.h"
#include <vector>
using namespace std;

CRegExpATL::CRegExpATL(void)
{
}

CRegExpATL::~CRegExpATL(void)
{
}


bool CRegExpATL::RegExpMatch(const wstring& regExp, const wstring& searchText, std::vector<wstring>& results)
{
//-------------------------------------------
//�������ܣ�������ʽ����
//����˵���� 
//ע����� 
//-------------------------------------------

	//CString strRegex = regExp;
	//CString strInput = searchText;
	//AfxMessageBox(regExp.c_str());
	//AfxMessageBox(searchText.c_str());

	// ��֤���ʽ�Ƿ���ȷ
	CAtlRegExp<CAtlRECharTraitsW> reRule;
	REParseError status = reRule.Parse( regExp.c_str(), TRUE);
	if (REPARSE_ERROR_OK != status)
	{
		return false;
	}

	// ƥ������
	wchar_t *pwSourceText = new wchar_t[searchText.size()+1];
	wchar_t *pSourceEnd = pwSourceText + searchText.size();
	if ( pwSourceText == NULL )
	{
		return false;
	}
	wcscpy_s(pwSourceText, searchText.size()+1, searchText.c_str());
	CAtlREMatchContext<CAtlRECharTraitsW> mc;
	const wchar_t *pFrom = pwSourceText;
	const wchar_t *pTo = NULL;
	BOOL bSucceed = TRUE;
	while (bSucceed)
	{
		if (!reRule.Match( pFrom, &mc, &pTo ) )
		{
			// AfxMessageBox(L"û���κ�ƥ�䣡");
			// û���κ�ƥ��
			break;
		}
		else
		{
			//CString temp;
			//temp.Format(L"���� %d ��ƥ�䣡", mc.m_uNumGroups);
			//AfxMessageBox(temp);

			for (UINT nGroupIndex = 0; nGroupIndex < mc.m_uNumGroups; ++nGroupIndex)
			{
				const CAtlREMatchContext<>::RECHAR* szStart = 0;
				const CAtlREMatchContext<>::RECHAR* szEnd = 0;
				mc.GetMatch(nGroupIndex, &szStart, &szEnd);
				if (szStart != NULL && szEnd != NULL)
				{
					ptrdiff_t nLength = szEnd - szStart;
					wstring matchString(szStart, static_cast<int>(nLength));
					//AfxMessageBox(matchString.c_str());
					results.push_back(matchString);
				}
				else
				{
					break;
				}

			}
			if (pTo < pSourceEnd)
			{
				pFrom = pTo;
			}
			else
			{
				break;
			}
		}
	}

	delete[] pwSourceText;
	return true;
}
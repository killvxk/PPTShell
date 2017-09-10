#ifndef ND_TOOL_PATCH_7Z_H
#define ND_TOOL_PATCH_7Z_H

#include "afx.h"

enum SZIPRES
{
	SZIP_OK,
	SZIP_FAIL,
	SZIP_PARAM_ERROR,
	SZIP_DLL_INTERFACE_ERROR,
	SZIP_READ_SRCFILE_FAIL,
	SZIP_WRITE_DESFILE_FAIL
};


// ���Ȼص�֪ͨ
class ISevenZipCallback
{
public:
	virtual ~ISevenZipCallback(){};
	virtual void CompressTotal(UINT64 ullSize)=0;
	virtual void CompressComplete(const UINT64* pullCompleteValue)=0;
};

/**
 * ����������ѹ���ļ���Ŀ¼
 * @param [IN]pcszDestFile Ŀ���ļ��� �磺*.7z
 * @param [IN]pcszBasePath ��·��
 * @param [IN]pcszSrcFileOrDirectory Դ�ļ�(�У�����ָ�룬����pcszBasePath�����·��
 * @param [IN]iCount Դ�ļ����У��������
 * @param [IN]cb ���Ȼص�֪ͨ
 * @return SZIPRES
 */

SZIPRES SevenZipCompressItems(const char* pcszDestFile,
							  const char* pcszBasePath,
							  char* pcszSrcFileOrDirectory[],
							  int iCount,
							  ISevenZipCallback* cb
							  );
/**
 * ������������ѹ�ļ�
 * @param [IN]pcszDestDirectory Ŀ���ļ��� ��ѹ���ļ������ڴ�Ŀ¼�£����Ϊ�գ����ѹ����ǰĿ¼��
 * @param [IN]pcszCompressedFile ����ѹ���ļ� �磺*.7z
 * @param [IN]cb ���Ȼص�
 * @return SZIPRES
 */
SZIPRES SevenZipDecompress(const char* pcszDestDirectory,
						   const char* pcszCompressedFile,
						   ISevenZipCallback* cb
						   );
/**
 * ������������ȡ����ѹ�ļ���ѹ��Ĵ�С
 * @param [IN] pcszCompressedFile ����ѹ�ļ��� ��*.7z
 * @param [OUT]pullSize ��С
 */
SZIPRES SevenZipGetDecompressSize(const char* pcszCompressedFile,
								  UINT64* pullSize
								  );

#endif
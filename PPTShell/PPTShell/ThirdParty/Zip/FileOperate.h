/*
 */

#ifndef _FILEOPERATE_H_
#define _FILEOPERATE_H_

#include <windows.h>
#include <string>

typedef enum _E_FILE_TYPE_
{
	E_FILE_TYPE_NOT_EXIST		= 0, //�ļ�������
	E_FILE_TYPE_NORMAL_FILE		= 1, //�ļ�Ϊ��ͨ�ļ�
	E_FILE_TYPE_DIRECTORY		= 2  //�ļ�ΪĿ¼
}E_FILE_TYPE;

class CFileOperate
{
public:
	//��ȡģ���ļ�·��
	static bool GetModuleFilePath(char* modulefilepath, int length, HMODULE hModule = NULL);
	//��ȡ�ļ�����
	static bool GetModuleFileName(char* modulefilename, int length, HMODULE hModule = NULL);
	//�����ļ�·��
	static bool ParseFilePath(const char* fileallpath, char* filepath, int length);
	//�����ļ���
	static bool ParseFileName(const char* filepath, char* filename, int length );
	//������׺��
	static bool ParsePostfix(const char* filepath, char* postfix, int length);
	//�����ļ�����������׺
	static bool ParseFileNameNoPostfix(const char* filepath, char* filenamenopostfix, int length);
	//����ļ��Ƿ����
	static bool CheckFileIsExist(const char* filePath);
	//����ļ�����
	static E_FILE_TYPE CheckFileAttribute(const char *pFilePath);
	//����Ŀ¼
	static bool CreateMultiFolder(const char *pDirPath);
	//ɾ��Ŀ¼
	static bool DeleteFolder(const char *pFolder);
	//���Ŀ¼
	static bool CleanFolder(const char *pFolder);

	static unsigned long GetFileSize(const char *pFile, unsigned long *High = NULL);

	static bool GetAboslutePath(const char* filepath, char *aboslutepath, int lenght);

	static std::string CheckFilePathReturnString(const char *filepath );
	static void CheckFilePathVoid( char *filepath );

	//����Ŀ¼�µ��ļ�
	static bool GetFirstFileName( const char *pFolder, HANDLE &handle, WIN32_FIND_DATAA& FindFileData );
	static bool GetNextFileName( HANDLE &findHandle, WIN32_FIND_DATA& FindFileData );
	static void CloseFindHandle(HANDLE &findHandle);
};

#endif // _FILEPATH_H_

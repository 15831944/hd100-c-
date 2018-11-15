/***********************************************************
* ��Ȩ���� (C)2013-2014,TenSun Industrial Equipment Co., Ltd.
* 
* �ļ����ƣ�ConfigOp.h
* ��ǰ�汾��V1.0
* ��   �ߣ� eddie_cheung     
* ������ڣ�2013��11��11��
* �ļ������� �����ļ�������
* ����˵�������ini���������ļ����ݵĶ�д������֧��ASCII/Unicode 
*           ģʽ��ini�����ļ�,��Ҫ���ļ�����ƥ�䡣
*            
* �޸ļ�¼��
*    <����>:              <�޸���>:          <�޸�����>:
*   2013.11.11            eddie_cheung       Creat
*   2013.12.4             eddie              �޸Ĺ�������Ϊ���ֽ��������л���ƥ��
*******************************************************/
#pragma once

// �����ļ����� 
enum
{
	MODE_UNICODE,  // unicode
	MODE_ANSI,     // Ansi
};

// ����ΪDLLģ�鵼����
class  CConfigOp
{
public:
	CConfigOp();
	CConfigOp(CString strFileName,int nMode = MODE_ANSI);
	~CConfigOp(void);

public:
	//INI���ݶ�д����
	BOOL  WriteStringToIni(const CString str,const CString key,const CString val);
	BOOL  WriteIntegerToIni(const CString str,const CString key,const int val);
	BOOL  WriteFloatToIni(const CString str,const CString key,const double val);
	BOOL  WriteStructToIni(LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct);

	double	ReadFloatFromIni(const CString str,const CString key);
	int	    ReadIntegerFromIni(const CString str,const CString key);
	CString ReadStringFromIni(const CString str,const CString key);
    BOOL    ReadStructFromIni(LPCSTR lpszSection, LPCSTR lpszKey, LPVOID& lpStruct);

     // �������ݵ��ļ�
	BOOL    SaveDateToDisk();    
public:
	BOOL    GetFile(CString strIniPath); // ��ȡ�ļ�����
	CString GetProfileString(const CString strSection, const CString strEntry, const CString strDefault);
	BOOL    WriteProfileString(const CString strSection, const CString strEntry, const CString strValue);

public:	
	int m_nMode;            // �ĵ�ģʽ    0 unicode  1 Ansic
	CString m_strFilePath;  // ȫ�ļ�·��,���ļ���
};

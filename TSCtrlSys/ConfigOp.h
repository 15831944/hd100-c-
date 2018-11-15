/***********************************************************
* 版权所有 (C)2013-2014,TenSun Industrial Equipment Co., Ltd.
* 
* 文件名称：ConfigOp.h
* 当前版本：V1.0
* 作   者： eddie_cheung     
* 完成日期：2013年11月11日
* 文件描述： 配置文件操作类
* 其它说明：完成ini类型配置文件内容的读写操作，支持ASCII/Unicode 
*           模式的ini配置文件,需要与文件类型匹配。
*            
* 修改记录：
*    <日期>:              <修改人>:          <修改内容>:
*   2013.11.11            eddie_cheung       Creat
*   2013.12.4             eddie              修改工程类型为多字节以与现有环境匹配
*******************************************************/
#pragma once

// 配置文件类型 
enum
{
	MODE_UNICODE,  // unicode
	MODE_ANSI,     // Ansi
};

// 此类为DLL模块导出类
class  CConfigOp
{
public:
	CConfigOp();
	CConfigOp(CString strFileName,int nMode = MODE_ANSI);
	~CConfigOp(void);

public:
	//INI数据读写操作
	BOOL  WriteStringToIni(const CString str,const CString key,const CString val);
	BOOL  WriteIntegerToIni(const CString str,const CString key,const int val);
	BOOL  WriteFloatToIni(const CString str,const CString key,const double val);
	BOOL  WriteStructToIni(LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct);

	double	ReadFloatFromIni(const CString str,const CString key);
	int	    ReadIntegerFromIni(const CString str,const CString key);
	CString ReadStringFromIni(const CString str,const CString key);
    BOOL    ReadStructFromIni(LPCSTR lpszSection, LPCSTR lpszKey, LPVOID& lpStruct);

     // 保存数据到文件
	BOOL    SaveDateToDisk();    
public:
	BOOL    GetFile(CString strIniPath); // 获取文件数据
	CString GetProfileString(const CString strSection, const CString strEntry, const CString strDefault);
	BOOL    WriteProfileString(const CString strSection, const CString strEntry, const CString strValue);

public:	
	int m_nMode;            // 文档模式    0 unicode  1 Ansic
	CString m_strFilePath;  // 全文件路径,含文件名
};

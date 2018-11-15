// mvFunction.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Function.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma warning(disable:94)

//////////////////////////////////////////////////////////////////////////

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

//////////////////////////////////////////////////////////////////////////
CFunction::CFunction()
{

}

CFunction::~CFunction()
{

}

CString CFunction::GetVerion()
{
	return "�汾��:[1.0.0.1]";
}

void CFunction::DoEvents()
{
	MSG msg;
	// Process existing messages in the application's message queue.
	// When the queue is empty, do clean up and return.
	/*while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	{
		if (!AfxGetThread()->PumpMessage())
			return;
		break;
	}*/
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

// ��ʱ ��λ:��
void CFunction::DelaySec(double dTime)
{
	DWORD time = (DWORD)(dTime*1000);
	DWORD t=::GetTickCount();
	while(::GetTickCount() -t < time)
	{
		DoEvents();
	}
}

// �߾�����ʱ ��λ����
void CFunction::DelayExSec(double dTime)
{
	double dt=GetCurrentTimeEx();
	while(GetCurrentTimeEx() -dt < dTime)
	{
		DoEvents();
	}
}
double CFunction::GetCurrentTimeEx()
{
	return double(GetCurrentCounter())/ GetCurrentFrequency();
}

CString CFunction::GetDirectory()
{
	char szPath[255];
	memset(szPath, 0, 255);
	GetDirectory(szPath);
	return CString(szPath);
}

void CFunction::GetDirectory(LPCSTR pExcutePath)
{
	TCHAR sFilename[_MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];

	GetModuleFileName(AfxGetInstanceHandle(), sFilename, _MAX_PATH);
	_splitpath_s(sFilename, sDrive, sDir, sFname, sExt);

	CString csVal(CString(sDrive) + CString(sDir));

	if (csVal.Right(1) != _T('\\'))
		csVal += _T("\\"); 
	
	int no = csVal.Find(_T("\\Bin"));
	CString str = csVal.Left(no+1);

	strcpy((LPSTR)pExcutePath,str.GetBuffer());
	str.ReleaseBuffer();
}


LONGLONG CFunction::GetCurrentCounter()
{
	LARGE_INTEGER lStartCount;
	QueryPerformanceCounter(&lStartCount);
	return lStartCount.QuadPart;
}

LONGLONG CFunction::GetCurrentFrequency()
{
	LARGE_INTEGER lFrequence;
	QueryPerformanceFrequency(&lFrequence);

	return lFrequence.QuadPart;
}

void CFunction::AppendEdit(CEdit *pEdit, LPCSTR pText)
{
	long nCount = pEdit->SendMessage(WM_GETTEXTLENGTH);
	if(nCount>10000)
	{
		pEdit->SetWindowText(_T(""));
	}
	pEdit->SetSel(nCount, nCount);
	pEdit->ReplaceSel(pText);
	pEdit->LineScroll(nCount);
}

//////////////////////////////////////////////////////////////////////////
void CFunction::HandleString(BOOL bReadParam, LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpValueDefault, LPCSTR lpFileName)
{
	if(bReadParam)
	{
		CString strValue('\0',256);
		::GetPrivateProfileString(lpAppName, lpKeyName, lpValueDefault, (LPSTR)(LPCSTR)strValue, 256, lpFileName);
		strcpy((LPSTR)lpValueDefault,strValue.GetBuffer(strValue.GetLength()));
		strValue.ReleaseBuffer();
	}
	else
	{
		::WritePrivateProfileString(lpAppName, lpKeyName, lpValueDefault, lpFileName);
	}
}

void CFunction::HandleInt(BOOL bReadParam, LPCSTR lpAppName, LPCSTR lpKeyName,   INT &nValueDefault, LPCSTR lpFileName)
{
	if(bReadParam)
	{
		nValueDefault = ::GetPrivateProfileInt(lpAppName, lpKeyName, nValueDefault, lpFileName);
	}
	else
	{
		CString strValue('\0', 256);
		strValue.Format(_T("%d"), nValueDefault);
		::WritePrivateProfileString(lpAppName, lpKeyName, strValue, lpFileName);
	}
}

void CFunction::HandleLong(BOOL bReadParam, LPCSTR lpAppName, LPCSTR lpKeyName,	long &nValueDefault, LPCSTR lpFileName)
{
	if(bReadParam)
	{
		nValueDefault = ::GetPrivateProfileInt(lpAppName, lpKeyName, nValueDefault, lpFileName);
	}
	else
	{
		CString strValue('\0', 256);
		strValue.Format(_T("%d"), nValueDefault);
		::WritePrivateProfileString(lpAppName, lpKeyName, strValue, lpFileName);
	}
}

void CFunction::HandleDWord(BOOL bReadParam, LPCSTR lpAppName, LPCSTR lpKeyName, DWORD &nValueDefault, LPCSTR lpFileName)
{
	if(bReadParam)
	{
		nValueDefault = ::GetPrivateProfileInt(lpAppName, lpKeyName, nValueDefault, lpFileName);
	}
	else
	{
		CString strValue('\0', 256);
		strValue.Format(_T("%d"), nValueDefault);
		::WritePrivateProfileString(lpAppName, lpKeyName, strValue, lpFileName);
	}
}
void CFunction::HandleByte(BOOL bReadParam, LPCSTR lpAppName, LPCSTR lpKeyName,	BYTE &nValueDefault, LPCSTR lpFileName)
{
	if(bReadParam)
	{
		nValueDefault = (BYTE)(::GetPrivateProfileInt(lpAppName, lpKeyName, (INT)nValueDefault, lpFileName));
	}
	else
	{
		CString strValue('\0', 256);
		strValue.Format(_T("%d"), nValueDefault);
		::WritePrivateProfileString(lpAppName, lpKeyName, strValue, lpFileName);
	}
}

void CFunction::HandleStruct(BOOL bReadParam, LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct,  UINT uSizeStruct, LPCSTR szFileName)
{
	if(bReadParam)
		::GetPrivateProfileStruct(lpszSection, lpszKey, lpStruct, uSizeStruct, szFileName);
	else
		::WritePrivateProfileStruct(lpszSection, lpszKey, lpStruct, uSizeStruct, szFileName);
}

void CFunction::HandleDouble(BOOL bReadParam, LPCSTR lpAppName, LPCSTR lpKeyName,  double &lpValueDefault, LPCSTR lpFileName)
{
	CString strValue('\0', 256);
	strValue.Format(_T("%.9f"), lpValueDefault);

	HandleString(bReadParam, lpAppName, lpKeyName, strValue, lpFileName);
	if(bReadParam)
		lpValueDefault = atof(strValue);
}
void CFunction::HandleFloat(BOOL bReadParam, LPCSTR lpAppName, LPCSTR lpKeyName, float &lpValueDefault, LPCSTR lpFileName)
{
	CString strValue('\0', 256);
	strValue.Format(_T("%.9f"), lpValueDefault);

	HandleString(bReadParam, lpAppName, lpKeyName, strValue, lpFileName);
	if(bReadParam)
		lpValueDefault = float(atof(strValue));
}

bool CFunction::IsNumber(CString strText)
{
	CString strNum(_T("0.123456789"));
	int nTotal = strText.GetLength();

	for (int i=0; i<nTotal; i++)
	{
		if ( strNum.Find(strText.GetAt(i)) == -1)
		{
			return false;
		}
	}
	return true;
}

//��¼��־
BOOL CFunction::WriteLog(const CString csFileFullName, const int iLineNo, const CString csContent, const int iLevel)
{
	// ��ȡ�����cpp�ļ���
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];
	_splitpath_s(csFileFullName, sDrive, sDir, sFname, sExt);
	CString strWarningFile = "";
	strWarningFile.Format("%s%s", sFname, sExt);

	// ����д���ļ���Log��Ϣ
	CString csData = "";

	//��ȡϵͳ��ǰʱ��
	CTime tTime;
	tTime = CTime::GetCurrentTime();
	CString csCurTime = tTime.Format("%Y-%m-%d %H:%M:%S"); //yyyy-MM-dd HH:mm:ss
	//////////////////////////////////////////////////////////////////////////

	CString csLevel = "";
	//��־����
	if (iLevel == LEV_INFO)	{
		csLevel = "INFO";
	}
	else if (iLevel == LEV_WARN)	{
		csLevel = "WARN";
	}
	else if (iLevel == LEV_ERROR)	{
		csLevel = "ERROR";
	}
	else if (iLevel == LEV_OPERAE)	{
		csLevel = "OPERATE";
	}
	else {//if (iLevel == LEV_FATAL)	
		csLevel = "FATAL";
	}

	// ����·��
	_mkdir(GetDirectory()+ "Log");

	// Log��־�����ļ�
	CString csFileName = GetDirectory()+ "Log\\"+csLevel+".log";

	CString csLineNo = "";
	csLineNo.Format("%d��", iLineNo);
	csData = "[" + csCurTime + "]" +  " " + strWarningFile + " " + csLineNo + " " + csLevel + " :"  + " " + csContent + "\r\n";
	//////////////////////////////////////////////////////////////////////////

	FILE *pf = fopen(csFileName, "a");
	if (NULL == pf)	{
		return FALSE;
	}

	fwrite(csData, 1, strlen(csData), pf);
	fclose(pf);

	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}


bool CFunction::DeleteDirectory(char* DirName)
{
	HANDLE hFirstFile = NULL; 
	WIN32_FIND_DATA FindData; 
	char currdir[MAX_PATH] = {0};
	sprintf_s(currdir, "%s\\*.*", DirName);
	hFirstFile = ::FindFirstFile(currdir, &FindData);
	if( hFirstFile == INVALID_HANDLE_VALUE )	return false;
	BOOL bRes = true;
	while(bRes) 
	{ 
		bRes = ::FindNextFile(hFirstFile, &FindData);
		if( (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) //����Ŀ¼
		{
			if( !strcmp(FindData.cFileName, ".") || !strcmp(FindData.cFileName, "..") ) //.��..
				continue;
			else
			{
				char tmppath[MAX_PATH] = {0};
				sprintf_s(tmppath, "%s\\%s", DirName, FindData.cFileName);
				DeleteDirectory(tmppath);
			}
		}
		else               //�����ļ�
		{
			char tmppath[MAX_PATH] = {0};
			sprintf_s(tmppath, "%s\\%s", DirName, FindData.cFileName);
			::DeleteFile(tmppath);   
		}
	} 

	::FindClose(hFirstFile);
	if(!RemoveDirectory(DirName))
	{
		return false ;
	}

	return true;
}


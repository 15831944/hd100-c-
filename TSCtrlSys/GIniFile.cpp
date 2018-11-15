// GIniFile.cpp: implementation of the CGIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "tchar.h"
#include "stdio.h"
#include "stdafx.h"
#include "GIniFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define RADIX10 10
#define DIGITS20 15
#define MAX_STRING_LENGTH 10000
#define CHAR_COMMA ','
#define CHAR_SEMICOLON ';'
#define CHAR_BLANK ' '

CGIniFile::CGIniFile()
{
	m_lpszPath = NULL;
}

CGIniFile::CGIniFile(LPCTSTR lpszPath)
{
	m_lpszPath = (LPTSTR)malloc(strlen(lpszPath) + 1);
	memcpy(m_lpszPath, lpszPath, strlen(lpszPath) + 1);
}

CGIniFile::~CGIniFile()
{
	if (m_lpszPath)
	{
		free(m_lpszPath);
		m_lpszPath = NULL;
	}
}

void CGIniFile::SetPath(LPCTSTR lpszPath)
{
	if (m_lpszPath)
	{
		free(m_lpszPath);
		m_lpszPath = NULL;
	}

	m_lpszPath = (LPTSTR)malloc(strlen(lpszPath) + 1);
	memcpy(m_lpszPath, lpszPath, strlen(lpszPath) + 1);
}

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	return ::WritePrivateProfileString(lpszSection, lpszKey, lpszValue, m_lpszPath);
}

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, long lValue)
{
	char szValue[MAX_PATH];
//	ltoa(lValue, szValue, RADIX10);
	sprintf(szValue, "%ld", lValue);
	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);	
}

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD dwValue)
{
	char szValue[MAX_PATH];	
//	ultoa(dwValue, szValue, RADIX10);
	sprintf(szValue, "%lu", dwValue);
	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);
}

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, WORD wValue)
{
	char szValue[MAX_PATH];
//	ultoa(wValue, szValue, RADIX10);
	sprintf(szValue, "%u", wValue);
	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);
}

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, int nValue)
{
	char szValue[MAX_PATH];	
//	itoa(nValue, szValue, RADIX10);
	sprintf(szValue, "%d", nValue);
	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);
}

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, double dValue, WORD wPrecision)
{
	char szValue[MAX_PATH];	
//	gcvt(dValue, (int)wPrecision, szValue);
	char szTemp[MAX_PATH], szTemp1[MAX_PATH];

	strcpy(szTemp, "%.");
	sprintf(szTemp1, "%u", wPrecision);
	strcat(szTemp, szTemp1);
	strcat(szTemp, "lf");
	sprintf(szValue, szTemp, dValue);
	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);		
}

// BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, BOOL bValue)
// {
// 	char szValue[MAX_PATH];	
// 	itoa(bValue, szValue, RADIX10);
// 	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);
// }

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPTSTR lpszValue)
{
	return ::GetPrivateProfileString(lpszSection, lpszKey, "", lpszValue, MAX_PATH, m_lpszPath);
}

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, long& lValue)
{
	char szValue[MAX_PATH];

	if (!::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_PATH, m_lpszPath))
		return FALSE;

	lValue = atol(szValue);
	return TRUE;
}

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD& dwValue)
{
	char szValue[MAX_PATH];

	if (!::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_PATH, m_lpszPath))
		return FALSE;	

	dwValue = atol(szValue);
	return TRUE;
}

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, WORD& wValue)
{
	char szValue[MAX_PATH];

	if (!::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_PATH, m_lpszPath))
		return FALSE;

	wValue = (WORD)atol(szValue);
	return TRUE;
}

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, int& nValue)
{
	char szValue[MAX_PATH];

	if (!::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_PATH, m_lpszPath))
		return FALSE;
	
	nValue = atoi(szValue);	
	return TRUE;
}

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, double& dValue)
{
	char szValue[MAX_PATH];

	if (!::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_PATH, m_lpszPath))
		return FALSE;
	
	dValue = atof(szValue);	
	return TRUE;
}

// BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, BOOL& bValue)
// {
// 	char szValue[MAX_PATH];
// 	::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_PATH, m_lpszPath);	
// 	bValue = atoi(szValue);			
// }

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, WORD* pwValue, DWORD dwLength)
{
	char szValue[MAX_STRING_LENGTH];
	long i = 0;
	long j = 0;
	long k = 0;

	if (dwLength < 1)
		return FALSE;

	for (; i < (long)dwLength - 1; i++)
	{
		k = sprintf(szValue + j, "%u,", *(pwValue + i));
		j += k;

		if (j > MAX_STRING_LENGTH)
			return FALSE;
	}

	k = sprintf(szValue + j, "%u", *(pwValue + i));
	j += k;

	if (j + 1 > MAX_STRING_LENGTH)
		return FALSE;

	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);
}

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, int* pnValue, DWORD dwLength)
{
	char szValue[MAX_STRING_LENGTH];
	long i = 0;
	long j = 0;
	long k = 0;

	if (dwLength < 1)
		return FALSE;

	for (; i < (long)dwLength - 1; i++)
	{
		k = sprintf(szValue + j, "%d,", *(pnValue + i));
		j += k;
		
		if (j > MAX_STRING_LENGTH)
			return FALSE;
	}
	
	k = sprintf(szValue + j, "%d", *(pnValue + i));
	j += k;
	
	if (j + 1 > MAX_STRING_LENGTH)
		return FALSE;
	
	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);	
}

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, long* plValue, DWORD dwLength)
{
	char szValue[MAX_STRING_LENGTH];
	long i = 0;
	long j = 0;
	long k = 0;

	if (dwLength < 1)
		return FALSE;

	for (; i < (long)dwLength - 1; i++)
	{
		k = sprintf(szValue + j, "%ld,", *(plValue + i));
		j += k;
		
		if (j > MAX_STRING_LENGTH)
			return FALSE;
	}
	
	k = sprintf(szValue + j, "%ld", *(plValue + i));
	j += k;
	
	if (j + 1 > MAX_STRING_LENGTH)
		return FALSE;
	
	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);	
}

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD* pdwValue, DWORD dwLength)
{
	char szValue[MAX_STRING_LENGTH];
	long i = 0;
	long j = 0;
	long k = 0;

	if (dwLength < 1)
		return FALSE;
	
	for (; i < (long)dwLength - 1; i++)
	{
		k = sprintf(szValue + j, "%lu,", *(pdwValue + i));
		j += k;
		
		if (j > MAX_STRING_LENGTH)
			return FALSE;
	}
	
	k = sprintf(szValue + j, "%lu", *(pdwValue + i));
	j += k;
	
	if (j + 1 > MAX_STRING_LENGTH)
		return FALSE;
	
	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);	
}

BOOL CGIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, double* pdValue, DWORD dwLength, WORD wPrecision)
{
	char szValue[MAX_STRING_LENGTH];
	long i = 0;
	long j = 0;
	long k = 0;

	if (dwLength < 1)
		return FALSE;

	char szTemp[MAX_PATH], szTemp1[MAX_PATH];

	strcpy(szTemp, "%.");
	sprintf(szTemp1, "%u", wPrecision);
	strcat(szTemp, szTemp1);
	strcat(szTemp, "lf,");
	
	for (; i < (long)dwLength - 1; i++)
	{

		k = sprintf(szValue + j, /*"%.%dlf,"*/szTemp, *(pdValue + i));
		j += k;
		
		if (j > MAX_STRING_LENGTH)
			return FALSE;
	}

	*(szTemp + strlen(szTemp) - 1) = '\0';	
	k = sprintf(szValue + j, /*"%.%dlf"*/szTemp, *(pdValue + i));
	j += k;
	
	if (j + 1 > MAX_STRING_LENGTH)
		return FALSE;
	
	return ::WritePrivateProfileString(lpszSection, lpszKey, szValue, m_lpszPath);	
}

long StringReplaceWithBlank(LPTSTR lpszString)
{
	long lCount = 0;

	if (0 ==strlen(lpszString))
		return 0;
	
	LPTSTR lpsz = lpszString;
	LPTSTR pszEnd = lpsz + strlen(lpszString);
	
	while (lpsz < pszEnd)
	{
		if (_istspace(*lpsz) || CHAR_COMMA == *lpsz || CHAR_SEMICOLON == *lpsz)
		{
			*lpsz = CHAR_BLANK;
			lCount++;
		}
		
		lpsz = _tcsinc(lpsz);
	}
	
	return lCount;
}

void StringTrim(LPTSTR lpszString)
{
	LPTSTR lpsz = lpszString;
	LPTSTR lpszLast = NULL;

	if (0 ==strlen(lpszString))
		return;

	while (_istspace(*lpsz))
		lpsz = _tcsinc(lpsz);

	if (lpsz != lpszString)
		memmove(lpszString, lpsz, strlen(lpsz) + 1);

	lpsz = lpszString;

	while (*lpsz != '\0')
	{
		if (_istspace(*lpsz))
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;

		lpsz = _tcsinc(lpsz);
	}
	
	if (lpszLast != NULL)
	{
		// truncate at trailing space start
		*lpszLast = '\0';
	}
}

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, WORD* pwValue, DWORD dwLength)
{
	char szValue[MAX_STRING_LENGTH];
	LPTSTR lpsz = szValue;
	LPTSTR lpszTemp;
	long i = 0;

	if (dwLength < 1)
		return FALSE;
	
	::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_STRING_LENGTH, m_lpszPath);
	StringReplaceWithBlank(lpsz);

	if (0 == strlen(lpsz))
		return FALSE;

	do 
	{
		StringTrim(lpsz);
		lpszTemp = strchr(lpsz, CHAR_BLANK);

		if (lpszTemp)
		{
			*lpszTemp = '\0';
			*(pwValue + i) = (WORD)atol(lpsz);
			i++;
			lpsz = lpszTemp + 1; 
		}

	} while ((i < (long)dwLength) && lpszTemp);
	
	if ((i < (long)dwLength) && !lpszTemp)
	{
		*(pwValue + i) = (WORD)atol(lpsz);
	}

	return TRUE;
}

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, int* pnValue, DWORD dwLength)
{
	char szValue[MAX_STRING_LENGTH];
	LPTSTR lpsz = szValue;
	LPTSTR lpszTemp;
	long i = 0;
	
	if (dwLength < 1)
		return FALSE;
	
	::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_STRING_LENGTH, m_lpszPath);
	StringReplaceWithBlank(lpsz);
	
	if (0 == strlen(lpsz))
		return FALSE;
	
	do 
	{
		StringTrim(lpsz);
		lpszTemp = strchr(lpsz, CHAR_BLANK);
		
		if (lpszTemp)
		{
			*lpszTemp = '\0';
			*(pnValue + i) = atoi(lpsz);
			i++;
			lpsz = lpszTemp + 1; 
		}
		
	} while ((i < (long)dwLength) && lpszTemp);
	
	if ((i < (long)dwLength) && !lpszTemp)
	{
		*(pnValue + i) = atoi(lpsz);
	}

	return TRUE;
}

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, long* plValue, DWORD dwLength)
{
	char szValue[MAX_STRING_LENGTH];
	LPTSTR lpsz = szValue;
	LPTSTR lpszTemp;
	long i = 0;
	
	if (dwLength < 1)
		return FALSE;
	
	::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_STRING_LENGTH, m_lpszPath);
	StringReplaceWithBlank(lpsz);
	
	if (0 == strlen(lpsz))
		return FALSE;
	
	do 
	{
		StringTrim(lpsz);
		lpszTemp = strchr(lpsz, CHAR_BLANK);
		
		if (lpszTemp)
		{
			*lpszTemp = '\0';
			*(plValue + i) = atol(lpsz);
			i++;
			lpsz = lpszTemp + 1; 
		}
		
	} while ((i < (long)dwLength) && lpszTemp);
	
	if ((i < (long)dwLength) && !lpszTemp)
	{
		*(plValue + i) = atol(lpsz);
	}

	return TRUE;	
}

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD* pdwValue, DWORD dwLength)
{
	char szValue[MAX_STRING_LENGTH];
	LPTSTR lpsz = szValue;
	LPTSTR lpszTemp;
	long i = 0;
	
	if (dwLength < 1)
		return FALSE;
	
	::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_STRING_LENGTH, m_lpszPath);
	StringReplaceWithBlank(lpsz);
	
	if (0 == strlen(lpsz))
		return FALSE;
	
	do 
	{
		StringTrim(lpsz);
		lpszTemp = strchr(lpsz, CHAR_BLANK);
		
		if (lpszTemp)
		{
			*lpszTemp = '\0';
			*(pdwValue + i) = atol(lpsz);
			i++;
			lpsz = lpszTemp + 1; 
		}
		
	} while ((i < (long)dwLength) && lpszTemp);
	
	if ((i < (long)dwLength) && !lpszTemp)
	{
		*(pdwValue + i) = atol(lpsz);
	}

	return TRUE;	
}

BOOL CGIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, double* pdValue, DWORD dwLength)
{
	char szValue[MAX_STRING_LENGTH];
	LPTSTR lpsz = szValue;
	LPTSTR lpszTemp;
	long i = 0;
	
	if (dwLength < 1)
		return FALSE;
	
	::GetPrivateProfileString(lpszSection, lpszKey, "", szValue, MAX_STRING_LENGTH, m_lpszPath);
	StringReplaceWithBlank(lpsz);
	
	if (0 == strlen(lpsz))
		return FALSE;
	
	do 
	{
		StringTrim(lpsz);
		lpszTemp = strchr(lpsz, CHAR_BLANK);
		
		if (lpszTemp)
		{
			*lpszTemp = '\0';
			*(pdValue + i) = atof(lpsz);
			i++;
			lpsz = lpszTemp + 1; 
		}
		
	} while ((i < (long)dwLength) && lpszTemp);
	
	if ((i < (long)dwLength) && !lpszTemp)
	{
		*(pdValue + i) = atof(lpsz);
	}

	return TRUE;	
}

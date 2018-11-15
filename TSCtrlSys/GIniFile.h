#ifndef __GINIFILE_H__
#define __GINIFILE_H__

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef char*               LPTSTR;
typedef const char*         LPCTSTR;

class CGIniFile  
{
public:
	CGIniFile();
	CGIniFile(LPCTSTR lpszPath);
	virtual ~CGIniFile();

	void SetPath(LPCTSTR lpszPath);

	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue);
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, WORD wValue);
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, int nValue);
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, long lValue);
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD dwValue);
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, double dValue, WORD wPrecision = 6);

	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPTSTR lpszValue);
	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, WORD& wValue);
	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, int& nValue);
	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, long& lValue);
	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD& dwValue);
	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, double& dValue);

	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, WORD* pwValue, DWORD dwLength = 1);
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, int* pnValue, DWORD dwLength = 1);
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, long* plValue, DWORD dwLength = 1);
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD* pdwValue, DWORD dwLength = 1);
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, double* pdValue, DWORD dwLength = 1, WORD wPrecision = 6);

	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, WORD* pwValue, DWORD dwLength = 1);
	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, int* pnValue, DWORD dwLength = 1);
	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, long* plValue, DWORD dwLength = 1);
	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD* pdwValue, DWORD dwLength = 1);
	BOOL Read(LPCTSTR lpszSection, LPCTSTR lpszKey, double* pdValue, DWORD dwLength = 1);
	
private:
	LPTSTR m_lpszPath;
};

#endif // __GINIFILE_H__

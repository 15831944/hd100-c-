
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS
#define _USE_MATH_DEFINES


#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����
#include <atlconv.h>		// ����ת��

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#include <afxsock.h>            // MFC �׽�����չ
#include <afxdlgs.h>
#include <map>

#include "CSVOperator.h"
#include "GlobalDef.h"
//////////////////////////////////////////////////////////////////
// #include "AVT/FGCamera.h"
// #include "AVT/convert.h"
// #pragma comment(lib, "AVT/FGCamera.lib")

extern const int IMG_WIDTH;
extern const int IMG_HEIGHT;

#include "Function.h"
#include "../../Include/TSC.h"
#pragma comment(lib,"../../Lib/TSC.lib")

#include "MathEx/MathEx.h"
#pragma comment(lib,"MathEx/MathEx.lib")

//////////////////////////////////////////////////////////////////////////
// ������� 
#include "Includes_HK/MvCameraControl.h"	// �������ͷ�ļ�

//////////////////////////////////////////////////////////////////////////
//							Mil  ͷ�ļ�									//
//////////////////////////////////////////////////////////////////////////
#include "mil.h"
#include ".\MilDyn\milpat.h"
#include ".\MilDyn\MilMod.h"
#pragma comment(lib,"mil.lib")
#pragma comment(lib,"milim.lib")
#pragma comment(lib,"milpat.lib")
#pragma comment(lib,"milmod.lib")
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#include "WEdit.h"
#include "CButtonST/ShadeButtonST.h"

//////////////////////////////////////////////////////////////////////////
#include "KFTimer.h"

//////////////////////////////////////////////////////////////////////////
// ʹ�õĹ����
#define NUM_CHANNEL 1
//////////////////////////////////////////////////////////////////////////

extern CEvent g_runListEvent;

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



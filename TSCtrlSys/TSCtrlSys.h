
// TSCtrlSys.h : TSCtrlSys Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "MainFrm.h"
#include "TSCtrlSysDoc.h"
#include "TSCtrlSysView.h"
//#include "ComPlc.h"
#include "SerialPort.h"
#include "UserManage.h"
#include <direct.h>

enum Mod_Index
{
	Mod_Calib,
	Mod_Mark1,
	Mod_Mark2,
	Mod_Mark3,
	Mod_Mark4
};

#define Valve_Kaifa		"Kaifa"
#define Valve_Vemers	"Vemers"
#define Valve_Musashi	"Musashi"
#define Valve_Yikai		"Yikai"

#define ValveCtrl_AxisPul	"������"
#define ValveCtrl_IO		"IO����"
#define ValveCtrl_CompareIO	"λ�ñȽ�"

#include "../../../../include/EncryptOp.h"
//#pragma comment(lib,"../../Lib/EncryptOp.lib")

//////////////////////////////////////////////////////////////////////////
extern CProg_List g_AllProgInfo[NUM_CHANNEL];	// ��¼���в�Ʒ����
extern CProg_Data g_ProgData;		// ��ǰ�Ĳ�Ʒ��Ϣ


int Round(double val);

// CTSCtrlSysApp:
// �йش����ʵ�֣������ TSCtrlSys.cpp
//
class CTSCtrlSysApp : public CWinAppEx
{
public:
	CTSCtrlSysApp();
  
// ��д
public:
	virtual BOOL InitInstance();
//    CComPlc		m_Serial;
// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	CUserManage		m_SysUser;
    CEncryptOp      m_EncryptOp;   // ����ģ��
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	//////////////////////////////////////////////////////////////////////////
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnFileOpen();
};

extern CTSCtrlSysApp theApp;
extern CMainFrame *g_pFrm;
extern CTSCtrlSysDoc *g_pDoc;
extern CString szCmdline[];
extern CTSCtrlSysView *g_pView;

extern BOOL g_bClose;   //�Ƿ�ر��˿��ƿ�
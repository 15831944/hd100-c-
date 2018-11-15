
// TSCtrlSys.h : TSCtrlSys 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
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

#define ValveCtrl_AxisPul	"轴脉冲"
#define ValveCtrl_IO		"IO控制"
#define ValveCtrl_CompareIO	"位置比较"

#include "../../../../include/EncryptOp.h"
//#pragma comment(lib,"../../Lib/EncryptOp.lib")

//////////////////////////////////////////////////////////////////////////
extern CProg_List g_AllProgInfo[NUM_CHANNEL];	// 记录所有产品名称
extern CProg_Data g_ProgData;		// 当前的产品信息


int Round(double val);

// CTSCtrlSysApp:
// 有关此类的实现，请参阅 TSCtrlSys.cpp
//
class CTSCtrlSysApp : public CWinAppEx
{
public:
	CTSCtrlSysApp();
  
// 重写
public:
	virtual BOOL InitInstance();
//    CComPlc		m_Serial;
// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	CUserManage		m_SysUser;
    CEncryptOp      m_EncryptOp;   // 加密模块
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

extern BOOL g_bClose;   //是否关闭了控制卡
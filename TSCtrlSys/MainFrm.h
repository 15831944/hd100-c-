
// MainFrm.h : CMainFrame 类的接口
//
#pragma once

#include "Resource.h"

#include "MFCRightBar.h"
#include "MFCOutputWnd.h"
//#include "CmdEditBar.h"
#include "DlgPathEdit.h"
#include "DlgPixel.h"
#include "DlgMark.h"
#include "DlgPosInfo.h"
#include "DlgShotCut.h"
#include "DlgIOCtrl.h"
#include "DlgProgOffset.h"
#include "DlgAxisPos.h"
#include "DlgCircle.h"
#include "DlgGoto.h"    //GoAddress
#include "DlgZHeight.h"
#include "DlgMachineSet.h"
//#include "DlgJetAdjust.h"
#include "DlgRepeatZheight.h"
#include "BoardCalibDlgEx.h"
#include "CmdRun.h"
#include "DlgDispenserParam.h"
//#include "ComPlc.h"
#include "SerialPort.h"
#include "ConfigOp.h"
#include "DlgWcLine.h"
#include "DlgWcPoint.h"
#include "DlgWCRect.h"
#include "UserRegister.h"
#include "DlgNeedleAdjust.h"
#include "DlgNeedleZAdjust.h"
#include "FeedModelCtrlDlg.h"
#include "DlgZBaseHightSet.h"
#include "Precision.h"
#include "DlgTrack.h"
#include "DlgTrackDraw.h"

#include "DlgDualValve.h"
#include "DlgParamDot.h"
#include "DlgParamLine.h"

// com口外设
#include "ComDisp.h"
#include "ComLaser.h"
#include "ComWeight.h"
#include "ComLight.h"

//#pragma comment(lib,"../../Lib/ConfigOp.lib")
class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return FALSE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); }
};

class CMainFrame : public CFrameWndEx
{	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// 属性
public:
	BOOL m_bSocketEnable;//远程网络开关

	CBitmap m_BitmapRun;
	CBitmap m_BitmapPause;
	CBitmap m_BitmapStop;
	CBitmap m_BitmapEStop;
	CBitmap m_BitmapReady;

	CConfigOp  *LanguageMgr; 
	void OnReadLanguageFile();
	// 操作
public:
	void AddMsg(CString strText);

	//////////////////////////////////////////////////////////////////////////
	//							文档操作									//
	//////////////////////////////////////////////////////////////////////////
	void RobotParam(BOOL bRead);		// 机器位置配置
	void SysParam(BOOL bRead);			// 配置文件操作
	void MotorParam(BOOL bRead);		// 马达参数操作
	void ComPortParam(BOOL bRead);		// 串口定义操作
	void InitComPort();

	LRESULT OnMessageInputEvent(WPARAM wparam, LPARAM lparam);
	LRESULT OnMessagePosEvent(WPARAM wparam, LPARAM lparam);
	LRESULT OnMessagePaneStatus(WPARAM wParam, LPARAM lParam);

	void DotParam(BOOL bRead);			// 点参数操作
	void LineParam(BOOL bRead);			// 线参数操作
	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // 控件条嵌入成员
	CMFCToolBar			m_wndToolBar;
	CMFCToolBar			m_DrawToolBar;
	CMFCStatusBar		m_wndStatusBar;
//	CMFCToolBarImages	m_UserImages;
	CMFCRightBar		m_wndRightBar;
	CMFCTitleBar		m_wndTitleBar;

	COutputWnd			m_wndOutput;
	CMFCOutlookBar		m_wndNavigationBar;

	CDlgPathEdit		m_wndEditBar;
	//CCmdEditBar		m_wndEditBar;
public:
	CCmdRun				*m_Robot;
	MOTOR_PARAM			m_mtrParamGts[MAX_AXIS_NUM];	// 固高卡轴参数
	MOTOR_PARAM			m_mtrParamLs[LS_AXIS_NUM];		// 雷赛卡轴参数
	tgSysParam			*m_pSysParam;		// 系统参数
	tgRobotParam		*m_pRobotParam;		// 机器位置参数
	tgBatetWCDate       m_BatetWCDate[MAX_WCDATE];
	//////////////////////////////////////////////
	CDlgAxisPos			m_dlgAxisPos;
	CDlgCircle			m_dlgCircle;
	CDlgZHeight       	m_dlgZHeight;
	CDlgRepeatZheight   m_dlgRepeatZHeight;
	CDlgGoto			m_dlgSkipRun;
	CDlgProgOffset		m_dlgProgOffset;
	CDlgMark			m_dlgMark;
	CDlgPixel			m_dlgPixel;
	CDlgMachineSet		m_dlgMachineSet;
//	CDlgJetAdjust       m_dlgJetAdjust;
	CDlgIOCtrl			m_dlgIOCtrl;
	CDlgTrack			m_dlgTrack;
	CDlgTrackDraw		m_dlgTrackDraw;
	CDlgShotCut			m_dlgShotCut;
	CBoardCalibDlgEx    m_dlgBoardCheckEx;
	CDlgWcLine          m_dlgWCL;			// 称重划线
	CDlgWcPoint         m_DlgWCP;			// 称重打点
	CDlgWCRect          m_dlgWcRect;		// 称重矩形
	CDlgNeedleAdjust	m_dlgNeedleAdjust;
	CDlgNeedleZAdjust   m_dlgNeedleZAdjust;
	CFeedModelCtrlDlg   m_DlgFeedCtrlMdl;
	CDlgDispenserParam  m_DlgDispParam;
	CDlgZBaseHightSet   m_DlgZBaseSet;
	CDlgDualValve		m_DlgDualValve;		// 双阀设置界面

	CPrecision m_Precision;
	BOOL  bisWeightError;

	DOT_PARAM	m_DotParam[MAX_DOT_PARAM_NUM];
	LINE_PARAM	m_LineParm[MAX_LINE_PARAM_NUM];
	CDlgParamDot	 m_dlgDotParam;
	CDlgParamLine	 m_dlgLintParam; 
	// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnToolCmd(UINT ID);
	afx_msg void OnUpdateToolCmd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLogon(CCmdUI* pCmdUI);

	afx_msg void OnSocketEnable();
	afx_msg void OnUpdateSocketEnable(CCmdUI* pCmdUI);
	afx_msg void OnRS232Config();
	afx_msg void OnUpdateRS232Config(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStatusBar(CCmdUI* pCmdUI);

	void SetDockingWindowIcons(BOOL bHiColorIcons);
//	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID,CCmdEditBar& calendar, int nInitialWidth);

	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);

	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;
	
public:
	void SetPanelTextEx(UINT ID, CString strText);
	void ShowPathEditDialog();
	void ShowProgOffsetDialog();
	void ShowMachineSetDlg();
	void ShowSysDialog();
	void SetWindowTextEx();
	void ShowIODialog();
	void ShowTrackDialog();
	void ShowPathDrawDlg();
	void ShowMarkDialog(tgCmdLine *pCmdLine=NULL, int iSelIndex=-1);
	void ShowPixelDialog();
	void ShowAxisPosDialog(tgCmdLine *pCmdLine, int iSelIndex=-1);
	void ShowCircleDialog(tgCmdLine *pCmdLine, int iSelIndex=-1);
	void ShowZHEIGHTDialog(tgCmdLine *pCmdLine, int iSelIndex);
	void ShowWCRectDialog(tgCmdLine *pCmdLine, int iSelIndex=-1);
	void ShowRepeatZHDialog(tgCmdLine *pCmdLine, int iSelIndex=-1);
	void ShowBoardCheckDlgEx(tgCmdLine *pCmdLine, int iSelIndex);
	void ShowWCLDialog(tgCmdLine *pCmdLine, int iSelIndex);
	void ShowWCPDialog(tgCmdLine *pCmdLine, int iSelIndex);
	void ShowNeedleAdjustDialog();
	void ShowNeedleZAdjustDialog();
	void ShowFeedMdlCtrlDlg();
	void ShowDispParam();
	void ShowZBaseSetDlg();
	void ShowDualValveDlg();
	void ShowDotParamDialog(tgCmdLine *pCmdLine, int iSelIndex=-1);
	void ShowLineParamDialog(tgCmdLine *pCmdLine, int iSelIndex=-1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCmdReopenFile();
	afx_msg void OnClose();
	afx_msg void OnViewCmdEdit();
	afx_msg void OnLogon();
	afx_msg void OnLogoff();
	afx_msg void OnDestroy();

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);


	//////////////////////////////////////////////////////////////////////////
	//							串口操作									//
	//////////////////////////////////////////////////////////////////////////
	SerialPort	m_ComPort[5];					// 串口定义： 点胶阀1	点胶阀2	激光测高	光源控制器	称重
	SerialPort*	GetComPort(CString strPorName);	// 串口定义： 点胶阀1	点胶阀2	激光测高	光源控制器	称重
	CComDisp	m_comDisp[2];		// 点胶阀对象
	CComWeight	m_comWeight;		// 电子秤对象
	CComLaser	m_comLaser;			// 激光测高仪对象
	CComLight	m_comLight;			// 光源控制器对象


	int m_nProcesserNum;	// CPU内核数目
	int m_nUIRunProcesser;	// 点胶线程运行于指定内核上。除点胶线程以外的线程可运行的内核
};
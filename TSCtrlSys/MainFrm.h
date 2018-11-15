
// MainFrm.h : CMainFrame ��Ľӿ�
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

// com������
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
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// ����
public:
	BOOL m_bSocketEnable;//Զ�����翪��

	CBitmap m_BitmapRun;
	CBitmap m_BitmapPause;
	CBitmap m_BitmapStop;
	CBitmap m_BitmapEStop;
	CBitmap m_BitmapReady;

	CConfigOp  *LanguageMgr; 
	void OnReadLanguageFile();
	// ����
public:
	void AddMsg(CString strText);

	//////////////////////////////////////////////////////////////////////////
	//							�ĵ�����									//
	//////////////////////////////////////////////////////////////////////////
	void RobotParam(BOOL bRead);		// ����λ������
	void SysParam(BOOL bRead);			// �����ļ�����
	void MotorParam(BOOL bRead);		// ����������
	void ComPortParam(BOOL bRead);		// ���ڶ������
	void InitComPort();

	LRESULT OnMessageInputEvent(WPARAM wparam, LPARAM lparam);
	LRESULT OnMessagePosEvent(WPARAM wparam, LPARAM lparam);
	LRESULT OnMessagePaneStatus(WPARAM wParam, LPARAM lParam);

	void DotParam(BOOL bRead);			// ���������
	void LineParam(BOOL bRead);			// �߲�������
	// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // �ؼ���Ƕ���Ա
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
	MOTOR_PARAM			m_mtrParamGts[MAX_AXIS_NUM];	// �̸߿������
	MOTOR_PARAM			m_mtrParamLs[LS_AXIS_NUM];		// �����������
	tgSysParam			*m_pSysParam;		// ϵͳ����
	tgRobotParam		*m_pRobotParam;		// ����λ�ò���
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
	CDlgWcLine          m_dlgWCL;			// ���ػ���
	CDlgWcPoint         m_DlgWCP;			// ���ش��
	CDlgWCRect          m_dlgWcRect;		// ���ؾ���
	CDlgNeedleAdjust	m_dlgNeedleAdjust;
	CDlgNeedleZAdjust   m_dlgNeedleZAdjust;
	CFeedModelCtrlDlg   m_DlgFeedCtrlMdl;
	CDlgDispenserParam  m_DlgDispParam;
	CDlgZBaseHightSet   m_DlgZBaseSet;
	CDlgDualValve		m_DlgDualValve;		// ˫�����ý���

	CPrecision m_Precision;
	BOOL  bisWeightError;

	DOT_PARAM	m_DotParam[MAX_DOT_PARAM_NUM];
	LINE_PARAM	m_LineParm[MAX_LINE_PARAM_NUM];
	CDlgParamDot	 m_dlgDotParam;
	CDlgParamLine	 m_dlgLintParam; 
	// ���ɵ���Ϣӳ�亯��
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
	//							���ڲ���									//
	//////////////////////////////////////////////////////////////////////////
	SerialPort	m_ComPort[5];					// ���ڶ��壺 �㽺��1	�㽺��2	������	��Դ������	����
	SerialPort*	GetComPort(CString strPorName);	// ���ڶ��壺 �㽺��1	�㽺��2	������	��Դ������	����
	CComDisp	m_comDisp[2];		// �㽺������
	CComWeight	m_comWeight;		// ���ӳӶ���
	CComLaser	m_comLaser;			// �������Ƕ���
	CComLight	m_comLight;			// ��Դ����������


	int m_nProcesserNum;	// CPU�ں���Ŀ
	int m_nUIRunProcesser;	// �㽺�߳�������ָ���ں��ϡ����㽺�߳�������߳̿����е��ں�
};
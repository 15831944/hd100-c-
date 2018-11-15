#include "DlgShotCut.h"
#include "paramdef.h"
#include <Afxtempl.h>
#include <vector>

#pragma once
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CCmdEditBar ����

class CCmdEditBar : public CDialogEx// CWnd
{
public:
	CCmdEditBar();
	virtual ~CCmdEditBar();

// ����
public:
	CDlgShotCut	m_wndShotCutEdit;
	CImageList	m_Images;
	CListCtrl	m_wndList;
	BOOL		m_bInsertCmd;
	CListBox	*m_pMsgList;
	CList <tgCmdLine, tgCmdLine&> m_rawList;  // ��ʱ��������
	CList <tgCmdLine,tgCmdLine&>  m_Tls;
public:
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID = (UINT)-1);
    BOOL CalcBoradArcData(double &dx,double &dy,double dCurXdis,double dCurYdis,double dArc);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCmdType(UINT ID);
protected:
//	afx_msg void OnClickListCmd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCmd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListCmd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLvnItemchangedListCmd(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void UpateCmdListBox(int iSelIndex);
	BOOL CheckMark();
    void ShowTrackPopupMenu(int MenuID);

	int GetPreTrackPoint(int iFrmIndex, BOOL bOnlyLinePoint=FALSE);
	int GetNextTrackPoint(int iFrmIndex, BOOL bOnlyLinePoint=FALSE);

	BOOL GetCmdArcInfo(tgCmdLine &tArcCmd, int iArcCmdIndex, tgArc *pArc);
	BOOL GetCmdArcInfo(tgCmdLine tPreDo,tgCmdLine tPassDo,tgCmdLine tNextDo, tgArc *pArc);

	int  GetPreTrackPointEx(int iFrmIndex, BOOL bOnlyLinePoint=FALSE);
	int  GetNextTrackPointEx(int iFrmIndex, BOOL bOnlyLinePoint=FALSE);
	BOOL GetCmdArcInfoEx(tgCmdLine *tArcCmd0, tgCmdLine *tArcCmd1,tgCmdLine *tArcCmd2,tgArc *pArc);
	BOOL TrackArcHandle(tgCmdLine &tPreDot, tgCmdLine &tPassDot, tgCmdLine &tNextDot, double dRound,double &Cx,double &Cy,BOOL &nDir);

	BOOL GetCmdCircleInfo(tgCmdLine &tCircleCmd, tgCircle *pArc);
	BOOL GetCmdBrushInfo(tgCmdLine &tCmd, tgBrush *pArea);
   
	void SaveCmdLine(LPCTSTR strFile);
	void ReadCmdLine(LPCTSTR strFile);
	
	static BOOL GetCircle(const tgCmdLine *tgCmdLine, double *pXc, double *pYc, double *pRc);
	static BOOL GetCircle(double dX[3], double dY[3], double *pXc, double *pYc, double *pRc);		
	static BOOL IsPtInTrangle(double x, double y, double dx[3], double dy[3]);
	static double GetTrangleArea(double dx[3], double dy[3]);
	static BOOL IsTrackPoint(tgCmdLine &tCmd, BOOL bOnlyLinePoint=FALSE);

	void FreeCmdList();
	int  GetCmdCurSel();
	void SetCmdCurSel(int nSelIndex);

	//����༭����
	void DeleteCurSel();
	void CutCurSel();
	void PasteData();
	void CopyCurSel();
   
	BOOL IsSelState(int iIndex);
	BOOL IsHaveRepeat(int iFrmIndex); //����ָ����Ƿ��������ָ��(������iFrmIndex, ��iFrmIndex+1��ʼ)

	void LoadListArrayPos(tgCmdLine &tCmd, int iStartDbelIndex=0);
	void SetZSame(int iFrmIndex);
	int ModifiedCmd(int iCmdIndex);

	//�����ǰ�ؽ�λ��dLen:��ǰ�ؽ�����
	BOOL GetEndPos(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py);
	BOOL GetEndPosEx(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py);
	BOOL GetEndActPos(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py,int nType);
	BOOL GetEndCirPos(tgCircle& tArc,double dfStrtAngle,double dLen, double *px, double *py,int nType);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


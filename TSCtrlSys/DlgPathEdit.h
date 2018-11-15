#pragma once
#include "DlgShotCut.h"
#include "paramdef.h"
#include <Afxtempl.h>
#include "ColorButton.h"
#include <vector>
#include "afxwin.h"

using namespace std;

// CDlgPathEdit 对话框

enum DOT_TYPE
{
	Dot1=0,
	Dot2,
	Dot3,
	Dot4,
	Dot5,
	Dot6,
	Dot7,
	Dot8,
	Dot9,
	Dot10
};

enum LINE_TYPE
{
	Line1=0,
	Line2,
	Line3,
	Line4,
	Line5,
	Line6,
	Line7,
	Line8,
	Line9,
	Line10,
};

class CDlgPathEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPathEdit)

public:
	CDlgPathEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPathEdit();

// 对话框数据
	enum { IDD = IDD_DLG_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	// 属性
public:
	CDlgShotCut	m_wndShotCutEdit;
	CImageList	m_Images;
	CListCtrl	m_wndList;
	BOOL		m_bInsertCmd;
	CListBox	*m_pMsgList;
	CList <tgCmdLine, tgCmdLine&> m_rawList;  // 临时命令链表
	CList <tgCmdLine,tgCmdLine&>  m_Tls;
public:
//	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID = (UINT)-1);
	BOOL CalcBoradArcData(double &dx,double &dy,double dCurXdis,double dCurYdis,double dArc);
protected:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCmdType(UINT ID);
protected:
	CFont m_fontEdit;
	int m_nClickRow; 
	int m_nClickCol;
	afx_msg void OnClickListCmd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCmd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListCmd(NMHDR* pNMHDR, LRESULT* pResult);

public:
	double m_dGWTotal, m_dGWFront, m_dGWBack;	//总胶重   正面胶重    背面胶重
	void UpdateCmdListBox(int iSelIndex);
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
	static BOOL IsParamPoint(tgCmdLine &tCmd, BOOL bParamDot = TRUE);

	void FreeCmdList();
	int  GetCmdCurSel();
	void SetCmdCurSel(int nSelIndex);

	//命令编辑功能
	void DeleteCurSel();
	void CutCurSel();
	void PasteData();
	void CopyCurSel();

	BOOL IsSelState(int iIndex);
	BOOL IsHaveRepeat(int iFrmIndex); //检测该指令后是否存在阵列指令(不包括iFrmIndex, 从iFrmIndex+1开始)

	void LoadListArrayPos(tgCmdLine &tCmd, int iStartDbelIndex=0);
	void SetZSame(int iFrmIndex);
	int ModifiedCmd(int iCmdIndex);

	//获得提前关胶位置dLen:提前关胶长度
	BOOL GetEndPos(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py);
	BOOL GetEndPosEx(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py);
	BOOL GetEndActPos(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py,int nType);
	BOOL GetEndCirPos(tgCircle& tArc,double dfStrtAngle,double dLen, double *px, double *py,int nType);
	virtual BOOL OnInitDialog();

	CButtonST m_btnEdit[10];
	afx_msg void OnBnClickedBtnShowpath();
	void EnableEditButton(BOOL bEnable);
	afx_msg void OnBnClickedButtonEditSingle();
	afx_msg void OnBnClickedButtonEditAll();
	// 编辑点胶参数时用到的控件
	CEdit m_editModify;

	// -------------------------------------------------------------------------------------
	// 在点击到列表相应单元格的时候，在相应的位置生成一个跟单元格大小相当的下拉列表框并贴在单元格上面
	int e_Item;				// 刚编辑的行  
	int e_SubItem;			// 刚编辑的列
	bool haveccomboboxcreate;		// 标志下拉列表框已经被创建

	CComboBox m_CtrlComboBoxModify;	// 生产单元格下拉列表对象
	
	// 创建单元格下拉列表框函数
	void CreateCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat);
	// 销毁单元格下拉列表框
	void DistroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem);
	afx_msg void OnKillfocusCcomboBox();		// 动态生成下拉列表框失去焦点响应函数

	afx_msg void OnKillfocusEditModify();
	afx_msg void OnBnClickedBtnReviewF();
	afx_msg void OnBnClickedBtnReviewB();
	afx_msg void OnBnClickedBtnSave();
	
	afx_msg void OnBnClickedBtnParam();
};
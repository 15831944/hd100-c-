#pragma once
//#include "e:\work\kaifa\16_hd100_std\hd100_ver1.0.0.20181107\hd100\tsctrlsys\grid\gridctrl_src\gridctrl.h"
#include "Resource.h"
#include "Grid/GridCtrl_src/GridCtrl.h"

// CDlgParamDot �Ի���
#define DOT_GRID_HEIGHT	10
#define DOT_GRID_COL_NUM	13
class CDlgParamDot : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgParamDot)

public:
	CDlgParamDot(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgParamDot();

// �Ի�������
	enum { IDD = IDD_DLG_PARAM_DOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	void CreateGrid();
	void SaveGrid();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CGridCtrl m_DotParamGrid;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnParamDotSave();
};

#pragma once
#include "Resource.h"
#include "Grid/GridCtrl_src/GridCtrl.h"
// CDlgParamLine �Ի���
#define LINE_GRID_HEIGHT	10
#define LINE_GRID_COL_NUM	15
class CDlgParamLine : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgParamLine)

public:
	CDlgParamLine(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgParamLine();

// �Ի�������
	enum { IDD = IDD_DLG_PARAM_LINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	void CreateGrid();
	void SaveGrid();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CGridCtrl m_LineParamGrid;
	afx_msg void OnBnClickedBtnParamLineSave();
};

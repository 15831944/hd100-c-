#pragma once

#include "resource.h"
#include "Grid/GridCtrl_src/GridCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"

// CPageMotor 对话框
#define COMPORT_GRID_HEIGHT		32
#define COMPORT_GRID_COL_NUM	3

// CPageComPort 对话框

class CPageComPort : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageComPort)

public:
	CPageComPort();
	virtual ~CPageComPort();

// 对话框数据
	enum { IDD = IDD_PAGE_COMPORT };

	CImageList m_ImageList;
	CGridCtrl m_ComPortGrid;
	int nValCH1, nValCH2;

protected:
	void CreateGrid();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnLink();
	afx_msg void OnBnClickedBtnWtSet0();
	afx_msg void OnBnClickedBtnWtRead();
	afx_msg void OnBnClickedBtnLaRead();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnSaveLight();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_sliderLight;
	CScrollBar m_scrollLight;
	CScrollBar m_scrollLight2;
};

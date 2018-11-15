#pragma once

#include "Grid/GridCtrl_src/GridCtrl.h"



#define OUT_POINT			16
#define EX_OUT_POINT		16

#define OUTPUT_GRID_HEIGHT		32
#define OUTPUT_GRID_COL_NUM		3

// CPageOut dialog

class CPageOut : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageOut)

public:
	CPageOut();
	virtual ~CPageOut();

// Dialog Data
	enum { IDD = IDD_PAGE_OUT };

	int		m_nFixCols;
	int		m_nFixRows;
	int		m_nCols;
	int		m_nRows;
	BOOL	m_bEditable;
	BOOL	m_bHorzLines;
	BOOL	m_bListMode;
	BOOL	m_bVertLines;
	BOOL	m_bSelectable;
	BOOL	m_bAllowColumnResize;
	BOOL	m_bAllowRowResize;
	BOOL	m_bHeaderSort;
	BOOL	m_btitleTips;
	BOOL	m_bSingleSelMode;
	BOOL    m_bSingleColSelMode;
	BOOL	m_bTrackFocus;
	BOOL	m_bFrameFocus;
	BOOL	m_bVirtualMode;
	BOOL	m_bCallback;
	BOOL    m_bVertical;
	BOOL    m_bExpandUseFixed;

	CGridCtrl m_OutputGrid;
	CGridCtrl m_OutputGridEx;
	int m_curSelTable;

	//////////////////////////////////////////////////////////////////////////
public:
	CImageList m_ImageList;
	//CStringArray  sArrayModeid,sArrayInfo;

	void UpdateIOSts(long sts);
	void UpdateIOStsEx(long sts);

	static BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam);
protected:
	void CreateGrid();
	void CreateGridEx();

	void ReadOutputMap();
	void ReadOutputMapEx();

	void OnVirtualMode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnGridDbClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridDbClickEx(NMHDR *pNotifyStruct, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
//	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
public:
	afx_msg void OnBnClickedDlgIoOutOpen();
	afx_msg void OnBnClickedDlgIoOutClose();
	afx_msg void OnBnClickedDlgIoOutJet();
};

#pragma once

#include "Grid/GridCtrl_src/GridCtrl.h"
//#include "IniFile.h"


#define IN_POINT	16		//ø®IO
#define EX_IN_POINT	16		//Õÿ’πIO

#define INPUT_GRID_HEIGHT		32
#define INPUT_GRID_COL_NUM		3


// CPageIn dialog

class CPageIn : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageIn)

public:
	CPageIn();
	virtual ~CPageIn();

// Dialog Data
	enum { IDD = IDD_PAGE_IN };

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

	CGridCtrl m_InputGrid;
	CGridCtrl m_ExInputGrid;

	//////////////////////////////////////////////////////////////////////////
public:
	CImageList m_ImageList;
    void GetPLCRSts();
	void UpdateIOSts(long sts);
	void UpdateExIOSts(long sts);

	static BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam);
protected:
	void CreateGrid();
	void CreateGridEx(void);
	void ReadInputMap();
	void ReadExInputMap();
	void OnVirtualMode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

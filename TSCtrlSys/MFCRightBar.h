#pragma once

#include "DlgPosInfo.h"


// CMFCRightBar

class CMFCRightBar: public CDockablePane
{
	DECLARE_DYNAMIC(CMFCRightBar)

public:
	CMFCRightBar();
	virtual ~CMFCRightBar();

	CDlgPosInfo m_DlgPosInfo;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnShowControlBarMenu(CPoint point);
	  virtual BOOL CanBeClosed()const;
};

// CMFCTitleBar
#include "DlgTitle.h"

class CMFCTitleBar: public CDockablePane
{
	DECLARE_DYNAMIC(CMFCTitleBar)

public:
	CMFCTitleBar();
	virtual ~CMFCTitleBar();

	CDlgTitle m_DlgTitle;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
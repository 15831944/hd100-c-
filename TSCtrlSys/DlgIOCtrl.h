#pragma once
#include "afxcmn.h"

#include "PageIn.h"
#include "PageOut.h"
#include "PageMotor.h"
#include "PageComPort.h"
#include "DlgTrack.h"

// CDlgIOCtrl dialog

class CDlgIOCtrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgIOCtrl)

public:
	CDlgIOCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgIOCtrl();

// Dialog Data
	enum { IDD = IDD_DLG_IOCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl		m_tabCtrl;
	CPageIn			pageInput;
	CPageOut		pageOutput;
	CPageMotor		pageMtrParam;
	CPageComPort	pageComPort;
	CDlgTrack		pageTrack;

	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeDlgIoTab(NMHDR *pNMHDR, LRESULT *pResult);

	void InitTableCtrl();
};

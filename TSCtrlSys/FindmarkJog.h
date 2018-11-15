#pragma once
#include "Resource.h"
#include "JogButton.h"
// CFindmarkJog dialog

class CFindmarkJog : public CDialog
{
	DECLARE_DYNAMIC(CFindmarkJog)

public:
	CFindmarkJog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFindmarkJog();

// Dialog Data
	enum { IDD = IDD_DLG_LIN_XYZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSetLineYn();
	afx_msg void OnBnClickedSetLineYp();
	afx_msg void OnBnClickedSetLineXn();
	afx_msg void OnBnClickedSetLineXp();
};

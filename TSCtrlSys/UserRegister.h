#pragma once
#include "Resource.h"
#include "../../../../Include/EncryptOp.h"
// CUserRegister dialog

class CUserRegister : public CDialog
{
	DECLARE_DYNAMIC(CUserRegister)

public:
	CUserRegister(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserRegister();

	CString  strDeviceCode;
// Dialog Data
	enum { IDD = IDD_DLG_USERGEGEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtRegesitsoft();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

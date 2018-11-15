#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "./CButtonST/BtnST.h"

// CDlgJetAdjust dialog

class CDlgJetAdjust : public CDialog
{
	DECLARE_DYNAMIC(CDlgJetAdjust)

public:
	CDlgJetAdjust(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgJetAdjust();

// Dialog Data
	enum { IDD = IDD_DLG_JET_ADJUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int Index;
	CProgressCtrl m_Prog;
	CCheckListBox m_ListFun;
	CGridCtrl m_GridPos;

	void CreateGrid(void);	// ����Grid
	void UpdateGrid(void);	// ˢ��Grid����
	bool JetFourAdjust(void);
	void MoveCarm(int PotID);
	bool CapCarmoffsetJet(void);
	void GetCarmPos(int PosID);
	void ShowPos(void);		
	void UpdateCtrl();		// ���¿ؼ���ʾ,������������ʾ������

	virtual BOOL OnInitDialog();
private:
	tgPos       adjustP[4];   //ͼ�����Ļ�еλ��
	tgPos       JetPost[4];   //�����еλ�� 

public:
	afx_msg void OnBnClickedBtEnd();
	afx_msg void OnBnClickedBtStart();
//	afx_msg void OnLbnSelchangeListfun();
	afx_msg void OnBnClickedDlgJetAdjustJog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnMovto();
	afx_msg void OnBnClickedBtnSet();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnContactTest();
	afx_msg void OnBnClickedBtnReadLaser();
	afx_msg void OnBnClickedBtnProbeSwitch();

	CButtonST m_btnHeightRead;
	CButtonST m_btnMovTo;
	CButtonST m_btnSave;
	CButtonST m_btnSetPos;

	afx_msg void OnGridDbClick(NMHDR *pNotifyStruct, LRESULT* pResult);
};

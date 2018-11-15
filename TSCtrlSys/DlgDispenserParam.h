#pragma once
#include "afxwin.h"
#include "ComDisp.h"

// CDlgDispenserParam �Ի���
UINT ThreadDotDisp(LPVOID lParam);

class CDlgDispenserParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDispenserParam)

public:
	CDlgDispenserParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDispenserParam();

// �Ի�������
	enum { IDD = IDD_DLG_DISPPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLoadFromDisp();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void InitScrollBar();
	void InitStaticCtrl(int dispType);
	CScrollBar m_scrollRising;
	CScrollBar m_scrollOpen;
	CScrollBar m_scrollLift;
	CScrollBar m_scrollFalling;
	CScrollBar m_scrollDelay;
	CScrollBar m_scrollPulse;
	CScrollBar m_scrollTemp;

	// �㽺������
	CComboBox m_comboDispIdx;
	// �㽺������
	CComboBox m_comboDispType;
	// ���Ʒ�ʽ		0:������		1:��ͨIO		2:λ�ñȽϸ���IO
	CComboBox m_comboDispCtrl;
	// ��ǰ�㽺����
	DispenserParam m_dispParam;
	afx_msg void OnBnClickedBtnLoadFromFile();
	afx_msg void OnBnClickedBtnSetParam();
//	afx_msg void OnBnClickedBtnSetWtpos();
	afx_msg void OnBnClickedBtnReadPressure();
//	afx_msg void OnBnClickedBtnMvtoWtpos();
	void MvtoWtpos(void);
	afx_msg void OnBnClickedBtnWtAdj();
	afx_msg void OnBnClickedBtnValveOn();
	afx_msg void OnCbnSelchangeComboDispType();
	afx_msg void OnCbnSelchangeComboDispSel();

	void UpdataCtrlVal(bool bIfShow);	// ˢ�½���ռ�ֵ   bIfShow--true:��ʾ����ֵ������    false:�ӽ����ȡֵ����
	afx_msg void OnBnClickedBtnSetPressure();

//	CWEdit m_edit_wtPos_x;
	int m_nWeightType;
	afx_msg void OnBnClickedChkWeight();
	afx_msg void OnBnClickedRadioT();
	afx_msg void OnBnClickedRadioP();
	afx_msg void OnCbnSelchangeComboDispCtrl();
};

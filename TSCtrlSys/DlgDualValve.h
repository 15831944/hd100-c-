#pragma once

#include "CameraHK.h"
// CDlgDualValve �Ի���

class CDlgDualValve : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDualValve)

public:
	CDlgDualValve(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDualValve();

// �Ի�������
	enum { IDD = IDD_DLG_DUAL_VALVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnTeachValve1();
	afx_msg void OnBnClickedBtnMoveValve1();
	afx_msg void OnBnClickedBtnTeachRotate1();
	afx_msg void OnBnClickedBtnTeachRotate2();
	afx_msg void OnBnClickedBtnMoveRotate1();
	afx_msg void OnBnClickedBtnMoveRotate2();
	afx_msg void OnBnClickedBtnRotate();

	// ��ʾͼ��
	BYTE* m_pImageBuffer;	// ������ͷ��ȡ��ͼ������ָ��
	CCameraHK	m_camBottomHK;
	MIL_ID 
		MilDisplay,             /* Display identifier.     */
		MilOverlayImage,		/* OverlayImage identifier.*/
		MilImage;
	void PaintShape(CRect rect);
	int InitBottomCamera();
	LRESULT OnMessageRedrawCCDEvent(WPARAM wparam, LPARAM lparam);

	void UpdateCtrl(bool bShow);	// ���½���ؼ�	bShow=		true:ֵ���µ�����	false:�ӽ����ȡ������
	double m_dRotateMarkX1;
	double m_dRotateMarkX2;
	double m_dRotateMarkY1;
	double m_dRotateMarkY2;
	double m_dValveSpace;
	double m_dValvePosX1;
	double m_dValvePosY1;
	double m_dValvePosZ1;
	double m_dNeedleRad;	// ��ͷͼ����ʾ�뾶
	afx_msg void OnBnClickedBtnMoveValve2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditNeedleRad();
	afx_msg void OnEnChangeEditZStep();
	afx_msg void OnBnClickedBtnMoveValveTd2();
	afx_msg void OnBnClickedBtnMoveValveTd1();
	double m_dZStep;
	afx_msg void OnBnClickedBtnZdown();
	afx_msg void OnBnClickedBtnZup();

	HICON m_hIcon_On;
	HICON m_hIcon_Off;
	CStatic m_staicToolDetection;
	void UpdateInputStatus(int inport);
	void UpdateZPos(double dZPos);
	double m_dZCurPos;
	afx_msg void OnBnClickedSave();
	virtual BOOL DestroyWindow();
};

#pragma once

#include "CameraHK.h"
// CDlgDualValve 对话框

class CDlgDualValve : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDualValve)

public:
	CDlgDualValve(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDualValve();

// 对话框数据
	enum { IDD = IDD_DLG_DUAL_VALVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

	// 显示图像
	BYTE* m_pImageBuffer;	// 从摄像头获取的图像数据指针
	CCameraHK	m_camBottomHK;
	MIL_ID 
		MilDisplay,             /* Display identifier.     */
		MilOverlayImage,		/* OverlayImage identifier.*/
		MilImage;
	void PaintShape(CRect rect);
	int InitBottomCamera();
	LRESULT OnMessageRedrawCCDEvent(WPARAM wparam, LPARAM lparam);

	void UpdateCtrl(bool bShow);	// 更新界面控件	bShow=		true:值更新到界面	false:从界面读取到参数
	double m_dRotateMarkX1;
	double m_dRotateMarkX2;
	double m_dRotateMarkY1;
	double m_dRotateMarkY2;
	double m_dValveSpace;
	double m_dValvePosX1;
	double m_dValvePosY1;
	double m_dValvePosZ1;
	double m_dNeedleRad;	// 针头图像显示半径
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

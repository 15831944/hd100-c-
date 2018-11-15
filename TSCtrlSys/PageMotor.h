#pragma once

#include "resource.h"
#include "Grid/GridCtrl_src/GridCtrl.h"

// CPageMotor 对话框
#define MOTOR_GRID_HEIGHT	32
#define MOTOR_GRID_COL_NUM	16

class CPageMotor : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageMotor)

public:
	CPageMotor();
	virtual ~CPageMotor();

// 对话框数据
	enum { IDD = IDD_PAGE_MOTOR };

	CImageList m_ImageList;
	CGridCtrl m_MotorParamGrid;

protected:
	void CreateGrid();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSave();
};

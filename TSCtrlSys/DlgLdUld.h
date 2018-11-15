#pragma once
#include "Grid/GridCtrl_src/GridCtrl.h"
#include "afxwin.h"


#define LdUld_pos_col	2	// Gridctrl����
enum Ld_pos_index		// Gridctrl����ʾ�ĵ�λ��˳��
{
	LdUld_pos_null,			// ��,ʹ��ʼ���λ1
	LdUld_pos_st_1,			// �ϲ��Ϻ���ʼλ
	LdUld_pos_end_1,		// �ϲ��Ϻ��е�λ
	LdUld_pos_st_2,			// �в��Ϻ���ʼλ
	LdUld_pos_end_2,		// �в��Ϻ��е�λ
	LdUld_pos_st_3,			// �²��Ϻ���ʼλ
	LdUld_pos_end_3,		// �²��Ϻ��е�λ
	LdUld_pos_num			// ��Ҫ���õĵ�λ��Ŀ
};

static CString g_strLdUldName[] =	// Gridctrl����ʾ�ĵ�λ�����ƺͽ���
{
	"",
	"�ϲ��Ϻ���ʼλ",				
	"�ϲ��Ϻ��յ�λ",	
	"�в��Ϻ���ʼλ",				
	"�в��Ϻ��յ�λ",	
	"�²��Ϻ���ʼλ",				
	"�²��Ϻ��յ�λ"	
};
// CDlgLdUld �Ի���

class CDlgLdUld : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLdUld)

public:
	CDlgLdUld(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLdUld();

// �Ի�������
	enum { IDD = IDD_DLG_LD_ULD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	HICON m_hIcon_On;
	HICON m_hIcon_Off;
	CStatic m_static_status[6];

	CGridCtrl m_GridLdPos;
	CGridCtrl m_GridUldPos;

	void UpdateGrid();
	void CreateLdGrid();
	void CreateUldGrid();
// 	afx_msg void OnLdGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
// 	afx_msg void OnUldGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	void InitCtrl();
	void UpdataIOStatus(long exInput);	// ����״̬

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSave();
	CComboBox m_cmbLdPitch;
	afx_msg void OnBnClickedBtnLoad1();
	afx_msg void OnBnClickedBtnLoadOut();
	afx_msg void OnBnClickedBtnLoadBack();
	afx_msg void OnBnClickedBtnLu();
	afx_msg void OnBnClickedBtnLd();
	afx_msg void OnBnClickedBtnLboxU();
	afx_msg void OnBnClickedBtnLboxD();
	afx_msg void OnBnClickedBtnLmetU();
	afx_msg void OnBnClickedBtnLmetD();
	afx_msg void OnBnClickedBtnLmov();
	afx_msg void OnBnClickedBtnLset();
	CComboBox m_cmbUldPitch;
	afx_msg void OnBnClickedBtnUnload1();
	afx_msg void OnBnClickedBtnUnloadOut();
	afx_msg void OnBnClickedBtnUnloadBack();
	afx_msg void OnBnClickedBtnUlu();
	afx_msg void OnBnClickedBtnUld();
	afx_msg void OnBnClickedBtnUlboxU();
	afx_msg void OnBnClickedBtnUlboxD();
	afx_msg void OnBnClickedBtnUlmetU();
	afx_msg void OnBnClickedBtnUlmetD();
	afx_msg void OnBnClickedBtnUlmov();
	afx_msg void OnBnClickedBtnUlset();
	int m_nLdLayerCount;
	int m_nUldLayerCount;

	CButtonST m_btnLoadUp;
	CButtonST m_btnLoadDown;
	CButtonST m_btnLoadBoxUp;
	CButtonST m_btnLoadBoxDown;
	CButtonST m_btnLoadLayerUp;
	CButtonST m_btnLoadLayerDown;

	CButtonST m_btnUnloadUp;
	CButtonST m_btnUnloadDown;
	CButtonST m_btnUnloadBoxUp;
	CButtonST m_btnUnloadBoxDown;
	CButtonST m_btnUnloadLayerUp;
	CButtonST m_btnUnloadLayerDown;
	afx_msg void OnBnClickedBtnHome();
	CButtonST m_btnHome;
	CButtonST m_btnLMov;
	CButtonST m_btnLSet;
	CButtonST m_btnSave;
	CButtonST m_btnUlMov;
	CButtonST m_btnUlSet;
};

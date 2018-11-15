// DlgLdUld.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgLdUld.h"
#include "afxdialogex.h"


// CDlgLdUld �Ի���

IMPLEMENT_DYNAMIC(CDlgLdUld, CDialogEx)

CDlgLdUld::CDlgLdUld(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLdUld::IDD, pParent)
{

	m_nLdLayerCount = 0;
	m_nUldLayerCount = 0;
}

CDlgLdUld::~CDlgLdUld()
{
}

void CDlgLdUld::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_2,	m_static_status[0]);
	DDX_Control(pDX, IDC_STATIC_3,	m_static_status[1]);
	DDX_Control(pDX, IDC_STATIC_5,	m_static_status[2]);
	DDX_Control(pDX, IDC_STATIC_6,	m_static_status[3]);
	DDX_Control(pDX, IDC_STATIC_1,	m_static_status[4]);
	DDX_Control(pDX, IDC_STATIC_4,	m_static_status[5]);	

	DDX_Control(pDX, IDC_COMBO1, m_cmbLdPitch);
	DDX_Control(pDX, IDC_COMBO2, m_cmbUldPitch);

	DDX_Control(pDX, IDC_GRID_LD_POS, m_GridLdPos);
	DDX_Control(pDX, IDC_GRID_ULD_POS, m_GridUldPos);
	DDX_Text(pDX, IDC_EDIT_LD_LAYERCOUNT, m_nLdLayerCount);
	DDV_MinMaxInt(pDX, m_nLdLayerCount, 2, 1000);
	DDX_Text(pDX, IDC_EDIT_ULD_LAYERCOUNT, m_nUldLayerCount);
	DDV_MinMaxInt(pDX, m_nUldLayerCount, 2, 1000);

	DDX_Control(pDX, IDC_BTN_LU, m_btnLoadUp);		
	DDX_Control(pDX, IDC_BTN_LD, m_btnLoadDown);	
	DDX_Control(pDX, IDC_BTN_LBOX_U, m_btnLoadBoxUp);	
	DDX_Control(pDX, IDC_BTN_LBOX_D, m_btnLoadBoxDown);	
	DDX_Control(pDX, IDC_BTN_LMET_U, m_btnLoadLayerUp);	
	DDX_Control(pDX, IDC_BTN_LMET_D, m_btnLoadLayerDown);	

	DDX_Control(pDX, IDC_BTN_ULU, m_btnUnloadUp);	
	DDX_Control(pDX, IDC_BTN_ULD, m_btnUnloadDown);	
	DDX_Control(pDX, IDC_BTN_ULBOX_U, m_btnUnloadBoxUp);	
	DDX_Control(pDX, IDC_BTN_ULBOX_D, m_btnUnloadBoxDown);	
	DDX_Control(pDX, IDC_BTN_ULMET_U, m_btnUnloadLayerUp);	
	DDX_Control(pDX, IDC_BTN_ULMET_D, m_btnUnloadLayerDown);	
	DDX_Control(pDX, IDC_BTN_HOME, m_btnHome);
	DDX_Control(pDX, IDC_BTN_LMOV, m_btnLMov);
	DDX_Control(pDX, IDC_BTN_LSET, m_btnLSet);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_ULMOV, m_btnUlMov);
	DDX_Control(pDX, IDC_BTN_ULSET, m_btnUlSet);
}


BEGIN_MESSAGE_MAP(CDlgLdUld, CDialogEx)
// 	ON_NOTIFY(NM_CLICK, IDC_GRID_LD_POS, OnLdGridClick)
// 	ON_NOTIFY(NM_CLICK, IDC_GRID_ULD_POS, OnUldGridClick)

	ON_BN_CLICKED(IDC_BTN_LOAD1, &CDlgLdUld::OnBnClickedBtnLoad1)
	ON_BN_CLICKED(IDC_BTN_LOAD_OUT, &CDlgLdUld::OnBnClickedBtnLoadOut)
	ON_BN_CLICKED(IDC_BTN_LOAD_BACK, &CDlgLdUld::OnBnClickedBtnLoadBack)
	ON_BN_CLICKED(IDC_BTN_LU, &CDlgLdUld::OnBnClickedBtnLu)
	ON_BN_CLICKED(IDC_BTN_LD, &CDlgLdUld::OnBnClickedBtnLd)
	ON_BN_CLICKED(IDC_BTN_LBOX_U, &CDlgLdUld::OnBnClickedBtnLboxU)
	ON_BN_CLICKED(IDC_BTN_LBOX_D, &CDlgLdUld::OnBnClickedBtnLboxD)
	ON_BN_CLICKED(IDC_BTN_LMET_U, &CDlgLdUld::OnBnClickedBtnLmetU)
	ON_BN_CLICKED(IDC_BTN_LMET_D, &CDlgLdUld::OnBnClickedBtnLmetD)
	ON_BN_CLICKED(IDC_BTN_LMOV, &CDlgLdUld::OnBnClickedBtnLmov)
	ON_BN_CLICKED(IDC_BTN_LSET, &CDlgLdUld::OnBnClickedBtnLset)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgLdUld::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_UNLOAD1, &CDlgLdUld::OnBnClickedBtnUnload1)
	ON_BN_CLICKED(IDC_BTN_UNLOAD_OUT, &CDlgLdUld::OnBnClickedBtnUnloadOut)
	ON_BN_CLICKED(IDC_BTN_UNLOAD_BACK, &CDlgLdUld::OnBnClickedBtnUnloadBack)
	ON_BN_CLICKED(IDC_BTN_ULU, &CDlgLdUld::OnBnClickedBtnUlu)
	ON_BN_CLICKED(IDC_BTN_ULD, &CDlgLdUld::OnBnClickedBtnUld)
	ON_BN_CLICKED(IDC_BTN_ULBOX_U, &CDlgLdUld::OnBnClickedBtnUlboxU)
	ON_BN_CLICKED(IDC_BTN_ULBOX_D, &CDlgLdUld::OnBnClickedBtnUlboxD)
	ON_BN_CLICKED(IDC_BTN_ULMET_U, &CDlgLdUld::OnBnClickedBtnUlmetU)
	ON_BN_CLICKED(IDC_BTN_ULMET_D, &CDlgLdUld::OnBnClickedBtnUlmetD)
	ON_BN_CLICKED(IDC_BTN_ULMOV, &CDlgLdUld::OnBnClickedBtnUlmov)
	ON_BN_CLICKED(IDC_BTN_ULSET, &CDlgLdUld::OnBnClickedBtnUlset)
	ON_BN_CLICKED(IDC_BTN_HOME, &CDlgLdUld::OnBnClickedBtnHome)
END_MESSAGE_MAP()


// CDlgLdUld ��Ϣ�������
BOOL CDlgLdUld::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_hIcon_On = ::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_LED_ON));
	m_hIcon_Off= ::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_LED_OFF));

	CreateLdGrid();
	CreateUldGrid();

	InitCtrl();

	m_nLdLayerCount = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount;
	m_nUldLayerCount = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nUldLayerCount;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgLdUld::InitCtrl()
{
	for (int i=0; i<6; i++)
	{
		m_static_status[i].ModifyStyle(0, SS_ICON);
		m_static_status[i].SetIcon(m_hIcon_Off);
	}

	m_cmbLdPitch.ResetContent();
	m_cmbLdPitch.AddString("0.001");
	m_cmbLdPitch.AddString("0.01");
	m_cmbLdPitch.AddString("0.1");
	m_cmbLdPitch.AddString("1");
	m_cmbLdPitch.AddString("10");
	m_cmbLdPitch.AddString("50");
	m_cmbLdPitch.SetCurSel(3);

	m_cmbUldPitch.ResetContent();
	m_cmbUldPitch.AddString("0.001");
	m_cmbUldPitch.AddString("0.01");
	m_cmbUldPitch.AddString("0.1");
	m_cmbUldPitch.AddString("1");
	m_cmbUldPitch.AddString("10");
	m_cmbUldPitch.AddString("50");
	m_cmbUldPitch.SetCurSel(3);

	m_btnLoadUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnLoadUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);
	m_btnLoadBoxUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnLoadBoxUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);
	m_btnLoadLayerUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnLoadLayerUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);
	m_btnUnloadUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnUnloadUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);
	m_btnUnloadBoxUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnUnloadBoxUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);
	m_btnUnloadLayerUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnUnloadLayerUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);

	m_btnLoadDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnLoadDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnLoadBoxDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnLoadBoxDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnLoadLayerDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnLoadLayerDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnUnloadDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnUnloadDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnUnloadBoxDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnUnloadBoxDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnUnloadLayerDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnUnloadLayerDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnHome.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnLMov.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnLSet.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnSave.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnUlMov.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnUlSet.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnHome.SetIcon(IDI_ICON_HOME);
	m_btnLMov.SetIcon(IDI_ICON_MOVETO);
	m_btnLSet.SetIcon(IDI_ICON_SINGLE_ADJUST);
	m_btnSave.SetIcon(IDI_ICON_SAVE_S);
	m_btnUlMov.SetIcon(IDI_ICON_MOVETO);
	m_btnUlSet.SetIcon(IDI_ICON_SINGLE_ADJUST);
}


void CDlgLdUld::UpdataIOStatus(long exInput)	// ����״̬
{
	//////////////////////////////////////////////////////////////////////////
	// input
	for (int i=0; i<6; i++)
	{
		if ((exInput&(0x01<<(i+6)))==(0x01<<(i+6)))
			m_static_status[i].SetIcon(m_hIcon_Off);
		else
			m_static_status[i].SetIcon(m_hIcon_On);
	}
}

void CDlgLdUld::CreateLdGrid()
{
	m_GridLdPos.EnableDragAndDrop(FALSE);
	m_GridLdPos.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_GridLdPos.SetEditable(TRUE);
	m_GridLdPos.SetListMode(TRUE);
	m_GridLdPos.SetTrackFocusCell(FALSE);
	m_GridLdPos.SetFrameFocusCell(FALSE);		//ȥ������ѡʱ�ı߿���  
	m_GridLdPos.SetColumnCount(LdUld_pos_col);	//��ʼ����,��˳��ֱ�Ϊ����λ����,��ֵ
	m_GridLdPos.SetRowCount(LdUld_pos_num + 4);			//���ó�ʼ����������ʼ��ֻ��һ��

	m_GridLdPos.SetFixedRowCount(1);	
	m_GridLdPos.SetFixedBkColor(RGB(128,128,192));
	m_GridLdPos.SetGridLines(GVL_BOTH);
	CString l_strTitle[] = {"  ��  ��  ��  ��  ","   ��   ֵ   "};
	//���ͷ���
	for (int i=0; i<LdUld_pos_col; i++)
	{
		m_GridLdPos.SetGridItemText(0,i,l_strTitle[i]);
		m_GridLdPos.ExpandColumnsToFit();
	}
	m_GridLdPos.AutoSizeColumns();
	m_GridLdPos.ExpandLastColumn();

	m_GridLdPos.SetColumnResize(TRUE);
	m_GridLdPos.SetRowResize(FALSE);

	//�����и�
	for(UINT i=0;i<LdUld_pos_num+4;i++)
	{
		m_GridLdPos.SetRowHeight(i,28);	
	}

	//�������
	for (UINT i=1;i<LdUld_pos_num+4;i++)
	{
		for (UINT j=0;j<LdUld_pos_col;j++)
		{	
			m_GridLdPos.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j)
			{	
				m_GridLdPos.GetCell(i, j)->SetBackClr(RGB(128,192,128));
				m_GridLdPos.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
		}
	}	

	UpdateGrid();
}

void CDlgLdUld::CreateUldGrid()
{
	m_GridUldPos.EnableDragAndDrop(FALSE);
	m_GridUldPos.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_GridUldPos.SetEditable(TRUE);
	m_GridUldPos.SetListMode(TRUE);
	m_GridUldPos.SetTrackFocusCell(FALSE);
	m_GridUldPos.SetFrameFocusCell(FALSE);		//ȥ������ѡʱ�ı߿���  
	m_GridUldPos.SetColumnCount(LdUld_pos_col);	//��ʼ����,��˳��ֱ�Ϊ����λ����,��ֵ
	m_GridUldPos.SetRowCount(LdUld_pos_num + 2);			//���ó�ʼ����������ʼ��ֻ��һ��

	m_GridUldPos.SetFixedRowCount(1);	
	m_GridUldPos.SetFixedBkColor(RGB(128,128,192));
	m_GridUldPos.SetGridLines(GVL_BOTH);
	CString l_strTitle[] = {"  ��  ��  ��  ��  ","   ��   ֵ   "};
	//���ͷ���
	for (int i=0; i<LdUld_pos_col; i++)
	{
		m_GridUldPos.SetGridItemText(0,i,l_strTitle[i]);
		m_GridUldPos.ExpandColumnsToFit();
	}
	m_GridUldPos.AutoSizeColumns();
	m_GridUldPos.ExpandLastColumn();

	m_GridUldPos.SetColumnResize(TRUE);
	m_GridUldPos.SetRowResize(FALSE);

	//�����и�
	for(UINT i=0;i<LdUld_pos_num+2;i++)
	{
		m_GridUldPos.SetRowHeight(i,28);	
	}

	//�������
	for (UINT i=1;i<LdUld_pos_num+2;i++)
	{
		for (UINT j=0;j<LdUld_pos_col;j++)
		{	
			m_GridUldPos.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j)
			{	
				m_GridUldPos.GetCell(i, j)->SetBackClr(RGB(128,192,128));
				m_GridUldPos.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
		}
	}	

	UpdateGrid();
}

void CDlgLdUld::UpdateGrid()
{
	CString str;
	for(UINT i=1;i<LdUld_pos_num;i++)
	{
		m_GridLdPos.SetGridItemText(i, 0, g_strLdUldName[i]);
		m_GridUldPos.SetGridItemText(i, 0, g_strLdUldName[i]);

		str.Format("%.3f", g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[i-1]);
		m_GridLdPos.SetGridItemText(i, 1, str);
		str.Format("%.3f", g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[i-1]);
		m_GridUldPos.SetGridItemText(i, 1, str);
	}

	{
		m_GridLdPos.SetGridItemText(LdUld_pos_num+1, 0, _T("�ظ����ϴ���"));
		str.Format("%d", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdRePushout);
		m_GridLdPos.SetGridItemText(LdUld_pos_num+1, 1, str);
		m_GridLdPos.SetGridItemText(LdUld_pos_num+2, 0, _T("���ϱ�������"));
		str.Format("%d", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdEmptyWarning);
		m_GridLdPos.SetGridItemText(LdUld_pos_num+2, 1, str);
		m_GridLdPos.SetGridItemText(LdUld_pos_num+3, 0, _T("ѭ������(0~1)"));
		str.Format("%d", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdContinue);
		m_GridLdPos.SetGridItemText(LdUld_pos_num+3, 1, str);
	}

	{
		m_GridUldPos.SetGridItemText(LdUld_pos_num+1, 0, _T("�Ƴ�����ʱ(s)"));
		str.Format("%.3f", g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay);
		m_GridUldPos.SetGridItemText(LdUld_pos_num+1, 1, str);
	}
	EnableWindow(1 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode);
}

// void CDlgLdUld::OnLdGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
// {
// 
// }
// 
// void CDlgLdUld::OnUldGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
// {
// 
// }


void CDlgLdUld::OnBnClickedBtnLoad1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bStatus = TRUE;
	g_pFrm->m_Robot->ExInput("13�Ź⿪", bStatus);
	if (bStatus)
	{
		AfxMessageBox("�㽺��λ�а�,ȡ�ߺ��ٲ��Խ��϶�����");
		return ;
	}

	short iRtn = 0;
	iRtn = g_pFrm->m_Robot->ExOutput("12���赲���׵�ŷ�", TRUE);
	// ����Ƥ��	
	g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
	
	// �����Ƴ�
	BOOL bBdDetect;
	iRtn = g_pFrm->m_Robot->LoadUnLoad_loadPushOut(bBdDetect);
	if (RTN_NONE != iRtn)
	{
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
		iRtn = g_pFrm->m_Robot->LoadUnLoad_loadPullBack();
		AfxMessageBox("�����Ƴ�����!");
		return;
	}
	iRtn = g_pFrm->m_Robot->LoadUnLoad_loadPullBack();
	
	if (!bBdDetect)
	{
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
		AfxMessageBox("���δ��⵽����");
		return;
	}

	CKFTimer tmTemp;
	tmTemp.TimerStart();
	while (!bStatus)
	{
		if (tmTemp.TimerGetSecond() > 4)
		{
			AfxMessageBox("���ϵ��㽺λ��ʱ!");
			g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
			return;
		}

		g_pFrm->m_Robot->ExInput("13�Ź⿪", bStatus);
		Sleep(20);
	}

	g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
	CFunction::DelaySec(0.2);

	// �������
	g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);

	g_pFrm->m_Robot->LoadUnLoad_loadLayerMove(g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId+1);
}


void CDlgLdUld::OnBnClickedBtnLoadOut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bBdDetect;
	g_pFrm->m_Robot->LoadUnLoad_loadPushOut(bBdDetect);
}


void CDlgLdUld::OnBnClickedBtnLoadBack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pFrm->m_Robot->LoadUnLoad_loadPullBack();
}


void CDlgLdUld::OnBnClickedBtnLu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;

	g_pFrm->m_Precision.GetCurPosMM();
	CString str;
	m_cmbLdPitch.GetWindowText(str);
	double dStepDist = atof(str);
	dStepDist = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[LOAD_AXIS]+dStepDist;
	short rtn = g_pFrm->m_Robot->m_pController->AxisMove(LOAD_AXIS,dStepDist,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Acc, TRUE);

	if (RTN_NONE != rtn)
	{
		AfxMessageBox("���ϵ���ƶ�����!");
	}
}


void CDlgLdUld::OnBnClickedBtnLd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;

	g_pFrm->m_Precision.GetCurPosMM();
	CString str;
	m_cmbLdPitch.GetWindowText(str);
	double dStepDist = atof(str);
	dStepDist = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[LOAD_AXIS]-dStepDist;
	short rtn = g_pFrm->m_Robot->m_pController->AxisMove(LOAD_AXIS,dStepDist,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Acc, TRUE);

	if (RTN_NONE != rtn)
	{
		AfxMessageBox("���ϵ���ƶ�����!");
	}
}


void CDlgLdUld::OnBnClickedBtnLboxU()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;
	int nCrtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId;
	g_pFrm->m_Robot->LoadUnLoad_loadBoxMove(nCrtBox+1);
}


void CDlgLdUld::OnBnClickedBtnLboxD()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;
	int nCrtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId;
	g_pFrm->m_Robot->LoadUnLoad_loadBoxMove(nCrtBox-1);
}


void CDlgLdUld::OnBnClickedBtnLmetU()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;
	int nCrtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId;
	g_pFrm->m_Robot->LoadUnLoad_loadLayerMove(nCrtLayer+1);
}


void CDlgLdUld::OnBnClickedBtnLmetD()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;
	int nCrtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId;
	g_pFrm->m_Robot->LoadUnLoad_loadLayerMove(nCrtLayer-1);
}


void CDlgLdUld::OnBnClickedBtnLmov()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;

	int row_click = -1;
	row_click = m_GridLdPos.GetFocusCell().row;
	if (row_click<1 || row_click>LdUld_pos_num)	return;

	CString strTemp;
	double ldPos=0;

	ldPos = g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[row_click-1];
	short rtn = g_pFrm->m_Robot->m_pController->AxisMove(LOAD_AXIS, ldPos,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Acc, TRUE);
	if (RTN_NONE != rtn)
	{
		AfxMessageBox("���ϵ���ƶ�����!");
		return ;
	}

	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId = (row_click-1)/ 2;
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId = (row_click&0x01) ? 0 : g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount-1;
}


void CDlgLdUld::OnBnClickedBtnLset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (IDOK != AfxMessageBox("�Ƿ���µ�λ!", MB_OKCANCEL))return;

	int row_click = -1;
	row_click = m_GridLdPos.GetFocusCell().row;
	if (row_click<1 || row_click>LdUld_pos_num)	return;

	g_pFrm->m_Precision.GetCurPosMM();
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[row_click-1] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[LOAD_AXIS];
	UpdateGrid();
}

void CDlgLdUld::OnBnClickedBtnSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount = GetDlgItemInt(IDC_EDIT_LD_LAYERCOUNT);
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nUldLayerCount = GetDlgItemInt(IDC_EDIT_ULD_LAYERCOUNT);

	for (int i=1; i<LdUld_pos_num; i++)
	{
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[i-1] = m_GridLdPos.GetItemDataDouble(i, 1);
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[i-1] = m_GridUldPos.GetItemDataDouble(i, 1);
	}

	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdRePushout = m_GridLdPos.GetItemDataInt(LdUld_pos_num+1, 1);
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdEmptyWarning = m_GridLdPos.GetItemDataInt(LdUld_pos_num+2, 1);
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdContinue = m_GridLdPos.GetItemDataInt(LdUld_pos_num+3, 1);

	g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay = m_GridUldPos.GetItemDataDouble(LdUld_pos_num+1, 1);
	g_pFrm->RobotParam(FALSE);
}


void CDlgLdUld::OnBnClickedBtnUnload1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bStatus1;
	BOOL bStatus2;
	BOOL bStatus = FALSE;
	short iRet = 0;
	g_pFrm->m_Robot->ExInput("13�Ź⿪", bStatus1);
	g_pFrm->m_Robot->ExInput("14�Ź⿪", bStatus2);

	if (!bStatus1 && !bStatus2)
	{
		return;
	}

	// �赲����
	g_pFrm->m_Robot->ExOutput("12���赲���׵�ŷ�", FALSE);
	// ����Ƥ��	
	g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

	CKFTimer tmTemp;
	tmTemp.TimerStart();
	while (!bStatus2)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
			g_pFrm->m_Robot->AfxMessageBox("���Ͽ���,�����ڼ��⿪��ʱ!");
			return;
		}

		CFunction::DelaySec(0.02);
		g_pFrm->m_Robot->ExInput("14�Ź⿪", bStatus2);
	}
	g_pFrm->m_Robot->ExInput("14�Ź⿪", bStatus2);
	tmTemp.TimerStart();
	while (bStatus2)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
			g_pFrm->m_Robot->AfxMessageBox("���Ͽڿ���!���ֶ�����");
		}

		iRet = g_pFrm->m_Robot->ExInput("14�Ź⿪", bStatus2);
		CFunction::DelaySec(0.02);
	}

	CFunction::DelaySec(g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay);

	//////////////////////////////////////////////////////////////////////////
	iRet = g_pFrm->m_Robot->ExOutput("2���Ƴ������׵�ŷ�",TRUE);
	if (iRet != RTN_NONE)	
	{
		g_pFrm->m_Robot->AfxMessageBox("���׶�������!");
	}

	iRet = g_pFrm->m_Robot->ExInput("�Ƴ�������-R", bStatus);
	tmTemp.TimerStart();
	while (!bStatus)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			if (IDOK == AfxMessageBox("�Ƴ�������2 ��λ��ʱ", MB_OKCANCEL))
			{
			}
		}

		iRet = g_pFrm->m_Robot->ExInput("�Ƴ�������-R", bStatus);
		CFunction::DelaySec(0.02);
	}

	iRet = g_pFrm->m_Robot->ExOutput("1���Ƴ������׵�ŷ�",TRUE);
	iRet = g_pFrm->m_Robot->ExInput("�Ƴ�������-L", bStatus);
	tmTemp.TimerStart();
	while (!bStatus)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			AfxMessageBox("�Ƴ�������1 ��λ��ʱ");
		}

		iRet = g_pFrm->m_Robot->ExInput("�Ƴ�������-L", bStatus);
		CFunction::DelaySec(0.02);
	}
	// �����˻�
	g_pFrm->m_Robot->ExOutput("1���Ƴ������׵�ŷ�", FALSE);
	g_pFrm->m_Robot->ExOutput("2���Ƴ������׵�ŷ�", FALSE);

	g_pFrm->m_Robot->ExInput("14�Ź⿪", bStatus);
	if (bStatus)
	{
		AfxMessageBox("���ڿ���,�������ڹ⿪��⵽������");
	}

	g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
	g_pFrm->m_Robot->LoadUnLoad_unloadLayerMove(g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId+1);
}


void CDlgLdUld::OnBnClickedBtnUnloadOut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pFrm->m_Robot->LoadUnLoad_unloadPushOut();
}


void CDlgLdUld::OnBnClickedBtnUnloadBack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pFrm->m_Robot->LoadUnLoad_unloadPullBack();
}


void CDlgLdUld::OnBnClickedBtnUlu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())			return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)			return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;

	g_pFrm->m_Precision.GetCurPosMM();
	CString str;
	m_cmbUldPitch.GetWindowText(str);
	double dStepDist = atof(str);
	dStepDist = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[UNLOAD_AXIS]+dStepDist;
	short rtn = g_pFrm->m_Robot->m_pController->AxisMove(UNLOAD_AXIS,dStepDist,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Acc, TRUE);

	if (RTN_NONE != rtn)
	{
		AfxMessageBox("���ϵ���ƶ�����!");
	}
}


void CDlgLdUld::OnBnClickedBtnUld()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;

	g_pFrm->m_Precision.GetCurPosMM();
	CString str;
	m_cmbUldPitch.GetWindowText(str);
	double dStepDist = atof(str);
	dStepDist = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[UNLOAD_AXIS]-dStepDist;
	short rtn = g_pFrm->m_Robot->m_pController->AxisMove(UNLOAD_AXIS,dStepDist,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Acc, TRUE);

	if (RTN_NONE != rtn)
	{
		AfxMessageBox("���ϵ���ƶ�����!");
	}
}


void CDlgLdUld::OnBnClickedBtnUlboxU()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;
	int nCrtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;
	g_pFrm->m_Robot->LoadUnLoad_unloadBoxMove(nCrtBox+1);
}


void CDlgLdUld::OnBnClickedBtnUlboxD()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;
	int nCrtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;
	g_pFrm->m_Robot->LoadUnLoad_unloadBoxMove(nCrtBox-1);
}


void CDlgLdUld::OnBnClickedBtnUlmetU()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;
	int nCrtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId;
	g_pFrm->m_Robot->LoadUnLoad_unloadLayerMove(nCrtLayer+1);
}


void CDlgLdUld::OnBnClickedBtnUlmetD()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;
	int nCrtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId;
	g_pFrm->m_Robot->LoadUnLoad_unloadLayerMove(nCrtLayer-1);
}


void CDlgLdUld::OnBnClickedBtnUlmov()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())return;

	int row_click = -1;
	row_click = m_GridUldPos.GetFocusCell().row;
	if (row_click<1 || row_click>LdUld_pos_num+1)	return;

	CString strTemp;
	double ldPos=0;

	if (row_click <= LdUld_pos_num)
	{
		ldPos = g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[row_click-1];
		short rtn = g_pFrm->m_Robot->m_pController->AxisMove(UNLOAD_AXIS, ldPos,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Acc, TRUE);
		if (RTN_NONE != rtn)
		{
			AfxMessageBox("���ϵ���ƶ�����!");
		}
	}
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId = (row_click-1)/ 2;
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId = (row_click&0x01) ? 0 : g_pFrm->m_pRobotParam->m_structLdUldParam.m_nUldLayerCount-1;
}


void CDlgLdUld::OnBnClickedBtnUlset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (IDOK != AfxMessageBox("�Ƿ���µ�λ!", MB_OKCANCEL))return;

	int row_click = -1;
	row_click = m_GridUldPos.GetFocusCell().row;
	if (row_click<1 || row_click>LdUld_pos_num+1)	return;

	if (row_click <= LdUld_pos_num)
	{
		g_pFrm->m_Precision.GetCurPosMM();
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[row_click-1] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[UNLOAD_AXIS];
	}
	else
	{		
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay = m_GridUldPos.GetItemDataDouble(LdUld_pos_num+1, 1);
	}
	UpdateGrid();
}


void CDlgLdUld::OnBnClickedBtnHome()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_pFrm->m_Robot->m_pController->IsInitOk()==FALSE) 		return;

	g_pFrm->m_Robot->LoadUnLoad_Home(0);
	g_pFrm->m_Robot->LoadUnLoad_Home(1);
// 	g_pFrm->m_Robot->m_pController->AxisHome(LOAD_AXIS, g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vhome, g_pFrm->m_mtrParamGts[LOAD_AXIS].m_VhomeLow,
// 										 g_pFrm->m_mtrParamGts[LOAD_AXIS].m_AccHome,  g_pFrm->m_mtrParamGts[LOAD_AXIS].m_HomeReturn);
// 	g_pFrm->m_Robot->m_pController->AxisHome(UNLOAD_AXIS, g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vhome, g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_VhomeLow,
// 		g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_AccHome,  g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_HomeReturn);

	g_pFrm->m_pRobotParam->m_structLdUldParam.Init();
}

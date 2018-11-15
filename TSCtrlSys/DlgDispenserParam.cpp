// DlgDispenserParam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgDispenserParam.h"
#include "afxdialogex.h"


// CDlgDispenserParam �Ի���

IMPLEMENT_DYNAMIC(CDlgDispenserParam, CDialogEx)

CDlgDispenserParam::CDlgDispenserParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDispenserParam::IDD, pParent)
{

	m_nWeightType = 0;
}

CDlgDispenserParam::~CDlgDispenserParam()
{
}

void CDlgDispenserParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCR_RISING, m_scrollRising);
	DDX_Control(pDX, IDC_SCR_OPEN, m_scrollOpen);
	DDX_Control(pDX, IDC_SCR_LIFT, m_scrollLift);
	DDX_Control(pDX, IDC_SCR_DOWN, m_scrollFalling);
	DDX_Control(pDX, IDC_SCR_DELAY, m_scrollDelay);
	DDX_Control(pDX, IDC_SCR_PUL, m_scrollPulse);
	DDX_Control(pDX, IDC_SCR_TEMP, m_scrollTemp);
	DDX_Control(pDX, IDC_COMBO_DISP_SEL, m_comboDispIdx);
	DDX_Control(pDX, IDC_COMBO_DISP_TYPE, m_comboDispType);
	DDX_Control(pDX, IDC_COMBO_DISP_CTRL, m_comboDispCtrl);
	DDX_Radio(pDX, IDC_RADIO_T, m_nWeightType);
}


BEGIN_MESSAGE_MAP(CDlgDispenserParam, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOAD_FROM_DISP, &CDlgDispenserParam::OnBnClickedBtnLoadFromDisp)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_LOAD_FROM_FILE, &CDlgDispenserParam::OnBnClickedBtnLoadFromFile)
	ON_BN_CLICKED(IDC_BTN_SET_PARAM, &CDlgDispenserParam::OnBnClickedBtnSetParam)
//	ON_BN_CLICKED(IDC_BTN_SET_WTPOS, &CDlgDispenserParam::OnBnClickedBtnSetWtpos)
	ON_BN_CLICKED(IDC_BTN_READ_PRESSURE, &CDlgDispenserParam::OnBnClickedBtnReadPressure)
//	ON_BN_CLICKED(IDC_BTN_MVTO_WTPOS, &CDlgDispenserParam::OnBnClickedBtnMvtoWtpos)
	ON_BN_CLICKED(IDC_BTN_WT_ADJ, &CDlgDispenserParam::OnBnClickedBtnWtAdj)
	ON_BN_CLICKED(IDC_BTN_VALVE_ON, &CDlgDispenserParam::OnBnClickedBtnValveOn)
	ON_CBN_SELCHANGE(IDC_COMBO_DISP_TYPE, &CDlgDispenserParam::OnCbnSelchangeComboDispType)
	ON_CBN_SELCHANGE(IDC_COMBO_DISP_SEL, &CDlgDispenserParam::OnCbnSelchangeComboDispSel)
	ON_BN_CLICKED(IDC_BTN_SET_PRESSURE, &CDlgDispenserParam::OnBnClickedBtnSetPressure)
	ON_BN_CLICKED(IDC_CHK_WEIGHT, &CDlgDispenserParam::OnBnClickedChkWeight)
	ON_BN_CLICKED(IDC_RADIO_T, &CDlgDispenserParam::OnBnClickedRadioT)
	ON_BN_CLICKED(IDC_RADIO_P, &CDlgDispenserParam::OnBnClickedRadioP)
	ON_CBN_SELCHANGE(IDC_COMBO_DISP_CTRL, &CDlgDispenserParam::OnCbnSelchangeComboDispCtrl)
END_MESSAGE_MAP()


// CDlgDispenserParam ��Ϣ�������
BOOL CDlgDispenserParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_dispParam = g_pFrm->m_comDisp[0].m_dispParam;
	InitScrollBar();

	m_comboDispIdx.Clear();
	m_comboDispIdx.AddString("�㽺��1");
	m_comboDispIdx.AddString("�㽺��2");
	m_comboDispIdx.SetCurSel(0);

	m_comboDispType.Clear();
	m_comboDispType.AddString(Valve_Vemers);
	m_comboDispType.AddString(Valve_Kaifa);
	m_comboDispType.AddString(Valve_Musashi);
	m_comboDispType.AddString(Valve_Yikai);
	m_comboDispType.SetCurSel(g_pFrm->m_pRobotParam->m_nValveType);
	
	m_comboDispCtrl.Clear();
//	m_comboDispCtrl.AddString(ValveCtrl_AxisPul);
	m_comboDispCtrl.AddString(ValveCtrl_IO);
	m_comboDispCtrl.AddString(ValveCtrl_CompareIO);
	m_comboDispCtrl.SetCurSel(g_pFrm->m_pRobotParam->m_nValveCtrlMode);

	OnBnClickedBtnLoadFromDisp();
	UpdataCtrlVal(true);

	// ���ñ�ѹ
	double dPressVal = g_pFrm->m_pRobotParam->m_dBackPress;
	g_pFrm->m_Robot->SetBackPressure(dPressVal);

	// �򿪱�ѹ��ŷ�
	short iRet = g_pFrm->m_Robot->ExOutput("1�ű�ѹ��ŷ�",TRUE);
	if (iRet!=RTN_NONE)	
	{
		MessageBox("�򿪱�ѹ����!!!");
	}

	CFont m_Font1;
	m_Font1.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,
		CLIP_STROKE_PRECIS,DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("UnidreamLED"));
//	m_edit_wtPos_x.SubclassDlgItem(IDC_EDIT_WTPOS_X, this);
	//m_edit_wtPos_x.SetFont(&m_Font1);

	// �Զ���������
	CString strTemp;
	CButton* pBtn;
	pBtn = (CButton*)GetDlgItem(IDC_CHK_WEIGHT);
	pBtn->SetCheck(g_pFrm->m_pRobotParam->m_nIfAutoWeight);
	GetDlgItem(IDC_RADIO_T)->EnableWindow(g_pFrm->m_pRobotParam->m_nIfAutoWeight);
	GetDlgItem(IDC_RADIO_P)->EnableWindow(g_pFrm->m_pRobotParam->m_nIfAutoWeight);
	m_nWeightType = g_pFrm->m_pRobotParam->m_nWeightIntervalType;
	GetDlgItem(IDC_EDIT_WEIGHT_INTERVAL_VALUE)->EnableWindow(g_pFrm->m_pRobotParam->m_nIfAutoWeight);
	strTemp.Format("%d", g_pFrm->m_pRobotParam->m_nWeightIntervalValue);
	SetDlgItemText(IDC_EDIT_WEIGHT_INTERVAL_VALUE, strTemp);

	SetDlgItemText(IDC_EDIT_OPEN_DOTNUM, _T("0"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgDispenserParam::UpdataCtrlVal(bool bIfShow)
{
	CString strTemp;
	if (bIfShow)
	{
// 		strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_posWeightAdj.x);
// 		SetDlgItemText(IDC_EDIT_WTPOS_X, strTemp);
// 		strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_posWeightAdj.y);
// 		SetDlgItemText(IDC_EDIT_WTPOS_Y, strTemp);
// 		strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_posWeightAdj.z);
// 		SetDlgItemText(IDC_EDIT_WTPOS_Z, strTemp);
		
		// ���ص���
		strTemp.Format("%d", g_pFrm->m_pRobotParam->m_nWeightAdjDotNum);
		SetDlgItemText(IDC_EDIT_WT_NUM, strTemp);
		// ���㽺��
		strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_dAvgWeight);
		SetDlgItemText(IDC_EDIT_DEF_GLUE_WT, strTemp);
		// ��Ͱ��ѹ
		strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_dBackPress);
		SetDlgItemText(IDC_EDIT_PRESSURE, strTemp);
	}
}

void CDlgDispenserParam::OnBnClickedBtnLoadFromDisp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nDispIdx = m_comboDispIdx.GetCurSel();
	int nDispType = m_comboDispType.GetCurSel();
	g_pFrm->m_comDisp[nDispIdx].SetDispenserType((DispenserType)nDispType);

	if (1 == g_pFrm->m_comDisp[nDispIdx].HandleParamDispenser(TRUE))
	{
		m_dispParam = g_pFrm->m_comDisp[nDispIdx].m_dispParam;
		InitScrollBar();
	}
}

void CDlgDispenserParam::InitScrollBar()
{
	InitStaticCtrl((int)g_pFrm->m_comDisp[0].m_eDispType);
	m_scrollRising.SetScrollRange(1, 100);
	m_scrollOpen.SetScrollRange(1, 150);
	m_scrollLift.SetScrollRange(1, 100);
	m_scrollFalling.SetScrollRange(1, 100);
	m_scrollDelay.SetScrollRange(1, 1000);
	m_scrollPulse.SetScrollRange(1, 1000);
	m_scrollTemp.SetScrollRange(1, 100);

	m_scrollRising.SetScrollPos(m_dispParam.Rising);
	m_scrollOpen.SetScrollPos(m_dispParam.OpenTime);
	m_scrollLift.SetScrollPos(m_dispParam.NeedleLift);
	m_scrollFalling.SetScrollPos(m_dispParam.Falling);
	m_scrollDelay.SetScrollPos(m_dispParam.Delay);
	m_scrollPulse.SetScrollPos(m_dispParam.NumOfPul);
	m_scrollTemp.SetScrollPos(m_dispParam.Temp);

	CString strTemp;
	if (Disp_Yikai == g_pFrm->m_comDisp[0].m_eDispType)
	{
		strTemp.Format("%.02f", (float)m_dispParam.Rising/100);
		SetDlgItemText(IDC_EDIT_RISINGTIME, strTemp);
		strTemp.Format("%.02f", (float)m_dispParam.OpenTime/100);
		SetDlgItemText(IDC_EDIT_OPEN, strTemp);
		strTemp.Format("%.02f", (float)m_dispParam.Falling/100);
		SetDlgItemText(IDC_EDIT_DOWN, strTemp);
		strTemp.Format("%.02f", (float)m_dispParam.Delay/100);
		SetDlgItemText(IDC_EDIT_DELAY, strTemp);
		strTemp.Format("%d", m_dispParam.NeedleLift);
		SetDlgItemText(IDC_EDIT_LIFT, strTemp);
		strTemp.Format("%d", m_dispParam.NumOfPul);
		SetDlgItemText(IDC_EDIT_PULSE, strTemp);
		strTemp.Format("%d", m_dispParam.Temp);
		SetDlgItemText(IDC_EDIT_TEMP, strTemp);

		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC11)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC14)->ShowWindow(SW_SHOW);

		m_scrollOpen.ShowWindow(SW_SHOW);
		m_scrollLift.ShowWindow(SW_SHOW);
		m_scrollDelay.ShowWindow(SW_SHOW);
		m_scrollTemp.ShowWindow(SW_SHOW);
	}
	else if (Disp_Vermes == g_pFrm->m_comDisp[0].m_eDispType)
	{
		strTemp.Format("%.02f", (float)m_dispParam.Rising/10);
		SetDlgItemText(IDC_EDIT_RISINGTIME, strTemp);
		strTemp.Format("%.02f", (float)m_dispParam.OpenTime/10);
		SetDlgItemText(IDC_EDIT_OPEN, strTemp);
		strTemp.Format("%.02f", (float)m_dispParam.Falling/100);
		SetDlgItemText(IDC_EDIT_DOWN, strTemp);
		strTemp.Format("%.02f", (float)m_dispParam.Delay/10);
		SetDlgItemText(IDC_EDIT_DELAY, strTemp);
		strTemp.Format("%d", m_dispParam.NeedleLift);
		SetDlgItemText(IDC_EDIT_LIFT, strTemp);
		strTemp.Format("%d", m_dispParam.NumOfPul);
		SetDlgItemText(IDC_EDIT_PULSE, strTemp);
		strTemp.Format("%d", m_dispParam.Temp);
		SetDlgItemText(IDC_EDIT_TEMP, strTemp);

		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC11)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC14)->ShowWindow(SW_SHOW);
		
		m_scrollOpen.ShowWindow(SW_SHOW);
		m_scrollLift.ShowWindow(SW_SHOW);
		m_scrollDelay.ShowWindow(SW_SHOW);
		m_scrollTemp.ShowWindow(SW_SHOW);
	}
	else if (Disp_Kaifa == g_pFrm->m_comDisp[0].m_eDispType)
	{
		strTemp.Format("%.02f", (float)m_dispParam.Rising/10);
		SetDlgItemText(IDC_EDIT_RISINGTIME, strTemp);
		strTemp.Format("%.02f", (float)m_dispParam.Falling/10);
		SetDlgItemText(IDC_EDIT_DOWN, strTemp);
		strTemp.Format("%d", m_dispParam.NumOfPul);
		SetDlgItemText(IDC_EDIT_PULSE, strTemp);

		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC14)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_LIFT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TEMP)->ShowWindow(SW_HIDE);
		m_scrollOpen.ShowWindow(SW_HIDE);
		m_scrollLift.ShowWindow(SW_HIDE);
		m_scrollDelay.ShowWindow(SW_HIDE);
		m_scrollTemp.ShowWindow(SW_HIDE);
	}
	else if (Disp_Musashi == g_pFrm->m_comDisp[0].m_eDispType)
	{
		m_scrollRising.SetScrollRange(1, 1000);
		m_scrollLift.SetScrollRange(1, 100);
		m_scrollFalling.SetScrollRange(1, 1000);
		m_scrollPulse.SetScrollRange(1, 1000);

		m_scrollRising.SetScrollPos(m_dispParam.Rising);
		m_scrollLift.SetScrollPos(m_dispParam.NeedleLift);
		m_scrollFalling.SetScrollPos(m_dispParam.Falling);
		m_scrollPulse.SetScrollPos(m_dispParam.NumOfPul);

		strTemp.Format("%.02f", (float)m_dispParam.Rising/100);
		SetDlgItemText(IDC_EDIT_RISINGTIME, strTemp);
		strTemp.Format("%.02f", (float)m_dispParam.Falling/100);
		SetDlgItemText(IDC_EDIT_DOWN, strTemp);
		strTemp.Format("%d", m_dispParam.NumOfPul);
		SetDlgItemText(IDC_EDIT_PULSE, strTemp);
		strTemp.Format("%d", m_dispParam.NeedleLift);
		SetDlgItemText(IDC_EDIT_LIFT, strTemp);

		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC14)->ShowWindow(SW_HIDE);

		m_scrollOpen.ShowWindow(SW_HIDE);
		m_scrollLift.ShowWindow(SW_SHOW);
		m_scrollDelay.ShowWindow(SW_HIDE);
		m_scrollTemp.ShowWindow(SW_HIDE);
	}
}

void CDlgDispenserParam::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int iTmpPos = 0;
	CString strTemp;
		 
	switch (nSBCode)
	{
	case SB_THUMBPOSITION:
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SCR_RISING:
			iTmpPos = nPos;
			m_scrollRising.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_RISINGTIME, strTemp);
			break;

		case IDC_SCR_OPEN:
			iTmpPos = nPos;
			m_scrollOpen.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_OPEN, strTemp);
			break;

		case IDC_SCR_DOWN:
			iTmpPos = nPos;
			m_scrollFalling.SetScrollPos(iTmpPos);
			if (Disp_Vermes == g_pFrm->m_comDisp[0].m_eDispType)
			{
				strTemp.Format("%.02f", (float)iTmpPos/100);
			}
			else if (Disp_Kaifa == g_pFrm->m_comDisp[0].m_eDispType)
			{
				strTemp.Format("%.02f", (float)iTmpPos/10);
			}
			SetDlgItemText(IDC_EDIT_DOWN, strTemp);
			break;

		case IDC_SCR_DELAY:
			iTmpPos = nPos;
			m_scrollDelay.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_DELAY, strTemp);
			break;

		case IDC_SCR_LIFT:
			iTmpPos = nPos;
			m_scrollLift.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_LIFT, strTemp);
			break;

		case IDC_SCR_PUL:
			iTmpPos = nPos;
			m_scrollPulse.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_PULSE, strTemp);
			break;

		case IDC_SCR_TEMP:
			iTmpPos = nPos;
			m_scrollTemp.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_TEMP, strTemp);
			break;
		}
		break;

	case SB_LINERIGHT:
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SCR_RISING:
			iTmpPos = m_scrollRising.GetScrollPos();
			if (iTmpPos < 100)
			{
				iTmpPos++;
			}
			m_scrollRising.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_RISINGTIME, strTemp);
			break;

		case IDC_SCR_OPEN:
			iTmpPos = m_scrollOpen.GetScrollPos();
			if (iTmpPos < 150)
			{
				iTmpPos++;
			}
			m_scrollOpen.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_OPEN, strTemp);
			break;

		case IDC_SCR_DOWN:
			iTmpPos = m_scrollFalling.GetScrollPos();
			if (iTmpPos < 100)
			{
				iTmpPos++;
			}
			m_scrollFalling.SetScrollPos(iTmpPos);		
			if (Disp_Vermes == g_pFrm->m_comDisp[0].m_eDispType)
			{
				strTemp.Format("%.02f", (float)iTmpPos/100);
			}
			else if (Disp_Kaifa == g_pFrm->m_comDisp[0].m_eDispType)
			{
				strTemp.Format("%.02f", (float)iTmpPos/10);
			}
			SetDlgItemText(IDC_EDIT_DOWN, strTemp);
			break;

		case IDC_SCR_DELAY:
			iTmpPos = m_scrollDelay.GetScrollPos();
			if (iTmpPos < 1000)
			{
				iTmpPos++;
			}
			m_scrollDelay.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_DELAY, strTemp);
			break;

		case IDC_SCR_LIFT:
			iTmpPos = m_scrollLift.GetScrollPos();
			if (iTmpPos < 100)
			{
				iTmpPos++;
			}
			m_scrollLift.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_LIFT, strTemp);
			break;

		case IDC_SCR_PUL:
			iTmpPos = m_scrollPulse.GetScrollPos();
			if (iTmpPos < 1000)
			{
				iTmpPos++;
			}
			m_scrollPulse.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_PULSE, strTemp);
			break;

		case IDC_SCR_TEMP:
			iTmpPos = m_scrollTemp.GetScrollPos();
			if (iTmpPos < 100)
			{
				iTmpPos++;
			}
			m_scrollTemp.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_TEMP, strTemp);
			break;
		}
		break;

	case SB_LINELEFT:
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SCR_RISING:
			iTmpPos = m_scrollRising.GetScrollPos();
			if (iTmpPos > 0)
			{
				iTmpPos--;
			}
			m_scrollRising.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_RISINGTIME, strTemp);
			break;

		case IDC_SCR_OPEN:
			iTmpPos = m_scrollOpen.GetScrollPos();
			if (iTmpPos > 0)
			{
				iTmpPos--;
			}
			m_scrollOpen.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_OPEN, strTemp);
			break;

		case IDC_SCR_DOWN:
			iTmpPos = m_scrollFalling.GetScrollPos();
			if (iTmpPos > 0)
			{
				iTmpPos--;
			}
			m_scrollFalling.SetScrollPos(iTmpPos);
			if (Disp_Vermes == g_pFrm->m_comDisp[0].m_eDispType)
			{
				strTemp.Format("%.02f", (float)iTmpPos/100);
			}
			else if (Disp_Kaifa == g_pFrm->m_comDisp[0].m_eDispType)
			{
				strTemp.Format("%.02f", (float)iTmpPos/10);
			}
			SetDlgItemText(IDC_EDIT_DOWN, strTemp);
			break;

		case IDC_SCR_DELAY:
			iTmpPos = m_scrollDelay.GetScrollPos();
			if (iTmpPos > 0)
			{
				iTmpPos--;
			}
			m_scrollDelay.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_DELAY, strTemp);
			break;

		case IDC_SCR_LIFT:
			iTmpPos = m_scrollLift.GetScrollPos();
			if (iTmpPos > 0)
			{
				iTmpPos--;
			}
			m_scrollLift.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_LIFT, strTemp);
			break;

		case IDC_SCR_PUL:
			iTmpPos = m_scrollPulse.GetScrollPos();
			if (iTmpPos > 0)
			{
				iTmpPos--;
			}
			m_scrollPulse.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_PULSE, strTemp);
			break;

		case IDC_SCR_TEMP:
			iTmpPos = m_scrollTemp.GetScrollPos();
			if (iTmpPos > 0)
			{
				iTmpPos--;
			}
			m_scrollTemp.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_TEMP, strTemp);
			break;
		}
		break;

	case SB_PAGELEFT:
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SCR_RISING:
			iTmpPos = m_scrollRising.GetScrollPos();
			if (iTmpPos > 10)
			{
				iTmpPos -= 10;
			}
			m_scrollRising.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_RISINGTIME, strTemp);
			break;

		case IDC_SCR_OPEN:
			iTmpPos = m_scrollOpen.GetScrollPos();
			if (iTmpPos > 10)
			{
				iTmpPos -= 10;
			}
			m_scrollOpen.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_OPEN, strTemp);
			break;

		case IDC_SCR_DOWN:
			iTmpPos = m_scrollFalling.GetScrollPos();
			if (iTmpPos > 10)
			{
				iTmpPos -= 10;
			}
			m_scrollFalling.SetScrollPos(iTmpPos);
			if (Disp_Vermes == g_pFrm->m_comDisp[0].m_eDispType)
			{
				strTemp.Format("%.02f", (float)iTmpPos/100);
			}
			else if (Disp_Kaifa == g_pFrm->m_comDisp[0].m_eDispType)
			{
				strTemp.Format("%.02f", (float)iTmpPos/10);
			}
			SetDlgItemText(IDC_EDIT_DOWN, strTemp);
			break;

		case IDC_SCR_DELAY:
			iTmpPos = m_scrollDelay.GetScrollPos();
			if (iTmpPos > 10)
			{
				iTmpPos -= 10;
			}
			m_scrollDelay.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_DELAY, strTemp);
			break;

		case IDC_SCR_LIFT:
			iTmpPos = m_scrollLift.GetScrollPos();
			if (iTmpPos > 10)
			{
				iTmpPos -= 10;
			}
			m_scrollLift.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_LIFT, strTemp);
			break;

		case IDC_SCR_PUL:
			iTmpPos = m_scrollPulse.GetScrollPos();
			if (iTmpPos > 10)
			{
				iTmpPos -= 10;
			}
			m_scrollPulse.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_PULSE, strTemp);
			break;

		case IDC_SCR_TEMP:
			iTmpPos = m_scrollTemp.GetScrollPos();
			if (iTmpPos > 10)
			{
				iTmpPos -= 10;
			}
			m_scrollTemp.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_TEMP, strTemp);
			break;
		}
		break;

	case SB_PAGERIGHT:
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SCR_RISING:
			iTmpPos = m_scrollRising.GetScrollPos();
			if (iTmpPos < 100)
			{
				iTmpPos += 10;
			}
			m_scrollRising.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_RISINGTIME, strTemp);
			break;

		case IDC_SCR_OPEN:
			iTmpPos = m_scrollOpen.GetScrollPos();
			if (iTmpPos < 150)
			{
				iTmpPos += 10;
			}
			m_scrollOpen.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_OPEN, strTemp);
			break;

		case IDC_SCR_DOWN:
			iTmpPos = m_scrollFalling.GetScrollPos();
			if (iTmpPos < 100)
			{
				iTmpPos += 10;
			}
			m_scrollFalling.SetScrollPos(iTmpPos);
			if (Disp_Vermes == g_pFrm->m_comDisp[0].m_eDispType)
			{
				strTemp.Format("%.02f", (float)iTmpPos/100);
			}
			else if (Disp_Kaifa == g_pFrm->m_comDisp[0].m_eDispType)
			{
				strTemp.Format("%.02f", (float)iTmpPos/10);
			}
			SetDlgItemText(IDC_EDIT_DOWN, strTemp);
			break;

		case IDC_SCR_DELAY:
			iTmpPos = m_scrollDelay.GetScrollPos();
			if (iTmpPos < 1000)
			{
				iTmpPos += 10;
			}
			m_scrollDelay.SetScrollPos(iTmpPos);
			strTemp.Format("%.02f", (float)iTmpPos/10);
			SetDlgItemText(IDC_EDIT_DELAY, strTemp);
			break;

		case IDC_SCR_LIFT:
			iTmpPos = m_scrollLift.GetScrollPos();
			if (iTmpPos < 100)
			{
				iTmpPos += 10;
			}
			m_scrollLift.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_LIFT, strTemp);
			break;

		case IDC_SCR_PUL:
			iTmpPos = m_scrollPulse.GetScrollPos();
			if (iTmpPos < 1000)
			{
				iTmpPos += 10;
			}
			m_scrollPulse.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_PULSE, strTemp);
			break;

		case IDC_SCR_TEMP:
			iTmpPos = m_scrollTemp.GetScrollPos();
			if (iTmpPos < 100)
			{
				iTmpPos += 10;
			}
			m_scrollTemp.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_TEMP, strTemp);
			break;
		}
		break;
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CDlgDispenserParam::OnBnClickedBtnLoadFromFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nDispIdx = m_comboDispIdx.GetCurSel();
	int nDispType = m_comboDispType.GetCurSel();

	g_pFrm->m_comDisp[nDispIdx].SetDispenserType(DispenserType(nDispType));
	if (1 == g_pFrm->m_comDisp[nDispIdx].HandleParamFile(TRUE))
	{
		m_dispParam = g_pFrm->m_comDisp[nDispIdx].m_dispParam;
		InitScrollBar();
	}
}


void CDlgDispenserParam::OnBnClickedBtnSetParam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nDispIdx = m_comboDispIdx.GetCurSel();
//	int nDispType = m_comboDispType.GetCurSel();
	int nDispCtrl = m_comboDispCtrl.GetCurSel();

	CString strTemp;

	GetDlgItem(IDC_EDIT_OPEN)->GetWindowText(strTemp);
	m_dispParam.OpenTime = int(atof(strTemp) * 10);
	GetDlgItem(IDC_EDIT_RISINGTIME)->GetWindowText(strTemp);
	m_dispParam.Rising = int(atof(strTemp) * 10);
	GetDlgItem(IDC_EDIT_DOWN)->GetWindowText(strTemp);
	m_dispParam.Falling = int(atof(strTemp) * 100);
	GetDlgItem(IDC_EDIT_LIFT)->GetWindowText(strTemp);
	m_dispParam.NeedleLift = atoi(strTemp);
	GetDlgItem(IDC_EDIT_PULSE)->GetWindowText(strTemp);
	m_dispParam.NumOfPul = atoi(strTemp);
	GetDlgItem(IDC_EDIT_TEMP)->GetWindowText(strTemp);
	//m_dispParam.Temp = atoi(strTemp);
	GetDlgItem(IDC_EDIT_DELAY)->GetWindowText(strTemp);
	m_dispParam.Delay = int(atof(strTemp) * 10);

	GetDlgItemText(IDC_COMBO_DISP_TYPE, strTemp);
	if (Valve_Yikai == strTemp)
	{
		g_pFrm->m_pRobotParam->m_nValveType = 3;

		GetDlgItem(IDC_EDIT_OPEN)->GetWindowText(strTemp);
		m_dispParam.OpenTime = int(atof(strTemp) * 100);
		GetDlgItem(IDC_EDIT_RISINGTIME)->GetWindowText(strTemp);
		m_dispParam.Rising = int(atof(strTemp) * 100);
		GetDlgItem(IDC_EDIT_DOWN)->GetWindowText(strTemp);
		m_dispParam.Falling = int(atof(strTemp) * 100);
		GetDlgItem(IDC_EDIT_LIFT)->GetWindowText(strTemp);
		m_dispParam.NeedleLift = atoi(strTemp);
		GetDlgItem(IDC_EDIT_PULSE)->GetWindowText(strTemp);
		m_dispParam.NumOfPul = atoi(strTemp);
		GetDlgItem(IDC_EDIT_TEMP)->GetWindowText(strTemp);
		m_dispParam.Temp = atoi(strTemp);
		GetDlgItem(IDC_EDIT_DELAY)->GetWindowText(strTemp);
		m_dispParam.Delay = int(atof(strTemp) * 100);

		g_pFrm->m_comDisp[nDispIdx].SetDispenserType(Disp_Yikai);
	}
	if (Valve_Vemers == strTemp)
	{
		g_pFrm->m_pRobotParam->m_nValveType = 0;

		GetDlgItem(IDC_EDIT_OPEN)->GetWindowText(strTemp);
		m_dispParam.OpenTime = int(atof(strTemp) * 10);
		GetDlgItem(IDC_EDIT_RISINGTIME)->GetWindowText(strTemp);
		m_dispParam.Rising = int(atof(strTemp) * 10);
		GetDlgItem(IDC_EDIT_DOWN)->GetWindowText(strTemp);
		m_dispParam.Falling = int(atof(strTemp) * 100);
		GetDlgItem(IDC_EDIT_LIFT)->GetWindowText(strTemp);
		m_dispParam.NeedleLift = atoi(strTemp);
		GetDlgItem(IDC_EDIT_PULSE)->GetWindowText(strTemp);
		m_dispParam.NumOfPul = atoi(strTemp);
		GetDlgItem(IDC_EDIT_TEMP)->GetWindowText(strTemp);
		m_dispParam.Temp = atoi(strTemp);
		GetDlgItem(IDC_EDIT_DELAY)->GetWindowText(strTemp);
		m_dispParam.Delay = int(atof(strTemp) * 10);

		g_pFrm->m_comDisp[nDispIdx].SetDispenserType(Disp_Vermes);
	}
	else if (Valve_Kaifa == strTemp)
	{
		g_pFrm->m_pRobotParam->m_nValveType = 1;

		GetDlgItem(IDC_EDIT_RISINGTIME)->GetWindowText(strTemp);
		m_dispParam.Rising = int(atof(strTemp) * 10);
		GetDlgItem(IDC_EDIT_DOWN)->GetWindowText(strTemp);
		m_dispParam.Falling = int(atof(strTemp) * 10);
		GetDlgItem(IDC_EDIT_PULSE)->GetWindowText(strTemp);
		m_dispParam.NumOfPul = atoi(strTemp);
		g_pFrm->m_comDisp[nDispIdx].SetDispenserType(Disp_Kaifa);
	}
	else if (Valve_Musashi == strTemp)
	{
		g_pFrm->m_pRobotParam->m_nValveType = 2;

		GetDlgItem(IDC_EDIT_RISINGTIME)->GetWindowText(strTemp);
		m_dispParam.Rising = int(atof(strTemp) * 100);
		GetDlgItem(IDC_EDIT_DOWN)->GetWindowText(strTemp);
		m_dispParam.Falling = int(atof(strTemp) * 100);
		GetDlgItem(IDC_EDIT_PULSE)->GetWindowText(strTemp);
		m_dispParam.NumOfPul = atoi(strTemp);
		GetDlgItem(IDC_EDIT_LIFT)->GetWindowText(strTemp);
		m_dispParam.NeedleLift = atoi(strTemp);
		g_pFrm->m_comDisp[nDispIdx].SetDispenserType(Disp_Musashi);
	}

	// ��ǰʹ�õĽ�������	m_eValveType
	CString strFile = CFunction::GetDirectory() + "system\\robotparam.dat";
	CFunction::HandleInt(FALSE, "CurDispType", "DispType", g_pFrm->m_pRobotParam->m_nValveType, strFile);


	g_pFrm->m_comDisp[nDispIdx].m_dispParam = m_dispParam;
 	if (1 == g_pFrm->m_comDisp[nDispIdx].HandleParamDispenser(FALSE))
	{
		g_pFrm->m_comDisp[nDispIdx].HandleParamFile(FALSE);
		AfxMessageBox("���õ㽺������������ɹ�!");
		// ��ǰʹ�õĿ��Ʒ�ʽ
		g_pFrm->m_pRobotParam->m_nValveCtrlMode = nDispCtrl;	
		g_pFrm->m_Robot->m_pController->TS_ListSetDispCtrlMode(DispenserCtrlMode(nDispCtrl));
		CFunction::HandleInt(FALSE, "CurDispCtrl", "DispCtrl", g_pFrm->m_pRobotParam->m_nValveCtrlMode, strFile);
	}
	else
	{
		AfxMessageBox("���õ㽺���������������!");
	}
}


// void CDlgDispenserParam::OnBnClickedBtnSetWtpos()
// {
// 	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	if (IDYES != AfxMessageBox("��ʹ�þ�ͷλ����Ϊ����У׼λ��\n  ��ȷ�Ͼ�ͷ��Ұ�ڷϽ�������", MB_YESNO))
// 	{
// 		return;
// 	}
// 	g_pFrm->m_Precision.GetCurPosMM();
// 
// 	g_pFrm->m_pRobotParam->m_posWeightAdj.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
// 	g_pFrm->m_pRobotParam->m_posWeightAdj.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
// 
// 	CString strTemp;
// 	//////////////////////////////////////////////////////////////////////////
// 	// ����У��λ��
// 	strTemp.Format("%.03f", g_pFrm->m_pRobotParam->m_posWeightAdj.x);
// 	SetDlgItemText(IDC_EDIT_WTPOS_X, strTemp);
// 	strTemp.Format("%.03f", g_pFrm->m_pRobotParam->m_posWeightAdj.y);
// 	SetDlgItemText(IDC_EDIT_WTPOS_Y, strTemp);
// 
// 	GetDlgItemText(IDC_EDIT_WTPOS_Z, strTemp);
// 	g_pFrm->m_pRobotParam->m_posWeightAdj.z = atof(strTemp);
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// ��׼����
// 	GetDlgItemText(IDC_EDIT_DEF_GLUE_WT, strTemp);
// 	double dStdGlueWt = atof(strTemp);
// 	g_pFrm->m_pRobotParam->m_dAvgWeight = dStdGlueWt;
// 
// 	g_pFrm->RobotParam(FALSE);
// }


void CDlgDispenserParam::OnBnClickedBtnReadPressure()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	short rtn;
	double dVal;
	rtn = g_pFrm->m_Robot->GetBackPressure(dVal);
	if (RTN_NONE == rtn)
	{
		CString strTemp;
		strTemp.Format("%.03f", dVal);
		SetDlgItemText(IDC_EDIT_PRESSURE, strTemp);
	}
	else
	{
		g_pFrm->m_Robot->AfxMessageBox(_T("��ȡ��ѹ����!"));
	}
}


// void CDlgDispenserParam::OnBnClickedBtnMvtoWtpos()
// {
// 	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	g_pFrm->m_Robot->GotoSafeZpos();	
// 	CString strTemp;
// 	GetDlgItemText(IDC_EDIT_WTPOS_Z, strTemp);
// 	g_pFrm->m_pRobotParam->m_posWeightAdj.z = atof(strTemp);
// 
// 	double x, y, z;
// 	x = g_pFrm->m_pRobotParam->m_posWeightAdj.x;
// 	x += g_pFrm->m_pSysParam->tOffset[0].x;
// 	y = g_pFrm->m_pRobotParam->m_posWeightAdj.y;
// 	y += g_pFrm->m_pSysParam->tOffset[0].y;
// 	z = g_pFrm->m_pRobotParam->m_zContactPos - z;
// 
// 	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_Vmin, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
// 	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc, TRUE);
// }

void CDlgDispenserParam::MvtoWtpos(void)
{ 	
	g_pFrm->m_Robot->GotoSafeZpos();	

	double x=0, y=0, z=0;
	x = g_pFrm->m_pRobotParam->m_posWeightAdj.x;
	x += g_pFrm->m_pSysParam->tOffset[0].x;
	y = g_pFrm->m_pRobotParam->m_posWeightAdj.y;
	y += g_pFrm->m_pSysParam->tOffset[0].y;
	z = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_posWeightAdj.z;

	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc, TRUE);
}

void CDlgDispenserParam::OnBnClickedBtnWtAdj()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MvtoWtpos();
	CString str;
	GetDlgItemText(IDC_EDIT_WT_NUM, str);
	if ("" == str)
	{
		AfxMessageBox("���ص�������Ϊ��!");
		return;
	}
	int dotNum = atoi(str);		// ����
	if (0 == dotNum)
	{
		AfxMessageBox("���ص�������Ϊ0!");
		return;
	}
	g_pFrm->m_pRobotParam->m_nWeightAdjDotNum = dotNum;

	GetDlgItemText(IDC_EDIT_DEF_GLUE_WT, str);
	if ("" == str)
	{
		AfxMessageBox("��׼���ز���Ϊ��!");
		return;
	}	
	double dotWt = atof(str);	// ���㽺��
	if (dotWt < EPS) 
	{
		AfxMessageBox("��׼���ز���Ϊ0!");
		return;
	}
	g_pFrm->m_pRobotParam->m_dAvgWeight = dotWt;
	g_pFrm->RobotParam(FALSE);

	GetDlgItem(IDC_BTN_WT_ADJ)->EnableWindow(FALSE);
	double dWtVal[3];	// ���ӳƶ���
	g_pFrm->m_comWeight.GetWeight(dWtVal[0]);
	g_pFrm->m_pRobotParam->m_dWasteGlueCur += dWtVal[0];

	g_pFrm->m_comWeight.SetZero();
	g_pFrm->m_comWeight.SetImdRpt();

	double dAvgWt = 0.0;

	for (int i=0; i<3; i++)
	{
		g_pFrm->m_Robot->DotDisp(dotNum);
		CFunction::DelaySec(2);
		g_pFrm->m_comWeight.GetWeight(dWtVal[i]);
		g_pFrm->m_pRobotParam->m_dWasteGlueCur += dWtVal[i];
		g_pFrm->m_comWeight.SetZero();
		
		if (dWtVal[i] < EPS)
		{
			AfxMessageBox("���㽺��̫С!");
			GetDlgItem(IDC_BTN_WT_ADJ)->EnableWindow(TRUE);
			return;
		}

		dAvgWt = dWtVal[i] / dotNum; 
		str.Format("%.3f", dAvgWt);
		SetDlgItemText(IDC_EDIT_DEF_GLUE_WT2, str);

		if ((dAvgWt > dotWt*2) || (dAvgWt < dotWt/2.0))
		{
			AfxMessageBox("����У������!");
			GetDlgItem(IDC_BTN_WT_ADJ)->EnableWindow(TRUE);
			return;
		}
	}

	if (g_pFrm->m_pRobotParam->m_dWasteGlueCur > (g_pFrm->m_pRobotParam->m_dWasteGlueAlarm*1000))
	{
		g_pFrm->m_Robot->ExOutput(_T("��ɫ��.��"),TRUE);
		g_pFrm->m_Robot->ExOutput(_T("������"),TRUE);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.m_bWasteGlueAlarm = true;
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetCurWasteGlue("�Ͻ�������");
	}
	else
	{
		CString strTemp;
		strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_dWasteGlueCur/1000.0);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetCurWasteGlue(strTemp);
	}

	dotWt = (dWtVal[0] + dWtVal[1] + dWtVal[2])/3.0/dotNum;
	str.Format("%.3f", dotWt);
	SetDlgItemText(IDC_EDIT_DEF_GLUE_WT, str);
	g_pFrm->m_pRobotParam->m_dAvgWeight = dotWt;
	g_pFrm->RobotParam(FALSE);
	GetDlgItem(IDC_BTN_WT_ADJ)->EnableWindow(TRUE);

	//////////////////////////////////////////////////////////////////////////
	// ���ؼ�ʱ��ʼ
	g_pFrm->m_Robot->m_tmAutoWeight.TimerStart();
}

UINT ThreadDotDisp(LPVOID lParam)
{
	int nDots = *(int*)lParam;
	short sTemp;

	double dDispCyc, dDispFreq;
	g_pFrm->m_comDisp[0].GetCycleTimeAndFreq(dDispCyc, dDispFreq);

	CKFTimer tmTemp;
	tmTemp.TimerStart();
	
	int j = 0;
	if (0 == nDots)		
		nDots = 99999999;	// �������ֱ���ֶ�ֹͣ

	while(g_pFrm->m_Robot->m_bDispOpen)
	{
		sTemp = g_pFrm->m_Robot->m_pController->ComparePulse();
		j++;

		if (j == nDots)
			break;

		while (tmTemp.TimerGetMs() < j*dDispCyc)
		{
			//CFunction::DoEvents();
		}

		if (sTemp != RTN_NONE)
		{
			return sTemp;
		}
	}

	return RTN_NONE;
}

void CDlgDispenserParam::OnBnClickedBtnValveOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp;
	GetDlgItemText(IDC_BTN_VALVE_ON, strTemp);
	if ("�㽺��[�ѹ�]" == strTemp)
	{
		SetDlgItemText(IDC_BTN_VALVE_ON, "�㽺��[�ѿ�]");
		if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
		{
			if (DispCtrlMode_IO == g_pFrm->m_pRobotParam->m_nValveCtrlMode)
			{
				g_pFrm->m_Robot->ExOutput(_T("1�ŵ��̬����"), TRUE);
			}
			else if (DispCtrlMode_Compare == g_pFrm->m_pRobotParam->m_nValveCtrlMode)
			{
				int nDots = GetDlgItemInt(IDC_EDIT_OPEN_DOTNUM);
				g_pFrm->m_Robot->m_bDispOpen = true;
				AfxBeginThread(ThreadDotDisp, &nDots);
			}
		}
		else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
		{
			g_pFrm->m_Robot->ExOutput("1�ŵ��̬����", TRUE);
		}
	}
	else
	{
		SetDlgItemText(IDC_BTN_VALVE_ON, "�㽺��[�ѹ�]");
		if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
		{
			if (DispCtrlMode_IO == g_pFrm->m_pRobotParam->m_nValveCtrlMode)
			{
				g_pFrm->m_Robot->ExOutput(_T("1�ŵ��̬����"), FALSE);
			}
			else if (DispCtrlMode_Compare == g_pFrm->m_pRobotParam->m_nValveCtrlMode)
			{
				g_pFrm->m_Robot->m_bDispOpen = false;
			}
		}
		else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
		{
			g_pFrm->m_Robot->ExOutput(_T("1�ŵ��̬����"), FALSE);
		}
	}	
}

void CDlgDispenserParam::InitStaticCtrl(int dispType)
{
	if (Disp_Vermes == dispType || Disp_Yikai == dispType)
	{
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC11)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC14)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_SCR_OPEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCR_LIFT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCR_DELAY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCR_TEMP)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_EDIT_OPEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_LIFT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_DELAY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_TEMP)->ShowWindow(SW_SHOW);
	}
	else if (Disp_Kaifa == dispType)
	{
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC14)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_SCR_OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCR_LIFT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCR_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCR_TEMP)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_LIFT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TEMP)->ShowWindow(SW_HIDE);
	}
	else if (Disp_Musashi == dispType)
	{
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC14)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_SCR_OPEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCR_LIFT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCR_DELAY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCR_TEMP)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_OPEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_LIFT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_DELAY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_TEMP)->ShowWindow(SW_HIDE);
	}
}

void CDlgDispenserParam::OnCbnSelchangeComboDispType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������		   
	int idx = m_comboDispType.GetCurSel();
	InitStaticCtrl(idx);
}


void CDlgDispenserParam::OnCbnSelchangeComboDispSel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp;
	GetDlgItemText(IDC_COMBO_DISP_SEL, strTemp);

//	int idx = m_comboDispIdx.GetCurSel();
}


void CDlgDispenserParam::OnBnClickedBtnSetPressure()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ����������		��Ͱ��ѹ	
	CString strTemp;
	GetDlgItemText(IDC_EDIT_PRESSURE, strTemp);
	double dPressVal = atof(strTemp);
	//short idata = (short)(dPressVal / 0.4 * 32767);

	if (RTN_NONE == g_pFrm->m_Robot->SetBackPressure(dPressVal))
	{
		AfxMessageBox("��Ͱ��ѹ���óɹ�!");
		g_pFrm->m_pRobotParam->m_dBackPress = dPressVal;
		g_pFrm->RobotParam(FALSE);
	}
	else
	{
		AfxMessageBox("��Ͱ��ѹ����ʧ��!");
		strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_dBackPress);
		SetDlgItemText(IDC_EDIT_PRESSURE, strTemp);
	}
}


void CDlgDispenserParam::OnBnClickedChkWeight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_WEIGHT);
	if (pBtn->GetCheck())
	{
		g_pFrm->m_pRobotParam->m_nIfAutoWeight = 1;
	}
	else
	{
		g_pFrm->m_pRobotParam->m_nIfAutoWeight = 0;
	}
	GetDlgItem(IDC_RADIO_T)->EnableWindow(g_pFrm->m_pRobotParam->m_nIfAutoWeight);
	GetDlgItem(IDC_RADIO_P)->EnableWindow(g_pFrm->m_pRobotParam->m_nIfAutoWeight);
	GetDlgItem(IDC_EDIT_WEIGHT_INTERVAL_VALUE)->EnableWindow(g_pFrm->m_pRobotParam->m_nIfAutoWeight);
}



void CDlgDispenserParam::OnBnClickedRadioT()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	g_pFrm->m_pRobotParam->m_nWeightIntervalType = m_nWeightType;
}


void CDlgDispenserParam::OnBnClickedRadioP()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	g_pFrm->m_pRobotParam->m_nWeightIntervalType = m_nWeightType;
}


void CDlgDispenserParam::OnCbnSelchangeComboDispCtrl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nCtrlMode = m_comboDispCtrl.GetCurSel();

	// ��ǰʹ�õĿ��Ʒ�ʽ
	CString strFile = CFunction::GetDirectory() + "system\\robotparam.dat";
	g_pFrm->m_pRobotParam->m_nValveCtrlMode = nCtrlMode;	
	g_pFrm->m_Robot->m_pController->TS_ListSetDispCtrlMode(DispenserCtrlMode(nCtrlMode));
	CFunction::HandleInt(FALSE, "CurDispCtrl", "DispCtrl", g_pFrm->m_pRobotParam->m_nValveCtrlMode, strFile);
}

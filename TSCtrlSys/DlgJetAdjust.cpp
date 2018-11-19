// DlgJetAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgJetAdjust.h"

#include "DlgJogXYZ.h"

#define pos_col	6	// Gridctrl列数
enum Sys_pos_index	// Gridctrl中显示的点位的顺序
{
	pos_null,			// 空,使安全按高度起始序号位1
	pos_z_safe,				// z轴安全高度
	pos_contact,			// 接触测高位置
	pos_laser_base,			// 激光测量接触传感器高度的位置
	pos_camera_cross,		// 相机观察十字线中心位置(相机与激光offset校正时用到)
	pos_laser_cross,		// 激光打在十字线中心的位置
	pos_jet_adjust,			// 四点校正喷射左上角坐标(针头)
	pos_jet_adjust_height,	// 四点校正相对高度
	pos_weight_camera,		// 称重校准位(镜头观察)
	pos_weight_disp,		// 称重校准位(针头)		(与镜头位置联动，修改一个另外一个随之变动)
	Pos_weight_height,		// 称重校准相对高度
	pos_clean_camera,		// 针头清洗位(镜头观察)
	pos_clean_disp,			// 针头清洗位(针头)		(与镜头位置联动，修改一个另外一个随之变动)
	pos_clean_height,		// 针头清洗高度
	pos_num					// 需要设置的点位数目
};

static CString g_strPosName[] =	// Gridctrl中显示的点位的名称和解释
{
	"","",		
	"安全高度",				"平移而不会撞机的高度",					//pos_z_safe=1,			// z轴安全高度
	"接触测高位",			"针头在接触测高传感器上方的位置",		//pos_contact,			// 接触测高位置
	"激光接触测高位",		"激光光斑在接触测高传感器正中间的位置",	//pos_laser_base,		// 激光测量接触传感器高度的位置
	"镜头十字线",			"镜头中心与校准十字线中心重合的位置",	//pos_camera_cross,		// 相机观察十字线中心位置(相机与激光offset校正时用到)
	"激光十字线",			"激光光斑打在校准十字线中心的位置",		//pos_laser_cross,		// 激光打在十字线中心的位置
	"四点校正位",			"四点校正时左上胶点位置(喷嘴设置)",		//pos_jet_adjust,		// 四点校正喷射左上角坐标(针头)
	"四点校正高度",			"相对陶瓷片的高度",						//pos_jet_adjust_height,// 四点校正相对高度
	"称重位置(相机)",		"称重位置(相机),与针头位置联动",		//pos_weight_camera,	// 称重校准位(镜头观察)
	"称重位置(针头)",		"称重位置(针头)",						//pos_weight_disp,		// 称重校准位(针头)		(与镜头位置联动，修改一个另外一个随之变动)
	"称重校准高度",			"称重校准高度,相对接触测高的高度",		//Pos_weight_height,	// 称重校准相对高度
	"清洗位置(相机)",		"清洗位置(相机),与针头位置联动",		//pos_clean_camera,		// 针头清洗位(镜头观察)
	"清洗位置(针头)",		"清洗位置(针头)",						//pos_clean_disp,		// 针头清洗位(针头)		(与镜头位置联动，修改一个另外一个随之变动)
	"清洗高度",				"清洗高度,相对接触测高的高度",			//pos_clean_height,		// 针头清洗高度
	//pos_num					// 需要设置的点位数目
};
// CDlgJetAdjust dialog

IMPLEMENT_DYNAMIC(CDlgJetAdjust, CDialog)

CDlgJetAdjust::CDlgJetAdjust(CWnd* pParent /*=NULL*/)
: CDialog(CDlgJetAdjust::IDD, pParent)
{

}

CDlgJetAdjust::~CDlgJetAdjust()
{
}

void CDlgJetAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_JET_ADJUST_LIST, m_ListFun);
	DDX_Control(pDX, IDC_DLG_JET_ADJUST_PROGRESS1, m_Prog);
	DDX_Control(pDX, IDC_GRID_POS, m_GridPos);
	DDX_Control(pDX, IDC_BTN_READ_LASER, m_btnHeightRead);
	DDX_Control(pDX, IDC_BTN_MOVTO, m_btnMovTo);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_SET, m_btnSetPos);
}


BEGIN_MESSAGE_MAP(CDlgJetAdjust, CDialog)
	ON_BN_CLICKED(IDC_DLG_JET_ADJUST_START, &CDlgJetAdjust::OnBnClickedBtStart)
//	ON_LBN_SELCHANGE(IDC_DLG_JET_ADJUST_LIST, &CDlgJetAdjust::OnLbnSelchangeListfun)
	ON_BN_CLICKED(IDC_DLG_JET_ADJUST_END, &CDlgJetAdjust::OnBnClickedBtEnd)
	ON_BN_CLICKED(IDC_DLG_JET_ADJUST_JOG, &CDlgJetAdjust::OnBnClickedDlgJetAdjustJog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgJetAdjust::OnBnClickedButton1)

	ON_BN_CLICKED(IDC_BTN_CONTACT_TEST, &CDlgJetAdjust::OnBnClickedBtnContactTest)

	ON_BN_CLICKED(IDC_BTN_MOVTO, &CDlgJetAdjust::OnBnClickedBtnMovto)
	ON_BN_CLICKED(IDC_BTN_SET, &CDlgJetAdjust::OnBnClickedBtnSet)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgJetAdjust::OnBnClickedBtnSave)
// 	ON_BN_CLICKED(IDC_BTN_MOVTO_JET_TEST_POS, &CDlgJetAdjust::OnBnClickedBtnMovtoJetTestPos)
// 	ON_BN_CLICKED(IDC_BTN_SET_JET_TEST_POS, &CDlgJetAdjust::OnBnClickedBtnSetJetTestPos)
// 	ON_BN_CLICKED(IDC_BTN_CONTACT_TEST, &CDlgJetAdjust::OnBnClickedBtnContactTest)
// 	ON_BN_CLICKED(IDC_BTN_SET_ZSAFE_POS, &CDlgJetAdjust::OnBnClickedBtnSetZsafePos)
// 	ON_BN_CLICKED(IDC_BTN_CONTACT_READ_HEIGHT, &CDlgJetAdjust::OnBnClickedBtnContactReadHeight)
// 	ON_BN_CLICKED(IDC_BTN_MOVTO_LASER_BASE_POS, &CDlgJetAdjust::OnBnClickedBtnMovtoLaserBasePos)
// 	ON_BN_CLICKED(IDC_BTN_SET_LASER_BASE_POS, &CDlgJetAdjust::OnBnClickedBtnSetLaserBasePos)
	ON_BN_CLICKED(IDC_BTN_READ_LASER, &CDlgJetAdjust::OnBnClickedBtnReadLaser)
	ON_BN_CLICKED(IDC_BTN_PROBE_SWITCH, &CDlgJetAdjust::OnBnClickedBtnProbeSwitch)

	ON_NOTIFY(NM_DBLCLK, IDC_GRID_POS, OnGridDbClick)
END_MESSAGE_MAP()


BOOL CDlgJetAdjust::OnInitDialog()
{
	CDialog::OnInitDialog();
     
// 	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Tag_0"));
// 	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Tag_1"));
// 	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Tag_2"));
// 	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Tag_3"));
// 	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Tag_4"));
// 	GetDlgItem(Tag_5)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Tag_5"));
// 	GetDlgItem(Tag_6)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Tag_6"));
// 	GetDlgItem(Tag_7)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Tag_7"));
	
	GetDlgItem(IDC_DLG_JET_ADJUST_START)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Btn_1"));
	GetDlgItem(IDC_DLG_JET_ADJUST_JOG)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Btn_2"));
	GetDlgItem(IDC_DLG_JET_ADJUST_END)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Btn_3"));

   
	CString strtemp;   // 
	strtemp.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Msg_1"));
	m_ListFun.AddString(strtemp); 
	strtemp.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Msg_2"));
	m_ListFun.AddString(strtemp); 
	strtemp.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Msg_3"));
	m_ListFun.AddString(strtemp); 
	strtemp.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Msg_4"));
	m_ListFun.AddString(strtemp); 
	strtemp.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Msg_5"));
	m_ListFun.AddString(strtemp); 
	strtemp.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgJetAdjust,"Msg_6"));
	m_ListFun.AddString(strtemp); 

	m_Prog.SetRange(0,100);

	m_ListFun.Enable(0,true);

	for (int i=1;i<6;i++)
	{
// 		strtemp.Format("%d", i);
// 		m_ListFun.AddString(strtemp);
		m_ListFun.Enable(i,false);
	}

	CreateGrid();
	m_btnMovTo.SetTooltipText(_T("移动到选定的点位"));
	m_btnSetPos.SetTooltipText(_T("更新当前点位坐标"));
	m_btnHeightRead.SetTooltipText(_T("测定当前选中点位的高度数值!"));

	m_btnSave.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnMovTo.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnSetPos.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnHeightRead.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnMovTo.SetIcon(IDI_ICON_MOVETO);
	m_btnSetPos.SetIcon(IDI_ICON_SINGLE_ADJUST);
	m_btnSave.SetIcon(IDI_ICON_SAVE_S);
	UpdateCtrl();

	// 创建Gridctrl
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgJetAdjust::UpdateCtrl()		// 更新控件显示,将机器参数显示到界面
{
	CString strtemp;   // 
	// 显示当前偏差
	strtemp.Format("%.3f",g_pFrm->m_pSysParam->tOffset[0].x);   
	SetDlgItemText(IDC_DLG_JET_ADJUST_RX,strtemp);
	strtemp.Format("%.3f",g_pFrm->m_pSysParam->tOffset[0].y);
	SetDlgItemText(IDC_DLG_JET_ADJUST_RY,strtemp);

	// 喷射阀&针头
	if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
	{
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DLG_JET_ADJUST_NUM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DLG_JET_ADJUST_OPEN_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DLG_JET_ADJUST_UP_HEIGHT)->ShowWindow(SW_HIDE);
	}
	else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
	{
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DLG_JET_ADJUST_NUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DLG_JET_ADJUST_OPEN_TIME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DLG_JET_ADJUST_UP_HEIGHT)->ShowWindow(SW_SHOW);
	}

	// 测高方式:激光测高&探针测高
	if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
	{
		GetDlgItem(IDC_BTN_PROBE_SWITCH)->ShowWindow(SW_HIDE);	
	}
	else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
	{
		GetDlgItem(IDC_BTN_READ_LASER)->SetWindowText(_T("探针测高"));	
	}
	strtemp.Format("%d",g_pFrm->m_pSysParam->nJetPulse);
	SetDlgItemText(IDC_DLG_JET_ADJUST_NUM,strtemp);
	strtemp.Format("%d", 100);
	SetDlgItemText(IDC_DLG_JET_ADJUST_OPEN_TIME,strtemp);
	strtemp.Format("%.3f", 5.0);
	SetDlgItemText(IDC_DLG_JET_ADJUST_UP_HEIGHT,strtemp);
}

void CDlgJetAdjust::OnBnClickedBtStart()
{
	// TODO: Add your control notification handler code here
	CButton  *bt1=(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_END);
	CButton  *bt2=(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_START);
	CButton  *bt3=(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_JOG);
	CString	 str;

	for (int i=0;i<6;i++)
	{
		if(m_ListFun.GetCheck(i))
			Index=i;
	}

	switch (Index)
	{
	case 0:    //  喷射4点坐标    
		if(JetFourAdjust()==false)   
 			return;
		m_Prog.SetPos(10);
		for (int i=1;i<6;i++)
		{
			m_ListFun.Enable(i,true);
		}
		break;

	case 1:   // 获取第一个点图像中心位置
		MoveCarm(1);   // 移动到第一个点图像中心大概位置
		m_Prog.SetPos(20);
		bt3->EnableWindow(TRUE);
		break;

	case 2:
		MoveCarm(2);
		m_Prog.SetPos(40);
		break;

	case 3:
		MoveCarm(3);
		m_Prog.SetPos(60);
		break;

	case 4:
		MoveCarm(4);
		m_Prog.SetPos(80);		
		break;

	case 5:
		CapCarmoffsetJet();

		bt1->EnableWindow(TRUE);
		bt2->EnableWindow(FALSE);
		m_Prog.SetPos(100);
		bt3->EnableWindow(FALSE);
		break;

	default:
		break;
	}
}

// void CDlgJetAdjust::OnLbnSelchangeListfun()
// {
// 	// TODO: Add your control notification handler code here
// 	for (int i=0;i<6;i++)
// 	{
// 		if(m_ListFun.GetCheck(i))
// 			Index=i;
// 	}
// 	CString str;
// 	CButton  *bt1=(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_END);
// 	switch (Index)
// 	{
// 	case 0:
// 		str.Format("%0.3f", g_pFrm->m_pSysParam->tJetAdjustPos.x);  
// 		SetDlgItemText(IDC_DLG_JET_ADJUST_EX,str);
// 		str.Format("%0.3f", g_pFrm->m_pSysParam->tJetAdjustPos.y);  
// 		SetDlgItemText(IDC_DLG_JET_ADJUST_EY,str);
// 		str.Format("%0.3f", g_pFrm->m_pSysParam->tJetAdjustPos.z);  
// 		SetDlgItemText(IDC_DLG_JET_ADJUST_EZ,str);
// 		bt1->EnableWindow(FALSE);
// 		break;
// 	case 1:
// 	case 2:
// 	case 3:
// 	case 4:
// 	case 5:
// 		//bt1->EnableWindow(TRUE);
// 		break;
// 	case 6:
// 		bt1->EnableWindow(FALSE);
// 		break;
// 	default:
// 		break;
// 
// 	}
// }

void CDlgJetAdjust::CreateGrid(void)
{
	//只支持8位
	//m_ImageList.Create(MAKEINTRESOURCE(IDB_BITMAP_IOSTS), 28, 1, RGB(255,255,255));
	m_GridPos.EnableDragAndDrop(FALSE);
	m_GridPos.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_GridPos.SetEditable(TRUE);
	m_GridPos.SetListMode(TRUE);
	m_GridPos.SetTrackFocusCell(FALSE);
	m_GridPos.SetFrameFocusCell(FALSE);		//去除鼠标点选时的边框线  
	// 	m_GridPos.SetColumnCount(0);		//清除默认设置的状态，重新创建	
	// 	m_GridPos.SetRowCount(0);			//设置初始行数量，初始化只有一行
	//	m_GridPos.SetSingleRowSelection();
	m_GridPos.SetColumnCount(pos_col);		//初始列数,按顺序分别为：点位名称,X,Y,Z,数值,点位的说明
	m_GridPos.SetRowCount(pos_num);			//设置初始行数量，初始化只有一行
	m_GridPos.SetFixedRowCount(1);	

	m_GridPos.SetFixedBkColor(RGB(128,128,192));
	m_GridPos.SetGridLines(GVL_BOTH);
	CString l_strTitle[] = {" 点  位  名  称 ",
							"     X     ",
							"     Y     ",
							"     Z     ",
							"  数   值  ",
							"   点   位   说   明   "};
	//序号头宽度
	for (int i=0; i<pos_col; i++)
	{
		m_GridPos.SetGridItemText(0,i,l_strTitle[i]);
		m_GridPos.ExpandColumnsToFit();
	}

	m_GridPos.AutoSizeColumns();
	m_GridPos.ExpandLastColumn();

	m_GridPos.SetColumnResize(TRUE);
	m_GridPos.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<pos_num;i++)
	{
		m_GridPos.SetRowHeight(i,28);	
	}

	//调整风格
	for (UINT i=1;i<pos_num;i++)
	{
		for (UINT j=0;j<pos_col;j++)
		{	
			m_GridPos.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j||5==j)
			{	// 点位名称和点位详细说明不能更改
				m_GridPos.GetCell(i, j)->SetBackClr(RGB(128,192,128));
				m_GridPos.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
		}
	}	

	UpdateGrid();
}


void CDlgJetAdjust::UpdateGrid(void)	// 刷新Grid数据
{
	CString str;
	// 探针测高,界面提示变更
	if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
	{
		g_strPosName[6] = _T("探针测高位");
		g_strPosName[7] = _T("探针针头正好在点触传感器上方");
		g_strPosName[10] = _T("探针十字线");
		g_strPosName[11] = _T("探针针头刚好在十字线上方");
	}

	for(UINT i=1;i<pos_num;i++)
	{
		m_GridPos.SetGridItemText(i, 0, g_strPosName[i*2]);
		m_GridPos.SetGridItemText(i, 5, g_strPosName[i*2+1]);
		switch (i)
		{
		case pos_z_safe:
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_zSafePos);
			m_GridPos.SetGridItemText(i, 4, str);
			break;

		case pos_contact:
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posContactTest.x);
			m_GridPos.SetGridItemText(i, 1, str);
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posContactTest.y);
			m_GridPos.SetGridItemText(i, 2, str);
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posContactTest.z);
			m_GridPos.SetGridItemText(i, 3, str);
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_zContactPos);
			m_GridPos.SetGridItemText(i, 4, str);
			break;

		case pos_laser_base:	
			if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
			{
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_posLaserBasePos.x);
				m_GridPos.SetGridItemText(i, 1, str);
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_posLaserBasePos.y);
				m_GridPos.SetGridItemText(i, 2, str);
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_zLaserBaseVal);
				m_GridPos.SetGridItemText(i, 4, str);
			}
			else
			{
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_posProbeBasePos.x);
				m_GridPos.SetGridItemText(i, 1, str);
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_posProbeBasePos.y);
				m_GridPos.SetGridItemText(i, 2, str);
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_zProbeBaseVal);
				m_GridPos.SetGridItemText(i, 4, str);
			}
			break;

		case pos_camera_cross:
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posCamereCross.x);
			m_GridPos.SetGridItemText(i, 1, str);
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posCamereCross.y);
			m_GridPos.SetGridItemText(i, 2, str);
// 			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posCamereCross.z);
// 			m_GridPos.SetGridItemText(i, 3, str);
			break;

		case pos_laser_cross:	
			if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
			{
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_posLaserCross.x);
				m_GridPos.SetGridItemText(i, 1, str);
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_posLaserCross.y);
				m_GridPos.SetGridItemText(i, 2, str);
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_zLaserCrossVal);
				m_GridPos.SetGridItemText(i, 4, str);
			}
			else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
			{
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_posProbeCross.x);
				m_GridPos.SetGridItemText(i, 1, str);
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_posProbeCross.y);
				m_GridPos.SetGridItemText(i, 2, str);
				str.Format("%.3f", g_pFrm->m_pRobotParam->m_zProbeCrossVal);
				m_GridPos.SetGridItemText(i, 4, str);
			}
			break;

		case pos_jet_adjust:
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posJetAdjust.x);
			m_GridPos.SetGridItemText(i, 1, str);
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posJetAdjust.y);
			m_GridPos.SetGridItemText(i, 2, str);
// 			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posJetAdjust.z);
// 			m_GridPos.SetGridItemText(i, 3, str);
			break;

		case pos_jet_adjust_height:
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posJetAdjust.z);
			m_GridPos.SetGridItemText(i, 4, str);
			break;

		case pos_weight_camera:
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posWeightAdj.x);
			m_GridPos.SetGridItemText(i, 1, str);
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posWeightAdj.y);
			m_GridPos.SetGridItemText(i, 2, str);
// 			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posWeightAdj.z);
// 			m_GridPos.SetGridItemText(i, 3, str);
			break;

		case pos_weight_disp:
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posWeightAdj.x+g_pFrm->m_pSysParam->tOffset[0].x);
			m_GridPos.SetGridItemText(i, 1, str);
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posWeightAdj.y+g_pFrm->m_pSysParam->tOffset[0].y);
			m_GridPos.SetGridItemText(i, 2, str);
// 			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posWeightAdj.z);
// 			m_GridPos.SetGridItemText(i, 3, str);
			break;

		case Pos_weight_height:
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posWeightAdj.z);
			m_GridPos.SetGridItemText(i, 4, str);
			break;

		case pos_clean_camera:	
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_structClean.m_posClean.x);
			m_GridPos.SetGridItemText(i, 1, str);
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_structClean.m_posClean.y);
			m_GridPos.SetGridItemText(i, 2, str);
// 			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posClean.z);
// 			m_GridPos.SetGridItemText(i, 3, str);	
			break;

		case pos_clean_disp:	
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_structClean.m_posClean.x+g_pFrm->m_pSysParam->tOffset[0].x);
			m_GridPos.SetGridItemText(i, 1, str);
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_structClean.m_posClean.y+g_pFrm->m_pSysParam->tOffset[0].y);
			m_GridPos.SetGridItemText(i, 2, str);
// 			str.Format("%.3f", g_pFrm->m_pRobotParam->m_posClean.z);
// 			m_GridPos.SetGridItemText(i, 3, str);
			break;

		case pos_clean_height:	
			str.Format("%.3f", g_pFrm->m_pRobotParam->m_structClean.m_posClean.z);
			m_GridPos.SetGridItemText(i, 4, str);

		default:
			break;
		}
	}	
}

// 喷射4点坐标
bool CDlgJetAdjust::JetFourAdjust(void)
{
	if(g_pFrm->m_pRobotParam->m_nWorkMode == 2)
	{	
		AfxMessageBox(_T("图像模式不允许标定，请选择工具类型"));
		return FALSE;
	}

	double dZDispHeight = 0;
	double tempVal;
	double dVel[4];
	double dAcc[4];
	for (int i=0;i<4;i++)
	{
		dVel[i] = 350;
		dAcc[i] = g_pFrm->m_mtrParamGts[i].m_Acc;//mm/s*s
	}

//	bool Z_UP=true;	

	double zUpHeight = 10;	// 针头点胶时的抬升高度
	int openTime = 0;

	if (g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		CString strHeight;
		if (g_pFrm->m_pRobotParam->m_posJetAdjust.z < 0)
		{
			AfxMessageBox("点胶相对高度值不能小于0");
			return FALSE;
		}

		// 点胶高度:	针头接触传感器高度-点胶相对高度+十字线激光读数-接触传感器激光读数
		if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			dZDispHeight = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_posJetAdjust.z+g_pFrm->m_pRobotParam->m_zLaserCrossVal-g_pFrm->m_pRobotParam->m_zLaserBaseVal;
		}
		else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			dZDispHeight = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_posJetAdjust.z+g_pFrm->m_pRobotParam->m_zProbeCrossVal-g_pFrm->m_pRobotParam->m_zProbeBaseVal;
		}
		else
		{
			return FALSE;
		}

		// 先在中心位置喷射2次
		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,g_pFrm->m_pRobotParam->m_posJetAdjust.x+g_pFrm->m_pSysParam->JetDistanceXY/2, 
			     g_pFrm->m_pRobotParam->m_posJetAdjust.y+g_pFrm->m_pSysParam->JetDistanceXY/2,
				 g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,dZDispHeight, 
			g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
		
		Sleep(200);
		CString strTemp;
		if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
		{
			GetDlgItemText(IDC_DLG_JET_ADJUST_NUM, strTemp);
			g_pFrm->m_pSysParam->nJetPulse = atoi(strTemp);
			g_pFrm->m_Robot->DotDisp(g_pFrm->m_pSysParam->nJetPulse);
		}
		else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
		{
			GetDlgItemText(IDC_DLG_JET_ADJUST_UP_HEIGHT, strTemp);
			zUpHeight = atof(strTemp);
			GetDlgItemText(IDC_DLG_JET_ADJUST_OPEN_TIME, strTemp);
			openTime = atoi(strTemp);
			if (zUpHeight < 0.05)
			{
				AfxMessageBox(_T("针头点胶高度太低!停止针头标定!"));
				return FALSE;
			}

			g_pFrm->m_Robot->JetOutput(1,TRUE);
			Sleep(openTime);
			g_pFrm->m_Robot->JetOutput(1,FALSE);
			g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,dZDispHeight-zUpHeight, 
				g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

		}

		// 开始喷射4点样品
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,g_pFrm->m_pRobotParam->m_posJetAdjust.x,g_pFrm->m_pRobotParam->m_posJetAdjust.y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);

		for (int i=0;i<4;i++)
		{
			if (i==1||i==3)
			{
				tempVal=g_pFrm->m_pRobotParam->m_posJetAdjust.x+g_pFrm->m_pSysParam->JetDistanceXY;
				JetPost[1].x=tempVal;
				JetPost[3].x=tempVal;

				adjustP[1].x = tempVal-g_pFrm->m_pSysParam->tOffset[0].x;
				adjustP[3].x = tempVal-g_pFrm->m_pSysParam->tOffset[0].x;
			}
			else
			{
				tempVal=g_pFrm->m_pRobotParam->m_posJetAdjust.x;
				JetPost[0].x=tempVal;
				JetPost[2].x=tempVal;
				adjustP[0].x = tempVal-g_pFrm->m_pSysParam->tOffset[0].x;
				adjustP[2].x = tempVal-g_pFrm->m_pSysParam->tOffset[0].x;
			}

			if (i==2||i==3)
			{
				tempVal=g_pFrm->m_pRobotParam->m_posJetAdjust.y+g_pFrm->m_pSysParam->JetDistanceXY;
				JetPost[2].y=tempVal;
				JetPost[3].y=tempVal;
				adjustP[2].y = tempVal-g_pFrm->m_pSysParam->tOffset[0].y;
				adjustP[3].y = tempVal-g_pFrm->m_pSysParam->tOffset[0].y;
			}
			else
			{
				tempVal=g_pFrm->m_pRobotParam->m_posJetAdjust.y;
				JetPost[0].y=tempVal;
				JetPost[1].y=tempVal;
				adjustP[0].y = tempVal-g_pFrm->m_pSysParam->tOffset[0].y;
				adjustP[1].y = tempVal-g_pFrm->m_pSysParam->tOffset[0].y;
			}

			g_pFrm->m_Precision.LineMoveXY(CRD1,0,JetPost[i].x,JetPost[i].y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			Sleep(50);      // 增加延时，保证运动到位  以后再进行出胶控制 

			if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
			{		
				g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,dZDispHeight, 
					g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

				g_pFrm->m_Robot->JetOutput(1,TRUE);
				Sleep(openTime);
				g_pFrm->m_Robot->JetOutput(1,FALSE);
				g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,dZDispHeight-zUpHeight, 
					g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
			}
			else if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
			{
				g_pFrm->m_Robot->DotDisp(g_pFrm->m_pSysParam->nJetPulse);
			}
          
			Sleep(50);
		}
		g_pFrm->m_Robot->GotoSafeZpos();

		return true;
	}
	return false;
}

// 移动到 喷射点位置
void CDlgJetAdjust::MoveCarm(int PotID)
{
	//tOffset
	BOOL Z_UP;
	double dValue[4];
	double dVel[4];
	double dAcc[4];

	CButton  *bt1=(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_START);
	bt1->EnableWindow(FALSE);

	for (int i=0;i<4;i++)
	{
		dVel[i] = 50;
		dAcc[i] = g_pFrm->m_mtrParamGts[i].m_Acc;//mm/s*s
	}

	if(PotID == 1)
	{ 
		dValue[2] = g_pFrm->m_pSysParam->tPixelAdjustPos.z;//用图像高度
	}
	else
	{
		if((adjustP[0].z>=0)&&(adjustP[0].z<g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive-10))
            dValue[2] =adjustP[0].z;
		else
            dValue[2] = g_pFrm->m_pSysParam->tPixelAdjustPos.z;
	}

	if (g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		switch (PotID)
		{
		case 1:  // 第一个图像中心大概位置
			dValue[0]=adjustP[0].x;
			dValue[1]=adjustP[0].y;
			Z_UP=TRUE;
			break;
		case 2:
			dValue[0]=adjustP[1].x;//+g_pFrm->m_pSysParam->JetDistanceXY;
			dValue[1]=adjustP[1].y;
			Z_UP=FALSE;
			break;
		case 3:
			dValue[0]=adjustP[2].x;
			dValue[1]=adjustP[2].y;//+g_pFrm->m_pSysParam->JetDistanceXY;
			//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
// 			g_pFrm->m_Precision.GetCurPosMM();
// 			dValue[0]=g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS]-g_pFrm->m_pSysParam->JetDistanceXY;
// 			dValue[1]=g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS]+g_pFrm->m_pSysParam->JetDistanceXY;
 			Z_UP=FALSE;
			break;
		case 4:
			dValue[0]=adjustP[3].x;//+g_pFrm->m_pSysParam->JetDistanceXY;
			dValue[1]=adjustP[3].y;//+g_pFrm->m_pSysParam->JetDistanceXY;
			Z_UP=FALSE;
			//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
// 			g_pFrm->m_Precision.GetCurPosMM();
// 			dValue[0]=g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS]+g_pFrm->m_pSysParam->JetDistanceXY;
// 			dValue[1]=g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
 			break;
		default:
			break;
		}

		if(Z_UP)	
			g_pFrm->m_Robot->GotoSafeZpos();

		g_pFrm->m_Precision.LineMoveXY(CRD1,0,dValue[0],dValue[1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,g_pFrm->m_pRobotParam->m_zSafePos, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
//		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,dValue[2], g_pFrm->m_mtrParam[Z_AXIS].m_Vmax,g_pFrm->m_mtrParam[Z_AXIS].m_Acc,TRUE);

//		GetCarmPos(PotID);//先记录微调之前的偏差值，应该为0
	}

	bt1->EnableWindow(TRUE);
}

bool CDlgJetAdjust::CapCarmoffsetJet(void)//计算平均偏差值
{
	tgPos  offsetP[4];
	double tempx=0.0;
	double tempy=0.0;
	
	CString str;
	for (int i=0;i<4;i++)
	{
		offsetP[i].x=JetPost[i].x-adjustP[i].x;
		offsetP[i].y=JetPost[i].y-adjustP[i].y;

	}
	for (int i=0;i<4;i++)
	{
		tempx+=offsetP[i].x;
		tempy+=offsetP[i].y;
	}
	tempx=tempx/4;
	tempy=tempy/4;

// 	str.Format("%0.4f",tempx);
// 	SetDlgItemText(IDC_DLG_JET_ADJUST_OFFX,str);
// 	str.Format("%0.4f",tempy);
// 	SetDlgItemText(IDC_DLG_JET_ADJUST_OFFY,str);

	g_pFrm->m_pSysParam->tOffset[0].x = tempx;
	g_pFrm->m_pSysParam->tOffset[0].y = tempy;

	CString str1,str2;
#ifdef  DEBUG_SD950
	str1.Format("x:%.4f",g_pFrm->m_pSysParam->tOffset[0].x);
	g_pFrm->AddMsg(str1);
	str2.Format("y:%.4f",g_pFrm->m_pSysParam->tOffset[0].y);
	g_pFrm->AddMsg(str2);
#endif
	return false;
}

void CDlgJetAdjust::OnBnClickedBtEnd()
{
	// TODO: Add your control notification handler code here
	for (int i=1;i<6;i++)
	{
		m_ListFun.Enable(i,FALSE);
		m_ListFun.SetCheck(i,0);
	}
	m_Prog.SetPos(0);

	(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_END)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_START)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_JOG)->EnableWindow(FALSE);	

	// 显示当前偏差
	CString strtemp;
	strtemp.Format("%.4f",g_pFrm->m_pSysParam->tOffset[0].x);   
	SetDlgItemText(IDC_DLG_JET_ADJUST_RX,strtemp);
	strtemp.Format("%.4f",g_pFrm->m_pSysParam->tOffset[0].y);
	SetDlgItemText(IDC_DLG_JET_ADJUST_RY,strtemp);
}

// 获取喷射点图像中心位置并计算和器喷射点位置坐标差值
void CDlgJetAdjust::GetCarmPos(int PosID)
{
	g_pFrm->m_Precision.GetCurPosMM();
	adjustP[PosID-1].x=g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	adjustP[PosID-1].y=g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
//	adjustP[PosID-1].z=g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	double delata_x=0;
	double delata_y=0;
	for (int i=0;i<PosID; i++)
	{
		delata_x += JetPost[i].x-adjustP[i].x;
		delata_y += JetPost[i].y-adjustP[i].y;
	}
	delata_x = delata_x/PosID;
	g_pFrm->m_pSysParam->tOffset[0].x = delata_x;
	delata_y = delata_y/PosID;
	g_pFrm->m_pSysParam->tOffset[0].y = delata_y;

	// 按照计算出的针头图像偏移,计算后续观察点位位置
	for (int i=PosID; i<4; i++)
	{
		adjustP[i].x = JetPost[i].x - g_pFrm->m_pSysParam->tOffset[0].x;
		adjustP[i].y = JetPost[i].y - g_pFrm->m_pSysParam->tOffset[0].y;
	}

	CString str;
	str.Format("%.4f",delata_x);
	SetDlgItemText(IDC_DLG_JET_ADJUST_RX,str);

	str.Format("%.4f",delata_y);
	SetDlgItemText(IDC_DLG_JET_ADJUST_RY,str);

	((CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_START))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_END))->EnableWindow(FALSE);
}

void CDlgJetAdjust::ShowPos(void)
{
// 	CString str;
// 	str.Format("%0.3f", g_pFrm->m_pSysParam->tJetAdjustPos.x);  
// 	SetDlgItemText(IDC_DLG_JET_ADJUST_EX,str);
// 	str.Format("%0.3f", g_pFrm->m_pSysParam->tJetAdjustPos.y);  
// 	SetDlgItemText(IDC_DLG_JET_ADJUST_EY,str);
// 	str.Format("%0.3f", g_pFrm->m_pSysParam->tJetAdjustPos.z);  
// 	SetDlgItemText(IDC_DLG_JET_ADJUST_EZ2,str);
}
void CDlgJetAdjust::OnBnClickedDlgJetAdjustJog()
{
	// TODO: Add your control notification handler code here
	CDlgJogXYZ m_JogXYZ(_T("JOG"));
	if(IDOK==m_JogXYZ.DoModal())
	{
		CButton  *bt1=(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_END);
		CButton  *bt2=(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_START);
		CString  str;

		for (int i=0;i<6;i++)
		{
			if(m_ListFun.GetCheck(i))
				Index=i;
		}
		switch (Index)
		{
		case 0:        
			break;
		case 1:
			GetCarmPos(1);
			//m_Prog.SetPos(20);
			//m_ListFun.SetCheck(1,1);
			//ShowPos();
			break;
		case 2:
			GetCarmPos(2);
			//m_Prog.SetPos(30);
			//m_ListFun.SetCheck(2,1);
			//ShowPos();
			break;
		case 3:
			GetCarmPos(3);
			//m_Prog.SetPos(40);
			//m_ListFun.SetCheck(3,1);
			//ShowPos();
			break;
		case 4:
			GetCarmPos(4);
			//m_Prog.SetPos(50);
			//m_ListFun.SetCheck(4,1);
			//ShowPos();
			break;
		case 5:
			//m_Prog.SetPos(60);
			//m_ListFun.SetCheck(5,1);
			bt2->EnableWindow(TRUE);
			bt1->EnableWindow(FALSE);  
			break;
		default:
			break;
		}
	}
}

void CDlgJetAdjust::OnBnClickedButton1()
{    
	CString  strx,stry;
	GetDlgItemText(IDC_DLG_JET_ADJUST_RX,strx);
	GetDlgItemText(IDC_DLG_JET_ADJUST_RY,stry);

	g_pFrm->m_pSysParam->tOffset[0].x = atof(strx);
	g_pFrm->m_pSysParam->tOffset[0].y = atof(stry);

	g_pFrm->SysParam(FALSE);

	CString str;   // 显示当前偏差
	str.Format("%.4f",g_pFrm->m_pSysParam->tOffset[0].x);   
	SetDlgItemText(IDC_DLG_JET_ADJUST_OFFX,str);
	str.Format("%.4f",g_pFrm->m_pSysParam->tOffset[0].y);
	SetDlgItemText(IDC_DLG_JET_ADJUST_OFFY,str);

	(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_END)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_START)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_DLG_JET_ADJUST_JOG)->EnableWindow(FALSE);
#ifdef  DEBUG_SD950
	CString str1,str2;
	str1.Format("x:%.4f",g_pFrm->m_pSysParam->tOffset[0].x);
	g_pFrm->AddMsg(str1);
	str2.Format("y:%.4f",g_pFrm->m_pSysParam->tOffset[0].y);
	g_pFrm->AddMsg(str2);
#endif
}


//void CDlgJetAdjust::OnBnClickedBtnMovtoJetTestPos()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// //	g_pFrm->m_Robot->GotoSafeZpos();	
// 
// 	double x, y, z;
// 	x = g_pFrm->m_pRobotParam->m_posContactTest.x;
// 	y = g_pFrm->m_pRobotParam->m_posContactTest.y;
// 	z = 0;//g_pFrm->m_pRobotParam->m_posContactTest.z;
// 
// 	
// 	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc, TRUE);
// 	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_Vmin, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
// }


//void CDlgJetAdjust::OnBnClickedBtnSetJetTestPos()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	// 更新位置,校正补偿后的位置
// 	g_pFrm->m_Precision.GetCurPosMM();
// 
// 	g_pFrm->m_pRobotParam->m_posContactTest.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
// 	g_pFrm->m_pRobotParam->m_posContactTest.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
// 	g_pFrm->m_pRobotParam->m_posContactTest.z = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
// 
// 	CString strTemp;
// 	strTemp.Format("%.03f", g_pFrm->m_pRobotParam->m_posContactTest.x);
// 	SetDlgItemText(IDC_EDIT_JET_TEST_X, strTemp);
// 	strTemp.Format("%.03f", g_pFrm->m_pRobotParam->m_posContactTest.y);
// 	SetDlgItemText(IDC_EDIT_JET_TEST_Y, strTemp);
// 	strTemp.Format("%.03f", g_pFrm->m_pRobotParam->m_posContactTest.z);
// 	SetDlgItemText(IDC_EDIT_JET_TEST_Z, strTemp);
// 
// 	g_pFrm->RobotParam(FALSE);
// }

void CDlgJetAdjust::OnBnClickedBtnContactTest()
{
	// TODO: 在此添加控件通知处理程序代码
	short sTemp = 0;
	double x, y, z;
	double dValue = 0.0;

	g_pFrm->m_Robot->GotoSafeZpos();
	x = g_pFrm->m_pRobotParam->m_posContactTest.x;
	y = g_pFrm->m_pRobotParam->m_posContactTest.y;
	z = g_pFrm->m_pRobotParam->m_zSafePos;

	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);

	double curZPos = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
	if (curZPos < g_pFrm->m_pRobotParam->m_zSafePos)
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, g_pFrm->m_pRobotParam->m_zSafePos,g_pFrm->m_mtrParamGts[Z_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc, TRUE);
	
	if (IDOK == g_pFrm->m_Robot->AfxMessageBox("请确认针嘴已到传感器上方,确定后将自动测高", MB_OKCANCEL))
	{
		sTemp = g_pFrm->m_Robot->m_pController->AxisNeedleCheckPort(Z_AXIS, z+60, 2, 100, g_pFrm->m_Robot->inSenseCheckZ, &dValue, FALSE);
		if(sTemp!=RTN_NONE) return;

		sTemp = g_pFrm->m_Robot->m_pController->AxisStop(0x0001<<Z_AXIS,TRUE,TRUE);
		if(sTemp!=RTN_NONE) return;

		if(dValue>0)
		{
			CString str;
			dValue -= g_pFrm->m_pRobotParam->m_zSysHeightOffset;	// 叠加系统偏差
			str.Format("%.3f",dValue);
			m_GridPos.SetGridItemText(pos_contact, 4, str);

			g_pFrm->m_pRobotParam->m_zContactPos = dValue;
		}

		g_pFrm->m_Robot->GotoSafeZpos();

		g_pFrm->RobotParam(FALSE);
		AfxMessageBox("接触测高完成");

		g_pFrm->m_Robot->m_bModifiedTrack = TRUE;
	}
}

//void CDlgJetAdjust::OnBnClickedBtnSetZsafePos()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	g_pFrm->m_Robot->m_pController->GetCurPosMM();
// 	g_pFrm->m_pRobotParam->m_zSafePos = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
// 	CString strTemp;
// 	strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_zSafePos);
// 	SetDlgItemText(IDC_EDIT_Z_SAFE_POS, strTemp);
// 	g_pFrm->RobotParam(FALSE);
// }


//void CDlgJetAdjust::OnBnClickedBtnContactReadHeight()
// {
// 	// TODO: 在此添加控件通知处理程序代码	
// 	double x, y, z;
// 	double dValue = 0.0;
// 
// 	g_pFrm->m_Robot->GotoSafeZpos();
// 	x = g_pFrm->m_pRobotParam->m_posLaserBasePos.x;
// 	y = g_pFrm->m_pRobotParam->m_posLaserBasePos.y;
// 	z = g_pFrm->m_pRobotParam->m_posLaserBasePos.z;
// 	
// 	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_Vmin, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
// 
// 	CFunction::DelayExSec(0.2);
// 
// 	if (!g_pFrm->m_comLaser.ReadHeightData(dValue))
// 	{
// 		AfxMessageBox("读取激光测高数值出错!");
// 		return;
// 	}
// 
// 	CString str;
// 	if(dValue<34 && dValue>26)
// 	{
// 		str.Format("%.3f",dValue);
// 		SetDlgItemText(IDC_EDIT_BASE_LASER_HEIGHT,str);
// 
// 		g_pFrm->m_pRobotParam->m_zLaserBaseVal = dValue;
// 		g_pFrm->RobotParam(FALSE);
// 
// 		AfxMessageBox("激光测高完成");
// 	}	
// 	else
// 	{
// 		str.Format(_T("读取到的高度数值:[%f]  超过了量程[26.0~34.0]!"), dValue);
// 		AfxMessageBox(str);
// 	}
// }


//void CDlgJetAdjust::OnBnClickedBtnMovtoLaserBasePos()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	g_pFrm->m_Robot->GotoSafeZpos();	
// 
// 	double x, y, z;
// 	x = g_pFrm->m_pRobotParam->m_posLaserBasePos.x;
// 	y = g_pFrm->m_pRobotParam->m_posLaserBasePos.y;
// 	z = 0;//g_pFrm->m_pRobotParam->m_posLaserBasePos.z;
// 
// 	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_Vmin, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
// 	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc, TRUE);
// }


//void CDlgJetAdjust::OnBnClickedBtnSetLaserBasePos()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	g_pFrm->m_Precision.GetCurPosMM();
// 
// 	g_pFrm->m_pRobotParam->m_posLaserBasePos.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
// 	g_pFrm->m_pRobotParam->m_posLaserBasePos.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
// 	g_pFrm->m_pRobotParam->m_posLaserBasePos.z = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
// 
// 	CString strTemp;
// 	strTemp.Format("%.03f", g_pFrm->m_pRobotParam->m_posLaserBasePos.x);
// 	SetDlgItemText(IDC_EDIT_JET_TEST_X2, strTemp);
// 	strTemp.Format("%.03f", g_pFrm->m_pRobotParam->m_posLaserBasePos.y);
// 	SetDlgItemText(IDC_EDIT_JET_TEST_Y3, strTemp);
// 	strTemp.Format("%.03f", g_pFrm->m_pRobotParam->m_posLaserBasePos.z);
// 	SetDlgItemText(IDC_EDIT_JET_TEST_Z2, strTemp);
// 
// 	g_pFrm->RobotParam(FALSE);
// }


void CDlgJetAdjust::OnBnClickedBtnMovto()
{
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())
		return;
	// TODO: 在此添加控件通知处理程序代码
	int row_click = -1;
	row_click = m_GridPos.GetFocusCell().row;
	if (row_click<1 || row_click>pos_num)	return;

	CString strTemp;
	double xPos=0, yPos=0, zPos=0;

	switch (row_click)
	{
	case	pos_z_safe:
		return;
	case 	pos_contact:
		g_pFrm->m_Robot->GotoSafeZpos();
		xPos = g_pFrm->m_pRobotParam->m_posContactTest.x;
		yPos = g_pFrm->m_pRobotParam->m_posContactTest.y;
		break;
	case 	pos_laser_base:
		g_pFrm->m_Robot->GotoSafeZpos();
		if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			xPos = g_pFrm->m_pRobotParam->m_posLaserBasePos.x;
			yPos = g_pFrm->m_pRobotParam->m_posLaserBasePos.y;
		}
		else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			xPos = g_pFrm->m_pRobotParam->m_posProbeBasePos.x;
			yPos = g_pFrm->m_pRobotParam->m_posProbeBasePos.y;
		}
		break;
	case 	pos_camera_cross:
		g_pFrm->m_Robot->GotoSafeZpos();
		xPos = g_pFrm->m_pRobotParam->m_posCamereCross.x;
		yPos = g_pFrm->m_pRobotParam->m_posCamereCross.y;
		break;
	case 	pos_laser_cross:
		g_pFrm->m_Robot->GotoSafeZpos();
		if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			xPos = g_pFrm->m_pRobotParam->m_posLaserCross.x;
			yPos = g_pFrm->m_pRobotParam->m_posLaserCross.y;
		}
		else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			xPos = g_pFrm->m_pRobotParam->m_posProbeCross.x;
			yPos = g_pFrm->m_pRobotParam->m_posProbeCross.y;
		}
		break;
	case 	pos_jet_adjust:
		g_pFrm->m_Robot->GotoSafeZpos();
		xPos = g_pFrm->m_pRobotParam->m_posJetAdjust.x;
		yPos = g_pFrm->m_pRobotParam->m_posJetAdjust.y;
		break;
	case 	pos_jet_adjust_height:
		return;
	case	pos_weight_camera:
		g_pFrm->m_Robot->GotoSafeZpos();
		xPos = g_pFrm->m_pRobotParam->m_posWeightAdj.x;
		yPos = g_pFrm->m_pRobotParam->m_posWeightAdj.y;
		break;
	case 	pos_weight_disp:
		g_pFrm->m_Robot->GotoSafeZpos();
		xPos = g_pFrm->m_pRobotParam->m_posWeightAdj.x+g_pFrm->m_pSysParam->tOffset[0].x;
		yPos = g_pFrm->m_pRobotParam->m_posWeightAdj.y+g_pFrm->m_pSysParam->tOffset[0].y;
		if (g_pFrm->m_pRobotParam->m_posWeightAdj.z < 0.5)
		{
			strTemp.Format("称重高度为:[%f],请确认此高度安全后继续" ,g_pFrm->m_pRobotParam->m_posWeightAdj.z);
			if (IDOK != AfxMessageBox(strTemp, MB_OKCANCEL))	return;
		}
		zPos = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_posWeightAdj.z;
		break;
	case 	Pos_weight_height:
		return;
	case 	pos_clean_camera:
		g_pFrm->m_Robot->GotoSafeZpos();
		xPos = g_pFrm->m_pRobotParam->m_structClean.m_posClean.x;
		yPos = g_pFrm->m_pRobotParam->m_structClean.m_posClean.y;
		break;
	case 	pos_clean_disp:
		g_pFrm->m_Robot->GotoSafeZpos();
		xPos = g_pFrm->m_pRobotParam->m_structClean.m_posClean.x+g_pFrm->m_pSysParam->tOffset[0].x;
		yPos = g_pFrm->m_pRobotParam->m_structClean.m_posClean.y+g_pFrm->m_pSysParam->tOffset[0].y;
		if (g_pFrm->m_pRobotParam->m_structClean.m_posClean.z < 0.5)
		{
			strTemp.Format("清洗高度为:[%f],请确认此高度安全后继续" ,g_pFrm->m_pRobotParam->m_structClean.m_posClean.z);
			if (IDOK != AfxMessageBox(strTemp, MB_OKCANCEL))	return;
		}
		zPos = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_structClean.m_posClean.z;
		break;
	case 	pos_clean_height:
		return;
	default:
		break;
	}
	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, xPos, yPos, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);

	if (pos_weight_disp==row_click || pos_clean_disp==row_click)
	{
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, zPos, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc, TRUE);
	}
}


void CDlgJetAdjust::OnBnClickedBtnSet()
{
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())
		return;

	// TODO: 在此添加控件通知处理程序代码
	int row_click = -1;
	row_click = m_GridPos.GetFocusCell().row;
	if (row_click<1 || row_click>pos_num)	return;

	CString strTemp;
	g_pFrm->m_Precision.GetCurPosMM();
	double 
		xPos=g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS],
		yPos=g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS], 
		zPos=g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS],
		dVal=0;

	switch (row_click)
	{
	case pos_z_safe:// z轴安全高度
		if (zPos > 32)
		{
			strTemp.Format("当前将设置的安全高度:[%f]小于32,确认此高度安全后继续", zPos);
			if (IDOK != AfxMessageBox(strTemp, MB_OKCANCEL))  return;
		}
		g_pFrm->m_pRobotParam->m_zSafePos = zPos;
		break;	

	case 	pos_contact:			// 接触测高位置
		g_pFrm->m_pRobotParam->m_posContactTest.x = xPos;
		g_pFrm->m_pRobotParam->m_posContactTest.y = yPos;
		break;

	case 	pos_laser_base:			// 激光测量接触传感器高度的位置
		if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			g_pFrm->m_pRobotParam->m_posLaserBasePos.x = xPos;
			g_pFrm->m_pRobotParam->m_posLaserBasePos.y = yPos;

			CFunction::DelaySec(0.1);
			if (!g_pFrm->m_comLaser.ReadHeightData(dVal))
			{
				AfxMessageBox("设置激光测高位时,读取激光测高数值出错!");
				return;
			}

			if(dVal<34 && dVal>26)
			{
				strTemp.Format("%.3f",dVal);		
				g_pFrm->m_pRobotParam->m_zLaserBaseVal = dVal;		
				AfxMessageBox("设置激光测高位时,激光测高完成");
			}	
			else
			{
				strTemp.Format(_T("读取到的高度数值:[%f]  超过了量程[26.0~34.0]!"), dVal);
				AfxMessageBox(strTemp);
			}
		}
		else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			g_pFrm->m_pRobotParam->m_posProbeBasePos.x = xPos;
			g_pFrm->m_pRobotParam->m_posProbeBasePos.y = yPos;
		}		
		break;

	case 	pos_camera_cross:		// 相机观察十字线中心位置(相机与激光offset校正时用到)
		g_pFrm->m_pRobotParam->m_posCamereCross.x = xPos;
		g_pFrm->m_pRobotParam->m_posCamereCross.y = yPos;
		break;

	case	pos_laser_cross:		// 激光打在十字线中心的位置
		if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			g_pFrm->m_pRobotParam->m_posLaserCross.x = xPos;
			g_pFrm->m_pRobotParam->m_posLaserCross.y = yPos;

			CFunction::DelaySec(0.1);
			if (!g_pFrm->m_comLaser.ReadHeightData(dVal))
			{
				AfxMessageBox("读取激光测高数值出错!");
				return;
			}

			if(dVal<34 && dVal>26)
			{
				strTemp.Format("%.3f",dVal);		
				g_pFrm->m_pRobotParam->m_zLaserCrossVal = dVal;		
				AfxMessageBox("激光测高完成");
			}	
			else
			{
				strTemp.Format(_T("读取到的高度数值:[%f]  超过了量程[26.0~34.0]!"), dVal);
				AfxMessageBox(strTemp);
			}
		}
		else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			g_pFrm->m_pRobotParam->m_posProbeCross.x = xPos;
			g_pFrm->m_pRobotParam->m_posProbeCross.y = yPos;
		}
		break;

	case	pos_jet_adjust:			// 四点校正喷射左上角坐标(针头)
		g_pFrm->m_pRobotParam->m_posJetAdjust.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
		g_pFrm->m_pRobotParam->m_posJetAdjust.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
		break;

	case	pos_jet_adjust_height:	// 四点校正相对高度		
		zPos = m_GridPos.GetItemDataDouble(row_click, 4);
		if (zPos < 0)
		{
			strTemp.Format("当前将设置的四点校正高度:[%f]小于0,请设置一个大于0的数后重新操作!!!");
			AfxMessageBox(strTemp, MB_OKCANCEL);
			return;
		}
		g_pFrm->m_pRobotParam->m_posJetAdjust.z = zPos;
		break;

	case	pos_weight_camera:		// 称重校准位(镜头观察)
		g_pFrm->m_pRobotParam->m_posWeightAdj.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
		g_pFrm->m_pRobotParam->m_posWeightAdj.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
		strTemp.Format(".3f", g_pFrm->m_pRobotParam->m_posWeightAdj.x+g_pFrm->m_pSysParam->tOffset[0].x);
		m_GridPos.SetGridItemText(pos_weight_disp, 1, strTemp);
		strTemp.Format(".3f", g_pFrm->m_pRobotParam->m_posWeightAdj.y+g_pFrm->m_pSysParam->tOffset[0].y);
		m_GridPos.SetGridItemText(pos_weight_disp, 2, strTemp);
		break;

	case	pos_weight_disp:		// 称重校准位(针头)		(与镜头位置联动，修改一个另外一个随之变动)
		g_pFrm->m_pRobotParam->m_posWeightAdj.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS]-g_pFrm->m_pSysParam->tOffset[0].x;
		g_pFrm->m_pRobotParam->m_posWeightAdj.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS]-g_pFrm->m_pSysParam->tOffset[0].y;
		strTemp.Format(".3f", g_pFrm->m_pRobotParam->m_posWeightAdj.x);
		m_GridPos.SetGridItemText(pos_weight_camera, 1, strTemp);
		strTemp.Format(".3f", g_pFrm->m_pRobotParam->m_posWeightAdj.y);
		m_GridPos.SetGridItemText(pos_weight_camera, 2, strTemp);
		break;

	case	Pos_weight_height:		// 称重校准相对高度		
		zPos = m_GridPos.GetItemDataDouble(row_click, 4);
		if (zPos < 1)
		{
			strTemp.Format("当前将设置的称重高度:[%f]小于1,确认此高度安全后继续");
			if (IDOK != AfxMessageBox(strTemp, MB_OKCANCEL))  return;
		}
		g_pFrm->m_pRobotParam->m_posWeightAdj.z = zPos;
		break;

	case	pos_clean_camera:		// 针头清洗位(镜头观察)
		g_pFrm->m_pRobotParam->m_structClean.m_posClean.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
		g_pFrm->m_pRobotParam->m_structClean.m_posClean.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
		strTemp.Format(".3f", g_pFrm->m_pRobotParam->m_structClean.m_posClean.x+g_pFrm->m_pSysParam->tOffset[0].x);
		m_GridPos.SetGridItemText(pos_clean_disp, 1, strTemp);
		strTemp.Format(".3f", g_pFrm->m_pRobotParam->m_structClean.m_posClean.y+g_pFrm->m_pSysParam->tOffset[0].y);
		m_GridPos.SetGridItemText(pos_clean_disp, 2, strTemp);
		break;

	case	pos_clean_disp:			// 针头清洗位(针头)		(与镜头位置联动，修改一个另外一个随之变动)	
		g_pFrm->m_pRobotParam->m_structClean.m_posClean.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS]-g_pFrm->m_pSysParam->tOffset[0].x;
		g_pFrm->m_pRobotParam->m_structClean.m_posClean.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS]-g_pFrm->m_pSysParam->tOffset[0].y;
		strTemp.Format(".3f", g_pFrm->m_pRobotParam->m_structClean.m_posClean.x);
		m_GridPos.SetGridItemText(pos_clean_camera, 1, strTemp);
		strTemp.Format(".3f", g_pFrm->m_pRobotParam->m_structClean.m_posClean.y);
		m_GridPos.SetGridItemText(pos_clean_camera, 2, strTemp);
		break;

	case	pos_clean_height:		// 针头清洗高度
		zPos = m_GridPos.GetItemDataDouble(row_click, 4);
		if (zPos < 1)
		{
			strTemp.Format("当前将设置的清洗高度:[%f]小于1,确认此高度安全后继续");
			if (IDOK != AfxMessageBox(strTemp, MB_OKCANCEL))  return;
		}
		g_pFrm->m_pRobotParam->m_structClean.m_posClean.z = m_GridPos.GetItemDataDouble(row_click, 4);
		break;
	}

	UpdateGrid();
}

void CDlgJetAdjust::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	for (int i=1; i<pos_num; i++)
	{
		switch (i)
		{
		case pos_z_safe:
			g_pFrm->m_pRobotParam->m_zSafePos = m_GridPos.GetItemDataDouble(i, 4);
			break;

		case pos_contact:
			g_pFrm->m_pRobotParam->m_posContactTest.x = m_GridPos.GetItemDataDouble(i, 1);
			g_pFrm->m_pRobotParam->m_posContactTest.y = m_GridPos.GetItemDataDouble(i, 2);
			g_pFrm->m_pRobotParam->m_posContactTest.z = m_GridPos.GetItemDataDouble(i, 3);
			g_pFrm->m_pRobotParam->m_zContactPos = m_GridPos.GetItemDataDouble(i, 4);
			break;
		case pos_laser_base:
			g_pFrm->m_pRobotParam->m_posLaserBasePos.x = m_GridPos.GetItemDataDouble(i, 1);
			g_pFrm->m_pRobotParam->m_posLaserBasePos.y = m_GridPos.GetItemDataDouble(i, 2);
			g_pFrm->m_pRobotParam->m_posLaserBasePos.z = m_GridPos.GetItemDataDouble(i, 3);
			g_pFrm->m_pRobotParam->m_zLaserBaseVal = m_GridPos.GetItemDataDouble(i, 4);
			break;		
		case pos_camera_cross:
			g_pFrm->m_pRobotParam->m_posCamereCross.x = m_GridPos.GetItemDataDouble(i, 1);
			g_pFrm->m_pRobotParam->m_posCamereCross.y = m_GridPos.GetItemDataDouble(i, 2);
			g_pFrm->m_pRobotParam->m_posCamereCross.z = m_GridPos.GetItemDataDouble(i, 3);
			break;		
		case pos_laser_cross:	
			g_pFrm->m_pRobotParam->m_posLaserCross.x = m_GridPos.GetItemDataDouble(i, 1);
			g_pFrm->m_pRobotParam->m_posLaserCross.y = m_GridPos.GetItemDataDouble(i, 2);
			g_pFrm->m_pRobotParam->m_posLaserCross.z = m_GridPos.GetItemDataDouble(i, 3);
			g_pFrm->m_pRobotParam->m_zLaserCrossVal = m_GridPos.GetItemDataDouble(i, 4);
			break;
		case pos_jet_adjust:	
			g_pFrm->m_pRobotParam->m_posJetAdjust.x = m_GridPos.GetItemDataDouble(i, 1);
			g_pFrm->m_pRobotParam->m_posJetAdjust.y = m_GridPos.GetItemDataDouble(i, 2);
			//g_pFrm->m_pRobotParam->m_posJetAdjust.z = m_GridPos.GetItemDataDouble(i, 3);
			break;
		case  pos_jet_adjust_height:
			g_pFrm->m_pRobotParam->m_posJetAdjust.z = m_GridPos.GetItemDataDouble(i, 4);
			break;
		case pos_weight_camera:
			g_pFrm->m_pRobotParam->m_posWeightAdj.x = m_GridPos.GetItemDataDouble(i, 1);
			g_pFrm->m_pRobotParam->m_posWeightAdj.y = m_GridPos.GetItemDataDouble(i, 2);
			//g_pFrm->m_pRobotParam->m_posWeightAdj.z = m_GridPos.GetItemDataDouble(i, 3);	
			break;
		case pos_weight_disp:
			break;
		case Pos_weight_height:
			g_pFrm->m_pRobotParam->m_posWeightAdj.z = m_GridPos.GetItemDataDouble(i, 4);
			break;
		case pos_clean_camera:	
			g_pFrm->m_pRobotParam->m_structClean.m_posClean.x = m_GridPos.GetItemDataDouble(i, 1);
			g_pFrm->m_pRobotParam->m_structClean.m_posClean.y = m_GridPos.GetItemDataDouble(i, 2);
			break;
		case pos_clean_disp:	
			break;
		case pos_clean_height:
			g_pFrm->m_pRobotParam->m_structClean.m_posClean.z = m_GridPos.GetItemDataDouble(i, 4);
			break;
		}
	}

	GetDlgItemText(IDC_DLG_JET_ADJUST_RX, strTemp);
	g_pFrm->m_pSysParam->tOffset[0].x = atof(strTemp);
	GetDlgItemText(IDC_DLG_JET_ADJUST_RY, strTemp);
	g_pFrm->m_pSysParam->tOffset[0].y = atof(strTemp);
	GetDlgItemText(IDC_DLG_JET_ADJUST_NUM, strTemp);
	g_pFrm->m_pSysParam->nJetPulse = atoi(strTemp);
	
	g_pFrm->SysParam(FALSE);
	g_pFrm->RobotParam(FALSE);
}

void CDlgJetAdjust::OnBnClickedBtnReadLaser()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())
		return;
	int row_click = -1;
	row_click = m_GridPos.GetFocusCell().row;
	if ((row_click<1) || (row_click>pos_num))							return;
	if (row_click != pos_laser_base && row_click != pos_laser_cross)
	{
		AfxMessageBox(_T("请选择需要测量的点位！"));
		return;
	}

	double dVal = 0;
	CString strTemp = "";

	if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
	{
		if (!g_pFrm->m_comLaser.ReadHeightData(dVal))
		{
			AfxMessageBox("读取激光测高数值出错!");
			return;
		}
		if(dVal>34 || dVal<26)
		{
			strTemp.Format(_T("读取到的高度数值:[%f]  超过了量程[26.0~34.0]!"), dVal);
			AfxMessageBox(strTemp);
		}
	
		if (row_click == pos_laser_base)
			g_pFrm->m_pRobotParam->m_zLaserBaseVal = dVal;	
		else if (row_click == pos_laser_cross)
			g_pFrm->m_pRobotParam->m_zLaserCrossVal = dVal;	

		AfxMessageBox("激光测高完成");
	}
	else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
	{
		g_pFrm->m_Robot->ExOutput(_T("探针气缸电磁阀"), TRUE);
		CFunction::DelaySec(1);
		double dPos = g_pFrm->m_pRobotParam->m_zContactPos-2;	// 探针测高极限位置
		double dVel = g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLSpd;
		short sRtn = g_pFrm->m_Robot->m_pController->AxisProbeCheck(Z_AXIS, dPos, dVel, 10, &dVal);
		g_pFrm->m_Robot->ExOutput(_T("探针气缸电磁阀"), FALSE);

		sRtn = g_pFrm->m_Robot->GotoSafeZpos();

		if (row_click == pos_laser_base)
			g_pFrm->m_pRobotParam->m_zProbeBaseVal = dVal;	
		else if (row_click == pos_laser_cross)
			g_pFrm->m_pRobotParam->m_zProbeCrossVal = dVal;	

		AfxMessageBox("探针测高完成");
	}

	UpdateGrid();
}

static uint probeOpIndex = 0;
void CDlgJetAdjust::OnBnClickedBtnProbeSwitch()
{
	// TODO: 在此添加控件通知处理程序代码
	if (probeOpIndex & 1)
		g_pFrm->m_Robot->ExOutput(_T("探针气缸电磁阀"), TRUE);
	else
		g_pFrm->m_Robot->ExOutput(_T("探针气缸电磁阀"), FALSE);

	probeOpIndex++;
}


void CDlgJetAdjust::OnGridDbClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{ 
//	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
//	TRACE(_T("DbClicked on 输入口 row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	OnBnClickedBtnMovto();
}
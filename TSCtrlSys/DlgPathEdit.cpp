// DlgPathEdit.cpp : 实现文件
//
#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgPathEdit.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include <math.h>

#include "./Smooth/PointGE.h"

#include "DLgSpeed.h"
#include "DlgRepeat.h"
#include "DlgAxisPos.h"
#include "DlgNote.h"
#include "DlgGoSub.h"
#include "DlgGoto.h"
#include "DlgLoop.h"
#include "DlgIOSet.h"
#include "DlgIOCtrl.h"
#include "DlgLineParam.h"
#include "DlgDelay.h"
#include "DlgZSame.h"
#include "DlgZClearance.h"
#include "DlgRepeatZheight.h"
#include "DlgPointParam.h"
#include "DlgLineEnd.h"
#include "DlgPointEnd.h"
#include "DlgCircle.h"
#include "DlgDispenser.h"
#include "DlgStandBy.h"
#include "DlgTool.h"
#include "DlgZSet.h"
#include "DlgJetParam.h"
#include "DlgHEndPoint.h"
#include "DlgHStartPoint.h"
#include "DlgHWorkPoint.h"
#include "DlgHDot.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern tgCmdLine tTempCmdPoint;//临时命令行
//////////////////////////////////////////////////////////////////////////
extern CString szCmdline[];// = 
// {
// 	"NONE",					//CMD_NONE=0,
// 	"MARK [ ╋ ]",			//Mark设置 ,tgPos[2],int iMarkNum
// 	"空点",					// 
// 	"单点 [ ● ]",			//CMD_POINT_DISPENSE,//单点点胶,tgPos[1]
// 	"线段起点 [●----]",	//CMD_LINE_START,//线段点胶起点tgPos[1]
// 	"线段中点 [--●--]",	//CMD_LINE_PASSING,// 线段点胶中间点tgPos[1]
// 	"圆弧中点 [--＾--]",	//CMD_LINE_ARC,//圆弧点胶 tgPos[1]
// 	"线段终点 [----●]",	//CMD_LINE_END,//线段点胶结束点,tgPos[1]
// 	"圆形点胶 [ ○ ]",      //CMD_CIRCLE,//圆形点胶 1:圆心+直径, tgPos[1], double diameter, 2:3点画圆,tgPos[3]
// 	"点胶阀",				//CMD_DISPENSER,//点胶阀开/关 BOOL On/Off
// 	"跳转",					//CMD_GOTO_ADDRESS,//跳转指令 int iIndex
// 	"X方向阵列",            //CMD_REPEATX,//X方向阵列 int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
// 	"Y方向阵列",            //CMD_REPEATY,//Y方向阵列 int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
// 	"涂覆",					//CMD_BRUSH_AREA,//涂覆,
// 	"调用子函数",			//CMD_CALL_SUBROUTINE,//调子程序
// 	"调用程序",
// 	"标贴",					//CMD_CALL_PROGRAM,//调用程序
// 	"    ",					//CMD_NULL_LINE,//空行
// 	//"等待输入",			//CMD_SET_INPUT,//设置IO状态
// 	//"输出",				//CMD_SET_OUTPUT,//设置IO状态
// 	"S设置IO",
// 	"延时",					//CMD_DELAY,//等待输入的秒数后继续往下执行纪录点。
// 	"移动到待机位",
// 	"停止位",				//CMD_STOP_POINT,//针头将移至此特定位置并且停止移动，直到使用者按下机台面板上的START按钮后继续往下执行纪录点
// 	"待机位",               //CMD_HOME,//X, Y, Z轴将回归软件原点(等待点)后继续往下执行纪录点。
// 	"循环",					//CMD_LOOP,//通常将循环执行地址设为1，当教导一组程序完成后，可利用此功能来设定运作次数
// 	"空点",					//CMD_DUNNY,//设定点胶针头实际所在的XYZ 坐标为空点，并记录于目前的纪录点位置。通常应用于避开工件的阻挡。
// 	"标号",					//CMD_NOTE,
// 	"清洗",					//清洗喷头
// 	"激光测高[♂]",			//激光测高度",
// 	"称重打点[§●]",       // 称重打点
// 	"称重划线[§----]",		// 称重划线
// 	"矩形 [□]",
// 	"选择阀",
// 	"激光测高阵列X",
// 	"激光测高阵列Y",
// 	"单点测试",
// 	"备用",
// 	"程序结束",				//CMD_END_PROGRAM
// 
// 	"线段点胶速度",			//PARAM_LINE_SPEED,//线段点胶速度
// 	"线段参数",				//PARAM_LINE_SETUP,//线段点胶参数设置
// 	"线段结束点",			//PARAM_LINE_END,//线段点胶结束设置
// 	"单点参数设置",			//PARAM_POINT_SETUP,//单点点胶参数设置
// 	"单点结束设置",			//PARAM_POINT_END,//单点点胶结束设置
// 	"Z轴提升高度",			//PARAM_Z_CLEARANCE,//Z轴提高高度设置 
// 	"Z轴极限值",			//PARAM_Z_LIMIT,//Z轴极限值设置 
// 	"XY空跑速度",			//PARAM_XY_VEL,//XY轴空跑移动速度
// 	"Z空跑速度",			//PARAM_Z_VEL,//Z轴空跑移动速度
// 	"Home点",				//PARAM_HOME_POSITION,//设置RUN模式时点胶结束针头停留位置
// 	"Z轴高度一致",			//PARAM_SET_Z_SAME
// 	"Z轴点胶高度",			// Z轴点胶高度
// 	"喷射参数",				// 喷射参数
// 	"称重矩形[§□]",		// 称重矩形
// 	//
// 	"极速喷射起点",			// 飞胶起点
// 	"极速喷射点",
// 	"极速喷射结束点",
// 	"分段加工符",
//     "Z轴高度设置",    //
//     "拼版加工符",    // 
// 	"基板夹角标定", 
// 	"翻板",
// };
extern CString szCmdlineEN[];// = 
// {
// 	"NONE",					//CMD_NONE=0,
// 	"MARK [ ╋ ]",			//Mark设置 ,tgPos[2],int iMarkNum
// 	"NonePoint",			// 
// 	"PointDisp [ ● ]",		//CMD_POINT_DISPENSE,//单点点胶,tgPos[1]
// 	"LineStart [●----]",	//CMD_LINE_START,//线段点胶起点tgPos[1]
// 	"LinPassing [--●--]",	//CMD_LINE_PASSING,// 线段点胶中间点tgPos[1]
// 	"LineArc [--＾--]",		//CMD_LINE_ARC,//圆弧点胶 tgPos[1]
// 	"LineEnd [----●]",		//CMD_LINE_END,//线段点胶结束点,tgPos[1]
// 	"Circle [ ○ ]",			//CMD_CIRCLE,//圆形点胶 1:圆心+直径, tgPos[1], double diameter, 2:3点画圆,tgPos[3]
// 	"Dispense",				//CMD_DISPENSER,//点胶阀开/关 BOOL On/Off
// 	"Goto",					//CMD_GOTO_ADDRESS,//跳转指令 int iIndex
// 	"RepeatX",				//CMD_REPEATX,//X方向阵列 int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
// 	"RepentY",				//CMD_REPEATY,//Y方向阵列 int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
// 	"BrushArea",			//CMD_BRUSH_AREA,//涂覆,
// 	"CallSubFun.",			//CMD_CALL_SUBROUTINE,//调子程序
// 	"CallSubPro.",
// 	"LABEL",				//CMD_CALL_PROGRAM,//调用程序
// 	"    ",					//CMD_NULL_LINE,//空行
// 	//"等待输入",			//CMD_SET_INPUT,//设置IO状态
// 	//"输出",				//CMD_SET_OUTPUT,//设置IO状态
// 	"SetIO",
// 	"Delay",				//CMD_DELAY,//等待输入的秒数后继续往下执行纪录点。
// 	"GotoStandBy",
// 	"StopPoint",			//CMD_STOP_POINT,//针头将移至此特定位置并且停止移动，直到使用者按下机台面板上的START按钮后继续往下执行纪录点
// 	"Home",					//CMD_HOME,//X, Y, Z轴将回归软件原点(等待点)后继续往下执行纪录点。
// 	"Loop",					//CMD_LOOP,//通常将循环执行地址设为1，当教导一组程序完成后，可利用此功能来设定运作次数
// 	"DunnySpeed",			//CMD_DUNNY,//设定点胶针头实际所在的XYZ 坐标为空点，并记录于目前的纪录点位置。通常应用于避开工件的阻挡。
// 	"Note",					//CMD_NOTE,
// 	"Wash",					//清洗喷头
// 	"Laser_Hight[♂]",		//激光测高度",
// 	"POT[§●]",			// 称重打点
// 	"WCLine[§----]",		// 称重划线
// 	"Rect  [□]",
// 	"SelectJet",
// 	"Laser_RepeatX",
// 	"Laser_RepeatY",
// 	"SingleDispTest",
// 	"备用",
// 	"EndProgram",			//CMD_END_PROGRAM
// 
// 	"LineSpeed",			//PARAM_LINE_SPEED,//线段点胶速度
// 	"LineSetup",			//PARAM_LINE_SETUP,//线段点胶参数设置
// 	"LinEnd",				//PARAM_LINE_END,//线段点胶结束设置
// 	"PointSetup",			//PARAM_POINT_SETUP,//单点点胶参数设置
// 	"PointEnd",				//PARAM_POINT_END,//单点点胶结束设置
// 	"Z_Clearance",			//PARAM_Z_CLEARANCE,//Z轴提高高度设置 
// 	"Z_Limit",				//PARAM_Z_LIMIT,//Z轴极限值设置 
// 	"XY_V0",				//PARAM_XY_VEL,//XY轴空跑移动速度
// 	"Z_V0",					//PARAM_Z_VEL,//Z轴空跑移动速度
// 	"HomePos",				//PARAM_HOME_POSITION,//设置RUN模式时点胶结束针头停留位置
// 	"Z_Same",				//PARAM_SET_Z_SAME
// 	"Z_DispPos",			// Z轴点胶高度
// 	"JetPara",				// 喷射参数
// 	"WCRect[§□]",			// 称重矩形
// 	"Reverse Board",		// 翻板指令
// };

IMPLEMENT_DYNAMIC(CDlgPathEdit, CDialogEx)

	CDlgPathEdit::CDlgPathEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPathEdit::IDD, pParent)
{
	m_bInsertCmd = FALSE;
	m_pMsgList = NULL;
	m_nClickRow = 0;
	m_nClickCol = 0;
}

CDlgPathEdit::~CDlgPathEdit()
{
	FreeCmdList();
	m_Tls.RemoveAll();
}

void CDlgPathEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_EDIT0, m_btnEdit[0]);
	DDX_Control(pDX, IDC_BUTTON_EDIT1, m_btnEdit[1]);
	DDX_Control(pDX, IDC_BUTTON_EDIT2, m_btnEdit[2]);
	DDX_Control(pDX, IDC_BUTTON_EDIT3, m_btnEdit[3]);
	DDX_Control(pDX, IDC_BUTTON_EDIT4, m_btnEdit[4]);

	DDX_Control(pDX, IDC_BUTTON_EDIT_SINGLE, m_btnEdit[5]);
	DDX_Control(pDX, IDC_BUTTON_EDIT_ALL, m_btnEdit[6]);

	DDX_Control(pDX, IDC_BTN_REVIEW_F, m_btnEdit[7]);
	DDX_Control(pDX, IDC_BTN_REVIEW_B, m_btnEdit[8]);

	DDX_Control(pDX, IDC_BTN_SAVE, m_btnEdit[9]);

	DDX_Control(pDX, IDC_EDIT_MODIFY, m_editModify);

	DDX_Control(pDX, IDC_LIST_PATH, m_wndList);
	DDX_Control(pDX, IDC_COMBO_MODIFY, m_CtrlComboBoxModify);
}


BEGIN_MESSAGE_MAP(CDlgPathEdit, CDialogEx)	
	ON_COMMAND_RANGE(IDC_BUTTON_EDIT0, IDC_BUTTON_EDIT8, &CDlgPathEdit::OnCmdType)
	ON_COMMAND_RANGE(ID_CMD_MARK, ID_PARAM_HOME, &CDlgPathEdit::OnCmdType)
	ON_COMMAND_RANGE(IDS_TOOL_DOT,IDS_TOOL_LISTEDIT, &CDlgPathEdit::OnCmdType) //画图工具栏按钮响应
	ON_COMMAND_RANGE(ID_CMD_SUPERSTART, ID_CMD_BORATARC, &CDlgPathEdit::OnCmdType)
	ON_COMMAND_RANGE(ID_CMD_ZHEIGHT, ID_CMD_ZHEIGHT, &CDlgPathEdit::OnCmdType)
	ON_COMMAND_RANGE(ID_CMD_REV_BOARD, ID_CMD_REV_BOARD, &CDlgPathEdit::OnCmdType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PATH, &CDlgPathEdit::OnClickListCmd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PATH, &CDlgPathEdit::OnDblclkListCmd)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PATH, &CDlgPathEdit::OnRclickListCmd)
	ON_BN_CLICKED(IDC_BTN_SHOWPATH, &CDlgPathEdit::OnBnClickedBtnShowpath)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_SINGLE, &CDlgPathEdit::OnBnClickedButtonEditSingle)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_ALL, &CDlgPathEdit::OnBnClickedButtonEditAll)
	ON_EN_KILLFOCUS(IDC_EDIT_MODIFY, &CDlgPathEdit::OnKillfocusEditModify)
	ON_CBN_KILLFOCUS(IDC_COMBO_MODIFY, &CDlgPathEdit::OnKillfocusCcomboBox)
	ON_BN_CLICKED(IDC_BTN_REVIEW_F, &CDlgPathEdit::OnBnClickedBtnReviewF)
	ON_BN_CLICKED(IDC_BTN_REVIEW_B, &CDlgPathEdit::OnBnClickedBtnReviewB)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgPathEdit::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_PARAM, &CDlgPathEdit::OnBnClickedBtnParam)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CDlgPathEdit
			     
///////////////  //////////////////////////////////////////////////////////////
// CDlgPathEdit   消息处理程序
// int CDlgPathEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
// {
// 	if (CWnd::OnCreate(lpCreateStruct) == -1)
// 		return -1;
// 
// 	CRect rectDummy(0, 0, 0, 0);
// // 	m_wndShotCutEdit.Create(IDD_DLG_SHOTCUT, this);
// // 
// // 	m_wndList.Create(WS_VISIBLE|LVS_REPORT|LVS_SHOWSELALWAYS, CRect(0,0,10,10), this, IDS_COMMAND_LIST);
// // 	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT/*|LVS_EX_CHECKBOXES*/);
// // 	m_wndList.InsertColumn(0, "序号", 0, 40);
// // 	m_wndList.InsertColumn(1, "类型", 0, 130);
// // 	m_wndList.InsertColumn(2, "参数", 0, 1500);
// 
// 	return 0;
// }

// void CDlgPathEdit::OnSize(UINT nType, int cx, int cy)
// {
// 	CWnd::OnSize(nType, cx, cy);
// 
// // 	CRect rc;
// // 	g_pView->GetWindowRect(rc);
// //	MoveWindow(rc.right, rc.top, 400, rc.Height());
// 
// 	if (m_wndShotCutEdit.GetSafeHwnd() != NULL)
// 	{	
// 		CRect rect;
// 		m_wndShotCutEdit.GetClientRect(rect);
// 		int iHeight = cy;
// 		m_wndShotCutEdit.SetWindowPos(NULL, 0, iHeight-rect.Height(), cx, rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
// 
// 		m_wndList.MoveWindow(0, 0, cx, iHeight-rect.Height()-48);
// //		GetDlgItem(IDC_BUTTON_EDIT_DOT)->MoveWindow(0,iHeight-rect.Height()-48, 48, 48);
// 	}
// }

void CDlgPathEdit::OnDblclkListCmd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	if(theApp.m_SysUser.m_CurUD.level<=GM_PERSONNEL)
	{
		AfxMessageBox("请登录管理员权限！");
		return;
	}

	int nSel = GetCmdCurSel();
	if (nSel<0)		return;
	POSITION posIndex = m_rawList.FindIndex(nSel);
	tgCmdLine &pLine = m_rawList.GetAt(posIndex);

	if (GetAsyncKeyState(VK_LSHIFT))
	{
		ModifiedCmd(nSel);   // 修改指令 
		UpdateCmdListBox(nSel);
	}
	else 
	{
		if (IsTrackPoint(pLine) && (pLine.tType!=CMD_ZMETER))
		{
			g_pFrm->m_Robot->GotoSafeZpos();
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,pLine.pDblValue[0],pLine.pDblValue[1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		}
		else
		{
			ModifiedCmd(nSel);   // 修改指令 
			UpdateCmdListBox(nSel);
		}
	}
	
	*pResult=0;
}

void CDlgPathEdit::OnRclickListCmd(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	UNREFERENCED_PARAMETER(pNMHDR);

	if(theApp.m_SysUser.m_CurUD.level<=GM_PERSONNEL)
	{
		AfxMessageBox("请登录管理员权限！");
		return;
	}

	ShowTrackPopupMenu(1);
	*pResult=1;
}

void CDlgPathEdit::ShowTrackPopupMenu(int MenuID)
{
	CMenu menu;
	if(g_pFrm->m_pSysParam->nLanguageType == 0)
	{// 中文
		switch(MenuID)
		{ 
		case 0:
			menu.LoadMenu(IDR_MENU_CMD_INSERT_PARAM_CN);
			m_bInsertCmd = TRUE;
			break;
		case 1:
			if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
				menu.LoadMenu(IDR_MENU_NEW); 
			else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
				menu.LoadMenu(IDR_MENU_NEW_NEEDLE);
			//menu.LoadMenu(IDR_MENU_CMD_INSERT_CN);
			m_bInsertCmd = TRUE;
			break;
		case 2:
			menu.LoadMenu(IDR_MENU_CMD_CN);
			m_bInsertCmd = FALSE;
			break;
		default:
			break;
		}
	}
	if(g_pFrm->m_pSysParam->nLanguageType == 1)
	{// 英文
		switch(MenuID)
		{ 
		case 0:
			menu.LoadMenu(IDR_MENU_CMD_INSERT_PARAM_EN);
			m_bInsertCmd = TRUE;
			break;
		case 1:
			if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
				menu.LoadMenu(IDR_MENU_NEW); 
			else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
				menu.LoadMenu(IDR_MENU_NEW_NEEDLE);
			//menu.LoadMenu(IDR_MENU_CMD_INSERT_EN);
			m_bInsertCmd = TRUE;
			break;
		case 2:
			menu.LoadMenu(IDR_MENU_CMD_EN);
			m_bInsertCmd = FALSE;
			break;
		default:
			break;
		}
	}
	if(g_pFrm->m_pSysParam->nLanguageType == 3)
	{// 韩文
		switch(MenuID)
		{ 
		case 0:
			menu.LoadMenu(IDR_MENU_CMD_INSERT_PARAM);
			m_bInsertCmd = TRUE;
			break;
		case 1:
			if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
				menu.LoadMenu(IDR_MENU_NEW); 
			else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
				menu.LoadMenu(IDR_MENU_NEW_NEEDLE);
			//menu.LoadMenu(IDR_MENU_CMD_INSERT);
			m_bInsertCmd = TRUE;
			break;
		case 2:
			menu.LoadMenu(IDR_MENU_CMD_CN);
			m_bInsertCmd = FALSE;
			break;
		default:
			break;
		}
	}
	CPoint p;
	GetCursorPos(&p);

	if(!g_pView->m_bImgInit)
		menu.EnableMenuItem(ID_CMD_MARK,MF_DISABLED|MF_GRAYED);

// 	if(0==g_pFrm->m_pSysParam->bBalanceEnable)
// 	{
// 		menu.EnableMenuItem(IDC_BUTTON_EDIT6,MF_DISABLED|MF_GRAYED);
// 		menu.EnableMenuItem(IDC_BUTTON_EDIT7,MF_DISABLED|MF_GRAYED);
// 		menu.EnableMenuItem(IDC_BUTTON_EDIT8,MF_DISABLED|MF_GRAYED);
// 	}
	
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, p.x+20, p.y, this);
}

void CDlgPathEdit::OnClickListCmd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pResult);
	if(theApp.m_SysUser.m_CurUD.level<=GM_PERSONNEL)
	{
		AfxMessageBox("请登录管理员权限！");
		return;
	}

	NM_LISTVIEW * pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nClickRow = pNMListView->iItem;
	m_nClickCol = pNMListView->iSubItem;

	if (0==m_nClickCol || 1==m_nClickCol || 6==m_nClickCol)
		return;

	POSITION posIndex = m_rawList.FindIndex(m_nClickRow);
	tgCmdLine &pLine = m_rawList.GetAt(posIndex);

	if ((2==m_nClickCol||3==m_nClickCol)&&(CMD_ZMETER==pLine.tType))
		return;
	if ((2==m_nClickCol)&&(CMD_SUPERSTART==pLine.tType))
		return;

 	if (IsTrackPoint(pLine) && 7!=m_nClickCol)
	{	
		CString csCellData;
		CRect rcCellRect;
		m_editModify.SetParent(&m_wndList);
		csCellData = m_wndList.GetItemText(m_nClickRow, m_nClickCol);  
		m_wndList.GetSubItemRect(m_nClickRow, m_nClickCol, LVIR_LABEL, rcCellRect);
		rcCellRect.SetRect(rcCellRect.left-2, rcCellRect.top-2, rcCellRect.right+2, rcCellRect.bottom+2);  
		m_editModify.MoveWindow(&rcCellRect);  
		m_editModify.ShowWindow(SW_SHOW);  
		//m_ctrlComboCell.SetWindowText(csCellData); //当显示Combo Box控件时里面的内容为原来的内容,不然是空白的  
		m_editModify.SetFocus(); //设置焦点
		m_editModify.SetWindowText(csCellData);
	}

//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	TRACE("行：%d，列：%d\n", pNMListView->iItem, pNMListView->iSubItem);
	if (pNMListView->iItem==-1)//点击到非工作区
	{
		if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
		{
			DistroyCcombobox(&m_wndList, &m_CtrlComboBoxModify, e_Item, e_SubItem);
			haveccomboboxcreate = false;
		}
	}
	else if (pNMListView->iSubItem != 7)		//如果不是参数选项，在单元格处生成下拉列表项
	{
		if (haveccomboboxcreate == true)		//如果之前创建了编辑框就销毁掉
		{
			DistroyCcombobox(&m_wndList, &m_CtrlComboBoxModify, e_Item, e_SubItem);
			haveccomboboxcreate = false;
		}
	}
	else
	{
		if (haveccomboboxcreate == true)
		{
			// 如果点中的单元格不是之前创建好的
			if (!(e_Item == pNMListView->iItem && e_SubItem == pNMListView->iSubItem))  
			{
				DistroyCcombobox(&m_wndList, &m_CtrlComboBoxModify, e_Item, e_SubItem);
				haveccomboboxcreate = false;
				CreateCcombobox(pNMListView, &m_CtrlComboBoxModify, e_Item, e_SubItem, haveccomboboxcreate);//创建编辑框
				
				CString strTemp;
				if (IsParamPoint(pLine))
				{
					for (int i=0; i<10; i++)
					{
						strTemp.Format("Dot%2d",i+1);
						m_CtrlComboBoxModify.AddString(strTemp);
					}
				} 
				else
				{
					for (int i=0; i<10; i++)
					{
						strTemp.Format("Line%2d",i+1);
						m_CtrlComboBoxModify.AddString(strTemp);
					}
				}

				m_CtrlComboBoxModify.ShowDropDown();//自动下拉
			}
			else//点中的单元格是之前创建好的
			{
 				m_CtrlComboBoxModify.SetFocus();//设置为焦点 
			}
		}
		else
		{
			e_Item = pNMListView->iItem;			//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			e_SubItem = pNMListView->iSubItem;		

			// 第一次
			static int i = 1;
			if (i==1)
			{
				DistroyCcombobox(&m_wndList, &m_CtrlComboBoxModify, e_Item, e_SubItem);
				haveccomboboxcreate = false;
				i++;
			}
//			DistroyCcombobox(&m_wndList, &m_CtrlComboBoxModify, e_Item, e_SubItem);
//			haveccomboboxcreate = true;

			CreateCcombobox(pNMListView, &m_CtrlComboBoxModify, e_Item, e_SubItem, haveccomboboxcreate);		// 创建编辑框
			
			CString strTemp;
			if (IsParamPoint(pLine))
			{
				for (i=0; i<10; i++)
				{
					strTemp.Format("Dot%2d",i+1);
					m_CtrlComboBoxModify.AddString(strTemp);
				}
			} 
			else
			{
				for (i=0; i<10; i++)
				{
					strTemp.Format("Line%2d",i+1);
					m_CtrlComboBoxModify.AddString(strTemp);
				}
			}
			
			
			m_CtrlComboBoxModify.ShowDropDown();			// 自动下拉
		} 
	}
	*pResult = 0;

}

// 检测当前列表中是否有Mark指令
BOOL CDlgPathEdit::CheckMark()
{
	for(int i=0; i<m_rawList.GetCount(); i++)
	{
		POSITION pos=m_rawList.FindIndex(i);
		if(m_rawList.GetAt(pos).tType == CMD_MARK)
			return TRUE;
	}
	return FALSE;
}
void CDlgPathEdit::UpdateCmdListBox(int iSelIndex)
{
	GetDlgItem(IDC_BTN_REVIEW_F)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_REVIEW_B)->ShowWindow(SW_HIDE);
	// 重新统计胶重
	m_dGWTotal = 0;
	m_dGWFront = 0;
	m_dGWBack  = 0;
	bool bFront = true;
 	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST_PATH);
	CString strTemp, strText;
	pList->SetFocus();
	pList->DeleteAllItems();

	POSITION pos;
	//////////////////////////////////////////////////////////////////////////
	m_wndList.SetRedraw(FALSE);
	for(int i=0; i<m_rawList.GetCount(); i++)
	{
		pos = m_rawList.FindIndex(i);
		tTempCmdPoint = m_rawList.GetAt(pos);

		strTemp.Format("%04d", i);
		pList->InsertItem(i, strTemp);
		pList->SetItemText(i, 1, szCmdline[tTempCmdPoint.tType]);
		
		if(tTempCmdPoint.tType == CMD_NOTE /*|| tTempCmdPoint.tType == CMD_LABEL*/ || tTempCmdPoint.tType == CMD_NULL_LINE)
		{
			strText = CString(tTempCmdPoint.pszNote);
		}
		else if(tTempCmdPoint.tType==CMD_GOTO_ADDRESS)
		{
			strText = CString(tTempCmdPoint.pszNote);
			for(int j=0; j<tTempCmdPoint.iDblNum; j++)
			{
				strTemp.Format("%.3f, ", tTempCmdPoint.pDblValue[j]);
				strText += strTemp;
			}
			strText += " | ";
			for(int j=0; j<tTempCmdPoint.iIntNum; j++)
			{
				strTemp.Format("%02d, ", tTempCmdPoint.pIntValue[j]);
				strText += strTemp;
			}
		}
		else if(tTempCmdPoint.tType == CMD_CALL_SUB)
		{
			strText = CString(tTempCmdPoint.pszNote);
			strText+=",";
			for(int j=0; j<tTempCmdPoint.iDblNum; j++)
			{
				strTemp.Format("%.3f, ", tTempCmdPoint.pDblValue[j]);
				strText += strTemp;
			}
			strText += " | ";
			for(int j=0; j<tTempCmdPoint.iIntNum; j++)
			{
				strTemp.Format("%02d, ", tTempCmdPoint.pIntValue[j]);
				strText += strTemp;
			}
		}
		else if(tTempCmdPoint.tType == CMD_REPEATARC)
		{
			strText = "";
			for(int j=0; j<tTempCmdPoint.iDblNum; j++)
			{
				strTemp.Format("%.8f, ", tTempCmdPoint.pDblValue[j]);
				strText += strTemp;
			}
			strText += " | ";
			for(int j=0; j<tTempCmdPoint.iIntNum; j++)
			{
				strTemp.Format("%02d, ", tTempCmdPoint.pIntValue[j]);
				strText += strTemp;
			}
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// Mark 点
			if (CMD_MARK == tTempCmdPoint.tType)
			{
				if (bFront)
					GetDlgItem(IDC_BTN_REVIEW_F)->ShowWindow(SW_SHOW);
				else
					GetDlgItem(IDC_BTN_REVIEW_B)->ShowWindow(SW_SHOW);
			}

			//////////////////////////////////////////////////////////////////////////
			// 翻版指令
			if (CMD_ReverseBoard == tTempCmdPoint.tType)
				bFront = !bFront;



			strText = "";
			if ((CMD_SUPEREND == tTempCmdPoint.tType) || (CMD_SUPERDOT == tTempCmdPoint.tType))
			{
				strText.Format("%.3f", tTempCmdPoint.pDblValue[3]);	// 胶量
				pList->SetItemText(i, 2, strText);
			}

			if (IsTrackPoint(tTempCmdPoint))
			{
				strText.Format("%.3f", tTempCmdPoint.pDblValue[0]);
				pList->SetItemText(i, 4, strText);
				strText.Format("%.3f", tTempCmdPoint.pDblValue[1]);
				pList->SetItemText(i, 5, strText);
				if (tTempCmdPoint.tType != CMD_ZMETER)
				{
					strText.Format("%.3f", tTempCmdPoint.pDblValue[4]);
					pList->SetItemText(i, 3, strText);
				}
			}

			strText = "";
			for(int j=0; j<tTempCmdPoint.iDblNum; j++)
			{
				strTemp.Format("%.3f, ", tTempCmdPoint.pDblValue[j]);
				strText += strTemp;
			}
			strText += " | ";
			for(int j=0; j<tTempCmdPoint.iIntNum; j++)
			{
				strTemp.Format("%02d, ", tTempCmdPoint.pIntValue[j]);
				strText += strTemp;
			}
		}
        pList->SetItemText(i, 6, strText);

		

		if(IsParamPoint(tTempCmdPoint))
		{
			strTemp.Format("Dot%d", (int)(tTempCmdPoint.pDblValue[7]+1));
			pList->SetItemText(i, 7, strTemp);
		}
		if(IsParamPoint(tTempCmdPoint,FALSE))
		{
			if ((CMD_SUPEREND == tTempCmdPoint.tType) || (PARAM_LINE_END == tTempCmdPoint.tType))
			{
				strTemp.Format("Line%d", (int)(tTempCmdPoint.pDblValue[7]+1));
				pList->SetItemText(i, 7, strTemp);
			} 
			else
			{
				strTemp = "";
				pList->SetItemText(i, 7, strTemp);
			}
			

		}
	}

//	if(g_pDoc != NULL){	g_pDoc->SetModifiedFlag(TRUE);}
	if(g_pFrm != NULL){ g_pFrm->SetWindowTextEx();}

	m_wndList.SetRedraw(TRUE);
	SetCmdCurSel(iSelIndex);

	g_pView->Invalidate();
	g_pFrm->m_Robot->m_bModifiedTrack=TRUE;	
}

BOOL CDlgPathEdit::GetCircle(double dX[3], double dY[3], double *pX, double *pY, double *pR)
{
	CPointGE pGE;
	pGE.CaclCirclePara(dX[0],dY[0],dX[1],dY[1],dX[2],dY[2],pX, pY, pR);
	
	return TRUE;//KS_GageCircle(GAGE_MODE_LSC, dX, dY, 3, pX, pY, pR);
}
BOOL CDlgPathEdit::GetCircle(const tgCmdLine *tgCmdLine, double *pX, double *pY, double *pR)
{
	if(tgCmdLine->tType != CMD_CIRCLE || tgCmdLine->iDblNum<12)return FALSE;
	double dX[3], dY[3];
	for(int i=0; i<3; i++)
	{
		dX[i] = tgCmdLine->pDblValue[i*4 + 0];
		dY[i] = tgCmdLine->pDblValue[i*4 + 1];
	}
	return GetCircle(dX, dY, pX, pY, pR);
}
double CDlgPathEdit::GetTrangleArea(double dx[3], double dy[3])
{
	double a = sqrt((dx[0]-dx[1])*(dx[0]-dx[1]) + (dy[0]-dy[1])*(dy[0]-dy[1]));
	double b = sqrt((dx[1]-dx[2])*(dx[1]-dx[2]) + (dy[1]-dy[2])*(dy[1]-dy[2]));
	double c = sqrt((dx[2]-dx[0])*(dx[2]-dx[0]) + (dy[2]-dy[0])*(dy[2]-dy[0]));
	double p = (a+b+c)/2;
	return sqrt(p*(p-a)*(p-b)*(p-c));
}
BOOL CDlgPathEdit::IsPtInTrangle(double x, double y, double dx[3], double dy[3])
{
	double a, b, c, m;
	double ddx[3], ddy[3];
	m = GetTrangleArea(dx, dy);
	memcpy(ddx,dx, sizeof(double)*3);
	memcpy(ddy,dy, sizeof(double)*3);
	ddx[0]=x;ddy[0]=y;
	a = GetTrangleArea(ddx, ddy);
	memcpy(ddx,dx, sizeof(double)*3);
	memcpy(ddy,dy, sizeof(double)*3);
	ddx[1]=x;ddy[1]=y;
	b = GetTrangleArea(ddx, ddy);
	memcpy(ddx,dx, sizeof(double)*3);
	memcpy(ddy,dy, sizeof(double)*3);
	ddx[2]=x;ddy[2]=y;
	c = GetTrangleArea(ddx, ddy);
	
	return fabs(a+b+c-m)<0.000001;
}
BOOL CDlgPathEdit::GetCmdCircleInfo(tgCmdLine &tCircleCmd, tgCircle *pArc)
{
	pArc->x = tCircleCmd.pDblValue[0];
	pArc->y = tCircleCmd.pDblValue[1];
	pArc->r = tCircleCmd.pDblValue[4];

	pArc->dx = pArc->x + pArc->r*cos(0.0f);
	pArc->dy = pArc->y + pArc->r*sin(0.0f);
	return TRUE;
}
BOOL CDlgPathEdit::GetCmdBrushInfo(tgCmdLine &tCmd, tgBrush *pArea)
{
	pArea->x = tCmd.pDblValue[0];
	pArea->y = tCmd.pDblValue[1];
	pArea->z = tCmd.pDblValue[2];
	pArea->u = tCmd.pDblValue[3];

	if(0 == tCmd.pIntValue[5])
	{//retangle
		pArea->dx = tCmd.pDblValue[4] - pArea->x;
		pArea->dy = tCmd.pDblValue[5] - pArea->y;

		if(pArea->nDir == 0)
		{
			pArea->nNum = (int)(fabs(pArea->dx)/tCmd.pDblValue[8]);
			pArea->dx = pArea->dx/pArea->nNum;

			if (pArea->nNum<=0) return FALSE;
		}
		else if(pArea->nDir == 1)
		{
			pArea->nNum = (int)(fabs(pArea->dy)/tCmd.pDblValue[8]);
			pArea->dy = pArea->dy/pArea->nNum;

			if (pArea->nNum<=0) return FALSE;
		}
		//pArea->dfCx[0] = tCmd.pDblValue[4];
		//pArea->dfCy[0] = tCmd.pDblValue[5];
		if (tCmd.pIntValue[6]==1)
		{
			pArea->dRadius = tCmd.pDblValue[9];
		}
		else
			pArea->dRadius = 0;
	}
	else
	{
		pArea->dfCx[0] = tCmd.pDblValue[4];
		pArea->dfCy[0] = tCmd.pDblValue[5];
		pArea->dx = (pArea->x - pArea->dfCx[0]);
		pArea->dy = (pArea->y - pArea->dfCy[0]);

		double dfR = sqrt( pArea->dx * pArea->dx + pArea->dy * pArea->dy);
		double dfA = atan2(pArea->dy ,pArea->dx)*180/M_PI;
		if(dfA>180) dfA=180;
	
		pArea->nNum = (int)(2 * dfR/tCmd.pDblValue[8]) - 1;

		if (pArea->nNum<=0) return FALSE;

		pArea->dfCx[1] = pArea->x;
		pArea->dfCy[1] = pArea->y;
		pArea->dAngle=dfA;
		pArea->dRadius=dfR;
		pArea->dStep=tCmd.pDblValue[8];
	}
	return TRUE;
}

BOOL CDlgPathEdit::GetCmdArcInfo(tgCmdLine &tArcCmd, int iArcCmdIndex,tgArc *pArc)
{
	UNREFERENCED_PARAMETER(tArcCmd);
	int m0=-1, m1=iArcCmdIndex, m2=-1;
	m0 = GetPreTrackPoint(m1, TRUE);
	m2 = GetNextTrackPoint(m1, TRUE);

	if(m0<0 || m1<0 || m2<0)return FALSE;


	pArc->dx[0] = m_rawList.GetAt(m_rawList.FindIndex(m0)).pDblValue[0];
	pArc->dx[1] = m_rawList.GetAt(m_rawList.FindIndex(m1)).pDblValue[0];
	pArc->dx[2] = m_rawList.GetAt(m_rawList.FindIndex(m2)).pDblValue[0];
	pArc->dy[0] = m_rawList.GetAt(m_rawList.FindIndex(m0)).pDblValue[1];;
	pArc->dy[1] = m_rawList.GetAt(m_rawList.FindIndex(m1)).pDblValue[1];
	pArc->dy[2] = m_rawList.GetAt(m_rawList.FindIndex(m2)).pDblValue[1];
	pArc->dz[0] = m_rawList.GetAt(m_rawList.FindIndex(m0)).pDblValue[2];
	pArc->dz[1] = m_rawList.GetAt(m_rawList.FindIndex(m1)).pDblValue[2];
	pArc->dz[2] = m_rawList.GetAt(m_rawList.FindIndex(m2)).pDblValue[2];

	if(GetCircle(pArc->dx, pArc->dy, &pArc->x, &pArc->y, &pArc->r))
	{
		double dAngle[3];//start, arc, end
		for(int i=0; i<3; i++)
		{
			dAngle[i] = atan2(pArc->dy[i] - pArc->y, pArc->dx[i] - pArc->x)*180/3.1415926;
			if(dAngle[i]<0)dAngle[i] += 360;
		}
		pArc->dStartAngle = dAngle[0];
		pArc->dEndAngle = dAngle[2];

		if(dAngle[2]<dAngle[0]) dAngle[2] += 360;
		if(dAngle[1]<dAngle[0]) dAngle[1] += 360;
		if(dAngle[0]<dAngle[1] && dAngle[1]<dAngle[2])
		{
			 //逆时针
			 pArc->angle = dAngle[2] - dAngle[0];
		}
		else
		{
			//顺时针
			pArc->angle = dAngle[2] - dAngle[0]-360;
		}
		pArc->iType = fabs(pArc->angle)<180 ? 0 : 1;
		return TRUE;
	}
	return FALSE;
}


BOOL CDlgPathEdit::GetCmdArcInfo(tgCmdLine tPreDo,tgCmdLine tPassDo,tgCmdLine tNextDo, tgArc *pArc)
{

	pArc->dx[0] =tPreDo.pDblValue[0];
	pArc->dx[1] = tPassDo.pDblValue[0];
	pArc->dx[2] = tNextDo.pDblValue[0];
	pArc->dy[0] = tPreDo.pDblValue[1];
	pArc->dy[1] = tPassDo.pDblValue[1];
	pArc->dy[2] = tNextDo.pDblValue[1];
	pArc->dz[0] = tPreDo.pDblValue[2];
	pArc->dz[1] = tPassDo.pDblValue[2];
	pArc->dz[2] = tNextDo.pDblValue[2];

	if(GetCircle(pArc->dx, pArc->dy, &pArc->x, &pArc->y, &pArc->r))
	{
		double dAngle[3];//start, arc, end
		for(int i=0; i<3; i++)
		{
			dAngle[i] = atan2(pArc->dy[i] - pArc->y, pArc->dx[i] - pArc->x)*180/M_PI;
			if(dAngle[i]<0)dAngle[i] += 360;
		}
		pArc->dStartAngle = dAngle[0];
		pArc->dEndAngle = dAngle[2];

		if(dAngle[2]<dAngle[0]) dAngle[2] += 360;
		if(dAngle[1]<dAngle[0]) dAngle[1] += 360;
		if(dAngle[0]<dAngle[1] && dAngle[1]<dAngle[2])
		{
			//逆时针
			pArc->angle = dAngle[2] - dAngle[0];
		}
		else
		{
			//顺时针
			pArc->angle = dAngle[2] - dAngle[0]-360;
		}
		pArc->iType = fabs(pArc->angle)<180 ? 0 : 1;
		return TRUE;
	}
	return FALSE;
}
BOOL CDlgPathEdit::TrackArcHandle(tgCmdLine &tPreDot, tgCmdLine &tPassDot, tgCmdLine &tNextDot, double dRound,double &Cx,double &Cy,BOOL &nDir)
{
	CPointGE pGE;
	CMyPoint p1,p2,p3,pCn;
	Bool dir;
	Bool IsOk=FALSE;

	p1.m_fX = tPreDot.pDblValue[0];
	p1.m_fY = tPreDot.pDblValue[1];
	p2.m_fX = tPassDot.pDblValue[0];
	p2.m_fY = tPassDot.pDblValue[1];
	p3.m_fX = tNextDot.pDblValue[0];
	p3.m_fY = tNextDot.pDblValue[1];

	IsOk=pGE.Pt3Circle(p1,p2,p3,dRound,10*dRound,pCn,dir);

	if (IsOk!=2) return FALSE;

	tPreDot.pDblValue[0]=p1.m_fX;
	tPreDot.pDblValue[1]=p1.m_fY;
	tNextDot.pDblValue[0]=p3.m_fX;
	tNextDot.pDblValue[1]=p3.m_fY;

	Cx = pCn.m_fX;
	Cy = pCn.m_fY;

	nDir = dir;

	return TRUE;
}
BOOL CDlgPathEdit::GetCmdArcInfoEx(tgCmdLine *tArcCmd0, tgCmdLine *tArcCmd1,tgCmdLine *tArcCmd2,tgArc *pArc)
{
	pArc->dx[0] = tArcCmd0->pDblValue[0];
	pArc->dx[1] = tArcCmd1->pDblValue[0];
	pArc->dx[2] = tArcCmd2->pDblValue[0];
	pArc->dy[0] = tArcCmd0->pDblValue[1];
	pArc->dy[1] = tArcCmd1->pDblValue[1];
	pArc->dy[2] = tArcCmd2->pDblValue[1];
	pArc->dz[0] = tArcCmd0->pDblValue[2];
	pArc->dz[1] = tArcCmd1->pDblValue[2];
	pArc->dz[2] = tArcCmd2->pDblValue[2];

	if(GetCircle(pArc->dx, pArc->dy, &pArc->x, &pArc->y, &pArc->r))
	{
		double dAngle[3];//start, arc, end
		for(int i=0; i<3; i++)
		{
			dAngle[i] = atan2(pArc->dy[i] - pArc->y, pArc->dx[i] - pArc->x)*180/M_PI;
			if(dAngle[i]<0)dAngle[i] += 360;
		}
		pArc->dStartAngle = dAngle[0];
		pArc->dEndAngle = dAngle[2];

		if(dAngle[2]<dAngle[0]) dAngle[2] += 360;
		if(dAngle[1]<dAngle[0]) dAngle[1] += 360;
		if(dAngle[0]<dAngle[1] && dAngle[1]<dAngle[2])
		{
			//逆时针
			pArc->angle = dAngle[2] - dAngle[0];
		}
		else
		{
			//顺时针
			pArc->angle = dAngle[2] - dAngle[0]-360;
		}
		pArc->iType = fabs(pArc->angle)<180 ? 0 : 1;
		return TRUE;
	}
	return FALSE;
}
BOOL CDlgPathEdit::IsTrackPoint(tgCmdLine &tCmd, BOOL bOnlyLinePoint)
{
	CMD_TYPE t = tCmd.tType;
	if(bOnlyLinePoint)
	{
		if(t==CMD_LINE_START	|| t==CMD_LINE_PASSING	|| t==CMD_LINE_END	|| t==CMD_LABEL
			|| t==CMD_LINE_ARC	|| t==CMD_ZMETER		|| t==CMD_REPEATX_ZH|| t==CMD_REPEATY_ZH
			|| t==CMD_SUPERSTART|| t==CMD_SUPERPOINT	|| t==CMD_SUPEREND	|| t==CMD_SUPERDOT)
			return TRUE;
	}
	else
	{
		if(t == CMD_POINT	||t == CMD_LINE_START	||t==CMD_LINE_PASSING	||t==CMD_LINE_END            
			||t==CMD_POT	||t==CMD_WCL			||t==CMD_WCRECT			||t==CMD_LINE_ARC	
			||t==CMD_CIRCLE		||t==CMD_DUMMY		||t==CMD_WAIT_POINT		||t==CMD_LABEL
			||t==CMD_ZMETER		||t==CMD_REPEATX_ZH		||t==CMD_REPEATY_ZH
			||t==CMD_SUPERSTART	||t==CMD_SUPERPOINT		||t==CMD_SUPEREND	||t==CMD_SUPERDOT)
			return TRUE;
	}
	return FALSE;
}

BOOL CDlgPathEdit::IsParamPoint(tgCmdLine &tCmd, BOOL bParamDot /* = TRUE */)
{
	CMD_TYPE t = tCmd.tType;
	if(bParamDot)					// 点参数
	{
		if(t == CMD_POINT	||t==CMD_POT
			|| t==CMD_SUPERDOT)
			return TRUE;
	}
	else
	{
		if(t == CMD_LINE_START	||t==CMD_LINE_PASSING	||t==CMD_LINE_END
			||t==CMD_SUPERSTART	||t==CMD_SUPERPOINT		||t==CMD_SUPEREND)
			return TRUE;
	}
	return FALSE;
}

int CDlgPathEdit::GetPreTrackPoint(int iFrmIndex, BOOL bOnlyLinePoint)
{
	for(int i=iFrmIndex-1; i>-1; i--)
	{
		if(i<0)return FALSE;
		if(IsTrackPoint(m_rawList.GetAt(m_rawList.FindIndex(i)), bOnlyLinePoint))return i;
	}
	return -1;
}
int CDlgPathEdit::GetNextTrackPoint(int iFrmIndex, BOOL bOnlyLinePoint)
{
	for(int i=iFrmIndex+1; i<m_rawList.GetCount(); i++)
	{
		if(IsTrackPoint(m_rawList.GetAt(m_rawList.FindIndex(i)), bOnlyLinePoint))return i;
	}
	return -1;
}
int CDlgPathEdit::GetPreTrackPointEx(int iFrmIndex, BOOL bOnlyLinePoint)
{
	for(int i=iFrmIndex-1; i>-1; i--)
	{
		if(i<0)return FALSE;
		if(IsTrackPoint(g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(i)), bOnlyLinePoint))return i;
	}
	return -1;
}
int CDlgPathEdit::GetNextTrackPointEx(int iFrmIndex, BOOL bOnlyLinePoint)
{
	for(int i=iFrmIndex+1; i<g_pFrm->m_Robot->m_cmdRunList.GetCount(); i++)
	{
		if(IsTrackPoint(g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(i)), bOnlyLinePoint))return i;
	}
	return -1;
}
void CDlgPathEdit::SaveCmdLine(LPCTSTR strFile)
{
	int i=0, j=0;
	int iCount = m_rawList.GetCount();
	CString strTemp, strText;
	CStdioFile sdFile;
	if(!sdFile.Open(strFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		AfxMessageBox("保存文件出错!");
		return;
	}
	strTemp.Format("保存文件:%s", strFile);
	g_pFrm->AddMsg(strTemp);

	//////////////////////////////////////////////////////////////////////////
	for(i=0; i<iCount; i++)
	{
		tTempCmdPoint = m_rawList.GetAt(m_rawList.FindIndex(i));
		if(tTempCmdPoint.tType == CMD_NOTE || tTempCmdPoint.tType == CMD_LABEL_END)
		{
			strTemp.Format("%%%d,", tTempCmdPoint.tType);
			strText = strTemp;

			strTemp.Format("%s", tTempCmdPoint.pszNote);
			strText += strTemp;
		}
		else if(tTempCmdPoint.tType == CMD_GOTO_ADDRESS 
			|| tTempCmdPoint.tType == CMD_CALL_SUB 
			|| tTempCmdPoint.tType == CMD_LABEL)
		{
			strTemp.Format("%%%d", tTempCmdPoint.tType);
			strText = strTemp;

			strTemp.Format(",%s", tTempCmdPoint.pszNote);
			strText += strTemp;

			strTemp.Format(",%d,%d,%d",tTempCmdPoint.iDotNum, tTempCmdPoint.iDblNum,tTempCmdPoint.iIntNum);
			strText += strTemp;

			for(j=0; j<tTempCmdPoint.iDblNum; j++)
			{
				strTemp.Format(",%.3fF", tTempCmdPoint.pDblValue[j]);
				strText += strTemp;
			}
			for(j=0; j<tTempCmdPoint.iIntNum; j++)
			{
				strTemp.Format(",%dD", tTempCmdPoint.pIntValue[j]);
				strText += strTemp;
			}

		}
		else if(tTempCmdPoint.tType == CMD_CALL_SUB)
		{
			strTemp.Format("%%%d,", tTempCmdPoint.tType);
			strText = strTemp;
			strTemp.Format("%s", tTempCmdPoint.pszNote);
			strText += strTemp;

			strTemp.Format(",%d,%d,%d",tTempCmdPoint.iDotNum, tTempCmdPoint.iDblNum,tTempCmdPoint.iIntNum);
			strText += strTemp;

			for(j=0; j<tTempCmdPoint.iDblNum; j++)
			{
				strTemp.Format(",%.3fF", tTempCmdPoint.pDblValue[j]);
				strText += strTemp;
			}
			for(j=0; j<tTempCmdPoint.iIntNum; j++)
			{
				strTemp.Format(",%dD", tTempCmdPoint.pIntValue[j]);
				strText += strTemp;
			}
		}
		else if(tTempCmdPoint.tType == CMD_REPEATARC)
		{
			strTemp.Format("%%%d,%d, %d,%d", tTempCmdPoint.tType, tTempCmdPoint.iDotNum, tTempCmdPoint.iDblNum,tTempCmdPoint.iIntNum);
			strText = strTemp;

			for(j=0; j<tTempCmdPoint.iDblNum; j++)
			{
				strTemp.Format(",%.8fF", tTempCmdPoint.pDblValue[j]);
				strText += strTemp;
			}
			for(j=0; j<tTempCmdPoint.iIntNum; j++)
			{
				strTemp.Format(",%dD", tTempCmdPoint.pIntValue[j]);
				strText += strTemp;
			}

		}
		else if((tTempCmdPoint.tType == CMD_REPEATX)||(tTempCmdPoint.tType == CMD_REPEATY))
		{
			strTemp.Format("%%%d,%d, %d,%d", tTempCmdPoint.tType, tTempCmdPoint.iDotNum, tTempCmdPoint.iDblNum,tTempCmdPoint.iIntNum);
			strText = strTemp;

			for(j=0; j<tTempCmdPoint.iDblNum; j++)
			{
				if(2 == j)
				{
					strTemp.Format(",%.9fF", tTempCmdPoint.pDblValue[j]);
					strText += strTemp;
				}
				else
				{
					strTemp.Format(",%.3fF", tTempCmdPoint.pDblValue[j]);
					strText += strTemp;
				}
				
			}
			for(j=0; j<tTempCmdPoint.iIntNum; j++)
			{
				strTemp.Format(",%dD", tTempCmdPoint.pIntValue[j]);
				strText += strTemp;
			}
		}
		else
		{
			strTemp.Format("%%%d,%d, %d,%d", tTempCmdPoint.tType, tTempCmdPoint.iDotNum, tTempCmdPoint.iDblNum,tTempCmdPoint.iIntNum);
			strText = strTemp;
		
			for(j=0; j<tTempCmdPoint.iDblNum; j++)
			{
				strTemp.Format(",%.3fF", tTempCmdPoint.pDblValue[j]);
				strText += strTemp;
			}
			for(j=0; j<tTempCmdPoint.iIntNum; j++)
			{
				strTemp.Format(",%dD", tTempCmdPoint.pIntValue[j]);
				strText += strTemp;
			}
		}
		strText += "*%\n";
		//strText += "*%\r\n";
		sdFile.WriteString(strText);
	}

	sdFile.Close();
}

void CDlgPathEdit::ReadCmdLine(LPCTSTR strFile)
 {
	CStdioFile sdFile;
	if(!sdFile.Open(strFile, CFile::modeRead|CFile::typeText))
	{
		AfxMessageBox("打开文件错误.");
		return;
	}
	CString strText, strTemp;
	strTemp.Format("打开文件:%s", strFile);
	g_pFrm->AddMsg(strTemp);

	int n=0, m=0, j=0, iDblN=0, iIntN=0;
	int k=0;
	tgCmdLine tReadCmdPoint;
	m_rawList.RemoveAll();

	while(sdFile.ReadString(strText) || strText.GetLength() != 0)
	{
//		if(strText.Mid(0,1) != "%")	break;
		if(strText.Mid(0,1) != "%")	continue;
		tReadCmdPoint.Clear();//初始化	
		//////////////////////////////////////////////////////////////////////////
		//type
		n = strText.Find(",");
		strTemp = strText.Mid(1, n-1);
		strText = strText.Mid(n+1);
		tReadCmdPoint.tType = CMD_TYPE(atoi(strTemp));
		if(tReadCmdPoint.tType == CMD_NOTE || tReadCmdPoint.tType == CMD_LABEL_END)
		{
			m = strText.Find("*");
			strcpy_s(tReadCmdPoint.pszNote, (LPSTR)(LPCSTR)strText.Mid(0, m));
			m_rawList.AddTail(tReadCmdPoint);
		    continue;
		}
		if(tReadCmdPoint.tType == CMD_GOTO_ADDRESS)
		{
			k = strText.Find(",");
			strcpy_s(tReadCmdPoint.pszNote, (LPSTR)(LPCSTR)strText.Mid(0, k));
			strText = strText.Mid(k+1);
		
			//continue;
		}
		if(tReadCmdPoint.tType == CMD_CALL_SUB || CMD_LABEL==tReadCmdPoint.tType)
		{
			n = strText.Find(",");
			strTemp = strText.Mid(0, n);
			strcpy_s(tReadCmdPoint.pszNote, (LPSTR)(LPCSTR)strText.Mid(0, n));
			strText = strText.Mid(n+1);

			//int iCNum		
			n = strText.Find(",");
			strTemp = strText.Mid(0, n);
			strText = strText.Mid(n+1);
			tReadCmdPoint.iDotNum = atoi(strTemp);

			//double num
			n = strText.Find(",");
			strTemp = strText.Mid(0, n);
			strText = strText.Mid(n+1);
			iDblN = atoi(strTemp);
			tReadCmdPoint.iDblNum = iDblN;

			//int num
			n = strText.Find(",");
			strTemp = strText.Mid(0, n);
			strText = strText.Mid(n+1);
			iIntN = atoi(strTemp);
			tReadCmdPoint.iIntNum = iIntN;

			for(j=0; j<tReadCmdPoint.iDblNum; j++)
			{
				n = strText.Find(",");
				if(n<0)n=strText.Find("*");
				strTemp = strText.Mid(0, n);
				strText = strText.Mid(n+1);
				tReadCmdPoint.pDblValue[j] = atof(strTemp);
			}
			for(j=0; j<tReadCmdPoint.iIntNum; j++)
			{
				n = strText.Find(",");
				if(n<0)n=strText.Find("*");
				strTemp = strText.Mid(0, n);
				strText = strText.Mid(n+1);
				tReadCmdPoint.pIntValue[j] = atoi(strTemp);
			}
			m_rawList.AddTail(tReadCmdPoint);
			continue;
		}

		//int iCNum		
		n = strText.Find(",");
		strTemp = strText.Mid(0, n);
		strText = strText.Mid(n+1);
		tReadCmdPoint.iDotNum = atoi(strTemp);

		//double num
		n = strText.Find(",");
		strTemp = strText.Mid(0, n);
		strText = strText.Mid(n+1);
		iDblN = atoi(strTemp);
		tReadCmdPoint.iDblNum = iDblN;
		
		//int num
		n = strText.Find(",");
		strTemp = strText.Mid(0, n);
		strText = strText.Mid(n+1);
		iIntN = atoi(strTemp);
		tReadCmdPoint.iIntNum = iIntN;
		
		//double value
		for(j=0; j<iDblN; j++)
		{
			n = strText.Find(",");
			if(n<0)n=strText.Find("*");
			strTemp = strText.Mid(0, n);
			strText = strText.Mid(n+1);
			tReadCmdPoint.pDblValue[j] = atof(strTemp);
		}
		//int value
		for(j=0; j<iIntN; j++)
		{
			n = strText.Find(",");
			if(n<0)n=strText.Find("*");
			strTemp = strText.Mid(0, n);
			strText = strText.Mid(n+1);
			tReadCmdPoint.pIntValue[j] = atoi(strTemp);
		}
		m_rawList.AddTail(tReadCmdPoint);

// 		if (CMD_END_PROGRAM == tReadCmdPoint.tType)
// 		{
// 			break;
// 		}
	}
	
	sdFile.Close();

	//////////////////////////////////////////////////////////////////////////
	UpdateCmdListBox(m_rawList.GetCount()-1);
	g_pFrm->m_Robot->m_iStepIndex = -1;//单步行号要复位
	g_pView->Invalidate();
}
void CDlgPathEdit::FreeCmdList()
{//清除列表
	m_rawList.RemoveAll();
}
int CDlgPathEdit::GetCmdCurSel()
{
	return m_wndList.GetNextItem(-1, LVNI_SELECTED);
}

BOOL CDlgPathEdit::IsSelState(int iIndex)
{
	if(iIndex<0 || iIndex>m_wndList.GetItemCount()-1)	return FALSE;

	return m_wndList.GetItemState(iIndex, LVNI_SELECTED) == LVNI_SELECTED;
}

BOOL CDlgPathEdit::IsHaveRepeat(int iFrmIndex)
{
	for(int i=iFrmIndex+1; i<m_rawList.GetCount(); i++)
	{
		if(m_rawList.GetAt(m_rawList.FindIndex(i)).tType == CMD_REPEATX || m_rawList.GetAt(m_rawList.FindIndex(i)).tType == CMD_REPEATY)
			return TRUE;
	}
	return FALSE;
}
void CDlgPathEdit::SetCmdCurSel(int nSelIndex)
{
	if(nSelIndex<0 || nSelIndex > m_wndList.GetItemCount()-1)
	{
		if (m_wndList.GetItemCount()>0)
		{
			for(int i=m_wndList.GetItemCount()-1; i>-1; i--)
			{
				if(m_wndList.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
				{
					m_wndList.SetItemState(i, 0, LVIS_SELECTED|LVIS_FOCUSED);
					break;
				}
			}
		}
		return;
	}
		
	m_wndList.EnsureVisible(nSelIndex, FALSE);  
	for(int i=m_wndList.GetItemCount()-1; i>-1; i--)
	{
		if(m_wndList.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_wndList.SetItemState(i, 0, LVIS_SELECTED|LVIS_FOCUSED);
			break;
		}
	}
//	TRACE(_T("nSelIndex=%d\n"),nSelIndex);
	m_wndList.SetItemState(nSelIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED);  
	m_wndList.SetFocus();
}
void CDlgPathEdit::DeleteCurSel()
{
	int iSelIndex = GetCmdCurSel();
	for(int i=m_wndList.GetItemCount()-1; i>-1; i--)
	{
		if(m_wndList.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_rawList.RemoveAt(m_rawList.FindIndex(i));
			m_wndList.DeleteItem(i);
		}
	}
	if(iSelIndex>m_wndList.GetItemCount()-1)
	{
		iSelIndex = m_wndList.GetItemCount()-1;
	}
	UpdateCmdListBox(iSelIndex);
}

void CDlgPathEdit::CutCurSel()
{
	int iSelIndex = GetCmdCurSel();
	m_Tls.RemoveAll();
	for(int i=m_wndList.GetItemCount()-1; i>-1; i--)
	{
		if(m_wndList.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_Tls.AddTail(m_rawList.GetAt(m_rawList.FindIndex(i)));
			m_rawList.RemoveAt(m_rawList.FindIndex(i));
			m_wndList.DeleteItem(i);
		}
	}
	if(iSelIndex>m_wndList.GetItemCount()-1)
	{
		iSelIndex = m_wndList.GetItemCount()-1;
	}
	UpdateCmdListBox(iSelIndex);
}

void CDlgPathEdit::PasteData()
{
	int iSelIndex = GetCmdCurSel();
	bool bIsHade = false;
	if(iSelIndex == -1)
	{
		bIsHade = true;
	}
	int iCount = m_Tls.GetCount()-1;
	for(int i=iCount;i>=0;i--)
	{
		if(bIsHade)
		{
			m_rawList.InsertAfter(m_rawList.FindIndex(iSelIndex),m_Tls.GetAt(m_Tls.FindIndex(i)));
			iSelIndex++;
		}
		else
		{
			m_rawList.InsertBefore(m_rawList.FindIndex(iSelIndex),m_Tls.GetAt(m_Tls.FindIndex(i)));
			iSelIndex++;
		}
	}
	UpdateCmdListBox(iSelIndex);
}

void CDlgPathEdit::CopyCurSel()
{
	m_Tls.RemoveAll();
	for(int i=m_wndList.GetItemCount()-1; i>-1; i--)
	{
		if(m_wndList.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_Tls.AddTail(m_rawList.GetAt(m_rawList.FindIndex(i))/*tTempCmdPoint*/);
		}
	}
}

// 保存当前位置到指令列表
void CDlgPathEdit::LoadListArrayPos(tgCmdLine &tCmd, int iStartDblIndex)
{
	if(!g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		for(int i=0;i<3;i++)
	 		tCmd.pDblValue[iStartDblIndex+i] = 0;
	}
	else
	{
		g_pFrm->m_Precision.GetCurPosMM();
		for(int i=0;i<3;i++)// XYZU
			tCmd.pDblValue[iStartDblIndex+i] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[i];
	}
	g_pView->Invalidate();
}

// 指令试教
void CDlgPathEdit::OnCmdType(UINT ID)
{
	// TODO: Add your command handler code here
//	g_pView->ImgStop();
	CString strTemp;
	strTemp.Format("ID %d", ID);

	int iSelIndex=-1;    // 当前指令ID

     // 插入指令
	if(m_bInsertCmd) 
	{
		iSelIndex = GetCmdCurSel();
		iSelIndex = (iSelIndex == -1) ?  m_rawList.GetCount() : iSelIndex;
	}
	else
		iSelIndex = m_wndList.GetItemCount();

	if(iSelIndex<0)
	{
		iSelIndex = 0;
	}	

	BOOL bListOper = TRUE;     // 需要操作列表或列表内容有改动的标志位
	BOOL bModify   = TRUE;

	tTempCmdPoint.Clear();     // 指令临时列表

	int k =0;
	//////////////////////////////////////////////////////////////////////////
	switch(ID)
	{
	case ID_CMD_REV_BOARD: // 翻板指令
		tTempCmdPoint.tType = CMD_ReverseBoard;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 0;
		break;

	case ID_CMD_MARK:      // Mark 
// 		if((g_pFrm->m_pSysParam->nFileSeparaEnable == 0)
// 			&&(g_pFrm->m_pSysParam->nFileScrapeEnable == 0))
// 		{  // 通用模式下检测是否只有一个Mark点
// 			if(CheckMark())
// 			{
// 				AfxMessageBox("Mark Command already exist in the list");
// 				return;
// 			}
// 		}

		tTempCmdPoint.tType =  CMD_MARK;   
		tTempCmdPoint.iDotNum =   2;      // 坐标点组个数 
		tTempCmdPoint.iDblNum = 8; 
		tTempCmdPoint.iIntNum = 5;

		for(k=0;k<tTempCmdPoint.iIntNum;k++)
			tTempCmdPoint.pIntValue[k] = 60;	// 相似度

		for (k=0;k<tTempCmdPoint.iDblNum;k++)
			tTempCmdPoint.pDblValue[k] = 0;
        // Mark点组编号
		if (CheckMark())
		{
			tTempCmdPoint.pIntValue[2] = 1;
		}
		else
		{
			tTempCmdPoint.pIntValue[2] = 0;
		}
		break;

	case IDS_TOOL_DOT:      // 单点出胶 
	case IDC_BUTTON_EDIT0:
		if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
			tTempCmdPoint.tType = CMD_SUPERDOT;
		else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
			tTempCmdPoint.tType = CMD_POINT;
		tTempCmdPoint.iDotNum=1;  // 需要记录的坐标组数
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;
        
        // 记录当前坐标位置到列表
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);  

		tTempCmdPoint.pIntValue[0]=0; //阀ID
		tTempCmdPoint.pIntValue[1]=1; //开阀
		tTempCmdPoint.pIntValue[3]=1; //下位
		bModify = FALSE;
		break;

	case IDS_TOOL_START:       // 直线起点 
		tTempCmdPoint.tType = CMD_LINE_START;
		tTempCmdPoint.iDotNum=1;  
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=1;
		tTempCmdPoint.pIntValue[3]=1;//下位
		bModify = FALSE;
		break;
	
	case IDS_TOOL_PASS:     // 线段中点
		tTempCmdPoint.tType = CMD_LINE_PASSING;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=1;
		bModify = FALSE;
		break;
	case IDS_TOOL_ARC:     // 圆弧中点
	case IDC_BUTTON_EDIT3:
		tTempCmdPoint.tType = CMD_LINE_ARC;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0;
		bModify = FALSE;
		break;
	case IDS_TOOL_END:         // 线段结束点
		tTempCmdPoint.tType = CMD_LINE_END;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0;
		bModify = FALSE;
		break;
	case IDS_TOOL_CIRCLE:     // 圆形点胶
	case IDC_BUTTON_EDIT5:
		tTempCmdPoint.tType = CMD_CIRCLE;
		tTempCmdPoint.iDotNum = 4;  // 4组坐标
		tTempCmdPoint.iDblNum = 20;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=1;
		tTempCmdPoint.pIntValue[3]=1;//下位
		break;
	case ID_CMD_DISPENSER:    // 点胶阀开关指令
		tTempCmdPoint.tType = CMD_DISPENSER;
		tTempCmdPoint.iDotNum = 0;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 4;

		tTempCmdPoint.pIntValue[0] = 0;
		tTempCmdPoint.pIntValue[1] = 1;
		tTempCmdPoint.pIntValue[2] = 0;
		tTempCmdPoint.pIntValue[3] = 1;
		break;

	case ID_CMD_TOOL:        // 选择阀
		tTempCmdPoint.tType = CMD_TOOL;
		tTempCmdPoint.iDotNum = 0;
		tTempCmdPoint.iDblNum = 3;
		tTempCmdPoint.iIntNum = 3;

		tTempCmdPoint.pIntValue[0] = 0;
		tTempCmdPoint.pIntValue[1] = 10;
		tTempCmdPoint.pIntValue[2] = 10;

		tTempCmdPoint.pDblValue[0] = 0;
		tTempCmdPoint.pDblValue[2] = 2;
		tTempCmdPoint.pDblValue[1] = 
			g_pFrm->m_pRobotParam->m_zContactPos - tTempCmdPoint.pDblValue[2];
		break;

	case ID_CMD_GOTOADDRESS:    // 跳转指令
		tTempCmdPoint.tType = CMD_GOTO_ADDRESS;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 4;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=0;
		strcpy_s(tTempCmdPoint.pszNote, "");
		break;
	case ID_CMD_BORATARC:         // 基板夹角参数
		tTempCmdPoint.tType = CMD_REPEATARC;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 1;
		tTempCmdPoint.pDblValue[0]=g_pFrm->m_pSysParam->m_BoardArc;  // 
		tTempCmdPoint.pIntValue[0]=0;
		break;
	case ID_CMD_STEPX:         // X 阵列
		tTempCmdPoint.tType = CMD_REPEATX;
		tTempCmdPoint.iDotNum = 3;
		tTempCmdPoint.iDblNum = 12;
		tTempCmdPoint.iIntNum = 8;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=0;
		tTempCmdPoint.pIntValue[4]=1;
		tTempCmdPoint.pIntValue[5]=1;
		tTempCmdPoint.pIntValue[7]=0;
		break;
	case ID_CMD_STEPY:         // Y 阵列
		tTempCmdPoint.tType = CMD_REPEATY;
		tTempCmdPoint.iDotNum = 3;
		tTempCmdPoint.iDblNum = 12;
		tTempCmdPoint.iIntNum = 8;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=0;
		tTempCmdPoint.pIntValue[4]=1;
		tTempCmdPoint.pIntValue[5]=1;
		tTempCmdPoint.pIntValue[7]=0;
		break;
	case ID_CMD_STEPX_ZH:     // 激光测高X阵列
		tTempCmdPoint.tType = CMD_REPEATX_ZH;
		tTempCmdPoint.iDotNum = 1;
		tTempCmdPoint.iDblNum = 10;
		tTempCmdPoint.iIntNum = 4;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		for (k=0;k<tTempCmdPoint.iDblNum;k++)
			tTempCmdPoint.pDblValue[k] = 0;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=0;
		tTempCmdPoint.pIntValue[2]=2;
		tTempCmdPoint.pIntValue[3]=2;

		tTempCmdPoint.pDblValue[4]=0;
		tTempCmdPoint.pDblValue[5]=0.4;

		break;
	case ID_CMD_STEPY_ZH:     // 激光测高Y阵列
		tTempCmdPoint.tType = CMD_REPEATY_ZH;
		tTempCmdPoint.iDotNum = 1;
		tTempCmdPoint.iDblNum = 10;
		tTempCmdPoint.iIntNum = 4;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		for (k=0;k<tTempCmdPoint.iDblNum;k++)
			tTempCmdPoint.pDblValue[k] = 0;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=0;
		tTempCmdPoint.pIntValue[2]=2;
		tTempCmdPoint.pIntValue[3]=2;

		tTempCmdPoint.pDblValue[4]=0;
		tTempCmdPoint.pDblValue[5]=0.4;

		break;

	case IDS_TOOLS_BDROP:  // 称重打点
    case IDC_BUTTON_EDIT7: 
		tTempCmdPoint.tType = CMD_POT;
		tTempCmdPoint.iDotNum=1;       // 坐标个数
		tTempCmdPoint.iDblNum = 5;   // double 数据个数
		tTempCmdPoint.iIntNum = 5;   // int 数据个数

		// 获取轴位置
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0; // 阀ID
		tTempCmdPoint.pIntValue[1]=1; // 开阀
		tTempCmdPoint.pIntValue[3]=1; // 下位

        tTempCmdPoint.pIntValue[4]=1; // 打点次数

		tTempCmdPoint.pDblValue[4]=10;// 重量
		
		break;
	case IDS_TOOLS_BLINE:  // 称重划线
    case IDC_BUTTON_EDIT6:

		tTempCmdPoint.tType = CMD_WCL;
		tTempCmdPoint.iDotNum=2;
		tTempCmdPoint.iDblNum = 10;  // 
		tTempCmdPoint.iIntNum = 5;
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pDblValue[8] = 10; //线段重量
        tTempCmdPoint.pDblValue[9] = 20; //线段速度
		tTempCmdPoint.pIntValue[0] = 0;  //工作模式
		tTempCmdPoint.pIntValue[1] = 20; //开阀时间
		tTempCmdPoint.pIntValue[2] = 20; //关阀时间
		tTempCmdPoint.pIntValue[3] = 10; //计数次
		tTempCmdPoint.pIntValue[4] = 1;  //开阀

		break;

	case IDS_TOOL_COATDOT:   // 称重矩形
    case IDC_BUTTON_EDIT8:  
		tTempCmdPoint.tType = CMD_WCRECT;
		tTempCmdPoint.iDotNum= 5;      // 数据点个数
		tTempCmdPoint.iDblNum = 22;  // 数据个数
		tTempCmdPoint.iIntNum = 5;   // 
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pDblValue[20] = 10; //线段重量  
		tTempCmdPoint.pDblValue[21] = 20; //线段速度
		tTempCmdPoint.pIntValue[0] = 0;   //阀ID
		tTempCmdPoint.pIntValue[1] = 1;   //开阀时间
		tTempCmdPoint.pIntValue[2] = 20;  //关阀时间
		tTempCmdPoint.pIntValue[3] = 10;  //计数次
		tTempCmdPoint.pIntValue[4] = 1;   //开阀
		 break;
	case ID_CMD_BRUSHAREA:   // 涂覆指令
		tTempCmdPoint.tType = CMD_BRUSH_AREA;
		tTempCmdPoint.iDotNum = 2;
		tTempCmdPoint.iDblNum = 12;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pDblValue[8]=5;//矩形涂覆宽度初始值
		tTempCmdPoint.pDblValue[9]=5;//圆形涂覆宽度初始值

		tTempCmdPoint.pIntValue[0]=0;//默认NO.0
		tTempCmdPoint.pIntValue[1]=1;//默认开阀
		tTempCmdPoint.pIntValue[3]=1;//下位
		tTempCmdPoint.pIntValue[4]=0;//默认X方向涂覆
		tTempCmdPoint.pIntValue[5]=0;//默认矩形涂覆
		
		break;
	case ID_CMD_CALLSUBROUTINUE:     // 调用子程序
		tTempCmdPoint.tType = CMD_CALL_SUB;
        tTempCmdPoint.iDotNum = 1;
		tTempCmdPoint.iDblNum = 6;
		tTempCmdPoint.iIntNum = 2;

        LoadListArrayPos(tTempCmdPoint, 0);
		
		tTempCmdPoint.pIntValue[0]=1;//0地址 1标号
		tTempCmdPoint.pIntValue[1]=0;//

        tTempCmdPoint.pDblValue[4]=0; // X方向微调
		tTempCmdPoint.pDblValue[5]=0; // y方向微调
		strcpy_s(tTempCmdPoint.pszNote, "");
		break;
	case ID_CMD_LABEL:         // 标号指令
		tTempCmdPoint.tType = CMD_LABEL;
		tTempCmdPoint.iDotNum=1;     // 坐标个数
		tTempCmdPoint.iDblNum = 8;   // double 数据个数
		tTempCmdPoint.iIntNum = 4;   // int 数据个数

		// 获取轴位置
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		strcpy_s(tTempCmdPoint.pszNote, "标号0");
		break;

	case ID_CMD_LABEL_END:     // 子程序结束 
		tTempCmdPoint.tType = CMD_LABEL_END;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 0;
		strcpy_s(tTempCmdPoint.pszNote, "");
		break;

	case ID_CMD_NULL_LINE:     // 空行 
		tTempCmdPoint.tType = CMD_NULL_LINE;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 0;
		strcpy_s(tTempCmdPoint.pszNote, "");
		break;

	case ID_CMD_SETIO:        // IO设置指令
		tTempCmdPoint.tType = CMD_SET_IO;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 3;

		tTempCmdPoint.pIntValue[0] = 0;		//0:input, 1:output
		tTempCmdPoint.pIntValue[1] = -1;	//端口号
		tTempCmdPoint.pIntValue[2] = 0;		//检测/输出状态
		break;

	case ID_CMD_WAITPOINT:      // 待机点指令
		tTempCmdPoint.tType = CMD_WAIT_POINT;
		tTempCmdPoint.iDotNum=1;  
		tTempCmdPoint.iDblNum = 5;
		tTempCmdPoint.iIntNum = 0;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pDblValue[4] = 0;//默认为 0s
		break;
	case ID_CMD_BATH:      //针头清洗指令 
		tTempCmdPoint.tType = CMD_BATH;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 5;
		tTempCmdPoint.iIntNum = 0;

		//for (k=0;k<tTempCmdPoint.iCNum;k++)
		//   LoadListArrayPos(tTempCmdPoint,k*4);
        tTempCmdPoint.pDblValue[0] = g_pFrm->m_pRobotParam->m_structClean.m_posClean.x+g_pFrm->m_pSysParam->tOffset[0].x;
		tTempCmdPoint.pDblValue[1] = g_pFrm->m_pRobotParam->m_structClean.m_posClean.y+g_pFrm->m_pSysParam->tOffset[0].y;
		tTempCmdPoint.pDblValue[2] = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_structClean.m_posClean.z;
		tTempCmdPoint.pDblValue[4] = 0;//默认为 0s
		break;
	case ID_CMD_ZHEIGHT:        // 基板高度检测
	case IDS_TOOL_ZHEIGHT:
		tTempCmdPoint.tType = CMD_ZMETER;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 6;
		tTempCmdPoint.iIntNum = 0;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
			tTempCmdPoint.pDblValue[4] = 30.0; // 基准值
		else
			tTempCmdPoint.pDblValue[4] = 0.0; // 基准值

		tTempCmdPoint.pDblValue[5] = 0.1;  // 延时 0s
		tTempCmdPoint.pDblValue[6] = 2.0;  // 点胶高度
		break;
	case ID_CMD_DELAY:      // 延时指令
		tTempCmdPoint.tType = CMD_DELAY;
		tTempCmdPoint.iDblNum=1;
		tTempCmdPoint.iIntNum = 0;

		tTempCmdPoint.pDblValue[0]=0; //默认为 0s
		break;
	case ID_CMD_STOPPOINT:    // 暂停指令
		tTempCmdPoint.tType = CMD_PAUSE;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 0;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);
		break;
	case ID_CMD_HOME:       // X, Y, Z轴将回归软件原点后继续往下执行纪录点
		tTempCmdPoint.tType = CMD_HOME;
		break;
	case ID_CMD_LOOP:       // 循环指令
		tTempCmdPoint.tType = CMD_LOOP;   
		tTempCmdPoint.iIntNum = 2;
		tTempCmdPoint.pIntValue[0] = 1;//默认循环次数为1 
		tTempCmdPoint.pIntValue[1] = 0;//默认行号为0开始循环
		break;
	case ID_CMD_DUMMY:     // 空点指令
		tTempCmdPoint.tType = CMD_DUMMY;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0;
		bModify = FALSE;
		break;
	case ID_CMD_END_PROGRAM:    // 程序结束指令
		tTempCmdPoint.tType = CMD_END_PROGRAM;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 0;
		break;
	case ID_CMD_NOTE:          // 注释行
		tTempCmdPoint.tType = CMD_NOTE;
		strcpy_s(tTempCmdPoint.pszNote, "该行为注释行");
		break;
	case ID_PARAM_LINESPEED:   // 线段速度指令
		tTempCmdPoint.tType = PARAM_LINE_SPEED;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 0;

		tTempCmdPoint.pDblValue[0] = 20;//默认点胶速度值20mm/s
		break;
	case ID_PARAM_LINESETUP:   // 线段参数
		tTempCmdPoint.tType = PARAM_LINE_SETUP;
		tTempCmdPoint.iDblNum = 6;
		tTempCmdPoint.iIntNum = 0;

		tTempCmdPoint.pDblValue[0]=0;
		tTempCmdPoint.pDblValue[1]=0;
		tTempCmdPoint.pDblValue[2]=5;//此项忽略
		tTempCmdPoint.pDblValue[3]=0; //s
		tTempCmdPoint.pDblValue[4]=0.10;
		tTempCmdPoint.pDblValue[5]=0.004;
		break;
	case ID_PARAM_LINEEND:   // 线段结束参数设置
		tTempCmdPoint.tType = PARAM_LINE_END;
		tTempCmdPoint.iDblNum = 3;
		tTempCmdPoint.iIntNum = 1;

		tTempCmdPoint.pDblValue[0]=5;//mm
		tTempCmdPoint.pDblValue[1]=5;//mm
		tTempCmdPoint.pDblValue[2]=20;//mm/s

		tTempCmdPoint.pIntValue[0]=0;
		break;
	case ID_PARAM_POINTSETUP:   // 单点参数
		tTempCmdPoint.tType = PARAM_POINT_SETUP;
		tTempCmdPoint.iDblNum = 3;
		tTempCmdPoint.iIntNum = 4;

		tTempCmdPoint.pDblValue[0]=0;
		tTempCmdPoint.pDblValue[1]=0.05;
		tTempCmdPoint.pDblValue[2]=0;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=5;
		tTempCmdPoint.pIntValue[2]=5;
		tTempCmdPoint.pIntValue[3]=1;
		break;
	case ID_PARAM_ZLIMITHIGH:    // Z 轴高度设置
		tTempCmdPoint.tType = PARAM_Z_WORKPOSSET;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 1;
		tTempCmdPoint.pDblValue[0]= 10; //mm
		tTempCmdPoint.pIntValue[0] = 1;
        	break;
	case ID_PARAM_JETSETUP:      // 喷射参数
		tTempCmdPoint.tType = PARAM_JET_SETUP;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 2;
		tTempCmdPoint.pDblValue[0]=10;
		tTempCmdPoint.pIntValue[0]=5;
		tTempCmdPoint.pIntValue[1]=5;
		break;
	case ID_PARAM_POINTEND:     // 单点结束参数
		tTempCmdPoint.tType = PARAM_POINT_END;
		tTempCmdPoint.iDblNum =3;
		tTempCmdPoint.iIntNum = 0;

		tTempCmdPoint.pDblValue[0]=100;//mm/s   空跑速度
		tTempCmdPoint.pDblValue[1]=20;//mm/s    空跑速度/4
		tTempCmdPoint.pDblValue[2]=0;//mm
		break;
	case ID_PARAM_Z_CLEARANCE:   // Z轴提高高度设置 
		tTempCmdPoint.tType = PARAM_Z_CLEARANCE;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 1;
		tTempCmdPoint.pDblValue[0]= 10;//mm
		tTempCmdPoint.pIntValue[0] = 1;
		break;
	case ID_PARAM_Z_SET_HEIGHT:   // Z 轴点胶高度
		tTempCmdPoint.tType = PARAM_Z_SET_HEIGHT;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 1;
		tTempCmdPoint.pDblValue[0] = 0;//mm
		tTempCmdPoint.pIntValue[0] = 1;
		break;
	case ID_PARAM_VELXY:          // XY 轴速度
		tTempCmdPoint.tType = PARAM_XY_VEL;
		tTempCmdPoint.iDblNum = 4;

		tTempCmdPoint.pDblValue[0]=1500;//mm/S
		tTempCmdPoint.pDblValue[1]=50;//mm/S
		tTempCmdPoint.pDblValue[2]=15;//mm/S
		tTempCmdPoint.pDblValue[3]=10;//mm/S
		break;
	case ID_PARAM_VELZ:           // Z 轴速度
		tTempCmdPoint.tType = PARAM_Z_VEL;
		tTempCmdPoint.iDblNum = 4;

		tTempCmdPoint.pDblValue[0]=100;//mm/S
		tTempCmdPoint.pDblValue[1]=100;//mm/S
		tTempCmdPoint.pDblValue[2]=5;//mm/S
		tTempCmdPoint.pDblValue[3]=0.3;//mm/S
		break;

	case ID_CMD_SET_Z_SAME:       // 将插入行以后的坐标点Z轴高度设为相同数值
		bListOper = FALSE;
		bModify = FALSE;
		SetZSame(iSelIndex);
		break;
		//////////////////////////////////////////////////////////////////////////
	case ID_CMD_DELETE:
		bListOper = FALSE;
		bModify = FALSE;
		DeleteCurSel();
		break;
	case ID_CMD_CUT:
		bListOper = FALSE;
		bModify = FALSE;
		CutCurSel();
		break;
	case ID_CMD_PASTE:
		bListOper = FALSE;
		bModify = FALSE;
		PasteData();
		break;
	case ID_CMD_COPY:
		bListOper = FALSE;
		bModify = FALSE;
		CopyCurSel();
		break;
	case ID_CMD_CLEAR_ALL:
		bListOper = FALSE;
		bModify = FALSE;
		FreeCmdList();
		break;
	case ID_CMD_REOPEN_FILE:  // 打开试教文件指令
		bListOper = FALSE;
		bModify = FALSE;
		ReadCmdLine(g_pDoc->GetPathName());
		break;
	///////////////////////////////////////////////

	case ID_CMD_SUPERSTART:      //  飞胶起始点
	case IDS_TOOLS_SUPERSTART:
	case IDC_BUTTON_EDIT1:
		tTempCmdPoint.tType = CMD_SUPERSTART;
		tTempCmdPoint.iDotNum=1;       // 需要记录的坐标组数
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		// 记录当前坐标位置到列表
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);  

		tTempCmdPoint.pIntValue[0]=0; //阀ID
		tTempCmdPoint.pIntValue[1]=1; //开阀
		tTempCmdPoint.pIntValue[3]=1; //下位

		tTempCmdPoint.pDblValue[4]=0; // 提前开阀距离
		bModify = FALSE;
		break;
	case ID_CMD_SUPERPOINT:      // 飞胶加工点
	case IDS_TOOLS_SUPERPOINT:   
	case IDC_BUTTON_EDIT2:
		tTempCmdPoint.tType = CMD_SUPERPOINT;
		tTempCmdPoint.iDotNum=1;       // 需要记录的坐标组数
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 3;

		// 记录当前坐标位置到列表
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);  

		tTempCmdPoint.pIntValue[0]=0; //阀ID
		tTempCmdPoint.pIntValue[1]=1; //开阀
		tTempCmdPoint.pIntValue[3]=1; //下位
   
		tTempCmdPoint.pDblValue[6]=0; // X微调距离
        tTempCmdPoint.pDblValue[7]=0; // Y微调距离
		bModify = FALSE;
		break;

	case ID_CMD_SUPEREND:       // 飞胶结束点
	case IDS_TOOLS_SUPEREND:
	case IDC_BUTTON_EDIT4:
		tTempCmdPoint.tType = CMD_SUPEREND;
		tTempCmdPoint.iDotNum=1;       // 需要记录的坐标组数
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		// 记录当前坐标位置到列表
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);  

		tTempCmdPoint.pIntValue[0]=0; //阀ID
		tTempCmdPoint.pIntValue[1]=1; //开阀
		tTempCmdPoint.pIntValue[3]=1; //下位

		tTempCmdPoint.pDblValue[4]=0; // 提前关阀距离
		bModify = FALSE;
		break;

	case ID_CMD_FILESEPARAT:  // 分段加工符

		tTempCmdPoint.tType = CMD_FILEDELIMITER;
		tTempCmdPoint.iDotNum=0;       
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 1;

		tTempCmdPoint.pIntValue[1] = 0;

		break;	
	case ID_CMD_FILESCRPEWORK:  // 拼版加工符

		tTempCmdPoint.tType = CMD_FILESCRPEWORK;
		tTempCmdPoint.iDotNum=0;       
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 1;

		tTempCmdPoint.pIntValue[1] = 0;

		break;
	default:
		NULL;
	}

	strTemp.Format("Add CMD：%s", szCmdline[tTempCmdPoint.tType]);
	g_pFrm->AddMsg(strTemp);

//	::WaitForSingleObject(g_runListEvent,INFINITE);

	if (bListOper)
	{
		if(m_bInsertCmd)
		{
			if(iSelIndex>=m_rawList.GetCount())
				m_rawList.AddTail(tTempCmdPoint);
			else
				m_rawList.InsertBefore(m_rawList.FindIndex(iSelIndex), tTempCmdPoint);
		}
		else
			m_rawList.AddTail(tTempCmdPoint);
	}
	if(bModify)//有窗口弹出来的情况
	{
		if(ModifiedCmd(iSelIndex) != IDOK)    
		{
			if(bListOper)
				m_rawList.RemoveAt(m_rawList.FindIndex(iSelIndex));
		}
	}
	//////////////////////////////////////////////////////////////////////////
	UpdateCmdListBox(iSelIndex);
//	g_runListEvent.SetEvent();
}

void CDlgPathEdit::SetZSame(int iFrmIndex)
{
	int i, j;
	
	CDlgZSame dlgZSame;
	tTempCmdPoint.tType = PARAM_SET_Z_SAME;
	tTempCmdPoint.iDblNum = 1;
	dlgZSame.m_pCmdLine = &tTempCmdPoint;
	if(dlgZSame.DoModal()!= IDOK)
		return;

	for(i=iFrmIndex; i<m_rawList.GetCount(); i++)
	{
		if(m_rawList.GetAt(m_rawList.FindIndex(i)).iDotNum<1)continue;
		for(j=0; j<m_rawList.GetAt(m_rawList.FindIndex(i)).iDotNum; j++)
		{
			m_rawList.GetAt(m_rawList.FindIndex(i)).pDblValue[j*4+2] = tTempCmdPoint.pDblValue[0];
		}
	}
}
/*=======================
// 基板夹角数据计算
// 参数说明： dx, dy 计算出来的阵列位置
//       dCurXDis,dCurYDis 本次阵列间距
========================*/

BOOL CDlgPathEdit::CalcBoradArcData(double &dx,double &dy,double dCurXdis,double dCurYdis,double dArc)
{
	// 基板夹角
	double darc = dArc;
	double dSinarc = sin(darc);
	double dCosarc = cos(darc);
	double dSinarcF = sin(-darc);
	double dCosarcF = cos(-darc);

	double tempXData =0;
	double tempYData =0;
    
	double tt1,tt2;
	///////////////////////////////////////// 
	// 先将当前坐标数据从查找模板坐标系(产品坐标)转换到标准模板坐标系(机械坐标)
	//X = Cos(-@)*x - Sin(-@)*y;
	//Y = Sin(-@)*x + Cos(-@)*y;
	tempXData = dx *dCosarcF - dSinarcF * dy; 
	tempYData = dx *dSinarcF + dCosarcF * dy; 

	tt1 = tempXData + dCurXdis;
	tt2 = tempYData + dCurYdis;

    // 再将当前坐标数据从标准模板坐标系(机械坐标)转换到查找模板坐标系(产品坐标)
	// X = Cos(@)*x - Sin(@)*y;
	// Y = Sin(@)*x + Cos(@)*y;
	tempXData =0;
	tempYData =0;

	tempXData = dCosarc * tt1 - dSinarc * tt2;
	tempYData = dSinarc * tt1 + dCosarc * tt2;

	dx = tempXData;
	dy = tempYData;

	///////////////////////////////////////////////
/*
	tempXData =0;
	tempYData =0;
	tempXData = dCurRX *dCosarc - dCurRY *dSinarc;
	tempYData = dCurRX *dSinarc + dCurRY *dCosarc;

	dx += tempXData; 
	dy += tempYData; 
*/
  return TRUE;
}
// 指令修改
int CDlgPathEdit::ModifiedCmd(int iCmdIndex)
{
	if(iCmdIndex<0) 
		return IDOK;

//	g_pView->ImgStop();

	POSITION posIndex = m_rawList.FindIndex(iCmdIndex);
	tgCmdLine &pLine = m_rawList.GetAt(posIndex);

	int i, j, k, m, q, ptime=0, stepcount=0;
	//////////////////////////////////////////////////////////////////////////
	CDlgRepeat dlgRepeat;
	CDlgGoto dlgGoto;
	CDlgAxisPos dlgAxisPos;
	CDlgNote dlgNote("注释");
	CDlgNote dlgSubProgStart("0");
	CDlgNote dlgSubProgEnd("0");
	CDlgGoSub dlgGoSub;
	CDlgLoop dlgLoop;
	CDlgIOSet dlgIOSet;
	CDLgSpeed dlgSpeed;
	CDlgLineParam dlgLineParam;
	CDlgZCLearance dlgZClearance;
	CDlgZSet dlgZSet;
	CDlgDelay dlgDelay;
	CDlgPointParam dlgPointParam;
	CDlgPointEnd dlgPointEnd;
	CDlgLineEnd dlgLineEnd;
	CDlgCircle dlgCircle;
	CDlgDispenser dlgDispenser;
	CDlgStandBy dlgStandby;
	CDlgTool dlgTool;
	CDlgJetParam dlgJetParam;
	CDlgRepeatZheight dlgRepeatZH;

	////////////////////////////////////////
    CDlgHStartPoint   dlgHPointStart;
	CDlgHWorkPoint    dlgHPointWork;
	CDlgHEndPoint     dlgHPointEnd;
	CDlgHDot		  dlgHDot;
  
	int iDlgResult = IDOK;

	CString strTemp;
	strTemp.Format("Modify CMD：%s", szCmdline[pLine.tType]);
	g_pFrm->AddMsg(strTemp);
////////////////////////////////////////////////////////// 
	// 基板夹角

	double dCurRX=0; // 阵列矩阵内各个结点起始位置 X坐标  
	double dCurRY=0;

    int nRStartCMDID;      // 阵列起始指令号
    int nRType   ;         // 0-s,1-N
    int nRXNum   ;         // 阵列X个数
    int nRYNum   ;         // 阵列Y个数
    double dRYDis,dRXDis;  // 阵列间距
    double dRArc;          // 阵列夹角
	switch(pLine.tType)
	{
	case CMD_MARK:
		//非模态
		g_pFrm->ShowMarkDialog(&pLine, iCmdIndex);
		break;
	case CMD_DELAY://延时
		dlgDelay.m_pCmdLine = &pLine;
		iDlgResult = dlgDelay.DoModal();
		break;
	case CMD_CIRCLE:  // 圆形指令
		//非模态
		g_pFrm->ShowCircleDialog(&pLine,iCmdIndex);
         break;
	case CMD_ZMETER:  // 激光测高
		g_pFrm->ShowZHEIGHTDialog(&pLine,iCmdIndex);	
		break;
	case CMD_WCL: // 称重划线
		g_pFrm->ShowWCLDialog(&pLine,iCmdIndex);
		break;
	case CMD_WCRECT:
		g_pFrm->ShowWCRectDialog(&pLine,iCmdIndex);
		break;
	case CMD_POT: // 称重打点
		g_pFrm->ShowWCPDialog(&pLine,iCmdIndex);
		break;
	case CMD_REPEATX_ZH:  // 激光测高阵列
	case CMD_REPEATY_ZH:  
		g_pFrm->ShowRepeatZHDialog(&pLine,iCmdIndex);	
		break;
	case CMD_POINT:      //
	case CMD_LINE_START:
	case CMD_LINE_PASSING:
	case CMD_LINE_ARC:
	case CMD_LINE_END:
	case CMD_PAUSE:
	case CMD_DUMMY:
		//非模态
		g_pFrm->ShowAxisPosDialog(&pLine, iCmdIndex);
		break;
	case CMD_WAIT_POINT:  //待机点 
		dlgStandby.m_pCmdLine = &pLine;
		iDlgResult = dlgStandby.DoModal();
		break;
	case CMD_DISPENSER:  // 阀开关指令
		dlgDispenser.m_pCmdLine = &pLine;
		iDlgResult = dlgDispenser.DoModal();
		break;
	case CMD_TOOL:      // 选择阀指令
		dlgTool.m_pCmdLine = &pLine;
		iDlgResult = dlgTool.DoModal();
	case CMD_HOME:     // 试教回零
		break;
	case CMD_REPEATARC:   // 夹角标定界面
        g_pFrm->ShowBoardCheckDlgEx(&pLine,iCmdIndex);	
		break;
	case CMD_GOTO_ADDRESS: // 指令跳转
		dlgGoto.m_pCmdLine = &pLine;
		iDlgResult = dlgGoto.DoModal();
		break;
	case CMD_CALL_SUB:     // 调用子函数
		dlgGoSub.m_pCmdLine = &pLine;
		iDlgResult = dlgGoSub.DoModal();
		break;
	case CMD_NULL_LINE:    // 空行
		break;
	case CMD_LABEL:        // 标号
		dlgSubProgStart.m_pCmdLine = &pLine;
		dlgSubProgStart.m_iSelIndex = iCmdIndex;
		iDlgResult = dlgSubProgStart.DoModal();
		break;
	case CMD_LABEL_END:        // 标号结束
		dlgSubProgEnd.m_pCmdLine = &pLine;
		dlgSubProgEnd.m_iSelIndex = iCmdIndex;
		iDlgResult = dlgSubProgStart.DoModal();
		break;
	case CMD_REPEATX:      // X阵列
		dlgRepeat.m_pCmdLine = &pLine;
		dlgRepeat.m_iSelIndex = iCmdIndex;
		iDlgResult = IDCANCEL;

		if(dlgRepeat.DoModal() == IDOK)
		{  
			nRStartCMDID = pLine.pIntValue[0];   // 阵列起始指令号
			nRType       = pLine.pIntValue[1];   // 0-s,1-N
			nRXNum       = pLine.pIntValue[2];   // 阵列X个数
			nRYNum       = pLine.pIntValue[3];   // 阵列Y个数
            dRXDis       = pLine.pDblValue[0];   // 阵列X间隔
			dRYDis       = pLine.pDblValue[1];   // 阵列X间隔
			dRArc        = pLine.pDblValue[2];   // 阵列夹角
			iDlgResult = IDOK;
			if(dlgRepeat.m_bExpand)
			{	// 阵列展开
				if(IsHaveRepeat(iCmdIndex))
				{
					AfxMessageBox("该指令后存在阵列指令不能执行展开,\n阵列只能展开最后一个阵列指令.");
					break;
				}
				m=1;
				ptime=0;   // 阵列次数
				stepcount = iCmdIndex - nRStartCMDID;   // 阵列包含指令个数   
				//
				for(i=0; i<nRYNum;i++)                  //  阵列 Y 个数
				{
					for(j=0; j< nRXNum; j++)            //  阵列 x 个数
					{
						if(i==0 && j==0)
						{
							ptime++;   // 第一个阵列结点
						    continue;
						}
						for(k= nRStartCMDID; k<iCmdIndex; k++)
						{  // 从起始阵列行 到 当前行指令 
                            
							tTempCmdPoint=m_rawList.GetAt(m_rawList.FindIndex(k));

							if (tTempCmdPoint.tType==CMD_MARK)
								continue;
                              // 阵列中有阵列
							if(tTempCmdPoint.tType == CMD_REPEATX || tTempCmdPoint.tType == CMD_REPEATY)
								tTempCmdPoint.pIntValue[0] += ptime * stepcount; 

							for(q=0; q<tTempCmdPoint.iDotNum; q++)
							{   // 当前阵列指令数据的复制

								if(nRType ==0) //S
								{
									if((i%2) == 0)
										dCurRX =  (dRXDis*j);     // x 间隔
									else
										dCurRX = (dRXDis*(nRXNum -1-j));//x
								}
								else//N
								{
									dCurRX = (dRXDis*j);     //x
								}

								dCurRY = (dRYDis*i);         //y
                                 
								/////////////////////////////////////////  
           
								CalcBoradArcData(tTempCmdPoint.pDblValue[q*4+0],tTempCmdPoint.pDblValue[q*4+1],dCurRX,dCurRY,dRArc);
								
							}	
							
							m_rawList.InsertAfter(m_rawList.FindIndex(iCmdIndex+m-1), tTempCmdPoint);
							m++;
						}
						ptime++;
					}//end of j[x]
				}//end of of i[y]
				m_rawList.RemoveAt(posIndex);
			}
		}
		break;
	case CMD_REPEATY:
		dlgRepeat.m_pCmdLine = &pLine;
		dlgRepeat.m_iSelIndex = iCmdIndex;
		iDlgResult = IDCANCEL;
		if(dlgRepeat.DoModal() == IDOK)
		{
			nRStartCMDID = pLine.pIntValue[0];   // 阵列起始指令号
			nRType       = pLine.pIntValue[1];   // 0-s,1-N
			nRXNum       = pLine.pIntValue[2];   // 阵列X个数
			nRYNum       = pLine.pIntValue[3];   // 阵列Y个数
			dRXDis       = pLine.pDblValue[0];   // 阵列X间隔
			dRYDis       = pLine.pDblValue[1];   // 阵列X间隔
			dRArc        = pLine.pDblValue[2];   // 阵列夹角
			iDlgResult = IDOK;
			if(dlgRepeat.m_bExpand)
			{
				if(IsHaveRepeat(iCmdIndex))
				{
					AfxMessageBox("该指令后存在阵列指令不能执行展开,\n阵列只能展开最后一个阵列指令.");
					break;
				}
				m=1;
				ptime=0;
				stepcount = iCmdIndex - nRStartCMDID;  // 指令个数
				//
				for(i=0; i<nRXNum;i++)//x
				{
					for(j=0; j<nRYNum; j++)//y
					{
						if(i==0 && j==0)
						{
							ptime++;
							continue;
						}
						for(k=nRStartCMDID; k<iCmdIndex; k++)
						{
							tTempCmdPoint=m_rawList.GetAt(m_rawList.FindIndex(k));
							if (tTempCmdPoint.tType==CMD_MARK)continue;
							if(tTempCmdPoint.tType == CMD_REPEATX || tTempCmdPoint.tType == CMD_REPEATY)
								tTempCmdPoint.pIntValue[0] += ptime * stepcount; 
							//
							for(q=0; q<tTempCmdPoint.iDotNum; q++)  
							{
								if(pLine.pIntValue[1]==0)//S
								{
									if((i%2)==0)
										dCurRY = dRYDis*j;//y
									else
										dCurRY= dRYDis*(nRYNum-1-j);//y
								}
								else//N
								{
									dCurRY = dRYDis*j;
								}

                                dCurRX = dRXDis*i;//x
								////
								CalcBoradArcData(tTempCmdPoint.pDblValue[q*4+0],tTempCmdPoint.pDblValue[q*4+1],dCurRX,dCurRY,dRArc);

							}
							m_rawList.InsertAfter(m_rawList.FindIndex(iCmdIndex+m-1), tTempCmdPoint);
							m++;
						}
						ptime++;
					}//end of j[y]
				}//end of i[y]
				m_rawList.RemoveAt(posIndex);
			}
		}
		break;

	case CMD_SET_IO: // Io 指令
		dlgIOSet.m_pCmdLine = &pLine;
		iDlgResult =dlgIOSet.DoModal();
		break;
	case CMD_LOOP: // 循环
		dlgLoop.m_pCmdLine = &pLine;
		iDlgResult =dlgLoop.DoModal();
		break;
	case CMD_NOTE:  // 注释
		dlgNote.m_pCmdLine = &pLine;
		iDlgResult =dlgNote.DoModal();
		break;
	case PARAM_LINE_SPEED:   // 速度指令
	case PARAM_XY_VEL:
	case PARAM_Z_VEL:
		dlgSpeed.m_pCmdLine = &pLine;
		iDlgResult =dlgSpeed.DoModal();
		break;
	case PARAM_POINT_END:    // 单点计算提升速度
		dlgPointEnd.m_pCmdLine = &pLine;
		iDlgResult =dlgPointEnd.DoModal();
		break;
	case PARAM_LINE_SETUP:   // 线段参数
		dlgLineParam.m_pCmdLine = &pLine;
		iDlgResult =dlgLineParam.DoModal();
		break;
	case PARAM_LINE_END:     // 线段结束方式
		dlgLineEnd.m_pCmdLine = &pLine;
		iDlgResult =dlgLineEnd.DoModal();
		break;
	case PARAM_POINT_SETUP:  // 单点参数
		dlgPointParam.m_pCmdLine = &pLine;
		iDlgResult =dlgPointParam.DoModal();
		break;
	case PARAM_JET_SETUP:     
		dlgJetParam.m_pCmdLine = &pLine;
		iDlgResult =dlgJetParam.DoModal();
		break;
	case PARAM_Z_CLEARANCE://单点点胶结束或线段点胶结束时Z轴提高高度
		dlgZClearance.m_pCmdLine = &pLine;
		iDlgResult =dlgZClearance.DoModal();
		break;
    case PARAM_Z_WORKPOSSET://
		//dlgZClearance.m_pCmdLine = &pLine;
		//iDlgResult =dlgZClearance.DoModal();
		break;
	case PARAM_Z_SET_HEIGHT: // Z 高度坐标设置
		dlgZSet.m_pCmdLine = &pLine;
		iDlgResult = dlgZSet.DoModal();
		break;
	case CMD_SUPERSTART:
		dlgHPointStart.m_pCmdLine = &pLine;
		iDlgResult = dlgHPointStart.DoModal();
		break;
	case CMD_SUPERPOINT:
		dlgHPointWork.m_pCmdLine = &pLine;
		iDlgResult = dlgHPointWork.DoModal();
		break;
	case CMD_SUPEREND:
		dlgHPointEnd.m_pCmdLine = &pLine;
		iDlgResult = dlgHPointEnd.DoModal();
		break;
	case CMD_SUPERDOT:
		dlgHDot.m_pCmdLine = &pLine;
		iDlgResult = dlgHDot.DoModal();
		break;
	default:NULL;
	}

	return iDlgResult;
}

BOOL CDlgPathEdit::GetEndPos(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py)
{//获取结束点提前断胶点的坐标----展开前临时列表
	if(pCmd->tType != CMD_LINE_END)
		return FALSE;
	
	int m = GetPreTrackPoint(iCmdIndex);
	if(m<0)return FALSE;
	if(m_rawList.GetAt(m_rawList.FindIndex(m)).tType == CMD_LINE_ARC)
	{
		int n = GetPreTrackPoint(m);
		if(n<0)return FALSE;

		tgArc tarc;
		GetCmdArcInfo(m_rawList.GetAt(m_rawList.FindIndex(m)), m, &tarc);
		double dArcLen = fabs(tarc.angle/180*M_PI * tarc.r);
		if(dArcLen<dLen)
		{
			*px = m_rawList.GetAt(m_rawList.FindIndex(n)).pDblValue[0];
			*py = m_rawList.GetAt(m_rawList.FindIndex(n)).pDblValue[1];
		}
		else
		{
			double dangle = dLen/tarc.r;
			if(tarc.angle>0)
			{
				*px = tarc.x + tarc.r * cos(tarc.dEndAngle/180*M_PI-dangle);
				*py = tarc.y + tarc.r * sin(tarc.dEndAngle/180*M_PI-dangle);
			}
			else
			{
				*px = tarc.x + tarc.r * cos(tarc.dEndAngle/180*M_PI+dangle);
				*py = tarc.y + tarc.r * sin(tarc.dEndAngle/180*M_PI+dangle);
			}
		}
	}
	else
	{
		double dLineLen = sqrt((m_rawList.GetAt(m_rawList.FindIndex(m)).pDblValue[0] - pCmd->pDblValue[0])*(m_rawList.GetAt(m_rawList.FindIndex(m)).pDblValue[0] - pCmd->pDblValue[0])
			+ (m_rawList.GetAt(m_rawList.FindIndex(m)).pDblValue[1] - pCmd->pDblValue[1])*(m_rawList.GetAt(m_rawList.FindIndex(m)).pDblValue[1] - pCmd->pDblValue[1]));
		if(dLineLen < dLen)
		{
			*px = m_rawList.GetAt(m_rawList.FindIndex(m)).pDblValue[0];
			*py = m_rawList.GetAt(m_rawList.FindIndex(m)).pDblValue[1];
		}
		else
		{
			double dangle = atan2(m_rawList.GetAt(m_rawList.FindIndex(m)).pDblValue[1] - pCmd->pDblValue[1], m_rawList.GetAt(m_rawList.FindIndex(m)).pDblValue[0] - pCmd->pDblValue[0]);
			*px = pCmd->pDblValue[0] + dLen * cos(dangle);
			*py = pCmd->pDblValue[1] + dLen * sin(dangle);
		}
	}

	return TRUE;
}

BOOL CDlgPathEdit::GetEndPosEx(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py)
{//获取结束点提前断胶点的坐标----展开后运行列表
	if(pCmd->tType != CMD_LINE_END)
		return FALSE;

	int m = GetPreTrackPointEx(iCmdIndex);
	if(m<0)return FALSE;
	if(g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).tType == CMD_LINE_ARC)
	{
		int n = GetPreTrackPointEx(m);
		if(n<0)return FALSE;

		tgArc tarc;
		GetCmdArcInfoEx(&g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m-1)),&g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)),&g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m+1)),&tarc);
		double dArcLen = fabs(tarc.angle/180*M_PI * tarc.r);
		if(dArcLen<dLen)
		{
			*px = g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(n)).pDblValue[0];
			*py = g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(n)).pDblValue[1];
		}
		else
		{
			double dangle = dLen/tarc.r;
			if(tarc.angle>0)
			{
				*px = tarc.x + tarc.r * cos(tarc.dEndAngle/180*M_PI-dangle);
				*py = tarc.y + tarc.r * sin(tarc.dEndAngle/180*M_PI-dangle);
			}
			else
			{
				*px = tarc.x + tarc.r * cos(tarc.dEndAngle/180*M_PI+dangle);
				*py = tarc.y + tarc.r * sin(tarc.dEndAngle/180*M_PI+dangle);
			}
		}
	}
	else
	{
		double dLineLen = sqrt((g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).pDblValue[0] - pCmd->pDblValue[0])*
			(g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).pDblValue[0] - pCmd->pDblValue[0])
			+ (g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).pDblValue[1] - pCmd->pDblValue[1])*
			(g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).pDblValue[1] - pCmd->pDblValue[1]));
		if(dLineLen < dLen)
		{
			*px = g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).pDblValue[0];
			*py = g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).pDblValue[1];
		}
		else
		{
			double dangle = atan2(g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).pDblValue[1] - pCmd->pDblValue[1], 
				g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).pDblValue[0] - pCmd->pDblValue[0]);
			*px = pCmd->pDblValue[0] + dLen * cos(dangle);
			*py = pCmd->pDblValue[1] + dLen * sin(dangle);
		}
	}

	return TRUE;
}

BOOL CDlgPathEdit::GetEndActPos(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py,int nType)
{//结束动作临时点获取
	if(pCmd->tType != CMD_LINE_END)
		return FALSE;

	int m = GetPreTrackPointEx(iCmdIndex);
	if(m<0)return FALSE;
	if(g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).tType == CMD_LINE_ARC)
	{
		int n = GetPreTrackPointEx(m);
		if(n<0)return FALSE;

		tgArc tarc;
		GetCmdArcInfoEx(&g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m-1)),&g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)),&g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m+1)), &tarc);
	//	double dArcLen = fabs(tarc.angle/180*3.1415926 * tarc.r);
		double dangle = dLen/tarc.r;
		if(tarc.angle>0)
		{
			switch(nType)
			{
			case 0:
				*px = tarc.x;
				*py = tarc.y;
				break;
			case 1:
			case 2:
				*px = tarc.x + tarc.r * cos(tarc.dEndAngle/180*M_PI-dangle);
				*py = tarc.y + tarc.r * sin(tarc.dEndAngle/180*M_PI-dangle);
				break;
			case 6:
			case 7:
				*px = tarc.x + tarc.r * cos(tarc.dEndAngle/180*M_PI+dangle);
				*py = tarc.y + tarc.r * sin(tarc.dEndAngle/180*M_PI+dangle);
				break;
			}			
		}
		else
		{
			switch(nType)
			{
			case 0:
				*px = tarc.x;
				*py = tarc.y;
				break;
			case 1:
			case 2:
				*px = tarc.x + tarc.r * cos(tarc.dEndAngle/180*M_PI+dangle);
				*py = tarc.y + tarc.r * sin(tarc.dEndAngle/180*M_PI+dangle);
				break;
			case 6:
			case 7:
				*px = tarc.x + tarc.r * cos(tarc.dEndAngle/180*M_PI-dangle);
				*py = tarc.y + tarc.r * sin(tarc.dEndAngle/180*M_PI-dangle);
				break;
			}		
		}
	}
	else
	{
		double dangle = atan2(g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).pDblValue[1] - pCmd->pDblValue[1], 
				g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(m)).pDblValue[0] - pCmd->pDblValue[0]);
			

			switch(nType)
			{
			case 0:
				*px = pCmd->pDblValue[0] ;
				*py = pCmd->pDblValue[1] ;
				break;
			case 1:
			case 2:
				*px = pCmd->pDblValue[0] + dLen * cos(dangle);
				*py = pCmd->pDblValue[1] + dLen * sin(dangle);
				break;
			case 6:
			case 7:
				*px = pCmd->pDblValue[0] - dLen * cos(dangle);
				*py = pCmd->pDblValue[1] - dLen * sin(dangle);
				break;
			}
	}

	return TRUE;
}

BOOL CDlgPathEdit::GetEndCirPos(tgCircle& tArc,double dfStrtAngle,double dLen, double *px, double *py,int nType)
{//圆弧结束动作临时点获取
	double dfAngle;
	double dfR = tArc.r;
	dfAngle = dLen/dfR;
	switch(nType)
	{
		case 0:
			*px = tArc.x + dfR * cos( dfStrtAngle );
			*py = tArc.y + dfR * sin( dfStrtAngle);
			break;
		case 1:
		case 2:
			*px = tArc.x + dfR * cos( dfStrtAngle- dfAngle );
			*py = tArc.y + dfR * sin( dfStrtAngle - dfAngle );
			break;
		case 6:
		case 7:
			*px = tArc.x + dfR * cos(dfStrtAngle + dfAngle );
			*py = tArc.y + dfR * sin(dfStrtAngle + dfAngle );
			break;
	}
	return TRUE;
}

BOOL CDlgPathEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化

	CImageList blankimagelist;
	blankimagelist.Create(1, 30, TRUE | ILC_COLOR32, 1, 0);
	m_wndList.SetImageList(&blankimagelist, LVSIL_SMALL);	//使用空白图片增加行高

	// 获取主窗口大小
	CRect rcMainfram;
	GetClientRect(rcMainfram);
	
// 	m_wndList.Create(WS_VISIBLE|LVS_REPORT|LVS_SHOWSELALWAYS, CRect(0,0,10,10), this, IDS_COMMAND_LIST);
 	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT/*|LVS_EX_CHECKBOXES*/);

	m_wndList.InsertColumn(0, "序号", 0, 50);
	m_wndList.InsertColumn(1, "类型", 0, 130);
	if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
	{
		m_wndList.InsertColumn(2, "胶量(mg)", 0, 70);
		m_wndList.InsertColumn(3, "提前开/关胶(mm)", 0, 100);
	}
	else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
	{	// 针头点胶时不显示胶量和提前开关胶
		m_wndList.InsertColumn(2, "胶量(mg)", 0, 0);
		m_wndList.InsertColumn(3, "提前开/关胶(mm)", 0, 0);
	}
	m_wndList.InsertColumn(4, "X坐标(mm)", 0, 75);
	m_wndList.InsertColumn(5, "Y坐标(mm)", 0, 75);
	// 参数页不显示
	m_wndList.InsertColumn(6, "参数", 0, 0);
	m_wndList.InsertColumn(7, "类型参数", 0, 100);

//	MoveWindow(rcMainfram.left, rcMainfram.bottom-tmpHeight-50, rcMainfram.Width(), tmpHeight+50);

	// 马达运动窗口大小
	CRect rc;
	m_wndShotCutEdit.Create(IDD_DLG_SHOTCUT, this);
	m_wndShotCutEdit.GetClientRect(rc);
	int tmpWidth = rc.Width();
	int tmpHeight = rc.Height();

	if (m_wndShotCutEdit.GetSafeHwnd() != NULL)
	{	
		GetClientRect(rc);		
		m_wndShotCutEdit.SetWindowPos(NULL, rcMainfram.Width()-tmpWidth, 0, 
			tmpWidth, tmpHeight, SWP_NOZORDER|SWP_NOACTIVATE);

//		m_wndShotCutEdit.MoveWindow(rcMainfram.Width()-tmpWidth, 0, tmpWidth, tmpHeight);

// 		GetDlgItem(IDC_BUTTON_EDIT0)->MoveWindow(0, 0, 48, 48);
// 		GetDlgItem(IDC_BUTTON_EDIT1)->MoveWindow(0,48, 48, 48);
// 		GetDlgItem(IDC_BUTTON_EDIT2)->MoveWindow(0,48*2, 48, 48);
// 		GetDlgItem(IDC_BUTTON_EDIT3)->MoveWindow(0,48*3, 48, 48);
// 		GetDlgItem(IDC_BUTTON_EDIT4)->MoveWindow(0,48*4, 48, 48);

//		GetDlgItem(IDC_BTN_SHOWPATH)->MoveWindow(0,48*5, 48, 48);

// 		CRect rcBtn;
// 		GetDlgItem(IDC_BUTTON_EDIT_SINGLE)->GetWindowRect(rcBtn);
// 		int nWidthBtn = rcBtn.Width();
// 		int nHeightBtn = rcBtn.Height();
// 		m_wndList.MoveWindow(48, 0, rcMainfram.Width()-48-tmpWidth-nWidthBtn, rc.Height());

//		GetDlgItem(IDC_BUTTON_EDIT_SINGLE)->MoveWindow(rcMainfram.Width()-tmpWidth-nWidthBtn, 2, nWidthBtn, nHeightBtn);
//		GetDlgItem(IDC_BUTTON_EDIT_ALL)->MoveWindow(rcMainfram.Width()-tmpWidth-nWidthBtn, 4+nHeightBtn, nWidthBtn, nHeightBtn);
	}

	{
		m_btnEdit[0].SetTooltipText("单点");
		m_btnEdit[1].SetTooltipText("线段起点");
		m_btnEdit[2].SetTooltipText("线段中间点");
		m_btnEdit[3].SetTooltipText("圆弧中间点");
		m_btnEdit[4].SetTooltipText("线段终点");

		m_btnEdit[5].SetIcon(IDI_ICON_SINGLE_ADJUST);
		m_btnEdit[5].SetTooltipText("修改选中的轨迹坐标为当前坐标值");
		m_btnEdit[6].SetIcon(IDI_ICON_ALL_ADJUST);
		m_btnEdit[6].SetTooltipText("所有轨迹坐标偏移");

		m_btnEdit[7].SetTooltipText("自动查找正面点胶的Mark点,然后修正轨迹");
		m_btnEdit[8].SetTooltipText("自动查找背面点胶的Mark点,然后修正轨迹");

		m_btnEdit[9].SetIcon(IDI_ICON_SAVE_S);
		m_btnEdit[9].SetTooltipText("保存轨迹数据");

		for (int i=0; i<10; i++)
		{
			m_btnEdit[i].OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
		}
	}

	// 编辑框
	m_fontEdit.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,
		CLIP_STROKE_PRECIS,DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("Arial"));
	m_editModify.ShowWindow(SW_HIDE);
	m_editModify.SetFont(&m_fontEdit);

	GetDlgItem(IDC_COMBO_MODIFY)->ShowWindow(SW_HIDE);	

	haveccomboboxcreate = false;	// 初始化标志位，表示还没有创建下拉列表框

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgPathEdit::EnableEditButton(BOOL bEnable)
{
	GetDlgItem(IDC_BUTTON_EDIT0)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_EDIT1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_EDIT2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_EDIT3)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_EDIT4)->EnableWindow(bEnable);
}

void CDlgPathEdit::OnBnClickedBtnShowpath()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->ShowPathDrawDlg();
}

void CDlgPathEdit::OnBnClickedButtonEditSingle()
{
	// TODO: 在此添加控件通知处理程序代码
	if(theApp.m_SysUser.m_CurUD.level<=GM_PERSONNEL)
	{
		AfxMessageBox("请登录管理员权限！");
		return;
	}

	if (IDYES == AfxMessageBox("是否更新当前点位", MB_YESNO))
	{
		int nSel = GetCmdCurSel();

		POSITION posIndex = m_rawList.FindIndex(nSel);
		tgCmdLine &pLine = m_rawList.GetAt(posIndex);
		LoadListArrayPos(pLine, 0);

		UpdateCmdListBox(nSel);
	}
}


void CDlgPathEdit::OnBnClickedButtonEditAll()
{
	if(theApp.m_SysUser.m_CurUD.level<=GM_PERSONNEL)
	{
		AfxMessageBox("请登录管理员权限！");
		return;
	}

	if (IDYES == AfxMessageBox("是否更新所有点位", MB_YESNO))
	{
		int nSel = GetCmdCurSel();

		POSITION posIndex = m_rawList.FindIndex(nSel);
		tgCmdLine &pLine = m_rawList.GetAt(posIndex);

		tgPos tTempOffset;
		g_pFrm->m_Precision.GetCurPosMM();
		double dCrtX = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[0];
		double dCrtY = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[1];

		tTempOffset.x = dCrtX - pLine.pDblValue[0];
		tTempOffset.y = dCrtY - pLine.pDblValue[1];
		tTempOffset.z = 0;

		int m_iStartIndex = 0;
		int m_iEndIndex = m_rawList.GetCount()-1;

		//这里将偏移更新到程序列表中
		if(g_pFrm->m_Robot->RunListOffsetChange(m_iStartIndex,m_iEndIndex,tTempOffset.x,tTempOffset.y,tTempOffset.z))//更新成功
		{
			g_pFrm->m_wndEditBar.UpdateCmdListBox(g_pFrm->m_wndEditBar.m_wndList.GetItemCount()-1);
		}
	}
}


void CDlgPathEdit::OnKillfocusEditModify()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csTemp = "";
	m_editModify.GetWindowText(csTemp);
	if (csTemp.GetLength() != 0)
	{
		//失去焦点的时候将原来选中的单元格设置为编辑框的值
		m_wndList.SetItemText(m_nClickRow, m_nClickCol, _T(csTemp));	

		POSITION posIndex = m_rawList.FindIndex(m_nClickRow);
		tgCmdLine &pLine = m_rawList.GetAt(posIndex);
		
		if (IsTrackPoint(pLine))
		{
			if (2 == m_nClickCol)	pLine.pDblValue[3] = atof(csTemp);
			if (3 == m_nClickCol)	pLine.pDblValue[4] = atof(csTemp);
			if (4 == m_nClickCol)	pLine.pDblValue[0] = atof(csTemp);
			if (5 == m_nClickCol)	pLine.pDblValue[1] = atof(csTemp);
		}
	}

	int nSel = GetCmdCurSel();
	UpdateCmdListBox(nSel);
	m_editModify.ShowWindow(SW_HIDE);
}


void CDlgPathEdit::OnBnClickedBtnReviewF()
{
	// TODO: 在此添加控件通知处理程序代码
	if (K_RUN_STS_STOP != g_pFrm->m_Robot->m_tSysStatus)
	{
		g_pFrm->AddMsg("停止状态才能重温点胶轨迹");
		return;
	}
	if(theApp.m_SysUser.m_CurUD.level<=GM_PERSONNEL)
	{
		AfxMessageBox("请登录管理员权限！");
		return;
	}	
	
	if (IDYES != AfxMessageBox("将要查找Mark点并自动修正正面点胶轨迹，是否继续!", MB_YESNO))	return;
	g_pFrm->m_Robot->GotoSafeZpos();

	int iCount=g_pFrm->m_wndEditBar.m_rawList.GetCount();
	POSITION posIndex;
	tgCmdLine  tgCmd1;
	int i;
	for (i=0; i<iCount; i++)
	{
		posIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(i);
		tgCmd1 = g_pFrm->m_wndEditBar.m_rawList.GetAt(posIndex);

		if (tgCmd1.tType == CMD_ReverseBoard)
		{
			g_pFrm->AddMsg("正面轨迹没有Mark点指令");
			return;
		}

		if (tgCmd1.tType == CMD_MARK)   
			break;
	}

	if (tgCmd1.tType != CMD_MARK)
	{
		g_pFrm->AddMsg("没有Mark点指令");
		return;
	}

	if (!g_pFrm->m_Robot->SubFindMarkEx(&tgCmd1))
	{
		g_pFrm->AddMsg("查找Mark点失败!");
		return;
	}

	tgCmdLine &tgCmdMark = g_pFrm->m_wndEditBar.m_rawList.GetAt(posIndex);
	tgCmdMark.pDblValue[0] = g_pFrm->m_Robot->m_tMarkFindPosEX[0][0].x;
	tgCmdMark.pDblValue[1] = g_pFrm->m_Robot->m_tMarkFindPosEX[0][0].y;
	tgCmdMark.pDblValue[4] = g_pFrm->m_Robot->m_tMarkFindPosEX[0][1].x;
	tgCmdMark.pDblValue[5] = g_pFrm->m_Robot->m_tMarkFindPosEX[0][1].y;

	double dx=0;
	double dy=0;

	for (; i<iCount; i++)
	{
		posIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(i);
		tgCmdLine &tgCmdTemp = g_pFrm->m_wndEditBar.m_rawList.GetAt(posIndex);
		tgCmd1 = tgCmdTemp;

		if (CMD_ReverseBoard == tgCmdTemp.tType)
		{
			break;
		}

		if (CMD_REPEATX == tgCmdTemp.tType || CMD_REPEATY == tgCmdTemp.tType)
		{
			double dPosX[4], dPosY[4];
			g_pFrm->m_Robot->GetRepeatPosition(&tgCmdTemp, dPosX, dPosY);
			tgCmdTemp.pDblValue[0] = dPosX[0];
			tgCmdTemp.pDblValue[1] = dPosY[0];
			tgCmdTemp.pDblValue[4] = dPosX[1];
			tgCmdTemp.pDblValue[5] = dPosY[1];
			tgCmdTemp.pDblValue[8] = dPosX[2];
			tgCmdTemp.pDblValue[9] = dPosY[2];
		}

		if (IsTrackPoint(tgCmdTemp))
		{
			g_pFrm->m_Robot->GetCmdCameraPosition(&tgCmd1, &dx, &dy);
			tgCmdTemp.pDblValue[0] = dx;
			tgCmdTemp.pDblValue[1] = dy;
		}
		UpdateCmdListBox(i);
	}
	g_pView->Invalidate();
}


void CDlgPathEdit::OnBnClickedBtnReviewB()
{
	// TODO: 在此添加控件通知处理程序代码
	if (K_RUN_STS_STOP != g_pFrm->m_Robot->m_tSysStatus)
	{
		g_pFrm->AddMsg("停止状态才能重温点胶轨迹");
		return;
	}
	if(theApp.m_SysUser.m_CurUD.level<=GM_PERSONNEL)
	{
		AfxMessageBox("请登录管理员权限！");
		return;
	}
}

void CDlgPathEdit::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	g_AllProgInfo[0].HandleAllProg(FALSE);
}

//pEditCtrl为列表对象指针，createccombobox为下拉列表框指针对象，
//Item为创建单元格在列表中的行，SubItem则为列，havecreat为对象创建标准
void CDlgPathEdit::CreateCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat)//创建单元格下拉列表框函数
{
	Item = pEditCtrl->iItem;			//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	SubItem = pEditCtrl->iSubItem;				//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	havecreat = true;

	createccomboboxobj->Create(WS_CHILD | WS_VISIBLE |  CBS_DROPDOWN | CBS_OEMCONVERT, CRect(0, 0, 10, 20), this, IDC_COMBO_MODIFY);
	createccomboboxobj->SetFont(this->GetFont(), FALSE);						//设置字体,不设置这里的话上面的字会很突兀的感觉
	createccomboboxobj->SetParent(&m_wndList);									//将list control设置为父窗口,生成的Ccombobox才能正确定位,这个也很重要
	
	CRect  EditRect;
	m_wndList.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_wndList.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1和-1可以让编辑框不至于挡住列表框中的网格线
	CString strItem = m_wndList.GetItemText(e_Item, e_SubItem);	//获得相应单元格字符
	createccomboboxobj->SetWindowText(strItem);					//将单元格字符显示在编辑框上
	createccomboboxobj->MoveWindow(&EditRect);					//将编辑框位置放在相应单元格上
	createccomboboxobj->ShowWindow(SW_SHOW);					//显示编辑框在单元格上面
}
void CDlgPathEdit::DistroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem)
{
	CString meditdata;
	//distroyccomboboxobj->GetWindowTextW(meditdata);
	distroyccomboboxobj->GetWindowText(meditdata);
	list->SetItemText(Item, SubItem, meditdata);    // 更新相应单元格字符
	distroyccomboboxobj->DestroyWindow();           // 销毁对象，有创建就要有销毁，不然会报错
}

void CDlgPathEdit::OnKillfocusCcomboBox()
{


//	return;
	CString csTemp = "";
	int iPos=m_CtrlComboBoxModify.GetCurSel();
	m_CtrlComboBoxModify.GetWindowText(csTemp);

	if (haveccomboboxcreate == true)				//如果之前创建了下拉列表框就销毁掉
	{
		DistroyCcombobox(&m_wndList, &m_CtrlComboBoxModify, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}

	if (csTemp.GetLength() != 0)
	{
		//	失去焦点的时候将原来选中的单元格设置为编辑框的值
		m_wndList.SetItemText(m_nClickRow, m_nClickCol, _T(csTemp));	

		POSITION posIndex = m_rawList.FindIndex(m_nClickRow);
		tgCmdLine &pLine = m_rawList.GetAt(posIndex);

		if (7 != m_nClickCol)
		{
			return;
		}

		pLine.pDblValue[7] =iPos;

		int nSel = GetCmdCurSel();
		UpdateCmdListBox(nSel);
	}


//	m_CtrlComboBoxModify.ShowWindow(SW_HIDE);
//	m_editModify.ShowWindow(SW_HIDE);

}

void CDlgPathEdit::OnBnClickedBtnParam()
{
	// TODO: 在此添加控件通知处理程序代码
//	int nSel = GetCmdCurSel();
	if (m_nClickRow<0)
	{
		AfxMessageBox(_T("请选择要修改参数的行"));
		return;
	}
	POSITION posIndex = m_rawList.FindIndex(m_nClickRow);
	tgCmdLine &pLine = m_rawList.GetAt(posIndex);

	if (IsParamPoint(pLine))
	{
		g_pFrm->ShowDotParamDialog(&pLine);
	} 
	else
	{
		g_pFrm->ShowLineParamDialog(&pLine);
	}
}

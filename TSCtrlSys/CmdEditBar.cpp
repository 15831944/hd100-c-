#include "stdafx.h"
#include "CmdEditBar.h"
#include "TSCtrlSys.h"
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
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

tgCmdLine tTempCmdPoint;//临时命令行
//////////////////////////////////////////////////////////////////////////
CString szCmdline[] = 
{
	"NONE",					//CMD_NONE=0,
	"MARK [ ╋ ]",			//Mark设置 ,tgPos[2],int iMarkNum
	"空点",					// 
	"单点 [ ● ]",			//CMD_POINT_DISPENSE,//单点点胶,tgPos[1]
	"线段起点 [●----]",	//CMD_LINE_START,//线段点胶起点tgPos[1]
	"线段中点 [--●--]",	//CMD_LINE_PASSING,// 线段点胶中间点tgPos[1]
	"圆弧中点 [--＾--]",	//CMD_LINE_ARC,//圆弧点胶 tgPos[1]
	"线段终点 [----●]",	//CMD_LINE_END,//线段点胶结束点,tgPos[1]
	"圆形点胶 [ ○ ]",      //CMD_CIRCLE,//圆形点胶 1:圆心+直径, tgPos[1], double diameter, 2:3点画圆,tgPos[3]
	"点胶阀",				//CMD_DISPENSER,//点胶阀开/关 BOOL On/Off
	"跳转",					//CMD_GOTO_ADDRESS,//跳转指令 int iIndex
	"X方向阵列",            //CMD_REPEATX,//X方向阵列 int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	"Y方向阵列",            //CMD_REPEATY,//Y方向阵列 int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	"涂覆",					//CMD_BRUSH_AREA,//涂覆,
	"调用子函数",			//CMD_CALL_SUBROUTINE,//调子程序
	"调用程序",
	"标贴",					//CMD_CALL_PROGRAM,//调用程序
	"    ",					//CMD_NULL_LINE,//空行
	//"等待输入",			//CMD_SET_INPUT,//设置IO状态
	//"输出",				//CMD_SET_OUTPUT,//设置IO状态
	"S设置IO",
	"延时",					//CMD_DELAY,//等待输入的秒数后继续往下执行纪录点。
	"移动到待机位",
	"停止位",				//CMD_STOP_POINT,//针头将移至此特定位置并且停止移动，直到使用者按下机台面板上的START按钮后继续往下执行纪录点
	"待机位",               //CMD_HOME,//X, Y, Z轴将回归软件原点(等待点)后继续往下执行纪录点。
	"循环",					//CMD_LOOP,//通常将循环执行地址设为1，当教导一组程序完成后，可利用此功能来设定运作次数
	"空点",					//CMD_DUNNY,//设定点胶针头实际所在的XYZ 坐标为空点，并记录于目前的纪录点位置。通常应用于避开工件的阻挡。
	"标号",					//CMD_NOTE,
	"清洗",					//清洗喷头
	"激光测高[♂]",			//激光测高度",
	"称重打点[§●]",       // 称重打点
	"称重划线[§----]",		// 称重划线
	"矩形 [□]",
	"选择阀",
	"激光测高阵列X",
	"激光测高阵列Y",
	"单点测试",
	"备用",
	"程序结束",				//CMD_END_PROGRAM

	"线段点胶速度",			//PARAM_LINE_SPEED,//线段点胶速度
	"线段参数",				//PARAM_LINE_SETUP,//线段点胶参数设置
	"线段结束点",			//PARAM_LINE_END,//线段点胶结束设置
	"单点参数设置",			//PARAM_POINT_SETUP,//单点点胶参数设置
	"单点结束设置",			//PARAM_POINT_END,//单点点胶结束设置
	"Z轴提升高度",			//PARAM_Z_CLEARANCE,//Z轴提高高度设置 
	"Z轴极限值",			//PARAM_Z_LIMIT,//Z轴极限值设置 
	"XY空跑速度",			//PARAM_XY_VEL,//XY轴空跑移动速度
	"Z空跑速度",			//PARAM_Z_VEL,//Z轴空跑移动速度
	"Home点",				//PARAM_HOME_POSITION,//设置RUN模式时点胶结束针头停留位置
	"Z轴高度一致",			//PARAM_SET_Z_SAME
	"Z轴点胶高度",			// Z轴点胶高度
	"喷射参数",				// 喷射参数
	"称重矩形[§□]",		// 称重矩形
	//
	"极速喷射起点",			// 飞胶起点
	"极速喷射点",
	"极速喷射结束点",
	"分段加工符",
    "Z轴高度设置",    //
    "拼版加工符",    // 
	"基板夹角标定", 
	"翻板",
	"急速喷射打点",
};
CString szCmdlineEN[] = 
{
	"NONE",					//CMD_NONE=0,
	"MARK [ ╋ ]",			//Mark设置 ,tgPos[2],int iMarkNum
	"NonePoint",			// 
	"PointDisp [ ● ]",		//CMD_POINT_DISPENSE,//单点点胶,tgPos[1]
	"LineStart [●----]",	//CMD_LINE_START,//线段点胶起点tgPos[1]
	"LinPassing [--●--]",	//CMD_LINE_PASSING,// 线段点胶中间点tgPos[1]
	"LineArc [--＾--]",		//CMD_LINE_ARC,//圆弧点胶 tgPos[1]
	"LineEnd [----●]",		//CMD_LINE_END,//线段点胶结束点,tgPos[1]
	"Circle [ ○ ]",			//CMD_CIRCLE,//圆形点胶 1:圆心+直径, tgPos[1], double diameter, 2:3点画圆,tgPos[3]
	"Dispense",				//CMD_DISPENSER,//点胶阀开/关 BOOL On/Off
	"Goto",					//CMD_GOTO_ADDRESS,//跳转指令 int iIndex
	"RepeatX",				//CMD_REPEATX,//X方向阵列 int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	"RepentY",				//CMD_REPEATY,//Y方向阵列 int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	"BrushArea",			//CMD_BRUSH_AREA,//涂覆,
	"CallSubFun.",			//CMD_CALL_SUBROUTINE,//调子程序
	"CallSubPro.",
	"LABEL",				//CMD_CALL_PROGRAM,//调用程序
	"    ",					//CMD_NULL_LINE,//空行
	//"等待输入",			//CMD_SET_INPUT,//设置IO状态
	//"输出",				//CMD_SET_OUTPUT,//设置IO状态
	"SetIO",
	"Delay",				//CMD_DELAY,//等待输入的秒数后继续往下执行纪录点。
	"GotoStandBy",
	"StopPoint",			//CMD_STOP_POINT,//针头将移至此特定位置并且停止移动，直到使用者按下机台面板上的START按钮后继续往下执行纪录点
	"Home",					//CMD_HOME,//X, Y, Z轴将回归软件原点(等待点)后继续往下执行纪录点。
	"Loop",					//CMD_LOOP,//通常将循环执行地址设为1，当教导一组程序完成后，可利用此功能来设定运作次数
	"DunnySpeed",			//CMD_DUNNY,//设定点胶针头实际所在的XYZ 坐标为空点，并记录于目前的纪录点位置。通常应用于避开工件的阻挡。
	"Note",					//CMD_NOTE,
	"Wash",					//清洗喷头
	"Laser_Hight[♂]",		//激光测高度",
	"POT[§●]",			// 称重打点
	"WCLine[§----]",		// 称重划线
	"Rect  [□]",
	"SelectJet",
	"Laser_RepeatX",
	"Laser_RepeatY",
	"SingleDispTest",
	"备用",
	"EndProgram",			//CMD_END_PROGRAM

	"LineSpeed",			//PARAM_LINE_SPEED,//线段点胶速度
	"LineSetup",			//PARAM_LINE_SETUP,//线段点胶参数设置
	"LinEnd",				//PARAM_LINE_END,//线段点胶结束设置
	"PointSetup",			//PARAM_POINT_SETUP,//单点点胶参数设置
	"PointEnd",				//PARAM_POINT_END,//单点点胶结束设置
	"Z_Clearance",			//PARAM_Z_CLEARANCE,//Z轴提高高度设置 
	"Z_Limit",				//PARAM_Z_LIMIT,//Z轴极限值设置 
	"XY_V0",				//PARAM_XY_VEL,//XY轴空跑移动速度
	"Z_V0",					//PARAM_Z_VEL,//Z轴空跑移动速度
	"HomePos",				//PARAM_HOME_POSITION,//设置RUN模式时点胶结束针头停留位置
	"Z_Same",				//PARAM_SET_Z_SAME
	"Z_DispPos",			// Z轴点胶高度
	"JetPara",				// 喷射参数
	"WCRect[§□]",			// 称重矩形
	"Reverse Board",		// 翻板指令
	"H Dots"
};

/////////////////////////////////////////////////////////////////////////////
// CCmdEditBar

CCmdEditBar::CCmdEditBar()
{
	m_bInsertCmd = FALSE;
	m_pMsgList = NULL;
}

CCmdEditBar::~CCmdEditBar()
{
	FreeCmdList();
	m_Tls.RemoveAll();
}
BEGIN_MESSAGE_MAP(CCmdEditBar, CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_COMMAND_RANGE(IDC_BUTTON_EDIT0, IDC_BUTTON_EDIT8, &CCmdEditBar::OnCmdType)
	ON_COMMAND_RANGE(ID_CMD_MARK, ID_PARAM_HOME, &CCmdEditBar::OnCmdType)
	ON_COMMAND_RANGE(ID_CMD_SUPERSTART, ID_CMD_BORATARC, &CCmdEditBar::OnCmdType)
	ON_COMMAND_RANGE(ID_CMD_ZHEIGHT, ID_CMD_ZHEIGHT, &CCmdEditBar::OnCmdType)
	ON_COMMAND_RANGE(ID_CMD_REV_BOARD, ID_CMD_REV_BOARD, &CCmdEditBar::OnCmdType)
//	ON_NOTIFY(NM_CLICK, IDS_COMMAND_LIST, &CCmdEditBar::OnClickListCmd)
	ON_NOTIFY(NM_DBLCLK, IDS_COMMAND_LIST, &CCmdEditBar::OnDblclkListCmd)
	ON_NOTIFY(NM_RCLICK, IDS_COMMAND_LIST, &CCmdEditBar::OnRclickListCmd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDS_COMMAND_LIST, &CCmdEditBar::OnLvnItemchangedListCmd)
	ON_WM_MBUTTONDOWN()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()
			     
///////////////  //////////////////////////////////////////////////////////////
// CCmdEditBar   消息处理程序
int CCmdEditBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy(0, 0, 0, 0);
	m_wndShotCutEdit.Create(IDD_DLG_SHOTCUT, this);

	m_wndList.Create(WS_VISIBLE|LVS_REPORT|LVS_SHOWSELALWAYS, CRect(0,0,10,10), this, IDS_COMMAND_LIST);
	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT/*|LVS_EX_CHECKBOXES*/);
	m_wndList.InsertColumn(0, "序号", 0, 40);
	m_wndList.InsertColumn(1, "类型", 0, 130);
	m_wndList.InsertColumn(2, "参数", 0, 1500);

	return 0;
}

BOOL CCmdEditBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}
void CCmdEditBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	CRect rc;
	g_pView->GetWindowRect(rc);
	if (m_wndShotCutEdit.GetSafeHwnd() != NULL)
	{	
		CRect rect;
		m_wndShotCutEdit.GetClientRect(rect);
		int iHeight = cy;
		m_wndShotCutEdit.SetWindowPos(NULL, 0, iHeight-rect.Height(), cx, rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		
		m_wndList.MoveWindow(0,20, cx, iHeight-rect.Height()-48);
		GetDlgItem(IDC_BUTTON_EDIT0)->MoveWindow(0,iHeight-rect.Height()-48, 48, 48);
	}
}
BOOL CCmdEditBar::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(NULL, _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER, rect, pParentWnd, nID);
}
void CCmdEditBar::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文
}

// void CCmdEditBar::OnClickListCmd(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	UNREFERENCED_PARAMETER(pNMHDR);
// 	SetFocus();
//	g_pView->Invalidate();
//	*pResult=0;
//}

void CCmdEditBar::OnDblclkListCmd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	if(theApp.m_SysUser.m_CurUD.level<=GM_PERSONNEL)
	{
		AfxMessageBox("请登录管理员权限！");
		return;
	}

	ModifiedCmd(GetCmdCurSel());   // 修改指令 
	UpateCmdListBox(GetCmdCurSel());
	*pResult=0;
}
void CCmdEditBar::OnRclickListCmd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CCmdEditBar::ShowTrackPopupMenu(int MenuID)
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
			menu.LoadMenu(IDR_MENU_CMD_INSERT_CN);
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
			menu.LoadMenu(IDR_MENU_CMD_INSERT_EN);
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
			menu.LoadMenu(IDR_MENU_CMD_INSERT);
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

	//if(!g_pView->m_ImgStatic.m_bImgInit)
	//	menu.EnableMenuItem(ID_CMD_MARK,MF_DISABLED|MF_GRAYED);

	if(0==g_pFrm->m_pSysParam->bBalanceEnable)
	{
		menu.EnableMenuItem(IDC_BUTTON_EDIT6,MF_DISABLED|MF_GRAYED);
		menu.EnableMenuItem(IDC_BUTTON_EDIT7,MF_DISABLED|MF_GRAYED);
		menu.EnableMenuItem(IDC_BUTTON_EDIT8,MF_DISABLED|MF_GRAYED);
	}
	
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, p.x+20, p.y, this);
}

void CCmdEditBar::OnLvnItemchangedListCmd(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
//	OnClickListCmd(pNMHDR, pResult);
	*pResult = 0;
}
// 检测当前列表中是否有Mark指令
BOOL CCmdEditBar::CheckMark()
{
	for(int i=0; i<m_rawList.GetCount(); i++)
	{
		POSITION pos=m_rawList.FindIndex(i);
		if(m_rawList.GetAt(pos).tType == CMD_MARK)
			return TRUE;
	}
	return FALSE;
}
void CCmdEditBar::UpateCmdListBox(int iSelIndex)
{
 	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDS_COMMAND_LIST);
	CString strTemp, strText;
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
		
		if(tTempCmdPoint.tType == CMD_NOTE || tTempCmdPoint.tType == CMD_LABEL || tTempCmdPoint.tType == CMD_NULL_LINE)
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
        pList->SetItemText(i, 2, strText);
	}

//	if(g_pDoc != NULL){	g_pDoc->SetModifiedFlag(TRUE);}
	if(g_pFrm != NULL){ g_pFrm->SetWindowTextEx();}

	m_wndList.SetRedraw(TRUE);
	SetCmdCurSel(iSelIndex);

	g_pView->Invalidate();
	g_pFrm->m_Robot->m_bModifiedTrack=TRUE;	
}

BOOL CCmdEditBar::GetCircle(double dX[3], double dY[3], double *pX, double *pY, double *pR)
{
	CPointGE pGE;
	pGE.CaclCirclePara(dX[0],dY[0],dX[1],dY[1],dX[2],dY[2],pX, pY, pR);
	
	return TRUE;//KS_GageCircle(GAGE_MODE_LSC, dX, dY, 3, pX, pY, pR);
}
BOOL CCmdEditBar::GetCircle(const tgCmdLine *tgCmdLine, double *pX, double *pY, double *pR)
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
double CCmdEditBar::GetTrangleArea(double dx[3], double dy[3])
{
	double a = sqrt((dx[0]-dx[1])*(dx[0]-dx[1]) + (dy[0]-dy[1])*(dy[0]-dy[1]));
	double b = sqrt((dx[1]-dx[2])*(dx[1]-dx[2]) + (dy[1]-dy[2])*(dy[1]-dy[2]));
	double c = sqrt((dx[2]-dx[0])*(dx[2]-dx[0]) + (dy[2]-dy[0])*(dy[2]-dy[0]));
	double p = (a+b+c)/2;
	return sqrt(p*(p-a)*(p-b)*(p-c));
}
BOOL CCmdEditBar::IsPtInTrangle(double x, double y, double dx[3], double dy[3])
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
BOOL CCmdEditBar::GetCmdCircleInfo(tgCmdLine &tCircleCmd, tgCircle *pArc)
{
	pArc->x = tCircleCmd.pDblValue[0];
	pArc->y = tCircleCmd.pDblValue[1];
	pArc->r = tCircleCmd.pDblValue[4];

	pArc->dx = pArc->x + pArc->r*cos(0.0f);
	pArc->dy = pArc->y + pArc->r*sin(0.0f);
	return TRUE;
}
BOOL CCmdEditBar::GetCmdBrushInfo(tgCmdLine &tCmd, tgBrush *pArea)
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

BOOL CCmdEditBar::GetCmdArcInfo(tgCmdLine &tArcCmd, int iArcCmdIndex,tgArc *pArc)
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


BOOL CCmdEditBar::GetCmdArcInfo(tgCmdLine tPreDo,tgCmdLine tPassDo,tgCmdLine tNextDo, tgArc *pArc)
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
BOOL CCmdEditBar::TrackArcHandle(tgCmdLine &tPreDot, tgCmdLine &tPassDot, tgCmdLine &tNextDot, double dRound,double &Cx,double &Cy,BOOL &nDir)
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
BOOL CCmdEditBar::GetCmdArcInfoEx(tgCmdLine *tArcCmd0, tgCmdLine *tArcCmd1,tgCmdLine *tArcCmd2,tgArc *pArc)
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
BOOL CCmdEditBar::IsTrackPoint(tgCmdLine &tCmd, BOOL bOnlyLinePoint)
{
	CMD_TYPE t = tCmd.tType;
	if(bOnlyLinePoint)
	{
		if(t == CMD_LINE_START || t==CMD_LINE_PASSING || t==CMD_LINE_END
			|| t == CMD_LINE_ARC||t==CMD_ZMETER||t==CMD_REPEATX_ZH||t==CMD_REPEATY_ZH)
			return TRUE;
	}
	else
	{
		if(t == CMD_POINT 
			|| t == CMD_LINE_START || t==CMD_LINE_PASSING || t==CMD_LINE_END
            || t==CMD_POT||t==CMD_WCL||t==CMD_WCRECT
			|| t == CMD_LINE_ARC||t==CMD_CIRCLE||t==CMD_DUMMY||t==CMD_WAIT_POINT
			|| t==CMD_ZMETER||t==CMD_REPEATX_ZH||t==CMD_REPEATY_ZH
			|| t==CMD_SUPERSTART|| t==CMD_SUPERPOINT|| t==CMD_SUPEREND)
			return TRUE;
	}
	return FALSE;
}
int CCmdEditBar::GetPreTrackPoint(int iFrmIndex, BOOL bOnlyLinePoint)
{
	for(int i=iFrmIndex-1; i>-1; i--)
	{
		if(i<0)return FALSE;
		if(IsTrackPoint(m_rawList.GetAt(m_rawList.FindIndex(i)), bOnlyLinePoint))return i;
	}
	return -1;
}
int CCmdEditBar::GetNextTrackPoint(int iFrmIndex, BOOL bOnlyLinePoint)
{
	for(int i=iFrmIndex+1; i<m_rawList.GetCount(); i++)
	{
		if(IsTrackPoint(m_rawList.GetAt(m_rawList.FindIndex(i)), bOnlyLinePoint))return i;
	}
	return -1;
}
int CCmdEditBar::GetPreTrackPointEx(int iFrmIndex, BOOL bOnlyLinePoint)
{
	for(int i=iFrmIndex-1; i>-1; i--)
	{
		if(i<0)return FALSE;
		if(IsTrackPoint(g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(i)), bOnlyLinePoint))return i;
	}
	return -1;
}
int CCmdEditBar::GetNextTrackPointEx(int iFrmIndex, BOOL bOnlyLinePoint)
{
	for(int i=iFrmIndex+1; i<g_pFrm->m_Robot->m_cmdRunList.GetCount(); i++)
	{
		if(IsTrackPoint(g_pFrm->m_Robot->m_cmdRunList.GetAt(g_pFrm->m_Robot->m_cmdRunList.FindIndex(i)), bOnlyLinePoint))return i;
	}
	return -1;
}
void CCmdEditBar::SaveCmdLine(LPCTSTR strFile)
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
		if(tTempCmdPoint.tType == CMD_NOTE || tTempCmdPoint.tType == CMD_LABEL)
		{
			strTemp.Format("%%%d,", tTempCmdPoint.tType);
			strText = strTemp;

			strTemp.Format("%s", tTempCmdPoint.pszNote);
			strText += strTemp;
		}
		else if(tTempCmdPoint.tType == CMD_GOTO_ADDRESS)
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
			for(int j=0; j<tTempCmdPoint.iIntNum; j++)
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

		}else if((tTempCmdPoint.tType == CMD_REPEATX)||(tTempCmdPoint.tType == CMD_REPEATY))
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

void CCmdEditBar::ReadCmdLine(LPCTSTR strFile)
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
		if(tReadCmdPoint.tType == CMD_NOTE || tReadCmdPoint.tType == CMD_LABEL)
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
		if(tReadCmdPoint.tType == CMD_CALL_SUB)
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

		if (CMD_END_PROGRAM == tReadCmdPoint.tType)
		{
			break;
		}
	}
	
	sdFile.Close();
	//////////////////////////////////////////////////////////////////////////
	UpateCmdListBox(m_rawList.GetCount()-1);
	g_pFrm->m_Robot->m_iStepIndex = -1;//单步行号要复位
	g_pView->Invalidate();
}
void CCmdEditBar::FreeCmdList()
{//清除列表
	m_rawList.RemoveAll();
}
int CCmdEditBar::GetCmdCurSel()
{
	return m_wndList.GetNextItem(-1, LVNI_SELECTED);
}

BOOL CCmdEditBar::IsSelState(int iIndex)
{
	if(iIndex<0 || iIndex>m_wndList.GetItemCount()-1)return FALSE;
	return m_wndList.GetItemState(iIndex, LVNI_SELECTED) == LVNI_SELECTED;
}

BOOL CCmdEditBar::IsHaveRepeat(int iFrmIndex)
{
	for(int i=iFrmIndex+1; i<m_rawList.GetCount(); i++)
	{
		if(m_rawList.GetAt(m_rawList.FindIndex(i)).tType == CMD_REPEATX || m_rawList.GetAt(m_rawList.FindIndex(i)).tType == CMD_REPEATY)
			return TRUE;
	}
	return FALSE;
}
void CCmdEditBar::SetCmdCurSel(int nSelIndex)
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
	m_wndList.SetItemState(nSelIndex, LVIS_SELECTED, LVIS_SELECTED);  
	m_wndList.SetFocus();
}
void CCmdEditBar::DeleteCurSel()
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
	UpateCmdListBox(iSelIndex);
}

void CCmdEditBar::CutCurSel()
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
	UpateCmdListBox(iSelIndex);
}

void CCmdEditBar::PasteData()
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
	UpateCmdListBox(iSelIndex);
}

void CCmdEditBar::CopyCurSel()
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

void CCmdEditBar::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnMButtonDown(nFlags, point);
}
BOOL CCmdEditBar::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_MBUTTONDOWN)
	{
		if(pMsg->wParam == MK_MBUTTON)
		{
			OnMButtonDown(MK_MBUTTON, CPoint(GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)));
			return TRUE;
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}
// 保存当前位置到指令列表
void CCmdEditBar::LoadListArrayPos(tgCmdLine &tCmd, int iStartDblIndex)
{
	if(!g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		for(int i=0;i<3;i++)
	 		tCmd.pDblValue[iStartDblIndex+i] = 0;
	}
	else
	{
		//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
		g_pFrm->m_Precision.GetCurPosMM();
		for(int i=0;i<3;i++)// XYZU
			tCmd.pDblValue[iStartDblIndex+i] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[i];
	}
	g_pView->Invalidate();
}

// 指令试教
void CCmdEditBar::OnCmdType(UINT ID)
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
	{iSelIndex = 0;}
	

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
	case IDC_BUTTON_EDIT1:
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
	case IDC_BUTTON_EDIT2:
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
	case IDC_BUTTON_EDIT4:
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
		tTempCmdPoint.iDblNum = 3;
		tTempCmdPoint.iIntNum = 5;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=0;
		break;
	case ID_CMD_STEPY:         // Y 阵列
		tTempCmdPoint.tType = CMD_REPEATY;
		tTempCmdPoint.iDotNum = 0;
		tTempCmdPoint.iDblNum = 3;
		tTempCmdPoint.iIntNum = 5;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=0;
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
        tTempCmdPoint.iDotNum =1;
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
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 0;

		strcpy_s(tTempCmdPoint.pszNote, "标号0");
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
		tTempCmdPoint.pDblValue[2] = g_pFrm->m_pRobotParam->m_zLaserBaseVal - g_pFrm->m_pRobotParam->m_structClean.m_posClean.z;
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

		tTempCmdPoint.pDblValue[4] = 30.0; // 基准值
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

		tTempCmdPoint.pDblValue[0]=100;//mm/S
		tTempCmdPoint.pDblValue[1]=100;//mm/S
		tTempCmdPoint.pDblValue[2]=5;//mm/S
		tTempCmdPoint.pDblValue[3]=0.3;//mm/S
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

		bModify = FALSE;
		break;
	case ID_CMD_SUPERPOINT:      // 飞胶加工点
	case IDS_TOOLS_SUPERPOINT:   
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

	::WaitForSingleObject(g_runListEvent,INFINITE);

	if (bListOper)
	{
		if(m_bInsertCmd)
		{
			if(iSelIndex>=m_rawList.GetCount())
				iSelIndex=m_rawList.GetCount();

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
	UpateCmdListBox(iSelIndex);
	g_runListEvent.SetEvent();
}

void CCmdEditBar::SetZSame(int iFrmIndex)
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

BOOL CCmdEditBar::CalcBoradArcData(double &dx,double &dy,double dCurXdis,double dCurYdis,double dArc)
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
int CCmdEditBar::ModifiedCmd(int iCmdIndex)
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
	CDlgNote dlgLabel("标号");
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
		dlgLabel.m_pCmdLine = &pLine;
		iDlgResult = dlgLabel.DoModal();
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
	default:NULL;
	}

	return iDlgResult;
}

BOOL CCmdEditBar::GetEndPos(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py)
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

BOOL CCmdEditBar::GetEndPosEx(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py)
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

BOOL CCmdEditBar::GetEndActPos(tgCmdLine *pCmd, int iCmdIndex, double dLen, double *px, double *py,int nType)
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

BOOL CCmdEditBar::GetEndCirPos(tgCircle& tArc,double dfStrtAngle,double dLen, double *px, double *py,int nType)
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

void CCmdEditBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	 return ;
}

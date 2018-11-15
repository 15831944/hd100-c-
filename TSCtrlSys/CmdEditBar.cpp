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

tgCmdLine tTempCmdPoint;//��ʱ������
//////////////////////////////////////////////////////////////////////////
CString szCmdline[] = 
{
	"NONE",					//CMD_NONE=0,
	"MARK [ �� ]",			//Mark���� ,tgPos[2],int iMarkNum
	"�յ�",					// 
	"���� [ �� ]",			//CMD_POINT_DISPENSE,//����㽺,tgPos[1]
	"�߶���� [��----]",	//CMD_LINE_START,//�߶ε㽺���tgPos[1]
	"�߶��е� [--��--]",	//CMD_LINE_PASSING,// �߶ε㽺�м��tgPos[1]
	"Բ���е� [--��--]",	//CMD_LINE_ARC,//Բ���㽺 tgPos[1]
	"�߶��յ� [----��]",	//CMD_LINE_END,//�߶ε㽺������,tgPos[1]
	"Բ�ε㽺 [ �� ]",      //CMD_CIRCLE,//Բ�ε㽺 1:Բ��+ֱ��, tgPos[1], double diameter, 2:3�㻭Բ,tgPos[3]
	"�㽺��",				//CMD_DISPENSER,//�㽺����/�� BOOL On/Off
	"��ת",					//CMD_GOTO_ADDRESS,//��תָ�� int iIndex
	"X��������",            //CMD_REPEATX,//X�������� int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	"Y��������",            //CMD_REPEATY,//Y�������� int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	"Ϳ��",					//CMD_BRUSH_AREA,//Ϳ��,
	"�����Ӻ���",			//CMD_CALL_SUBROUTINE,//���ӳ���
	"���ó���",
	"����",					//CMD_CALL_PROGRAM,//���ó���
	"    ",					//CMD_NULL_LINE,//����
	//"�ȴ�����",			//CMD_SET_INPUT,//����IO״̬
	//"���",				//CMD_SET_OUTPUT,//����IO״̬
	"S����IO",
	"��ʱ",					//CMD_DELAY,//�ȴ�������������������ִ�м�¼�㡣
	"�ƶ�������λ",
	"ֹͣλ",				//CMD_STOP_POINT,//��ͷ���������ض�λ�ò���ֹͣ�ƶ���ֱ��ʹ���߰��»�̨����ϵ�START��ť���������ִ�м�¼��
	"����λ",               //CMD_HOME,//X, Y, Z�Ὣ�ع����ԭ��(�ȴ���)���������ִ�м�¼�㡣
	"ѭ��",					//CMD_LOOP,//ͨ����ѭ��ִ�е�ַ��Ϊ1�����̵�һ�������ɺ󣬿����ô˹������趨��������
	"�յ�",					//CMD_DUNNY,//�趨�㽺��ͷʵ�����ڵ�XYZ ����Ϊ�յ㣬����¼��Ŀǰ�ļ�¼��λ�á�ͨ��Ӧ���ڱܿ��������赲��
	"���",					//CMD_NOTE,
	"��ϴ",					//��ϴ��ͷ
	"������[��]",			//�����߶�",
	"���ش��[���]",       // ���ش��
	"���ػ���[��----]",		// ���ػ���
	"���� [��]",
	"ѡ��",
	"����������X",
	"����������Y",
	"�������",
	"����",
	"�������",				//CMD_END_PROGRAM

	"�߶ε㽺�ٶ�",			//PARAM_LINE_SPEED,//�߶ε㽺�ٶ�
	"�߶β���",				//PARAM_LINE_SETUP,//�߶ε㽺��������
	"�߶ν�����",			//PARAM_LINE_END,//�߶ε㽺��������
	"�����������",			//PARAM_POINT_SETUP,//����㽺��������
	"�����������",			//PARAM_POINT_END,//����㽺��������
	"Z�������߶�",			//PARAM_Z_CLEARANCE,//Z����߸߶����� 
	"Z�Ἣ��ֵ",			//PARAM_Z_LIMIT,//Z�Ἣ��ֵ���� 
	"XY�����ٶ�",			//PARAM_XY_VEL,//XY������ƶ��ٶ�
	"Z�����ٶ�",			//PARAM_Z_VEL,//Z������ƶ��ٶ�
	"Home��",				//PARAM_HOME_POSITION,//����RUNģʽʱ�㽺������ͷͣ��λ��
	"Z��߶�һ��",			//PARAM_SET_Z_SAME
	"Z��㽺�߶�",			// Z��㽺�߶�
	"�������",				// �������
	"���ؾ���[���]",		// ���ؾ���
	//
	"�����������",			// �ɽ����
	"���������",
	"�������������",
	"�ֶμӹ���",
    "Z��߶�����",    //
    "ƴ��ӹ���",    // 
	"����нǱ궨", 
	"����",
	"����������",
};
CString szCmdlineEN[] = 
{
	"NONE",					//CMD_NONE=0,
	"MARK [ �� ]",			//Mark���� ,tgPos[2],int iMarkNum
	"NonePoint",			// 
	"PointDisp [ �� ]",		//CMD_POINT_DISPENSE,//����㽺,tgPos[1]
	"LineStart [��----]",	//CMD_LINE_START,//�߶ε㽺���tgPos[1]
	"LinPassing [--��--]",	//CMD_LINE_PASSING,// �߶ε㽺�м��tgPos[1]
	"LineArc [--��--]",		//CMD_LINE_ARC,//Բ���㽺 tgPos[1]
	"LineEnd [----��]",		//CMD_LINE_END,//�߶ε㽺������,tgPos[1]
	"Circle [ �� ]",			//CMD_CIRCLE,//Բ�ε㽺 1:Բ��+ֱ��, tgPos[1], double diameter, 2:3�㻭Բ,tgPos[3]
	"Dispense",				//CMD_DISPENSER,//�㽺����/�� BOOL On/Off
	"Goto",					//CMD_GOTO_ADDRESS,//��תָ�� int iIndex
	"RepeatX",				//CMD_REPEATX,//X�������� int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	"RepentY",				//CMD_REPEATY,//Y�������� int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	"BrushArea",			//CMD_BRUSH_AREA,//Ϳ��,
	"CallSubFun.",			//CMD_CALL_SUBROUTINE,//���ӳ���
	"CallSubPro.",
	"LABEL",				//CMD_CALL_PROGRAM,//���ó���
	"    ",					//CMD_NULL_LINE,//����
	//"�ȴ�����",			//CMD_SET_INPUT,//����IO״̬
	//"���",				//CMD_SET_OUTPUT,//����IO״̬
	"SetIO",
	"Delay",				//CMD_DELAY,//�ȴ�������������������ִ�м�¼�㡣
	"GotoStandBy",
	"StopPoint",			//CMD_STOP_POINT,//��ͷ���������ض�λ�ò���ֹͣ�ƶ���ֱ��ʹ���߰��»�̨����ϵ�START��ť���������ִ�м�¼��
	"Home",					//CMD_HOME,//X, Y, Z�Ὣ�ع����ԭ��(�ȴ���)���������ִ�м�¼�㡣
	"Loop",					//CMD_LOOP,//ͨ����ѭ��ִ�е�ַ��Ϊ1�����̵�һ�������ɺ󣬿����ô˹������趨��������
	"DunnySpeed",			//CMD_DUNNY,//�趨�㽺��ͷʵ�����ڵ�XYZ ����Ϊ�յ㣬����¼��Ŀǰ�ļ�¼��λ�á�ͨ��Ӧ���ڱܿ��������赲��
	"Note",					//CMD_NOTE,
	"Wash",					//��ϴ��ͷ
	"Laser_Hight[��]",		//�����߶�",
	"POT[���]",			// ���ش��
	"WCLine[��----]",		// ���ػ���
	"Rect  [��]",
	"SelectJet",
	"Laser_RepeatX",
	"Laser_RepeatY",
	"SingleDispTest",
	"����",
	"EndProgram",			//CMD_END_PROGRAM

	"LineSpeed",			//PARAM_LINE_SPEED,//�߶ε㽺�ٶ�
	"LineSetup",			//PARAM_LINE_SETUP,//�߶ε㽺��������
	"LinEnd",				//PARAM_LINE_END,//�߶ε㽺��������
	"PointSetup",			//PARAM_POINT_SETUP,//����㽺��������
	"PointEnd",				//PARAM_POINT_END,//����㽺��������
	"Z_Clearance",			//PARAM_Z_CLEARANCE,//Z����߸߶����� 
	"Z_Limit",				//PARAM_Z_LIMIT,//Z�Ἣ��ֵ���� 
	"XY_V0",				//PARAM_XY_VEL,//XY������ƶ��ٶ�
	"Z_V0",					//PARAM_Z_VEL,//Z������ƶ��ٶ�
	"HomePos",				//PARAM_HOME_POSITION,//����RUNģʽʱ�㽺������ͷͣ��λ��
	"Z_Same",				//PARAM_SET_Z_SAME
	"Z_DispPos",			// Z��㽺�߶�
	"JetPara",				// �������
	"WCRect[���]",			// ���ؾ���
	"Reverse Board",		// ����ָ��
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
// CCmdEditBar   ��Ϣ�������
int CCmdEditBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy(0, 0, 0, 0);
	m_wndShotCutEdit.Create(IDD_DLG_SHOTCUT, this);

	m_wndList.Create(WS_VISIBLE|LVS_REPORT|LVS_SHOWSELALWAYS, CRect(0,0,10,10), this, IDS_COMMAND_LIST);
	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT/*|LVS_EX_CHECKBOXES*/);
	m_wndList.InsertColumn(0, "���", 0, 40);
	m_wndList.InsertColumn(1, "����", 0, 130);
	m_wndList.InsertColumn(2, "����", 0, 1500);

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
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
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
		AfxMessageBox("���¼����ԱȨ�ޣ�");
		return;
	}

	ModifiedCmd(GetCmdCurSel());   // �޸�ָ�� 
	UpateCmdListBox(GetCmdCurSel());
	*pResult=0;
}
void CCmdEditBar::OnRclickListCmd(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	UNREFERENCED_PARAMETER(pNMHDR);

	if(theApp.m_SysUser.m_CurUD.level<=GM_PERSONNEL)
	{
		AfxMessageBox("���¼����ԱȨ�ޣ�");
		return;
	}

	ShowTrackPopupMenu(1);
	*pResult=1;
}
void CCmdEditBar::ShowTrackPopupMenu(int MenuID)
{
	CMenu menu;
	if(g_pFrm->m_pSysParam->nLanguageType == 0)
	{// ����
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
	{// Ӣ��
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
	{// ����
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
// ��⵱ǰ�б����Ƿ���Markָ��
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
			 //��ʱ��
			 pArc->angle = dAngle[2] - dAngle[0];
		}
		else
		{
			//˳ʱ��
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
			//��ʱ��
			pArc->angle = dAngle[2] - dAngle[0];
		}
		else
		{
			//˳ʱ��
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
			//��ʱ��
			pArc->angle = dAngle[2] - dAngle[0];
		}
		else
		{
			//˳ʱ��
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
		AfxMessageBox("�����ļ�����!");
		return;
	}
	strTemp.Format("�����ļ�:%s", strFile);
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
		AfxMessageBox("���ļ�����.");
		return;
	}
	CString strText, strTemp;
	strTemp.Format("���ļ�:%s", strFile);
	g_pFrm->AddMsg(strTemp);

	int n=0, m=0, j=0, iDblN=0, iIntN=0;
	int k=0;
	tgCmdLine tReadCmdPoint;
	m_rawList.RemoveAll();

	while(sdFile.ReadString(strText) || strText.GetLength() != 0)
	{
//		if(strText.Mid(0,1) != "%")	break;
		if(strText.Mid(0,1) != "%")	continue;
		tReadCmdPoint.Clear();//��ʼ��	
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
	g_pFrm->m_Robot->m_iStepIndex = -1;//�����к�Ҫ��λ
	g_pView->Invalidate();
}
void CCmdEditBar::FreeCmdList()
{//����б�
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
// ���浱ǰλ�õ�ָ���б�
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

// ָ���Խ�
void CCmdEditBar::OnCmdType(UINT ID)
{
	// TODO: Add your command handler code here
//	g_pView->ImgStop();
	CString strTemp;
	strTemp.Format("ID %d", ID);

	int iSelIndex=-1;    // ��ǰָ��ID

     // ����ָ��
	if(m_bInsertCmd) 
	{
		iSelIndex = GetCmdCurSel();
		iSelIndex = (iSelIndex == -1) ?  m_rawList.GetCount() : iSelIndex;
	}
	else
		iSelIndex = m_wndList.GetItemCount();

	if(iSelIndex<0)
	{iSelIndex = 0;}
	

	BOOL bListOper = TRUE;     // ��Ҫ�����б���б������иĶ��ı�־λ
	BOOL bModify   = TRUE;

	tTempCmdPoint.Clear();     // ָ����ʱ�б�

	int k =0;
	//////////////////////////////////////////////////////////////////////////
	switch(ID)
	{
	case ID_CMD_REV_BOARD: // ����ָ��
		tTempCmdPoint.tType = CMD_ReverseBoard;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 0;
		break;

	case ID_CMD_MARK:      // Mark 
// 		if((g_pFrm->m_pSysParam->nFileSeparaEnable == 0)
// 			&&(g_pFrm->m_pSysParam->nFileScrapeEnable == 0))
// 		{  // ͨ��ģʽ�¼���Ƿ�ֻ��һ��Mark��
// 			if(CheckMark())
// 			{
// 				AfxMessageBox("Mark Command already exist in the list");
// 				return;
// 			}
// 		}

		tTempCmdPoint.tType =  CMD_MARK;   
		tTempCmdPoint.iDotNum =   2;      // ���������� 
		tTempCmdPoint.iDblNum = 8; 
		tTempCmdPoint.iIntNum = 5;

		for(k=0;k<tTempCmdPoint.iIntNum;k++)
			tTempCmdPoint.pIntValue[k] = 60;	// ���ƶ�

		for (k=0;k<tTempCmdPoint.iDblNum;k++)
			tTempCmdPoint.pDblValue[k] = 0;
        // Mark������
		if (CheckMark())
		{
			tTempCmdPoint.pIntValue[2] = 1;
		}
		else
		{
			tTempCmdPoint.pIntValue[2] = 0;
		}
		break;

	case IDS_TOOL_DOT:      // ������� 
	case IDC_BUTTON_EDIT0:
		tTempCmdPoint.tType = CMD_POINT;
		tTempCmdPoint.iDotNum=1;  // ��Ҫ��¼����������
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;
        
        // ��¼��ǰ����λ�õ��б�
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);  

		tTempCmdPoint.pIntValue[0]=0; //��ID
		tTempCmdPoint.pIntValue[1]=1; //����
		tTempCmdPoint.pIntValue[3]=1; //��λ
		bModify = FALSE;
		break;
	case IDS_TOOL_START:       // ֱ����� 
	case IDC_BUTTON_EDIT1:
		tTempCmdPoint.tType = CMD_LINE_START;
		tTempCmdPoint.iDotNum=1;  
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=1;
		tTempCmdPoint.pIntValue[3]=1;//��λ
		bModify = FALSE;
		break;
	
	case IDS_TOOL_PASS:     // �߶��е�
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
	case IDS_TOOL_ARC:     // Բ���е�
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
	case IDS_TOOL_END:         // �߶ν�����
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
	case IDS_TOOL_CIRCLE:     // Բ�ε㽺
	case IDC_BUTTON_EDIT5:
		tTempCmdPoint.tType = CMD_CIRCLE;
		tTempCmdPoint.iDotNum = 4;  // 4������
		tTempCmdPoint.iDblNum = 20;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=1;
		tTempCmdPoint.pIntValue[3]=1;//��λ
		break;
	case ID_CMD_DISPENSER:    // �㽺������ָ��
		tTempCmdPoint.tType = CMD_DISPENSER;
		tTempCmdPoint.iDotNum = 0;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 4;

		tTempCmdPoint.pIntValue[0] = 0;
		tTempCmdPoint.pIntValue[1] = 1;
		tTempCmdPoint.pIntValue[2] = 0;
		tTempCmdPoint.pIntValue[3] = 1;
		break;

	case ID_CMD_TOOL:        // ѡ��
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

	case ID_CMD_GOTOADDRESS:    // ��תָ��
		tTempCmdPoint.tType = CMD_GOTO_ADDRESS;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 4;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=0;
		strcpy_s(tTempCmdPoint.pszNote, "");
		break;
	case ID_CMD_BORATARC:         // ����нǲ���
		tTempCmdPoint.tType = CMD_REPEATARC;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 1;
		tTempCmdPoint.pDblValue[0]=g_pFrm->m_pSysParam->m_BoardArc;  // 
		tTempCmdPoint.pIntValue[0]=0;
		break;
	case ID_CMD_STEPX:         // X ����
		tTempCmdPoint.tType = CMD_REPEATX;
		tTempCmdPoint.iDblNum = 3;
		tTempCmdPoint.iIntNum = 5;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=0;
		break;
	case ID_CMD_STEPY:         // Y ����
		tTempCmdPoint.tType = CMD_REPEATY;
		tTempCmdPoint.iDotNum = 0;
		tTempCmdPoint.iDblNum = 3;
		tTempCmdPoint.iIntNum = 5;

		tTempCmdPoint.pIntValue[0]=0;
		tTempCmdPoint.pIntValue[1]=0;
		break;
	case ID_CMD_STEPX_ZH:     // ������X����
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
	case ID_CMD_STEPY_ZH:     // ������Y����
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
	case IDS_TOOLS_BDROP:  // ���ش��
    case IDC_BUTTON_EDIT7: 

		tTempCmdPoint.tType = CMD_POT;
		tTempCmdPoint.iDotNum=1;       // �������
		tTempCmdPoint.iDblNum = 5;   // double ���ݸ���
		tTempCmdPoint.iIntNum = 5;   // int ���ݸ���

		// ��ȡ��λ��
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0; // ��ID
		tTempCmdPoint.pIntValue[1]=1; // ����
		tTempCmdPoint.pIntValue[3]=1; // ��λ

        tTempCmdPoint.pIntValue[4]=1; // ������

		tTempCmdPoint.pDblValue[4]=10;// ����
		
		break;
	case IDS_TOOLS_BLINE:  // ���ػ���
    case IDC_BUTTON_EDIT6:

		tTempCmdPoint.tType = CMD_WCL;
		tTempCmdPoint.iDotNum=2;
		tTempCmdPoint.iDblNum = 10;  // 
		tTempCmdPoint.iIntNum = 5;
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pDblValue[8] = 10; //�߶�����
        tTempCmdPoint.pDblValue[9] = 20; //�߶��ٶ�
		tTempCmdPoint.pIntValue[0] = 0;  //����ģʽ
		tTempCmdPoint.pIntValue[1] = 20; //����ʱ��
		tTempCmdPoint.pIntValue[2] = 20; //�ط�ʱ��
		tTempCmdPoint.pIntValue[3] = 10; //������
		tTempCmdPoint.pIntValue[4] = 1;  //����

		break;

	case IDS_TOOL_COATDOT:   // ���ؾ���
    case IDC_BUTTON_EDIT8:  
		tTempCmdPoint.tType = CMD_WCRECT;
		tTempCmdPoint.iDotNum= 5;      // ���ݵ����
		tTempCmdPoint.iDblNum = 22;  // ���ݸ���
		tTempCmdPoint.iIntNum = 5;   // 
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pDblValue[20] = 10; //�߶�����  
		tTempCmdPoint.pDblValue[21] = 20; //�߶��ٶ�
		tTempCmdPoint.pIntValue[0] = 0;   //��ID
		tTempCmdPoint.pIntValue[1] = 1;   //����ʱ��
		tTempCmdPoint.pIntValue[2] = 20;  //�ط�ʱ��
		tTempCmdPoint.pIntValue[3] = 10;  //������
		tTempCmdPoint.pIntValue[4] = 1;   //����
		 break;
	case ID_CMD_BRUSHAREA:   // Ϳ��ָ��
		tTempCmdPoint.tType = CMD_BRUSH_AREA;
		tTempCmdPoint.iDotNum = 2;
		tTempCmdPoint.iDblNum = 12;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pDblValue[8]=5;//����Ϳ����ȳ�ʼֵ
		tTempCmdPoint.pDblValue[9]=5;//Բ��Ϳ����ȳ�ʼֵ

		tTempCmdPoint.pIntValue[0]=0;//Ĭ��NO.0
		tTempCmdPoint.pIntValue[1]=1;//Ĭ�Ͽ���
		tTempCmdPoint.pIntValue[3]=1;//��λ
		tTempCmdPoint.pIntValue[4]=0;//Ĭ��X����Ϳ��
		tTempCmdPoint.pIntValue[5]=0;//Ĭ�Ͼ���Ϳ��
		
		break;
	case ID_CMD_CALLSUBROUTINUE:     // �����ӳ���
		tTempCmdPoint.tType = CMD_CALL_SUB;
        tTempCmdPoint.iDotNum =1;
		tTempCmdPoint.iDblNum = 6;
		tTempCmdPoint.iIntNum = 2;

        LoadListArrayPos(tTempCmdPoint, 0);
		
		tTempCmdPoint.pIntValue[0]=1;//0��ַ 1���
		tTempCmdPoint.pIntValue[1]=0;//

        tTempCmdPoint.pDblValue[4]=0; // X����΢��
		tTempCmdPoint.pDblValue[5]=0; // y����΢��
		strcpy_s(tTempCmdPoint.pszNote, "");
		break;
	case ID_CMD_LABEL:         // ���ָ��
		tTempCmdPoint.tType = CMD_LABEL;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 0;

		strcpy_s(tTempCmdPoint.pszNote, "���0");
		break;
	case ID_CMD_NULL_LINE:     // ���� 
		tTempCmdPoint.tType = CMD_NULL_LINE;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 0;

		strcpy_s(tTempCmdPoint.pszNote, "");
		break;
	case ID_CMD_SETIO:        // IO����ָ��
		tTempCmdPoint.tType = CMD_SET_IO;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 3;

		tTempCmdPoint.pIntValue[0] = 0;		//0:input, 1:output
		tTempCmdPoint.pIntValue[1] = -1;	//�˿ں�
		tTempCmdPoint.pIntValue[2] = 0;		//���/���״̬
		break;
	case ID_CMD_WAITPOINT:      // ������ָ��
		tTempCmdPoint.tType = CMD_WAIT_POINT;
		tTempCmdPoint.iDotNum=1;  
		tTempCmdPoint.iDblNum = 5;
		tTempCmdPoint.iIntNum = 0;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pDblValue[4] = 0;//Ĭ��Ϊ 0s
		break;
	case ID_CMD_BATH:      //��ͷ��ϴָ�� 
		tTempCmdPoint.tType = CMD_BATH;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 5;
		tTempCmdPoint.iIntNum = 0;

		//for (k=0;k<tTempCmdPoint.iCNum;k++)
		//   LoadListArrayPos(tTempCmdPoint,k*4);
		tTempCmdPoint.pDblValue[0] = g_pFrm->m_pRobotParam->m_structClean.m_posClean.x+g_pFrm->m_pSysParam->tOffset[0].x;   
		tTempCmdPoint.pDblValue[1] = g_pFrm->m_pRobotParam->m_structClean.m_posClean.y+g_pFrm->m_pSysParam->tOffset[0].y;
		tTempCmdPoint.pDblValue[2] = g_pFrm->m_pRobotParam->m_zLaserBaseVal - g_pFrm->m_pRobotParam->m_structClean.m_posClean.z;
		tTempCmdPoint.pDblValue[4] = 0;//Ĭ��Ϊ 0s
		break;
	case ID_CMD_ZHEIGHT:        // ����߶ȼ��
	case IDS_TOOL_ZHEIGHT:
		tTempCmdPoint.tType = CMD_ZMETER;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 6;
		tTempCmdPoint.iIntNum = 0;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pDblValue[4] = 30.0; // ��׼ֵ
		tTempCmdPoint.pDblValue[5] = 0.1;  // ��ʱ 0s
		tTempCmdPoint.pDblValue[6] = 2.0;  // �㽺�߶�
		break;
	case ID_CMD_DELAY:      // ��ʱָ��
		tTempCmdPoint.tType = CMD_DELAY;
		tTempCmdPoint.iDblNum=1;
		tTempCmdPoint.iIntNum = 0;

		tTempCmdPoint.pDblValue[0]=0; //Ĭ��Ϊ 0s
		break;
	case ID_CMD_STOPPOINT:    // ��ָͣ��
		tTempCmdPoint.tType = CMD_PAUSE;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 0;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);
		break;
	case ID_CMD_HOME:       // X, Y, Z�Ὣ�ع����ԭ����������ִ�м�¼��
		tTempCmdPoint.tType = CMD_HOME;
		break;
	case ID_CMD_LOOP:       // ѭ��ָ��
		tTempCmdPoint.tType = CMD_LOOP;   
		tTempCmdPoint.iIntNum = 2;
		tTempCmdPoint.pIntValue[0] = 1;//Ĭ��ѭ������Ϊ1 
		tTempCmdPoint.pIntValue[1] = 0;//Ĭ���к�Ϊ0��ʼѭ��
		break;

	case ID_CMD_DUMMY:     // �յ�ָ��
		tTempCmdPoint.tType = CMD_DUMMY;
		tTempCmdPoint.iDotNum=1;
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);

		tTempCmdPoint.pIntValue[0]=0;
		bModify = FALSE;
		break;
	case ID_CMD_END_PROGRAM:    // �������ָ��
		tTempCmdPoint.tType = CMD_END_PROGRAM;
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 0;
		break;
	case ID_CMD_NOTE:          // ע����
		tTempCmdPoint.tType = CMD_NOTE;
		strcpy_s(tTempCmdPoint.pszNote, "����Ϊע����");
		break;
	case ID_PARAM_LINESPEED:   // �߶��ٶ�ָ��
		tTempCmdPoint.tType = PARAM_LINE_SPEED;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 0;

		tTempCmdPoint.pDblValue[0] = 20;//Ĭ�ϵ㽺�ٶ�ֵ20mm/s
		break;
	case ID_PARAM_LINESETUP:   // �߶β���
		tTempCmdPoint.tType = PARAM_LINE_SETUP;
		tTempCmdPoint.iDblNum = 6;
		tTempCmdPoint.iIntNum = 0;

		tTempCmdPoint.pDblValue[0]=0;
		tTempCmdPoint.pDblValue[1]=0;
		tTempCmdPoint.pDblValue[2]=5;//�������
		tTempCmdPoint.pDblValue[3]=0; //s
		tTempCmdPoint.pDblValue[4]=0.10;
		tTempCmdPoint.pDblValue[5]=0.004;
		break;
	case ID_PARAM_LINEEND:   // �߶ν�����������
		tTempCmdPoint.tType = PARAM_LINE_END;
		tTempCmdPoint.iDblNum = 3;
		tTempCmdPoint.iIntNum = 1;

		tTempCmdPoint.pDblValue[0]=5;//mm
		tTempCmdPoint.pDblValue[1]=5;//mm
		tTempCmdPoint.pDblValue[2]=20;//mm/s

		tTempCmdPoint.pIntValue[0]=0;
		break;
	case ID_PARAM_POINTSETUP:   // �������
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
	case ID_PARAM_ZLIMITHIGH:    // Z ��߶�����
		tTempCmdPoint.tType = PARAM_Z_WORKPOSSET;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 1;
		tTempCmdPoint.pDblValue[0]= 10; //mm
		tTempCmdPoint.pIntValue[0] = 1;
        	break;
	case ID_PARAM_JETSETUP:      // �������
		tTempCmdPoint.tType = PARAM_JET_SETUP;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 2;
		tTempCmdPoint.pDblValue[0]=10;
		tTempCmdPoint.pIntValue[0]=5;
		tTempCmdPoint.pIntValue[1]=5;
		break;
	case ID_PARAM_POINTEND:     // �����������
		tTempCmdPoint.tType = PARAM_POINT_END;
		tTempCmdPoint.iDblNum =3;
		tTempCmdPoint.iIntNum = 0;

		tTempCmdPoint.pDblValue[0]=100;//mm/s   �����ٶ�
		tTempCmdPoint.pDblValue[1]=20;//mm/s    �����ٶ�/4
		tTempCmdPoint.pDblValue[2]=0;//mm
		break;
	case ID_PARAM_Z_CLEARANCE:   // Z����߸߶����� 
		tTempCmdPoint.tType = PARAM_Z_CLEARANCE;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 1;
		tTempCmdPoint.pDblValue[0]= 10;//mm
		tTempCmdPoint.pIntValue[0] = 1;
		break;
	case ID_PARAM_Z_SET_HEIGHT:   // Z ��㽺�߶�
		tTempCmdPoint.tType = PARAM_Z_SET_HEIGHT;
		tTempCmdPoint.iDblNum = 1;
		tTempCmdPoint.iIntNum = 1;
		tTempCmdPoint.pDblValue[0] = 0;//mm
		tTempCmdPoint.pIntValue[0] = 1;
		break;
	case ID_PARAM_VELXY:          // XY ���ٶ�
		tTempCmdPoint.tType = PARAM_XY_VEL;
		tTempCmdPoint.iDblNum = 4;

		tTempCmdPoint.pDblValue[0]=100;//mm/S
		tTempCmdPoint.pDblValue[1]=100;//mm/S
		tTempCmdPoint.pDblValue[2]=5;//mm/S
		tTempCmdPoint.pDblValue[3]=0.3;//mm/S
		break;
	case ID_PARAM_VELZ:           // Z ���ٶ�
		tTempCmdPoint.tType = PARAM_Z_VEL;
		tTempCmdPoint.iDblNum = 4;

		tTempCmdPoint.pDblValue[0]=100;//mm/S
		tTempCmdPoint.pDblValue[1]=100;//mm/S
		tTempCmdPoint.pDblValue[2]=5;//mm/S
		tTempCmdPoint.pDblValue[3]=0.3;//mm/S
		break;

	case ID_CMD_SET_Z_SAME:       // ���������Ժ�������Z��߶���Ϊ��ͬ��ֵ
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
	case ID_CMD_REOPEN_FILE:  // ���Խ��ļ�ָ��
		bListOper = FALSE;
		bModify = FALSE;
		ReadCmdLine(g_pDoc->GetPathName());
		break;
	///////////////////////////////////////////////

	case ID_CMD_SUPERSTART:      //  �ɽ���ʼ��
	case IDS_TOOLS_SUPERSTART:
		tTempCmdPoint.tType = CMD_SUPERSTART;
		tTempCmdPoint.iDotNum=1;       // ��Ҫ��¼����������
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		// ��¼��ǰ����λ�õ��б�
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);  

		tTempCmdPoint.pIntValue[0]=0; //��ID
		tTempCmdPoint.pIntValue[1]=1; //����
		tTempCmdPoint.pIntValue[3]=1; //��λ

		bModify = FALSE;
		break;
	case ID_CMD_SUPERPOINT:      // �ɽ��ӹ���
	case IDS_TOOLS_SUPERPOINT:   
		tTempCmdPoint.tType = CMD_SUPERPOINT;
		tTempCmdPoint.iDotNum=1;       // ��Ҫ��¼����������
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 3;

		// ��¼��ǰ����λ�õ��б�
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);  

		tTempCmdPoint.pIntValue[0]=0; //��ID
		tTempCmdPoint.pIntValue[1]=1; //����
		tTempCmdPoint.pIntValue[3]=1; //��λ
   
		tTempCmdPoint.pDblValue[6]=0; // X΢������
        tTempCmdPoint.pDblValue[7]=0; // Y΢������
		bModify = FALSE;
		break;

	case ID_CMD_SUPEREND:       // �ɽ�������
	case IDS_TOOLS_SUPEREND:
		tTempCmdPoint.tType = CMD_SUPEREND;
		tTempCmdPoint.iDotNum=1;       // ��Ҫ��¼����������
		tTempCmdPoint.iDblNum = 8;
		tTempCmdPoint.iIntNum = 10;

		// ��¼��ǰ����λ�õ��б�
		for (k=0;k<tTempCmdPoint.iDotNum;k++)
			LoadListArrayPos(tTempCmdPoint,k*4);  

		tTempCmdPoint.pIntValue[0]=0; //��ID
		tTempCmdPoint.pIntValue[1]=1; //����
		tTempCmdPoint.pIntValue[3]=1; //��λ
		bModify = FALSE;
		break;

	case ID_CMD_FILESEPARAT:  // �ֶμӹ���

		tTempCmdPoint.tType = CMD_FILEDELIMITER;
		tTempCmdPoint.iDotNum=0;       
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 1;

		tTempCmdPoint.pIntValue[1] = 0;

		break;	
	case ID_CMD_FILESCRPEWORK:  // ƴ��ӹ���

		tTempCmdPoint.tType = CMD_FILESCRPEWORK;
		tTempCmdPoint.iDotNum=0;       
		tTempCmdPoint.iDblNum = 0;
		tTempCmdPoint.iIntNum = 1;

		tTempCmdPoint.pIntValue[1] = 0;

		break;
	default:
		NULL;
	}

	strTemp.Format("Add CMD��%s", szCmdline[tTempCmdPoint.tType]);
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
	if(bModify)//�д��ڵ����������
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
// ����н����ݼ���
// ����˵���� dx, dy �������������λ��
//       dCurXDis,dCurYDis �������м��
========================*/

BOOL CCmdEditBar::CalcBoradArcData(double &dx,double &dy,double dCurXdis,double dCurYdis,double dArc)
{
	// ����н�
	double darc = dArc;
	double dSinarc = sin(darc);
	double dCosarc = cos(darc);
	double dSinarcF = sin(-darc);
	double dCosarcF = cos(-darc);

	double tempXData =0;
	double tempYData =0;
    
	double tt1,tt2;
	///////////////////////////////////////// 
	// �Ƚ���ǰ�������ݴӲ���ģ������ϵ(��Ʒ����)ת������׼ģ������ϵ(��е����)
	//X = Cos(-@)*x - Sin(-@)*y;
	//Y = Sin(-@)*x + Cos(-@)*y;
	tempXData = dx *dCosarcF - dSinarcF * dy; 
	tempYData = dx *dSinarcF + dCosarcF * dy; 

	tt1 = tempXData + dCurXdis;
	tt2 = tempYData + dCurYdis;

    // �ٽ���ǰ�������ݴӱ�׼ģ������ϵ(��е����)ת��������ģ������ϵ(��Ʒ����)
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
// ָ���޸�
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
	CDlgNote dlgNote("ע��");
	CDlgNote dlgLabel("���");
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
	strTemp.Format("Modify CMD��%s", szCmdline[pLine.tType]);
	g_pFrm->AddMsg(strTemp);
////////////////////////////////////////////////////////// 
	// ����н�

	double dCurRX=0; // ���о����ڸ��������ʼλ�� X����  
	double dCurRY=0;

    int nRStartCMDID;      // ������ʼָ���
    int nRType   ;         // 0-s,1-N
    int nRXNum   ;         // ����X����
    int nRYNum   ;         // ����Y����
    double dRYDis,dRXDis;  // ���м��
    double dRArc;          // ���мн�
	switch(pLine.tType)
	{
	case CMD_MARK:
		//��ģ̬
		g_pFrm->ShowMarkDialog(&pLine, iCmdIndex);
		break;
	case CMD_DELAY://��ʱ
		dlgDelay.m_pCmdLine = &pLine;
		iDlgResult = dlgDelay.DoModal();
		break;
	case CMD_CIRCLE:  // Բ��ָ��
		//��ģ̬
		g_pFrm->ShowCircleDialog(&pLine,iCmdIndex);
         break;
	case CMD_ZMETER:  // ������
		g_pFrm->ShowZHEIGHTDialog(&pLine,iCmdIndex);	
		break;
	case CMD_WCL: // ���ػ���
		g_pFrm->ShowWCLDialog(&pLine,iCmdIndex);
		break;
	case CMD_WCRECT:
		g_pFrm->ShowWCRectDialog(&pLine,iCmdIndex);
		break;
	case CMD_POT: // ���ش��
		g_pFrm->ShowWCPDialog(&pLine,iCmdIndex);
		break;
	case CMD_REPEATX_ZH:  // ����������
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
		//��ģ̬
		g_pFrm->ShowAxisPosDialog(&pLine, iCmdIndex);
		break;
	case CMD_WAIT_POINT:  //������ 
		dlgStandby.m_pCmdLine = &pLine;
		iDlgResult = dlgStandby.DoModal();
		break;
	case CMD_DISPENSER:  // ������ָ��
		dlgDispenser.m_pCmdLine = &pLine;
		iDlgResult = dlgDispenser.DoModal();
		break;
	case CMD_TOOL:      // ѡ��ָ��
		dlgTool.m_pCmdLine = &pLine;
		iDlgResult = dlgTool.DoModal();
	case CMD_HOME:     // �Խ̻���
		break;
	case CMD_REPEATARC:   // �нǱ궨����
        g_pFrm->ShowBoardCheckDlgEx(&pLine,iCmdIndex);	
		break;
	case CMD_GOTO_ADDRESS: // ָ����ת
		dlgGoto.m_pCmdLine = &pLine;
		iDlgResult = dlgGoto.DoModal();
		break;
	case CMD_CALL_SUB:     // �����Ӻ���
		dlgGoSub.m_pCmdLine = &pLine;
		iDlgResult = dlgGoSub.DoModal();
		break;
	case CMD_NULL_LINE:    // ����
		break;
	case CMD_LABEL:        // ���
		dlgLabel.m_pCmdLine = &pLine;
		iDlgResult = dlgLabel.DoModal();
		break;
	case CMD_REPEATX:      // X����
		dlgRepeat.m_pCmdLine = &pLine;
		dlgRepeat.m_iSelIndex = iCmdIndex;
		iDlgResult = IDCANCEL;

		if(dlgRepeat.DoModal() == IDOK)
		{  
			nRStartCMDID = pLine.pIntValue[0];   // ������ʼָ���
			nRType       = pLine.pIntValue[1];   // 0-s,1-N
			nRXNum       = pLine.pIntValue[2];   // ����X����
			nRYNum       = pLine.pIntValue[3];   // ����Y����
            dRXDis       = pLine.pDblValue[0];   // ����X���
			dRYDis       = pLine.pDblValue[1];   // ����X���
			dRArc        = pLine.pDblValue[2];   // ���мн�
			iDlgResult = IDOK;
			if(dlgRepeat.m_bExpand)
			{	// ����չ��
				if(IsHaveRepeat(iCmdIndex))
				{
					AfxMessageBox("��ָ����������ָ���ִ��չ��,\n����ֻ��չ�����һ������ָ��.");
					break;
				}
				m=1;
				ptime=0;   // ���д���
				stepcount = iCmdIndex - nRStartCMDID;   // ���а���ָ�����   
				//
				for(i=0; i<nRYNum;i++)                  //  ���� Y ����
				{
					for(j=0; j< nRXNum; j++)            //  ���� x ����
					{
						if(i==0 && j==0)
						{
							ptime++;   // ��һ�����н��
						    continue;
						}
						for(k= nRStartCMDID; k<iCmdIndex; k++)
						{  // ����ʼ������ �� ��ǰ��ָ�� 
                            
							tTempCmdPoint=m_rawList.GetAt(m_rawList.FindIndex(k));

							if (tTempCmdPoint.tType==CMD_MARK)
								continue;
                              // ������������
							if(tTempCmdPoint.tType == CMD_REPEATX || tTempCmdPoint.tType == CMD_REPEATY)
								tTempCmdPoint.pIntValue[0] += ptime * stepcount; 

							for(q=0; q<tTempCmdPoint.iDotNum; q++)
							{   // ��ǰ����ָ�����ݵĸ���

								if(nRType ==0) //S
								{
									if((i%2) == 0)
										dCurRX =  (dRXDis*j);     // x ���
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
			nRStartCMDID = pLine.pIntValue[0];   // ������ʼָ���
			nRType       = pLine.pIntValue[1];   // 0-s,1-N
			nRXNum       = pLine.pIntValue[2];   // ����X����
			nRYNum       = pLine.pIntValue[3];   // ����Y����
			dRXDis       = pLine.pDblValue[0];   // ����X���
			dRYDis       = pLine.pDblValue[1];   // ����X���
			dRArc        = pLine.pDblValue[2];   // ���мн�
			iDlgResult = IDOK;
			if(dlgRepeat.m_bExpand)
			{
				if(IsHaveRepeat(iCmdIndex))
				{
					AfxMessageBox("��ָ����������ָ���ִ��չ��,\n����ֻ��չ�����һ������ָ��.");
					break;
				}
				m=1;
				ptime=0;
				stepcount = iCmdIndex - nRStartCMDID;  // ָ�����
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
	case CMD_SET_IO: // Io ָ��
		dlgIOSet.m_pCmdLine = &pLine;
		iDlgResult =dlgIOSet.DoModal();
		break;
	case CMD_LOOP: // ѭ��
		dlgLoop.m_pCmdLine = &pLine;
		iDlgResult =dlgLoop.DoModal();
		break;
	case CMD_NOTE:  // ע��
		dlgNote.m_pCmdLine = &pLine;
		iDlgResult =dlgNote.DoModal();
		break;
	case PARAM_LINE_SPEED:   // �ٶ�ָ��
	case PARAM_XY_VEL:
	case PARAM_Z_VEL:
		dlgSpeed.m_pCmdLine = &pLine;
		iDlgResult =dlgSpeed.DoModal();
		break;
	case PARAM_POINT_END:    // ������������ٶ�
		dlgPointEnd.m_pCmdLine = &pLine;
		iDlgResult =dlgPointEnd.DoModal();
		break;
	case PARAM_LINE_SETUP:   // �߶β���
		dlgLineParam.m_pCmdLine = &pLine;
		iDlgResult =dlgLineParam.DoModal();
		break;
	case PARAM_LINE_END:     // �߶ν�����ʽ
		dlgLineEnd.m_pCmdLine = &pLine;
		iDlgResult =dlgLineEnd.DoModal();
		break;
	case PARAM_POINT_SETUP:  // �������
		dlgPointParam.m_pCmdLine = &pLine;
		iDlgResult =dlgPointParam.DoModal();
		break;
	case PARAM_JET_SETUP:     
		dlgJetParam.m_pCmdLine = &pLine;
		iDlgResult =dlgJetParam.DoModal();
		break;
	case PARAM_Z_CLEARANCE://����㽺�������߶ε㽺����ʱZ����߸߶�
		dlgZClearance.m_pCmdLine = &pLine;
		iDlgResult =dlgZClearance.DoModal();
		break;
    case PARAM_Z_WORKPOSSET://
		//dlgZClearance.m_pCmdLine = &pLine;
		//iDlgResult =dlgZClearance.DoModal();
		break;
	case PARAM_Z_SET_HEIGHT: // Z �߶���������
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
{//��ȡ��������ǰ�Ͻ��������----չ��ǰ��ʱ�б�
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
{//��ȡ��������ǰ�Ͻ��������----չ���������б�
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
{//����������ʱ���ȡ
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
{//Բ������������ʱ���ȡ
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

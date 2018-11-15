// DlgMark.cpp : implementation file
//
#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgMark.h"
#include "MainFrm.h"
#include "DlgPathEdit.h"
//#include "CmdEditBar.h"
// CDlgMark dialog

IMPLEMENT_DYNAMIC(CDlgMark, CDialog)

CDlgMark::CDlgMark(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMark::IDD, pParent)
{
	m_pCmdLine	= NULL;
	m_bIsEditMod = false;
	m_bLBtnDown = false;
}

CDlgMark::~CDlgMark()
{

}

void CDlgMark::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgMark, CDialog)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgMark::OnBnClickedButtonApply)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_LEARN, &CDlgMark::OnBnClickedButtonLearn)
	ON_BN_CLICKED(IDC_BUTTON_PAT_TEST, &CDlgMark::OnBnClickedButtonPatTest)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgMark::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDCANCEL, &CDlgMark::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_MARK, &CDlgMark::OnCbnSelchangeComboMark)
	ON_BN_CLICKED(IDC_BUTTON_PAT_SAVE, &CDlgMark::OnBnClickedButtonPatSave)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_MARKMODEID, &CDlgMark::OnCbnSelchangeComboMarkmodeid)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_EDIT, &CDlgMark::OnBnClickedBtnEdit)
END_MESSAGE_MAP()

BOOL CDlgMark::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Tag_1"));
	GetDlgItem(IDC_BUTTON_LEARN)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Btn_1"));
	GetDlgItem(IDC_BUTTON_PAT_TEST)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Btn_2"));
	GetDlgItem(IDC_BTN_MOVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Btn_3"));
	GetDlgItem(IDC_BUTTON_PAT_SAVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Btn_4"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Btn_5"));
   
	GetDlgItem(IDC_STATIC_MARK_PAT)->ModifyStyle(0, SS_OWNERDRAW);
// 	// 模板组编号
//     CComboBox *pGID = (CComboBox*)GetDlgItem(IDC_COMBO_MARKMODEID);
// 
//     int nMarkGropNum =1;
// 
// 	// 文件分段使能时有2组Mark点
// 	if(g_pFrm->m_pSysParam->nFileSeparaEnable == 1)
// 	{
// 		nMarkGropNum = 2;  
// 	}
// 	// 文件拼接使能时有nFileScrapeNum 组Mark点
// 	if(g_pFrm->m_pSysParam->nFileScrapeEnable == 1)
// 	{
// 		nMarkGropNum = g_pFrm->m_pSysParam->nFileScrapeNum;  
// 	}
//     pGID->ResetContent();
// 	switch(nMarkGropNum)
// 	{
// 	case 1:
// 		pGID->AddString("Group 1");
// 		break;
// 	case 2:
// 		pGID->AddString("Group 1");
// 		pGID->AddString("Group 2");
// 		break;
// 	case 3:
// 		pGID->AddString("Group 1");
// 		pGID->AddString("Group 2");
// 		pGID->AddString("Group 3");
// 		break;
// 	case 4:
// 		pGID->AddString("Group 1");
// 		pGID->AddString("Group 2");
// 		pGID->AddString("Group 3");
// 		pGID->AddString("Group 4");
// 		break;
// 	}

	// 模板组内模板个数
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_MARK);
  
	int nNum = g_pFrm->m_pSysParam->iMarkNum;
	switch(nNum)
	{
	case 1:
		pBox->AddString("MARK 1");
		break;
	case 2:
		pBox->AddString("MARK 1");
		pBox->AddString("MARK 2");
		break;
	case 3:
		pBox->AddString("MARK 1");
		pBox->AddString("MARK 2");
		pBox->AddString("MARK 3");
		break;
	}

//    pGID->SetCurSel(0);
	pBox->SetCurSel(0);
    
	//
	if(m_pCmdLine != NULL)
	{
		int nGrdID= m_pCmdLine->pIntValue[2];// 组ID
//		pGID->SetCurSel(nGrdID);

		ShowMarkPat(nGrdID,0);
	}

	// 分段或拼版模式下打开组ID
// 	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
// 		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
// 	{
// 		pGID->EnableWindow(TRUE);
// 	}else
// 	{
// 		pGID->EnableWindow(FALSE);
// 	}
	return TRUE;  
}

BOOL CDlgMark::ShowMarkPat(int nGid,int nMarkID)
{
	UNREFERENCED_PARAMETER(nGid);
	CString strTemp;
	if(m_pCmdLine != NULL)
	{   
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[nMarkID*4+0]);
		SetDlgItemText(IDC_MARK_EDIT_X, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[nMarkID*4+1]);
		SetDlgItemText(IDC_MARK_EDIT_Y, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[nMarkID*4+2]);
		SetDlgItemText(IDC_MARK_EDIT_Z, strTemp);

		strTemp.Format("%d", m_pCmdLine->pIntValue[nMarkID]);
		SetDlgItemText(IDC_MARK_EDIT_ACCEPTANCE, strTemp);
		if(!g_pView->PatLoad(nMarkID+1+nGid*2))
		{
			SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Msg_2"));
			SetDlgItemText(IDC_MARK_EDIT_ACCEPTANCE, _T("80"));
		}
		else
		{
			SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Msg_3"));
			g_pView->PatShow(GetDlgItem(IDC_STATIC_MARK_PAT)->m_hWnd, nMarkID+1);
			Invalidate(TRUE);
		}
	}
	return TRUE;
}
void CDlgMark::OnBnClickedButtonApply()
{
	if(m_pCmdLine != NULL)
	{
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_MARK);
		CString strTemp;
		int n = pBox->GetCurSel();
		g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, n*4);
        // 相似度
		GetDlgItemText(IDC_MARK_EDIT_ACCEPTANCE ,strTemp);
		m_pCmdLine->pIntValue[n] = atoi(strTemp);
      
		g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);
        
		OnCbnSelchangeComboMark();
	}
}
void CDlgMark::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(bShow && m_pCmdLine != NULL)
	{
		
		CComboBox *pGid = (CComboBox*)GetDlgItem(IDC_COMBO_MARKMODEID); 

		int nGrdID= m_pCmdLine->pIntValue[2];// 组ID
		int nMarkGropNum =1;

		// 文件分段使能时有2组Mark点
		if(g_pFrm->m_pSysParam->nFileSeparaEnable == 1)
		{
			nMarkGropNum = 2;  
		}
		// 文件拼接使能时有nFileScrapeNum 组Mark点
		if(g_pFrm->m_pSysParam->nFileScrapeEnable == 1)
		{
			nMarkGropNum = g_pFrm->m_pSysParam->nFileScrapeNum;  
		}
        pGid->ResetContent();
		switch(nMarkGropNum)
		{
		case 1:
			pGid->AddString("Group 1");
			break;
		case 2:
			pGid->AddString("Group 1");
			pGid->AddString("Group 2");
			break;
		case 3:
			pGid->AddString("Group 1");
			pGid->AddString("Group 2");
			pGid->AddString("Group 3");
			break;
		case 4:
			pGid->AddString("Group 1");
			pGid->AddString("Group 2");
			pGid->AddString("Group 3");
			pGid->AddString("Group 4");
			break;
		}


		pGid->SetCurSel(nGrdID);

		if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
			(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
		{
			pGid->EnableWindow(TRUE);
		}else
		{
			pGid->EnableWindow(FALSE);
		}

		OnCbnSelchangeComboMark();
	}
}

void CDlgMark::OnBnClickedButtonLearn()
{
	// TODO: Add your control notification handler code here
	int nModeGroupIndex = m_pCmdLine->pIntValue[2];
	CString strFile, strTemp;
	CComboBox *pGid = (CComboBox*)GetDlgItem(IDC_COMBO_MARK); 
//	CComboBox *pGid = (CComboBox*)GetDlgItem(IDC_COMBO_MARKMODEID); 
	g_pView->PatAlloc(pGid->GetCurSel()+1+nModeGroupIndex*2);
	g_pView->PatShow(GetDlgItem(IDC_STATIC_MARK_PAT)->m_hWnd, pGid->GetCurSel()+1+nModeGroupIndex*2);

	OnBnClickedButtonPatSave(); // 保存模板
 	OnBnClickedButtonApply();   // 更新位置
// 
// 	CRect rect;
// 	GetWindowRect(&rect);
// 	rect.left = rect.left+1;
// 	MoveWindow(rect,TRUE);
// 
// 	rect.left = rect.left-1;
// 	MoveWindow(rect,TRUE);
}

void CDlgMark::OnBnClickedButtonPatTest()
{
	g_pView->PaintShape(CRect(0, 0, IMG_WIDTH, IMG_HEIGHT));
	// TODO: Add your control notification handler code here
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_MARK);
	int index = pBox->GetCurSel();
	int nGroupIndex = m_pCmdLine->pIntValue[2];

	double dx, dy;

	CString strTemp;
	GetDlgItemText(IDC_MARK_EDIT_ACCEPTANCE ,strTemp);
	m_pCmdLine->pIntValue[index] = atoi(strTemp);
	if(m_pCmdLine->pIntValue[index] == 0)
	{
		AfxMessageBox(_T("相似度不能为0"));
		return ;
	}

	if(g_pView->PatFind(index+1+nGroupIndex*2, (double)(m_pCmdLine->pIntValue[index]),dx, dy)<=0)
	{  
		strTemp.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLOOP,"Msg_1"));
		AfxMessageBox(strTemp);
	}
}

void CDlgMark::OnBnClickedButtonMove()
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_MARK);
	CString strTemp; 
	int n = pBox->GetCurSel()*4;

	double x = m_pCmdLine->pDblValue[n+0];
	double y = m_pCmdLine->pDblValue[n+1];
//	double z = g_pFrm->m_pSysParam->tPixelAdjustPos.z;//高度同标定模板高度

	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
//	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
}

void CDlgMark::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	g_pView->ImgLive();
	OnCancel();
}

void CDlgMark::OnCbnSelchangeComboMark()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CComboBox *p = (CComboBox*)GetDlgItem(IDC_COMBO_MARK);

		int nModeGroupIndex = m_pCmdLine->pIntValue[2];
		CString strTemp;
		int n = p->GetCurSel()*4;
        
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n+0]);
		SetDlgItemText(IDC_MARK_EDIT_X, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n+1]);
		SetDlgItemText(IDC_MARK_EDIT_Y, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n+2]);
		SetDlgItemText(IDC_MARK_EDIT_Z, strTemp);

		strTemp.Format("%d", m_pCmdLine->pIntValue[p->GetCurSel()]);
		SetDlgItemText(IDC_MARK_EDIT_ACCEPTANCE, strTemp);
		if(!g_pView->PatLoad(p->GetCurSel()+1+nModeGroupIndex*2))
		{
			SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Msg_2"));
			SetDlgItemText(IDC_MARK_EDIT_ACCEPTANCE, _T("80"));
		}
		else
		{
			SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Msg_3"));
			g_pView->PatShow(GetDlgItem(IDC_STATIC_MARK_PAT)->m_hWnd, p->GetCurSel()+1+nModeGroupIndex*2);
			Invalidate(TRUE);
		}
	}
}

void CDlgMark::OnBnClickedButtonPatSave()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		int nModeGroupIndex = m_pCmdLine->pIntValue[2];
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_MARK);
		int n = pBox->GetCurSel();

		CString strTemp;
		GetDlgItemText(IDC_MARK_EDIT_ACCEPTANCE ,strTemp);
		m_pCmdLine->pIntValue[n] = atoi(strTemp);
		if(m_pCmdLine->pIntValue[n] == 0)
		{
			AfxMessageBox(_T("相似度不能为0"));
			return ;
		}
		g_pView->PatSetParam(n+1+nModeGroupIndex*2, atof(strTemp));
		g_pView->PatSave(n+1+nModeGroupIndex*2);
		g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);

		AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDMark,"Msg_4"));
		g_pView->ImgLive();
	}
}

void CDlgMark::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	return;

	CDialog::OnOK();
}

void CDlgMark::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	g_pFrm->SysParam(FALSE);

	CDialog::OnClose();
}

// 模板组编号
void CDlgMark::OnCbnSelchangeComboMarkmodeid()
{
	
}


void CDlgMark::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bLBtnDown = true;
	CDialog::OnLButtonDown(nFlags, point);
}


void CDlgMark::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bLBtnDown = false;
	CDialog::OnLButtonUp(nFlags, point);
}


void CDlgMark::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bIsEditMod && m_bLBtnDown)
	{
		int nId = m_pCmdLine->pIntValue[2];
		CComboBox *pGid = (CComboBox*)GetDlgItem(IDC_COMBO_MARK); 
		CRect rt;
		GetDlgItem(IDC_STATIC_MARK_PAT)->GetWindowRect(rt);
		ScreenToClient(rt);

		if (PtInRect(rt, point))
		{
			point.x -= rt.left;
			point.y -= rt.top;
			g_ProgData.m_mod[pGid->GetCurSel()+1+nId*2].EditMod(point);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CDlgMark::OnBnClickedBtnEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	GetDlgItem(IDC_BTN_EDIT)->GetWindowText(strTemp);
	if ("编辑模板" == strTemp)
	{
		m_bIsEditMod = true;
		GetDlgItem(IDC_BTN_EDIT)->SetWindowText("编辑中");
	}
	else
	{
		m_bIsEditMod = false;
		GetDlgItem(IDC_BTN_EDIT)->SetWindowText("编辑模板");
	}
}

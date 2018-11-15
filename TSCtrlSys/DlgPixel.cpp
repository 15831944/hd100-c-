// DlgPixel.cpp : implementation file
//
#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgPixel.h"
#include "MainFrm.h"
#include "DlgPathEdit.h"
#include "math.h"
// CDlgPixel dialog

IMPLEMENT_DYNAMIC(CDlgPixel, CDialog)

CDlgPixel::CDlgPixel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPixel::IDD, pParent)
{
	m_bIsEditMod = false;
	m_bLBtnDown = false;
}

CDlgPixel::~CDlgPixel()
{

}

void CDlgPixel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPixel, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDlgPixel::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DLG_PIXEL_BTN_LEARN, &CDlgPixel::OnBnClickedDlgPixelBtnLearn)
	ON_BN_CLICKED(IDC_DLG_PIXEL_BTN_MOVE, &CDlgPixel::OnBnClickedDlgPixelBtnMove)
	ON_BN_CLICKED(IDC_DLG_PIXEL_BTN_PAT_TEST, &CDlgPixel::OnBnClickedDlgPixelBtnPatTest)
	ON_BN_CLICKED(IDC_DLG_PIXEL_BTN_PAT_SAVE, &CDlgPixel::OnBnClickedDlgPixelBtnPatSave)
	ON_BN_CLICKED(IDC_DLG_PIXEL_BTN_CALI, &CDlgPixel::OnBnClickedDlgPixelBtnCali)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_DLG_PIXEL_BTN_PAT_EDIT, &CDlgPixel::OnBnClickedDlgPixelBtnPatEdit)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CDlgPixel message handlers

BOOL CDlgPixel::OnInitDialog()
{
	CDialog::OnInitDialog();
 
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Tag_3"));

	GetDlgItem(IDC_DLG_PIXEL_BTN_LEARN)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Btn_1"));
	GetDlgItem(IDC_DLG_PIXEL_BTN_PAT_TEST)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Btn_2"));
	GetDlgItem(IDC_DLG_PIXEL_BTN_PAT_SAVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Btn_3"));
	GetDlgItem(IDC_DLG_PIXEL_BTN_MOVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Btn_4"));
	GetDlgItem(IDC_DLG_PIXEL_BTN_CALI)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Btn_5"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Btn_6"));

	CString strTemp;
     
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.x);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.y);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.z);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_Z, strTemp);

	strTemp.Format("%.3f", g_pFrm->m_pSysParam->m_patCaliAccepatance);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_ACCEPTANCE, strTemp);

	strTemp.Format("%.6f", g_pFrm->m_pSysParam->dPixelScale);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_RATE, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->dPixelAngle);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_ANG, strTemp);

	GetDlgItem(IDC_STATIC_PIXEL_PAT)->ModifyStyle(0, SS_OWNERDRAW);

	//标定模板为0号
	if(!g_pView->PatLoad(/*(LPSTR)(LPCSTR)g_pDoc->GetModeFileEx(0)*/))
	{
		SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Msg_2"));
	}
	else
	{
		SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Msg_3"));
		g_pView->PatShow(GetDlgItem(IDC_STATIC_PIXEL_PAT)->m_hWnd);
		Invalidate(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPixel::OnBnClickedDlgPixelBtnLearn()
{
	// TODO: Add your control notification handler code here
	CString strTemp;

	g_pView->PatAlloc(/*(LPSTR)(LPCSTR)g_pDoc->GetModeFileEx(0)*/);
	g_pView->PatShow(GetDlgItem(IDC_STATIC_PIXEL_PAT)->m_hWnd);

	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	g_pFrm->m_pSysParam->tPixelAdjustPos.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tPixelAdjustPos.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	g_pFrm->m_pSysParam->tPixelAdjustPos.z = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.x);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.y);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.z);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_Z, strTemp);

	CRect rect;
	GetWindowRect(&rect);
    rect.left = rect.left+1;
	MoveWindow(rect,TRUE);

	rect.left = rect.left-1;
	MoveWindow(rect,TRUE);
}

void CDlgPixel::OnBnClickedDlgPixelBtnMove()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	GetDlgItemText(IDC_DLG_PIXEL_EDIT_X, strTemp);
	g_pFrm->m_pSysParam->tPixelAdjustPos.x=atof(strTemp);

	GetDlgItemText(IDC_DLG_PIXEL_EDIT_Y, strTemp);
	g_pFrm->m_pSysParam->tPixelAdjustPos.y=atof(strTemp);

	GetDlgItemText(IDC_DLG_PIXEL_EDIT_Z, strTemp);
	g_pFrm->m_pSysParam->tPixelAdjustPos.z=atof(strTemp);

	double x = g_pFrm->m_pSysParam->tPixelAdjustPos.x;
	double y = g_pFrm->m_pSysParam->tPixelAdjustPos.y;
	double z = g_pFrm->m_pSysParam->tPixelAdjustPos.z;

	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
}

void CDlgPixel::OnBnClickedDlgPixelBtnPatTest()
{
	// TODO: Add your control notification handler code here
	double dx, dy;

	CString strTemp;
	GetDlgItemText(IDC_DLG_PIXEL_EDIT_ACCEPTANCE ,strTemp);
	g_pFrm->m_pSysParam->m_patCaliAccepatance = atof(strTemp);

	if(g_pView->PatFind(Mod_Calib, g_pFrm->m_pSysParam->m_patCaliAccepatance,dx, dy)<=0)
	{
		AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Msg_1"));
	}
}

void CDlgPixel::OnBnClickedDlgPixelBtnPatSave()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	GetDlgItemText(IDC_DLG_PIXEL_EDIT_ACCEPTANCE ,strTemp);
	g_pFrm->m_pSysParam->m_patCaliAccepatance = atof(strTemp);
	g_pView->PatSetParam(Mod_Calib, atof(strTemp));
	g_pView->PatSave(/*(LPSTR)(LPCSTR)g_pDoc->GetModeFileEx(0)*/);

	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	g_pFrm->m_pSysParam->tPixelAdjustPos.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tPixelAdjustPos.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	g_pFrm->m_pSysParam->tPixelAdjustPos.z = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.x);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.y);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.z);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_Z, strTemp);
	AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Msg_4"));
	g_pView->ImgLive();
}
void CDlgPixel::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(bShow)
	{ 
		CString strTemp;
	     strTemp = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Tag_0");
		SetWindowText(strTemp);

		
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.x);
		SetDlgItemText(IDC_DLG_PIXEL_EDIT_X, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.y);
		SetDlgItemText(IDC_DLG_PIXEL_EDIT_Y, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.z);
		SetDlgItemText(IDC_DLG_PIXEL_EDIT_Z, strTemp);
	}
}

void CDlgPixel::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	g_pView->ImgLive();
	OnCancel();
}

void CDlgPixel::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	g_pFrm->SysParam(FALSE);
	CDialog::OnClose();
}

void CDlgPixel::OnBnClickedDlgPixelBtnCali()
{
	// TODO: Add your control notification handler code here
	double ddx[2], ddy[2];
	double ddpx[2], ddpy[2];
	double dx, dy;

	CString strTemp;
	GetDlgItemText(IDC_DLG_PIXEL_EDIT_ACCEPTANCE ,strTemp);
	g_pFrm->m_pSysParam->m_patCaliAccepatance = atof(strTemp);

	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	dx = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	dy = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,dx+1, dy+1,1,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	CFunction::DelaySec(0.5);

	if(g_pView->PatFind(Mod_Calib, g_pFrm->m_pSysParam->m_patCaliAccepatance, ddpx[0], ddpy[0]) < 1)
	{
		AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Msg_5"));
	
		return;
	}

	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	ddx[0] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	ddy[0] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,dx-1, dy-1,1,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	CFunction::DelaySec(0.5);

	if(g_pView->PatFind(Mod_Calib, g_pFrm->m_pSysParam->m_patCaliAccepatance,ddpx[1], ddpy[1]) < 1)
	{
		AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Msg_5"));
		return;
	}

	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	ddx[1] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	ddy[1] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];

	double x_fScale = fabs((ddx[0] - ddx[1])/(ddpx[0] - ddpx[1]));//计算X方向的 pixel->mm
	double y_fScale = fabs((ddy[0] - ddy[1])/(ddpy[0] - ddpy[1]));//计算Y方向的 pixel->mm
	if( fabs(x_fScale)<0 &&  fabs(y_fScale)<0 )
	{
		AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Msg_6"));
	
		return;
	}

	double dPixelLen = sqrt((ddpx[0] - ddpx[1])*(ddpx[0] - ddpx[1]) + (ddpy[0] - ddpy[1])*(ddpy[0] - ddpy[1]));
	double dAxisLen = sqrt((ddx[0] - ddx[1])*(ddx[0] - ddx[1]) + (ddy[0] - ddy[1])*(ddy[0] - ddy[1]));

	if(fabs(dPixelLen)<=0)
	{
		AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Msg_7"));
		return;
	}

	g_pFrm->m_pSysParam->dPixelScale= dAxisLen / dPixelLen;
	strTemp.Format("%.6f", g_pFrm->m_pSysParam->dPixelScale);
	SetDlgItemText(IDC_DLG_PIXEL_EDIT_RATE, strTemp);
	AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgPix,"Msg_8"));
}

void CDlgPixel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
}


void CDlgPixel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bLBtnDown = true;
	CDialog::OnLButtonDown(nFlags, point);
}


void CDlgPixel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bLBtnDown = false;
//	g_pView->PatShow(GetDlgItem(IDC_STATIC_PIXEL_PAT)->m_hWnd);
	CDialog::OnLButtonUp(nFlags, point);
}


void CDlgPixel::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bIsEditMod && m_bLBtnDown)
	{
		CRect rt;
		GetDlgItem(IDC_STATIC_PIXEL_PAT)->GetWindowRect(rt);
		ScreenToClient(rt);

		if (PtInRect(rt, point))
		{
				point.x -= rt.left;
				point.y -= rt.top;
				g_ProgData.m_mod[Mod_Calib].EditMod(point);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CDlgPixel::OnBnClickedDlgPixelBtnPatEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	GetDlgItemText(IDC_DLG_PIXEL_BTN_PAT_EDIT, strTemp);
	if ("编辑模板" == strTemp)
	{
		m_bIsEditMod = true;
		SetDlgItemText(IDC_DLG_PIXEL_BTN_PAT_EDIT, "编辑中");
	}
	else
	{
		m_bIsEditMod = false;
		SetDlgItemText(IDC_DLG_PIXEL_BTN_PAT_EDIT, "编辑模板");
	}
}

BOOL CDlgPixel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_PIXEL_PAT)
	{
		if (m_bIsEditMod)
		{
			SetCursor(LoadCursor(NULL,IDC_HAND));
			return TRUE;
		}
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CDlgPixel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (GetDlgItem(IDC_STATIC_PIXEL_PAT)->GetSafeHwnd() == pMsg->hwnd && 
		pMsg->message == WM_MOUSEMOVE)
	{
		OnMouseMove(MK_MBUTTON, pMsg->pt);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

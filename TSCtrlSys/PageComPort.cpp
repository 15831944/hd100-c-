// PageComPort.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PageComPort.h"
#include "afxdialogex.h"

const LPCSTR g_strComPortParamName[COMPORT_GRID_COL_NUM] = 
{_T("端 口 名 称"), _T("序 号"), _T("波 特 率")};

// CPageComPort 对话框

IMPLEMENT_DYNAMIC(CPageComPort, CPropertyPage)

CPageComPort::CPageComPort()
	: CPropertyPage(CPageComPort::IDD)
{

}

CPageComPort::~CPageComPort()
{
}

void CPageComPort::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_COM_GRID, m_ComPortGrid);
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCR_LIGHT, m_scrollLight);
}


BEGIN_MESSAGE_MAP(CPageComPort, CPropertyPage)
	ON_NOTIFY(NM_CLICK, IDC_COM_GRID, OnGridClick)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CPageComPort::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_LINK, &CPageComPort::OnBnClickedBtnLink)
	ON_BN_CLICKED(IDC_BTN_WT_SET0, &CPageComPort::OnBnClickedBtnWtSet0)
	ON_BN_CLICKED(IDC_BTN_WT_READ, &CPageComPort::OnBnClickedBtnWtRead)
	ON_BN_CLICKED(IDC_BTN_LA_READ, &CPageComPort::OnBnClickedBtnLaRead)
	ON_BN_CLICKED(IDC_BUTTON1, &CPageComPort::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_SAVE_LIGHT, &CPageComPort::OnBnClickedBtnSaveLight)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CPageComPort 消息处理程序
BOOL CPageComPort::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CreateGrid();

	m_scrollLight.SetScrollRange(0, 99);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPageComPort::CreateGrid()
{
	//nInitial用来确定图像列表包含的图像数量；nGrow用来确定图像列表可控制的图像数量
	m_ImageList.Create(30,30,ILC_COLOR24|ILC_MASK,2,1);
	CBitmap a;
	a.LoadBitmap(IDB_BITMAP_IOSTS);		
	m_ImageList.Add(&a,RGB(255,255,255));
	m_ComPortGrid.SetImageList(&m_ImageList);//设置大图标控制

	m_ComPortGrid.EnableDragAndDrop(FALSE);
	m_ComPortGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_ComPortGrid.SetEditable(TRUE);
	//OnVirtualMode();    // Sets the grid mode, fills the grid
	m_ComPortGrid.SetListMode(TRUE);
	m_ComPortGrid.SetTrackFocusCell(FALSE);
	//m_ComPortGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_ComPortGrid.SetFrameFocusCell(FALSE);	//去除鼠标点选时的边框线  
	m_ComPortGrid.SetSingleRowSelection();
	m_ComPortGrid.SetColumnCount(COMPORT_GRID_COL_NUM);		//初始列数：第一列为序号，第二列为状态，第三列为详细说明
	m_ComPortGrid.SetRowCount(COM_PORT_USED+1);		//设置初始行数量，初始化只有一行
	m_ComPortGrid.SetFixedRowCount(1);	

	m_ComPortGrid.SetFixedBkColor(RGB(128,128,192));
	m_ComPortGrid.SetGridLines(GVL_BOTH);
	//序号头宽度
	for (int i=0; i<COMPORT_GRID_COL_NUM; i++)
	{
		m_ComPortGrid.SetGridItemText(0,i,g_strComPortParamName[i]);
	}
	m_ComPortGrid.ExpandColumnsToFit();
	m_ComPortGrid.AutoSizeColumns();

	m_ComPortGrid.SetColumnResize(TRUE);
	m_ComPortGrid.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<(COM_PORT_USED+1);i++)
	{
		m_ComPortGrid.SetRowHeight(i,32);	
	}

	//调整风格
	for (UINT i=1;i<(COM_PORT_USED+1);i++)
	{
		for (UINT j=0;j<COMPORT_GRID_COL_NUM;j++)
		{	
			m_ComPortGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j)
			{	
				m_ComPortGrid.GetCell(i, j)->SetBackClr(RGB(192,192,192));
				m_ComPortGrid.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
		}
	}	

	CString str;
	int idx=1;
	for (int i=0; i<COM_PORT_USED; i++)
	{
		m_ComPortGrid.SetGridItemText(idx,0,g_pFrm->m_ComPort[i].m_szPortName);	
		str.Format(_T("%d"),g_pFrm->m_ComPort[i].m_nComNo);
		m_ComPortGrid.SetGridItemText(idx,1,str);	
		str.Format(_T("%d"),(int)g_pFrm->m_ComPort[i].m_nBaud);
		m_ComPortGrid.SetGridItemText(idx,2,str);
		idx++;
	}
}

void CPageComPort::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CPageComPort::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0; i<COM_PORT_USED; i++)
	{
		g_pFrm->m_ComPort[i].CloseConnection();
	}
}


void CPageComPort::OnBnClickedBtnLink()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = 1;
	for (int i=0; i<COM_PORT_USED; i++)
	{
		g_pFrm->m_ComPort[i].m_szPortName = m_ComPortGrid.GetItemText(index, 0);
		g_pFrm->m_ComPort[i].m_nComNo = m_ComPortGrid.GetItemDataInt(index, 1);
		g_pFrm->m_ComPort[i].m_nBaud = (DWORD)m_ComPortGrid.GetItemDataInt(index, 2);
		index++;
	}

	g_pFrm->ComPortParam(FALSE);
	g_pFrm->InitComPort();
}	

// 称重置零
void CPageComPort::OnBnClickedBtnWtSet0()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_comWeight.SetZero();
}


// 称重读数
void CPageComPort::OnBnClickedBtnWtRead()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_WT_WEIGHT, "  ");

	double dWt = 0;
	g_pFrm->m_comWeight.SetImdRpt();
	Sleep(200);
	if (!g_pFrm->m_comWeight.GetWeight(dWt))
	{
		AfxMessageBox("读取称重重量出错!");
	}	

	CString strTemp;
	strTemp.Format("%.3f", dWt);
	SetDlgItemText(IDC_EDIT_WT_WEIGHT, strTemp);
}


// 激光测高读数
void CPageComPort::OnBnClickedBtnLaRead()
{
	// TODO: 在此添加控件通知处理程序代码
	double dHt = 0;
	if (!g_pFrm->m_comLaser.ReadHeightData(dHt))
	{
		AfxMessageBox("读取激光测高数值出错!");
		return;
	}

	CString strTemp;
	strTemp.Format("%.04f",dHt);
	SetDlgItemText(IDC_EDIT_LA_HEIGHT, strTemp);
}


void CPageComPort::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int nLt = 0;
	if (!g_pFrm->m_comLight.GetLightPower(nLt))
	{
	}

	g_pFrm->m_comLight.SetLightPower(99);
}


void CPageComPort::OnBnClickedBtnSaveLight()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CPageComPort::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int iTmpPos = 0;
	CString strTemp;

	switch (nSBCode)
	{
	case SB_THUMBPOSITION:
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SCR_LIGHT:
			iTmpPos = nPos;
			m_scrollLight.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_LIGHT, strTemp);
			break;

		default:
			break;
		}
		break;

	case SB_LINERIGHT:		
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SCR_LIGHT:
			iTmpPos = m_scrollLight.GetScrollPos();
			if (iTmpPos<100)
			{
				iTmpPos++;
			}
			m_scrollLight.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_LIGHT, strTemp);
			break;

		default:
			break;
		}
		break;

	case SB_LINELEFT:
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SCR_LIGHT:
			iTmpPos = m_scrollLight.GetScrollPos();
			if (iTmpPos>0)
			{
				iTmpPos--;
			}
			m_scrollLight.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_LIGHT, strTemp);
			break;

		default:
			break;
		}
		break;

	case SB_PAGELEFT:
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SCR_LIGHT:
			iTmpPos = m_scrollLight.GetScrollPos();
			if (iTmpPos>10)
			{
				iTmpPos -= 10;
			}
			m_scrollLight.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_LIGHT, strTemp);
			break;

		default:
			break;
		}
		break;

	case SB_PAGERIGHT:		
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SCR_LIGHT:
			iTmpPos = m_scrollLight.GetScrollPos();
			if (iTmpPos<90)
			{
				iTmpPos += 10;
			}
			m_scrollLight.SetScrollPos(iTmpPos);
			strTemp.Format("%d", iTmpPos);
			SetDlgItemText(IDC_EDIT_LIGHT, strTemp);
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	g_pFrm->m_comLight.SetLightPower(iTmpPos);

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

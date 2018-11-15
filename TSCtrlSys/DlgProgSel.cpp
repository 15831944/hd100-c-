// DlgProgSel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgProgSel.h"
#include "afxdialogex.h"


// CDlgProgSel �Ի���

IMPLEMENT_DYNAMIC(CDlgProgSel, CDialogEx)

CDlgProgSel::CDlgProgSel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProgSel::IDD, pParent)
{

}

CDlgProgSel::~CDlgProgSel()
{
}

void CDlgProgSel::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_PROG_LIST, m_progListGrid);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgProgSel, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_PROG_LIST, OnGridClick)
	ON_BN_CLICKED(IDC_BTN_SEL, &CDlgProgSel::OnBnClickedBtnSel)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgProgSel::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgProgSel::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDOK, &CDlgProgSel::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgProgSel ��Ϣ�������
BOOL CDlgProgSel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CreateGrid();

	SetDlgItemText(IDC_EDIT_CUR_PROG, g_AllProgInfo[0].m_curProgName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgProgSel::CreateGrid()
{
	m_progListGrid.EnableDragAndDrop(FALSE);
	m_progListGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_progListGrid.SetEditable(TRUE);
	m_progListGrid.SetListMode(TRUE);
	m_progListGrid.SetTrackFocusCell(FALSE);
	m_progListGrid.SetFrameFocusCell(FALSE);	//ȥ������ѡʱ�ı߿���  
	m_progListGrid.SetColumnCount(3);		//��ʼ��������һ��Ϊ��ţ��ڶ���Ϊ״̬��������Ϊ��ϸ˵��
	m_progListGrid.SetRowCount(g_AllProgInfo[0].m_nProgNum+1);		//���ó�ʼ����������ʼ��ֻ��һ��
	m_progListGrid.SetFixedRowCount(1);	
	m_progListGrid.SetFixedBkColor(RGB(128,128,192));
	m_progListGrid.SetGridLines(GVL_BOTH);

	//���ͷ���
	m_progListGrid.SetGridItemText(0, 0, "   ��   Ʒ   ��   ��   ");
	m_progListGrid.SetGridItemText(0, 1, "   ��      ��      ʱ      ��   ");
	m_progListGrid.SetGridItemText(0, 2, "��        Ʒ        ��        ��");
	m_progListGrid.ExpandColumnsToFit();
	m_progListGrid.AutoSizeColumns();
	//m_progListGrid.SetColumnWidth(i,30);	//�����п�

	m_progListGrid.ExpandLastColumn();

	m_progListGrid.SetColumnResize(TRUE);
	m_progListGrid.SetRowResize(FALSE);

	//�����и�
	for(int i=0;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		m_progListGrid.SetRowHeight(i,32);	
	}

	//�������
	for (int i=1;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		for (UINT j=0;j<3;j++)
		{	
			m_progListGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
		}
	}	

	CString str;
	for(int i=1;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		POSITION ps;
		ps = g_AllProgInfo[0].m_listAllProgName.FindIndex(i-1);
		str = g_AllProgInfo[0].m_listAllProgName.GetAt(ps);
		m_progListGrid.SetGridItemText(i, 0, str);
		ps = g_AllProgInfo[0].m_listProgCreateTime.FindIndex(i-1);	
		str = g_AllProgInfo[0].m_listProgCreateTime.GetAt(ps);
		m_progListGrid.SetGridItemText(i, 1, str);
		ps = g_AllProgInfo[0].m_listProgDiscription.FindIndex(i-1);
		str = g_AllProgInfo[0].m_listProgDiscription.GetAt(ps);
		m_progListGrid.SetGridItemText(i, 2, str);
	}	
}

void CDlgProgSel::UpdataGrid()
{	
	m_progListGrid.SetRowCount(g_AllProgInfo[0].m_nProgNum+1);		//���ó�ʼ����������ʼ��ֻ��һ��

	//�������
	for (int i=1;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		for (UINT j=0;j<3;j++)
		{	
			m_progListGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
		}
	}	

	CString str;
	for(int i=1;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		POSITION ps;
		ps = g_AllProgInfo[0].m_listAllProgName.FindIndex(i-1);
		m_progListGrid.SetGridItemText(i, 0, g_AllProgInfo[0].m_listAllProgName.GetAt(ps));
		ps = g_AllProgInfo[0].m_listProgCreateTime.FindIndex(i-1);	
		m_progListGrid.SetGridItemText(i, 1, g_AllProgInfo[0].m_listProgCreateTime.GetAt(ps));
		ps = g_AllProgInfo[0].m_listProgDiscription.FindIndex(i-1);
		m_progListGrid.SetGridItemText(i, 2, g_AllProgInfo[0].m_listProgDiscription.GetAt(ps));
	}	

	SetDlgItemText(IDC_EDIT_CUR_PROG, g_AllProgInfo[0].m_curProgName);
}

void CDlgProgSel::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on ����� row %d, col %d\n"), pItem->iRow, pItem->iColumn);

// 	CString str = m_progListGrid.GetItemText(pItem->iRow,0);
// 	g_AllProgInfo[0].Select(str);
}

void CDlgProgSel::OnBnClickedBtnAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strName = "";
	GetDlgItemText(IDC_EDIT_ADD_PROG, strName);
	if ("" == strName)
	{
		g_pFrm->m_Robot->AfxMessageBox("��Ʒ���Ʋ���Ϊ��!!!");
		return;
	}

	CString strDisc = "";
	GetDlgItemText(IDC_EDIT_ADD_PROG_DISCRIPTION, strDisc);
	g_AllProgInfo[0].Add(strName, strDisc);
	UpdataGrid();
}

void CDlgProgSel::OnBnClickedBtnSel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int rowClick = m_progListGrid.GetFocusCell().row;
	if (rowClick < 1)
	{
		return ;
	}

	CString strProgName = m_progListGrid.GetItemText(rowClick, 0);
	if ("" == strProgName)
	{
		return;
	}

	g_AllProgInfo[0].Select(strProgName);
	UpdataGrid();
}

void CDlgProgSel::OnBnClickedBtnDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int rowClick = m_progListGrid.GetFocusCell().row;
	if (rowClick < 1)
	{
		g_pFrm->m_Robot->AfxMessageBox(_T("û��ѡ��Ҫɾ���ĳ���!"));
		return ;
	}

	CString strProgName = m_progListGrid.GetItemText(rowClick, 0);
	if ("" == strProgName)
	{
		return;
	}

	if (IDOK != g_pFrm->m_Robot->AfxMessageBox(_T("�Ƿ�Ҫɾ������ɾ�����޷��ָ�!"), MB_OKCANCEL))
	{
		return;
	}

	g_AllProgInfo[0].Delete(strProgName);
	UpdataGrid();
}


void CDlgProgSel::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp[3];

	g_AllProgInfo[0].m_listAllProgName.RemoveAll();
	g_AllProgInfo[0].m_listProgCreateTime.RemoveAll();
	g_AllProgInfo[0].m_listProgDiscription.RemoveAll();
	for(int i=1;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		strTemp[0] = m_progListGrid.GetItemText(i, 0);
		g_AllProgInfo[0].m_listAllProgName.AddTail(strTemp[0]);
		strTemp[1] = m_progListGrid.GetItemText(i, 1);
		g_AllProgInfo[0].m_listProgCreateTime.AddTail(strTemp[1]);
		strTemp[2] = m_progListGrid.GetItemText(i, 2);
		g_AllProgInfo[0].m_listProgDiscription.AddTail(strTemp[2]);
	}

	g_AllProgInfo[0].HandleAllProg(FALSE);
	g_ProgData.LoadProgData();

	CDialogEx::OnOK();
}

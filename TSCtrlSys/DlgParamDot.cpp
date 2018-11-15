// DlgParamDot.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgParamDot.h"
#include "afxdialogex.h"
#include "TSCtrlSys.h"

// CDlgParamDot �Ի���

LPCSTR g_strDotParamName[DOT_GRID_COL_NUM] = 
{_T("��������"), _T("���"), _T("ͣ��ʱ��"), _T("Z���½��ٶ�"),_T("Z���½����ٶ�"),
_T("����ʱ��"),_T("�㽺�߶�"),_T("������"),
_T("�㽺��ͣ��ʱ��"), _T("�ܿ������߶�"),_T("�����ٶ�"), _T("�������ٶ�"),
_T("����ʱ��")};

IMPLEMENT_DYNAMIC(CDlgParamDot, CDialogEx)

CDlgParamDot::CDlgParamDot(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgParamDot::IDD, pParent)
{

}

CDlgParamDot::~CDlgParamDot()
{
}

void CDlgParamDot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_PARAM_DOT_GRID, m_DotParamGrid);
}


BEGIN_MESSAGE_MAP(CDlgParamDot, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgParamDot::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_DLG_PARAM_DOT_GRID, OnGridClick)
	ON_BN_CLICKED(IDC_BTN_PARAM_DOT_SAVE, &CDlgParamDot::OnBnClickedBtnParamDotSave)
END_MESSAGE_MAP()


// CDlgParamDot ��Ϣ�������


BOOL CDlgParamDot::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CreateGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgParamDot::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������



	CDialogEx::OnOK();
}

void CDlgParamDot::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CDlgParamDot::CreateGrid()
{
	//ֻ֧��8λ
	//m_ImageList.Create(MAKEINTRESOURCE(IDB_BITMAP_IOSTS), 28, 1, RGB(255,255,255));

	//nInitial����ȷ��ͼ���б������ͼ��������nGrow����ȷ��ͼ���б�ɿ��Ƶ�ͼ������
	//	m_ImageList.Create(30,30,ILC_COLOR24|ILC_MASK,2,1);
	// 	CBitmap a;
	// 	a.LoadBitmap(IDB_BITMAP_IOSTS);		
	// 	m_ImageList.Add(&a,RGB(255,255,255));

	//	m_MotorParamGrid.SetImageList(&m_ImageList);//���ô�ͼ�����



	m_DotParamGrid.EnableDragAndDrop(FALSE);
	m_DotParamGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_DotParamGrid.SetEditable(TRUE);
	//OnVirtualMode();    // Sets the grid mode, fills the grid
	m_DotParamGrid.SetListMode(TRUE);
	m_DotParamGrid.SetTrackFocusCell(FALSE);
	//m_MotorParamGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_DotParamGrid.SetFrameFocusCell(FALSE);	//ȥ������ѡʱ�ı߿���  
	// 	m_MotorParamGrid.SetColumnCount(0);			//���Ĭ�����õ�״̬�����´���	
	// 	m_MotorParamGrid.SetRowCount(0);			//���ó�ʼ����������ʼ��ֻ��һ��
	//	m_MotorParamGrid.SetSingleRowSelection();
	m_DotParamGrid.SetColumnCount(DOT_GRID_COL_NUM);			// ��ʼ��������һ��Ϊ��ţ��ڶ���Ϊ״̬��������Ϊ��ϸ˵��
	m_DotParamGrid.SetRowCount(DOT_GRID_HEIGHT+1);				// ���ó�ʼ����������ʼ��ֻ��һ��
	m_DotParamGrid.SetFixedRowCount(1);	

	m_DotParamGrid.SetFixedBkColor(RGB(128,128,192));
	m_DotParamGrid.SetGridLines(GVL_BOTH);
	//���ͷ���
	for (int i=0; i<DOT_GRID_COL_NUM; i++)
	{
		m_DotParamGrid.SetGridItemText(0,i,g_strDotParamName[i]);
		//size_t len = strlen(g_strMtrParamName[i]);
		m_DotParamGrid.ExpandColumnsToFit();
		//m_MotorParamGrid.SetColumnWidth(i,30);	//�����п�
	}
	m_DotParamGrid.AutoSizeColumns();
	//m_MotorParamGrid.ExpandLastColumn();

	m_DotParamGrid.SetColumnResize(TRUE);
	m_DotParamGrid.SetRowResize(FALSE);

	//�����и�
	for(UINT i=0;i<(DOT_GRID_HEIGHT+1);i++)
	{
		m_DotParamGrid.SetRowHeight(i,32);	
	}

	//�������
	for (UINT i=1;i<(DOT_GRID_HEIGHT+1);i++)
	{
		for (UINT j=0;j<DOT_GRID_COL_NUM;j++)
		{	
			m_DotParamGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j||1==j/*||9==j||14==j||15==j*/)
			{	// ����ID���������͡������͡����㷽���ܸ���
				m_DotParamGrid.GetCell(i, j)->SetBackClr(RGB(192,192,192));
				m_DotParamGrid.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
			if (2==j||3==j||4==j)
			{
				m_DotParamGrid.GetCell(i, j)->SetBackClr(RGB(230,230,230));
			}
			if (5==j||6==j||7==j)
			{
				m_DotParamGrid.GetCell(i, j)->SetBackClr(RGB(143,138,242));
			}
		}
	}	

	// 	m_MotorParamGrid.SetGridItemText(0,0,_T("�˿ں�"));
	// 	m_MotorParamGrid.SetGridItemText(0,1,_T("״̬"));
	// 	m_MotorParamGrid.SetGridItemText(0,2,_T("˵��"));

	CString str;
	for(UINT i=1;i<(MAX_DOT_PARAM_NUM+1);i++)
	{
		int j=0;
		m_DotParamGrid.SetGridItemText(i,j,g_pFrm->m_DotParam[i-1].strName);	j++;
		str.Format(_T("%d"),g_pFrm->m_DotParam[i-1].nTypeID);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfSettlingTime);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfDownSpeed);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfDownAccel);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfOnTime);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfDispenseGap);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfNumShots);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfDwellSecs);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfRetractDistance);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfRetractSpeed);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfRetractAccel);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfSuckback);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;

//		m_DotParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamGts[i-1].m_MoveMode);j++;
//		m_DotParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamGts[i-1].m_CardType);
	}
}

void CDlgParamDot::OnBnClickedBtnParamDotSave()
{
	CString str;
	// ���������
	for(UINT i=1;i<(MAX_DOT_PARAM_NUM+1);i++)
	{
		int j=0;
		g_pFrm->m_DotParam[i-1].strName			= m_DotParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_DotParam[i-1].nTypeID			= m_DotParamGrid.GetItemDataInt(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfSettlingTime	= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfDownSpeed				= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfDownAccel				= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		
		g_pFrm->m_DotParam[i-1].dfOnTime				= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfDispenseGap			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfNumShots			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		
		g_pFrm->m_DotParam[i-1].dfDwellSecs			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfRetractDistance			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfRetractSpeed  = m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfRetractAccel  = m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfSuckback			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
//		g_pFrm->m_DotParam[i-1].m_HomeStep			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
//		g_pFrm->m_DotParam[i-1].m_MoveMode			= m_DotParamGrid.GetItemText(i, j);			j++;
//		g_pFrm->m_DotParam[i-1].m_CardType			= m_DotParamGrid.GetItemText(i, j);			
	}	

	g_pFrm->DotParam(FALSE);
	GetDlgItem(IDC_STATIC_PARAM_DOT)->SetWindowText(_T("�������"));
}

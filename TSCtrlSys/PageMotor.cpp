// PageMotor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PageMotor.h"
#include "afxdialogex.h"


LPCSTR g_strMtrParamName[MOTOR_GRID_COL_NUM] = 
{_T("�������"), _T("���"), _T("�ֶ��ٶ�"), _T("����ٶ�"),_T("���ٶ�"), _T("�����ٶ�"),_T("�������"),
_T("������ٶ�"),_T("�����������"), _T("���㷽��"),_T("������"), _T("��  ��  ��"), _T("��  ��  ��"), 
/*_T("����"),_T("ÿȦ������"), _T("�������"), */_T("������������"), _T("�˶���ʽ"), _T("������")};
// CPageMotor �Ի���

IMPLEMENT_DYNAMIC(CPageMotor, CPropertyPage)

CPageMotor::CPageMotor()
	: CPropertyPage(CPageMotor::IDD)
{

}

CPageMotor::~CPageMotor()
{
}

void CPageMotor::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_DLG_MTR_GRID, m_MotorParamGrid);
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageMotor, CPropertyPage)
	ON_NOTIFY(NM_CLICK, IDC_DLG_MTR_GRID, OnGridClick)
	ON_BN_CLICKED(ID_SAVE, &CPageMotor::OnBnClickedSave)
END_MESSAGE_MAP()


// CPageMotor ��Ϣ�������


BOOL CPageMotor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CreateGrid();

// 	CRect rc;
// 	GetClientRect(&rc);
//	GetDlgItem(IDC_DLG_MTR_GRID)->MoveWindow(0,0,rc.right-rc.left-5,rc.bottom-rc.top);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPageMotor::CreateGrid()
{
	//ֻ֧��8λ
	//m_ImageList.Create(MAKEINTRESOURCE(IDB_BITMAP_IOSTS), 28, 1, RGB(255,255,255));

	//nInitial����ȷ��ͼ���б������ͼ��������nGrow����ȷ��ͼ���б�ɿ��Ƶ�ͼ������
//	m_ImageList.Create(30,30,ILC_COLOR24|ILC_MASK,2,1);
// 	CBitmap a;
// 	a.LoadBitmap(IDB_BITMAP_IOSTS);		
// 	m_ImageList.Add(&a,RGB(255,255,255));

//	m_MotorParamGrid.SetImageList(&m_ImageList);//���ô�ͼ�����

	m_MotorParamGrid.EnableDragAndDrop(FALSE);
	m_MotorParamGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_MotorParamGrid.SetEditable(TRUE);
	//OnVirtualMode();    // Sets the grid mode, fills the grid
	m_MotorParamGrid.SetListMode(TRUE);
	m_MotorParamGrid.SetTrackFocusCell(FALSE);
	//m_MotorParamGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_MotorParamGrid.SetFrameFocusCell(FALSE);	//ȥ������ѡʱ�ı߿���  
// 	m_MotorParamGrid.SetColumnCount(0);			//���Ĭ�����õ�״̬�����´���	
// 	m_MotorParamGrid.SetRowCount(0);			//���ó�ʼ����������ʼ��ֻ��һ��
//	m_MotorParamGrid.SetSingleRowSelection();
	m_MotorParamGrid.SetColumnCount(MOTOR_GRID_COL_NUM);		//��ʼ��������һ��Ϊ��ţ��ڶ���Ϊ״̬��������Ϊ��ϸ˵��
	m_MotorParamGrid.SetRowCount(MAX_AXIS_NUM+LS_AXIS_NUM+1);		//���ó�ʼ����������ʼ��ֻ��һ��
	m_MotorParamGrid.SetFixedRowCount(1);	

// 	CFont m_font2;
// 	m_font2.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,
// 			CLIP_STROKE_PRECIS,DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("Arial"));
// 	m_MotorParamGrid.SetFont(&m_font2);

	m_MotorParamGrid.SetFixedBkColor(RGB(128,128,192));
	m_MotorParamGrid.SetGridLines(GVL_BOTH);
	//���ͷ���
	for (int i=0; i<MOTOR_GRID_COL_NUM; i++)
	{
		m_MotorParamGrid.SetGridItemText(0,i,g_strMtrParamName[i]);
		//size_t len = strlen(g_strMtrParamName[i]);
		m_MotorParamGrid.ExpandColumnsToFit();
		//m_MotorParamGrid.SetColumnWidth(i,30);	//�����п�
	}
	m_MotorParamGrid.AutoSizeColumns();
	//m_MotorParamGrid.ExpandLastColumn();

	m_MotorParamGrid.SetColumnResize(TRUE);
	m_MotorParamGrid.SetRowResize(FALSE);

	//�����и�
	for(UINT i=0;i<(MAX_AXIS_NUM+LS_AXIS_NUM+1);i++)
	{
		m_MotorParamGrid.SetRowHeight(i,32);	
	}

	//�������
	for (UINT i=1;i<(MAX_AXIS_NUM+LS_AXIS_NUM+1);i++)
	{
		for (UINT j=0;j<MOTOR_GRID_COL_NUM;j++)
		{	
			m_MotorParamGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j||1==j||9==j||14==j||15==j)
			{	// ���ID���˶����͡������͡����㷽���ܸ���
				m_MotorParamGrid.GetCell(i, j)->SetBackClr(RGB(192,192,192));
				m_MotorParamGrid.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
		}
	}	

// 	m_MotorParamGrid.SetGridItemText(0,0,_T("�˿ں�"));
// 	m_MotorParamGrid.SetGridItemText(0,1,_T("״̬"));
// 	m_MotorParamGrid.SetGridItemText(0,2,_T("˵��"));

	CString str;
	for(UINT i=1;i<(MAX_AXIS_NUM+1);i++)
	{
		int j=0;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamGts[i-1].m_MotorName);	j++;
		str.Format(_T("%d"),g_pFrm->m_mtrParamGts[i-1].m_MotorID);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_VhandMove);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_Vmax);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_Acc);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_Vhome);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_VhomeLow);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_AccHome);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_HomeReturn);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%d"),g_pFrm->m_mtrParamGts[i-1].m_HomeDir);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_MaxTravelNegative);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_MaxTravelPositive);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_Resolution);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;

		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_HomeStep);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamGts[i-1].m_MoveMode);j++;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamGts[i-1].m_CardType);
	}	
	for(UINT i=MAX_AXIS_NUM+1;i<(MAX_AXIS_NUM+LS_AXIS_NUM+1);i++)
	{
		int j=0;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamLs[i-MAX_AXIS_NUM-1].m_MotorName);	j++;
		str.Format(_T("%d"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MotorID);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_VhandMove);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Vmax);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Acc);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Vhome);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_VhomeLow);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_AccHome);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeReturn);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%d"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeDir);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MaxTravelNegative);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MaxTravelPositive);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Resolution);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;

		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeStep);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MoveMode);j++;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_CardType);
	}
}

void CPageMotor::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CPageMotor::OnBnClickedSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	// �̸߿���������
	for(UINT i=1;i<(MAX_AXIS_NUM+1);i++)
	{
		int j=0;
		g_pFrm->m_mtrParamGts[i-1].m_MotorName			= m_MotorParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_mtrParamGts[i-1].m_MotorID			= m_MotorParamGrid.GetItemDataInt(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_VhandMove				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_Vmax				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_Acc				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_Vhome				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_VhomeLow			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_AccHome			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_HomeReturn			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_HomeDir			= m_MotorParamGrid.GetItemDataInt(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_MaxTravelNegative  = m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_MaxTravelPositive  = m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_Resolution			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_HomeStep			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_MoveMode			= m_MotorParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_mtrParamGts[i-1].m_CardType			= m_MotorParamGrid.GetItemText(i, j);			
	}	
	// ��������������
	for(UINT i=MAX_AXIS_NUM+1;i<(MAX_AXIS_NUM+LS_AXIS_NUM+1);i++)
	{
		int j=0;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MotorName			= m_MotorParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MotorID			= m_MotorParamGrid.GetItemDataInt(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_VhandMove				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Vmax				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Acc				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Vhome				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_VhomeLow			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_AccHome			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeReturn			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeDir			= m_MotorParamGrid.GetItemDataInt(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MaxTravelNegative  = m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MaxTravelPositive  = m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Resolution			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeStep			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MoveMode			= m_MotorParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_CardType			= m_MotorParamGrid.GetItemText(i, j);			
	}	
	g_pFrm->MotorParam(FALSE);
	AfxMessageBox("�������!");
}

// DlgParamLine.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgParamLine.h"
#include "afxdialogex.h"
#include "TSCtrlSys.h"

// CDlgParamLine �Ի���
LPCSTR g_strLineParamName[LINE_GRID_COL_NUM] = 
{_T("��������"), _T("���"), _T("ͣ��ʱ��"), _T("Z���½��ٶ�"),_T("Z���½����ٶ�"),
_T("�㽺�߶�"),_T("�㽺�߶��ٶ�"),_T("��ǰ�ؽ�����"),_T("����ʱ��"),_T("�㽺��ͣ��ʱ��"),
_T("�ܿ������߶�"),_T("�ܿ�̧�߸߶�"), _T("���߾���"),
_T("�ܿ������ٶ�"),_T("�ܿ��������ٶ�")};

IMPLEMENT_DYNAMIC(CDlgParamLine, CDialogEx)

CDlgParamLine::CDlgParamLine(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgParamLine::IDD, pParent)
{

}

CDlgParamLine::~CDlgParamLine()
{
}

void CDlgParamLine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_PARAM_LINE_GRID, m_LineParamGrid);
}


BEGIN_MESSAGE_MAP(CDlgParamLine, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_DLG_PARAM_LINE_GRID, OnGridClick)
	ON_BN_CLICKED(IDC_BTN_PARAM_LINE_SAVE, &CDlgParamLine::OnBnClickedBtnParamLineSave)
END_MESSAGE_MAP()


// CDlgParamLine ��Ϣ�������


BOOL CDlgParamLine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CreateGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgParamLine::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CDlgParamLine::CreateGrid()
{
	m_LineParamGrid.EnableDragAndDrop(FALSE);
	m_LineParamGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_LineParamGrid.SetEditable(TRUE);
	//OnVirtualMode();    // Sets the grid mode, fills the grid
	m_LineParamGrid.SetListMode(TRUE);
	m_LineParamGrid.SetTrackFocusCell(FALSE);
	//m_MotorParamGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_LineParamGrid.SetFrameFocusCell(FALSE);	//ȥ������ѡʱ�ı߿���  
	// 	m_MotorParamGrid.SetColumnCount(0);			//���Ĭ�����õ�״̬�����´���	
	// 	m_MotorParamGrid.SetRowCount(0);			//���ó�ʼ����������ʼ��ֻ��һ��
	//	m_MotorParamGrid.SetSingleRowSelection();
	m_LineParamGrid.SetColumnCount(LINE_GRID_COL_NUM);			// ��ʼ��������һ��Ϊ��ţ��ڶ���Ϊ״̬��������Ϊ��ϸ˵��
	m_LineParamGrid.SetRowCount(LINE_GRID_HEIGHT+1);		//���ó�ʼ����������ʼ��ֻ��һ��
	m_LineParamGrid.SetFixedRowCount(1);	

	m_LineParamGrid.SetFixedBkColor(RGB(128,128,192));
	m_LineParamGrid.SetGridLines(GVL_BOTH);
	//���ͷ���
	for (int i=0; i<LINE_GRID_COL_NUM; i++)
	{
		m_LineParamGrid.SetGridItemText(0,i,g_strLineParamName[i]);
		//size_t len = strlen(g_strMtrParamName[i]);
		m_LineParamGrid.ExpandColumnsToFit();
		//m_MotorParamGrid.SetColumnWidth(i,30);	//�����п�
	}
	m_LineParamGrid.AutoSizeColumns();
	//m_MotorParamGrid.ExpandLastColumn();

	m_LineParamGrid.SetColumnResize(TRUE);
	m_LineParamGrid.SetRowResize(FALSE);

	//�����и�
	for(UINT i=0;i<(LINE_GRID_HEIGHT+1);i++)
	{
		m_LineParamGrid.SetRowHeight(i,32);	
	}

	//�������
	for (UINT i=1;i<(LINE_GRID_HEIGHT+1);i++)
	{
		for (UINT j=0;j<LINE_GRID_COL_NUM;j++)
		{	
			m_LineParamGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j||1==j/*||9==j||14==j||15==j*/)
			{	// ����ID���������Ͳ��ܸ���
				m_LineParamGrid.GetCell(i, j)->SetBackClr(RGB(192,192,192));
				m_LineParamGrid.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
			if (2==j||3==j||4==j)
			{
				m_LineParamGrid.GetCell(i, j)->SetBackClr(RGB(191,188,248));
			}
			
			if (5==j||6==j||7==j||8==j||9==j)
			{
				m_LineParamGrid.GetCell(i, j)->SetBackClr(RGB(143,138,242));
			}
		}
	}	

	// 	m_MotorParamGrid.SetGridItemText(0,0,_T("�˿ں�"));
	// 	m_MotorParamGrid.SetGridItemText(0,1,_T("״̬"));
	// 	m_MotorParamGrid.SetGridItemText(0,2,_T("˵��"));

	CString str;
	for(UINT i=1;i<(MAX_LINE_PARAM_NUM+1);i++)
	{
		int j=0;
		m_LineParamGrid.SetGridItemText(i,j,g_pFrm->m_LineParm[i-1].strName);	j++;
		str.Format(_T("%d"),g_pFrm->m_LineParm[i-1].nTypeID);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfPremoveDelay);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfDownSpeed);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfDownAccel);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfDispenseGap);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfSpeed);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfShutOffDistance);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfSuckback);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfDwellSecs);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfRetractDistance);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfBacktrackGap);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfBacktrackLength);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfRetractSpeed);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfRetractAccel);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
	}
}

void CDlgParamLine::OnBnClickedBtnParamLineSave()
{
	CString str;
	// �߲�������
	for(UINT i=1;i<(MAX_LINE_PARAM_NUM+1);i++)
	{
		int j=0;
		g_pFrm->m_LineParm[i-1].strName			= m_LineParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_LineParm[i-1].nTypeID			= m_LineParamGrid.GetItemDataInt(i, j);		j++;
		
		g_pFrm->m_LineParm[i-1].dfPremoveDelay				= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfDownSpeed				= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfDownAccel				= m_LineParamGrid.GetItemDataDouble(i, j);		j++;

		g_pFrm->m_LineParm[i-1].dfDispenseGap				= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfSpeed			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfShutOffDistance			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;

		g_pFrm->m_LineParm[i-1].dfSuckback			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfDwellSecs			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfRetractDistance  = m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfBacktrackGap  = m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfBacktrackLength			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfRetractSpeed			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfRetractAccel			= m_LineParamGrid.GetItemDataDouble(i, j);			j++;
		
	}	

	g_pFrm->DotParam(FALSE);
	AfxMessageBox("�������!");
}

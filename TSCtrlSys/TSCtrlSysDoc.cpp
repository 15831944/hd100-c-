
// TSCtrlSysDoc.cpp : CTSCtrlSysDoc 类的实现
//

#include "stdafx.h"
#include "TSCtrlSys.h"

#include "DlgProgSel.h"
#include "TSCtrlSysDoc.h"
#include "DxfClass.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CProg_List g_AllProgInfo[NUM_CHANNEL];	// 记录所有产品名称
CProg_Data g_ProgData;		// 当前的产品信息
// CTSCtrlSysDoc

IMPLEMENT_DYNCREATE(CTSCtrlSysDoc, CDocument)

BEGIN_MESSAGE_MAP(CTSCtrlSysDoc, CDocument)
	ON_COMMAND(ID_FILE_NEW, &CTSCtrlSysDoc::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN_DXF, &CTSCtrlSysDoc::OnFileOpenDxf)
	ON_COMMAND(ID_FILE_SAVE, &CTSCtrlSysDoc::OnFileSave)
END_MESSAGE_MAP()


// CTSCtrlSysDoc 构造/析构

CTSCtrlSysDoc::CTSCtrlSysDoc()
{
	for (int i=0; i<NUM_CHANNEL; i++)
	{
		g_AllProgInfo[i].SetChannel(i);
	}
	// TODO: 在此添加一次性构造代码
}

CTSCtrlSysDoc::~CTSCtrlSysDoc()
{

}

BOOL CTSCtrlSysDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	g_pDoc = this;
	return TRUE;
}

// CTSCtrlSysDoc 序列化

void CTSCtrlSysDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CTSCtrlSysDoc 诊断

#ifdef _DEBUG
void CTSCtrlSysDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTSCtrlSysDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTSCtrlSysDoc 命令

BOOL CTSCtrlSysDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
 	if (!CDocument::OnOpenDocument(lpszPathName))
 		return FALSE;

	// TODO:  Add your specialized creation code here
	if(g_pFrm != NULL)
	{
		SetPathName(lpszPathName);
		g_pFrm->m_wndEditBar.ReadCmdLine(GetPathName());
		SetModifiedFlag(FALSE);
		g_pFrm->SetWindowTextEx();
		SetTitle(GetPathName());
	}
	return TRUE;
}

BOOL CTSCtrlSysDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if(g_pFrm != NULL)
	{
		g_pFrm->m_wndEditBar.SaveCmdLine(lpszPathName);
	}
    // 没有Mark指令时 完成保存
	if(g_pFrm->m_wndEditBar.CheckMark() == FALSE)
	{
		SetPathName(lpszPathName);
		SetModifiedFlag(FALSE);   //FALSE
		g_pFrm->SetWindowTextEx();
		return TRUE;
	}

	CString strTemp(lpszPathName);
	strTemp = strTemp.Right(strTemp.GetLength()-strTemp.ReverseFind('\\'));

	int nMarkGropNum = 1;

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
	
	for (int j=0;j<nMarkGropNum;j++)
	{	
		for ( int i=0; i<2; i++ )
		{
			CString strPath = GetModeFile(i+1,j+1);
			CString strNewPath, strFileName;
			strNewPath = strPath.Left( strPath.ReverseFind('\\') );
			strFileName.Format(_T("%s%s_%d_%d.mod"), strNewPath, strTemp,j+1,i+1);
			BOOL rtn ;
			rtn = CopyFile( strPath, strFileName, TRUE);
			g_pFrm->AddMsg(strPath);
			g_pFrm->AddMsg(strFileName);

		}
	}

	SetPathName(lpszPathName);
	SetModifiedFlag(FALSE);   //FALSE
	g_pFrm->SetWindowTextEx();

	return TRUE;
}

void CTSCtrlSysDoc::OnCloseDocument()
{
	// TODO: Add your specialized code here and/or call the base class

	CDocument::OnCloseDocument();
}

void CTSCtrlSysDoc::OnFileNew()
{
	g_pFrm->ShowPathEditDialog();
	return;
	// TODO: Add your command handler code here
// 	CFileDialog FileDlg(FALSE,"*.ctrl","无标题",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
// 		"ITSCtrl Files(*.ctrl)|*.ctrl||");
// 
// 	CString path; 
// 	GetModuleFileName(NULL,path.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
// 	path.ReleaseBuffer(); 
// 	int pos = path.ReverseFind('\\'); 
// 	path = path.Left(pos); 
// 	path +=_T("\\Project");
// 	if ( FALSE == PathFileExists(path) )
// 	{
// 		CreateDirectory(path, NULL);
// 	}
// 	FileDlg.m_ofn.lpstrTitle = "新建"; 
// 	FileDlg.m_ofn.lpstrInitialDir=path;
// 
// 	if(FileDlg.DoModal()==IDOK)
// 	{
// 		//AfxMessageBox( FileDlg.GetPathName() );
// 		//g_pView->SendMessage(WM_COMMAND, MAKEWPARAM(ID_FILE_SAVE_AS,0),0);
// 		SetPathName(FileDlg.GetPathName());
// 		SetModifiedFlag(FALSE);//FALSE
// 		g_pFrm->SetWindowTextEx();
// 		g_pFrm->m_wndEditBar.OnCmdType(ID_CMD_CLEAR_ALL);
// 	} 
}

// 模板路径 
CString CTSCtrlSysDoc::GetModeFile(int iIndex,int nGroupID)   //1:Mark1, 2: Mark2
{
	CString strTemp;
    strTemp.Format("product\\%s_%d_%d.mod", GetTitle(), nGroupID,iIndex);
	return (CFunction::GetDirectory() + strTemp);
}

// 基板模板路径 
CString CTSCtrlSysDoc::GetBoardModeFile(int iIndex)//0:A, 1: B
{
	CString strTemp;
	if(iIndex)
	{
      strTemp.Format("product\\%s_A.mod", "Board");
	}
	else
	{
       strTemp.Format("product\\%s_B.mod", "Board");
	}
	return (CFunction::GetDirectory() + strTemp);
}

CString CTSCtrlSysDoc::GetModeFileEx(int iIndex)//0:Mark1, 1: Mark2
{
	CString strTemp;
	strTemp.Format("product\\%s_%d.mod", "CCD", iIndex);
	return (CFunction::GetDirectory() + strTemp);
}

void CTSCtrlSysDoc::OnFileOpenDxf()
{
	// TODO: Add your command handler code here
	CFileDialog fd(TRUE, "dxf", NULL, 4|2, "DXF Files(*.dxf)|*.dxf||");
	if(fd.DoModal() == IDOK)
	{
		CString strTemp;

		CFileDialog fcmd(FALSE, "ctrl", NULL, 4|2, "TSCtrl Files(*.ctrl)|*.ctrl||");
		if(fcmd.DoModal() != IDOK)
			return;
		CDxfClass dxf;
		dxf.OpenDxfFile(fd.GetPathName(), fcmd.GetPathName());

		SetPathName(fcmd.GetPathName());

		if(g_pFrm != NULL)
		{
			strTemp.Format("DXF转档:%s --> %s", fd.GetPathName(), fcmd.GetPathName());
			//g_pFrm->m_wndEditBar.AddMsg(strTemp);
			g_pFrm->AddMsg(strTemp);

			g_pFrm->m_wndEditBar.ReadCmdLine(fcmd.GetPathName());
			g_pFrm->SetWindowTextEx();
			
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//							Prog List									//
//////////////////////////////////////////////////////////////////////////
//Prog_Info g_AllProgName;

CProg_List::CProg_List()
{
	m_nChannel = -1;
	for (int i=0; i<NUM_CHANNEL; i++)
	{
		m_curProgName = "";
		m_nProgNum = 0;
		m_listAllProgInfo.RemoveAll();
	}
}    

CProg_List::~CProg_List()
{
	m_listAllProgInfo.RemoveAll();
}

void CProg_List::SetChannel(int iChannel)
{
	m_nChannel = iChannel;
}

void CProg_List::HandleAllProg(const BOOL bIfRead, const int unUsedParam)		// 参数   bIfRead  TRUE:从文件加载数据    FALSE:保存数据到文件	
{
	UNREFERENCED_PARAMETER(unUsedParam);
	CString strFile;		
	strFile = CFunction::GetDirectory() + "product\\progName.ini";
	CString strKey, strApp;
	CString strTemp('\0', 100);
	tgStructProgInfo tmpInfo;

	strApp.Format(_T("Channel_%d"), m_nChannel);

	strKey.Format(_T("ProgNum"));
	CFunction::HandleInt(bIfRead, strApp, strKey, m_nProgNum, strFile);

	int i=0;
	if (bIfRead)
	{
		strKey.Format(_T("CurProg"));
		CFunction::HandleString(bIfRead, strApp, strKey, strTemp, strFile);
		m_curProgName.Format("%s", strTemp);

		m_listAllProgInfo.RemoveAll();
		for (i=0; i<m_nProgNum; i++)
		{		
			strKey.Format(_T("Prog%d"), i);
			CFunction::HandleString(bIfRead, strApp, strKey, strTemp, strFile);
			tmpInfo.m_strProgName.Format("%s", strTemp);

			strKey.Format(_T("CreateTime%d"), i);
			CFunction::HandleString(bIfRead, strApp, strKey, strTemp, strFile);
			tmpInfo.m_strCreateTime.Format("%s", strTemp);

			strKey.Format(_T("Discription%d"), i);
			CFunction::HandleString(bIfRead, strApp, strKey, strTemp, strFile);
			tmpInfo.m_strDiscription.Format("%s", strTemp);

			strKey.Format(_T("LastCallTime%d"), i);
			CFunction::HandleString(bIfRead, strApp, strKey, strTemp, strFile);
			if ("" == strTemp)
				tmpInfo.m_strLastSelect.Format("%s", tmpInfo.m_strCreateTime);
			else
				tmpInfo.m_strLastSelect.Format("%s", strTemp);

			m_listAllProgInfo.AddTail(tmpInfo);
		}

		if ("" != m_curProgName)
		{
			g_ProgData.ResetProgData();	
			g_ProgData.SetCurProg(m_curProgName);
			g_ProgData.LoadProgData();
		}
	}
	else
	{
		strKey.Format(_T("CurProg"));
		CFunction::HandleString(bIfRead, strApp, strKey, m_curProgName, strFile);

		for (i=0; i<m_nProgNum; i++)
		{		
			POSITION pz;

			pz = m_listAllProgInfo.FindIndex(i);
			tmpInfo = m_listAllProgInfo.GetAt(pz);

			strKey.Format(_T("Prog%d"), i);
			CFunction::HandleString(bIfRead, strApp, strKey, tmpInfo.m_strProgName, strFile);
			strKey.Format(_T("CreateTime%d"), i);
			CFunction::HandleString(bIfRead, strApp, strKey, tmpInfo.m_strCreateTime, strFile);
			strKey.Format(_T("Discription%d"), i);
			CFunction::HandleString(bIfRead, strApp, strKey, tmpInfo.m_strDiscription, strFile);
			strKey.Format(_T("LastCallTime%d"), i);
			CFunction::HandleString(bIfRead, strApp, strKey, tmpInfo.m_strLastSelect, strFile);
		}

		if ("" != m_curProgName)
		{
			g_ProgData.SaveProgData();
		}
	}
}

void CProg_List::Add(CString progName, CString progDiscription)
{
	if ("" == progName)
	{
		return;
	}

	m_curProgName = progName;

	tgStructProgInfo tmpInfo;
	tmpInfo.m_strProgName = progName;
	CTime tm = CTime::GetCurrentTime();
	CString tmpTime = tm.Format("%Y-%m-%d %H:%M:%S");
	tmpInfo.m_strCreateTime = tmpTime;
	tmpInfo.m_strLastSelect = tmpTime;
	tmpInfo.m_strDiscription = progDiscription;
	m_listAllProgInfo.AddHead(tmpInfo);

	m_nProgNum++;

	g_ProgData.SetCurProg(m_curProgName);
	g_ProgData.ResetProgData();

	CString strFile;		
	strFile = CFunction::GetDirectory() + "product\\" + progName;
	_mkdir(strFile);

	HandleAllProg(FALSE);
}

void CProg_List::Delete(const CString progName)
{
	if ("" == progName)		return;
	if (0 == m_nProgNum)	return;

	POSITION ps;
	for (int i=0; i<m_nProgNum; i++)
	{
		ps = m_listAllProgInfo.FindIndex(i);
		if (progName == m_listAllProgInfo.GetAt(ps).m_strProgName)
		{
			m_listAllProgInfo.RemoveAt(ps);
			
			m_nProgNum--;

			CString strDir;
			strDir = CFunction::GetDirectory() + "product\\" + progName;

			CFunction::DeleteDirectory(strDir.GetBuffer(strDir.GetLength()));
			strDir.ReleaseBuffer();

			break;
		}
	}
	
	HandleAllProg(FALSE);
}

void CProg_List::Select(const CString progName)	// 选择程序
{
	if ("" == progName)				return;
	if (0  == m_nProgNum)			return;

	POSITION ps;
	tgStructProgInfo tmpInfo;
	for (int i=0; i<m_nProgNum; i++)
	{
		ps = m_listAllProgInfo.FindIndex(i);
		tmpInfo = m_listAllProgInfo.GetAt(ps);
		if (progName == m_listAllProgInfo.GetAt(ps).m_strProgName)
		{
			m_curProgName = progName;
			g_ProgData.SetCurProg(progName);
			g_ProgData.LoadProgData();

			CTime tm = CTime::GetCurrentTime();
			CString tmpTime = tm.Format("%Y-%m-%d %H:%M:%S");
			tmpInfo.m_strLastSelect = tmpTime;

			m_listAllProgInfo.RemoveAt(ps);
			m_listAllProgInfo.AddHead(tmpInfo);
			return;
		}
	}

	if (progName != m_curProgName)
	{
		AfxMessageBox("选择程序出错!");
	}
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CProg_Data::CProg_Data()
{
	m_nChannel = 0;	
	m_strProgName = "";
};

CProg_Data::~CProg_Data()
{

};

void CProg_Data::SetCurProg(CString strProg)
{
	m_strProgName.Format("%s", strProg);
};

void CProg_Data::FreeProgMem()		// 释放程序占用空间
{
	for (int i=0; i<Mod_Num; i++)
	{
		m_mod[i].FreeMem();
	}
}

void CProg_Data::ResetProgData()	// 新建程序或者切换程序时用到
{
	g_pFrm->m_wndEditBar.m_rawList.RemoveAll();
	g_pFrm->m_wndEditBar.UpdateCmdListBox(0);
}

int CProg_Data::LoadProgData()
{
	if ("" == g_AllProgInfo[m_nChannel].m_curProgName)
	{
		return 0;
	}

	CString strTemp;
	strTemp.Format("%sproduct\\%s\\Path.ctrl", CFunction::GetDirectory(), g_AllProgInfo[m_nChannel].m_curProgName);

	g_pFrm->m_wndEditBar.ReadCmdLine(strTemp.GetBuffer(strTemp.GetLength()));
	strTemp.ReleaseBuffer();

	for (int i=0; i<Mod_Num; i++)
	{
		m_mod[i].m_modId = i;
		m_mod[i].LoadMod();
	}

	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetCurProgName();
	return 1;
};

int CProg_Data::SaveProgData()
{
	if (g_pFrm == NULL)			return 0;
	if ("" == m_strProgName)	return 0;

	CString strTemp;
	strTemp.Format("%sproduct\\%s\\Path.ctrl", CFunction::GetDirectory(), g_AllProgInfo[0].m_curProgName);
	g_pFrm->m_wndEditBar.SaveCmdLine(strTemp);
	
	for (int i=0; i<Mod_Num; i++)
	{
		m_mod[i].SaveMod();
	}

	return 1;
};


void CTSCtrlSysDoc::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	for (int i=0; i<NUM_CHANNEL; i++)
	{
		g_AllProgInfo[i].HandleAllProg(FALSE);
	}
}

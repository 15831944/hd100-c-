// UserRegister.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "UserRegister.h"

// CUserRegister dialog

IMPLEMENT_DYNAMIC(CUserRegister, CDialog)

CUserRegister::CUserRegister(CWnd* pParent /*=NULL*/)
: CDialog(CUserRegister::IDD, pParent)
{
	strDeviceCode = _T("");
}

CUserRegister::~CUserRegister()
{
}

void CUserRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUserRegister, CDialog)
	ON_BN_CLICKED(ID_BT_REGESITSOFT, &CUserRegister::OnBnClickedBtRegesitsoft)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CUserRegister::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString Str;
	Str = theApp.m_EncryptOp.GetMacInfo(0);
	if(!Str.IsEmpty())
	{
		SetDlgItemText(IDC_EDIT_REGISTER_DEVICEID,Str);
	}
	strDeviceCode = Str; 

	int nID =-1;
	theApp.m_EncryptOp.GetRegisterCurID(nID);
	if(nID == -1)
	{
		SetDlgItemText(IDC_EDIT_LIFESPAN,_T(""));
		SetDlgItemText(IDC_EDIT_PASSWORDTYPE,_T(""));

	}else
	{
		// 获取对应注册码

		CString strcodeT;
		TCHAR szTchar[100]; 
	    theApp.m_EncryptOp.GetRegisterCode(nID,szTchar); 
		strcodeT.Format(_T("%s"),szTchar); 
        GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowText(strcodeT);
		//  当前注册码信息显示
		PASSWORDINFO info;
		theApp.m_EncryptOp.GetPassWordInfo(nID,&info);
		CString str;
		switch(info.nPassWordType)
		{
		case 0:
			str.Format(_T("正版注册码"));
			break;
		case 1:
			str.Format(_T("试用版一级注册码"));
			break;
		case 2:
			str.Format(_T("试用版二级注册码"));
			break;
		case 3:
			str.Format(_T("试用版三级注册码"));
			break;
		}
		SetDlgItemText(IDC_EDIT_PASSWORDTYPE,str);

		str.Format(_T("%d"),info.nLifeSpan);
		if(info.nLifeSpan == -1)
		{
			SetDlgItemText(IDC_EDIT_LIFESPAN,_T("持续有效"));
		}
		else
		{
			SetDlgItemText(IDC_EDIT_LIFESPAN,str);
		}
	}
	SetTimer(0,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
// 注册
void CUserRegister::OnBnClickedBtRegesitsoft()
{
	if(theApp.m_EncryptOp.GetRegisterFlag())
	{// 已注册为正版
		AfxMessageBox(_T("已注册为正版!"),MB_OK);
		return;
	}else
	{// 未注册为正版

		CString strCode;
		GetDlgItem(IDC_EDIT_REGISTER_CODE)->GetWindowText(strCode);
		if(_T("") == strCode)
		{
			AfxMessageBox(_T("请输入12位注册码"));
			return ;
		}
		//
		int nnum = strCode.GetLength();
		if(nnum != 12 )
		{
			AfxMessageBox(_T("输入的注册码位数不对请重新输入！"));
			GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowText(_T(""));
			return ;
		}
    
		int rtn = -1;
		int nID = -1;  // 注册码ID
		rtn = theApp.m_EncryptOp.CalcPassWord(strCode,strDeviceCode,nID);
		if(rtn == -1)
		{
			AfxMessageBox(_T("该注册码已使用"));
			GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowText(_T(""));
			return ;
		}
		if(rtn == 0)
		{
			AfxMessageBox(_T("注册成功"));

			theApp.m_EncryptOp.RegistInfo(strCode,nID);

			//  注册码信息显示
			CString str;
			//  注册时间信息
			PASSWORDINFO info;
			theApp.m_EncryptOp.GetPassWordInfo(nID,&info);
			switch(info.nPassWordType)
			{
			case 0:
				str.Format(_T("正版注册码"));
				break;
			case 1:
				str.Format(_T("试用版一级注册码"));
				break;
			case 2:
				str.Format(_T("试用版二级注册码"));
				break;
			case 3:
				str.Format(_T("试用版三级注册码"));
				break;
			}

			SetDlgItemText(IDC_EDIT_PASSWORDTYPE,str);
            
			if(info.nLifeSpan == -1)
			{
                str.Format(_T("持续有效"));
			}
			else
			{  
				str.Format(_T("%d"),info.nLifeSpan);
			}
               SetDlgItemText(IDC_EDIT_LIFESPAN,str);
		}
		else if(rtn == 1)
		{
			AfxMessageBox(_T("注册失败，请验证注册码是否输入正确！"));
			GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowText(_T(""));
			return ;
		}
	}
}

void CUserRegister::OnTimer(UINT_PTR nIDEvent)
{
	
	// 获取当前注册码信息
	int nID =-1;

	theApp.m_EncryptOp.GetRegisterCurID(nID);
    if(nID!=-1)
	{
		BOOL flag = theApp.m_EncryptOp.GetRegisterFlag();
		if(flag == TRUE)
		{
		    GetDlgItem(IDC_EDIT_LIFESPAN_NOUSERED)->SetWindowText(_T(""));
		}else
		{
			if(theApp.m_EncryptOp.m_bLockSystem==FALSE)
			{

			// 获取到期时间
			CTime LimitTime;
			theApp.m_EncryptOp.GetLimitTime(LimitTime);

			// 获取当前时间
			CTime CurTime = CTime::GetCurrentTime();

			// 计算剩余时间
			CTimeSpan timeDis= LimitTime - CurTime;
			CString strtemp;
			strtemp = timeDis.Format(_T("%D天 %H小时 %M分"));

			GetDlgItem(IDC_EDIT_LIFESPAN_NOUSERED)->SetWindowText(strtemp);
			}
			else{
              GetDlgItem(IDC_EDIT_LIFESPAN_NOUSERED)->SetWindowText(_T("软件已过期锁定！"));
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

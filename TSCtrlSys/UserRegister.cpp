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
		// ��ȡ��Ӧע����

		CString strcodeT;
		TCHAR szTchar[100]; 
	    theApp.m_EncryptOp.GetRegisterCode(nID,szTchar); 
		strcodeT.Format(_T("%s"),szTchar); 
        GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowText(strcodeT);
		//  ��ǰע������Ϣ��ʾ
		PASSWORDINFO info;
		theApp.m_EncryptOp.GetPassWordInfo(nID,&info);
		CString str;
		switch(info.nPassWordType)
		{
		case 0:
			str.Format(_T("����ע����"));
			break;
		case 1:
			str.Format(_T("���ð�һ��ע����"));
			break;
		case 2:
			str.Format(_T("���ð����ע����"));
			break;
		case 3:
			str.Format(_T("���ð�����ע����"));
			break;
		}
		SetDlgItemText(IDC_EDIT_PASSWORDTYPE,str);

		str.Format(_T("%d"),info.nLifeSpan);
		if(info.nLifeSpan == -1)
		{
			SetDlgItemText(IDC_EDIT_LIFESPAN,_T("������Ч"));
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
// ע��
void CUserRegister::OnBnClickedBtRegesitsoft()
{
	if(theApp.m_EncryptOp.GetRegisterFlag())
	{// ��ע��Ϊ����
		AfxMessageBox(_T("��ע��Ϊ����!"),MB_OK);
		return;
	}else
	{// δע��Ϊ����

		CString strCode;
		GetDlgItem(IDC_EDIT_REGISTER_CODE)->GetWindowText(strCode);
		if(_T("") == strCode)
		{
			AfxMessageBox(_T("������12λע����"));
			return ;
		}
		//
		int nnum = strCode.GetLength();
		if(nnum != 12 )
		{
			AfxMessageBox(_T("�����ע����λ���������������룡"));
			GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowText(_T(""));
			return ;
		}
    
		int rtn = -1;
		int nID = -1;  // ע����ID
		rtn = theApp.m_EncryptOp.CalcPassWord(strCode,strDeviceCode,nID);
		if(rtn == -1)
		{
			AfxMessageBox(_T("��ע������ʹ��"));
			GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowText(_T(""));
			return ;
		}
		if(rtn == 0)
		{
			AfxMessageBox(_T("ע��ɹ�"));

			theApp.m_EncryptOp.RegistInfo(strCode,nID);

			//  ע������Ϣ��ʾ
			CString str;
			//  ע��ʱ����Ϣ
			PASSWORDINFO info;
			theApp.m_EncryptOp.GetPassWordInfo(nID,&info);
			switch(info.nPassWordType)
			{
			case 0:
				str.Format(_T("����ע����"));
				break;
			case 1:
				str.Format(_T("���ð�һ��ע����"));
				break;
			case 2:
				str.Format(_T("���ð����ע����"));
				break;
			case 3:
				str.Format(_T("���ð�����ע����"));
				break;
			}

			SetDlgItemText(IDC_EDIT_PASSWORDTYPE,str);
            
			if(info.nLifeSpan == -1)
			{
                str.Format(_T("������Ч"));
			}
			else
			{  
				str.Format(_T("%d"),info.nLifeSpan);
			}
               SetDlgItemText(IDC_EDIT_LIFESPAN,str);
		}
		else if(rtn == 1)
		{
			AfxMessageBox(_T("ע��ʧ�ܣ�����֤ע�����Ƿ�������ȷ��"));
			GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowText(_T(""));
			return ;
		}
	}
}

void CUserRegister::OnTimer(UINT_PTR nIDEvent)
{
	
	// ��ȡ��ǰע������Ϣ
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

			// ��ȡ����ʱ��
			CTime LimitTime;
			theApp.m_EncryptOp.GetLimitTime(LimitTime);

			// ��ȡ��ǰʱ��
			CTime CurTime = CTime::GetCurrentTime();

			// ����ʣ��ʱ��
			CTimeSpan timeDis= LimitTime - CurTime;
			CString strtemp;
			strtemp = timeDis.Format(_T("%D�� %HСʱ %M��"));

			GetDlgItem(IDC_EDIT_LIFESPAN_NOUSERED)->SetWindowText(strtemp);
			}
			else{
              GetDlgItem(IDC_EDIT_LIFESPAN_NOUSERED)->SetWindowText(_T("����ѹ���������"));
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

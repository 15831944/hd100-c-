#include "StdAfx.h"

#define ENCRYPT_EXPORTS  
#include "../../../../Include/EncryptOp.h"

#define  DefRegeditInfoPass  "SOFTWARE\\TenSunCtrl\\License"   // ע���д��·��
#define  DefInStalled        "Installed" 

// ���ʱ��
#define  DefLastYear     "LastYear"
#define  DefLastMonth    "LastMonth"
#define  DefLastDay      "LastDay"
#define  DefLastHour     "LastHour"
#define  DefLastMinute   "LastMinute"
#define  DefLastSecond   "LastSecond"

// ���õ�����ʱ��
#define  DefLimitYear     "Year"
#define  DefLimitMonth    "Month"
#define  DefLimitDay      "Day"
#define  DefLimitHour     "Hour"
#define  DefLimitMinute   "Minute"
#define  DefLimitSecond   "Second"

#pragma comment(lib,"iphlpapi")
#include <SensAPI.h>      //IsNetworkAlive
#pragma comment(lib,"Sensapi")
#include <Wininet.h>    //InternetGetConnectedState
#pragma comment(lib,"Wininet")
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi")

CEncryptOp::CEncryptOp(void)
{
    m_bLockSystem = FALSE;
   	m_IsInfoReady=FALSE;

	m_nLevel_1_Delay = 60;
	m_nLevel_2_Delay = 30;
	m_nLevel_3_Delay = 15;

   InitPassWordInfoList();

}

CEncryptOp::~CEncryptOp(void)
{
   m_Adapters.clear();
}
// д�뵱ǰע������Ϣ
BOOL CEncryptOp::SetRegisterCurID(DWORD nID)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ע�����д�뵱ǰʱ������
	ret = RegSetValueEx(m_hKey, "CurPassWordID", NULL, REG_DWORD, (CONST BYTE*)&nID, sizeof(DWORD));
	// �ر�ע���
	RegCloseKey(m_hKey);
	return TRUE;
}
//
BOOL CEncryptOp::GetPWordLevelDelay(int &nID,int nlevel)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}

	// ��ע����ж���ע����Ϣ
	int dwRID;
	DWORD dwType;
	DWORD dwLen;

	CString str ;
	str.Format(_T("%d"),nlevel);
	str = _T("PWLevel")+str;
	ret = RegQueryValueEx(m_hKey, str, NULL, &dwType, (unsigned char*)&dwRID, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_hKey);
		return FALSE;
	}
	// �ر�ע���
	RegCloseKey(m_hKey);

	nID = dwRID;
	return TRUE;
}

// д��ע����Ȩ��ʱ��ȼ���Ϣ
BOOL CEncryptOp::SetPWordLevelDelay(DWORD nID,int nlevel)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	CString str ;
	str.Format(_T("%d"),nlevel);
	str = _T("PWLevel")+str;
	// ��ע�����д�뵱ǰʱ������
	ret = RegSetValueEx(m_hKey, str, NULL, REG_DWORD, (CONST BYTE*)&nID, sizeof(DWORD));
	// �ر�ע���
	RegCloseKey(m_hKey);
	return TRUE;
}
// ��ȡ��ǰע����ID
BOOL CEncryptOp::GetRegisterCurID(int &nID)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}

	// ��ע����ж���ע����Ϣ
	int dwRID;
	DWORD dwType;
	DWORD dwLen;

	ret = RegQueryValueEx(m_hKey, "CurPassWordID", NULL, &dwType, (unsigned char*)&dwRID, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_hKey);
		return FALSE;
	}
	// �ر�ע���
	RegCloseKey(m_hKey);
	
	nID = dwRID;
	return TRUE;
}

// ��ע����ж�������ֵ
BOOL CEncryptOp::IsInstalled()
{
	BOOL bInstalled = FALSE;
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass,0L,NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	// ��ע���ʧ��
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ע����ж�������ֵ
	DWORD dwVal,dwType,dwLen;
	ret = RegQueryValueEx(m_hKey, DefInStalled, NULL,
		&dwType, (unsigned char*)&dwVal,&dwLen);
	// ��ȡ����ʧ��
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ������������ֵ�Ƿ�Ϊ1
	bInstalled = (dwVal==1);
	// �ر�ע���
	RegCloseKey(m_hKey);
	return bInstalled;
}
// ��ע����ж���������������
BOOL CEncryptOp::GetLimitTime(CTime& t)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	// ��ע���ʧ��
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ע����ж���������������
	DWORD nYear,nMonth,nDay,nHour,nMinute,nSecond;
	DWORD dwType;
	DWORD dwLen;
	// ��ȡ���
	ret = RegQueryValueEx(m_hKey, DefLimitYear, NULL, &dwType, (unsigned char*)&nYear, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ȡ�·�
	ret = RegQueryValueEx(m_hKey, "Month", NULL, &dwType, (unsigned char*)&nMonth, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ȡ��
	ret = RegQueryValueEx(m_hKey, "Day", NULL, &dwType, (unsigned char*)&nDay, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ȡСʱ
	ret = RegQueryValueEx(m_hKey, "Hour", NULL, &dwType, (unsigned char*)&nHour, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ȡ����
	ret = RegQueryValueEx(m_hKey, "Minute", NULL, &dwType, (unsigned char*)&nMinute, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ȡ��
	ret = RegQueryValueEx(m_hKey, "Second", NULL, &dwType, (unsigned char*)&nSecond, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// �ر�ע���
	RegCloseKey(m_hKey);
	t = CTime(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	return TRUE;
}
// ������������
BOOL CEncryptOp::SetLimitTime(DWORD nYear,DWORD nMonth,DWORD nDay,DWORD nHour,DWORD nMinute,DWORD nSecond)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ע�����д��������������
	ret = RegSetValueEx(m_hKey, DefLimitYear, NULL, REG_DWORD, (CONST BYTE*)&nYear, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "Month", NULL, REG_DWORD, (CONST BYTE*)&nMonth, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "Day", NULL, REG_DWORD, (CONST BYTE*)&nDay, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "Hour", NULL, REG_DWORD, (CONST BYTE*)&nHour, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "Minute", NULL, REG_DWORD, (CONST BYTE*)&nMinute, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "Second", NULL, REG_DWORD, (CONST BYTE*)&nSecond, sizeof(DWORD));
	// д����������
	DWORD dwVal = 1;
	ret = RegSetValueEx(m_hKey, DefInStalled, NULL, REG_DWORD, (CONST BYTE*)&dwVal, sizeof(DWORD));
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// �ر�ע���
	RegCloseKey(m_hKey);
	return TRUE;
}
// ��ȡ��ǰʱ��
BOOL CEncryptOp::GetLastTime(CTime& t)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	// ��ע���ʧ��
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ע����ж���������������
	DWORD nYear,nMonth,nDay,nHour,nMinute,nSecond;
	DWORD dwType;
	DWORD dwLen;
	// ��ȡ���
	ret = RegQueryValueEx(m_hKey, "LastYear", NULL, &dwType, (unsigned char*)&nYear, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ȡ�·�
	ret = RegQueryValueEx(m_hKey, "LastMonth", NULL, &dwType, (unsigned char*)&nMonth, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ȡ��
	ret = RegQueryValueEx(m_hKey, "LastDay", NULL, &dwType, (unsigned char*)&nDay, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ȡСʱ
	ret = RegQueryValueEx(m_hKey, "LastHour", NULL, &dwType, (unsigned char*)&nHour, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ȡ����
	ret = RegQueryValueEx(m_hKey, "LastMinute", NULL, &dwType, (unsigned char*)&nMinute, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ȡ��
	ret = RegQueryValueEx(m_hKey, "LastSecond", NULL, &dwType, (unsigned char*)&nSecond, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// �ر�ע���
	RegCloseKey(m_hKey);
	t = CTime(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	return TRUE;
}
// ���õ�ǰʱ��
BOOL CEncryptOp::SetLastTime(DWORD nYear,DWORD nMonth,DWORD nDay,DWORD nHour,DWORD nMinute,DWORD nSecond)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ע�����д�뵱ǰʱ������
	ret = RegSetValueEx(m_hKey, "LastYear", NULL, REG_DWORD, (CONST BYTE*)&nYear, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "LastMonth", NULL, REG_DWORD, (CONST BYTE*)&nMonth, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "LastDay", NULL, REG_DWORD, (CONST BYTE*)&nDay, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "LastHour", NULL, REG_DWORD, (CONST BYTE*)&nHour, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "LastMinute", NULL, REG_DWORD, (CONST BYTE*)&nMinute, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "LastSecond", NULL, REG_DWORD, (CONST BYTE*)&nSecond, sizeof(DWORD));
	// �ر�ע���
	RegCloseKey(m_hKey);
	return TRUE;
}
// ��ȡע����Ϣ
BOOL CEncryptOp::GetRegisterFlag()
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ע����ж���ע����Ϣ
	DWORD dwRegister;
	DWORD dwType;
	DWORD dwLen;

	ret = RegQueryValueEx(m_hKey, "Register", NULL, &dwType, (unsigned char*)&dwRegister, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_hKey);
		return FALSE;
	}
	// �ر�ע���
	RegCloseKey(m_hKey);
	if(dwRegister==1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
// ����ע����Ϣ
BOOL CEncryptOp::SetRegisterFlag(BOOL bEnable)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_hKey);
		return FALSE;
	}
	// ��ע�����д��������������
	ret = RegSetValueEx(m_hKey, "Register", NULL, REG_DWORD, (CONST BYTE*)&bEnable, sizeof(DWORD));
	// �ر�ע���
	RegCloseKey(m_hKey);
	return TRUE;
}
// ��ȡע����
BOOL CEncryptOp::GetRegisterCode(int nNo,TCHAR* stemp)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ��ע����ж���ע����Ϣ
	DWORD dwType;
	DWORD dwLen;
	// 
	CString str;
	str.Format(_T("%d"),nNo);
	str = "RegisterCode" +str;
	ret = RegQueryValueEx(m_hKey, str, NULL, &dwType, (LPBYTE)stemp, &dwLen);
	//str.Format("%s",szChar);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_hKey);
		return FALSE;
	}
	// �ر�ע���
	RegCloseKey(m_hKey);
	//if(szChar!=NULL)
	//{
	//	delete[] szChar;
	//}
	return TRUE;
}
// ����ע����
BOOL CEncryptOp::SetRegisterCode(int nNo,TCHAR * stemp)
{
	DWORD dwDisposition;
	HKEY m_hKey;

	//TCHAR szTchar[100] = L"TCHAR";   
	CString  str;   
	str.Format(_T("%s"),stemp);  

	// ��ע���
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_hKey);
		return FALSE;
	}

	// ��ע�����д��ע����
	CString strtemp;
	strtemp.Format(_T("%d"),nNo);
	strtemp = "RegisterCode" +strtemp;

	ret = RegSetValueEx(m_hKey, strtemp, NULL, REG_SZ, (LPBYTE)(LPCTSTR)str, str.GetLength());
	
	// �ر�ע���
	RegCloseKey(m_hKey);
	return TRUE;
}
BOOL  CEncryptOp::CheckLMTTime(CTime CurTime,int &nDay)
{
	CTime LastTime;

	CTime LimitTime;
	GetLimitTime(LimitTime);
	 
	CTimeSpan span;
	CString   str;
   int day;
	if (CurTime < LimitTime )
	{  
        span =LimitTime- CurTime;
		str = span.Format(_T("%D"));
        day = atoi(str);
		if(day < Def_CheckLMTDayNum)
		{
			nDay = atoi(str);
			return FALSE;
		}
	}
  return TRUE;   
} 
BOOL  CEncryptOp::InitRegeditInfo(CTime CurTime)
{
	//û��ע�����Ϣ��д��
	CTime t = CTime::GetCurrentTime();
	SetLimitTime(t.GetYear(),t.GetMonth(),t.GetDay(),
		t.GetHour(),t.GetMinute(),t.GetSecond());

	SetLastTime(CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(),
		CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond());

	SetRegisterCode(0,DefSysPassWordA);
	SetRegisterFlag(FALSE);

	return TRUE;
}
BOOL  CEncryptOp::RegistInfo(CString strCode,int nID)
{
	// д��ע����
	TCHAR* szMsg = new TCHAR[100];
	CTime CurTime,TempTime;
	szMsg = strCode.GetBuffer(strCode.GetLength());
	SetRegisterCode(nID,szMsg);

	// д��ע����ID
	SetRegisterCurID(nID);

	//  ע��ʱ����Ϣ
	PASSWORDINFO info;
	GetPassWordInfo(nID,&info);
	int ndaleyNum=-1;
	if(info.nPassWordType == 0)
	{
		SetRegisterFlag(TRUE);
		// ����ע��    
	}else
	{  
		// ���ð�
		ndaleyNum = info.nLifeSpan;
		CurTime = CTime::GetCurrentTime();
		TempTime = CurTime + CTimeSpan(ndaleyNum,0,0,0);   

		SetLimitTime(TempTime.GetYear(),TempTime.GetMonth(),TempTime.GetDay(),
			TempTime.GetHour(),TempTime.GetMinute(),TempTime.GetSecond());

		SetLastTime(CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(),
			CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond());
	}
    return TRUE;
}
int  CEncryptOp::CalcPassWord(CString strCode,CString strDeviceCode,int &ID)
{
	// ����ע���� ����ע��ID��ע����Ϣ
	CString Str1,Str2,StrSub,StrTemp;

	int nID =-1;
	LONGLONG lMac = 0;
	LONGLONG lTemp =0;

	Str1 = strCode;         // ����ע����

	StrSub = Str1.Mid(2,2); // ��ȡID
	nID = atoi(StrSub);
    
	ID = nID;
	StrSub = Str1.Left(2);  // ��ȡ ע����
	StrTemp =Str1.Right(8);
	Str1 = StrSub + StrTemp;

	// �ж��Ƿ�Ϊ��ʹ�õ�ע����
	CString strcodeT;
	TCHAR szTchar[100]=_T("0000"); 
	for(int i=0;i<10;i++)
	{ 
		GetRegisterCode(i,szTchar); 
		strcodeT.Format(_T("%s"),szTchar); 
		if(strcodeT == strCode)
		{
			return  -1;
			/*AfxMessageBox(_T("��ע������ʹ��"));
			GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowText(_T(""));*/
		}
	}
	// ���ػ���������ת��
	StrTemp =strDeviceCode;
	while(StrTemp!=_T(""))
	{
		StrSub = StrTemp.Left(2);   // ȡ��λ����
		StrTemp.Delete(0,2);
		StrSub = _T("0x")+StrSub; 
		int o = 0; // ����Ϊ16������
		::StrToIntEx(StrSub.GetString(),STIF_SUPPORT_HEX,&o); // ���ַ���תΪ16��������
		lMac = lMac*256+o;
	}

	// ����ע����ID ��λ
	lMac = lMac<<(nID+1); 

	// ����תΪ�ַ�
	for(int i=0;i<6;i++)
	{
		lTemp = lMac%256;
		lMac = lMac/256;
		StrTemp.Format(_T("%02X"),lTemp);
		Str2 = StrTemp + Str2;
	}
	Str2 = Str2.Right(10);

	if(Str1 == Str2)
		return 0;
	else
		return 1;
}
// ��ȡ������Ϣ
CString CEncryptOp::GetMacInfo(int nID)
{
	CString Str;
	GetNetworkAdaptersInfo();
	UINT Nums=GetAdapterNums(); 
	if(nID<(int)Nums)
	{
		GetAdapterMac(nID,Str);
	}
	return Str;
}

BOOL CEncryptOp::GetNetworkAdaptersInfo()
{
	CString Str;
    WCHAR temp[100];
	PIP_ADAPTER_ADDRESSES pAddresses;
	ULONG outBufLen = 0;
	DWORD dwRetVal = 0;
    //int nLen =0;
	pAddresses = (IP_ADAPTER_ADDRESSES*) malloc(sizeof(IP_ADAPTER_ADDRESSES));
    USES_CONVERSION;
	if (GetAdaptersAddresses(AF_INET, 0, NULL, pAddresses, &outBufLen) 
		== ERROR_BUFFER_OVERFLOW) {
			free(pAddresses);
			pAddresses = (IP_ADAPTER_ADDRESSES*) malloc(outBufLen);
	}
   
	if ((dwRetVal = GetAdaptersAddresses(AF_INET, 
		0, 
		NULL, 
		pAddresses, 
		&outBufLen)) == NO_ERROR) {
			PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
			while (pCurrAddresses) 
			{

				if(pCurrAddresses->IfType== IF_TYPE_SOFTWARE_LOOPBACK )
				{
					pCurrAddresses = pCurrAddresses->Next;
					continue;
				}

				if(pCurrAddresses->Description!=NULL)
					wcscpy_s(temp,pCurrAddresses->Description);//�豸����

               // nLen = strlen(_T("Microsoft Loopback Adapter"));
                int p =  strcmp(W2A(temp),_T("Microsoft Loopback Adapter")/*,nLen*/);
				if(!p)
				{   // 
					pCurrAddresses = pCurrAddresses->Next;
					continue;
				}

				ADAPTERINFO info;
				memset(&info,0,sizeof(info));
				if(pCurrAddresses->FriendlyName!=NULL)
				wcscpy_s(info.FriendName,pCurrAddresses->FriendlyName);//����"��������1"����������

				if(pCurrAddresses->AdapterName!=NULL)
				strcpy_s(info.Name,pCurrAddresses->AdapterName);//�豸��

				if(pCurrAddresses->Description!=NULL)
					wcscpy_s(info.Description,pCurrAddresses->Description);//�豸����

				if(pCurrAddresses->DnsSuffix!=NULL)
				wcscpy_s(info.DomainName,pCurrAddresses->DnsSuffix);//����

				//ip
				if(pCurrAddresses->FirstUnicastAddress!=NULL)
				memcpy(info.Ip,&(pCurrAddresses->FirstUnicastAddress->Address.lpSockaddr->sa_data[2]),4);//*/

				//�����ַ
				if(pCurrAddresses->PhysicalAddress!=NULL)
				memcpy(info.Mac,pCurrAddresses->PhysicalAddress,8);

				if(pCurrAddresses->FirstDnsServerAddress!=NULL)
				{
					//��ѡDNS
					memcpy(info.FirDns,&(pCurrAddresses->FirstDnsServerAddress->Address.lpSockaddr->sa_data[2]),4);
					if(pCurrAddresses->FirstDnsServerAddress->Next!=NULL)
						//����DNS
						memcpy(info.SecDns,&(pCurrAddresses->FirstDnsServerAddress->Next->Address.lpSockaddr->sa_data[2]),4);

				}
				info.flag=pCurrAddresses->Flags;
				info.Type=pCurrAddresses->IfType;

				m_Adapters.push_back(info);
				m_IsInfoReady=TRUE;
				pCurrAddresses = pCurrAddresses->Next;
			}
	}
	
	free(pAddresses);
	return TRUE;
}
UINT CEncryptOp::GetAdapterNums()
{
	return m_Adapters.size();
}

BOOL CEncryptOp::GetAdapterMac(int Index,CString &Mac)
{
	Mac=TEXT("");
	int Size=m_Adapters.size();
	if(Size<=0 || Index>=Size || !m_IsInfoReady)
	{
		return FALSE;
	}
	Mac.Format(TEXT("%02X%02X%02X%02X%02X%02X"),m_Adapters[Index].Mac[0],m_Adapters[Index].Mac[1],m_Adapters[Index].Mac[2],
		m_Adapters[Index].Mac[3],m_Adapters[Index].Mac[4],m_Adapters[Index].Mac[5]);
	return TRUE;
}

BOOL CEncryptOp::GetPassWordInfo(int nId,PASSWORDINFO *info)
{
	if(nId<0||nId>9)
	{
		return FALSE;
	}
	info->nID = nId;
	info->nLifeSpan = m_PassWordInfo[nId].nLifeSpan;
	info->nPassWordType = m_PassWordInfo[nId].nPassWordType ; 
	return TRUE;
}
// ��ʼ��ע������Ϣ�б�
BOOL CEncryptOp::InitPassWordInfoList()
{
	for (int i=0;i<10;i++)
	{
		m_PassWordInfo[i].nID =i;

		switch(i)
		{
		case 0:
		case 1:
		case 2:
		case 3:  // һ��ע���� ������Ч��
			m_PassWordInfo[i].nLifeSpan = m_nLevel_1_Delay;
			m_PassWordInfo[i].nPassWordType =1;
			break;
		case 4:
		case 5:
		case 6:  // ����ע���� 20����Ч��
			m_PassWordInfo[i].nLifeSpan = m_nLevel_2_Delay;
			m_PassWordInfo[i].nPassWordType =2;
			break;
		case 7:
		case 8:  // ����ע���� 10����Ч��
			m_PassWordInfo[i].nLifeSpan = m_nLevel_3_Delay;
			m_PassWordInfo[i].nPassWordType =3;
			break;
		case 9:   // ����ע���� 
			m_PassWordInfo[i].nLifeSpan = -1;     
			m_PassWordInfo[i].nPassWordType =0;
			break;
		}
	}
	return TRUE;
}


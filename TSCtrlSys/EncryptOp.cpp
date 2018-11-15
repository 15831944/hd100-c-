#include "StdAfx.h"

#define ENCRYPT_EXPORTS  
#include "../../../../Include/EncryptOp.h"

#define  DefRegeditInfoPass  "SOFTWARE\\TenSunCtrl\\License"   // 注册表写入路径
#define  DefInStalled        "Installed" 

// 最近时间
#define  DefLastYear     "LastYear"
#define  DefLastMonth    "LastMonth"
#define  DefLastDay      "LastDay"
#define  DefLastHour     "LastHour"
#define  DefLastMinute   "LastMinute"
#define  DefLastSecond   "LastSecond"

// 设置的限制时间
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
// 写入当前注册码信息
BOOL CEncryptOp::SetRegisterCurID(DWORD nID)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 向注册表中写入当前时间数据
	ret = RegSetValueEx(m_hKey, "CurPassWordID", NULL, REG_DWORD, (CONST BYTE*)&nID, sizeof(DWORD));
	// 关闭注册表
	RegCloseKey(m_hKey);
	return TRUE;
}
//
BOOL CEncryptOp::GetPWordLevelDelay(int &nID,int nlevel)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}

	// 从注册表中读入注册信息
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
	// 关闭注册表
	RegCloseKey(m_hKey);

	nID = dwRID;
	return TRUE;
}

// 写入注册码权限时间等级信息
BOOL CEncryptOp::SetPWordLevelDelay(DWORD nID,int nlevel)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	CString str ;
	str.Format(_T("%d"),nlevel);
	str = _T("PWLevel")+str;
	// 向注册表中写入当前时间数据
	ret = RegSetValueEx(m_hKey, str, NULL, REG_DWORD, (CONST BYTE*)&nID, sizeof(DWORD));
	// 关闭注册表
	RegCloseKey(m_hKey);
	return TRUE;
}
// 读取当前注册码ID
BOOL CEncryptOp::GetRegisterCurID(int &nID)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}

	// 从注册表中读入注册信息
	int dwRID;
	DWORD dwType;
	DWORD dwLen;

	ret = RegQueryValueEx(m_hKey, "CurPassWordID", NULL, &dwType, (unsigned char*)&dwRID, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_hKey);
		return FALSE;
	}
	// 关闭注册表
	RegCloseKey(m_hKey);
	
	nID = dwRID;
	return TRUE;
}

// 从注册表中读入特征值
BOOL CEncryptOp::IsInstalled()
{
	BOOL bInstalled = FALSE;
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass,0L,NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	// 打开注册表失败
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 从注册表中读入特征值
	DWORD dwVal,dwType,dwLen;
	ret = RegQueryValueEx(m_hKey, DefInStalled, NULL,
		&dwType, (unsigned char*)&dwVal,&dwLen);
	// 读取数据失败
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 检测读到的特征值是否为1
	bInstalled = (dwVal==1);
	// 关闭注册表
	RegCloseKey(m_hKey);
	return bInstalled;
}
// 从注册表中读入试用期限数据
BOOL CEncryptOp::GetLimitTime(CTime& t)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	// 打开注册表失败
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 从注册表中读入试用期限数据
	DWORD nYear,nMonth,nDay,nHour,nMinute,nSecond;
	DWORD dwType;
	DWORD dwLen;
	// 读取年份
	ret = RegQueryValueEx(m_hKey, DefLimitYear, NULL, &dwType, (unsigned char*)&nYear, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 读取月份
	ret = RegQueryValueEx(m_hKey, "Month", NULL, &dwType, (unsigned char*)&nMonth, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 读取天
	ret = RegQueryValueEx(m_hKey, "Day", NULL, &dwType, (unsigned char*)&nDay, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 读取小时
	ret = RegQueryValueEx(m_hKey, "Hour", NULL, &dwType, (unsigned char*)&nHour, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 读取分钟
	ret = RegQueryValueEx(m_hKey, "Minute", NULL, &dwType, (unsigned char*)&nMinute, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 读取秒
	ret = RegQueryValueEx(m_hKey, "Second", NULL, &dwType, (unsigned char*)&nSecond, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 关闭注册表
	RegCloseKey(m_hKey);
	t = CTime(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	return TRUE;
}
// 设置期限数据
BOOL CEncryptOp::SetLimitTime(DWORD nYear,DWORD nMonth,DWORD nDay,DWORD nHour,DWORD nMinute,DWORD nSecond)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 向注册表中写入试用期限数据
	ret = RegSetValueEx(m_hKey, DefLimitYear, NULL, REG_DWORD, (CONST BYTE*)&nYear, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "Month", NULL, REG_DWORD, (CONST BYTE*)&nMonth, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "Day", NULL, REG_DWORD, (CONST BYTE*)&nDay, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "Hour", NULL, REG_DWORD, (CONST BYTE*)&nHour, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "Minute", NULL, REG_DWORD, (CONST BYTE*)&nMinute, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "Second", NULL, REG_DWORD, (CONST BYTE*)&nSecond, sizeof(DWORD));
	// 写入特征数据
	DWORD dwVal = 1;
	ret = RegSetValueEx(m_hKey, DefInStalled, NULL, REG_DWORD, (CONST BYTE*)&dwVal, sizeof(DWORD));
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 关闭注册表
	RegCloseKey(m_hKey);
	return TRUE;
}
// 获取当前时间
BOOL CEncryptOp::GetLastTime(CTime& t)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	// 打开注册表失败
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 从注册表中读入试用期限数据
	DWORD nYear,nMonth,nDay,nHour,nMinute,nSecond;
	DWORD dwType;
	DWORD dwLen;
	// 读取年份
	ret = RegQueryValueEx(m_hKey, "LastYear", NULL, &dwType, (unsigned char*)&nYear, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 读取月份
	ret = RegQueryValueEx(m_hKey, "LastMonth", NULL, &dwType, (unsigned char*)&nMonth, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 读取天
	ret = RegQueryValueEx(m_hKey, "LastDay", NULL, &dwType, (unsigned char*)&nDay, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 读取小时
	ret = RegQueryValueEx(m_hKey, "LastHour", NULL, &dwType, (unsigned char*)&nHour, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 读取分钟
	ret = RegQueryValueEx(m_hKey, "LastMinute", NULL, &dwType, (unsigned char*)&nMinute, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 读取秒
	ret = RegQueryValueEx(m_hKey, "LastSecond", NULL, &dwType, (unsigned char*)&nSecond, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 关闭注册表
	RegCloseKey(m_hKey);
	t = CTime(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	return TRUE;
}
// 设置当前时间
BOOL CEncryptOp::SetLastTime(DWORD nYear,DWORD nMonth,DWORD nDay,DWORD nHour,DWORD nMinute,DWORD nSecond)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 向注册表中写入当前时间数据
	ret = RegSetValueEx(m_hKey, "LastYear", NULL, REG_DWORD, (CONST BYTE*)&nYear, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "LastMonth", NULL, REG_DWORD, (CONST BYTE*)&nMonth, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "LastDay", NULL, REG_DWORD, (CONST BYTE*)&nDay, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "LastHour", NULL, REG_DWORD, (CONST BYTE*)&nHour, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "LastMinute", NULL, REG_DWORD, (CONST BYTE*)&nMinute, sizeof(DWORD));
	ret = RegSetValueEx(m_hKey, "LastSecond", NULL, REG_DWORD, (CONST BYTE*)&nSecond, sizeof(DWORD));
	// 关闭注册表
	RegCloseKey(m_hKey);
	return TRUE;
}
// 读取注册信息
BOOL CEncryptOp::GetRegisterFlag()
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 从注册表中读入注册信息
	DWORD dwRegister;
	DWORD dwType;
	DWORD dwLen;

	ret = RegQueryValueEx(m_hKey, "Register", NULL, &dwType, (unsigned char*)&dwRegister, &dwLen);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_hKey);
		return FALSE;
	}
	// 关闭注册表
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
// 设置注册信息
BOOL CEncryptOp::SetRegisterFlag(BOOL bEnable)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_hKey);
		return FALSE;
	}
	// 向注册表中写入试用期限数据
	ret = RegSetValueEx(m_hKey, "Register", NULL, REG_DWORD, (CONST BYTE*)&bEnable, sizeof(DWORD));
	// 关闭注册表
	RegCloseKey(m_hKey);
	return TRUE;
}
// 读取注册码
BOOL CEncryptOp::GetRegisterCode(int nNo,TCHAR* stemp)
{
	DWORD dwDisposition;
	HKEY m_hKey;
	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// 从注册表中读入注册信息
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
	// 关闭注册表
	RegCloseKey(m_hKey);
	//if(szChar!=NULL)
	//{
	//	delete[] szChar;
	//}
	return TRUE;
}
// 设置注册码
BOOL CEncryptOp::SetRegisterCode(int nNo,TCHAR * stemp)
{
	DWORD dwDisposition;
	HKEY m_hKey;

	//TCHAR szTchar[100] = L"TCHAR";   
	CString  str;   
	str.Format(_T("%s"),stemp);  

	// 打开注册表
	LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER, DefRegeditInfoPass, 0L, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisposition);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_hKey);
		return FALSE;
	}

	// 向注册表中写入注册码
	CString strtemp;
	strtemp.Format(_T("%d"),nNo);
	strtemp = "RegisterCode" +strtemp;

	ret = RegSetValueEx(m_hKey, strtemp, NULL, REG_SZ, (LPBYTE)(LPCTSTR)str, str.GetLength());
	
	// 关闭注册表
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
	//没有注册表信息，写入
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
	// 写入注册码
	TCHAR* szMsg = new TCHAR[100];
	CTime CurTime,TempTime;
	szMsg = strCode.GetBuffer(strCode.GetLength());
	SetRegisterCode(nID,szMsg);

	// 写入注册码ID
	SetRegisterCurID(nID);

	//  注册时间信息
	PASSWORDINFO info;
	GetPassWordInfo(nID,&info);
	int ndaleyNum=-1;
	if(info.nPassWordType == 0)
	{
		SetRegisterFlag(TRUE);
		// 正版注册    
	}else
	{  
		// 试用版
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
	// 分析注册码 分离注册ID和注册信息
	CString Str1,Str2,StrSub,StrTemp;

	int nID =-1;
	LONGLONG lMac = 0;
	LONGLONG lTemp =0;

	Str1 = strCode;         // 复制注册码

	StrSub = Str1.Mid(2,2); // 获取ID
	nID = atoi(StrSub);
    
	ID = nID;
	StrSub = Str1.Left(2);  // 获取 注册码
	StrTemp =Str1.Right(8);
	Str1 = StrSub + StrTemp;

	// 判断是否为已使用的注册码
	CString strcodeT;
	TCHAR szTchar[100]=_T("0000"); 
	for(int i=0;i<10;i++)
	{ 
		GetRegisterCode(i,szTchar); 
		strcodeT.Format(_T("%s"),szTchar); 
		if(strcodeT == strCode)
		{
			return  -1;
			/*AfxMessageBox(_T("该注册码已使用"));
			GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowText(_T(""));*/
		}
	}
	// 本地机器码数据转换
	StrTemp =strDeviceCode;
	while(StrTemp!=_T(""))
	{
		StrSub = StrTemp.Left(2);   // 取两位数据
		StrTemp.Delete(0,2);
		StrSub = _T("0x")+StrSub; 
		int o = 0; // 保存为16进制数
		::StrToIntEx(StrSub.GetString(),STIF_SUPPORT_HEX,&o); // 将字符串转为16进制数据
		lMac = lMac*256+o;
	}

	// 根据注册码ID 移位
	lMac = lMac<<(nID+1); 

	// 新码转为字符
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
// 获取网卡信息
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
					wcscpy_s(temp,pCurrAddresses->Description);//设备描述

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
				wcscpy_s(info.FriendName,pCurrAddresses->FriendlyName);//类似"本地连接1"这样的名字

				if(pCurrAddresses->AdapterName!=NULL)
				strcpy_s(info.Name,pCurrAddresses->AdapterName);//设备名

				if(pCurrAddresses->Description!=NULL)
					wcscpy_s(info.Description,pCurrAddresses->Description);//设备描述

				if(pCurrAddresses->DnsSuffix!=NULL)
				wcscpy_s(info.DomainName,pCurrAddresses->DnsSuffix);//域名

				//ip
				if(pCurrAddresses->FirstUnicastAddress!=NULL)
				memcpy(info.Ip,&(pCurrAddresses->FirstUnicastAddress->Address.lpSockaddr->sa_data[2]),4);//*/

				//物理地址
				if(pCurrAddresses->PhysicalAddress!=NULL)
				memcpy(info.Mac,pCurrAddresses->PhysicalAddress,8);

				if(pCurrAddresses->FirstDnsServerAddress!=NULL)
				{
					//首选DNS
					memcpy(info.FirDns,&(pCurrAddresses->FirstDnsServerAddress->Address.lpSockaddr->sa_data[2]),4);
					if(pCurrAddresses->FirstDnsServerAddress->Next!=NULL)
						//备用DNS
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
// 初始化注册码信息列表
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
		case 3:  // 一级注册码 试用有效期
			m_PassWordInfo[i].nLifeSpan = m_nLevel_1_Delay;
			m_PassWordInfo[i].nPassWordType =1;
			break;
		case 4:
		case 5:
		case 6:  // 二级注册码 20天有效期
			m_PassWordInfo[i].nLifeSpan = m_nLevel_2_Delay;
			m_PassWordInfo[i].nPassWordType =2;
			break;
		case 7:
		case 8:  // 三级注册码 10天有效期
			m_PassWordInfo[i].nLifeSpan = m_nLevel_3_Delay;
			m_PassWordInfo[i].nPassWordType =3;
			break;
		case 9:   // 正版注册码 
			m_PassWordInfo[i].nLifeSpan = -1;     
			m_PassWordInfo[i].nPassWordType =0;
			break;
		}
	}
	return TRUE;
}


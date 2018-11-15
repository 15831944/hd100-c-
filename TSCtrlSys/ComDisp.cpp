#include "StdAfx.h"
#include "TSCtrlSys.h"
#include "ComDisp.h"
#include "StringParser.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CRC16 检验算法源码
static unsigned char  auchCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
static unsigned char  auchCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

static unsigned int Crc16(unsigned char* chFrame, int chLen)	// chFrame:发送的数据帧		chLen:数据长度
{
	/* int result;
	BYTE high,low;*/
	unsigned char ucCRCHi = 0xFF;
	unsigned char ucCRCLo = 0xFF; 
	int iIndex = 0x0000;
	while (chLen--)
	{
		iIndex = ucCRCLo ^ *chFrame++;
		ucCRCLo = ucCRCHi ^ auchCRCHi[iIndex];
		ucCRCHi = auchCRCLo[iIndex];
	}

	// return (ucCRCHi << 8 | ucCRCLo);
	return (ucCRCLo << 8 | ucCRCHi);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// 武藏阀使用，int转换为16进制的二位数，再转换为ASCII，  ch 至少包含2个字符
int Int2Asc16Bit(int Val, char* ch)
{
	if (NULL == ch)		return -1;
			
	int a = Val & 0xF;
	int b = Val>>4 & 0xF;
	char chr[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	ch[0] = chr[b];
	ch[1] = chr[a];

	return 1;
}

CComDisp::CComDisp()
{
	m_eDispType = Disp_Vermes;
	m_pComPort = NULL;

	m_cycTimeMs = 5;
	m_freq = 100;
}


CComDisp::~CComDisp(void)
{
	m_pComPort = NULL;
}

// 从文件加载胶阀参数或保存参数到文件		bLoad : true= 加载   false=保存
int CComDisp::HandleParamFile(BOOL bLoad)
{
	CString strFile = CFunction::GetDirectory() + "system\\DispenserParam.dat";
	CString strTemp, strKey;
//	int i=0;

	switch (m_eDispType)
	{
	case Disp_Vermes:
		strKey.Format("Vermes阀:%d", m_nIndex);
		break;
	case Disp_Kaifa:
		strKey.Format("Kaifa阀:%d", m_nIndex);
		break;
	case Disp_Musashi:
		strKey.Format("Musashi阀:%d", m_nIndex);
		break;
	case Disp_Yikai:
		strKey.Format("Yikai阀:%d", m_nIndex);
		break;
	default:
		strTemp.Format("存档或保存胶阀参数时，输入错误的阀类型：%d", m_eDispType);
		AfxMessageBox(strTemp);
		return 0;
	}

	CFunction::HandleInt(bLoad, strKey, "Rising",	  m_dispParam.Rising,	  strFile);
	CFunction::HandleInt(bLoad, strKey, "OpenTime",	  m_dispParam.OpenTime,	  strFile);
	CFunction::HandleInt(bLoad, strKey, "Falling",	  m_dispParam.Falling,	  strFile);
	CFunction::HandleInt(bLoad, strKey, "NeedleLift", m_dispParam.NeedleLift, strFile);
	CFunction::HandleInt(bLoad, strKey, "NumOfPul",	  m_dispParam.NumOfPul,	  strFile);
	CFunction::HandleInt(bLoad, strKey, "Delay",	  m_dispParam.Delay,	  strFile);
	CFunction::HandleInt(bLoad, strKey, "Temp",		  m_dispParam.Temp,		  strFile);
	CFunction::HandleInt(bLoad, strKey, "Presure",	  m_dispParam.Presure,	  strFile);

	// 当前使用的胶阀类型	m_eValveType
	strFile = CFunction::GetDirectory() + "system\\robotparam.dat";
	CFunction::HandleInt(bLoad, "CurDispType", "DispType", g_pFrm->m_pRobotParam->m_nValveType, strFile);
	CFunction::HandleInt(bLoad, "CurDispCtrl", "DispCtrl", g_pFrm->m_pRobotParam->m_nValveCtrlMode, strFile);
	return 1;
}

// 从胶阀读取参数或设置参数到点胶阀			bLoad : true= 读取   false=设置
int CComDisp::HandleParamDispenser(BOOL bLoad)
{
	if (NULL == m_pComPort)
	{
		return 0;
	}

	CString strGet = "";
	CString strSend = "";
	CString strTemp = "";
	char szBuf[128] = {0};

	DWORD lgStarttime = GetTickCount();

	//超过此时间就跳出循环
	DWORD dwDelaytime = 2000;

	m_pComPort->PurgeWritebuf();
	m_pComPort->PurgeReadbuf();

	int nActWriteLength = 0;	// 实际发送成功数据长度
	int nActRecLength = 0;

	if (bLoad)	// 从点胶阀读取参数
	{
		switch (m_eDispType)
		{
		case Disp_Yikai:
			{
				unsigned char chBuf[32];
				chBuf[0] = 0x01;
				chBuf[1] = 0x03;
				chBuf[2] = 0x00;
				chBuf[3] = 0x11;
				chBuf[4] = 0x00;
				chBuf[5] = 0x09;
				unsigned int crc  = Crc16(chBuf, 6);
				memcpy(szBuf, chBuf, 6);
 				szBuf[7] = BYTE(crc & 0xff);
 				szBuf[6] = BYTE((crc>>8) & 0xff);
				nActWriteLength = m_pComPort->WriteComm(szBuf, 8);
				if (nActWriteLength <= 0)
				{
					strTemp.Format("调用接口WriteComm()读取胶阀参数,返回长度:[%d]失败!", nActWriteLength);
					AfxMessageBox(strTemp);
					return 0;
				}

				CFunction::DelaySec(0.05);
				// 等待ACK
				while ((GetTickCount()-lgStarttime) <= dwDelaytime)
				{
					nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
					if (nActRecLength > 0)
					{
						m_dispParam.OpenTime = (UCHAR)szBuf[3]<<24 | (UCHAR)szBuf[4]<<16 | (UCHAR)szBuf[5]<<8 | (UCHAR)szBuf[6];
						m_dispParam.Delay	 = (UCHAR)szBuf[7]<<24 | (UCHAR)szBuf[8]<<16 | (UCHAR)szBuf[9]<<8 | (UCHAR)szBuf[10];
						m_dispParam.Rising	 = (UCHAR)szBuf[11]<<8 | (UCHAR)szBuf[12];
						m_dispParam.Falling  = (UCHAR)szBuf[13]<<8 | (UCHAR)szBuf[14];
						m_dispParam.NumOfPul = (UCHAR)szBuf[15]<<24 | (UCHAR)szBuf[16]<<16 | (UCHAR)szBuf[17]<<8 | (UCHAR)szBuf[18];
						m_dispParam.NeedleLift = (UCHAR)szBuf[19]<<24 | (UCHAR)szBuf[20];
						if (0x01 == szBuf[0] &&
							0x03 == szBuf[1] &&
							0x12 == szBuf[2])	break;
						else	
						{
							AfxMessageBox("等待Yikai阀返回ACK出错!");
							return 0;
						}
					}
				}
				m_cycTimeMs = m_dispParam.Rising + m_dispParam.Falling + m_dispParam.OpenTime + m_dispParam.Delay;
				m_cycTimeMs = m_cycTimeMs /100;

				if (m_cycTimeMs < 0.001)
				{
					m_cycTimeMs = 0.001;
				}
				m_freq = 1000 / m_cycTimeMs;
			}
			break;

		case Disp_Musashi:
			{
				char ch = 0x05;
				// 发送武藏阀ENQ
				nActWriteLength = m_pComPort->WriteComm(&ch, 1);
				if (nActWriteLength <= 0)
				{
					strTemp.Format("调用接口WriteComm()读取胶阀参数,返回长度:[%d]失败!", nActWriteLength);
					AfxMessageBox(strTemp);
					return 0;
				}
				CFunction::DelaySec(0.05);
				// 等待ACK
				while ((GetTickCount()-lgStarttime) <= dwDelaytime)
				{
					nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
					if (nActRecLength > 0)
					{
						if (0x06 == szBuf[0])	break;
						else	
						{
							AfxMessageBox("等待武藏阀返回ACK出错!");
							return 0;
						}
					}
				}
				// 发送上载包
				m_pComPort->PurgeReadbuf();
				char chSend[] = "a08UL000,01DAa";
				chSend[0] = 0x02;
				chSend[13] = 0x03;
				nActWriteLength = m_pComPort->WriteComm(chSend, 14);
				if (nActWriteLength <= 0)
				{
					strTemp.Format("调用接口WriteComm()读取胶阀参数,返回长度:[%d]失败!", nActWriteLength);
					AfxMessageBox(strTemp);
					return 0;
				}
				lgStarttime = GetTickCount();
				DWORD readLen;
				while ((GetTickCount()-lgStarttime) <= dwDelaytime)
				{				
					BOOL bRtn = m_pComPort->ReadComByEndChar(szBuf, 255, 0x03, &readLen, 2000);
					if (bRtn && (readLen==66))	break;
					else	
					{
						AfxMessageBox("通过串口读取武藏阀参数出错!");
						return 0;
					}
				}

				ch = 0x04;
				// 发送武藏阀查询结束
				nActWriteLength = m_pComPort->WriteComm(&ch, 1);
				if (nActWriteLength <= 0)
				{
					strTemp.Format("武藏阀查询结束出错,返回长度:[%d]失败!", nActWriteLength);
					AfxMessageBox(strTemp);
					return 0;
				}

				CString strTemp, strVal;
				strTemp.Format("%s", szBuf);
				strVal = strTemp.Mid(5, 3);
				m_dispParam.Musashi_Channel = atoi(strVal);
				strVal = strTemp.Mid(9, 1);
				m_dispParam.Musashi_Main = atoi(strVal);
				strVal = strTemp.Mid(11, 1);
				m_dispParam.Musashi_Sub = atoi(strVal);
				strVal = strTemp.Mid(13, 6);
				m_dispParam.Rising = atoi(strVal);
				strVal = strTemp.Mid(20, 6);
				m_dispParam.Falling = atoi(strVal);
				strVal = strTemp.Mid(27, 6);
				m_dispParam.Musashi_DotOpen = atoi(strVal);
				strVal = strTemp.Mid(34, 8);
				m_dispParam.NumOfPul = atoi(strVal);
				strVal = strTemp.Mid(43, 3);
				m_dispParam.NeedleLift = atoi(strVal);
				strVal = strTemp.Mid(47, 3);
				m_dispParam.Musashi_Up = atoi(strVal);
				strVal = strTemp.Mid(51, 3);
				m_dispParam.Musashi_Down = atoi(strVal);
				strVal = strTemp.Mid(55, 4);
				m_dispParam.Temp = atoi(strVal);
				strVal = strTemp.Mid(60, 3);
				m_dispParam.Presure = atoi(strVal);

				m_cycTimeMs = m_dispParam.Rising + m_dispParam.Falling;
				m_cycTimeMs = m_cycTimeMs /100;

				if (m_cycTimeMs < 0.001)
				{
					m_cycTimeMs = 0.001;
				}
				m_freq = 1000 / m_cycTimeMs;
				//AfxMessageBox("读取成功！");
				break;
			}

		case Disp_Vermes:
			strSend = "TRIGGER:SET:?\n\r";

			nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
			if (nActWriteLength <= 0)
			{
				strTemp.Format("调用接口WriteComm()读取胶阀参数,返回长度:[%d]失败!", nActWriteLength);
				AfxMessageBox(strTemp);
				return 0;
			}
			CFunction::DelaySec(0.035);

			while ((GetTickCount()-lgStarttime) <= dwDelaytime)
			{
				nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
				if (nActRecLength > 0)
				{
					std::string tmpStr(szBuf);
					std::vector<u32> intVec;
					StringParser::GetParamFromString(tmpStr, intVec);

					if (intVec.size() != 6)
					{
						strTemp.Format("读取到的Vermes参数位数:%d不对!不为6", intVec.size());
						AfxMessageBox(strTemp);
					}
					else
					{
						m_dispParam.Rising = intVec.at(0);
						m_dispParam.OpenTime = intVec.at(1);	
						m_dispParam.Falling = intVec.at(2);		
						m_dispParam.NeedleLift = intVec.at(3);	
						m_dispParam.NumOfPul = intVec.at(4);	
						m_dispParam.Delay = intVec.at(5);		
						
						m_cycTimeMs = m_dispParam.Rising*10 + m_dispParam.Falling + m_dispParam.OpenTime*10 + m_dispParam.Delay*10;
						m_cycTimeMs = m_cycTimeMs /100;

						if (m_cycTimeMs < 0.001)
						{
							m_cycTimeMs = 0.001;
						}
						m_freq = 1000 / m_cycTimeMs;
// 						m_dispParam.Temp = intVec.at(0);		
// 						m_dispParam.Presure = intVec.at(0);
					}
				}
			}
			break;

		case Disp_Kaifa:
			strSend = "READ\n\r";
			lgStarttime = GetTickCount();
			nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
			if (nActWriteLength <= 0)
			{
				strTemp.Format("调用接口WriteComm()读取胶阀参数,返回长度:[%d]失败!", nActWriteLength);
				AfxMessageBox(strTemp);
				return 0;
			}
			Sleep(50);

			while ((GetTickCount()-lgStarttime) <= dwDelaytime)
			{
				nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
				if (nActRecLength > 0)
				{
					CString strTemp, strTemp1;
					strTemp.Format("%s", szBuf);
					strTemp1 = strTemp.Left(3);
					m_dispParam.Rising = atoi(strTemp1);
					strTemp1 = strTemp.Mid(3, 3);
					m_dispParam.Falling = atoi(strTemp1);
					strTemp1 = strTemp.Mid(6, 3);
					m_dispParam.NumOfPul = atoi(strTemp1);

					m_cycTimeMs = m_dispParam.Rising + m_dispParam.Falling;
					m_cycTimeMs = m_cycTimeMs /10;

					if (m_cycTimeMs < 0.001)
					{
						m_cycTimeMs = 0.001;
					}
					m_freq = 1000 / m_cycTimeMs;
					break;
				}
			}

			m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
			break;

		default:
			strTemp.Format("从点胶阀加载参数时出错,错误的点胶阀类型:[%d]!", m_eDispType);
			AfxMessageBox(strTemp);
			return 0;
		}
	}
	else	// 将参数设置到点胶阀
	{
		switch (m_eDispType)
		{
		case Disp_Yikai:
			{
				unsigned char chBuf[32];
				chBuf[0] = 0x01;	    
				chBuf[1] = 0x10;
				chBuf[2] = 0x00;
				chBuf[3] = 0x11;
				chBuf[4] = 0x00;
				chBuf[5] = 0x09;
				chBuf[6] = 0x12;
				//////////////////////////////////////////////////////////////////////////
				chBuf[10] = BYTE(m_dispParam.OpenTime  );
				chBuf[9] = BYTE(m_dispParam.OpenTime>>8  );
				chBuf[8] = BYTE(m_dispParam.OpenTime>>16  );
				chBuf[7] = BYTE(m_dispParam.OpenTime>>24  );

				chBuf[14] = m_dispParam.Delay & 0xff;
				chBuf[13] = m_dispParam.Delay>>8 & 0xff;
				chBuf[12] = m_dispParam.Delay>>16 & 0xff;
				chBuf[11] = m_dispParam.Delay>>24 & 0xff;

				chBuf[16] = m_dispParam.Rising & 0xff;
				chBuf[15] = m_dispParam.Rising>>8 & 0xff;
				
				chBuf[18] = m_dispParam.Falling & 0xff;
				chBuf[17] = m_dispParam.Falling>>8 & 0xff;

				chBuf[22] = m_dispParam.NumOfPul & 0xff;
				chBuf[21] = m_dispParam.NumOfPul>>8 & 0xff;
				chBuf[20] = m_dispParam.NumOfPul>>16 & 0xff;
				chBuf[19] = m_dispParam.NumOfPul>>24 & 0xff;

				chBuf[24] = m_dispParam.NeedleLift & 0xff;
				chBuf[23] = m_dispParam.NeedleLift>>8 & 0xff;

				unsigned int crc  = Crc16(chBuf, 25);
				memcpy(szBuf, chBuf, 25);
				szBuf[26] = BYTE(crc & 0xff);
				szBuf[25] = BYTE((crc>>8) & 0xff);
				nActWriteLength = m_pComPort->WriteComm(szBuf, 27);
				if (nActWriteLength <= 0)
				{
					strTemp.Format("调用接口WriteComm()读取胶阀参数,返回长度:[%d]失败!", nActWriteLength);
					AfxMessageBox(strTemp);
					return 0;
				}

				m_cycTimeMs = m_dispParam.Rising + m_dispParam.Falling + m_dispParam.OpenTime + m_dispParam.Delay;
				m_cycTimeMs = m_cycTimeMs /100;

				if (m_cycTimeMs < 0.001)
				{
					m_cycTimeMs = 0.001;
				}
				m_freq = 1000 / m_cycTimeMs;

				return 1;
			}
			break;
		case Disp_Musashi:
			{
				char ch = 0x05;
				// 发送武藏阀ENQ
				nActWriteLength = m_pComPort->WriteComm(&ch, 1);
				if (nActWriteLength <= 0)
				{
					strTemp.Format("调用接口WriteComm()读取胶阀参数,返回长度:[%d]失败!", nActWriteLength);
					AfxMessageBox(strTemp);
					return 0;
				}
				CFunction::DelaySec(0.05);
				// 等待ACK
				while ((GetTickCount()-lgStarttime) <= dwDelaytime)
				{
					nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
					if (nActRecLength > 0)
					{
						if (0x06 == szBuf[0])	break;
						else	
						{
							AfxMessageBox("等待武藏阀返回ACK出错!");
							return 0;
						}
					}
				}
				// 发送下载包
				m_pComPort->PurgeReadbuf();
				strSend.Format("3CDL%03d,%d,%d,%06d,%06d,%06d,%08d,%03d,%03d,%03d,%04d,%03d",					
					m_dispParam.Musashi_Channel,
					m_dispParam.Musashi_Main,
					m_dispParam.Musashi_Sub,
					m_dispParam.Rising,
					m_dispParam.Falling,
					m_dispParam.Musashi_DotOpen,
					m_dispParam.NumOfPul,
					m_dispParam.NeedleLift,
					m_dispParam.Musashi_Up,
					m_dispParam.Musashi_Down,
					m_dispParam.Temp,
					m_dispParam.Presure
					);
				char chSend[66];
				chSend[0] = 0x02;
				memcpy(&chSend[1], strSend.GetBuffer(strSend.GetLength()), strSend.GetLength());
				int val = 0;
				for (int i=1; i<63; i++)
				{
					val -= chSend[i];
				}

				Int2Asc16Bit(val, &chSend[63]);
				chSend[65] = 0x03;

				nActWriteLength = m_pComPort->WriteComm(chSend, 66);
				if (nActWriteLength <= 0)
				{
					AfxMessageBox("写入武藏阀参数时出错!");
					return 0;
				}
				lgStarttime = GetTickCount();
				DWORD readLen;
				while ((GetTickCount()-lgStarttime) <= dwDelaytime)
				{				
					BOOL bRtn = m_pComPort->ReadComByEndChar(szBuf, 255, 0x03, &readLen, 2000);
					if (bRtn)	break;
					else	
					{
						AfxMessageBox("通过串口读取武藏阀参数出错!");
						return 0;
					}
				}

				ch = 0x04;
				// 发送武藏阀查询结束
				nActWriteLength = m_pComPort->WriteComm(&ch, 1);
				if (nActWriteLength <= 0)
				{
					strTemp.Format("武藏阀查询结束出错,返回长度:[%d]失败!", nActWriteLength);
					AfxMessageBox(strTemp);
					return 0;
				}

				CString strTemp;
				strTemp.Format("%s", szBuf);
				if ("A0" != strTemp.Mid(3,2))
				{
					AfxMessageBox("设置武藏点胶阀参数出错!");
					return 0;
				}
				return 1;
			}
			break;
		case Disp_Vermes:
			strSend.Format(
				"TRIGGER:SET:%d,%d,%d,%d,%d,%d\n\r",
				m_dispParam.Rising,
				m_dispParam.OpenTime,
				m_dispParam.Falling,
				m_dispParam.NeedleLift,
				m_dispParam.NumOfPul,
				m_dispParam.Delay);

			nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
			if (nActWriteLength <= 0)
			{
				return 0;
			}

			//////////////////////////////////////////////////////////////////////////
			while ((GetTickCount()-lgStarttime) <= dwDelaytime)
			{
				nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
				if (nActRecLength > 0)
				{
					CString strTemp;
					strTemp.Format("%s", szBuf);
					if ("OK" == strTemp.Left(2))	break;
					else 
					{
							AfxMessageBox("写入Vermes参数出错");
							return 0;
					}

					CFunction::DelaySec(0.01);
				}
			}
			//////////////////////////////////////////////////////////////////////////

			m_cycTimeMs = m_dispParam.Rising*10 + m_dispParam.Falling + m_dispParam.OpenTime*10 + m_dispParam.Delay*10;
			m_cycTimeMs = m_cycTimeMs /100;

			if (m_cycTimeMs < 0.001)
			{
				m_cycTimeMs = 0.001;
			}
			m_freq = 1000 / m_cycTimeMs;
			break;

		case Disp_Kaifa:
			strSend.Format(
				"SETVONT%03dVOFT%03dSNUM%03d\n\r",
				m_dispParam.Rising,
				m_dispParam.Falling,
				m_dispParam.NumOfPul
				);
			nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
			if (nActWriteLength <= 0)
			{
				return 0;
			}

			//////////////////////////////////////////////////////////////////////////
			while ((GetTickCount()-lgStarttime) <= dwDelaytime)
			{
				nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
				if (nActRecLength > 0)
				{
					CString strTemp;
 					strTemp.Format("%s", szBuf);
					if ("OK" == strTemp.Left(2))
					{
						break;
					}
					else 
					{
						AfxMessageBox("写入Kaifa参数出错");
						return 0;
					}

					CFunction::DelaySec(0.01);
				}
			}
			//////////////////////////////////////////////////////////////////////////

			m_cycTimeMs = m_dispParam.Rising + m_dispParam.Falling;
			m_cycTimeMs = m_cycTimeMs /10;

			if (m_cycTimeMs < 0.001)
			{
				m_cycTimeMs = 0.001;
			}
			m_freq = 1000 / m_cycTimeMs;
			break;

		default:
			strTemp.Format("设置点胶阀参数时出错,错误的点胶阀类型:[%d]!", m_eDispType);
			AfxMessageBox(strTemp);
			break;
		}
	}

	strTemp.Format("点胶阀周期：[%.3f], 点胶阀频率：[%.3f]", m_cycTimeMs, m_freq);
	g_pFrm->AddMsg(strTemp);
	return 1;
}


void CComDisp::SetDispenserType(DispenserType eType)
{
	m_eDispType = eType;
}


int CComDisp::SetComPort(SerialPort* pPort)
{
	if (NULL == pPort)
	{
		AfxMessageBox("配置点胶阀串口时出错，指针为空!");
		return 0;
	}

	CString str = pPort->m_szPortName;
	TRACE("配置点胶阀为:%s\n\r", str);

	m_pComPort = pPort;
	return 1;
}

// 配置胶阀
int CComDisp::AttachDispenser(int idx, DispenserType dispType, SerialPort* pPort)
{
	if ((idx > 1) || (NULL==pPort))
	{
		AfxMessageBox("配置点胶阀串口时出错");
		return 0;
	}

	m_nIndex = idx;
	m_eDispType = dispType;

	CString str = pPort->m_szPortName;
	TRACE("配置点胶阀为:%s\n\r", str);

	m_pComPort = pPort;
	return 1;
}

// 获取周期时间
int CComDisp::GetCycleTimeAndFreq(double &cycTimeMs, double &freq)
{
// 	switch (m_eDispType)
// 	{
// 	case Disp_Vermes:
// 		cycTimeMs = m_dispParam.Falling + m_dispParam.Rising*10 + m_dispParam.OpenTime*10 + m_dispParam.Delay*10;
// 		cycTimeMs = cycTimeMs / 100;
// 		if (cycTimeMs < 0.01)
// 		{
// 			return 0;
// 		}
// 		freq = 1000 / cycTimeMs;
// 		break;
// 	case Disp_Kaifa:
// 		cycTimeMs = m_dispParam.Falling + m_dispParam.Rising;
// 		cycTimeMs = cycTimeMs / 10;
// 		if (cycTimeMs < 0.01)
// 		{
// 			return 0;
// 		}
// 		freq = 1000 / cycTimeMs;
// 		break;
// 	default:
// 		AfxMessageBox("获取阀的周期和频率时，不支持的阀类型!");
// 		return 0;
// 	}
	cycTimeMs = m_cycTimeMs;
	freq = m_freq;

	return 1;
}

// pdp.pComparePos[0]
// pdp.pComparePos[1]
// pdp.pComparePos[2]
// pdp.pComparePos[3]
// pdp.pComparePos[4]
// pdp.pComparePos[5]
// pdp.pComparePos[6]
// pdp.pComparePos[7]
// pdp.pComparePos[8]
// pdp.pComparePos[9]
// pdp.pComparePos[10]
// pdp.pComparePos[11]
// pdp.pComparePos[12]
// pdp.pComparePos[13]
// pdp.pComparePos[14]
// pdp.pComparePos[15]
// pdp.pComparePos[16]
// pdp.pComparePos[17]
// pdp.pComparePos[18]
// pdp.pComparePos[19]
// pdp.pComparePos[20]
// pdp.pComparePos[21]
// pdp.pComparePos[22]
// pdp.pComparePos[23]
// pdp.pComparePos[24]
// pdp.pComparePos[25]
// pdp.pComparePos[26]
// pdp.pComparePos[27]
// pdp.pComparePos[28]
// pdp.pComparePos[29]
// pdp.pComparePos[30]
// pdp.pComparePos[31]
// pdp.pComparePos[32]
// pdp.pComparePos[33]
// pdp.pComparePos[34]
// pdp.pComparePos[35]
// pdp.pComparePos[36]
// pdp.pComparePos[37]
// pdp.pComparePos[38]
// pdp.pComparePos[39]
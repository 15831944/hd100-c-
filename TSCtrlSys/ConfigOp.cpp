/***********************************************************
* ��Ȩ���� (C)2013-2014,TenSun Industrial Equipment Co., Ltd.
* 
* �ļ����ƣ�SerialPort.h
* ��ǰ�汾��V1.0
* ��   �ߣ� eddie_cheung     
* ������ڣ�2013��11��11��
* �ļ������� �����ļ�������
* ����˵�������ini���������ļ����ݵĶ�д������֧��ASCII/Unicode 
*           ģʽ��ini�����ļ�,��Ҫ���ļ�����ƥ�䡣
*
* �޸ļ�¼��
*    <����>:              <�޸���>:          <�޸�����>:
*   2013.11.11            eddie_cheung       Creat
*******************************************************/

#include "StdAfx.h"

#include "ConfigOp.h"

CString m_strFileInfo;  // �����ļ�����
/*************************************************
* �������ƣ�CConfigOp
* ����˵���� ���캯��
* ���������int nMode - �ĵ�ģʽ  0 unicode  1 Ansic
* ���������
* �� �� ֵ��
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
CConfigOp::CConfigOp()
{
	m_nMode = MODE_ANSI;
	m_strFilePath = _T("");
}
/*************************************************
* �������ƣ�CConfigOp
* ����˵���� ���캯��
* ��������� CString strFileName --ini�ļ���
*            CString strPath     -- �ļ�·��
*            int nMode - �ĵ�ģʽ  0 unicode  1 Ansic
* ���������
* �� �� ֵ��
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
CConfigOp::CConfigOp(CString strFileName,int nMode)
{
	m_nMode = nMode;
	m_strFilePath = strFileName; //strFileName Ϊȫ·��  

	if(_T("") != m_strFilePath)
	{
		GetFile(m_strFilePath);
	}
}

CConfigOp::~CConfigOp(void)
{
}
/*************************************************
* �������ƣ�GetFile
* ����˵����  ��ȡ�ļ����� �������б�
* ���������CString strIniPath   -- ȫ�ļ���
* ���������
* �� �� ֵ��BOOL  True / False 
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
BOOL CConfigOp::GetFile(CString strIniPath)
{
	m_strFilePath =strIniPath;
	m_strFileInfo =_T("");
	CFile    IniFile;
	TRY
	{
		// ���ļ�
		if(! IniFile.Open(m_strFilePath, CFile::modeRead))
		{
			return FALSE;
		}
		// �ж��ļ��Ƿ�Ϊ��
		if(IniFile.GetLength() == 0)
		{
			IniFile.Close();
			return FALSE;
		}

		// ----------�����ļ��������ݵ�������---------------
		if(MODE_UNICODE == m_nMode)
		{
			// �����ļ��������ݵ�������
			WCHAR  *pBuf;
			pBuf = new WCHAR[(unsigned int)(IniFile.GetLength() / 2 + 1)];  
			if(pBuf == NULL)
			{
				IniFile.Close();
				return FALSE;
			}
			if(IniFile.Read(pBuf, (UINT)(IniFile.GetLength())) != IniFile.GetLength())
			{
				delete[]  pBuf;
				IniFile.Close();
				return FALSE;
			}
			pBuf[IniFile.GetLength() / 2] = NULL; 
	        m_strFileInfo=pBuf;
			delete[]   pBuf;
		}
		else
		{
			char  *pBuf;
			pBuf = new char[(unsigned int)(IniFile.GetLength()+1)];  
			if(pBuf == NULL)
			{
				IniFile.Close();
				return FALSE;
			}
			if(IniFile.Read(pBuf,(UINT)(IniFile.GetLength())) != IniFile.GetLength())
			{
				delete[]  pBuf;
				IniFile.Close();
				return FALSE;
			}
			pBuf[IniFile.GetLength()] = NULL;
			m_strFileInfo = pBuf;
			delete[]   pBuf;
		}
	}
	CATCH(CFileException, e)
	{		
	}
	END_CATCH      //�������Ժ�
		IniFile.Close();  
	return TRUE;
}

/*************************************************
* �������ƣ�SaveFile
* ����˵���� �������ݵ��ļ�
* ���������
* ���������
* �� �� ֵ��BOOL  True / False 
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
BOOL CConfigOp::SaveDateToDisk()
{	
	if(_T("")==m_strFileInfo)
	{
		return FALSE;
	}

	CString strOneRow=_T("\r\n");
	if(strOneRow != m_strFileInfo.Right(strOneRow.GetLength()))
	{
		m_strFileInfo += strOneRow;
	}

	CFile    IniFile;
	TRY
	{
		//���ļ�
		if(! IniFile.Open(m_strFilePath, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return FALSE;
		}

		IniFile.SetLength(0);  
		IniFile.SeekToBegin();

		if(MODE_UNICODE == m_nMode)
		{
			LPTSTR  lpCombine = m_strFileInfo.GetBuffer(0);
			IniFile.Write(lpCombine, m_strFileInfo.GetLength() * 2);
		}
		else
		{
			char *pWriteBuf;
			pWriteBuf=new char[m_strFileInfo.GetLength()+1];
			for(int i=0;i<m_strFileInfo.GetLength();i++)
			{
				pWriteBuf[i]=(char)(m_strFileInfo.GetAt(i));
			}
			IniFile.Write(pWriteBuf, m_strFileInfo.GetLength());
			delete [] pWriteBuf;
		}

		IniFile.Close();

	} // end TRY
	CATCH(CFileException, e)
	{
	   ;
	}
	END_CATCH     ///�������Ժ�

		return TRUE;
}
/*************************************************
* �������ƣ�WriteProfileString
* ����˵���� �������ݵ������б�
* ���������CString strSection  ������
*           CString strEntry    ����
*           CString strValue  Ĭ��ֵ
* ���������
* �� �� ֵ��BOOL  Ture / False 
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
BOOL  CConfigOp::WriteProfileString(const CString strSection, const CString strEntry, const CString strValue)
{
	if(MODE_UNICODE == m_nMode)
	{
		// ��ʼ���ң���section��entry�Ƿ��Ѿ�����
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = m_strFileInfo.Find(_T("[") + strSection + _T("]\r\n"));
		if(iIndex1 == -1)  //û�ҵ�
		{
			m_strFileInfo += _T("[") + strSection + _T("]") + _T("\r\n") 
				+ strEntry + _T("=") + strValue + _T("\r\n");

			return TRUE;
		}
		iIndexT = iIndex1 + 4 + strSection.GetLength();
		iIndex2 = m_strFileInfo.Find(strEntry + _T("="), iIndexT);
		if(iIndex2 == -1)  //û�ҵ�
		{
			m_strFileInfo.Insert(iIndexT, strEntry + _T("=") + strValue + _T("\r\n"));

			return TRUE;
		}
		else
		{
			iIndex3 = m_strFileInfo.Find(_T("\r\n"), iIndex2 + 1);
			if(iIndex3 == -1)
			{
				return FALSE;
			}
			iIndexT = iIndex2 + 1 + strEntry.GetLength();
			m_strFileInfo.Delete(iIndexT, iIndex3 - iIndexT);
			m_strFileInfo.Insert(iIndexT, strValue);

			return TRUE;
		}
	}
	else
	{
		// ���ж����в����Ƿ�Ϊ��
		if(strSection == _T("") || strEntry ==_T("")|| strValue == _T(""))
		{
			return FALSE;
		}

		// ��ʼ���ң���section��entry�Ƿ��Ѿ�����
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = m_strFileInfo.Find(_T("[") + strSection + _T("]"));
		if(iIndex1 == -1)  //û�ҵ�
		{
			m_strFileInfo += _T("[") + strSection + _T("]") + _T("\r\n") 
				+ strEntry + _T("=") + strValue + _T("\r\n");

			return TRUE;
		}

		iIndexT = iIndex1 + 2 + strSection.GetLength();
		iIndex2 = m_strFileInfo.Find(_T("\r\n")+strEntry + _T("="), iIndexT);

		if(iIndex2 == -1)  //û�ҵ�
		{
			m_strFileInfo.Insert(iIndexT+2, strEntry + _T("=") + strValue + _T("\r\n"));

			return TRUE;
		}
		else
		{
			iIndex3 = m_strFileInfo.Find(_T("\r\n"), iIndex2 + 2);
			if(iIndex3 == -1)
			{
				return FALSE;
			}
			iIndexT = iIndex2 + 3 + strEntry.GetLength();
			m_strFileInfo.Delete(iIndexT, iIndex3 - iIndexT);
			m_strFileInfo.Insert(iIndexT, strValue);

			return TRUE;
		}	
	}

	return FALSE;
}

/*************************************************
* �������ƣ�GetProfileString
* ����˵���� ��ini�ж�һ���ַ������ݣ�
*            �Ƚ�ȫ�������ݶ���buff�ٲ���
* ���������CString strSection  ������
*           CString strEntry    ����
*           CString strValue  Ĭ��ֵ
* ���������
* �� �� ֵ��CString  ��ȡ�����ַ���
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
CString CConfigOp::GetProfileString(const CString strSection, const CString strEntry, const CString strDefault)
{	
	if(MODE_UNICODE == m_nMode)
	{
		// ��ʼ���ң���section��entry�Ƿ��Ѿ�����
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = m_strFileInfo.Find(_T("[" )+ strSection + _T("]\r\n"));
		if(iIndex1 == -1)  //û�ҵ�
		{
			return strDefault;
		}
		iIndexT = iIndex1 + 4 + strSection.GetLength();
		iIndex2 = m_strFileInfo.Find(strEntry + _T("="), iIndexT);
		if(iIndex2 == -1)  //û�ҵ�
		{
			return strDefault;
		}
		else
		{
			iIndex3 = m_strFileInfo.Find(_T("\r\n"), iIndex2 + 1);
			if(iIndex3 == -1)
			{
				return strDefault;
			}
			iIndexT = iIndex2 + 1 + strEntry.GetLength();

			return  m_strFileInfo.Mid(iIndexT, iIndex3 - iIndexT);
		}
	}
	else
	{
		// ���ж�ǰ���������Ƿ�Ϊ��
		if(strSection == _T("") || strEntry == _T(""))
		{
			return strDefault;   //���ɹ��򷵻�Ĭ��ֵ
		}

		// ��ʼ���ң���section��entry�Ƿ��Ѿ�����
		int iIndex1, iIndex2, iIndex3, iIndexT;   
		iIndex1 = m_strFileInfo.Find(_T("[") + strSection + _T("]"));
		if(iIndex1 == -1)  //û�ҵ�
		{
			return strDefault;
		}
		iIndexT = iIndex1 + 2 + strSection.GetLength();
		iIndex2 = m_strFileInfo.Find(_T("\r\n")+strEntry + _T("="), iIndexT);

		if(iIndex2 == -1)  //û�ҵ�
		{
			return strDefault;
		}
		else
		{
			iIndex3 = m_strFileInfo.Find(_T("\r\n"), iIndex2 + 2);
			if(iIndex3 == -1)
			{
				return strDefault;
			}
			iIndexT = iIndex2 + 3 + strEntry.GetLength();

			return  m_strFileInfo.Mid(iIndexT, iIndex3 - iIndexT);
		}
	}
}

/*************************************************
* �������ƣ�WriteIntegerToIni
* ����˵������ini��дһ��int����
* ���������CString str  ������
*           CString key  ����
*           int     val  д�������ֵ
* ���������
* �� �� ֵ��BOOL    T /F
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
BOOL CConfigOp::WriteIntegerToIni(const CString str,const CString key,const int val)
{
	CString temp;
	temp.Format(_T("%d"),val);
	BOOL rtn =  WriteProfileString(str,key,temp);

	return rtn;

}

/*************************************************
* �������ƣ�WriteFloatToIni
* ����˵������ini��дһ��float����
* ���������CString str  ������
*           CString key  ����
*           double  val  д�������ֵ
* ���������
* �� �� ֵ��BOOL    T /F
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
BOOL CConfigOp::WriteFloatToIni(const CString str,const CString key,const double val)
{
	CString temp;
	temp.Format(_T("%.8f"),val);
	BOOL rtn = WriteProfileString(str,key,temp);
	return rtn;

}

/*************************************************
* �������ƣ�ReadFloatfromIni
* ����˵������ini�ж�ȡһ��float����
* ���������CString str  ������
*           CString key  ����
* ���������
* �� �� ֵ��double   ��ȡ��double��ֵ
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
double CConfigOp::ReadFloatFromIni(const CString str,const CString key)
{
	CString temp;
	double rtn;
	temp=GetProfileString(str,key,_T(""));
	rtn = _tcstod(temp,NULL);
	return rtn;
}

/*************************************************
* �������ƣ�ReadIntegerFromIni
* ����˵������ini�ж�ȡһ��int����
* ���������CString str  ������
*           CString key  ����
* ���������
* �� �� ֵ��int   ��ȡ��int��ֵ
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
int CConfigOp::ReadIntegerFromIni(const CString str,const CString key)
{
	CString temp;
	int rtn;
	temp=GetProfileString(str,key,_T(""));
	rtn=_ttoi(temp);
	return rtn;
}

/*************************************************
* �������ƣ�ReadStringfromIni
* ����˵������ini�ж�ȡһ���ַ���
* ���������CString str  ������
*           CString key  ����
* ���������
* �� �� ֵ��CString  ��ȡ���ַ�����ֵ
* ����˵����
* �汾��ʷ��
* �޸�����       �汾��    �޸���	      �޸�����
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
CString CConfigOp::ReadStringFromIni(const CString str,const CString key)
{
	CString temp;
	temp=GetProfileString(str,key,_T(""));
	return temp;
}

BOOL CConfigOp::WriteStringToIni(const CString str,const CString key,const CString val)
{
	BOOL rtn =  WriteProfileString(str,key,val);
	return rtn;
}


/***********************************************************
* 版权所有 (C)2013-2014,TenSun Industrial Equipment Co., Ltd.
* 
* 文件名称：SerialPort.h
* 当前版本：V1.0
* 作   者： eddie_cheung     
* 完成日期：2013年11月11日
* 文件描述： 配置文件操作类
* 其它说明：完成ini类型配置文件内容的读写操作，支持ASCII/Unicode 
*           模式的ini配置文件,需要与文件类型匹配。
*
* 修改记录：
*    <日期>:              <修改人>:          <修改内容>:
*   2013.11.11            eddie_cheung       Creat
*******************************************************/

#include "StdAfx.h"

#include "ConfigOp.h"

CString m_strFileInfo;  // 配置文件内容
/*************************************************
* 函数名称：CConfigOp
* 函数说明： 构造函数
* 输入参数：int nMode - 文档模式  0 unicode  1 Ansic
* 输出参数：
* 返 回 值：
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
CConfigOp::CConfigOp()
{
	m_nMode = MODE_ANSI;
	m_strFilePath = _T("");
}
/*************************************************
* 函数名称：CConfigOp
* 函数说明： 构造函数
* 输入参数： CString strFileName --ini文件名
*            CString strPath     -- 文件路径
*            int nMode - 文档模式  0 unicode  1 Ansic
* 输出参数：
* 返 回 值：
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
CConfigOp::CConfigOp(CString strFileName,int nMode)
{
	m_nMode = nMode;
	m_strFilePath = strFileName; //strFileName 为全路径  

	if(_T("") != m_strFilePath)
	{
		GetFile(m_strFilePath);
	}
}

CConfigOp::~CConfigOp(void)
{
}
/*************************************************
* 函数名称：GetFile
* 函数说明：  获取文件数据 到数据列表
* 输入参数：CString strIniPath   -- 全文件名
* 输出参数：
* 返 回 值：BOOL  True / False 
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
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
		// 打开文件
		if(! IniFile.Open(m_strFilePath, CFile::modeRead))
		{
			return FALSE;
		}
		// 判断文件是否为空
		if(IniFile.GetLength() == 0)
		{
			IniFile.Close();
			return FALSE;
		}

		// ----------读出文件所有数据到缓冲区---------------
		if(MODE_UNICODE == m_nMode)
		{
			// 读出文件所有数据到缓冲区
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
	END_CATCH      //结束调试宏
		IniFile.Close();  
	return TRUE;
}

/*************************************************
* 函数名称：SaveFile
* 函数说明： 保存数据到文件
* 输入参数：
* 输出参数：
* 返 回 值：BOOL  True / False 
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
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
		//打开文件
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
	END_CATCH     ///结束调试宏

		return TRUE;
}
/*************************************************
* 函数名称：WriteProfileString
* 函数说明： 保存数据到数据列表
* 输入参数：CString strSection  数据名
*           CString strEntry    段名
*           CString strValue  默认值
* 输出参数：
* 返 回 值：BOOL  Ture / False 
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
BOOL  CConfigOp::WriteProfileString(const CString strSection, const CString strEntry, const CString strValue)
{
	if(MODE_UNICODE == m_nMode)
	{
		// 开始查找，看section和entry是否已经存在
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = m_strFileInfo.Find(_T("[") + strSection + _T("]\r\n"));
		if(iIndex1 == -1)  //没找到
		{
			m_strFileInfo += _T("[") + strSection + _T("]") + _T("\r\n") 
				+ strEntry + _T("=") + strValue + _T("\r\n");

			return TRUE;
		}
		iIndexT = iIndex1 + 4 + strSection.GetLength();
		iIndex2 = m_strFileInfo.Find(strEntry + _T("="), iIndexT);
		if(iIndex2 == -1)  //没找到
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
		// 先判断所有参数是否为空
		if(strSection == _T("") || strEntry ==_T("")|| strValue == _T(""))
		{
			return FALSE;
		}

		// 开始查找，看section和entry是否已经存在
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = m_strFileInfo.Find(_T("[") + strSection + _T("]"));
		if(iIndex1 == -1)  //没找到
		{
			m_strFileInfo += _T("[") + strSection + _T("]") + _T("\r\n") 
				+ strEntry + _T("=") + strValue + _T("\r\n");

			return TRUE;
		}

		iIndexT = iIndex1 + 2 + strSection.GetLength();
		iIndex2 = m_strFileInfo.Find(_T("\r\n")+strEntry + _T("="), iIndexT);

		if(iIndex2 == -1)  //没找到
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
* 函数名称：GetProfileString
* 函数说明： 从ini中读一个字符串数据，
*            先将全部的数据读入buff再查找
* 输入参数：CString strSection  数据名
*           CString strEntry    段名
*           CString strValue  默认值
* 输出参数：
* 返 回 值：CString  读取到的字符串
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
* -----------------------------------------------
*   2013.11.11            eddie_cheung       create
*************************************************/
CString CConfigOp::GetProfileString(const CString strSection, const CString strEntry, const CString strDefault)
{	
	if(MODE_UNICODE == m_nMode)
	{
		// 开始查找，看section和entry是否已经存在
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = m_strFileInfo.Find(_T("[" )+ strSection + _T("]\r\n"));
		if(iIndex1 == -1)  //没找到
		{
			return strDefault;
		}
		iIndexT = iIndex1 + 4 + strSection.GetLength();
		iIndex2 = m_strFileInfo.Find(strEntry + _T("="), iIndexT);
		if(iIndex2 == -1)  //没找到
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
		// 先判断前两个参数是否为空
		if(strSection == _T("") || strEntry == _T(""))
		{
			return strDefault;   //不成功则返回默认值
		}

		// 开始查找，看section和entry是否已经存在
		int iIndex1, iIndex2, iIndex3, iIndexT;   
		iIndex1 = m_strFileInfo.Find(_T("[") + strSection + _T("]"));
		if(iIndex1 == -1)  //没找到
		{
			return strDefault;
		}
		iIndexT = iIndex1 + 2 + strSection.GetLength();
		iIndex2 = m_strFileInfo.Find(_T("\r\n")+strEntry + _T("="), iIndexT);

		if(iIndex2 == -1)  //没找到
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
* 函数名称：WriteIntegerToIni
* 函数说明：向ini中写一个int数据
* 输入参数：CString str  数据名
*           CString key  段名
*           int     val  写入的数据值
* 输出参数：
* 返 回 值：BOOL    T /F
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
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
* 函数名称：WriteFloatToIni
* 函数说明：向ini中写一个float数据
* 输入参数：CString str  数据名
*           CString key  段名
*           double  val  写入的数据值
* 输出参数：
* 返 回 值：BOOL    T /F
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
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
* 函数名称：ReadFloatfromIni
* 函数说明：从ini中读取一个float数据
* 输入参数：CString str  数据名
*           CString key  段名
* 输出参数：
* 返 回 值：double   读取的double数值
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
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
* 函数名称：ReadIntegerFromIni
* 函数说明：从ini中读取一个int数据
* 输入参数：CString str  数据名
*           CString key  段名
* 输出参数：
* 返 回 值：int   读取的int数值
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
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
* 函数名称：ReadStringfromIni
* 函数说明：从ini中读取一个字符串
* 输入参数：CString str  数据名
*           CString key  段名
* 输出参数：
* 返 回 值：CString  读取的字符串数值
* 其它说明：
* 版本历史：
* 修改日期       版本号    修改人	      修改内容
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


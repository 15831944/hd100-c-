// TSCtrlSysDoc.h : CTSCtrlSysDoc 类的接口
//
#pragma once
#include "MilMod.h"

//////////////////////////////////////////////////////////////////////////
//					记载所有产品列表的类								//
//////////////////////////////////////////////////////////////////////////
class CProg_List
{
public:
	CProg_List();
	~CProg_List();

	int m_nChannel;		// 轨道,	0:轨道1       1:轨道2
	void SetChannel(int iChannel);

	int m_nProgNum;
	CString m_curProgName;

	CList <CString, CString&> m_listAllProgName;
	CList <CString, CString&> m_listProgCreateTime;	
	CList <CString, CString&> m_listProgDiscription;	
	void HandleAllProg(const BOOL bIfRead, const int unUsedParam=0);		// 参数   bIfRead  TRUE:从文件加载数据    FALSE:保存数据到文件	
	void Add(CString progName, CString progDiscription);
	void Delete(const CString progName);
	void Select(const CString progName);	// 选择程序
};



//////////////////////////////////////////////////////////////////////////
//					记载当前产品信息的类								//
//////////////////////////////////////////////////////////////////////////
const int Mod_Num = 5;
class CProg_Data	
{
public:
	CProg_Data();
	~CProg_Data();
	void SetCurProg(CString strProg);

	void FreeProgMem();		// 释放程序占用空间
	void ResetProgData();	// 新建程序或者切换程序时用到

	int LoadProgData();
	int SaveProgData();

	int m_nChannel;		// 轨道,	0:轨道1       1:轨道2
	CString m_strProgName;
	CMilMod m_mod[Mod_Num];
};

class CTSCtrlSysDoc : public CDocument
{
protected: // 仅从序列化创建
	CTSCtrlSysDoc();
	DECLARE_DYNCREATE(CTSCtrlSysDoc)

// 属性
public:

// 操作
public:
	CString GetModeFile(int iIndex,int nGroupID=1);      //0:Mark1, 1: Mark2
	CString GetBoardModeFile(int iIndex);// 基板模板路径 
	CString GetModeFileEx(int iIndex);//ccd pixel
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CTSCtrlSysDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpenDxf();

	afx_msg void OnFileSave();
};



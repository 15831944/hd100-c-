// TSCtrlSysDoc.h : CTSCtrlSysDoc ��Ľӿ�
//
#pragma once
#include "MilMod.h"

//////////////////////////////////////////////////////////////////////////
//					�������в�Ʒ�б����								//
//////////////////////////////////////////////////////////////////////////
class CProg_List
{
public:
	CProg_List();
	~CProg_List();

	int m_nChannel;		// ���,	0:���1       1:���2
	void SetChannel(int iChannel);

	int m_nProgNum;
	CString m_curProgName;

	CList <CString, CString&> m_listAllProgName;
	CList <CString, CString&> m_listProgCreateTime;	
	CList <CString, CString&> m_listProgDiscription;	
	void HandleAllProg(const BOOL bIfRead, const int unUsedParam=0);		// ����   bIfRead  TRUE:���ļ���������    FALSE:�������ݵ��ļ�	
	void Add(CString progName, CString progDiscription);
	void Delete(const CString progName);
	void Select(const CString progName);	// ѡ�����
};



//////////////////////////////////////////////////////////////////////////
//					���ص�ǰ��Ʒ��Ϣ����								//
//////////////////////////////////////////////////////////////////////////
const int Mod_Num = 5;
class CProg_Data	
{
public:
	CProg_Data();
	~CProg_Data();
	void SetCurProg(CString strProg);

	void FreeProgMem();		// �ͷų���ռ�ÿռ�
	void ResetProgData();	// �½���������л�����ʱ�õ�

	int LoadProgData();
	int SaveProgData();

	int m_nChannel;		// ���,	0:���1       1:���2
	CString m_strProgName;
	CMilMod m_mod[Mod_Num];
};

class CTSCtrlSysDoc : public CDocument
{
protected: // �������л�����
	CTSCtrlSysDoc();
	DECLARE_DYNCREATE(CTSCtrlSysDoc)

// ����
public:

// ����
public:
	CString GetModeFile(int iIndex,int nGroupID=1);      //0:Mark1, 1: Mark2
	CString GetBoardModeFile(int iIndex);// ����ģ��·�� 
	CString GetModeFileEx(int iIndex);//ccd pixel
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CTSCtrlSysDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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



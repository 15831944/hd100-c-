#pragma once
#include "paramdef.h"
#include "../../Include/TSC.h"
#include <Afxtempl.h>
#include "vector"

#include "IOCtrl.h"
#include "GIniFile.h"
#include "SMC606.h"

#define	 CRD_TYPE_GTS	1

#define PARAM_POS		7
enum K_RUN_STATUS
{
	K_RUN_STS_NONE,
	K_RUN_STS_RUN,
	K_RUN_STS_PAUSE,
	K_RUN_STS_STOP,
	K_RUN_STS_RUN_1_UNIT,	// ���е㽺ʱ������һ��unit��ֹͣ����
	K_RUN_STS_RUN_1_BOARD,	// ���굱ǰ��Ƭ��ֹͣ����

	K_RUN_STS_WEIGHTERROR,
	K_RUN_STS_EMERGENCY,
	K_AXIS_STS_LIMIT_XP,	
	K_AXIS_STS_LIMIT_XS,	
	K_AXIS_STS_LIMIT_YP,
	K_AXIS_STS_LIMIT_YS,
	K_AXIS_STS_LIMIT_ZP,
	K_AXIS_STS_LIMIT_ZS,
	K_AXIS_STS_ALM_X,
	K_AXIS_STS_ALM_Y,
	K_AXIS_STS_ALM_Z,
	K_RUN_STS_WARNING,
};

enum SYNTAX_ERROR_CODE
{
    SYNTAX_NO_ERROR,
	SYNTAX_NO_LINE_END,
	SYNTAX_NO_LINE_START,
	SYNTAX_LINE_PASS_ERR,
	SYNTAX_ARC_ERR,
	SYNTAX_DATA_VOER,
	SYNTAX_GOTO_ADD_ERR,
	SYNTAX_LINE_END_ERR,
	SYNTAX_REPEAT_ERR,
	SYNTAX_BRUSHAREA_ERROR,
	SYNTAX_LOOP_ADD_ERR,
	SYNTAX_POINT_DISPENSE_ERROR,
	SYNTAX_NO_END_PROGRAME,
	SYNTAX_CIRCLE_ERROR,
	SYNTAX_WCL_PARAERROR
};

struct tgZMeter
{
	int    index;  //�к���
	double delta;  //�߶Ȳ������
};

using namespace std;

struct tgPathInfo	// ·����Ϣ,����ʱ���﷨�����,��¼������Ϣ
{
	tgPathInfo()
	{
		m_bFindMark = false;		// �������Ƿ�Ҫ����Mark��
		m_nMarkNum	= 0;			// ������Mark����Ŀ
		m_bIfReverseFirst = false;

//		m_bRepeat	= false;		// �Ƿ������е㽺
// 		m_bRepeatSetStart = false;	// �����Ƿ���м俪ʼ�㽺
// 		m_nRepeatStX = 1;		// ������ʼ��
// 		m_nRepeatStY = 1;		// ������ʼ��
	}

	bool m_bIfReverseFirst;		// �Ƿ��ȷ���
	bool m_bFindMark;		// �������Ƿ�Ҫ����Mark��
	int	 m_nMarkNum;		// ������Mark����Ŀ

	bool m_bRepeat;			// �Ƿ������е㽺
	bool m_bRepeatSetStart;	// �����Ƿ���м俪ʼ�㽺
	int  m_nRepeatStX;		// ������ʼ��
	int  m_nRepeatStY;		// ������ʼ��
};

class CCmdRun
{
public:
	CCmdRun(void);
	~CCmdRun(void);

	BOOL Init();
	void LoadIOMap();

	tgPathInfo m_structPathInfo;
	//////////////////////////////////////////////////////////////////////////
public:	
	static void __stdcall CommandHandle(char *command,short error);
	void CheckIO();
	void CheckStatus();
    int  checkWeightLMT(int ID);

	int AfxMessageBox(LPCTSTR lpszText, UINT nType=MB_OK, UINT nIDHelp=0);
	unsigned int m_nLoadStep;
	unsigned int m_nUnloadStep;
	CKFTimer m_tmLoad;		// ���ϼ�ʱ��
	CKFTimer m_tmUnload;	// ���ϼ�ʱ��
	CKFTimer m_tmAutoRps;	// �Զ����̻����������ʱ��
	CKFTimer m_tmAutoWeight;// �Զ����ؼ�ʱ��
	bool  m_bLoadFinish;
	bool  m_bLoadSuccess;
	bool  m_bUnloadFinish;
	bool  m_bUnloadSuccess;

	bool  m_bCanWeightAndWash;	// ������λ�������źŻ�����ڹ⿪��Ӧ�����Ժ�,�㽺�߳̿���ȥ���غ���ϴ����
	bool  m_bLoadEnable;
	bool  m_bUnloadEnable;
	bool  m_bDispEnable;		// �Ƿ��Ѿ�������ɣ�����㽺����
	int	  LoadStep(void);
	void  LoadBoard(void);
	int   UnLoadStep(void);
	void  UnloadBoard(void);
	int   FixBoard(void);
	int   UnFixBoard(void);
	int	  ReverseBoard(void);	// �㽺�����еķ���ָ��

	int	  LoadStep_3Box(void);		// ����ʽ3���Ϻ����ϲ���
	int	  UnloadStep_3Box(void);	// ����ʽ3���Ϻ����ϲ���

public:
	void Jog(int iAxisNo, double dMvSpeed/*0~1*/, int iDir/*-1, +1*/,BOOL bEnableStep, double fDist);
    short CommMotionCtrl(int port,bool bOn);
	short ResetFeed(int port);

	void PrepareHome(void);
	bool m_bHomeSuccess;	// �����Ƿ��Ѿ�����ɹ�
	void Home();
	void Run();	
	void Step();
	void Pause();
	void Ready();
	void Stop(bool bExitThread = true);
	int Play();
	void Emergency();
	void Net();
	void UnNet();
	void Free();
    void JetPressCtrl(double dDate);        // ����ѹ������  �������������䷧ϵ�У�
	void TubePressCtrl(double dDate);       // ��Ͳѹ������  ������ѹ��/�������䷧ϵ�У�
	short TestDisp();
	void NeedleTestDisp();  // ��Ͳ��������
	void SetLamp();
	void WashNeedle();    // ����ʽ��ϴ����
	void WashJet(bool bIfDisp=false);    // ����ʽ��ϴ����	num:��ϴ����    bIfDisp:�Ƿ��Զ�����ʱ�㽺

	short DotDisp(int dotNum, RunMode md=RunMode_Wet);		// ����ָ�������ĵĵ�
	short WeightAdjust(void);		// ����У׼
	short DispOpen(bool bOpen = false);
	short GotoSafeZpos(BOOL bWaitStop=TRUE);
	short JetOutput(long PulseNum,BOOL bOn);
	short NeedleDisp(int times,BOOL Open=TRUE);
	short NeedleDispEx(long PulseNum,long opentime,long closetime);
    short JetOutputEx(long PulseNum,long opentime,long closetime,BOOL bOn);// ���������亯��
	//////////////////////////////////////////////////////////////////////////
	short ExInput(CString strBitName,BOOL &bOn);
	short ExOutput(CString strBitName,BOOL bOn);
	int	  ExInputBitNameToNo(CString strBitName);
	int   ExOutputBitNameToNo(CString strBitName);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ģ����ģ�� ��Ͱ��ѹ����������
	short SetBackPressure(double dVal);
	short GetBackPressure(double &dVal);
	//////////////////////////////////////////////////////////////////////////
public:
	int GetPreDot(int iCurIndex, tgCmdLine &tDot);
	int GetNextDot(int iCurIndex, tgCmdLine &tDot);
	int GetFindCallSub(int iCurIndex, tgCmdLine &tDot); //���ӳ���ָ��
	int GetFindCallJump(int iCurIndex, tgCmdLine &tDot); //����תָ��
	int GetPreLineDot(int iCurIndex, tgCmdLine &tDot);
	int GetNextLineDot(int iCurIndex, tgCmdLine &tDot);
    int GetFindCallFiledLimiter(int iCurIndex, tgCmdLine &tDot); //�зֶ�ָ��
	int GetFindCallFiledScorp(int iCurIndex, tgCmdLine &tDot);   //��ƴ��ָ��
	short GetRecord(tgCmdLine &address,int Index);
	TS_CMD_TYPE TranslateType(CMD_TYPE tType);

	BOOL RunListOffsetChange(int iStartIndex, int iEndIndex, double dOffsetX, double dOffsetY,double dOffsetZ);
	BOOL GetCircle(const tgCmdLine *tgCmd, double *pX, double *pY, double *pR);
	//
	void AutoRPSRun();		// �����Զ�����
	void AutoRps(void);		// �Զ��������춯��
private:
	void RunListClear();
	void RunListShow();
	BOOL RunListExpand();
	BOOL RunListOffset(tgCmdLine *pCmd,double dOffsetX, double dOffsetY,double dOffsetZ);
	//////////////////////////////////////////////////////////////////////////
protected:
	BOOL FindMark(const tgCmdLine *pCmd);
	BOOL SubFindMark(const tgCmdLine *pCmd);
	BOOL GetZCheckCmdPosition(double x, double y, double *pX, double *pY, double dOffsetX, double dOffsetY);
	BOOL GetCmdPosition(tgCmdLine *pCmd, double *pX, double *pY, double *pZ, double dOffsetX, double dOffsetY, int iPosIndex=0/*���*/);
    
private:
	//////////////////////////////////////////////////////////////////////////
    BOOL   Syntax();
	SYNTAX_ERROR_CODE  SubSyntax(void);
    BOOL   CheckWCLData(tgCmdLine *pCmd);
	short  CheckPos(tgCmdLine *pCmd);
	short  SearchLabel(char *note);
	short  SearchLabelEnd(char *note);
	void   GetGotoOffsetEx(int uiTemp,tgCmdLine address1);
    void   GetGotoOffset(int uiTemp,tgCmdLine address1);
private:
	static UINT PlayThread(LPVOID lparam);
	static UINT UpdateThread(LPVOID lparam);
	static UINT MonitorThread(LPVOID lparam);
    static UINT LineSignStsThread(LPVOID lparam);
	static UINT AutoRPSThread(LPVOID lparam);
	static UINT LoadThread(LPVOID lparam);
	static UINT UnloadThread(LPVOID lparam);
	static UINT LightSetThread(LPVOID lparam);
	static UINT LoadThread_3Box(LPVOID lparam);		// ����ʽ3���Ϻ������߳�
	static UINT UnloadThread_3Box(LPVOID lparam);	// ����ʽ3���Ϻ������߳�
	static UINT ThreadHomeLd(LPVOID lparam);		// ����ʽ����������
	static UINT ThreadHomeUld(LPVOID lparam);		// ����ʽ����������

public:
	CList  <tgCmdLine, tgCmdLine&> m_cmdRunList;      //��������
	CList  <t_HPoint, t_HPoint&> m_cmdHComperList;    //���αȽ������������
	vector <tgZMeter> m_ZMeterVector;                //�߶����в������
    short RunBoardArc(tgCmdLine pCmd,double dOffsetX,double dOffsetY);
	short RunRepeat(unsigned char type,/*int uiIndex,*/tgCmdLine pCmd,double dOffsetX,double dOffsetY);
	short RunRepeatZMeter(unsigned char type,tgCmdLine *pCmd,double dOffsetX,double dOffsetY);
	short RunAdd(int *iCurIndex,tgCmdLine *tDot0,tgCmdLine *tDot1,tgCmdLine *tDot2,tgCmdLine *PreTrackPoint,tgCmdLine *NextTrackPoint,double dOffsetX,double dOffsetY,double dOffsetZ=0.0);
	short RunSub(int iStartIndex, int iEndIndex, double dOffsetX,double dOffsetY,double dOffsetZ=0/*�ο���ƫ�Ƶ���*/);
	short RunStep(int *pIndex,double dOffsetX,double dOffsetY);

	short RunDelay(tgCmdLine *pCmd);
	short RunPause();
	short RunHome();
	short RunSetIO(tgCmdLine *pCmd);
	short RunGoto(int * uiIndex,tgCmdLine &address0,tgCmdLine &address1,tgCmdLine &address2);

	short RunDispensor(tgCmdLine *pCmd);
	short RunTool(tgCmdLine *pCmd);
	short RunWashNeedle();
	short RunStandBy(tgCmdLine *pCmd,double dOffsetX,double dOffsetY);
	short RunDummy(tgCmdLine *pCmd,double dOffsetX,double dOffsetY);
	short RunPoint(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunLineStart(tgCmdLine *pCmd1,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunLinePass(tgCmdLine *pPreTrackCmd,tgCmdLine *pCmd1,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunLineArc(tgCmdLine *pPreTrackCmd,tgCmdLine *pCmd1,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunCircle(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunLineEnd(tgCmdLine *pPreTrackCmd,tgCmdLine *pCmd1,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunZHeight(tgCmdLine *pCmd,double dOffsetX,double dOffsetY,double *dValue);
	short RunWCLine(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunWCPot(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunWCRect(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
    
	// �ɽ��йغ���
	short RunSub_EX(int iStartIndex, int iEndIndex, double dOffsetX,double dOffsetY,double dOffsetZ=0/*�ο���ƫ�Ƶ���*/);
    short RunAdd_EX(int *iCurIndex,tgCmdLine *tDot0,tgCmdLine *tDot1,tgCmdLine *tDot2,tgCmdLine *PreTrackPoint,tgCmdLine *NextTrackPoint,double dOffsetX,double dOffsetY,double dOffsetZ=0.0);
	short RunFlyPoint_StartPos(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunFlyPoint_MidPos(tgCmdLine *pPreTrackCmd, tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunFlyPoint_EndPos(tgCmdLine *pPreTrackCmd, tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
    short RunFlyPoint_Dots(tgCmdLine *pCmd,double dOffsetX,double dOffsetY);
	int   nFlyDataStartID;   
	int   nFlydataEndID;
    CARTESIAN_POINT  CmpOrgPos;  //
	BOOL bNeedFillCMPList;        // �Ƿ���Ҫ���Ƚ��б�
	double CalcVelFlyOffset(double dvel,double djetdelay);  // �����ٶȼ���ƫ����
   
///////////�ֶ��йغ���//////////////////////
	tgPos		m_tMarkFindPosEX[4][2];          // ���ҵ���ģ�����Ļ�е����
	tgPos		m_tOffsetEX[4],                  // ��ǰ����ģ������λ�úͱ궨ģ������λ��ƫ��
		        m_tMarkCenterEX[4];              // 2���궨ģ��mark �� ����λ��
	double		m_dRotateAngleEX[4];             // ��ǰģ��ͱ�׼ģ��֮ǰ�ĽǶ�ƫ��
	
	int  m_MarkGropID;	// mark���� 
    BOOL m_bAskSeparaSign;

	BOOL  FindMarkEx(tgCmdLine *pCmd);     
	BOOL  SubFindMarkEx(tgCmdLine *pCmd);
	BOOL  GetCmdCameraPosition(tgCmdLine *pCmd, double *pX, double *pY);
	BOOL  GetCmdPositionEX(tgCmdLine *pCmd, double *pX, double *pY, double *pZ, double dOffsetX, double dOffsetY,int nGID);
	BOOL  GetRepeatPosition(tgCmdLine *pCmd, double *pX, double *pY);	// ����ʱ,��ȡ���о���λ�õ�3������
	BOOL  ReviewFindMark(tgCmdLine *pCmd);
	
	static int m_WCBatetID ;   // �հ����ID
	static int m_WCBatetID2 ;   // �������ID�� 
public: 
	BOOL m_bDoorSFSts; // ��ȫ��״̬ 0=�� 1=�ر�
	BOOL CalcBoradArcData(double &dx, double &dy, double dCurXdis, double dCurYdis, double dRArc);
	BOOL CalcBoradArcData(double &dx, double &dy, double dXDis, double dYDis);
	bool RunWCL_addSTrack;//���ӿ�ʼ��
  
	// ��ȡģ������������
	BOOL GetLaserData(unsigned short &date,int ntime=4,int nDelay=0.1);
	unsigned int PoaIndex;             //�ӳ��������
	HWND m_hMsgReceiveWnd;
    int m_DispCYtime;
	double g_CurDrawPos[4];//ʵʱλ�û�ͼ ��ɫʮ��

	CIOCtrl *m_InPutCtrl;
	CIOCtrl *m_OutPutCtrl;

	CController	*m_pController;

	//////////////////////////////////////////////////////////////////////////
	//Ϊ�˿��ٷ���IO��,��������
	int inEmergecy;     // ��ͣ
	int inWashPort;		// ������
	int inPLCError;		// PLC�����ź�
    int inSenseCheckZ;	// Z ��У׼�ź�

	int outJetPort;		// �㽺�������
	//////////////////////////////////////////////////////////////////////////

	int m_CurToolNo;     // ��ǰѹ�뻺�����Ĺ��ߺ�

	t_Tool *dcp;         // ����ָ��

	tgCmdParam  m_cmdParam;
	
	long m_outputSts;    // ʵʱ�������״̬
	long m_inputSts;     // ʵʱ�������״̬
	long m_exInputSts;	 // ��չ�������״̬
	unsigned short m_ExInputSts;      // ʵʱ��������չ�����״̬  add by zhang.f
	unsigned short m_ExOutputSts;     // ʵʱ��������չ�����״̬  add by zhang.f
	K_RUN_STATUS	m_tSysStatus;	
     
	tgPos		m_tMarkFindPos[3];          // Mark ����ģ�����Ļ�е����
	tgPos		m_tOffset,                  // ��ǰ����ģ������λ�úͱ궨ģ������λ��ƫ��
		        m_tMarkCenter;              // 2���궨ģ��mark �� ����λ��
	double		m_dRotateAngle;             // ��ǰģ��ͱ�׼ģ��֮ǰ�ĽǶ�ƫ��
	double		m_dFirstHeightOffset;       // ��һ�����߶�ƫ��

	SYSTEMTIME   st1,st2;//��������ʱ�� 
    
	BOOL m_bAutoPreFic;     // �Զ����̻�ģʽ

	int m_iCurIndex;		// ��ǰִ��ָ���к�
	int m_iStepIndex;
	BOOL m_bModifiedTrack; // �б��޸Ĺ�
	BOOL m_SocketOnline;   // TCP/IP������־

	BOOL bRobotIsMoving;

	BOOL m_bFlagRunning;   // �Ƿ�����������ɱ�־
	bool m_bIsHandPause;


	bool m_bWorkStopCYSign;  // �ӹ����赲���׵�λ�ź�
	bool m_bSqeStopCYSign;   // �ֶ��赲���׵�λ�ź�
	bool m_bUpStopCYSign1;   // ����������λ�ź�
	bool m_bUpStopCYSign2;   // ����������λ�ź�

	//////////////////////////////////////////////////////////////////////////
	//					����ʽ3���Ϻ�����									//
	//////////////////////////////////////////////////////////////////////////
	short LoadUnLoad_loadBoxMove(int idx, bool bWaitStop=true);
	short LoadUnLoad_loadLayerMove(int idx);
	short LoadUnLoad_unloadBoxMove(int idx, bool bWaitStop=true);
	short LoadUnLoad_unloadLayerMove(int idx);

	short LoadUnLoad_loadPushOut(BOOL &bBdDetect, BOOL bToEnd=FALSE);	//bBdDetect:����Ƿ��⵽����   bToEnd:�Ƿ����ϵ��յ�
	short LoadUnLoad_loadPullBack();
	short LoadUnLoad_unloadPushOut();
	short LoadUnLoad_unloadPullBack();

	// ���ϵ�������ƶ�ǰ��״̬ȷ�ϣ�һ��Ҫ�������������ƶ�����ֹ����
	short LoadUnLoad_loadCheck();
	// ���ϵ�������ƶ�ǰ��״̬ȷ�ϣ�һ��Ҫ�������������ƶ�����ֹ����
	short LoadUnLoad_unloadCheck();
	bool m_bRunOnce;				// �����Ϻ����,���һƬ�ϵ����ֹͣ
	int  m_nRePushCount;			// �ظ����ϼ���
	int  m_nEmptyLayerCount;		// �������ϲ����
	bool m_bDownLayerAfterWarning;	// ���ϳ�������,�Ϻ���û������һ��

	short LoadUnLoad_Home(int mtrIndex);	// �����ϻ�ԭ��		mtrIndex:   0-���ϵ��	1-���ϵ��
	bool m_bLdHomeSucceed;	// �����Ƿ��Ѿ��ɹ���������
	bool m_bLdHomeFinish;	// �����Ƿ��Ѿ���ɻ�������
	bool m_bUldHomeSucceed;	// �����Ƿ��Ѿ��ɹ���������
	bool m_bUldHomeFinish;	// �����Ƿ��Ѿ���ɻ�������

	//////////////////////////////////////////////////////////////////////////
	// �㽺ģʽ     dryRun    wetRun
	void SetRunMode(RunMode md);
	RunMode m_runMode;

	bool m_bDispOpen;
	//////////////////////////////////////////////////////////////////////////
	// �������ƿ�
	CSMC606 m_lsCtrl;
private:
	int m_iLoop;
	int m_iDebugErrorLine;
};
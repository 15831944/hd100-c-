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
	K_RUN_STS_RUN_1_UNIT,	// 阵列点胶时，点完一个unit才停止程序
	K_RUN_STS_RUN_1_BOARD,	// 点完当前料片就停止程序

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
	int    index;  //行和列
	double delta;  //高度测量结果
};

using namespace std;

struct tgPathInfo	// 路径信息,运行时在语法检查中,记录程序信息
{
	tgPathInfo()
	{
		m_bFindMark = false;		// 程序中是否要查找Mark点
		m_nMarkNum	= 0;			// 程序中Mark点数目
		m_bIfReverseFirst = false;

//		m_bRepeat	= false;		// 是否有阵列点胶
// 		m_bRepeatSetStart = false;	// 阵列是否从中间开始点胶
// 		m_nRepeatStX = 1;		// 阵列起始列
// 		m_nRepeatStY = 1;		// 阵列起始列
	}

	bool m_bIfReverseFirst;		// 是否先翻板
	bool m_bFindMark;		// 程序中是否要查找Mark点
	int	 m_nMarkNum;		// 程序中Mark点数目

	bool m_bRepeat;			// 是否有阵列点胶
	bool m_bRepeatSetStart;	// 阵列是否从中间开始点胶
	int  m_nRepeatStX;		// 阵列起始列
	int  m_nRepeatStY;		// 阵列起始列
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
	CKFTimer m_tmLoad;		// 进料计时器
	CKFTimer m_tmUnload;	// 出料计时器
	CKFTimer m_tmAutoRps;	// 自动防固化清理喷嘴计时器
	CKFTimer m_tmAutoWeight;// 自动称重计时器
	bool  m_bLoadFinish;
	bool  m_bLoadSuccess;
	bool  m_bUnloadFinish;
	bool  m_bUnloadSuccess;

	bool  m_bCanWeightAndWash;	// 读到上位机出板信号或者入口光开感应到板以后,点胶线程可以去称重和清洗动作
	bool  m_bLoadEnable;
	bool  m_bUnloadEnable;
	bool  m_bDispEnable;		// 是否已经进料完成，激活点胶动作
	int	  LoadStep(void);
	void  LoadBoard(void);
	int   UnLoadStep(void);
	void  UnloadBoard(void);
	int   FixBoard(void);
	int   UnFixBoard(void);
	int	  ReverseBoard(void);	// 点胶流程中的翻板指令

	int	  LoadStep_3Box(void);		// 离线式3层料盒上料步骤
	int	  UnloadStep_3Box(void);	// 离线式3层料盒下料步骤

public:
	void Jog(int iAxisNo, double dMvSpeed/*0~1*/, int iDir/*-1, +1*/,BOOL bEnableStep, double fDist);
    short CommMotionCtrl(int port,bool bOn);
	short ResetFeed(int port);

	void PrepareHome(void);
	bool m_bHomeSuccess;	// 机器是否已经回零成功
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
    void JetPressCtrl(double dDate);        // 阀体压力设置  （适用气动喷射阀系列）
	void TubePressCtrl(double dDate);       // 胶筒压力设置  （适用压电/气动喷射阀系列）
	short TestDisp();
	void NeedleTestDisp();  // 针筒出胶测试
	void SetLamp();
	void WashNeedle();    // 单吸式清洗动作
	void WashJet(bool bIfDisp=false);    // 擦拭式清洗动作	num:清洗次数    bIfDisp:是否自动运行时点胶

	short DotDisp(int dotNum, RunMode md=RunMode_Wet);		// 喷射指定数量的的点
	short WeightAdjust(void);		// 称重校准
	short DispOpen(bool bOpen = false);
	short GotoSafeZpos(BOOL bWaitStop=TRUE);
	short JetOutput(long PulseNum,BOOL bOn);
	short NeedleDisp(int times,BOOL Open=TRUE);
	short NeedleDispEx(long PulseNum,long opentime,long closetime);
    short JetOutputEx(long PulseNum,long opentime,long closetime,BOOL bOn);// 称重用喷射函数
	//////////////////////////////////////////////////////////////////////////
	short ExInput(CString strBitName,BOOL &bOn);
	short ExOutput(CString strBitName,BOOL bOn);
	int	  ExInputBitNameToNo(CString strBitName);
	int   ExOutputBitNameToNo(CString strBitName);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 模拟量模块 胶桶背压比例阀操作
	short SetBackPressure(double dVal);
	short GetBackPressure(double &dVal);
	//////////////////////////////////////////////////////////////////////////
public:
	int GetPreDot(int iCurIndex, tgCmdLine &tDot);
	int GetNextDot(int iCurIndex, tgCmdLine &tDot);
	int GetFindCallSub(int iCurIndex, tgCmdLine &tDot); //有子程序指令
	int GetFindCallJump(int iCurIndex, tgCmdLine &tDot); //有跳转指令
	int GetPreLineDot(int iCurIndex, tgCmdLine &tDot);
	int GetNextLineDot(int iCurIndex, tgCmdLine &tDot);
    int GetFindCallFiledLimiter(int iCurIndex, tgCmdLine &tDot); //有分段指令
	int GetFindCallFiledScorp(int iCurIndex, tgCmdLine &tDot);   //有拼版指令
	short GetRecord(tgCmdLine &address,int Index);
	TS_CMD_TYPE TranslateType(CMD_TYPE tType);

	BOOL RunListOffsetChange(int iStartIndex, int iEndIndex, double dOffsetX, double dOffsetY,double dOffsetZ);
	BOOL GetCircle(const tgCmdLine *tgCmd, double *pX, double *pY, double *pR);
	//
	void AutoRPSRun();		// 启动自动清喷
	void AutoRps(void);		// 自动清理喷嘴动作
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
	BOOL GetCmdPosition(tgCmdLine *pCmd, double *pX, double *pY, double *pZ, double dOffsetX, double dOffsetY, int iPosIndex=0/*序号*/);
    
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
	static UINT LoadThread_3Box(LPVOID lparam);		// 离线式3层料盒上料线程
	static UINT UnloadThread_3Box(LPVOID lparam);	// 离线式3层料盒下料线程
	static UINT ThreadHomeLd(LPVOID lparam);		// 离线式上料马达回零
	static UINT ThreadHomeUld(LPVOID lparam);		// 离线式下料马达回零

public:
	CList  <tgCmdLine, tgCmdLine&> m_cmdRunList;      //运行链表
	CList  <t_HPoint, t_HPoint&> m_cmdHComperList;    //单次比较输出数据链表
	vector <tgZMeter> m_ZMeterVector;                //高度阵列测量结果
    short RunBoardArc(tgCmdLine pCmd,double dOffsetX,double dOffsetY);
	short RunRepeat(unsigned char type,/*int uiIndex,*/tgCmdLine pCmd,double dOffsetX,double dOffsetY);
	short RunRepeatZMeter(unsigned char type,tgCmdLine *pCmd,double dOffsetX,double dOffsetY);
	short RunAdd(int *iCurIndex,tgCmdLine *tDot0,tgCmdLine *tDot1,tgCmdLine *tDot2,tgCmdLine *PreTrackPoint,tgCmdLine *NextTrackPoint,double dOffsetX,double dOffsetY,double dOffsetZ=0.0);
	short RunSub(int iStartIndex, int iEndIndex, double dOffsetX,double dOffsetY,double dOffsetZ=0/*参考点偏移导入*/);
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
    
	// 飞胶有关函数
	short RunSub_EX(int iStartIndex, int iEndIndex, double dOffsetX,double dOffsetY,double dOffsetZ=0/*参考点偏移导入*/);
    short RunAdd_EX(int *iCurIndex,tgCmdLine *tDot0,tgCmdLine *tDot1,tgCmdLine *tDot2,tgCmdLine *PreTrackPoint,tgCmdLine *NextTrackPoint,double dOffsetX,double dOffsetY,double dOffsetZ=0.0);
	short RunFlyPoint_StartPos(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunFlyPoint_MidPos(tgCmdLine *pPreTrackCmd, tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
	short RunFlyPoint_EndPos(tgCmdLine *pPreTrackCmd, tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY);
    short RunFlyPoint_Dots(tgCmdLine *pCmd,double dOffsetX,double dOffsetY);
	int   nFlyDataStartID;   
	int   nFlydataEndID;
    CARTESIAN_POINT  CmpOrgPos;  //
	BOOL bNeedFillCMPList;        // 是否需要填充比较列表
	double CalcVelFlyOffset(double dvel,double djetdelay);  // 根据速度计算偏移量
   
///////////分段有关函数//////////////////////
	tgPos		m_tMarkFindPosEX[4][2];          // 查找到的模板中心机械坐标
	tgPos		m_tOffsetEX[4],                  // 当前查找模板中心位置和标定模板中心位置偏差
		        m_tMarkCenterEX[4];              // 2个标定模板mark 点 中心位置
	double		m_dRotateAngleEX[4];             // 当前模板和标准模板之前的角度偏差
	
	int  m_MarkGropID;	// mark组编号 
    BOOL m_bAskSeparaSign;

	BOOL  FindMarkEx(tgCmdLine *pCmd);     
	BOOL  SubFindMarkEx(tgCmdLine *pCmd);
	BOOL  GetCmdCameraPosition(tgCmdLine *pCmd, double *pX, double *pY);
	BOOL  GetCmdPositionEX(tgCmdLine *pCmd, double *pX, double *pY, double *pZ, double dOffsetX, double dOffsetY,int nGID);
	BOOL  GetRepeatPosition(tgCmdLine *pCmd, double *pX, double *pY);	// 阵列时,获取阵列矩阵位置的3点坐标
	BOOL  ReviewFindMark(tgCmdLine *pCmd);
	
	static int m_WCBatetID ;   // 空板称重ID
	static int m_WCBatetID2 ;   // 满板称重ID　 
public: 
	BOOL m_bDoorSFSts; // 安全门状态 0=打开 1=关闭
	BOOL CalcBoradArcData(double &dx, double &dy, double dCurXdis, double dCurYdis, double dRArc);
	BOOL CalcBoradArcData(double &dx, double &dy, double dXDis, double dYDis);
	bool RunWCL_addSTrack;//增加开始线
  
	// 获取模拟量激光数据
	BOOL GetLaserData(unsigned short &date,int ntime=4,int nDelay=0.1);
	unsigned int PoaIndex;             //子程序组序号
	HWND m_hMsgReceiveWnd;
    int m_DispCYtime;
	double g_CurDrawPos[4];//实时位置绘图 红色十字

	CIOCtrl *m_InPutCtrl;
	CIOCtrl *m_OutPutCtrl;

	CController	*m_pController;

	//////////////////////////////////////////////////////////////////////////
	//为了快速访问IO口,单独定义
	int inEmergecy;     // 急停
	int inWashPort;		// 真空清洁
	int inPLCError;		// PLC报警信号
    int inSenseCheckZ;	// Z 轴校准信号

	int outJetPort;		// 点胶阀喷射口
	//////////////////////////////////////////////////////////////////////////

	int m_CurToolNo;     // 当前压入缓冲区的工具号

	t_Tool *dcp;         // 工具指针

	tgCmdParam  m_cmdParam;
	
	long m_outputSts;    // 实时的输出口状态
	long m_inputSts;     // 实时的输入口状态
	long m_exInputSts;	 // 扩展口输入口状态
	unsigned short m_ExInputSts;      // 实时的数字扩展输入口状态  add by zhang.f
	unsigned short m_ExOutputSts;     // 实时的数字扩展输出口状态  add by zhang.f
	K_RUN_STATUS	m_tSysStatus;	
     
	tgPos		m_tMarkFindPos[3];          // Mark 查找模板中心机械坐标
	tgPos		m_tOffset,                  // 当前查找模板中心位置和标定模板中心位置偏差
		        m_tMarkCenter;              // 2个标定模板mark 点 中心位置
	double		m_dRotateAngle;             // 当前模板和标准模板之前的角度偏差
	double		m_dFirstHeightOffset;       // 第一个检测高度偏差

	SYSTEMTIME   st1,st2;//单次周期时间 
    
	BOOL m_bAutoPreFic;     // 自动防固化模式

	int m_iCurIndex;		// 当前执行指令行号
	int m_iStepIndex;
	BOOL m_bModifiedTrack; // 列表被修改过
	BOOL m_SocketOnline;   // TCP/IP联网标志

	BOOL bRobotIsMoving;

	BOOL m_bFlagRunning;   // 是否完整运行完成标志
	bool m_bIsHandPause;


	bool m_bWorkStopCYSign;  // 加工区阻挡气缸到位信号
	bool m_bSqeStopCYSign;   // 分段阻挡气缸到位信号
	bool m_bUpStopCYSign1;   // 顶升气缸上位信号
	bool m_bUpStopCYSign2;   // 顶升气缸下位信号

	//////////////////////////////////////////////////////////////////////////
	//					离线式3层料盒上料									//
	//////////////////////////////////////////////////////////////////////////
	short LoadUnLoad_loadBoxMove(int idx, bool bWaitStop=true);
	short LoadUnLoad_loadLayerMove(int idx);
	short LoadUnLoad_unloadBoxMove(int idx, bool bWaitStop=true);
	short LoadUnLoad_unloadLayerMove(int idx);

	short LoadUnLoad_loadPushOut(BOOL &bBdDetect, BOOL bToEnd=FALSE);	//bBdDetect:入口是否检测到物料   bToEnd:是否推料到终点
	short LoadUnLoad_loadPullBack();
	short LoadUnLoad_unloadPushOut();
	short LoadUnLoad_unloadPullBack();

	// 进料电机上下移动前的状态确认，一定要满足条件才能移动，防止卡料
	short LoadUnLoad_loadCheck();
	// 下料电机上下移动前的状态确认，一定要满足条件才能移动，防止卡料
	short LoadUnLoad_unloadCheck();
	bool m_bRunOnce;				// 进料料盒完后,最后一片料点完才停止
	int  m_nRePushCount;			// 重复推料计数
	int  m_nEmptyLayerCount;		// 连续空料层计数
	bool m_bDownLayerAfterWarning;	// 下料出错报警后,料盒有没有下移一层

	short LoadUnLoad_Home(int mtrIndex);	// 上下料回原点		mtrIndex:   0-上料电机	1-下料电机
	bool m_bLdHomeSucceed;	// 程序是否已经成功回零上料
	bool m_bLdHomeFinish;	// 程序是否已经完成回零上料
	bool m_bUldHomeSucceed;	// 程序是否已经成功回零下料
	bool m_bUldHomeFinish;	// 程序是否已经完成回零下料

	//////////////////////////////////////////////////////////////////////////
	// 点胶模式     dryRun    wetRun
	void SetRunMode(RunMode md);
	RunMode m_runMode;

	bool m_bDispOpen;
	//////////////////////////////////////////////////////////////////////////
	// 雷赛控制卡
	CSMC606 m_lsCtrl;
private:
	int m_iLoop;
	int m_iDebugErrorLine;
};
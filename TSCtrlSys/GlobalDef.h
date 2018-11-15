//////////////////////////////////////////////////////////////////////////
#if  !defined(GLOBALDEF_H)
#define GLOBALDEF_H
//////////////////////////////////////////////////////////////////////////

#define IDM_USER_PLAYBACK_FINISHED			  WM_USER+500
#define IDM_USER_SETFOCUS_NOW_PLAYBACK_INDEX  WM_USER+502
#define IDM_USER_MODIFY_STATUS				  WM_USER+503
#define IDM_USER_PLAYBACK_PAUSE				  WM_USER+504	

#define IDM_USER_FEED_READYINFO			      WM_USER+505
#define IDM_USER_UNFEED_READYINFO			  WM_USER+506

//自定义消息
#define WM_MSG_PANE_STS		    WM_USER + 0x100
#define WM_MSG_INPUT_EVENT	    WM_USER + 0x102//0x0400
#define WM_MSG_POS_EVENT	    WM_USER + 0x103//0x0400
#define WM_MSG_NEED_WASH	    WM_USER + 0x104
#define	WM_REDRAWIMAGEFROMCCD	WM_USER + 0x105	// 重绘CCD获取图像

#define CARDNUM 1
#define MAX_AXIS_NUM 8
#define LS_AXIS_NUM	 6
#define K_ALL_PORTS -1
#define PROJECTTYPE 0		// 0 点胶，1喷涂
#define PAINTTYPE   0		// 0,左右上下阀(PT541)，1左右上下，旋转阀

#define CRD1	1
#define CRD2	2

#define MAXTRACKNUM		4000	//支持的最大轨迹段

//////////////////////////////////////////////////////////////////////////
const int COM_PORT_USED = 5;	// 机器使用的串口数目

///////////////////////////////////
//IO信号自定义宏

#define IO_OUTPUT_OPEN    FALSE     // 打开输出
#define IO_OUTPUT_CLOSE   TRUE      // 关闭输出

#define IO_INPUT_HAVESIGN  TRUE   // 输入有信号
#define IO_INPUT_NOSIGN    FALSE  // 输入无信号

#define GRIP_OPEN        FALSE    // 夹爪打开
#define GRIP_CLOSE       TRUE     // 夹爪关闭

#define TYPE_INPUT				1
#define TYPE_OUTPUT				2

#define MODE_NULL				0
#define MODE_AUTO				1
#define MODE_TEACH				2

#define EDGE_ASCENDING			0	//上升沿
#define EDGE_DESCENDING			1	//下降沿

#define DIR_POSITIVE			1
#define DIR_NEGTIVE				-1

//////////////////////////////////
#define VEL_ACC_RATIO		200.
#define pi                  3.141592653589793238

//////////////////////////////////////////////////////////////////////////
//					函数返回值检查宏									//
//////////////////////////////////////////////////////////////////////////
//检查函数返回值,并打印出错信息,不返回值
#define CHECK_bool_NO_RTN_VAL(HintInfo, RtnVal) \
	do \
	{ \
	if (true != RtnVal) \
	{ \
		CString csErrInfo = HintInfo; \
		AfxMessageBox(csErrInfo); \
		return; \
	} \
	} while (0); 

#define CHECK_RTN_NO_RTNVAL(HintInfo, RtnVal) \
	do \
{ \
	if (RTN_NONE != RtnVal) \
{ \
	CString csErrInfo = HintInfo; \
	AfxMessageBox(csErrInfo); \
	return; \
} \
} while (0); 

#define CHECK_RTN_RTNVAL(HintInfo, RtnVal, rtnVal) \
	do \
{ \
	if (RTN_NONE != RtnVal) \
{ \
	CString csErrInfo = HintInfo; \
	AfxMessageBox(csErrInfo); \
	return rtnVal; \
} \
} while (0); 




//////////////////////////////////////////////////////////////////////////
//PLC Comunication send port
#define PLC_SEND_PORT_RESET					1000
#define PLC_SEND_PORT_MOTOR_BELT_ONOFF		1001
#define PLC_SEND_PORT_CYLIDER_BUFFER_ANTI	1002
#define PLC_SEND_PORT_CYLIDER_DISP_ANTI		1003
#define PLC_SEND_PORT_CYLIDER_DISP_UP		1004
#define PLC_SEND_PORT_CYLIDER_DISP_LOCA		1005
#define PLC_SEND_PORT_CYLIDER_UNLOAD_UP		1006
#define PLC_SEND_PORT_CYLIDER_UNLOAD_PUSH	1007

#define PLC_SEND_PORT_BACK1					1008
#define PLC_SEND_PORT_BACK2					1009
#define PLC_SEND_PORT_BACK3					1010
#define PLC_SEND_PORT_BACK4					1011
#define PLC_SEND_PORT_BACK5					1012
#define PLC_SEND_PORT_BACK6					1013
#define PLC_SEND_PORT_BACK7					1014
#define PLC_SEND_PORT_BACK8					1015

#define PLC_SEND_PORT_SDSET					1200
#define PLC_READ_PORT_STS_IO                7
#define PLC_READ_PORT_STS_HEART             2500   
#define PLC_READ_PORT_STS_BeltSepareMode    2502
#define PLC_SEAD_PORT_BELT_VEL				32720      // (10~300mm/s)    输送带速度
#define PLC_SEND_PORT_TEMPERATRUE           32722      // 喷头温度设置端口

//PLC Comunication read port
#define PLC_READ_PORT_HEIGHT				32710
#define PLC_READ_PORT_HEIGHT_BACK1			32712
#define PLC_READ_PORT_HEIGHT_BACK2			32714
#define PLC_READ_PORT_HEIGHT_BACK3			32716
#define PLC_READ_PORT_HEIGHT_BACK4			32718
 
#define PLC_READ_PORT_TEMPERATRUE			410  //(1~1000)   喷头温度读取地址

///////PLC输入信号定义
#define PLC_READ_PORT_BEIT_AIRSAFE         1550    // 气源检测
#define PLC_READ_PORT_BEIT_WAITSIGN        1551    // 待料区进料检测
#define PLC_READ_PORT_BEIT_WORKSIGN        1552    // 工作区工件检测
#define PLC_READ_PORT_BEIT_SEQARESIGN      1553    // 分段区工件检测
#define PLC_READ_PORT_BEIT_POSCYUP         1554    // 顶升气缸上位检测
#define PLC_READ_PORT_BEIT_POSCYDN         1555    // 顶升气缸下位检测
#define PLC_READ_PORT_BEIT_WORKSTOPCY      1556    // 加工阻挡气缸上位检测
#define PLC_READ_PORT_BEIT_SEQARESTOPCY    1557    // 分段阻挡气缸上位检测

///////////////////////////////////

// 联建定制接驳台通讯定义
#define PLC_SEND_PORT_FEEDUP		    1200  // 上料电子称上升运动
#define PLC_SEND_PORT_FEEDDOWN		    1201  // 上料电子称下降运动

#define PLC_SEND_PORT_UNFEEDUP		    1202   // 下料电子称上升运动
#define PLC_SEND_PORT_UNFEEDDOWN        1224   // 下料电子称下降运动
 
#define PLC_SEND_PORT_FEEDGETWIGHTFINISH		  1210   // 上料称重完成
#define PLC_SEND_PORT_UNFEEDGETWIGHTFINISH		  1214   // 下料称重完成

#define PLC_SEND_PORT_FEEDRESET          19    // 上料接驳台复位
#define PLC_SEND_PORT_UNFEEDRESET        18    // 下料接驳台复位

#define PLC_SEAD_PORT_FEEDUP_VEL				32724    // 上料电子称上升运动速度
#define PLC_SEAD_PORT_FEEDDOWN_VEL				32726    // 上料电子称下降运动速度
#define PLC_SEAD_PORT_UNFEEDUP_VEL				32728    // 下料电子称上升运动速度
#define PLC_SEAD_PORT_UNFEEDDOWN_VEL		    32730    // 下料电子称下升运动速度


static CString IO_CONFIG_FILE_PATH=_T("system\\ExtMdl.cfg");
static LPCTSTR IOMAP_FILE_PATH=_T("system\\iomap.ini");
static LPCTSTR IOMAP_FILE_PATH_OFFLINE = _T("system\\iomap(Probe).ini");

static unsigned long IOstatusBitsMask[16]={0x0001,0x0002,0x0004,0x0008,
											0x0010,0x0020,0x0040,0x0080,
											0x0100,0x0200,0x0400,0x0800,
											0x1000,0x2000,0x4000,0x8000};

const unsigned short LmtMask[8]={
	0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
};

const unsigned short OpenMask[16]={
	0xfffe,0xfffd,0xfffb,0xfff7,0xffef,0xffdf,0xffbf,0xff7f,
        0xfeff,0xfdff,0xfbff,0xf7ff,0xefff,0xdfff,0xbfff,0x7fff
};
const unsigned short CloseMask[16]={
	0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
		0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000
};

#define DefSysPassWordA  _T("000000") 
//#define DEBUG_SD950 
///////////////////
#define  DefDlgInfo "DLGINFO"  //
#define  DefShotCut "DLGUIJOG"
#define  DefMainFrm "MAINFRM"
#define  DefPageSys "DLGPAGESETTING"
#define  DefPagePos "DLGPAGEPOSSET"
#define  DefPageBalance "DLGPAGEBALABCE"
#define  DefPageLogin   "DLGLOGIN"
#define  DefPageFeedSys  "DLGPAGEFEEDSYS"
#define  DefDlgBoardCalib "DLGBOARDCALIB"
#define  DefPageJet    "DLGPAGEJET"
#define  DefPageAPP    "DLGPAGEAPP"
#define  DefPageOther  "DLGPAGEOTHER"
#define  DefDlgWCPL    "DLGWCPL"
#define  DefDlgWCRECT  "DLGWCRECT"
#define  DefDlgCMDAxis  "DLGCMDAXIS"
#define  DefDlgCMDCIRC  "DLGCMDCIRC"
#define  DefDlgCMDDELAY  "DLGCMDDELAY"
#define  DefDlgCMDJETCTRL "DLGCMDJETCTRL"
#define  DefDlgCMDCallSub  "DLGCMDSUBFUN"
#define  DefDlgCMDGOTO  "DLGCMDGOTO"
#define  DefDlgJetAdjust "DLGJETADJUST"
#define  DefDlgCMDJetPara "DLGCMDJETPARA"
#define  DefDlgCMDLineEnd "DLGCMDLINEEND"
#define  DefDlgCMDLinePara "DLGCMDLINESETUP"
#define  DefDlgCMDLOOP     "DLGCMDLOOP"
#define  DefDlgCMDMark     "DLGCMDMARK"
#define  DefDlgPix         "DLGIMGCALIB"
#define  DefDlgCMDPointEnd "DLGCMDPOINTEND"
#define  DefDlgCMDPointPara "DLGCMDPOINTPARA"
#define  DefDlgCMDRepeat    "DLGCMDREPEAT"
#define  DefDlgCMDIO         "DLGCMDIOSET"
#define  DefDlgCMDSpeed      "DLGCMDSPEEDPARA"
#define  DefDlgCMDStandBy    "DLGCMDSTANDBY" 
#define  DefDlgCMDSelectJet  "DLGCMDSELECTJET"
#define  DefDlgCMDZClear    "DLGCMDZCLEAR"
#define  DefDlgCMDJetHight  "DLGCMDJETHIGHT"
#define  DefDlgCMDZlimit    "DLGCMDZSAME"
#define  DefDlgOffset       "DLGCOFFSET"

//  
#define  DefCtrlMode_Pulse    0    // 脉冲控制模式
#define  DefCtrlMode_Analog   1    // 模拟量控制

#define  DefCCDType_Color           1
#define  DefCCDType_Monochrome      0
#endif
//////////////////////////////////////////////////////////////////////////
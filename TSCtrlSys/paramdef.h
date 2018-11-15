#ifndef TK_PARAM_DEFINE_H_
#define TK_PARAM_DEFINE_H_


extern CString szCmdline[];

#define TOOL_NUM 2    //末端工具数量
#define TOOL_MAXNUM		3

#define COMMAND_DOUBLE_LEN  20 //命令指令行的double个数
#define COMMAND_INT_LEN     10 //命令指令行的int个数

#define MAX_WCDATE 50

struct tgAxis
{
	tgAxis(){scale = 250;  minpos = 0;maxpos = 100;acc=0.5;}
	double scale;
	double minpos;
	double maxpos;
	double acc;
};

struct tgPos
{
	tgPos(){x = y= z = u = 0;}
	tgPos(double dx, double dy, double dz, double du){x =dx; y= dy;z =dz; u = du;}
	double x, y, z, u;
};

struct tgCircle
{
	double x,y;
	double r;
	double dx, dy;//开始点
};

struct tgBrush
{
	double x,y,z,u;				// 起点
	double dx,dy;				// 矩形步进距离
	double dfCx[2],dfCy[2];		// 半圆圆心
	int nDir;					// 矩形方向 0 = x ,1 = y
	int nNum;	// 轨迹条数
	double dRadius;
	double dAngle;
	double dStep;

};
struct tgArc
{
	double dx[3], dy[3],dz[3];//起点，中间点， 终点
	double x, y;//圆心
	double r;
	double angle;//[-360~360]
	double dStartAngle,dEndAngle;//[-360~360]
	int iType;//0:劣弧，1:优弧
	//
	BOOL bAddDir; //TRUE：正向旋转，FALSE:负向旋转
	BOOL bBigArc;//TRUE：劣弧，FALSE：优弧
};
//////////////////////////////////////////////////////////////////////////
//命令类型
enum CMD_TYPE
{
	CMD_NONE=0,
	CMD_MARK,			// Mark设置 ,tgPos[2],int iMarkNum
	CMD_DUMMY,			// 设定点胶针头实际所在的XYZ 坐标为空点，并记录于目前的纪录点位置。通常应用于避开工件的阻挡。
	CMD_POINT,			// 单点点胶,tgPos[1]
	CMD_LINE_START,		// 线段点胶起点tgPos[1]
	CMD_LINE_PASSING,	// 线段点胶中间点tgPos[1]
	CMD_LINE_ARC,		// 圆弧点胶 tgPos[1]
	CMD_LINE_END,		// 线段点胶结束点,tgPos[1]
	CMD_CIRCLE,			// 圆形点胶 1:圆心+直径, tgPos[1], double diameter, 2:3点画圆,tgPos[3]
	CMD_DISPENSER,		// 点胶阀开/关   BOOL On/Off
	CMD_GOTO_ADDRESS,	// 跳转指令   int iIndex
	CMD_REPEATX,		// X方向阵列  int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	CMD_REPEATY,		// Y方向阵列  int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	CMD_BRUSH_AREA,		// 涂覆,
	CMD_CALL_SUB,       // 调子函数
	CMD_CALL_PRO,       // 调子文件
	CMD_LABEL,			// 调用程序
	CMD_NULL_LINE,		// 空行指令
	CMD_SET_IO,
	CMD_DELAY,
	CMD_WAIT_POINT,     // 待机点
	CMD_PAUSE,			// 针头将移至此特定位置并且停止移动，直到使用者按下机台面板上的START按钮后继续往下执行纪录点
	CMD_HOME,			// X, Y, Z轴将回归软件原点(等待点)后继续往下执行纪录点。
	CMD_LOOP,			// 通常将循环执行地址设为1，当教导一组程序完成后，可利用此功能来设定运作次数
	CMD_DUNNY_SPEED,     
	CMD_NOTE,
	CMD_BATH,           // 清洗指令
	CMD_ZMETER,      	// 激光测高度,
	CMD_POT,            // 称重打点
	CMD_WCL,            // 称重划线
	CMD_RECT,   
	CMD_TOOL,
	CMD_REPEATX_ZH,
	CMD_REPEATY_ZH,
	CMD_OMRON,
	CMD_BACK5,
	CMD_END_PROGRAM,		//程序结束命令

	//////////////////////////////////////////////////////////////////////////
	PARAM_LINE_SPEED,		//线段点胶速度
	PARAM_LINE_SETUP,		//线段点胶参数设置
	PARAM_LINE_END,			//线段点胶结束设置
	PARAM_POINT_SETUP,		//单点点胶参数设置
	PARAM_POINT_END,		//单点点胶结束设置
	PARAM_Z_CLEARANCE,		//Z轴提高高度设置 
	PARAM_Z_LIMIT,			//Z轴极限值设置 
	PARAM_XY_VEL,			//XY轴空跑移动速度
	PARAM_Z_VEL,			//Z轴空跑移动速度
	PARAM_HOME_POSITION,	//设置RUN模式时点胶结束针头停留位置(等待点)
	PARAM_SET_Z_SAME,		//将插入行以后的坐标点Z轴高度设为相同数值
	PARAM_Z_SET_HEIGHT,		//设置Z轴高度
	PARAM_JET_SETUP,        //选择阀参数设置  
	CMD_WCRECT,             //称重矩形
	///////////////////////////////////////////////

	CMD_SUPERSTART,         //飞胶起点
	CMD_SUPERPOINT,         //飞胶单点
	CMD_SUPEREND,           //飞胶结束点
	//////////////////////////////////////////////
	CMD_FILEDELIMITER,      // 试教分段符
	PARAM_Z_WORKPOSSET,     // Z轴高度设置
	CMD_FILESCRPEWORK,      // 拼版加工符
	CMD_REPEATARC,          // 阵列夹角参数

	CMD_ReverseBoard,		// 翻板指令
	CMD_SUPERDOT,			// 高速打点指令
	CMD_LABEL_END,			// 子程序结束
};

enum
{
	AddOn_Reverse,
	AddOn_Heat,
	AddOn_None
};

enum
{
	FixMode_OnlyLift,
	FixMode_1Clamp,
	FixMode_1Lift
};

struct system_PID
{
	system_PID()
	{	reset();
	}
	void reset()
	{
		PID_P=1;
		PID_D = 0;
		PID_I = 0;
		Kvff = 0;
		Kaff = 0;
		PrfVelFeilter = 2;
	}
	double PID_P;
	double PID_D;
	double PID_I;
	double Kvff;
	double Kaff;
	double PrfVelFeilter;
};

struct tgBatetWCDate
{
	tgBatetWCDate()
	{	reset();
	}
	void reset()
	{
		ID = -1;
		dWCDate_0 = -1;
		dWCDate_1 = -1;
	}
	int   ID;
	double dWCDate_0;
	double dWCDate_1;
};

struct tgRobotParam		// 机器参数
{
	// 上下料参数
	struct tgLdUldParam
	{
		tgLdUldParam()
		{
			m_nCrtLdBoxId = 0;
			m_nCrtLdLayerId = 0;
			m_nLdLayerCount = 20;
			m_nLdRePushout = 1;		// 推料没感应到,重复推料次数
			m_nLdEmptyWarning = 1;		// 连续几层没推到料片报警
			m_nLdContinue = 0;		// 进料是否3层循环不报警

			m_nCrtUldBoxId = 0;
			m_nCrtUldLayerId = 0;
			m_nUldLayerCount = 20;
			m_dUldDelay = 0;

			for (int i=0; i<6; i++)
			{
				m_dLdPos[i] = 0;
				m_dUldPos[i] = 0;
			}
		}

		void Init()
		{
			m_nCrtLdBoxId = 0;
			m_nCrtLdLayerId = 0;

			m_nCrtUldBoxId = 0;
			m_nCrtUldLayerId = 0;
		}

		int m_nCrtLdBoxId;
		int m_nCrtLdLayerId;
		int m_nLdLayerCount;
		double m_dLdPos[6];

		int m_nLdRePushout;		// 推料没感应到,重复推料次数
		int m_nLdEmptyWarning;		// 连续几层没推到料片报警
		int m_nLdContinue;		// 进料是否3层循环不报警

		int m_nCrtUldBoxId;
		int m_nCrtUldLayerId;
		int m_nUldLayerCount;
		double m_dUldPos[6];
		double m_dUldDelay;		// 出料口推出料延时
	}m_structLdUldParam;

	// 清洗参数
	struct tgCleanParam
	{
		tgCleanParam()
		{
			m_nIfAutoClean	 = 0;
			m_nCleanInterval = 1;		// 点胶时自动清洗间隔
			m_nCleanTimes = 1;
			m_dCleanDownMM = 1;	// 针嘴清洗时,下降高度
		}
		tgPos m_posClean;		// 清洗喷嘴位置
		int m_nCleanInterval;	// 点胶时自动清洗间隔
		int m_nIfAutoClean;		// 点胶过程中是否自动清洗	1:自动清洗		0:关闭自动清洗
		int m_nCleanTimes;		// 清洗次数
		double m_dCleanDownMM;	// 针嘴清洗时,下降高度
	}m_structClean;

	// 测高参数
	struct tgHeightReadParam
	{
		tgHeightReadParam()
		{
			m_nHtReadMode = 0;
			m_dProbeHSpd = 50;	// 探针高速
			m_dProbeLSpd = 25;	// 探针低速
			m_dProbeLHeight = 5;// 低速高度
		}
		// 测高方式，修改后需要重启程序
		int m_nHtReadMode;		// 测高方式		0:激光测高				1:探针测高
		double m_dProbeHSpd;	// 探针高速
		double m_dProbeLSpd;	// 探针低速
		double m_dProbeLHeight;	// 低速高度
	}m_structHeightRead;

	// 试胶参数
	struct tgTestGlueParam
	{
		tgTestGlueParam()
		{
			m_nIfTestGlue = 0;		// 是否启用自动试胶
			m_nGlueLen = 5;			// 胶线长度
			m_nGLueSpace = 2;		// 胶线距离

			m_dWorkVel = 20;		// 试胶线速度

			m_nBdRows = 2;			// 陶瓷板行数
			m_nBdColumns = 2;		// 陶瓷板列数
			m_nCurIndex = 0;		// 陶瓷板已试胶数目
		}
		// 试胶参数
		int		m_nIfTestGlue;		// 是否启用自动试胶
		int		m_nGlueLen;			// 胶线长度
		int		m_nGLueSpace;		// 胶线距离
		int		m_nBdRows;			// 陶瓷板行数
		int		m_nBdColumns;		// 陶瓷板列数
		int		m_nCurIndex;		// 陶瓷板已试胶数目
		double  m_dWorkVel;			// 试胶线速度
		tgPos	m_posTopLeft;		// 陶瓷板左上角坐标
		tgPos	m_posButRight;		// 陶瓷板左上角坐标
	}m_structTestGlue;

	// 胶量报警配置
	struct tgGlueWarning
	{
		tgGlueWarning()
		{
			m_nIfGlueWarning = 0;		// 是否启用胶量报警
			m_nGlueWarningMode = 0;		// 胶量报警模式		0:传感器检测胶量	1:预设好板数	2:预设好过期时间
			m_nGlueWarningPcs = 100;	// 胶量报警:可使用的板数目
			m_nGlueWarningMin = 120;	// 胶量报警:胶水有效时间

			InitGlue();
		}

		void InitGlue()		// 初始化胶水信息,换胶完成调用
		{
			m_tmGlueStTime.TimerStart();
			m_nCurMin = 0;
			m_nCurPcs = 0;
		}

		bool IfGlueUsedOut()	// 判断胶水是否已经过期
		{
			if (0 == m_nIfGlueWarning)			return false;
			if (0 == m_nGlueWarningMode)		return false;
			else if (2 == m_nGlueWarningMode)
			{
				m_nCurMin = (int)m_tmGlueStTime.TimerGetMinute();
				if (m_nCurMin < m_nGlueWarningMin)	return false;
			}
			else if (1 == m_nGlueWarningMode)
			{
				if (m_nCurPcs < m_nGlueWarningPcs)	return false;
			}

			return true;
		}

		int m_nIfGlueWarning;			// 是否启用胶量报警
		int m_nGlueWarningMode;			// 胶量报警模式		0:传感器检测胶量	1:预设好板数	2:预设好过期时间
		int m_nGlueWarningPcs;			// 胶量报警:可使用的板数目
		int m_nGlueWarningMin;			// 胶量报警:胶水有效时间

		int m_nCurPcs;	// 当前胶水已点板数目
		int m_nCurMin;	// 当前胶水已使用时间
		CKFTimer m_tmGlueStTime;	// 胶水的开始时间
	}m_structGlueWarning;

	tgRobotParam()
	{
		m_zSysHeightOffset = 0.08;	// 接触测高的系统偏差
		m_zSafePos		= 0.0;
		m_zContactPos	= 0.0;
		m_zLaserBaseVal = 30.0;
		m_zProbeBaseVal = 0.0;	// 探针测量接触传感器的高度.
		m_zLaserCrossVal= 30;
		m_dBackPress	= 0.0;
		m_nValveType	= 1;
		m_nValveCtrlMode = 1;
		m_dTimeAskPre	= 1;
		m_nWeightAdjDotNum = 20;	// 称重胶滴数
		m_dAvgWeight	= 0.0;		// 单点胶重

		m_zProbeCrossVal = 0.0;		// 探针测量十字线的读数

		m_nAddOn	= 0;	
		m_nFixMode  = 0;

		m_dWasteGlueAlarm = 10;		// 废胶报警重量
		m_dWasteGlueCur = 0;		// 当前废胶盒中胶重

		m_nIfAutoWeight = 0;
		m_nWeightIntervalValue = 30;
		m_nWeightIntervalType = 0;
		
		m_nIfAutoRps = 0;			// 是否启用防固化自动喷胶
		m_dAutoRpsIntervalMin = 3;	// 防固化自动喷胶间隔
		m_nAutoRpsDots = 10;		// 防固化自动喷胶胶点数目
		
		m_nLoadUnloadMode = 0;		// 自动进出料	
		m_nCameraType = 0;			// 相机配置
		m_nWorkMode = 0;			// 工作模式
				
		m_nIfCheckSafeDoor = 1;	// 是否检测安全门

		// 使用的阀数目
		m_nValveNum = 0;		// 0:单阀		1:同步双阀
		m_dValveSpace = 0.0;	// 阀间距离
		// 是否使用底部相机
		m_nIfUseCameraNeedle = 0;	// 0:不使用		1:使用

		// 是否使用底部旋转
		m_nIfRotateWorkpiece = 0;	// 0:不使用		1:使用
	}

	int m_nAddOn;			// 外挂辅助功能  0:翻板机    1:加热			2:无
	int m_nFixMode;			// 物料定位方式  0:仅顶升    1:先侧夹后顶升 2:先顶升后侧夹

	double m_zSysHeightOffset;	// 接触测高的系统偏差
	double m_zSafePos;		// Z轴安全高度,在此高度平移不会有碰撞风险
	double m_zContactPos;	// 针头刚好碰到接触传感器时的Z轴高度.
	double m_zLaserBaseVal;	// 激光测量接触传感器的高度读数.
	double m_zProbeBaseVal;	// 探针测量接触传感器的高度.

	tgPos m_posWeightAdj;	// 胶量校正位置
	tgPos m_posContactTest;	// 接触测高位置
	tgPos m_posLaserBasePos;// 激光测量接触传感器高度位置
	tgPos m_posProbeBasePos;// 探针测量接触传感器高度位置

	tgPos m_posCamereCross;	// 镜头观察十字线位置
	double m_zProbeCrossVal;// 探针测量十字线的读数
	tgPos m_posProbeCross;	// 探针针头在十字线中心的位置
	double m_zLaserCrossVal;// 激光测量十字线的读数
	tgPos m_posLaserCross;	// 激光光斑在十字线中心的位置
	tgPos m_posJetAdjust;	// 4点校正位置

	int m_nValveType;		// 当前使用的点胶阀类型			0:Vemers	1:kaifa		2:musashi		3:yikai
	int m_nValveCtrlMode;	// 点胶阀控制模式
	double m_dBackPress;	// 胶桶背压

	int m_nWeightAdjDotNum;	// 称重胶滴数
	double m_dAvgWeight;	// 单点胶重

	double m_dTimeAskPre;	// 向上要板时间

	double m_dWasteGlueAlarm;	// 废胶报警重量
	double m_dWasteGlueCur;		// 当前废胶盒中胶重

	int m_nIfAutoWeight;		// 是否自动称重
	int m_nWeightIntervalType;	// 自动称重间隔类型(0:时间		1:板数)
	int m_nWeightIntervalValue;	// 自动称重间隔(时间:分钟		板数:pcs)

	int m_nIfAutoRps;				// 是否启用防固化自动喷胶
	double m_dAutoRpsIntervalMin;	// 防固化自动喷胶间隔
	int m_nAutoRpsDots;				// 防固化自动喷胶胶点数目
		
	// 进出料方式，修改后需要重启程序
	int m_nLoadUnloadMode;	// 进出料方式	0:在线式自动进出料		1:离线式3层料盒
	
	// 相机配置
	int m_nCameraType;		// 相机			0:Dalsa					1:HKVision
	// 工作模式
	int m_nWorkMode;		// 0:喷射阀		1:针头					2:相机
	// 是否检测安全门
	int m_nIfCheckSafeDoor;	// 0:不检测		1:检测

	// 使用的阀数目
	int m_nValveNum;		// 0:单阀		1:同步双阀
	
	tgPos m_posCCDValve1;	// 下相机阀1位置
	double m_dValveSpace;	// 阀间距离
	// 是否使用底部相机
	int m_nIfUseCameraNeedle;	// 0:不使用		1:使用

	// 是否使用底部旋转
	int m_nIfRotateWorkpiece;	// 0:不使用		1:使用
	tgPos m_posRotateMark1;		// 工件调平Mark1位置
	tgPos m_posRotateMark2;		// 工件调平Mark2位置
};

struct tgSysParam
{
	tgSysParam()
	{
		strcpy(szApp, "HD100 System");
		nLanguageType =0;
		bBalanceEnable = 0;
		dPixelScale = 0.005839;
		bLoadSysEnaLoad= FALSE;
		dPixelAngle = 0;
		m_patCaliAccepatance= 60;	
		m_BoardCaliAccA=70;      // 基板夹角模板A相似度
		m_BoardCaliAccB=72;      // 基板夹角模板B相似度
		dBCDispTime =5;			//称重测试 出胶时间
		dBCDispDis = 5;			//称重测试出胶间隔 
		dBCDispNum = 5;			// 称重测试出胶次数
		dBCMGPorDOT = 0.0;
		snapSecond = 0; 
		lRunCounter = 0;
		iLookNum = 100;
		dLookAcc = 3500;//常规设置0.35g
		dLookMaxAcc = 20000;//最大2g	
		nAutoMode = 1;//在线
		uiWashPinCycle=1;
		iWashNum =1;
		iWashType =0;
		dDrawCircleR1 = 1.0;
		dDrawCircleR2 = 0;
		iMarkNum = 2;
		m_ccdMoveCnt = 4;
		m_ccdMoveDis = 3;

		dCaliNeedleDelta = 0;
		dCaliJetDelta = 0;

		dSpeedMaxJogXY = 200;
		dSpeedMaxJogZ = 200;
		dSpeedMaxJogU =100;
		dSpeedMaxJogVW= 200;
		dSpeedMaxJogMN= 200;
		dSpeedMoveXY = 200;
		dSpeedMoveZ = 100;
		dSpeedMoveU = 50;
		dSpeedMovetoXY = 200;
		dSpeedMovetoZ = 100;
		dSpeedMovetoU = 50;
		dSpeedImageMoveXY = 200;
		dSpeedImageMoveZ = 50;
		dSpeedDisp = 100;
		dSpeedHomeL = 20;
		dSpeedHomeH1 = 100;
		dSpeedHomeH2 = 50;
		dSpeedBelt = 200;
		dBeltUnloadDelay = 30;
		dSpeedMaxJogVW = 50;
		dSpeedMoveVW = 50;

		nToolType = 0;   // 图像模式
		nGoStandByPos=1; // 回加工待机位

		nJetPulse = 1;
		nOpenTime = 20;
		nCloseTime = 20;
		dPressJet = 3;
		dPressDisp = 3;
		dJetCurTemprature = 0;
		dJetSetTemperature = 25;

		iXDir = 0;
		iYDir = 0;
		JetDistanceXY = 10;
		nByPassFlag = 0;

		dNeedleAdjustDispTime = 0.1;
		dNeedleAdjustWaitTime = 0.1;
		dNeedleAdjustUpHeight = 0.5;

		dCaliNeedleStdHeight = 0;
		dCaliJetStdHeight = 0;
		m_LookTunScale  = 8;    // 前瞻拐弯降速比
		m_LookEvenTime = 10;    // 缓冲匀速段时间
		nRobotCtrlMode = 0;
		nCCDType =1;
//		nWorkMode=0;
		dJetOpenDelay = 0.4;
		nFileSeparaEnable=0;   // 文件分版加工使能
		nFileScrapeEnable=0;   // 文件拼凑加工使能
		nFileScrapeNum=4;      // 文件拼凑加工个数
		nSFDoorEnable = FALSE;
		nLaserReadPortID =0;
		nFindMarkJogMove=0;
		m_BoardArc = 0.0;
		dAutoPRSdelay =5;
		dPRSTimes=3;
		dSPRSTime=20;
		dSPRSDelay=10;
		dSpeedFeedUpVel=20;   // 接驳台上升速度
		dSpeedFeedDownVel=20; // 接驳台下降速度
		dJetZBaseHightValue = 5; // Z轴基准面高度值
		bEnablePrecFun = FALSE;
		dWeightLmtMax=0;
		dWeightLmtMin=0;
		bEableWeightAlm = TRUE;

	}

	char szApp[255];
	int nRobotCtrlMode;			// 机械臂控制模式 0:脉冲模式 1：模拟量(速度)
	int nCCDType;				// CCD类型 0 - 黑白相机， 1- 彩色相机 
	system_PID  PIDParam[3];	// PID 参数
	int  nLanguageType;			// 0：中文   1：英文  
	int  bBalanceEnable;		// 0：不使用 1: 天平使能
//	int  nWorkMode;				// 喷胶工作模式 0-普通模式 1-飞行喷射模式

	int nFindMarkJogMove;		// mark 失败以后是否手动查找

	double dJetOpenDelay;		// 喷射阀开关延时（ms）

	tgPos tOffset[TOOL_MAXNUM];   // 0 喷射与图像偏差  1针头与图像偏差 2图像与图像偏差=0
	tgPos tNeedleAdjustPos[2];    // 参考点----针头中心XY校正点
	//  天平称重 参数　
	tgPos  tBCTestPos;     // 天平称重位置
	double dBCDispTime;   // 称重测试出胶时间
	double dBCDispDis;    // 称重测试出胶间隔 
	double dBCDispNum;    // 称重测试出胶次数
	double dBCMGPorDOT;   // 单点重量 mg

	tgPos tDefHomePos;    // 自定义原点位置     
	tgPos tAutoPfsPos;     // 自动清喷位置
	double dAutoPRSdelay;  // 自动清喷间隔
	double dPRSTimes;      // 淸喷次数
	double dSPRSTime;      // 单次淸喷开阀时间  ms
	double dSPRSDelay;     // 单次淸喷间隔时间 ms
	tgPos tNeedleZAdjustPos;     // 针头Z向校正位置
	tgPos tJetZAdjustPos;        // 喷射阀Z方向校准位置

	tgPos tPixelAdjustPos;       // 模板标定位置
	double m_patCaliAccepatance; // 标定模板相似度。

	tgPos  tBoardCalib_A;        // 基板夹角标定A坐标位置
	tgPos  tBoardCalib_B;        // 基板夹角标定B坐标位置
	double m_BoardCaliAccA;      // 基板夹角模板A相似度
	double m_BoardCaliAccB;      // 基板夹角模板B相似度 
	double m_BoardArc;           // 基板夹角 

	int   JetDistanceXY;

	tgPos tDispTestPos;           // 试胶位置

	double dNeedleAdjustDispTime;//针头图像偏差校正出胶时间
	double dNeedleAdjustWaitTime;//针头图像偏差校正等待时间
	double dNeedleAdjustUpHeight;//针头图像偏差校正提高高度
	////////////////////////////////////
	double dPixelScale;    // 机械坐标与图像坐标比例系数 
	double dPixelAngle;
	int    nToolType;      // 0: 喷射模式，1：针头模式 2 图像模式
	int    nGoStandByPos;  // 0: 自动加工完不回加工待机点 1：自动加工完回加工待机点
//	int    nJetType;       // 0: 气动式喷射阀  1：压电式喷射阀   add by eddie.cheung 4.29    
	long   nJetPulse;      //  出胶次数
	int    nOpenTime;      // 喷射出胶时间  （对于EFD 阀，数据要大于打点模式下循环周期 ）
	int    nCloseTime;     // 喷射间隔周期  （对于EFD 阀，用于两个出胶周期之间的间隔）
	double dPressJet;      // 0.15-6bar   胶桶压力
	double dPressDisp;     // 0.15-6bar   阀体压力(不需要此项的或复用为针筒喷射模式下的胶压控制）
	double dJetSetTemperature;    // 喷射阀设置温度
	double dJetCurTemprature;     // 喷射阀当前温度
	////////////////////////////////////
	BOOL   bLoadSysEnaLoad;                  // 上下料机构使能  

	double dSpeedMaxJogXY;   
	double dSpeedMaxJogZ;
	double dSpeedMaxJogU;
	double dSpeedMaxJogVW;
	double dSpeedMaxJogMN;

	double dSpeedMoveXY;
	double dSpeedMoveZ;
	double dSpeedMoveU;
	double dSpeedMoveVW;
	double dSpeedMoveMN;

	double dSpeedMovetoXY;  
	double dSpeedMovetoZ;
	double dSpeedMovetoU;
	double dSpeedMovetoVW;
	double dSpeedMovetoMN;

	double dSpeedHomeLoadH;
	double dSpeedHomeLoadM;
	double dSpeedHomeLoadL;
	double dSpeedImageMoveXY;    // 移动到模板查找位速度
	double dSpeedImageMoveZ;
	double dSpeedDisp;
	double dSpeedHomeH1;//机械臂回原点速度
	double dSpeedHomeH2;//针筒电机回原点速度
	double dSpeedHomeL;//搜寻到home开关后低速
	double dSpeedBelt;
	double dBeltUnloadDelay;   // 出料区加热延时 单位：0.1s
	double dCaliNeedleStdHeight;
	double dCaliJetStdHeight;

	double dCaliNeedleDelta;
	double dCaliJetDelta;

	long lRunCounter;     // 运行次数
	int uiWashPinCycle;   // 自动运行时，自动清洗加工工件间隔个数
	int nAutoMode;        // 在线模式
	int iWashNum;         // 每次清洗喷头次数
	int iWashType;        // 清洗类型 0-擦拭式清洗 1-单吸式清洗
	int nByPassFlag;

	double dDrawCircleR1;//
	int dDrawCircleR2;//
	int iLookNum;//200
	double dLookMaxAcc;
	double dLookAcc;

	double  m_LookTunScale;     // 前瞻拐弯降速比
	int     m_LookEvenTime;     // 缓冲匀速段时间

	int iYDir;
	int iXDir;

	double m_ccdMoveDis;        // Mark点查找微动距离
	int m_ccdMoveCnt;    
	int iMarkNum;

	int nFileSeparaEnable;  // 文件分版加工使能
	int nFileScrapeEnable;  // 文件拼凑加工使能
	int nFileScrapeNum;     // 文件拼凑加工个数

	//周期时间定义
	union   { 
		FILETIME   ft; 
		LONGLONG   val; 
	}t1,t2; 

	CTime m_SystemBeginTime;  // 系统软件启动时间
	double snapSecond;        // 单次运行时间

	BOOL  nSFDoorEnable;      // 安全门功能使能

	int  nLaserReadPortID;    // 激光测高模拟量端口
	double dSpeedFeedUpVel;   // 接驳台上升速度
	double dSpeedFeedDownVel; // 接驳台下降速度
	double dJetZBaseHightValue; // Z轴基准面高度值
	BOOL   bEnablePrecFun;       // 使能精度补偿。
	double dWeightLmtMax;
	double dWeightLmtMin;
	BOOL   bEableWeightAlm;
};

struct tgCmdParamLine
{
	tgCmdParamLine()
	{	
		reset();
	}
	void reset()
	{	
		dVel = 50;
		dPreTime = 0;
		dNodeTime = 0;
		dNodeVel = 30;//No used
		dTailCloseLen = 5;
		dTailTime = 0;

		dEndLen = 0;
		dEndZupHigh = 0;
		dEndVel = 40;
		iEndType = 0;
		dPreMoveTime=0;
		dPreMoveD=0.10;
		dPresclae=0.004;
	}
	//线段点胶速度
	double dVel;

	//线段点胶参数：前延时间，过渡点暂停时间，线段速度，提前关胶长度
	double dPreTime, dNodeTime, dTailTime,dNodeVel, dTailCloseLen;

	//线段结束参数：收胶长度，收胶高度，收胶动作速度
	double dEndLen, dEndZupHigh, dEndVel;
	int iEndType;//点胶结束动作类型

	double dPreMoveTime;//运动后延时出胶

	double  dPreMoveD;//提前开胶距离
	double  dPresclae;//系数

};

struct tgCmdParamPoint
{
	tgCmdParamPoint()
	{	reset();
	}
	void reset()
	{
		dPreTime =0;
		dDispTime = 0.003;
		dTailTime = 0;
		dZupHigh = 0;
		iZupType = 0;
		dZupHVel = 100;
		dZupLVel = 20;

	}
	//单点点胶参数:前延时间，点胶时间，点胶结束暂停时间
	double dPreTime, dDispTime, dTailTime;

	//Z提升高度,
	double dZupHigh;
	int iZupType; //提升模式0:相对高度, 1,绝对高度

	//提升速度
	double dZupHVel;
	double dZupLVel;
	int DdispOn_T;//开阀时间
	int DdispClose_T;//关阀时间
	int DispCount;//开阀次数
	int DispUseWParam;

};
struct tgCmdParamHPoint
{
	tgCmdParamHPoint()
	{	reset();
	}
	void reset()
	{
		dPreTime =0;
		dDispTime = 0.003;
		dTailTime = 0;
		dZupHigh = 0;
		iZupType = 0;
		dZupHVel = 100;
		dZupLVel = 20;

	}
	//单点点胶参数:前延时间，点胶时间，点胶结束暂停时间
	double dPreTime, dDispTime, dTailTime;

	//Z提升高度,
	double dZupHigh;
	int iZupType; //提升模式0:相对高度, 1,绝对高度

	//提升速度
	double dZupHVel;
	double dZupLVel;
	int DdispOn_T;//开阀时间
	int DdispClose_T;//关阀时间
	int DispCount;//开阀次数
	int DispUseWParam;

};
struct tgCmdParam
{//命令行参数结构体
	tgCmdParam()
	{	
		reset();
	}

	void reset()
	{
		tLine.reset();
		tPoint.reset();

		dVelXY = 1500;
		dVelZ  = 50;
		dVelA=60;
		//dZLimit = 100;
		m_DummySpeed = 200;

		dZcleaType = 0;//绝对
		dZclearance = 0;//默认无效

		dZSetType = 0;
		dZSetHeight= 0;

		dAccXY = 15;//mm/s*s
		dAccZ= 15;//mm/s*s

		nLowTime = 20;
		nHighTime = 20;

		lPulseNum = 0;
		dBoardArcForRepeat = 0.0;
	}
	tgCmdParamLine tLine;
	tgCmdParamPoint tPoint;
	tgCmdParamHPoint tHPoint;
	double dBoardArcForRepeat;
	//空跑速度
	double dVelXY,dVelZ;
	double dVelA;
	double dAccXY, dAccZ;

	//Z轴提高高度类型 相对，绝对 ，位置 
	double dZcleaType;//==0绝对 1==相对
	double dZclearance;

	double dZSetType;
	double dZSetHeight;   // Z轴工作高度

	//空点移动速度
	double m_DummySpeed;
	//double dZLimit;

	int nLowTime;
	int nHighTime;
	long lPulseNum;
};

//////////////////////////////////////////////////////////////////////////
//一条指令一行
//命令以%开始, 类型, 坐标点组数(每组包括xyzu),浮点参数个数(先坐标点接着是其它浮点参数),整型参数个数, 浮点数F, 正数D, *%结束
//% , TYPE ,CN, N1, N2, Value1, Value2 , .. *%
struct tgCmdLine
{
	tgCmdLine()
	{
		tType = CMD_NONE;
		memset(pIntValue, 0, sizeof(int)*COMMAND_INT_LEN);
		memset(pDblValue, 0, sizeof(double)*COMMAND_DOUBLE_LEN);
		strcpy(pszNote," ");
		iDblNum = 0;
		iIntNum = 0;
		iDotNum = 0;
	}

	void Clear()
	{//谨慎使用
		tType = CMD_NONE;
		memset(pIntValue, 0, sizeof(int)*COMMAND_INT_LEN);
		memset(pDblValue, 0, sizeof(double)*COMMAND_DOUBLE_LEN);
		strcpy(pszNote,"");
		iDblNum = 0;
		iIntNum = 0;
		iDotNum = 0;
	}
public:
	CMD_TYPE tType;

	int pIntValue[COMMAND_INT_LEN];
	double pDblValue[COMMAND_DOUBLE_LEN];
	char pszNote[255];

	int iIntNum;
	int iDblNum;
	int iDotNum;      //坐标点组个数	
};
#endif
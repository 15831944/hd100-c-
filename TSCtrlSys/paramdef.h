#ifndef TK_PARAM_DEFINE_H_
#define TK_PARAM_DEFINE_H_


extern CString szCmdline[];

#define TOOL_NUM 2    //ĩ�˹�������
#define TOOL_MAXNUM		3

#define COMMAND_DOUBLE_LEN  20 //����ָ���е�double����
#define COMMAND_INT_LEN     10 //����ָ���е�int����

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
	double dx, dy;//��ʼ��
};

struct tgBrush
{
	double x,y,z,u;				// ���
	double dx,dy;				// ���β�������
	double dfCx[2],dfCy[2];		// ��ԲԲ��
	int nDir;					// ���η��� 0 = x ,1 = y
	int nNum;	// �켣����
	double dRadius;
	double dAngle;
	double dStep;

};
struct tgArc
{
	double dx[3], dy[3],dz[3];//��㣬�м�㣬 �յ�
	double x, y;//Բ��
	double r;
	double angle;//[-360~360]
	double dStartAngle,dEndAngle;//[-360~360]
	int iType;//0:�ӻ���1:�Ż�
	//
	BOOL bAddDir; //TRUE��������ת��FALSE:������ת
	BOOL bBigArc;//TRUE���ӻ���FALSE���Ż�
};
//////////////////////////////////////////////////////////////////////////
//��������
enum CMD_TYPE
{
	CMD_NONE=0,
	CMD_MARK,			// Mark���� ,tgPos[2],int iMarkNum
	CMD_DUMMY,			// �趨�㽺��ͷʵ�����ڵ�XYZ ����Ϊ�յ㣬����¼��Ŀǰ�ļ�¼��λ�á�ͨ��Ӧ���ڱܿ��������赲��
	CMD_POINT,			// ����㽺,tgPos[1]
	CMD_LINE_START,		// �߶ε㽺���tgPos[1]
	CMD_LINE_PASSING,	// �߶ε㽺�м��tgPos[1]
	CMD_LINE_ARC,		// Բ���㽺 tgPos[1]
	CMD_LINE_END,		// �߶ε㽺������,tgPos[1]
	CMD_CIRCLE,			// Բ�ε㽺 1:Բ��+ֱ��, tgPos[1], double diameter, 2:3�㻭Բ,tgPos[3]
	CMD_DISPENSER,		// �㽺����/��   BOOL On/Off
	CMD_GOTO_ADDRESS,	// ��תָ��   int iIndex
	CMD_REPEATX,		// X��������  int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	CMD_REPEATY,		// Y��������  int iIndex, int stepType(S/N), int iXNum, int iYNum,  double xSpace, double ySpace
	CMD_BRUSH_AREA,		// Ϳ��,
	CMD_CALL_SUB,       // ���Ӻ���
	CMD_CALL_PRO,       // �����ļ�
	CMD_LABEL,			// ���ó���
	CMD_NULL_LINE,		// ����ָ��
	CMD_SET_IO,
	CMD_DELAY,
	CMD_WAIT_POINT,     // ������
	CMD_PAUSE,			// ��ͷ���������ض�λ�ò���ֹͣ�ƶ���ֱ��ʹ���߰��»�̨����ϵ�START��ť���������ִ�м�¼��
	CMD_HOME,			// X, Y, Z�Ὣ�ع����ԭ��(�ȴ���)���������ִ�м�¼�㡣
	CMD_LOOP,			// ͨ����ѭ��ִ�е�ַ��Ϊ1�����̵�һ�������ɺ󣬿����ô˹������趨��������
	CMD_DUNNY_SPEED,     
	CMD_NOTE,
	CMD_BATH,           // ��ϴָ��
	CMD_ZMETER,      	// �����߶�,
	CMD_POT,            // ���ش��
	CMD_WCL,            // ���ػ���
	CMD_RECT,   
	CMD_TOOL,
	CMD_REPEATX_ZH,
	CMD_REPEATY_ZH,
	CMD_OMRON,
	CMD_BACK5,
	CMD_END_PROGRAM,		//�����������

	//////////////////////////////////////////////////////////////////////////
	PARAM_LINE_SPEED,		//�߶ε㽺�ٶ�
	PARAM_LINE_SETUP,		//�߶ε㽺��������
	PARAM_LINE_END,			//�߶ε㽺��������
	PARAM_POINT_SETUP,		//����㽺��������
	PARAM_POINT_END,		//����㽺��������
	PARAM_Z_CLEARANCE,		//Z����߸߶����� 
	PARAM_Z_LIMIT,			//Z�Ἣ��ֵ���� 
	PARAM_XY_VEL,			//XY������ƶ��ٶ�
	PARAM_Z_VEL,			//Z������ƶ��ٶ�
	PARAM_HOME_POSITION,	//����RUNģʽʱ�㽺������ͷͣ��λ��(�ȴ���)
	PARAM_SET_Z_SAME,		//���������Ժ�������Z��߶���Ϊ��ͬ��ֵ
	PARAM_Z_SET_HEIGHT,		//����Z��߶�
	PARAM_JET_SETUP,        //ѡ�񷧲�������  
	CMD_WCRECT,             //���ؾ���
	///////////////////////////////////////////////

	CMD_SUPERSTART,         //�ɽ����
	CMD_SUPERPOINT,         //�ɽ�����
	CMD_SUPEREND,           //�ɽ�������
	//////////////////////////////////////////////
	CMD_FILEDELIMITER,      // �Խ̷ֶη�
	PARAM_Z_WORKPOSSET,     // Z��߶�����
	CMD_FILESCRPEWORK,      // ƴ��ӹ���
	CMD_REPEATARC,          // ���мнǲ���

	CMD_ReverseBoard,		// ����ָ��
	CMD_SUPERDOT,			// ���ٴ��ָ��
	CMD_LABEL_END,			// �ӳ������
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

struct tgRobotParam		// ��������
{
	// �����ϲ���
	struct tgLdUldParam
	{
		tgLdUldParam()
		{
			m_nCrtLdBoxId = 0;
			m_nCrtLdLayerId = 0;
			m_nLdLayerCount = 20;
			m_nLdRePushout = 1;		// ����û��Ӧ��,�ظ����ϴ���
			m_nLdEmptyWarning = 1;		// ��������û�Ƶ���Ƭ����
			m_nLdContinue = 0;		// �����Ƿ�3��ѭ��������

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

		int m_nLdRePushout;		// ����û��Ӧ��,�ظ����ϴ���
		int m_nLdEmptyWarning;		// ��������û�Ƶ���Ƭ����
		int m_nLdContinue;		// �����Ƿ�3��ѭ��������

		int m_nCrtUldBoxId;
		int m_nCrtUldLayerId;
		int m_nUldLayerCount;
		double m_dUldPos[6];
		double m_dUldDelay;		// ���Ͽ��Ƴ�����ʱ
	}m_structLdUldParam;

	// ��ϴ����
	struct tgCleanParam
	{
		tgCleanParam()
		{
			m_nIfAutoClean	 = 0;
			m_nCleanInterval = 1;		// �㽺ʱ�Զ���ϴ���
			m_nCleanTimes = 1;
			m_dCleanDownMM = 1;	// ������ϴʱ,�½��߶�
		}
		tgPos m_posClean;		// ��ϴ����λ��
		int m_nCleanInterval;	// �㽺ʱ�Զ���ϴ���
		int m_nIfAutoClean;		// �㽺�������Ƿ��Զ���ϴ	1:�Զ���ϴ		0:�ر��Զ���ϴ
		int m_nCleanTimes;		// ��ϴ����
		double m_dCleanDownMM;	// ������ϴʱ,�½��߶�
	}m_structClean;

	// ��߲���
	struct tgHeightReadParam
	{
		tgHeightReadParam()
		{
			m_nHtReadMode = 0;
			m_dProbeHSpd = 50;	// ̽�����
			m_dProbeLSpd = 25;	// ̽�����
			m_dProbeLHeight = 5;// ���ٸ߶�
		}
		// ��߷�ʽ���޸ĺ���Ҫ��������
		int m_nHtReadMode;		// ��߷�ʽ		0:������				1:̽����
		double m_dProbeHSpd;	// ̽�����
		double m_dProbeLSpd;	// ̽�����
		double m_dProbeLHeight;	// ���ٸ߶�
	}m_structHeightRead;

	// �Խ�����
	struct tgTestGlueParam
	{
		tgTestGlueParam()
		{
			m_nIfTestGlue = 0;		// �Ƿ������Զ��Խ�
			m_nGlueLen = 5;			// ���߳���
			m_nGLueSpace = 2;		// ���߾���

			m_dWorkVel = 20;		// �Խ����ٶ�

			m_nBdRows = 2;			// �մɰ�����
			m_nBdColumns = 2;		// �մɰ�����
			m_nCurIndex = 0;		// �մɰ����Խ���Ŀ
		}
		// �Խ�����
		int		m_nIfTestGlue;		// �Ƿ������Զ��Խ�
		int		m_nGlueLen;			// ���߳���
		int		m_nGLueSpace;		// ���߾���
		int		m_nBdRows;			// �մɰ�����
		int		m_nBdColumns;		// �մɰ�����
		int		m_nCurIndex;		// �մɰ����Խ���Ŀ
		double  m_dWorkVel;			// �Խ����ٶ�
		tgPos	m_posTopLeft;		// �մɰ����Ͻ�����
		tgPos	m_posButRight;		// �մɰ����Ͻ�����
	}m_structTestGlue;

	// ������������
	struct tgGlueWarning
	{
		tgGlueWarning()
		{
			m_nIfGlueWarning = 0;		// �Ƿ����ý�������
			m_nGlueWarningMode = 0;		// ��������ģʽ		0:��������⽺��	1:Ԥ��ð���	2:Ԥ��ù���ʱ��
			m_nGlueWarningPcs = 100;	// ��������:��ʹ�õİ���Ŀ
			m_nGlueWarningMin = 120;	// ��������:��ˮ��Чʱ��

			InitGlue();
		}

		void InitGlue()		// ��ʼ����ˮ��Ϣ,������ɵ���
		{
			m_tmGlueStTime.TimerStart();
			m_nCurMin = 0;
			m_nCurPcs = 0;
		}

		bool IfGlueUsedOut()	// �жϽ�ˮ�Ƿ��Ѿ�����
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

		int m_nIfGlueWarning;			// �Ƿ����ý�������
		int m_nGlueWarningMode;			// ��������ģʽ		0:��������⽺��	1:Ԥ��ð���	2:Ԥ��ù���ʱ��
		int m_nGlueWarningPcs;			// ��������:��ʹ�õİ���Ŀ
		int m_nGlueWarningMin;			// ��������:��ˮ��Чʱ��

		int m_nCurPcs;	// ��ǰ��ˮ�ѵ����Ŀ
		int m_nCurMin;	// ��ǰ��ˮ��ʹ��ʱ��
		CKFTimer m_tmGlueStTime;	// ��ˮ�Ŀ�ʼʱ��
	}m_structGlueWarning;

	tgRobotParam()
	{
		m_zSysHeightOffset = 0.08;	// �Ӵ���ߵ�ϵͳƫ��
		m_zSafePos		= 0.0;
		m_zContactPos	= 0.0;
		m_zLaserBaseVal = 30.0;
		m_zProbeBaseVal = 0.0;	// ̽������Ӵ��������ĸ߶�.
		m_zLaserCrossVal= 30;
		m_dBackPress	= 0.0;
		m_nValveType	= 1;
		m_nValveCtrlMode = 1;
		m_dTimeAskPre	= 1;
		m_nWeightAdjDotNum = 20;	// ���ؽ�����
		m_dAvgWeight	= 0.0;		// ���㽺��

		m_zProbeCrossVal = 0.0;		// ̽�����ʮ���ߵĶ���

		m_nAddOn	= 0;	
		m_nFixMode  = 0;

		m_dWasteGlueAlarm = 10;		// �Ͻ���������
		m_dWasteGlueCur = 0;		// ��ǰ�Ͻ����н���

		m_nIfAutoWeight = 0;
		m_nWeightIntervalValue = 30;
		m_nWeightIntervalType = 0;
		
		m_nIfAutoRps = 0;			// �Ƿ����÷��̻��Զ��罺
		m_dAutoRpsIntervalMin = 3;	// ���̻��Զ��罺���
		m_nAutoRpsDots = 10;		// ���̻��Զ��罺������Ŀ
		
		m_nLoadUnloadMode = 0;		// �Զ�������	
		m_nCameraType = 0;			// �������
		m_nWorkMode = 0;			// ����ģʽ
				
		m_nIfCheckSafeDoor = 1;	// �Ƿ��ⰲȫ��

		// ʹ�õķ���Ŀ
		m_nValveNum = 0;		// 0:����		1:ͬ��˫��
		m_dValveSpace = 0.0;	// �������
		// �Ƿ�ʹ�õײ����
		m_nIfUseCameraNeedle = 0;	// 0:��ʹ��		1:ʹ��

		// �Ƿ�ʹ�õײ���ת
		m_nIfRotateWorkpiece = 0;	// 0:��ʹ��		1:ʹ��
	}

	int m_nAddOn;			// ��Ҹ�������  0:�����    1:����			2:��
	int m_nFixMode;			// ���϶�λ��ʽ  0:������    1:�Ȳ�к��� 2:�ȶ�������

	double m_zSysHeightOffset;	// �Ӵ���ߵ�ϵͳƫ��
	double m_zSafePos;		// Z�ᰲȫ�߶�,�ڴ˸߶�ƽ�Ʋ�������ײ����
	double m_zContactPos;	// ��ͷ�պ������Ӵ�������ʱ��Z��߶�.
	double m_zLaserBaseVal;	// ��������Ӵ��������ĸ߶ȶ���.
	double m_zProbeBaseVal;	// ̽������Ӵ��������ĸ߶�.

	tgPos m_posWeightAdj;	// ����У��λ��
	tgPos m_posContactTest;	// �Ӵ����λ��
	tgPos m_posLaserBasePos;// ��������Ӵ��������߶�λ��
	tgPos m_posProbeBasePos;// ̽������Ӵ��������߶�λ��

	tgPos m_posCamereCross;	// ��ͷ�۲�ʮ����λ��
	double m_zProbeCrossVal;// ̽�����ʮ���ߵĶ���
	tgPos m_posProbeCross;	// ̽����ͷ��ʮ�������ĵ�λ��
	double m_zLaserCrossVal;// �������ʮ���ߵĶ���
	tgPos m_posLaserCross;	// ��������ʮ�������ĵ�λ��
	tgPos m_posJetAdjust;	// 4��У��λ��

	int m_nValveType;		// ��ǰʹ�õĵ㽺������			0:Vemers	1:kaifa		2:musashi		3:yikai
	int m_nValveCtrlMode;	// �㽺������ģʽ
	double m_dBackPress;	// ��Ͱ��ѹ

	int m_nWeightAdjDotNum;	// ���ؽ�����
	double m_dAvgWeight;	// ���㽺��

	double m_dTimeAskPre;	// ����Ҫ��ʱ��

	double m_dWasteGlueAlarm;	// �Ͻ���������
	double m_dWasteGlueCur;		// ��ǰ�Ͻ����н���

	int m_nIfAutoWeight;		// �Ƿ��Զ�����
	int m_nWeightIntervalType;	// �Զ����ؼ������(0:ʱ��		1:����)
	int m_nWeightIntervalValue;	// �Զ����ؼ��(ʱ��:����		����:pcs)

	int m_nIfAutoRps;				// �Ƿ����÷��̻��Զ��罺
	double m_dAutoRpsIntervalMin;	// ���̻��Զ��罺���
	int m_nAutoRpsDots;				// ���̻��Զ��罺������Ŀ
		
	// �����Ϸ�ʽ���޸ĺ���Ҫ��������
	int m_nLoadUnloadMode;	// �����Ϸ�ʽ	0:����ʽ�Զ�������		1:����ʽ3���Ϻ�
	
	// �������
	int m_nCameraType;		// ���			0:Dalsa					1:HKVision
	// ����ģʽ
	int m_nWorkMode;		// 0:���䷧		1:��ͷ					2:���
	// �Ƿ��ⰲȫ��
	int m_nIfCheckSafeDoor;	// 0:�����		1:���

	// ʹ�õķ���Ŀ
	int m_nValveNum;		// 0:����		1:ͬ��˫��
	
	tgPos m_posCCDValve1;	// �������1λ��
	double m_dValveSpace;	// �������
	// �Ƿ�ʹ�õײ����
	int m_nIfUseCameraNeedle;	// 0:��ʹ��		1:ʹ��

	// �Ƿ�ʹ�õײ���ת
	int m_nIfRotateWorkpiece;	// 0:��ʹ��		1:ʹ��
	tgPos m_posRotateMark1;		// ������ƽMark1λ��
	tgPos m_posRotateMark2;		// ������ƽMark2λ��
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
		m_BoardCaliAccA=70;      // ����н�ģ��A���ƶ�
		m_BoardCaliAccB=72;      // ����н�ģ��B���ƶ�
		dBCDispTime =5;			//���ز��� ����ʱ��
		dBCDispDis = 5;			//���ز��Գ������ 
		dBCDispNum = 5;			// ���ز��Գ�������
		dBCMGPorDOT = 0.0;
		snapSecond = 0; 
		lRunCounter = 0;
		iLookNum = 100;
		dLookAcc = 3500;//��������0.35g
		dLookMaxAcc = 20000;//���2g	
		nAutoMode = 1;//����
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

		nToolType = 0;   // ͼ��ģʽ
		nGoStandByPos=1; // �ؼӹ�����λ

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
		m_LookTunScale  = 8;    // ǰհ���併�ٱ�
		m_LookEvenTime = 10;    // �������ٶ�ʱ��
		nRobotCtrlMode = 0;
		nCCDType =1;
//		nWorkMode=0;
		dJetOpenDelay = 0.4;
		nFileSeparaEnable=0;   // �ļ��ְ�ӹ�ʹ��
		nFileScrapeEnable=0;   // �ļ�ƴ�ռӹ�ʹ��
		nFileScrapeNum=4;      // �ļ�ƴ�ռӹ�����
		nSFDoorEnable = FALSE;
		nLaserReadPortID =0;
		nFindMarkJogMove=0;
		m_BoardArc = 0.0;
		dAutoPRSdelay =5;
		dPRSTimes=3;
		dSPRSTime=20;
		dSPRSDelay=10;
		dSpeedFeedUpVel=20;   // �Ӳ�̨�����ٶ�
		dSpeedFeedDownVel=20; // �Ӳ�̨�½��ٶ�
		dJetZBaseHightValue = 5; // Z���׼��߶�ֵ
		bEnablePrecFun = FALSE;
		dWeightLmtMax=0;
		dWeightLmtMin=0;
		bEableWeightAlm = TRUE;

	}

	char szApp[255];
	int nRobotCtrlMode;			// ��е�ۿ���ģʽ 0:����ģʽ 1��ģ����(�ٶ�)
	int nCCDType;				// CCD���� 0 - �ڰ������ 1- ��ɫ��� 
	system_PID  PIDParam[3];	// PID ����
	int  nLanguageType;			// 0������   1��Ӣ��  
	int  bBalanceEnable;		// 0����ʹ�� 1: ��ƽʹ��
//	int  nWorkMode;				// �罺����ģʽ 0-��ͨģʽ 1-��������ģʽ

	int nFindMarkJogMove;		// mark ʧ���Ժ��Ƿ��ֶ�����

	double dJetOpenDelay;		// ���䷧������ʱ��ms��

	tgPos tOffset[TOOL_MAXNUM];   // 0 ������ͼ��ƫ��  1��ͷ��ͼ��ƫ�� 2ͼ����ͼ��ƫ��=0
	tgPos tNeedleAdjustPos[2];    // �ο���----��ͷ����XYУ����
	//  ��ƽ���� ������
	tgPos  tBCTestPos;     // ��ƽ����λ��
	double dBCDispTime;   // ���ز��Գ���ʱ��
	double dBCDispDis;    // ���ز��Գ������ 
	double dBCDispNum;    // ���ز��Գ�������
	double dBCMGPorDOT;   // �������� mg

	tgPos tDefHomePos;    // �Զ���ԭ��λ��     
	tgPos tAutoPfsPos;     // �Զ�����λ��
	double dAutoPRSdelay;  // �Զ�������
	double dPRSTimes;      // �[�����
	double dSPRSTime;      // ���Μ[�翪��ʱ��  ms
	double dSPRSDelay;     // ���Μ[����ʱ�� ms
	tgPos tNeedleZAdjustPos;     // ��ͷZ��У��λ��
	tgPos tJetZAdjustPos;        // ���䷧Z����У׼λ��

	tgPos tPixelAdjustPos;       // ģ��궨λ��
	double m_patCaliAccepatance; // �궨ģ�����ƶȡ�

	tgPos  tBoardCalib_A;        // ����нǱ궨A����λ��
	tgPos  tBoardCalib_B;        // ����нǱ궨B����λ��
	double m_BoardCaliAccA;      // ����н�ģ��A���ƶ�
	double m_BoardCaliAccB;      // ����н�ģ��B���ƶ� 
	double m_BoardArc;           // ����н� 

	int   JetDistanceXY;

	tgPos tDispTestPos;           // �Խ�λ��

	double dNeedleAdjustDispTime;//��ͷͼ��ƫ��У������ʱ��
	double dNeedleAdjustWaitTime;//��ͷͼ��ƫ��У���ȴ�ʱ��
	double dNeedleAdjustUpHeight;//��ͷͼ��ƫ��У����߸߶�
	////////////////////////////////////
	double dPixelScale;    // ��е������ͼ���������ϵ�� 
	double dPixelAngle;
	int    nToolType;      // 0: ����ģʽ��1����ͷģʽ 2 ͼ��ģʽ
	int    nGoStandByPos;  // 0: �Զ��ӹ��겻�ؼӹ������� 1���Զ��ӹ���ؼӹ�������
//	int    nJetType;       // 0: ����ʽ���䷧  1��ѹ��ʽ���䷧   add by eddie.cheung 4.29    
	long   nJetPulse;      //  ��������
	int    nOpenTime;      // �������ʱ��  ������EFD ��������Ҫ���ڴ��ģʽ��ѭ������ ��
	int    nCloseTime;     // ����������  ������EFD ��������������������֮��ļ����
	double dPressJet;      // 0.15-6bar   ��Ͱѹ��
	double dPressDisp;     // 0.15-6bar   ����ѹ��(����Ҫ����Ļ���Ϊ��Ͳ����ģʽ�µĽ�ѹ���ƣ�
	double dJetSetTemperature;    // ���䷧�����¶�
	double dJetCurTemprature;     // ���䷧��ǰ�¶�
	////////////////////////////////////
	BOOL   bLoadSysEnaLoad;                  // �����ϻ���ʹ��  

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
	double dSpeedImageMoveXY;    // �ƶ���ģ�����λ�ٶ�
	double dSpeedImageMoveZ;
	double dSpeedDisp;
	double dSpeedHomeH1;//��е�ۻ�ԭ���ٶ�
	double dSpeedHomeH2;//��Ͳ�����ԭ���ٶ�
	double dSpeedHomeL;//��Ѱ��home���غ����
	double dSpeedBelt;
	double dBeltUnloadDelay;   // ������������ʱ ��λ��0.1s
	double dCaliNeedleStdHeight;
	double dCaliJetStdHeight;

	double dCaliNeedleDelta;
	double dCaliJetDelta;

	long lRunCounter;     // ���д���
	int uiWashPinCycle;   // �Զ�����ʱ���Զ���ϴ�ӹ������������
	int nAutoMode;        // ����ģʽ
	int iWashNum;         // ÿ����ϴ��ͷ����
	int iWashType;        // ��ϴ���� 0-����ʽ��ϴ 1-����ʽ��ϴ
	int nByPassFlag;

	double dDrawCircleR1;//
	int dDrawCircleR2;//
	int iLookNum;//200
	double dLookMaxAcc;
	double dLookAcc;

	double  m_LookTunScale;     // ǰհ���併�ٱ�
	int     m_LookEvenTime;     // �������ٶ�ʱ��

	int iYDir;
	int iXDir;

	double m_ccdMoveDis;        // Mark�����΢������
	int m_ccdMoveCnt;    
	int iMarkNum;

	int nFileSeparaEnable;  // �ļ��ְ�ӹ�ʹ��
	int nFileScrapeEnable;  // �ļ�ƴ�ռӹ�ʹ��
	int nFileScrapeNum;     // �ļ�ƴ�ռӹ�����

	//����ʱ�䶨��
	union   { 
		FILETIME   ft; 
		LONGLONG   val; 
	}t1,t2; 

	CTime m_SystemBeginTime;  // ϵͳ�������ʱ��
	double snapSecond;        // ��������ʱ��

	BOOL  nSFDoorEnable;      // ��ȫ�Ź���ʹ��

	int  nLaserReadPortID;    // ������ģ�����˿�
	double dSpeedFeedUpVel;   // �Ӳ�̨�����ٶ�
	double dSpeedFeedDownVel; // �Ӳ�̨�½��ٶ�
	double dJetZBaseHightValue; // Z���׼��߶�ֵ
	BOOL   bEnablePrecFun;       // ʹ�ܾ��Ȳ�����
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
	//�߶ε㽺�ٶ�
	double dVel;

	//�߶ε㽺������ǰ��ʱ�䣬���ɵ���ͣʱ�䣬�߶��ٶȣ���ǰ�ؽ�����
	double dPreTime, dNodeTime, dTailTime,dNodeVel, dTailCloseLen;

	//�߶ν����������ս����ȣ��ս��߶ȣ��ս������ٶ�
	double dEndLen, dEndZupHigh, dEndVel;
	int iEndType;//�㽺������������

	double dPreMoveTime;//�˶�����ʱ����

	double  dPreMoveD;//��ǰ��������
	double  dPresclae;//ϵ��

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
	//����㽺����:ǰ��ʱ�䣬�㽺ʱ�䣬�㽺������ͣʱ��
	double dPreTime, dDispTime, dTailTime;

	//Z�����߶�,
	double dZupHigh;
	int iZupType; //����ģʽ0:��Ը߶�, 1,���Ը߶�

	//�����ٶ�
	double dZupHVel;
	double dZupLVel;
	int DdispOn_T;//����ʱ��
	int DdispClose_T;//�ط�ʱ��
	int DispCount;//��������
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
	//����㽺����:ǰ��ʱ�䣬�㽺ʱ�䣬�㽺������ͣʱ��
	double dPreTime, dDispTime, dTailTime;

	//Z�����߶�,
	double dZupHigh;
	int iZupType; //����ģʽ0:��Ը߶�, 1,���Ը߶�

	//�����ٶ�
	double dZupHVel;
	double dZupLVel;
	int DdispOn_T;//����ʱ��
	int DdispClose_T;//�ط�ʱ��
	int DispCount;//��������
	int DispUseWParam;

};
struct tgCmdParam
{//�����в����ṹ��
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

		dZcleaType = 0;//����
		dZclearance = 0;//Ĭ����Ч

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
	//�����ٶ�
	double dVelXY,dVelZ;
	double dVelA;
	double dAccXY, dAccZ;

	//Z����߸߶����� ��ԣ����� ��λ�� 
	double dZcleaType;//==0���� 1==���
	double dZclearance;

	double dZSetType;
	double dZSetHeight;   // Z�Ṥ���߶�

	//�յ��ƶ��ٶ�
	double m_DummySpeed;
	//double dZLimit;

	int nLowTime;
	int nHighTime;
	long lPulseNum;
};

//////////////////////////////////////////////////////////////////////////
//һ��ָ��һ��
//������%��ʼ, ����, ���������(ÿ�����xyzu),�����������(�����������������������),���Ͳ�������, ������F, ����D, *%����
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
	{//����ʹ��
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
	int iDotNum;      //����������	
};
#endif
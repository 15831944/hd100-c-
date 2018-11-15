#pragma once
#include "LTSMC.h"
#pragma comment(lib,"LTSMC.lib")

enum SMC_DI{
	smc_di_decSpd_1,	// 减速1
	smc_di_decSpd_2,	// 减速2
	smc_di_revChk_1,	// 翻版检测1
	smc_di_revChk_2,	// 翻版检测2
	smc_di_stopChk_1,	// 阻挡检测1
	smc_di_stopChk_2,	// 阻挡检测2
	smc_di_clampChk_1,	// 夹持检测1
	smc_di_clampChk_2,	// 夹持检测2
	smc_di_num
};

enum SMC_DO{
	smc_do_stop_1,		// 阻挡气缸1
	smc_do_stop_2,		// 阻挡气缸2
	smc_do_clamp_1,		// 夹持气缸1
	smc_do_num
};

enum SMC_Mtr{
	smc_mtr_rev,
	smc_mtr_width,
	smc_mtr_track,
	smc_mtr_num
};


class CSMC606
{
public:
	CSMC606(void);
	~CSMC606(void);

	short Init();
	short Free(); 
	short ReadDiBit(WORD idx, BOOL &bOn);
	DWORD ReadDiPort();
	short ReadDoBit(WORD idx, BOOL &bOn);
	short SetDoBit(WORD idx, BOOL bOn);
	DWORD ReadDoPort();
	short Home(WORD iAxisIdx);

	bool m_bRevZero;
	short RevBoard();
	short CheckBoard();			// 检查翻板轨道上是否有板
	short TrackToBack();
	short BackToTrack();
	short PrepareReverse();		// 准备翻板，轨道右边阻止，皮带开始转动
	short PrepareOutBoard();	// 准备出板，轨道导通，皮带开始转动
	short BackOutBoard();		// 准备出板，轨道导通，皮带开始转动

	short AxisMoveAbs(WORD iAxis, double dPos, double dVal, bool bWaitStop=true);	// 绝对位置
	short AxisMoveRel(WORD iAxis, double dPos, double dVal, bool bWaitStop=true);	// 相对距离
	short AxisMoveJog(WORD iAxis, double dVal);	// jog运动
	short AxisStop(WORD iAxis);

	bool m_bInit;	//  初始化成功
	WORD m_nConnectNo;
};


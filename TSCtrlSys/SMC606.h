#pragma once
#include "LTSMC.h"
#pragma comment(lib,"LTSMC.lib")

enum SMC_DI{
	smc_di_decSpd_1,	// ����1
	smc_di_decSpd_2,	// ����2
	smc_di_revChk_1,	// ������1
	smc_di_revChk_2,	// ������2
	smc_di_stopChk_1,	// �赲���1
	smc_di_stopChk_2,	// �赲���2
	smc_di_clampChk_1,	// �гּ��1
	smc_di_clampChk_2,	// �гּ��2
	smc_di_num
};

enum SMC_DO{
	smc_do_stop_1,		// �赲����1
	smc_do_stop_2,		// �赲����2
	smc_do_clamp_1,		// �г�����1
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
	short CheckBoard();			// ��鷭�������Ƿ��а�
	short TrackToBack();
	short BackToTrack();
	short PrepareReverse();		// ׼�����壬����ұ���ֹ��Ƥ����ʼת��
	short PrepareOutBoard();	// ׼�����壬�����ͨ��Ƥ����ʼת��
	short BackOutBoard();		// ׼�����壬�����ͨ��Ƥ����ʼת��

	short AxisMoveAbs(WORD iAxis, double dPos, double dVal, bool bWaitStop=true);	// ����λ��
	short AxisMoveRel(WORD iAxis, double dPos, double dVal, bool bWaitStop=true);	// ��Ծ���
	short AxisMoveJog(WORD iAxis, double dVal);	// jog�˶�
	short AxisStop(WORD iAxis);

	bool m_bInit;	//  ��ʼ���ɹ�
	WORD m_nConnectNo;
};


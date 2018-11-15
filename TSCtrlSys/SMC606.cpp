#include "stdafx.h"
#include "TSCtrlSys.h"
#include "SMC606.h"

#define BackTrackSpd 5000

CSMC606::CSMC606(void)
{
	m_bInit = false;
	m_nConnectNo = 0;
	m_bRevZero = true;

//	Init();
}


CSMC606::~CSMC606(void)
{
	Free();
}

short CSMC606::Init()
{
	if (m_bInit)
	{
		return 1;
	}
	short rtn; 
	rtn = smc_board_init(m_nConnectNo,2,_T("192.168.5.11"), 115200);//��������
	if( rtn != 0 )   //���ƿ��ĳ�ʼ������
	{
		AfxMessageBox("���������ʧ�ܣ�");
		return 0;
	}

	m_bInit = true;
	return 1;
}

short CSMC606::Free()
{
	smc_board_close(m_nConnectNo);            //�ͷſ��ƿ���Դ
	return 1;
}

DWORD CSMC606::ReadDiPort()
{
	if (!m_bInit)			return 0;
	return smc_read_inport(m_nConnectNo, 0);
}

DWORD CSMC606::ReadDoPort()
{
	if (!m_bInit)			return 0;
	return smc_read_outport(m_nConnectNo, 0);
}

short CSMC606::ReadDiBit(WORD idx, BOOL &bOn)
{	
	if (!m_bInit)			return 0;
	if (idx > smc_di_num)	return 0;

	short rtn = smc_read_inbit(m_nConnectNo, idx);
	bOn = (rtn==1) ? FALSE:TRUE;

	return 1;
}
short CSMC606::ReadDoBit(WORD idx, BOOL &bOn)
{
	if (!m_bInit)			return 0;
	if (idx > smc_do_num)	return 0;

	short rtn = smc_read_outbit(m_nConnectNo, idx);
	bOn = (rtn==1) ? FALSE:TRUE;

	return 1;
}
short CSMC606::SetDoBit(WORD idx, BOOL bOn)
{
	if (!m_bInit)	return 0;
	if (idx > smc_do_num)	return 0;

	short rtn = smc_write_outbit(m_nConnectNo,idx,!bOn);
	return (0 == rtn) ? 1 : 0;
}

short CSMC606::Home(WORD iAxisIdx)
{
	if (!m_bInit)	return 0;

	BOOL bRevChk = TRUE;
	ReadDiBit(smc_di_revChk_1, bRevChk);
	if (bRevChk)
	{
		AfxMessageBox("����ʱ [������⿪1] ���ź�");
		return 0;
	}
	ReadDiBit(smc_di_revChk_2, bRevChk);
	if (bRevChk)
	{
		AfxMessageBox("����ʱ [������⿪2] ���ź�");
		return 0;
	}

	short iret = 0;
	iret = smc_set_pulse_outmode(m_nConnectNo,iAxisIdx,0);  //�����������ģʽ
	iret = smc_set_equiv(m_nConnectNo, iAxisIdx, 1);//�������嵱��
	iret = smc_set_alm_mode(m_nConnectNo, iAxisIdx, 0, 0, 0);//���ñ���ʹ�ܣ��رձ���
	iret = smc_write_sevon_pin(m_nConnectNo, iAxisIdx, 0);//���ŷ�ʹ��
	iret = smc_set_home_profile_unit(m_nConnectNo,iAxisIdx, 500, 4000, 0.1, 0.1);//�����ٶ�����
	iret = smc_set_home_pin_logic(m_nConnectNo,iAxisIdx,0,0);//����ԭ����Ч��ƽ��0-����Ч��1-����Ч
//	iret = smc_set_ez_mode(m_nConnectNo,iAxisIdx,m_nEzLogic,0,0);//����EZ��Ч��ƽ����0-����Ч��1-����Ч
	iret = smc_set_homemode(m_nConnectNo,iAxisIdx, 0, 1, 2,1);//���û��㷽ʽ
	iret = smc_home_move(m_nConnectNo,iAxisIdx);//���������˶�
	while (smc_check_done(m_nConnectNo,iAxisIdx)==0)      //�жϵ�ǰ��״̬
	{
		AfxGetApp()->PumpMessage();
	}
	WORD state = 0;
	iret = smc_get_home_result(m_nConnectNo,iAxisIdx,&state);
	if(state==1)//������ɺ�ָ������
	{
		iret = smc_set_position_unit(m_nConnectNo,iAxisIdx,0);
	}

	iret = smc_set_position_unit(m_nConnectNo,smc_mtr_track,0);
	m_bRevZero = true;

	iret = smc_write_outport(m_nConnectNo, 0, 0xffff);

	AxisStop(smc_mtr_track);
	return (0 == iret) ? 1 : 0;
}
short CSMC606::AxisMoveAbs(WORD iAxis, double dPos, double dVal, bool bWaitStop)	// ����λ��
{
	if (!m_bInit)	return 0;
	if (iAxis > 2)	return 0;

	short iret = 0;
	if (smc_check_done( m_nConnectNo,iAxis) == 0) //�Ѿ����˶���
		return 1; 	
	iret = smc_set_equiv(m_nConnectNo, iAxis, 1);//�������嵱��
	iret = smc_set_alm_mode(m_nConnectNo, iAxis, 0, 0, 0);//���ñ���ʹ�ܣ��رձ���
	iret = smc_write_sevon_pin(m_nConnectNo, iAxis, 0);//���ŷ�ʹ��
	iret = smc_set_pulse_outmode(m_nConnectNo, iAxis, 0);//�趨����ģʽ���˴�����ģʽ�̶�ΪP+D��������+����	
	iret = smc_set_profile_unit(m_nConnectNo,iAxis, 500, dVal, 0.05, 0.05, 500);//�趨�����˶��ٶȲ���	
	iret = smc_set_s_profile(m_nConnectNo,iAxis, 0, 0.05);//�趨S��ʱ��

	iret = smc_pmove_unit(m_nConnectNo, iAxis, dPos, 1);// ����λ���˶�
	if (bWaitStop)
	{
		DWORD tmSt = GetTickCount();
		while (smc_check_done( m_nConnectNo,iAxis) == 0)
		{
			if (GetTickCount()-tmSt > 10000)
			{
				AfxMessageBox("��ﳬʱ��δ��λ!");
				AxisStop(iAxis);
				return 0;
			}
			CFunction::DelaySec(0.02);
		}
	}
	
	return 1;
}
short CSMC606::AxisMoveRel(WORD iAxis, double dPos, double dVal, bool bWaitStop)	// ��Ծ���
{
	if (!m_bInit)	return 0;
	if (iAxis > 2)	return 0;

	short iret = 0;
	if (smc_check_done( m_nConnectNo,iAxis) == 0) //�Ѿ����˶���
		return 1; 	
	iret = smc_set_equiv(m_nConnectNo, iAxis, 1);//�������嵱��
	iret = smc_set_alm_mode(m_nConnectNo, iAxis, 0, 0, 0);//���ñ���ʹ�ܣ��رձ���
	iret = smc_write_sevon_pin(m_nConnectNo, iAxis, 0);//���ŷ�ʹ��
	iret = smc_set_pulse_outmode(m_nConnectNo, iAxis, 0);//�趨����ģʽ���˴�����ģʽ�̶�ΪP+D��������+����	
	iret = smc_set_profile_unit(m_nConnectNo,iAxis, 500, dVal, 0.1, 0.1, 500);//�趨�����˶��ٶȲ���	
	iret = smc_set_s_profile(m_nConnectNo,iAxis, 0, 0.05);//�趨S��ʱ��

	iret = smc_pmove_unit(m_nConnectNo, iAxis, dPos, 0);//��Զ����˶�
	if (bWaitStop)
	{
		DWORD tmSt = GetTickCount();
		while (smc_check_done( m_nConnectNo,iAxis) == 0)
		{
			if (GetTickCount()-tmSt > 10000)
			{
				AfxMessageBox("��ﳬʱ��δ��λ!");
				AxisStop(iAxis);
				return 0;
			}
			CFunction::DelaySec(0.02);
		}
	}
	return 1;
}
short CSMC606::AxisMoveJog(WORD iAxis, double dVal)	// jog�˶�
{
	if (!m_bInit)	return 0;
	if (iAxis > 2)	return 0;

	short iret = 0;
	if (smc_check_done( m_nConnectNo,iAxis) == 0) //�Ѿ����˶���
		return 1; 	
	iret = smc_set_equiv(m_nConnectNo, iAxis, 1);//�������嵱��
	iret = smc_set_alm_mode(m_nConnectNo, iAxis, 0, 0, 0);//���ñ���ʹ�ܣ��رձ���
	iret = smc_write_sevon_pin(m_nConnectNo, iAxis, 0);//���ŷ�ʹ��
	iret = smc_set_pulse_outmode(m_nConnectNo, iAxis, 0);//�趨����ģʽ���˴�����ģʽ�̶�ΪP+D��������+����	
	iret = smc_set_profile_unit(m_nConnectNo,iAxis, 500, dVal, 0.1, 0.1, 500);//�趨�����˶��ٶȲ���	
	iret = smc_set_s_profile(m_nConnectNo,iAxis, 0, 0.05);//�趨S��ʱ��

	if (dVal < 0)
	{
		iret = smc_vmove(m_nConnectNo, iAxis, 0);	//�����˶�
	}
	else
	{
		iret = smc_vmove(m_nConnectNo, iAxis, 1);	//�����˶�
	}
	return 1;
}
short CSMC606::AxisStop(WORD iAxis)
{
	if (!m_bInit)	return 0;
	if (iAxis > 2)	return 0;

	smc_set_dec_stop_time(m_nConnectNo,iAxis, 0.01);//����10ms����ֹͣʱ��
	smc_stop(m_nConnectNo, iAxis, 0);   //����ֹͣ	
	return 1;
}

short CSMC606::CheckBoard()	// ��鷭�������Ƿ��а�
{
	if (!m_bInit)			return 0;
	BOOL bDiStatus[4];

	ReadDiBit(smc_di_revChk_1, bDiStatus[0]);
	ReadDiBit(smc_di_revChk_2, bDiStatus[1]);
	ReadDiBit(smc_di_decSpd_1, bDiStatus[2]);
	ReadDiBit(smc_di_decSpd_2, bDiStatus[3]);

	if (bDiStatus[0] || bDiStatus[1] || bDiStatus[2] || bDiStatus[3])
		return 1;
	else
		return 0;
}
short CSMC606::TrackToBack()
{
	if (!m_bInit)			return 0;

	BOOL bDiStatus = FALSE;
	SetDoBit(smc_do_clamp_1, FALSE);
	CKFTimer tmTemp;
	if (m_bRevZero)
	{
		// ���嵼ͨ
		SetDoBit(smc_do_stop_1, TRUE);
		SetDoBit(smc_do_stop_2, FALSE);
		tmTemp.TimerStart();
		while (1)
		{
			ReadDiBit(smc_di_stopChk_2, bDiStatus);
			if (bDiStatus)	break;

			if (tmTemp.TimerGetSecond() > 3.0)
			{
				AfxMessageBox("�赲����2�źŵ�λ��ʱ");
				return 0;
			}
		}

		tmTemp.TimerStart();
		AxisMoveJog(smc_mtr_track, 5000);
		while (1)
		{
			ReadDiBit(smc_di_decSpd_2, bDiStatus);
			if (bDiStatus)	break;

			if (tmTemp.TimerGetSecond() > 6.0)
			{
				AfxMessageBox("�㽺λ�������ʱ����");
				return 0;
			}
		}
		Sleep(200);
		AxisStop(smc_mtr_track);
	}
	else
	{
		// ���嵼ͨ
		SetDoBit(smc_do_stop_1, FALSE);
		SetDoBit(smc_do_stop_2, TRUE);
		tmTemp.TimerStart();
		while (1)
		{
			ReadDiBit(smc_di_stopChk_1, bDiStatus);
			if (bDiStatus)	break;

			if (tmTemp.TimerGetSecond() > 3.0)
			{
				AfxMessageBox("�赲����2�źŵ�λ��ʱ");
				return 0;
			}
		}

		tmTemp.TimerStart();
		AxisMoveJog(smc_mtr_track, -5000);
		while (1)
		{
			ReadDiBit(smc_di_decSpd_1, bDiStatus);
			if (bDiStatus)	break;

			if (tmTemp.TimerGetSecond() > 6.0)
			{
				AfxMessageBox("�㽺λ�������ʱ����");
				return 0;
			}
		}
		Sleep(200);
		AxisStop(smc_mtr_track);
	}
	return 1;
}

short CSMC606::BackToTrack()
{
	if (!m_bInit)			return 0;

	if (!CheckBoard())
	{
		AfxMessageBox("�������ް�");
		return 1;
	}
	BOOL bDiStatus = FALSE;
	CKFTimer tmTemp;
	if (m_bRevZero)
	{
		// ���嵼ͨ
		SetDoBit(smc_do_clamp_1, FALSE);
		SetDoBit(smc_do_stop_1, TRUE);
		SetDoBit(smc_do_stop_2, FALSE);
		tmTemp.TimerStart();
		while (1)
		{
			ReadDiBit(smc_di_stopChk_2, bDiStatus);
			if (bDiStatus)	break;

			if (tmTemp.TimerGetSecond() > 3.0)
			{
				AfxMessageBox("�赲����2�źŵ�λ��ʱ");
				return 0;
			}
		}

		tmTemp.TimerStart();
		AxisMoveJog(smc_mtr_track, -5000);
		while (1)
		{
			ReadDiBit(smc_di_revChk_1, bDiStatus);
			if (bDiStatus)	break;

			if (tmTemp.TimerGetSecond() > 6.0)
			{
				AfxMessageBox("��������㽺λʱ����");
				return 0;
			}
		}
	}
	else
	{
		// ���嵼ͨ
		SetDoBit(smc_do_clamp_1, FALSE);
		SetDoBit(smc_do_stop_2, TRUE);
		SetDoBit(smc_do_stop_1, FALSE);
		tmTemp.TimerStart();
		while (1)
		{
			ReadDiBit(smc_di_stopChk_1, bDiStatus);
			if (bDiStatus)	break;

			if (tmTemp.TimerGetSecond() > 3.0)
			{
				AfxMessageBox("�赲����2�źŵ�λ��ʱ");
				return 0;
			}
		}

		tmTemp.TimerStart();
		AxisMoveJog(smc_mtr_track, 5000);
		while (1)
		{
			ReadDiBit(smc_di_revChk_1, bDiStatus);
			if (bDiStatus)	break;

			if (tmTemp.TimerGetSecond() > 6.0)
			{
				AfxMessageBox("��������㽺λʱ����");
				return 0;
			}
		}
	}
	return 1;
}
short CSMC606::RevBoard()
{
	if (!m_bInit)			return 0;
	short rtn = 0;

	BOOL bRevChk = TRUE;
	ReadDiBit(smc_di_revChk_1, bRevChk);
	if (bRevChk)
	{
		AfxMessageBox("����ǰ [������⿪1] ���ź�");
		return 0;
	}
	ReadDiBit(smc_di_revChk_2, bRevChk);
	if (bRevChk)
	{
		AfxMessageBox("����ǰ [������⿪2] ���ź�");
		return 0;
	}

	SetDoBit(smc_do_clamp_1, TRUE);
	Sleep(100);

	if (m_bRevZero)
	{
		rtn = AxisMoveAbs(smc_mtr_rev, 21000, 10000);
		m_bRevZero = !m_bRevZero;
	}
	else
	{
		rtn = AxisMoveAbs(smc_mtr_rev, -1000, 10000);
		m_bRevZero = !m_bRevZero;
	}

	return 1==rtn ? 1:0;
}

short CSMC606::PrepareReverse()		// ׼�����壬����ұ���ֹ��Ƥ����ʼת��
{
	if (!m_bInit)			return 0;
	SetDoBit(smc_do_clamp_1, FALSE);	// �رռг�����

	BOOL bDiStatus = FALSE;
	CKFTimer tmTemp;
	tmTemp.TimerStart();

	if (m_bRevZero)
	{
		SetDoBit(smc_do_stop_1, TRUE);
		SetDoBit(smc_do_stop_2, FALSE);
		while (TRUE)
		{
			ReadDiBit(smc_di_stopChk_2, bDiStatus);
			if (!bDiStatus)		Sleep(20);
			else				break;

			if (tmTemp.TimerGetMs() > 1000)
			{
				AfxMessageBox("�赲����2�رյ�λ��ʱ");
				return -1;
			}
		}

		AxisMoveJog(smc_mtr_track, BackTrackSpd);
	}
	else
	{
		SetDoBit(smc_do_stop_1, FALSE);
		SetDoBit(smc_do_stop_2, TRUE);

		while (TRUE)
		{
			ReadDiBit(smc_di_stopChk_1, bDiStatus);
			if (!bDiStatus)		Sleep(20);
			else				break;

			if (tmTemp.TimerGetMs() > 1000)
			{
				AfxMessageBox("�赲����1�رյ�λ��ʱ");
				return -1;
			}
		}
		AxisMoveJog(smc_mtr_track, -BackTrackSpd);
	}

	return 1;
}

short CSMC606::PrepareOutBoard()	// ׼�����壬�����ͨ��Ƥ����ʼת��
{
	if (!m_bInit)			return 0;
	SetDoBit(smc_do_stop_1, TRUE);
	SetDoBit(smc_do_stop_2, TRUE);
	SetDoBit(smc_do_clamp_1, FALSE);

	if (m_bRevZero)
	{
		AxisMoveJog(smc_mtr_track, BackTrackSpd);
	}
	else
	{
		AxisMoveJog(smc_mtr_track, -BackTrackSpd);
	}

	return 1;
}

short CSMC606::BackOutBoard() // ׼�����壬�����ͨ��Ƥ����ʼת��
{
	if (!m_bInit)			return 0;
	BOOL bDists = FALSE;
	CKFTimer tmTemp;

	tmTemp.TimerStart();
	while (1)
	{
		ReadDiBit(smc_di_revChk_2, bDists);
		if (bDists)
		{
			break;
		}
		if (tmTemp.TimerGetSecond() > 10.0)
		{
			AfxMessageBox("���忨��!");
			AxisStop(smc_mtr_track);
			return 0;
		}
		CFunction::DelaySec(0.01);
	}

	tmTemp.TimerStart();
	while (1)
	{
		ReadDiBit(smc_di_revChk_2, bDists);
		if (!bDists)
		{
			break;
		}
		if (tmTemp.TimerGetSecond() > 10.0)
		{
			AfxMessageBox("���忨��!");
			AxisStop(smc_mtr_track);
			return 0;
		}
		CFunction::DelaySec(0.01);
	}
	Sleep(200);
	AxisStop(smc_mtr_track);
	return 1;
}
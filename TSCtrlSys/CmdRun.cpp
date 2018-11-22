#include "StdAfx.h"
#include "CmdRun.h"
#include "DlgPathEdit.h"
//#include "CmdEditBar.h"
#include "MainFrm.h"
#include "TSCtrlSys.h"
#include <math.h>
#include "./Smooth/PointGE.h"
#include "DlgJogXYZ.h"
//#pragma warning(disable: 4100)
#include "FindmarkJog.h"
#define INPUT_LOW      0x00
#define INPUT_HIGHT    0x01
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define Vel_Limit 500

//////////////////////////////////////////////
CWinThread *g_pMonitorThread = NULL;		 // ����߳�
CWinThread *g_pPlayThread = NULL;			 // ����߳�
CWinThread *g_pUpdateThread = NULL;			 // ����߳�
CWinThread *g_pLineSignStsThread = NULL;	 // ��ˮ���ź�״̬����߳�
CWinThread *g_pAutoRPSThread = NULL; 
CWinThread *g_pLoadThread = NULL;
CWinThread *g_pLoadReadyThread = NULL;
CWinThread *g_pUnloadThread = NULL;
CWinThread *g_pUnloadReadyThread = NULL;
CWinThread *g_pLightsetThread = NULL;	 
CWinThread *g_pHomeLdThread = NULL;
CWinThread *g_pHomeUldThread = NULL;
CWinThread *g_pHomeRotateTable = NULL;

BOOL g_bStop = TRUE;	// �Ƿ�ֹͣ�㽺
BOOL g_bClose = FALSE;   //�Ƿ�ر��˿��ƿ�
BOOL g_bUpdate = FALSE;   

double m_GotoOffSet[MAX_AXIS_NUM];//��תƫ��
CEvent m_EvtAskSeparaRun;// = CEvent(FALSE, TRUE);            // ����ֶμӹ��¼� 
//////////////////////////////////////////////////////////////////////////
int LoopCount    =- 20;    
int LoopAddCount =- 20;    

int CCmdRun::m_WCBatetID  = 0;   // �հ����ID
int CCmdRun::m_WCBatetID2 = 0;   // �������ID��

//////////////////////////////////////////////////////////////////////////
//wparam:axis pos double[4]
//lparam:no used
UINT CCmdRun::PlayThread(LPVOID lparam)
{
	CCmdRun *p0 = (CCmdRun*)lparam;
	//Play�������治��ʲô������Ҫ���أ���������߳��ڴ�й©

	//////////////////////////////////////////////////////////////////////////
	BOOL bInpStatus = FALSE;
	short iRtn = 0;

	// �Ƿ����ý�������
// 	if (g_pFrm->m_pRobotParam->m_structGlueWarning.m_nIfGlueWarning)
// 	{	// ������⴫����
// 		if (0 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)
// 		{
// 			iRtn = p0->ExInput(_T("�������1"), bInpStatus);	
// 			if (RTN_NONE != iRtn || !bInpStatus)
// 			{
// 				if (IDOK != p0->AfxMessageBox(_T("�����Ѿ�����,\n������ʾ�����㽺��:[ȷ��].\n�˳������:[ȡ��]"), MB_OKCANCEL))
// 				{
// 					return 0;
// 				}
// 			}
// 		}
// 		else if (g_pFrm->m_pRobotParam->m_structGlueWarning.IfGlueUsedOut())
// 		{	// Ԥ��õĵ㽺ʱ��
// 			if (2 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)
// 			{
// 				if (IDOK != p0->AfxMessageBox(_T("��ˮ�Ѿ�����,\n������ʾ�����㽺��:[ȷ��].\n�˳������:[ȡ��]"), MB_OKCANCEL))
// 				{
// 					return 0;
// 				}
// 			}
// 			else if (1 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)	// ����
// 			{
// 				if (IDOK != p0->AfxMessageBox(_T("�����Ѿ�����,\n������ʾ�����㽺��:[ȷ��].\n�˳������:[ȡ��]"), MB_OKCANCEL))
// 				{
// 					return 0;
// 				}
// 			}
// 		}
// 	}
	//////////////////////////////////////////////////////////////////////////
	// �����㽺�̣߳����ȳ���.��Ϊֹͣʱ�ܿ����ǻ���ˮ��
	if (g_pFrm->m_pRobotParam->m_nIfAutoWeight)
	{
		iRtn = p0->WeightAdjust();
		if (1 != iRtn)
		{
			g_bStop = TRUE;
			p0->m_tSysStatus = K_RUN_STS_WEIGHTERROR;
			return 0;
		}
	}
	p0->m_tmAutoRps.TimerStart();

	while (!g_bStop || p0->m_bRunOnce)
	{		
		if (p0->m_tSysStatus == K_RUN_STS_PAUSE)
		{
			Sleep(100);
			continue;
		}

		if (p0->m_bCanWeightAndWash)	// �����̼߳�⵽�������
		{		
			// ����У׼ʱ�䵽��
			if (g_pFrm->m_pRobotParam->m_nIfAutoWeight && (p0->m_tmAutoWeight.TimerGetMinute() > g_pFrm->m_pRobotParam->m_nWeightIntervalValue))
			{
				iRtn = p0->WeightAdjust();
				if (1 != iRtn)
				{
					p0->m_tSysStatus = K_RUN_STS_WEIGHTERROR;
					g_bStop = TRUE;
					break;
				}
			}

			// �Ƿ����ý�������
			if (g_pFrm->m_pRobotParam->m_structGlueWarning.m_nIfGlueWarning)
			{	// ������⴫����
				if (0 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)
				{
					iRtn = p0->ExInput(_T("�������1"), bInpStatus);	
					if (RTN_NONE != iRtn || !bInpStatus)
					{
						if (IDOK != p0->AfxMessageBox(_T("�����Ѿ�����,\n������ʾ�����㽺��:[ȷ��].\n�˳������:[ȡ��]"), MB_OKCANCEL))
						{
							p0->Stop();
							return 0;
						}
					}
				}
				else if (g_pFrm->m_pRobotParam->m_structGlueWarning.IfGlueUsedOut())
				{	// Ԥ��õĵ㽺ʱ��
					if (2 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)
					{
						if (IDOK != p0->AfxMessageBox(_T("��ˮ�Ѿ�����,\n������ʾ�����㽺��:[ȷ��].\n�˳������:[ȡ��]"), MB_OKCANCEL))
						{
							p0->Stop();
							return 0;
						}
					}
					else if (1 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)	// ����
					{
						if (IDOK != p0->AfxMessageBox(_T("�����Ѿ�����,\n������ʾ�����㽺��:[ȷ��].\n�˳������:[ȡ��]"), MB_OKCANCEL))
						{
							p0->Stop();
							return 0;
						}
					}
				}
			}

			// ����������
			if ((0!=g_pFrm->m_pRobotParam->m_structClean.m_nCleanInterval) 
				&& g_pFrm->m_pRobotParam->m_structClean.m_nIfAutoClean)
			{
				if (0 == (g_pFrm->m_pSysParam->lRunCounter%g_pFrm->m_pRobotParam->m_structClean.m_nCleanInterval))
				{
					if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
					{
						// ��������
						p0->WashJet(true);
					}
					else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
					{
						p0->WashNeedle();
						if (g_pFrm->m_pRobotParam->m_structTestGlue.m_nIfTestGlue)
						{
							p0->TestDisp();
						}
					}
				}
			}
			p0->m_bCanWeightAndWash = false;
		}

		// �Զ���������
		if (!p0->m_bDispEnable)
		{
			if ((0==g_pFrm->m_pRobotParam->m_nWorkMode) && (1==g_pFrm->m_pRobotParam->m_nIfAutoRps))
			{
				if (p0->m_tmAutoRps.TimerGetMinute() > g_pFrm->m_pRobotParam->m_dAutoRpsIntervalMin)
				{
					p0->AutoRps();
				}
			}
			Sleep(20);
			continue;
		}

		p0->m_bDispEnable = false;
		if (1 != p0->Play())
		{
			g_bStop = TRUE;
			break;
		}
		// ������̻������ʱ
		p0->m_tmAutoRps.TimerStart();
	}

	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION, _T("ֹͣ"));
	p0->m_bFlagRunning = FALSE;

	return 0;
}
int CCmdRun::checkWeightLMT(int ID)
{
	double dete =-1;
	dete = g_pFrm->m_BatetWCDate[ID].dWCDate_1 
		-g_pFrm->m_BatetWCDate[ID].dWCDate_0 ;

	if(g_pFrm->m_pSysParam->dWeightLmtMin>dete)
	{
		return -1;
	}
	else if(g_pFrm->m_pSysParam->dWeightLmtMax<dete)
	{
		return  1;
	}else
		return 0;
}

UINT CCmdRun::AutoRPSThread(LPVOID lparam)
{
	CCmdRun *p0 = (CCmdRun*)lparam;
	// ����
	p0->Home();

	// ����λ��
	double x = g_pFrm->m_pRobotParam->m_posWeightAdj.x;
	double y = g_pFrm->m_pRobotParam->m_posWeightAdj.y;
	double z = g_pFrm->m_pRobotParam->m_zContactPos-g_pFrm->m_pRobotParam->m_posWeightAdj.z;

//	double distime = g_pFrm->m_pRobotParam->m_dAutoRpsIntervalMin*60;	// ���ʱ��
	int times = g_pFrm->m_pRobotParam->m_nAutoRpsDots;				// �����������
// 	double opentimes  =g_pFrm->m_pSysParam->dSPRSTime;       // ����ʱ�� ms
// 	double closetimes =g_pFrm->m_pSysParam->dSPRSDelay;      // �ط�ʱ�� ms

	// �ƶ���λ������λ��
	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

	while (TRUE)
	{
		p0->DotDisp(times);
// 		if(g_pFrm->m_Robot->m_bAutoPreFic == TRUE)
// 		{
// 			g_pFrm->m_Robot->JetOutputEx((long)times, (long)opentimes, (long)closetimes,TRUE);
// 
// 			// ����ʱ��
// 			distime=g_pFrm->m_pSysParam->dAutoPRSdelay*60;
// 			CFunction::DelaySec(distime);
// 		}else
// 		{
// 			return 0;
// 		}
	}

	return 0;
}

UINT CCmdRun::UpdateThread(LPVOID lparam)
{
	CCmdRun *p1 = (CCmdRun*)(lparam);

	while(!g_bUpdate)
	{
		p1->CheckStatus();
		Sleep(500);
	}

	return 0;
}

UINT CCmdRun::LineSignStsThread(LPVOID lparam)
{
	UNREFERENCED_PARAMETER(lparam);
	//CCmdRun *p1 = (CCmdRun*)(lparam);

	while(!g_bUpdate)
	{
		if(g_pFrm->m_dlgIOCtrl.GetSafeHwnd() && g_pFrm->m_dlgIOCtrl.IsWindowVisible())
		{
			g_pFrm->m_dlgIOCtrl.pageInput.GetPLCRSts();
		}	
		Sleep(500);
	}

	return 0;
}

// Io �����߳�
UINT CCmdRun::MonitorThread(LPVOID lparam)
{
	CCmdRun *p2 = (CCmdRun*)(lparam);

	while(!g_bClose)
	{
		p2->CheckIO();
		Sleep(5);
	}
	return 0;
}

// �ƹ������߳�
UINT CCmdRun::LightSetThread(LPVOID lparam)
{
	CCmdRun *p2 = (CCmdRun*)(lparam);
	while (!g_bClose)
	{
		switch (p2->m_tSysStatus)
		{
		case K_RUN_STS_PAUSE:
		case K_RUN_STS_STOP:
		case K_RUN_STS_NONE:
			p2->ExOutput(_T("��ɫ��.��"),FALSE);
			p2->ExOutput(_T("��ɫ��.��"),TRUE);
			p2->ExOutput(_T("��ɫ��.��"),FALSE);
			p2->ExOutput(_T("������"),FALSE);
			break;
		case K_RUN_STS_RUN:
		case K_RUN_STS_RUN_1_UNIT:	// ���е㽺ʱ������һ��unit��ֹͣ����
		case K_RUN_STS_RUN_1_BOARD:	// ���굱ǰ��Ƭ��ֹͣ����
			p2->ExOutput(_T("��ɫ��.��"),FALSE);
			p2->ExOutput(_T("��ɫ��.��"),FALSE);
			p2->ExOutput(_T("��ɫ��.��"),TRUE);
			p2->ExOutput(_T("������"),FALSE);
			break;
		default:
			p2->ExOutput(_T("��ɫ��.��"),TRUE);
			p2->ExOutput(_T("��ɫ��.��"),FALSE);
			p2->ExOutput(_T("��ɫ��.��"),FALSE);
			p2->ExOutput(_T("������"),TRUE);
			Sleep(500);
			p2->ExOutput(_T("��ɫ��.��"),FALSE);
			p2->ExOutput(_T("��ɫ��.��"),FALSE);
			p2->ExOutput(_T("��ɫ��.��"),FALSE);
			p2->ExOutput(_T("������"),FALSE);
			break;
		}
		Sleep(500);
	}

	return 1;
}

UINT CCmdRun::LoadThread_3Box(LPVOID lparam)		// ����ʽ3���Ϻ������߳�
{	
	CCmdRun *p2 = (CCmdRun*)(lparam);
	p2->m_bLoadFinish = false;
	p2->m_bLoadSuccess = false;

	while(!g_bStop)
	{	
		if (p2->m_tSysStatus == K_RUN_STS_PAUSE)
		{
			Sleep(10);
			continue;
		}

		if (1 != p2->LoadStep_3Box())
		{
			g_bStop = TRUE;
			break;
		}
		Sleep(20);
	}

	g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("ֹͣ");
	p2->m_bLoadFinish = true;

	return 0;
}

UINT CCmdRun::ThreadRotateTableHome(LPVOID lparam)	// ��תƽ̨��ԭ��
{
	CCmdRun *p4 = (CCmdRun*)lparam;
	p4->RotateTable_Home();
	return 0;
}

UINT CCmdRun::ThreadHomeLd(LPVOID lparam)	// ����ʽ����������
{
	CCmdRun *p3 = (CCmdRun*)lparam;
	p3->LoadUnLoad_Home(0);
	return 0;
}
UINT CCmdRun::ThreadHomeUld(LPVOID lparam)	// ����ʽ����������
{
	CCmdRun *p3 = (CCmdRun*)lparam;
	p3->LoadUnLoad_Home(1);
	return 0;
}

// �����߳�
UINT CCmdRun::LoadThread(LPVOID lparam)
{
	CCmdRun *p2 = (CCmdRun*)(lparam);
	p2->m_bLoadFinish = false;
	p2->m_bLoadSuccess = false;

	while(!g_bStop)
	{	
		if (p2->m_tSysStatus == K_RUN_STS_PAUSE)
		{
			Sleep(100);
			continue;
		}

		if (1 != p2->LoadStep())
		{
			g_bStop = TRUE;
			break;
		}
		Sleep(20);
	}

	g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("ֹͣ");
	p2->m_bLoadFinish = true;
	return 0;
}

UINT CCmdRun::UnloadThread_3Box(LPVOID lparam)	// ����ʽ3���Ϻ������߳�
{
	CCmdRun *p2 = (CCmdRun*)(lparam);
	p2->m_bUnloadFinish = false;
	p2->m_bUnloadSuccess = false;
	short rtn = 0;

	// ���㽺��λ�Ƿ��а�		
	BOOL bInpStatus[4];
	if (RTN_NONE != p2->ExInput(_T("14�Ź⿪"), bInpStatus[0]))
	{
		g_bStop = TRUE;
		return 0;
	}
	if (RTN_NONE != p2->ExInput(_T("13�Ź⿪"), bInpStatus[1]))
	{
		g_bStop = TRUE;
		return 0;
	}

	if (bInpStatus[0] || bInpStatus[1])
	{
		if (IDYES == p2->AfxMessageBox(_T("�㽺��λ�а�,�Ƿ���Ҫ����!"), MB_YESNO))
		{
			// �ɰ� ׼������
			p2->ExOutput(_T("12���赲���׵�ŷ�"), FALSE);
			p2->UnFixBoard();

			p2->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("����Ƥ��"));

			CKFTimer tmTemp;
			tmTemp.TimerStart();
			while (TRUE)
			{
				rtn = p2->ExInput(_T("14�Ź⿪"), bInpStatus[0]);
				if (RTN_NONE != rtn)
				{
					g_bStop = TRUE;
					return 0;
				}
				if (bInpStatus[0])	break;
				else				Sleep(20);

				if (tmTemp.TimerGetSecond() > 6.0)
				{
					p2->AfxMessageBox("�㽺��λ����!���ڹ⿪δ��Ӧ�����ϳ�ʱ!");
					g_bStop = TRUE;
					return 0;
				}
			}
			Sleep(DWORD(g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay * 1000 * 2));

			rtn = p2->LoadUnLoad_unloadPushOut();
			rtn |= p2->LoadUnLoad_unloadPullBack();
			if (RTN_NONE != rtn)
			{
				g_bStop = TRUE;
				return 0;
			}

			rtn = p2->ExInput(_T("14�Ź⿪"), bInpStatus[0]);
			if (RTN_NONE != rtn)
			{
				g_bStop = TRUE;
				return 0;
			}
			if (bInpStatus[0])
			{
				p2->AfxMessageBox("�Ƴ��Ϻ���ڹ⿪����Ӧ�����ϣ����ڿ��ϻ��ߴ������ź��������̽�ֹͣ");
				g_bStop = TRUE;
				return 0;
			}

			// �Ϻ�����һ��
			int crtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId;
			int crtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;

			if ((g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount-1) == crtLayer)
			{
				if (2 == crtBox)
				{
					p2->ExOutput(_T("��ɫ��.��"),TRUE);
					p2->ExOutput(_T("������"),TRUE);
					rtn = p2->LoadUnLoad_unloadBoxMove(0, false);
					if (RTN_NONE != rtn)	
					{
						g_bStop = TRUE;
						return 0;
					}

					if (IDOK == p2->AfxMessageBox(_T("�ѵ��Ϻ��յ㣬�����ÿ��Ϻк� [ȷ��]  �˳����̰� [ȡ��]"), MB_OKCANCEL))
					{
						p2->m_pController->WaitAxisStop(UNLOAD_AXIS);
						p2->ExOutput(_T("��ɫ��.��"), FALSE);
						p2->ExOutput(_T("������"), FALSE);

						rtn = p2->ExInput("Unload���Ϻм��", bInpStatus[0]);
						if (!bInpStatus[0])
						{
							p2->AfxMessageBox(_T("���ڵ�һ��ź��Ϻк����"));
						}
					}
					else
					{
						g_bStop = TRUE;
						return 0;
					}
				}
				else
				{
					p2->LoadUnLoad_unloadBoxMove(crtBox+1);
					rtn = p2->ExInput("Unload���Ϻм��", bInpStatus[0]);
					if (!bInpStatus[0])
					{
						p2->AfxMessageBox(_T("���ڵ�ǰ��ź��Ϻк����"));
					}
				}
			}
			else
			{
				rtn = (short)p2->LoadUnLoad_unloadLayerMove(crtLayer+1);
			}

			p2->m_bLoadEnable = true;
		}
		else
		{
			p2->m_bCanWeightAndWash = true;
			CKFTimer tmTemp;
			BOOL bDiStatus = FALSE;
			//p2->m_pController->AxisJog(T1_AXIS, -g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax/4, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
			p2->ExOutput("12���赲���׵�ŷ�", TRUE);
			tmTemp.TimerStart();
			while (tmTemp.TimerGetSecond() < 2.0)
			{
				g_pFrm->m_Robot->ExInput("12���赲���׼��", bDiStatus);
				if (bDiStatus)
				{
					break;
				}
			}
			g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax/2, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

			tmTemp.TimerStart();
			while (tmTemp.TimerGetSecond() < 2.0)
			{
				g_pFrm->m_Robot->ExInput("13�Ź⿪", bDiStatus);
				if (bDiStatus)
				{
					CFunction::DelaySec(0.2);
					g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
					break;
				}
			}
			p2->FixBoard();
			p2->m_bDispEnable = true;
			p2->m_bLoadEnable = false;
		}
	}

	// �������
	p2->m_bUnloadSuccess = true;
	p2->m_bUnloadFinish = true;
/*	p2->m_bLoadEnable = true;*/

	while(!g_bStop || p2->m_bRunOnce)
	{		
		if (p2->m_tSysStatus == K_RUN_STS_PAUSE)
		{
			Sleep(10);
			continue;
		}

		if (1 != p2->UnloadStep_3Box())
		{
			g_bStop = TRUE;
			break;
		}

		Sleep(20);
	}

	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("ֹͣ");
	p2->m_bUnloadFinish = true;
	return 0;
}

// �����߳�
UINT CCmdRun::UnloadThread(LPVOID lparam)
{
	CCmdRun *p2 = (CCmdRun*)(lparam);
	p2->m_bUnloadFinish = false;
	p2->m_bUnloadSuccess = false;
	short rtn = 0;

	// ���ӷ���ģ��
	if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
	{
		if (p2->m_lsCtrl.CheckBoard())	
		{
			p2->AfxMessageBox("�������а壬���ڳ�����������");

			BOOL bInpStatus = FALSE;	
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("�ȴ���λ��Ҫ���ź�");
			while (TRUE)
			{
				rtn = p2->ExInput(_T("��λ��Ҫ���ź�1"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					p2->AfxMessageBox(_T("��ȡ��λ��Ҫ���źų���"));
					g_bStop = TRUE;
					return 0;
				}

				// ����λ��Ҫ���ź�
				if (!bInpStatus)
				{
					p2->ExOutput("����λ�����ź�1", TRUE);
					break;
				}
				Sleep(100);
			}

			p2->m_lsCtrl.PrepareOutBoard();
			if (1 != p2->m_lsCtrl.BackOutBoard())
			{
				p2->ExOutput("����λ�����ź�1", FALSE);
				p2->AfxMessageBox("������������!����ֹͣ");
				g_bStop = TRUE;

				return 0;
			}
			p2->ExOutput("����λ�����ź�1", FALSE);
		}
	}

	// ���㽺��λ�Ƿ��а�		
	BOOL bInpStatus[4];
	if (RTN_NONE != p2->ExInput(_T("14�Ź⿪"), bInpStatus[0]))
	{
		g_bStop = TRUE;
		return 0;
	}
	if (RTN_NONE != p2->ExInput(_T("13�Ź⿪"), bInpStatus[1]))
	{
		g_bStop = TRUE;
		return 0;
	}
	if (bInpStatus[0] || bInpStatus[1])
	{
		if (IDYES == ::AfxMessageBox("�㽺��λ�а�,�Ƿ���Ҫ����!", MB_YESNO))
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("�ȴ���λ��Ҫ���ź�");
			while (TRUE)
			{
				rtn = p2->ExInput(_T("��λ��Ҫ���ź�1"), bInpStatus[2]);
				if (RTN_NONE != rtn)
				{
					p2->AfxMessageBox(_T("��ȡ��λ��Ҫ���źų���"));
					g_bStop = TRUE;
					return 0;
				}

				// ����λ��Ҫ���ź�
				if (!bInpStatus[2])
				{
					p2->ExOutput("����λ�����ź�1", TRUE);
					break;
				}
				Sleep(100);
			}
			if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
			{
				p2->m_lsCtrl.PrepareOutBoard();
			}
			p2->ExOutput("11�Ŷ������׵�ŷ�", FALSE);	
			p2->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("����Ƥ��"));

			if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
			{
				if (1 != p2->m_lsCtrl.BackOutBoard())
				{
					p2->ExOutput("����λ�����ź�1", FALSE);

					p2->AfxMessageBox("������������!����ֹͣ");
					g_bStop = TRUE;

					return 0;
				}

				p2->ExOutput("����λ�����ź�1", FALSE);
			}
			else
			{
				CKFTimer tmTemp;
				tmTemp.TimerStart();
				while (TRUE)
				{
					rtn = p2->ExInput(_T("14�Ź⿪"), bInpStatus[0]);
					if (RTN_NONE != rtn)
					{
						g_bStop = TRUE;
						return 0;
					}
					if (bInpStatus[0])	break;
					else				Sleep(10);

					if (tmTemp.TimerGetSecond() > 6.0)
					{
						p2->AfxMessageBox("�㽺��λ����!");
						g_bStop = TRUE;
						return 0;
					}
				}
				tmTemp.TimerStart();
				while (TRUE)
				{
					rtn = p2->ExInput(_T("14�Ź⿪"), bInpStatus[0]);
					if (RTN_NONE != rtn)
					{
						p2->ExOutput("����λ�����ź�1", FALSE);
						g_bStop = TRUE;
						return 0;
					}
					if (!bInpStatus[0])	break;
					else				Sleep(10);

					if (tmTemp.TimerGetSecond() > 6.0)
					{
						p2->AfxMessageBox("�Ӳ�̨������!");
						p2->ExOutput("����λ�����ź�1", FALSE);
						g_bStop = TRUE;
						return 0;
					}
				}

				p2->ExOutput("����λ�����ź�1", FALSE);
			}

			p2->m_pController->AxisStop(T1_AXIS);
		}
	}

	// �������
	p2->m_bUnloadSuccess = true;
	p2->m_bUnloadFinish = true;
	p2->m_bLoadEnable = true;

	while(!g_bStop)
	{		
		if (p2->m_tSysStatus == K_RUN_STS_PAUSE)
		{
			Sleep(100);
			continue;
		}

		if (1 != p2->UnLoadStep())
		{
			g_bStop = TRUE;
			break;
		}

		Sleep(20);
	}

	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("ֹͣ");
	p2->m_bUnloadFinish = true;
	return 0;
}

CCmdRun::CCmdRun(void)
{
	m_pController	=	new CController;
	m_InPutCtrl		=	new CIOCtrl[IN_POINT+EX_IN_POINT];
	m_OutPutCtrl	=	new CIOCtrl[OUT_POINT+EX_OUT_POINT];

	m_hMsgReceiveWnd = NULL;

	m_tSysStatus = K_RUN_STS_STOP;
	m_dRotateAngle = 0.0f;
	m_dRotateAngleEX[0] = 0.0f;
	m_dRotateAngleEX[1] = 0.0f;
	m_dRotateAngleEX[2] = 0.0f;
	m_dRotateAngleEX[3] = 0.0f;
	m_iCurIndex = 0;
	m_CurToolNo = 0;
	m_iStepIndex=-1;
	inSenseCheckZ = -1;
	m_bModifiedTrack = TRUE;
	m_SocketOnline = FALSE;

	bRobotIsMoving = FALSE;
	m_bFlagRunning = FALSE;
	m_bIsHandPause = false;
	RunWCL_addSTrack=true;
	m_iLoop = 0;
	m_outputSts		= 0xffff;
	m_ExInputSts	= 0x0000;
	m_ExOutputSts	= 0x0000;
	inEmergecy = -1;//Ϊ�˿��ٷ���IO��,��������

	//inFeedEmergecy = -1; 
	//inUNFeedEmergecy = -1; 
	inWashPort = -1;
	outJetPort = -1;
	inPLCError = -1;

	for(int k=0;k<4;k++)
		g_CurDrawPos[k]=0.0;
	for(int t=0;t<MAX_AXIS_NUM;t++)
		m_GotoOffSet[t]=0.0;
	m_DispCYtime  =0;

	nFlyDataStartID=-1;
	nFlydataEndID =-1;
	bNeedFillCMPList=FALSE;

	m_MarkGropID  = 0;
	m_bAskSeparaSign = TRUE;
	m_bDoorSFSts= FALSE;

	m_bWorkStopCYSign = FALSE;  // �ӹ����赲���׵�λ�ź�
	m_bSqeStopCYSign= FALSE;    // �ֶ��赲���׵�λ�ź�
	m_bUpStopCYSign1= FALSE;	// ����������λ�ź�
	m_bUpStopCYSign2= FALSE;	// ����������λ�ź�

	m_bAutoPreFic = FALSE;		// �¹̻�ģʽ

	//////////////////////////////////////////////////////////////////////////
	// Load Unload ����
	m_nLoadStep = 1;
	m_nUnloadStep = 1;
	m_bLoadFinish = true;
	m_bLoadSuccess = false;
	m_bUnloadFinish = true;
	m_bUnloadSuccess = true;
	m_bCanWeightAndWash = false;

	m_bLdHomeSucceed = false;	// �����Ƿ��Ѿ��ɹ���������
	m_bLdHomeFinish = false;	// �����Ƿ��Ѿ���ɻ�������
	m_bUldHomeSucceed = false;	// �����Ƿ��Ѿ��ɹ���������
	m_bUldHomeFinish = false;	// �����Ƿ��Ѿ���ɻ�������

	m_bLoadEnable = false;
	m_bUnloadEnable = false;
	m_bDispEnable = false;

	m_nRePushCount = 0;		// �ظ����ϼ���
	m_nEmptyLayerCount = 0;	// �������ϲ����
	m_bDownLayerAfterWarning = true;

	m_bDispOpen = false;
	m_bHomeSuccess = false;

	m_runMode = RunMode_Wet;
	m_bRunOnce = false;
}
CCmdRun::~CCmdRun(void)
{
	RunListClear();

	if (m_pController!=NULL) 
		delete m_pController;

	if (m_InPutCtrl!=NULL) 
		delete []m_InPutCtrl;

	if (m_OutPutCtrl!=NULL) 
		delete []m_OutPutCtrl;

	if (dcp!=NULL) 
		delete []dcp;	

	TRACE(_T("\n~CCmdRun()"));
}

//////////////////////////////////////////////////////////////////////////
void CCmdRun::LoadIOMap()
{
	CString IInfo = _T("InPut");
	CString IIDInfo = _T("ͨ��IN#"); 
	CString IStsInfo = _T(""); 
	CString strinId,str;

	char temp[MAX_PATH];

	CGIniFile m_IniFile(CFunction::GetDirectory() + IOMAP_FILE_PATH);
// 	if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
// 	{	// ���߽��Ϸ�ʽ������MEMS����
// 		m_IniFile.SetPath(CFunction::GetDirectory() + IOMAP_FILE_PATH_OFFLINE);
// 	}

	for(int i=0;i<IN_POINT;i++)
	{
		strinId.Format(_T("%d"),i%IN_POINT);    //ID 
		str = IIDInfo+strinId; // 
		m_IniFile.Read(IInfo,str,temp);
		m_InPutCtrl[i].InitIOChannel((WORD)(_ttoi)(strinId),TYPE_INPUT,temp);
	}

	IIDInfo = _T("��չIN#");
	for(int i=IN_POINT;i<(IN_POINT+EX_IN_POINT);i++)
	{
		strinId.Format(_T("%d"),i%(IN_POINT+EX_IN_POINT));    //ID 
		str = IIDInfo+strinId; // 
		m_IniFile.Read(IInfo,str,temp);

		strinId.Format(_T("%d"),(i%(IN_POINT+EX_IN_POINT))-IN_POINT);    //ID 

		m_InPutCtrl[i].InitIOChannel((WORD)(_ttoi)(strinId),TYPE_INPUT,temp);
	}
	//////////////////////////////////////////////////////////////////////////
	IInfo = _T("OutPut");
	IIDInfo = _T("ͨ��OT#"); 
	IStsInfo = _T(""); 

	for(int j=0;j<OUT_POINT;j++)
	{
		strinId.Format(_T("%d"),j%OUT_POINT);    //ID 
		str = IIDInfo+strinId; // 
		m_IniFile.Read(IInfo,str,temp);
		m_OutPutCtrl[j].InitIOChannel((WORD)(_ttoi)(strinId),TYPE_OUTPUT,temp);
	}

	IIDInfo = _T("��չOT#");
	for(int j=OUT_POINT;j<(OUT_POINT+EX_OUT_POINT);j++)
	{
		strinId.Format(_T("%d"),j%(OUT_POINT+EX_OUT_POINT));    //ID 
		str = IIDInfo+strinId; // 
		m_IniFile.Read(IInfo,str,temp);

		strinId.Format(_T("%d"),(j%(OUT_POINT+EX_OUT_POINT))-OUT_POINT);    //ID 

		m_OutPutCtrl[j].InitIOChannel((WORD)(_ttoi)(strinId),TYPE_OUTPUT,temp);
	}
	//////////////////////////////////////////////////////////////////////////
	//CString stemp;	
	for (int i=0;i<TOOL_MAXNUM;i++)
	{
		for(int j=0;j<MAX_DISPEN;j++)
		{  
			// 			if( 0 == i)
			// 				str.Format(_T("Jet%d"),j+1);
			// 			if( 1 == i)
			// 				str.Format(_T("Disp%d"),j+1);
			// 			if( 2 == i)
			// 				str.Format(_T("IMG%d"),j+1);
			str.Format(_T("1�ŵ��̬����"));

			dcp[i].outBit[j]= ExOutputBitNameToNo(str);
			dcp[i].bBit = FALSE;
			if(dcp[i].outBit[j]==-1)
			{
				dcp[i].bBitEnable[j] = FALSE;
			}
			else
			{
				dcp[i].bBitEnable[j] = TRUE;
				dcp[i].bBit = TRUE;
			}
		}
	}
}
int CCmdRun::ExInputBitNameToNo(CString strBitName)
{
	for (int n=0;n<(IN_POINT+EX_IN_POINT);n++)
	{
		if (m_InPutCtrl[n].m_strIOName==strBitName)
		{
			return n;
		}
	}
	return -1;
}

int CCmdRun::ExOutputBitNameToNo(CString strBitName)
{
	for (int n=0;n<(OUT_POINT+EX_OUT_POINT);n++)
	{
		if (m_OutPutCtrl[n].m_strIOName==strBitName)
		{
			return n;
		}
	}

	return -1;
}

short CCmdRun::ExInput(CString strBitName,BOOL &bOn)
{
	BOOL bFound = FALSE;
	WORD rtsts;
	for (int n=0;n<(IN_POINT+EX_IN_POINT);n++)
	{
		if (m_InPutCtrl[n].m_strIOName==strBitName)
		{		   
			bFound=TRUE;

			if (n<IN_POINT)
			{
				m_InPutCtrl[n].GetInputSts(rtsts);
				bOn = (rtsts==RTN_ON) ? TRUE : FALSE;
			}
			else if (n<IN_POINT+EX_IN_POINT)
			{
				m_InPutCtrl[n].GetExInputSts(rtsts);
				bOn = (rtsts==RTN_ON) ? FALSE : TRUE;
			}
// 			if (rtsts==RTN_ON)
// 			{
// 				if (n<IN_POINT)
// 				{
// 					m_InPutCtrl[n].m_IsOpen=TRUE;
// 					bOn = TRUE;
// 				}
// 				else
// 				{
// 					m_InPutCtrl[n].m_IsOpen=FALSE;
// 					bOn = FALSE;
// 				}
// 			}
// 			else
// 			{
// 				if (n<IN_POINT)
// 				{
// 					m_InPutCtrl[n].m_IsOpen=FALSE;
// 					bOn = FALSE;
// 				}
// 				else
// 				{
// 					m_InPutCtrl[n].m_IsOpen=TRUE;
// 					bOn = TRUE;
// 				}
// 			}
			break;
		}
		else
		{
			bOn = FALSE;
		}
	}

	if (!bFound)
	{
		return RTN_NO_ASSIGNED;
	}
	else
		return RTN_NONE;
}

// ��ͷ�ֶ�����
short CCmdRun::NeedleDisp(int times,BOOL Open)
{
	short dwRet;
	unsigned short uHighTime = (unsigned short)g_pFrm->m_pSysParam->nOpenTime;
	unsigned short uLowTime = (unsigned short)g_pFrm->m_pSysParam->nCloseTime;
	if(Open == TRUE)
	{ 
		if(times == -1)
		{   // ������ʱΪ��1ʱһֱ��
			dwRet=m_pController->SetOutport((short)outJetPort,TRUE);
		}
		// ��������
		for(int i=0;i<times;i++)
		{
			dwRet=m_pController->SetOutport((short)outJetPort,TRUE);
			CFunction::DelaySec(uHighTime* 0.001);
			dwRet=m_pController->SetOutport((short)outJetPort,FALSE);
			CFunction::DelaySec(uLowTime* 0.001);
		}
	}else
	{
		dwRet=m_pController->SetOutport((short)outJetPort,FALSE);
	}
	return TRUE;
}

// ��ͷ�ֶ�����
short CCmdRun::NeedleDispEx(long PulseNum,long opentime,long closetime)
{
	short dwRet;
	unsigned short uHighTime = (unsigned short)opentime;
	unsigned short uLowTime = (unsigned short)closetime;
	long lPulseNum = PulseNum;

	for(int i=0;i<lPulseNum;i++)
	{
		dwRet=m_pController->SetOutport((short)outJetPort,TRUE);
		CFunction::DelaySec(uHighTime);
		dwRet=m_pController->SetOutport((short)outJetPort,FALSE);
		CFunction::DelaySec(uLowTime);
	}

	return TRUE;
}
// �ֶ��������
short CCmdRun::JetOutput(long PulseNum,BOOL bOn)
{
	short dwRet;
	unsigned short uHighTime = (unsigned short)g_pFrm->m_pSysParam->nCloseTime;
	unsigned short uLowTime = (unsigned short)g_pFrm->m_pSysParam->nOpenTime;
	long lPulseNum = PulseNum;
	CString str;
	dwRet=m_pController->JetOutp((short)outJetPort,uHighTime,uLowTime,lPulseNum,1,bOn);
	return dwRet;
}
// 
short CCmdRun::JetOutputEx(long PulseNum,long opentime,long closetime,BOOL bOn)
{
	short dwRet;
	unsigned short uHighTime = (unsigned short)closetime;
	unsigned short uLowTime  = (unsigned short)opentime;
	long lPulseNum = PulseNum;

	dwRet=m_pController->JetOutp((short)outJetPort,uHighTime,uLowTime,lPulseNum,1,bOn);

	return dwRet;
}

short CCmdRun::CommMotionCtrl(int port,bool bOn)
{
	UNREFERENCED_PARAMETER(port);
	UNREFERENCED_PARAMETER(bOn);
	// 	CString str;
	// 	int nBitNum = port;
	// 	str.Format("%d",nBitNum);
	// 	if ((theApp.m_Serial.m_Port[0].m_iComState == 1)||(theApp.m_Serial.m_Port[0].m_iComState == 2))
	// 	{
	// 		theApp.m_Serial.WritePlcROut(str,bOn);
	// 		Sleep(100);
	// 		//theApp.m_Serial.WritePlcROut(str,FALSE);
	// 	}
	// 	else
	// 	{
	// 		return RTN_PARAMETER_ERR;
	// 	}
	return RTN_NONE;
}

short CCmdRun::ResetFeed(int port)
{
	UNREFERENCED_PARAMETER(port);
	// 	CString str;
	// 	int nBitNum = port;
	// 	str.Format("%d",nBitNum);
	// 	if ((theApp.m_Serial.m_Port[0].m_iComState == 1)||(theApp.m_Serial.m_Port[0].m_iComState == 2))
	// 	{
	// 		theApp.m_Serial.WritePlcROut(str,TRUE);
	// 		Sleep(100);
	// 		theApp.m_Serial.WritePlcROut(str,FALSE);
	// 	}
	// 	else
	// 	{
	// 		return RTN_PARAMETER_ERR;
	// 	}
	return RTN_NONE;
}

short CCmdRun::ExOutput(CString strBitName,BOOL bOn)
{
	short dwRet; 			
	bool bFound=false;

	for (int n=0;n<(OUT_POINT+EX_OUT_POINT);n++)
	{
		if (m_OutPutCtrl[n].m_strIOName==strBitName)
		{		   
			bFound=true;

			if (n<OUT_POINT)
			{
				dwRet=m_OutPutCtrl[n].Output(bOn);	
			}
			else//��չIO
			{
				dwRet=m_OutPutCtrl[n].ExtOutput(bOn);	
			}
			if (dwRet!=RTN_NONE)
			{
				return RTN_NO_ASSIGNED;	
			}

			if (bOn==TRUE)
			{
				m_OutPutCtrl[n].m_IsOpen=TRUE;
			}
			else
			{
				m_OutPutCtrl[n].m_IsOpen=FALSE;
			}

			break;
		}
	}	

	if (!bFound)
		return RTN_NO_ASSIGNED;
	else
		return RTN_NONE;
}

int  CCmdRun::FixBoard(void)
{
	g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
	BOOL bInpStatus  = FALSE;	// ����˿��ź�
	short rtn		 = 0;		// ����ֵ		
	CKFTimer tmTemp;			// ��ʱ��

	if (1 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode)
	{
		rtn = ExOutput(_T("11�Ŷ������׵�ŷ�"), TRUE);
		CHECK_RTN_RTNVAL(_T("11�Ŷ������׵�ŷ��򿪳���"), rtn, 0);
		rtn = ExOutput(_T("����յ�ŷ�"), FALSE);

		rtn = ExOutput(_T("�������ŷ�"), TRUE);
		Sleep(200);

		tmTemp.TimerStart();
		while (1)
		{
			rtn = ExInput(_T("11�Ŷ�������1���"), bInpStatus);
			if (RTN_NONE != rtn)
			{
				AfxMessageBox(_T("��ⶥ�����׳���!"));
				return 0;
			}

			if (bInpStatus)
				break;

			if (tmTemp.TimerGetSecond() > 4)
			{
				AfxMessageBox(_T("�������׵�λ��ʱ!"));
				return 0;
			}
		}

		CFunction::DelaySec(0.1);	
	}
	else if (0 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode)
	{
		switch (g_pFrm->m_pRobotParam->m_nFixMode)
		{
		case FixMode_OnlyLift:
			ExOutput(_T("11�Ŷ������׵�ŷ�"), TRUE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11�Ŷ�������1���"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("��ⶥ�����׳���!"));
					return 0;
				}

				if (!bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("�������׵�λ��ʱ!"));
					return 0;
				}
			}

			CFunction::DelaySec(0.1);		
			break;

		case FixMode_1Clamp:
			ExOutput(_T("11�Ų�����׵�ŷ�"), TRUE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11�Ų�����׼��"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("��������׳���!"));
					return 0;
				}
				if (!bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("������״򿪵�λ��ʱ!"));
					return 0;
				}
			}
			CFunction::DelaySec(0.1);

			ExOutput(_T("11�Ų�����׵�ŷ�"), FALSE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11�Ų�����׼��"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("��������׳���!"));
					return 0;
				}
				if (bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("������׹رյ�λ��ʱ!"));
					return 0;
				}
			}

			ExOutput(_T("11�Ŷ������׵�ŷ�"), TRUE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11�Ŷ�������1���"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("��ⶥ�����׳���!"));
					return 0;
				}

				if (!bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("�������׵�λ��ʱ!"));
					return 0;
				}
			}

			CFunction::DelaySec(0.1);
			break;

		case FixMode_1Lift:
			ExOutput(_T("11�Ŷ������׵�ŷ�"), TRUE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11�Ŷ�������1���"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("��ⶥ�����׳���!"));
					return 0;
				}

				if (!bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("�������׵�λ��ʱ!"));
					return 0;
				}
			}

			CFunction::DelaySec(0.1);


			ExOutput(_T("11�Ų�����׵�ŷ�"), TRUE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11�Ų�����׼��"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("��������׳���!"));
					return 0;
				}
				if (!bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("������״򿪵�λ��ʱ!"));
					return 0;
				}
			}
			CFunction::DelaySec(0.1);

			ExOutput(_T("11�Ų�����׵�ŷ�"), FALSE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11�Ų�����׼��"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("��������׳���!"));
					return 0;
				}
				if (bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("������׹رյ�λ��ʱ!"));
					return 0;
				}
			}
			break;
		}
	}
	
	ExOutput(_T("12���赲���׵�ŷ�"), FALSE);
	ExOutput(_T("11�Ų�����׵�ŷ�"), FALSE);


	rtn = ExOutput(_T("�������ŷ�"), FALSE);
	Sleep(500);

	rtn = ExOutput(_T("�������ŷ�"), TRUE);
	return 1;
}

int  CCmdRun::UnFixBoard(void)
{
	if (1 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode)
	{
		ExOutput(_T("�������ŷ�"), FALSE);
		ExOutput(_T("����յ�ŷ�"), TRUE);
		Sleep(200);
		ExOutput(_T("����յ�ŷ�"), FALSE);
	}

	ExOutput(_T("11�Ŷ������׵�ŷ�"), FALSE);
	return 1;
}

int CCmdRun::ReverseBoard(void)	// �㽺�����еķ���ָ��
{
	if (AddOn_Reverse != g_pFrm->m_pRobotParam->m_nAddOn)
	{
		AfxMessageBox("û�����÷��幦��");
		return -1;
	}

	BOOL bInpStatus;
	short rtn = 0;

	ExOutput(_T("11�Ŷ������׵�ŷ�"), FALSE);
	// �ȴ��������̳ɹ�
	while (!m_bUnloadFinish || !m_bUnloadSuccess)
	{
		if (m_tSysStatus == K_RUN_STS_PAUSE)
		{
			Sleep(10);
			continue;
		}
		if (g_bStop)	return 0;

		Sleep(50);
	}

	// ����Ƥ��	
	m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

	if (1 != m_lsCtrl.PrepareReverse())
	{
		return -1;
	}

	CKFTimer tmTemp;
	tmTemp.TimerStart();

	while (true)
	{
		if (tmTemp.TimerGetSecond() > 10.0)
		{
			AfxMessageBox("����㽺�������������");
			return -1;
		}

		if (m_lsCtrl.m_bRevZero)
		{
			rtn = m_lsCtrl.ReadDiBit(smc_di_decSpd_2, bInpStatus);
		}
		else
		{
			rtn = m_lsCtrl.ReadDiBit(smc_di_decSpd_1, bInpStatus);
		}
		if (bInpStatus)		break;	

		Sleep(800);		
	}
	g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);	
	g_pFrm->m_Robot->m_lsCtrl.AxisStop(smc_mtr_track);
	Sleep(50);
	g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, -g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

	if (1 != m_lsCtrl.RevBoard())
	{
		return -1;
	}


	if (g_pFrm->m_Robot->m_lsCtrl.BackToTrack())
	{
		tmTemp.TimerStart();
		while (1)
		{
			g_pFrm->m_Robot->ExInput("13�Ź⿪", bInpStatus);
			if (bInpStatus)
			{
				g_pFrm->m_Robot->m_lsCtrl.AxisStop(smc_mtr_track);
				break;
			}
			if (tmTemp.TimerGetSecond() > 6.0)
			{
				g_pFrm->m_Robot->m_lsCtrl.AxisStop(smc_mtr_track);
				g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
				AfxMessageBox("��������㽺λ����!");
				return -1;
			}
		}
		tmTemp.TimerStart();
		while (1)
		{
			g_pFrm->m_Robot->ExInput("13�Ź⿪", bInpStatus);
			if (!bInpStatus)
			{
				break;
			}
			if (tmTemp.TimerGetSecond() > 4.0)
			{
				AfxMessageBox("��������㽺λʱ���ڵ㽺��λ����");
				return -1;
			}
		}

		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, -g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax/4, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		g_pFrm->m_Robot->ExOutput("12���赲���׵�ŷ�", TRUE);
		tmTemp.TimerStart();
		while (tmTemp.TimerGetSecond() < 2.0)
		{
			g_pFrm->m_Robot->ExInput("12���赲���׼��", bInpStatus);
			if (bInpStatus)
			{
				break;
			}
		}
		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

		tmTemp.TimerStart();
		while (tmTemp.TimerGetSecond() < 2.0)
		{
			g_pFrm->m_Robot->ExInput("13�Ź⿪", bInpStatus);
			if (bInpStatus)
			{		
				g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax/2, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

				CFunction::DelaySec(0.2);
				g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
				break;
			}
		}
	}
	else
	{
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
		return -1;
	}

	return FixBoard();
}

int	  CCmdRun::LoadStep_3Box(void)	// ����ʽ3���Ϻ����ϲ���
{
	BOOL bInpStatus1 = FALSE;	// ����˿��ź�
	int  rtn = 0;

	switch (m_nLoadStep)
	{
	case 1:		// �ȴ�����ʹ��
		if (m_bLoadEnable)
		{
			// ���赲����
			if (RTN_NONE != ExOutput(_T("12���赲���׵�ŷ�"), TRUE))
			{
				g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("���赲���׳���");
				m_bLoadFinish = true;
				m_bLoadSuccess = false;
				return -1;
			}
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("�յ� [��������ź�]");
			m_bLoadEnable = false;
			m_nRePushCount = 0;		// �ظ����ϼ���
			m_nEmptyLayerCount = 0;	// �������ϲ����
			// 13��Ӧ����
			rtn = ExInput(_T("13�Ź⿪"), bInpStatus1);
			if (RTN_NONE != rtn)
				return -1;
			if (bInpStatus1)	m_nLoadStep = 20;
			else				m_nLoadStep = 10;
		}

		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("�ȴ� [��������ź�]");
		Sleep(50);
		break;

	case 10:	// �Ƹ�����
		m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		rtn = LoadUnLoad_loadPushOut(bInpStatus1);
		CHECK_RTN_RTNVAL("�Ƹ����϶�������",rtn, -1);
		rtn = LoadUnLoad_loadPullBack();
		CHECK_RTN_RTNVAL("�Ƹ��˻ض�������",rtn, -1);

		m_nRePushCount++;
		if (!bInpStatus1)
		{
			if (m_nRePushCount < g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdRePushout)
			{
				Sleep(100);
				break;
			}
			else
			{
				m_nRePushCount = 0;
				m_nLoadStep = 11;
			}
		}
		else
		{
			m_nLoadStep = 15;
		}

		m_tmLoad.TimerStart();
		break;

	case 11:	// ��һ��Ƭ
		{
			m_nEmptyLayerCount++;
			if (m_nEmptyLayerCount >= g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdEmptyWarning)
			{
				CString str;
				str.Format("����%d��û�м�⵽����,\r\nֹͣ�����:[��ֹ]\r\n������һ���:[����]\r\n�㽺��λ�Ѿ��а��:[����]", m_nEmptyLayerCount);
				int msgRtn = AfxMessageBox(str, MB_ABORTRETRYIGNORE);
				m_nEmptyLayerCount = 0;
				if (IDABORT == msgRtn)
				{
					g_bStop = TRUE;
					return -1;
				}
				else if (IDRETRY == msgRtn)
				{
					m_nLoadStep = 10;
					break;
				}
				else if (IDIGNORE == msgRtn)
				{
					m_nLoadStep = 15;
					m_tmLoad.TimerStart();
					break;
				}
			}

			int crtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId;
			if (crtLayer < g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount-1)
			{
				rtn = LoadUnLoad_loadLayerMove(crtLayer+1);
				CHECK_RTN_RTNVAL(_T("���Ϻ�����һ�����"), rtn, -1);
				m_nLoadStep = 10;
			}
			else
			{	// ��һ�Ϻ�
				m_nLoadStep = 12;
				break;
			}
		}
		break;

	case 12:	// �Ϻ�����һ��
		{
			int crtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId;
			if (crtBox<2)
			{
				rtn = LoadUnLoad_loadBoxMove(crtBox+1);
				CHECK_RTN_RTNVAL(_T("�Ϻ��ƶ�����!"), rtn, -1);
				m_nLoadStep = 13;
				break;
			}
			else
			{
				LoadUnLoad_loadBoxMove(0, false);
				if (IDOK == AfxMessageBox("�Ϻ����꣬�����Ϻк��:[ȷ��]\r\n  ֹͣ���̵�:[ȡ��]"))
				{
					m_pController->WaitAxisStop(LOAD_AXIS);
					m_nLoadStep = 13;
					break;
				}
				else
				{
					g_bStop = TRUE;
					return -1;
				}
			}
		}
		break;

	case 13:
		rtn = ExInput("Load���Ϻм��", bInpStatus1);
		CHECK_RTN_RTNVAL(_T("Load���Ϻм�����"), rtn, -1);

		// �ƶ�����һ�����һ���Ϻ�
		if (bInpStatus1)
			m_nLoadStep = 10;
		else
			m_nLoadStep = 12;

		break;

	case 15: 
		if (m_tmLoad.TimerGetSecond() > 6)
		{
			if (IDOK == AfxMessageBox(_T("��ڽ��忨��!���¼���:[ȷ��], \r\nֹͣ��:[ȡ��]"), MB_OKCANCEL))
			{
				m_tmLoad.TimerStart();
				break;
			}
			else
			{
				return -1;
			}
		}

		// 13��Ӧ����
		m_bCanWeightAndWash = true;
		rtn = ExInput(_T("13�Ź⿪"), bInpStatus1);
		if (RTN_NONE != rtn)
			return -1;
		if (bInpStatus1)
		{
			m_nLoadStep = 20;
			break;
		}

		Sleep(50);
		break;

	case 20:	// Ƥ����������
		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		Sleep(300);
		// �������
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
		m_nLoadStep = 30;
		break;

	case 30:	// �а�
		rtn = FixBoard();
		if (1 != rtn)
		{
			AfxMessageBox(_T("�а嶯��ʧ��"));
			return -1;
		}
		m_bDispEnable = true;
		m_nLoadStep = 40;
		break;

	case 40:	// ��һ��Ƭ
		{
			int crtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId;
			if (crtLayer < g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount-1)
			{
				LoadUnLoad_loadLayerMove(crtLayer+1);
				m_nLoadStep = 1;
				break;
			}
			else
			{	// ��һ�Ϻ�
				m_nLoadStep = 42;
				break;
			}
		}
		break;

	case 42:	// �Ϻ�����һ��
		{
			int crtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId;
			if (crtBox<2)
			{
				rtn = LoadUnLoad_loadBoxMove(crtBox+1);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox("���Ϻ��ƶ�����!");
					g_bStop = TRUE;
					return -1;
				}
				m_nLoadStep = 44;
				break;
			}
			else
			{
				LoadUnLoad_loadBoxMove(0, false);
				if (IDOK == AfxMessageBox("�Ϻ����꣬�����Ϻк��:[ȷ��]  ֹͣ���̵�:[ȡ��]"))
				{
					m_pController->WaitAxisStop(LOAD_AXIS);
					m_nLoadStep = 44;
					break;
				}
				else
				{
					m_bRunOnce = true;
					g_bStop = TRUE;
					return -1;
				}
			}
		}
		break;

	case 44:
		rtn = ExInput("Load���Ϻм��", bInpStatus1);
		if (RTN_NONE != rtn)
		{
			AfxMessageBox("�Ϻм���źų���!");
			g_bStop = TRUE;
			return -1;
		}

		if (bInpStatus1)
		{
			m_nLoadStep = 1;
			break;
		}
		else
		{
			m_nLoadStep = 42;
			break;
		}
		break;

	default:
		{
			CString str;
			str.Format("����Ľ��嶯�����:[%d]", m_nLoadStep);
			AfxMessageBox(str);
		}
		break;
	}

	return 1;
}

int  CCmdRun::LoadStep(void)
{
	BOOL bInpStatus1 = FALSE;	// ����˿��ź�
	BOOL bInpStatus2 = FALSE;	// ����˿��ź�
	BOOL bInpStatus3 = FALSE;	// ����˿��ź�
	int  rtn = 0;
	switch (m_nLoadStep)
	{
	case 1:		// �ȴ�����ʹ��
		if (m_bLoadEnable)
		{
			// ���赲����
			if (RTN_NONE != ExOutput(_T("12���赲���׵�ŷ�"), TRUE))
			{
				g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("���赲���׳���");
				m_bLoadFinish = true;
				m_bLoadSuccess = false;
				return -1;
			}
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("����ʹ�� OK");
			m_bLoadEnable = false;

			// 13��Ӧ����
			rtn = ExInput(_T("13�Ź⿪"), bInpStatus1);
			if (RTN_NONE != rtn)
				return -1;
			if (bInpStatus1)	m_nLoadStep = 20;
			else				m_nLoadStep = 10;
		}

		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("�ȴ�����ʹ��");
		Sleep(50);
		break;

	case 10:	// �ȴ���λ���źź���ڹ����и�Ӧ
		rtn = ExOutput(_T("����λҪ���ź�1"), TRUE);
		if (RTN_NONE != rtn)
			return -1;

		rtn = ExInput(_T("��λ�������ź�1"), bInpStatus1);
		if (RTN_NONE != rtn)
			return -1;
		rtn = ExInput(_T("11�Ź⿪"), bInpStatus2);
		if (RTN_NONE != rtn)
			return -1;

		if (bInpStatus1 || bInpStatus2)
		{	// ����Ƥ��	
			g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
			m_tmLoad.TimerStart();
			m_nLoadStep = 15;
			m_bCanWeightAndWash = true;
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("���忪ʼ!");
		}
		else
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("�ȴ���λ�������ź�!");
			Sleep(20);
		}
		break;

	case 15: 
// 		if (m_tmLoad.TimerGetMs() > g_pFrm->m_pRobotParam->m_dTimeAskPre)
// 		{
// 			ExOutput(_T("����λҪ���ź�1"), FALSE);
// 		}

		if (m_tmLoad.TimerGetSecond() > 6)
		{
			AfxMessageBox(_T("��ڽ��峬ʱ!"));
			return -1;
		}

		// 13��Ӧ����
		rtn = ExInput(_T("13�Ź⿪"), bInpStatus1);
		if (RTN_NONE != rtn)
			return -1;
		if (bInpStatus1)
		{
			rtn = ExOutput(_T("����λҪ���ź�1"), FALSE);
			if (RTN_NONE != rtn)
				return -1;

			rtn = ExInput(_T("11�Ź⿪"), bInpStatus2);
			rtn = ExInput(_T("12�Ź⿪"), bInpStatus3);
			if (bInpStatus3||bInpStatus2)
			{
				AfxMessageBox("11��12��翪����13ͬʱ���ź�,����13��翪�����Ӧ!!");
				return -1;
			}
			m_nLoadStep = 20;
			break;
		}
		else
		{
			Sleep(20);
		}
		break;

	case 20:	// Ƥ����������
		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax/2.0, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		Sleep(300);
		// �������
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
		m_nLoadStep = 30;
		break;

	case 30:	// �а�
		rtn = FixBoard();
		if (1 != rtn)
		{
			AfxMessageBox(_T("�а嶯��ʧ��"));
			return -1;
		}
		m_bDispEnable = true;
		m_nLoadStep = 1;
		break;
	}

	return 1;
}

int	  CCmdRun::UnloadStep_3Box(void)	// ����ʽ3���Ϻ����ϲ���
{
	BOOL bInpStatus = FALSE;	// ����˿��ź�
	short rtn = 0;

	switch (m_nUnloadStep)
	{
	case 1:	
		if (m_bUnloadEnable)
		{	
			m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("�㽺��ɣ��յ������ź�!");
			UnFixBoard();
			m_bUnloadEnable = false;
			m_bUnloadFinish = false;
			m_nUnloadStep = 10;
			m_tmUnload.TimerStart();

			break;			
		}

		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("�ȴ��㽺���,�����ź�");
		Sleep(50);
		break;

	case 10:	// �ȴ�����⿪��Ӧ���ź�
		rtn = ExInput(_T("14�Ź⿪"), bInpStatus);
		if (RTN_NONE != rtn)
		{
			g_bStop = TRUE;
			return 0;
		}

		if (bInpStatus)
		{
			m_nUnloadStep = 20;
			break;
		}
		else
		{
			if (m_tmUnload.TimerGetSecond() > 6.0)
			{
				AfxMessageBox("�㽺��λ����!���ڹ⿪δ��Ӧ�����ϳ�ʱ!");
				m_bDownLayerAfterWarning = false;
				m_pController->AxisStop(1<<T1_AXIS);
				g_bStop = TRUE;
				return 0;
			}
			Sleep(10);
		}
		break; 

	case 20:	// �ȴ���Ƭ��������⿪
		rtn = ExInput(_T("14�Ź⿪"), bInpStatus);
		if (RTN_NONE != rtn)
		{
			g_bStop = TRUE;
			return 0;
		}

		if (!bInpStatus)
		{
			m_nUnloadStep = 22;
			break;
		}
		else
		{
			if (m_tmUnload.TimerGetSecond() > 2.0)
			{
				AfxMessageBox("���ڿ���!���ڹ⿪��һֱ������!");
				m_bDownLayerAfterWarning = false;
				m_pController->AxisStop(1<<T1_AXIS);
				g_bStop = TRUE;
				return 0;
			}
			Sleep(10);
		}
		break;

	case 22:	// ��ʱ�Ƴ�����ʱʱ��
		Sleep(DWORD(g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay * 1000));
		rtn = LoadUnLoad_unloadPushOut();
		if (RTN_NONE != rtn)
		{
			g_bStop = TRUE;
			return 0;
		}
		m_pController->AxisStop(1<<T1_AXIS);
		rtn = LoadUnLoad_unloadPullBack();
		m_bRunOnce = false;
		m_nUnloadStep = 30;
		break;

	case 30:	// ����Ƿ���
		rtn = ExInput(_T("14�Ź⿪"), bInpStatus);
		if (RTN_NONE != rtn)
		{
			g_bStop = TRUE;
			return 0;
		}
		if (bInpStatus)
		{
			AfxMessageBox("�Ƴ��Ϻ���ڹ⿪����Ӧ�����ϣ����ڿ��ϻ��ߴ������ź��������˹�����");
		}
		m_nUnloadStep = 40;
		break;

	case 40:	// �Ϻ�����һ��
		{
			m_bLoadEnable = true;
			int crtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId;
			int crtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;

			if ((g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount-1) == crtLayer)
			{
				if (2 == crtBox)
				{
					LoadUnLoad_unloadBoxMove(0);
					if (IDOK == AfxMessageBox(_T("�ѵ��Ϻ��յ㣬�����ÿ��Ϻк�:[ȷ��]  \r\n�˳����̰�:[ȡ��]"), MB_OKCANCEL))
					{
						m_pController->WaitAxisStop(UNLOAD_AXIS);
						m_nUnloadStep = 50;
						break;
					}
					else
					{
						g_bStop = TRUE;
						return 0;
					}
				}
				else
				{
					LoadUnLoad_unloadBoxMove(crtBox+1);
				}
				m_nUnloadStep = 50;
			}
			else
			{
				rtn = (short)LoadUnLoad_unloadLayerMove(crtLayer+1);
				m_nUnloadStep = 1;
			}
		}
		break;

	case 50:// ����Ϻ�
		rtn = ExInput("Unload���Ϻм��", bInpStatus);
		if (!bInpStatus)
		{
			int crtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;
			if (2 == crtBox)
			{
				LoadUnLoad_unloadBoxMove(0, false);

				if (IDOK == AfxMessageBox("���²�û�м�⵽���Ϻ�,�뻻���Ϻк��:[ȷ��]  ֹͣ��:[ȡ��]", MB_OKCANCEL))
				{
					m_pController->WaitAxisStop(UNLOAD_AXIS);
					break;
				}
				else
					g_bStop = TRUE;
			}
			else
			{
				LoadUnLoad_unloadBoxMove(crtBox+1);
				break;
			}
		}
		else
		{
			m_nUnloadStep = 1;
		}
		break;

	default:
		{
			CString str;
			str.Format("���ϲ������:[%d]δ����", m_nUnloadStep);
			AfxMessageBox(str);
		}
		break;
	}	

	return 1;
}

int  CCmdRun::UnLoadStep(void)
{
	BOOL bInpStatus = FALSE;	// ����˿��ź�
	short rtn = 0;

	switch (m_nUnloadStep)
	{
	case 1:		// �ȴ�����
		if (m_bUnloadEnable)
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("�ȴ����� OK");
			UnFixBoard();
			m_bUnloadEnable = false;
			m_bUnloadFinish = false;
			m_nUnloadStep = 5;
			break;
		}

		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("�ȴ�����");
		Sleep(50);
		break;

	case 5:
		rtn = ExInput(_T("13�Ź⿪"), bInpStatus);	
		if (RTN_NONE != rtn)
		{
			m_pController->AxisStop(1<<T1_AXIS);
			AfxMessageBox(_T("����ʱ��ȡ13�Ź⿪����"));
			return -1;
		}
		if (!bInpStatus)	// ����Ҫ����
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("�㽺λ�ް�!�������");
			m_bLoadEnable = true;
			m_bUnloadFinish = true;
			m_bUnloadSuccess = true;
			m_nUnloadStep = 1;
			break;
		}

		ExOutput(_T("12���赲���׵�ŷ�"), FALSE);
		ExOutput(_T("����λ�����ź�1"), TRUE);
		m_nUnloadStep = 10;
		break;

	case 10:	// �ȴ���λ��Ҫ��
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("�ȴ���λ��Ҫ��"));
		rtn = ExInput(_T("��λ��Ҫ���ź�1"), bInpStatus);
		if (RTN_NONE != rtn)
		{
			AfxMessageBox(_T("��ȡ��λ��Ҫ���źų���"));
			return -1;
		}
		if (bInpStatus)
		{
			m_nUnloadStep = 20;
		}
		Sleep(20);
		break;

	case 20:		// ����Ƥ��	
		m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("����Ƥ��"));
		if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
		{
			m_lsCtrl.PrepareOutBoard();
		}
		m_tmUnload.TimerStart();
		m_nUnloadStep = 30;
		break;

	case 30:	// 14�Ź⿪������
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("�ȴ�������14�⿪"));
		rtn = ExInput(_T("14�Ź⿪"), bInpStatus);	
		if (RTN_NONE != rtn)
		{
			m_pController->AxisStop(1<<T1_AXIS);
			AfxMessageBox(_T("����ʱ��ȡ14�Ź⿪����"));
			return -1;
		}
		if (bInpStatus)
		{
			m_nUnloadStep = 40;
			m_tmUnload.TimerStart();
			break;
		}

		if (m_tmUnload.TimerGetSecond() > 4)
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("14�Ź⿪δ������"));
			m_pController->AxisStop(1<<T1_AXIS);
			AfxMessageBox(_T("����ʱ����"));
			return -1;
		}
		break;

	case 40:	// ������14�Ź⿪
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("�ȴ�������14�⿪"));
		rtn = ExInput(_T("14�Ź⿪"), bInpStatus);	
		if (RTN_NONE != rtn)
		{
			g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
			AfxMessageBox(_T("����ʱ��ȡ14�Ź⿪����"));
			return -1;
		}
		if (!bInpStatus)
		{
			if (AddOn_None == g_pFrm->m_pRobotParam->m_nAddOn
				|| AddOn_Heat == g_pFrm->m_pRobotParam->m_nAddOn)
			{
				m_nUnloadStep = 50;
			}
			else
			{
				m_nUnloadStep = 60;
			}
			break;
		}

		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("�ȴ�������14�⿪");
		if (m_tmUnload.TimerGetSecond() > 4)
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("14�⿪����"));
			m_pController->AxisStop(1<<T1_AXIS);
			AfxMessageBox(_T("����ʱ����"));
			g_bStop = TRUE;
			return -1;
		}
		break;

	case 50:	// �رճ����ź�
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("�������");
		ExOutput(_T("����λ�����ź�1"), FALSE);
		m_pController->AxisStop(1<<T1_AXIS);
		m_bLoadEnable = true;
		m_bUnloadSuccess = true;
		m_bUnloadFinish = true;
		m_nUnloadStep = 1;
		break;

	case 60:	// ���������
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("���������"));
		if (1 != m_lsCtrl.BackOutBoard())
		{
			return -1;
		}
		m_nUnloadStep = 50;
		break;
	}

	return 1;
}

/*====================================
*�������ƣ�CheckIO
*�������ܣ�������������˿�״̬
*       m_inputSts=1 ,InTemp=0  sts=1   ����
*       m_inputSts=0 ,InTemp=1  sts=-2  �˿ڷ����仯(�½���)
*       m_inputSts=0 ,InTemp=0  sts=0   �˿ڷ����仯(�仯������)
*       m_inputSts=1 ,InTemp=1  sts=2   �˿ڷ����仯(������)
*����˵����
*����ֵ��
*�汾��ʷ��
====================================*/
void CCmdRun::CheckIO()
{
	static BOOL bEmergecy       = FALSE;   // �豸��ͣ
	static BOOL bDoorSf         = FALSE;   // ��ȫ��
	static long inOldStatus = 0xFFFF;  
	long inTemp = 0xFFFF;

	int sts=0;
	if(m_hMsgReceiveWnd == NULL)	// ��û���źŽ��մ���
	{
		return;
	}

	if(m_pController->IsInitOk())
	{
		// ��ȡ��ǰ�ź�״̬
		m_pController->GetInputSts(&m_inputSts);         // ��ȡ�忨�����״̬
		m_pController->GetOutputSts(&m_outputSts);       // ��ȡ���������״̬
		m_pController->GetExInputSts(0, &m_exInputSts);	 // ��ȡ��չ������״̬
		bRobotIsMoving = m_pController->IsMoving(CRD1);

		//��ͣ������
		if((m_inputSts&(0x01<<inEmergecy))==(0x01<<inEmergecy))
		{
			bEmergecy = TRUE;		
			::SendMessage(m_hMsgReceiveWnd, WM_MSG_INPUT_EVENT, inEmergecy, TRUE);
		}

		// �����趨����
		if(g_pFrm->m_dlgMachineSet.GetSafeHwnd() && g_pFrm->m_dlgMachineSet.IsWindowVisible())
		{
			int index = g_pFrm->m_dlgMachineSet.m_tabCtrl.GetCurSel();
			if (2 == index)
			{
				g_pFrm->m_dlgMachineSet.m_pageLoadUnload.UpdataIOStatus(m_exInputSts);
			}
		}

		// ˫���趨����
		if (g_pFrm->m_DlgDualValve.GetSafeHwnd() && g_pFrm->m_DlgDualValve.IsWindowVisible())
		{
			g_pFrm->m_DlgDualValve.UpdateInputStatus(m_inputSts);
		}

		// IO ״̬��ʾ����
		if(g_pFrm->m_dlgIOCtrl.GetSafeHwnd() && g_pFrm->m_dlgIOCtrl.IsWindowVisible())
		{
			int index = g_pFrm->m_dlgIOCtrl.m_tabCtrl.GetCurSel();

			if(index == 0)   // �����״̬
			{  
				//ˢ�µ�ǰ�����״̬
				g_pFrm->m_dlgIOCtrl.pageInput.UpdateIOSts(m_inputSts);
				//ˢ�µ�ǰ��չ��״̬
				g_pFrm->m_dlgIOCtrl.pageInput.UpdateExIOSts(m_exInputSts);
			}
			else if(index == 1)	// �����״̬
			{
				if(g_pFrm->m_pSysParam->nAutoMode==1)
				{
					if(g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_JET)->IsWindowVisible()==TRUE)
						g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_JET)->ShowWindow(SW_HIDE);
					if(g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_OPEN)->IsWindowVisible()==TRUE)
						g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_OPEN)->ShowWindow(SW_HIDE);
					if(g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->IsWindowVisible()==TRUE)
						g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->ShowWindow(SW_HIDE);
				}
				else
				{
					if(g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_OPEN)->IsWindowVisible()==FALSE)
						g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_OPEN)->ShowWindow(SW_SHOW);
					if(g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->IsWindowVisible()==FALSE)
						g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->ShowWindow(SW_SHOW);

					// ���ݹ����������� Io�������ֶ����书��״̬
					if (g_pFrm->m_pSysParam->nToolType==0)
					{    
						if(g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_JET)->IsWindowVisible()==FALSE)
							g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_JET)->ShowWindow(SW_SHOW);
					}
					else
					{
						if(g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_JET)->IsWindowVisible()==TRUE)
							g_pFrm->m_dlgIOCtrl.pageOutput.GetDlgItem(IDC_DLG_IO_OUT_JET)->ShowWindow(SW_HIDE);
					}
				}
				g_pFrm->m_dlgIOCtrl.pageOutput.UpdateIOSts(m_outputSts);
			}
			else if (2 == index)	// ������Խ���
			{
				g_pFrm->m_dlgIOCtrl.pageTrack.UpdataIOState(m_inputSts, m_exInputSts, m_outputSts);
			}
		}

		//��ͣ���±���λ
		if (bEmergecy&&((m_inputSts&(0x01<<inEmergecy))!=(0x01<<inEmergecy)))
		{
			bEmergecy = FALSE;
			::SendMessage(m_hMsgReceiveWnd, WM_MSG_INPUT_EVENT, ExInputBitNameToNo(_T("ֹͣ����")), -2);//
		}

		inTemp = inOldStatus ^ m_inputSts;
		inOldStatus = m_inputSts;

		for(int i=0; i<MAX_I0_NUM; i++)
		{
			if(m_inputSts & (1<<i))
				sts=1;
			else 
				sts=0;
			if(inTemp & (1<<i))//1�˿ڷ����ı�
			{
				if(m_inputSts & (1<<i))
					sts=2;
				else 
					sts=-2;
			}

			// ���Ǽ�ͣ�ź�
			if ((i != inEmergecy))  
				::SendMessage(m_hMsgReceiveWnd, WM_MSG_INPUT_EVENT, i, sts);
		}
	}
}

// ���״̬
void CCmdRun::CheckStatus()
{
	double dAxisPos[8];
	long   axissts[8];
	CString str;

	if(m_hMsgReceiveWnd == NULL) 
		return;

	// ���λ��ˢ��
	if(!m_pController->IsInitOk())
	{
		for(unsigned short j=0; j<m_pController->g_ExMtnPara->axisNum; j++)
			dAxisPos[j] = 0;//double(rand())/RAND_MAX*400;
	}
	else
	{
		//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
		g_pFrm->m_Precision.GetCurPosMM();
		for(unsigned short j=0; j<3; j++)
		{
			dAxisPos[j] = m_pController->g_ExMtnPara->dfCurpos[j];
			m_pController->GetAxisSts(j+1,&axissts[j]);
		}
	}

	::SendMessage(m_hMsgReceiveWnd, WM_MSG_POS_EVENT, (WPARAM)dAxisPos, (LPARAM)axissts);
	// ˫�����ý���,ˢ��Z������
	if (g_pFrm->m_DlgDualValve.GetSafeHwnd() && g_pFrm->m_DlgDualValve.IsWindowVisible())
	{
		g_pFrm->m_DlgDualValve.UpdateZPos(dAxisPos[Z_AXIS]);
	}

	
	//������������ʱ��
	if(m_tSysStatus==K_RUN_STS_RUN)
	{
		GetLocalTime(&st2); 
		SystemTimeToFileTime(&st2,&g_pFrm->m_pSysParam->t2.ft);
		g_pFrm->m_pSysParam->snapSecond =(g_pFrm->m_pSysParam->t2.val-g_pFrm->m_pSysParam->t1.val)/10000000.0;
		str.Format("CT(s):%.1f ", g_pFrm->m_pSysParam->snapSecond);
		g_pFrm->SetPanelTextEx(IDS_CYCLE_TIME, str);
		str.Format("%.1f", g_pFrm->m_pSysParam->snapSecond);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_CYCLETIME,str);
	}
	//�������緢������״̬
	///��ǰʱ��
	CTime time; 
	time=CTime::GetCurrentTime(); 
	str = time.Format("%Y-%m-%d %H:%M:%S ");
	g_pFrm->SetPanelTextEx(IDS_CUR_CLOCK,str); 

	CTimeSpan ctSpan = time-g_pFrm->m_pSysParam->m_SystemBeginTime;
	str = ctSpan.Format(_T("%D��-%Hʱ-%M��-%S��"));
	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_RUNTIME,str);

	//���ݹ�����������״̬����Ӧ״̬
	if (g_pFrm->m_pSysParam->nToolType==0)
	{   
		g_pFrm->SetPanelTextEx(IDS_TOOL_STATUS, "��ǰģʽ:���䷧");
// 		if(g_pFrm->m_pSysParam->nJetType == 0)
// 			g_pFrm->SetPanelTextEx(IDS_TOOL_STATUS, "��ǰģʽ:����ʽ���䷧");
// 		else
// 			g_pFrm->SetPanelTextEx(IDS_TOOL_STATUS, "��ǰģʽ:ѹ��ʽ���䷧");
	}
	else if(g_pFrm->m_pSysParam->nToolType==1)
	{
		g_pFrm->SetPanelTextEx(IDS_TOOL_STATUS, "��ǰģʽ:��Ͳ");
	}
	else if(g_pFrm->m_pSysParam->nToolType==2)
	{
		g_pFrm->SetPanelTextEx(IDS_TOOL_STATUS, "��ǰģʽ::ͼ��ģʽ");
	}
}
// ����ѹ������  ������Jet8ϵ�У�
void CCmdRun::JetPressCtrl(double dDate)
{
	unsigned short temp;
	temp = (unsigned short)(dDate*10.0*65535/6);
	g_pFrm->m_Robot->m_pController->SetExtADDAVal(0,0,temp);
}
// ��Ͳѹ������  ������EFD/Jet8ϵ�У�
void CCmdRun::TubePressCtrl(double dDate) 
{  
	unsigned short temp;
	temp = (unsigned short)(dDate*10.0*65535/6);
	g_pFrm->m_Robot->m_pController->SetExtADDAVal(0,1,temp);
}
void CCmdRun::Free()
{
	//�ͷ�֮ǰ���߳�����return����
	g_bClose = TRUE;
	g_bStop = TRUE;
	g_bUpdate = TRUE;
	//////////////////////////////
	Sleep(50);

	//////////////////////////////////////////////////////////////////////////
	// �����߳�
	if (g_pLoadThread != NULL)
	{
		switch(WaitForSingleObject(g_pLoadThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pLoadThread->m_hThread, 0);
			break;
		}
		g_pLoadThread=NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	// �ź����߳� 
	if (g_pLightsetThread != NULL)
	{
		switch(WaitForSingleObject(g_pLightsetThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pLightsetThread->m_hThread, 0);
			break;
		}
		g_pLightsetThread=NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// �����߳�
	if (g_pUnloadThread != NULL)
	{
		switch(WaitForSingleObject(g_pUnloadThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pUnloadThread->m_hThread, 0);
			break;
		}
		g_pUnloadThread=NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// IO����߳�
	if(g_pMonitorThread != NULL)
	{
		switch(WaitForSingleObject(g_pMonitorThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pMonitorThread->m_hThread, 0);
			break;
		}
		g_pMonitorThread=NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// ����ˢ���߳�
	if(g_pUpdateThread != NULL)
	{
		switch(WaitForSingleObject(g_pUpdateThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pUpdateThread->m_hThread, 0);
			break;
		}
		g_pUpdateThread=NULL;
	}	
	if(g_pLineSignStsThread != NULL)
	{
		switch(WaitForSingleObject(g_pLineSignStsThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pLineSignStsThread->m_hThread, 0);
			break;
		}
		g_pLineSignStsThread=NULL;
	}
	if(g_pAutoRPSThread != NULL)
	{
		switch(WaitForSingleObject(g_pAutoRPSThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pAutoRPSThread->m_hThread, 0);
			break;
		}
		g_pAutoRPSThread=NULL;
	}
	//�ͷ�֮ǰ���߳�g_pPlayThread����return����
	if(g_pPlayThread != NULL)
	{
		switch(WaitForSingleObject(g_pPlayThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pPlayThread->m_hThread, 0);
			break;
		}
		g_pPlayThread=NULL;
	}	
	if(g_pLoadReadyThread != NULL)
	{
		switch(WaitForSingleObject(g_pLoadReadyThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pLoadReadyThread->m_hThread, 0);
			break;
		}
		g_pLoadReadyThread=NULL;
	}
	if(g_pUnloadThread != NULL)
	{
		switch(WaitForSingleObject(g_pUnloadThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pUnloadThread->m_hThread, 0);
			break;
		}
		g_pUnloadThread=NULL;
	}
	if(g_pUnloadReadyThread != NULL)
	{
		switch(WaitForSingleObject(g_pUnloadReadyThread->m_hThread, 500))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			TerminateThread(g_pUnloadReadyThread->m_hThread, 0);
			break;
		}
		g_pUnloadReadyThread=NULL;
	}
}

int CCmdRun::Play()
{
	short sTemp;
	if(m_pController == NULL)
	{
		m_tSysStatus = K_RUN_STS_STOP;
		return 0;
	}

	// ���ñ�ѹֵ
	SetBackPressure(g_pFrm->m_pRobotParam->m_dBackPress);
	// �򿪱�ѹ
	ExOutput(_T("1�ű�ѹ��ŷ�"),TRUE);

	RunWCL_addSTrack=true;
	//�ֶ������ֹ����
	g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(FALSE);
	g_pFrm->m_wndEditBar.m_wndList.EnableWindow(FALSE);

	//�����ǰ��⵽�ĸ߶�ƫ��
	m_dFirstHeightOffset = 0;
	m_ZMeterVector.clear();

	//Ԥ������·�� ��������ͼ��ƫ��
	if(!RunListExpand())
	{
		m_tSysStatus = K_RUN_STS_STOP;
		m_pController->TS_ListSetSystemStop(CRD1);

		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
			g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
		}
		return -1;
	}

	//����﷨
	if(!Syntax())
	{
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
			g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
		}
		return -1;
	}

	g_pFrm->AddMsg("---> RUN  [BEGIN] ...");

	//��¼��ʼʱ���
	CString strTime;
	GetLocalTime(&st1); 
	strTime.Format("%4d-%2d-%2d %d:%d:%d",st1.wYear,st1.wMonth,st1.wDay,st1.wHour,st1.wMinute,st1.wSecond);
	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_STARTTIME,strTime);
	SystemTimeToFileTime(&st1,   &g_pFrm->m_pSysParam->t1.ft); 

	m_pController->TS_ListSetSystemRun(CRD1);

	//��������״̬
	m_tSysStatus = K_RUN_STS_RUN;

	//Z����ߵ���ȫ�߶�
	sTemp = GotoSafeZpos();
	//if(RTN_NONE!=sTemp)
	//	return;

	///////////////
	m_iCurIndex = 0;//��ͷ��ʼ����

	CString strTemp;
	strTemp.Format("StartFrom NO:%04d", m_iCurIndex);
	g_pFrm->AddMsg(strTemp);

	SetLamp();

	m_cmdParam.reset();//�����ʼ����Ҫ���������¸�λ
	m_cmdParam.dAccXY = g_pFrm->m_mtrParamGts[X_AXIS].m_Acc;
	m_cmdParam.dAccZ = g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc;

	try
	{
		if(1 == g_pFrm->m_pRobotParam->m_nWorkMode)
			sTemp=RunSub(m_iCurIndex, m_cmdRunList.GetCount(),0,0);
		else if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
			sTemp=RunSub_EX(m_iCurIndex, m_cmdRunList.GetCount(),0,0);
	}
	catch(_com_error e)
	{
		g_pFrm->AddMsg("Err:RunFail!");
	}
	g_pView->ImgLive();
//	GotoSafeZpos();

	////////////////////////////////
	if(RTN_STP==sTemp)
	{
		m_tSysStatus = K_RUN_STS_STOP;
		g_pFrm->AddMsg("Proc Stop!");
		return 0;
	}
	else if(RTN_ESTP==sTemp)
	{
		g_pFrm->AddMsg("Proc E-Stop!");
		return -1;
	}
	else if(RTN_TIMEOUT==sTemp)
	{
		g_pFrm->AddMsg("Run OutTime!");
		AfxMessageBox("Proc OutTime");
		return -1;
	}
	else if(RTN_PARAMETER_ERR==sTemp)
	{
		g_pFrm->AddMsg("CMD Para Set Error!");
		return -1;
	}
	else if (RTN_Reverse_Err == sTemp)
	{
		g_pFrm->AddMsg("CMD �������!");
		return -1;
	}
	else if (RTN_ZHeight_Err == sTemp)
	{
		g_pFrm->AddMsg("��߳���!������Χ");
		return -1;
	}
	else if(RTN_MARK_ERROR==sTemp)
	{
		g_pFrm->AddMsg("Find Mark Error!");
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"Find Mark Error!");
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
			g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
		}

		if(IDOK == AfxMessageBox(_T("Find Mark Fail,Try again��"),MB_OKCANCEL))
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"");
			m_tSysStatus=K_RUN_STS_STOP;
			return -1;	
		}
		else
		{
			goto NoFindMarkLab;
		}
	}

	if(RTN_NONE!=sTemp) 
	{
		g_pFrm->AddMsg("Pro.Error!");
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"other Run Error!");
		return -1;
	}

	/////////////  �������  /////////////////////////////
	if(m_tSysStatus == K_RUN_STS_RUN)
	{ 
		//���ﻹ��Ҫ�жϻ������Ƿ�û���κ�ָ�
		{
			for(int i=0;i<m_pController->g_ExMtnPara->axisNum;i++)
			{
				m_pController->ClrAxisSts((unsigned short)i);
			}
			m_pController->TS_ListSetSystemStop(CRD1);
		}
		m_bUnloadEnable = true;

NoFindMarkLab:
		if(g_pFrm->m_pSysParam->nAutoMode==1)
		{
//			g_pFrm->AddMsg(_T("�������"));

// 			// ���͵㽺����źŸ�PLC
// 			ExOutput(_T("��ɼӹ�����"),TRUE);
// 			CFunction::DelaySec(0.2);                   //s
// 			ExOutput(_T("��ɼӹ�����"),FALSE);
		}

		g_pFrm->m_pSysParam->lRunCounter++;   // �ӹ����� �ۼ�
		g_pFrm->m_pRobotParam->m_structGlueWarning.m_nCurPcs++;		// ����������,�ѵ��������

		///////////////////////////////////////////////////////
// 		if(g_pFrm->m_pSysParam->nAutoMode)//�Զ�ģʽ��
// 		{
// 			if(g_pFrm->m_pSysParam->uiWashPinCycle>0)
// 			{
// 				int iTempCounter = g_pFrm->m_pSysParam->lRunCounter - g_pFrm->m_pSysParam->uiWashPinCycle*(g_pFrm->m_pSysParam->lRunCounter/g_pFrm->m_pSysParam->uiWashPinCycle);
// 				if((iTempCounter==0)&&(g_pFrm->m_pSysParam->lRunCounter>1))
// 				{
// 					if(0 == g_pFrm->m_pSysParam->iWashType)
// 					{  // ����ʽ��ϴ
// 						WashJet(g_pFrm->m_pSysParam->iWashNum);   // ��ϴ����
// 					}
// 					else
// 					{  // ����ʽ��ϴ
// 						WashNeedle(g_pFrm->m_pSysParam->iWashNum);   // ��ϴ����   
// 					}
// 				}			
// 			}
// 		}

		CString str;
		str.Format("PN(pcs):%ld", g_pFrm->m_pSysParam->lRunCounter);
		g_pFrm->SetPanelTextEx(IDS_RUN_COUNT, str);
		str.Format("%ld",g_pFrm->m_pSysParam->lRunCounter);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_PRODUCTION,str);
		
		////
		//��������ƶ����ӹ�����λ
		GotoSafeZpos();
		if( 1 == g_pFrm->m_pSysParam->nGoStandByPos)
		{
			//g_pFrm->m_Robot->m_pController->LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParam[X_AXIS].m_Vmin,g_pFrm->m_mtrParam[X_AXIS].m_Acc);
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,g_pFrm->m_pSysParam->tDefHomePos.x,g_pFrm->m_pSysParam->tDefHomePos.y,g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		}

		// ���ر���ʱ���嵱ǰ״̬
		// 		if(m_tSysStatus != K_RUN_STS_WEIGHTERROR)
		// 		{
		// 			m_tSysStatus = K_RUN_STS_STOP;
		// 			g_pFrm->AddMsg(_T("WErrorInit"));
		// 		}

		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
			g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
		} 

		g_pFrm->AddMsg("<----- RUN [END]");
	}

	return 1;
}

BOOL CCmdRun::Init()
{   
	g_pFrm->AddMsg("Start Init.....");

	if(m_pController->IsInitOk()==TRUE) 
		return RTN_NONE;

	m_pController->SetCardNum(CARDNUM);// ���ÿ���
	m_pController->SetToolNum(TOOL_NUM);

	dcp = new t_Tool[TOOL_MAXNUM];

	LoadIOMap();

	m_pController->MapToolStruct(dcp);

	// 3���е�� ��ͣ��������
	inEmergecy = ExInputBitNameToNo(_T("��ͣ��ť"));
	if (-1==inEmergecy)
		return RTN_PARAMETER_ERR;
	m_pController->MapEStop(inEmergecy);             

	// ������
	inWashPort = ExOutputBitNameToNo(_T("��๤��վ��ŷ�"));  
	if (-1==inWashPort)
		return RTN_PARAMETER_ERR;

	// Z����ͷλ�ô������ź�
	inSenseCheckZ = ExInputBitNameToNo(_T("�㽺��ͷ�߶ȱ궨"));
	if (-1==inSenseCheckZ)
		return RTN_PARAMETER_ERR;


	outJetPort = ExOutputBitNameToNo(_T("1�ŵ��̬����"));
	if (-1==outJetPort)
		return RTN_PARAMETER_ERR;

	TS_MTN_PARAM tsMtnParam;
	tsMtnParam.axisNum = MAX_AXIS_NUM; // ����������

	for(int i=1;i<=tsMtnParam.axisNum;i++)
	{
		tsMtnParam.dfLength[i-1] = g_pFrm->m_mtrParamGts[i-1].m_MaxTravelPositive-g_pFrm->m_mtrParamGts[i-1].m_MaxTravelNegative;
		tsMtnParam.dMaxpos[i-1]  = g_pFrm->m_mtrParamGts[i-1].m_MaxTravelPositive;
		tsMtnParam.dMinpos[i-1]  = g_pFrm->m_mtrParamGts[i-1].m_MaxTravelNegative;
		tsMtnParam.dfScale[i-1]	 = g_pFrm->m_mtrParamGts[i-1].m_Resolution; //pulse/mm
	}

	//	tsMtnParam.dzSafePost=m_pSysParam->dzSafePost;//Z�ᰲȫ�߶�
	tsMtnParam.iCardType = CRD_TYPE_GTS;
	tsMtnParam.iLmtSns	= 0x0000;	// ��λ�����������ƽ
	tsMtnParam.iHomeSns = 0x0000;

	tsMtnParam.maxVel	= 2000;
	tsMtnParam.maxAcc	= g_pFrm->m_pSysParam->dLookMaxAcc;
	tsMtnParam.startVel	= 0;
	tsMtnParam.stpAcc	= 10000;
	tsMtnParam.synAcc	= g_pFrm->m_pSysParam->dLookAcc;
	tsMtnParam.synVel	= 200;

	//�ŷ������ʹ�ܱ����ź�
	for (int j=0;j<3/*tsMtnParam.axisNum*/;j++)
	{
		if (3 >= j)
		{
			tsMtnParam.alarmON[j] = TRUE;
		}
		else
		{
			tsMtnParam.alarmON[j] = FALSE;   //��4��Ϊ�㽺�����Ƶ��
		}
	}

	short m_Ret;
	// ��ʼ������ϵͳ
	if(g_pFrm->m_pSysParam->nRobotCtrlMode == DefCtrlMode_Pulse)
	{	

		for (int i=0; i<MAX_AXIS_NUM; i++)
		{	// ����ģʽ
			tsMtnParam.iCtrlMode[i] = DefCtrlMode_Pulse;
		}

		g_pFrm->AddMsg("Start AttachCard.....");
		m_Ret = m_pController->AttachCard(&tsMtnParam);
	}
	else
	{  
		// ģ����
		tsMtnParam.iCtrlMode[0] = DefCtrlMode_Analog;
		tsMtnParam.iCtrlMode[1] = DefCtrlMode_Analog;
		tsMtnParam.iCtrlMode[2] = DefCtrlMode_Analog;
		tsMtnParam.iCtrlMode[3] = DefCtrlMode_Pulse;
		for (int i=0;i<3;i++)
		{
			tsMtnParam.PID_P[i]=g_pFrm->m_pSysParam->PIDParam[i].PID_P;
			tsMtnParam.PID_I[i]=g_pFrm->m_pSysParam->PIDParam[i].PID_I;
			tsMtnParam.PID_D[i]=g_pFrm->m_pSysParam->PIDParam[i].PID_D;
			tsMtnParam.PrfVelFilter[i] = g_pFrm->m_pSysParam->PIDParam[i].PrfVelFeilter;
			tsMtnParam.Kaff[i] = g_pFrm->m_pSysParam->PIDParam[i].Kaff;
			tsMtnParam.kvff[i] = g_pFrm->m_pSysParam->PIDParam[i].Kvff;
		}

		g_pFrm->AddMsg("Start AttachCardEX.....");
		m_Ret = m_pController->AttachCardEx(&tsMtnParam);
	}

	if(m_Ret != RTN_NONE)
	{
		g_pFrm->AddMsg(_T("Init Motion Card Error."));
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,_T("Init Error"));
	}
	else
	{   
		// ���û��������˶�ʱ��
		m_pController->KF_SetLookEvenTime(g_pFrm->m_pSysParam->m_LookEvenTime);
		// ���û�����併�ٱ�
		m_pController->KF_SetLookTunScale(g_pFrm->m_pSysParam->m_LookTunScale);

		//��ʼ������ϵ����
		if(CRD_TYPE_GTS == tsMtnParam.iCardType)
		{
			t_CrdP crd1;
			memset(&crd1,0,sizeof(t_CrdP));
			crd1.dimension = 3;
			crd1.profile[0]	= 1;//�滮��1��ӦX��
			crd1.profile[1]	= 2;//�滮��2��ӦY��
			crd1.profile[2]	= 3;//�滮��3��ӦZ��
			m_pController->TS_EX_SetCoordinate(1,&crd1);//����1������ϵ
		}
	}

	//////////////////////////////////////////////////////////////////////////
	m_hMsgReceiveWnd = ((CMainFrame*)AfxGetMainWnd())->GetSafeHwnd();

	//////////////////////////////////////////////////////////////////////////
	if (NULL == g_pUpdateThread)
	{
		g_pUpdateThread = AfxBeginThread(CCmdRun::UpdateThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		if(g_pUpdateThread==NULL)
		{
			g_pFrm->AddMsg("�����̴߳���ʧ��");
			return RTN_PARAMETER_ERR;
		}
		::SetThreadAffinityMask(g_pUpdateThread->m_hThread, g_pFrm->m_nUIRunProcesser);
		g_pUpdateThread->ResumeThread();
	}
	if (NULL == g_pMonitorThread)
	{
		g_pMonitorThread = AfxBeginThread(CCmdRun::MonitorThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		if(g_pMonitorThread==NULL)
		{
			g_pFrm->AddMsg("�����̴߳���ʧ��");
			return RTN_PARAMETER_ERR;
		}
		::SetThreadAffinityMask(g_pMonitorThread->m_hThread, g_pFrm->m_nUIRunProcesser);
		g_pMonitorThread->ResumeThread();
	}
	if (NULL == g_pLightsetThread)
	{
		g_pLightsetThread = AfxBeginThread(CCmdRun::LightSetThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		if(g_pLightsetThread==NULL)
		{
			g_pFrm->AddMsg("�ź����̴߳���ʧ��");
			return RTN_PARAMETER_ERR;
		}
		::SetThreadAffinityMask(g_pLightsetThread->m_hThread, g_pFrm->m_nUIRunProcesser);
		g_pLightsetThread->ResumeThread();
	}

	//g_pLineSignStsThread = AfxBeginThread(CCmdRun::LineSignStsThread, this);
	//if(g_pLineSignStsThread==NULL)
	//{
	//	g_pFrm->AddMsg("�������źż����̴߳���ʧ��");
	///	return RTN_PARAMETER_ERR;
	//}

	// 	// �ֶμӹ�ʹ��
	//     if(g_pFrm->m_pSysParam->nFileSeparaEnable == 1)
	// 	    g_pFrm->m_Robot->ExOutput("�ְ�ʹ��",TRUE);
	// 	else
	//         g_pFrm->m_Robot->ExOutput("�ְ�ʹ��",FALSE);

	return m_Ret;
}
void CCmdRun::Jog(int iAxisNo, double dMvSpeed/*0~1*/,int iDir/*-1, +1*/,BOOL bEnableStep, double fDist)
{ 
	if(m_tSysStatus >= K_AXIS_STS_LIMIT_XP && m_tSysStatus <= K_AXIS_STS_ALM_Z)
		return;
	if(m_tSysStatus == K_RUN_STS_RUN || m_tSysStatus == K_RUN_STS_PAUSE)
		return;

	double dStepDist;

	if (bEnableStep)
	{
		if(fDist<m_pController->g_ExMtnPara->dMinpos[iAxisNo])
			fDist = g_pFrm->m_mtrParamGts[iAxisNo].m_MaxTravelNegative;
		if(fDist>m_pController->g_ExMtnPara->dMaxpos[iAxisNo])
			fDist = m_pController->g_ExMtnPara->dMaxpos[iAxisNo];

		dStepDist = iDir<0 ? -fDist: fDist;

		//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
		g_pFrm->m_Precision.GetCurPosMM();
		dStepDist = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[iAxisNo]+dStepDist;
	}
	else
	{
		dStepDist = iDir<0 ? g_pFrm->m_mtrParamGts[iAxisNo].m_MaxTravelNegative : g_pFrm->m_mtrParamGts[iAxisNo].m_MaxTravelPositive;
	}


	CString strTemp;
	if ((R_AXIS==iAxisNo) || (X_AXIS==iAxisNo) || (Y_AXIS==iAxisNo) || (Z_AXIS==iAxisNo))
	{
		m_pController->AxisMove(iAxisNo,dStepDist,g_pFrm->m_mtrParamGts[iAxisNo].m_VhandMove*dMvSpeed, g_pFrm->m_mtrParamGts[iAxisNo].m_Acc*dMvSpeed, FALSE);
	}
	else if (T1_AXIS == iAxisNo)
	{
		m_pController->AxisJog(iAxisNo, iDir*g_pFrm->m_mtrParamGts[iAxisNo].m_VhandMove, g_pFrm->m_mtrParamGts[iAxisNo].m_Acc);
	}
	else if (W1_AXIS == iAxisNo)
	{
		m_pController->AxisJog(iAxisNo, -iDir*g_pFrm->m_mtrParamGts[iAxisNo].m_VhandMove*dMvSpeed, g_pFrm->m_mtrParamGts[iAxisNo].m_Acc*dMvSpeed);
	}
// 	else if((iAxisNo == X_AXIS)||(iAxisNo == Y_AXIS))
// 	{
// 		dvel = g_pFrm->m_mtrParamGts[iAxisNo].m_VhandMove;
// 		m_pController->AxisMove(iAxisNo,dStepDist,dvel*dMvSpeed, g_pFrm->m_mtrParamGts[iAxisNo].m_Acc*dMvSpeed, FALSE);
// 		strTemp.Format("Move(%d, %.3f, v_%.3f, acc_%.3f, W%d", iAxisNo+1, iDir<0 ? 0 : m_pController->g_ExMtnPara->dfLength[iAxisNo], g_pFrm->m_pSysParam->dSpeedMaxJogXY*dMvSpeed, g_pFrm->m_mtrParamGts[iAxisNo].m_Acc*dMvSpeed, FALSE);
// 	}
// 	else if (iAxisNo == Z_AXIS)
// 	{ //�����Z��dfLengthӦ��Ϊlimit
// 		m_pController->AxisMove(iAxisNo,dStepDist,g_pFrm->m_pSysParam->dSpeedMaxJogZ*dMvSpeed, g_pFrm->m_mtrParamGts[iAxisNo].m_Acc*dMvSpeed, FALSE);
// 		strTemp.Format("Move(%d, %.3f, v_%.3f, acc_%.3f, W%d", iAxisNo+1, iDir<0 ? 0 : m_pController->g_ExMtnPara->dfLength[iAxisNo], g_pFrm->m_pSysParam->dSpeedMaxJogZ*dMvSpeed, g_pFrm->m_mtrParamGts[iAxisNo].m_Acc*dMvSpeed, FALSE);
	// 	}
	strTemp.Format("Move(%d, %.3f, v_%.3f, acc_%.3f, W%d", iAxisNo+1, iDir<0 ? 0 : m_pController->g_ExMtnPara->dfLength[iAxisNo], g_pFrm->m_mtrParamGts[iAxisNo].m_VhandMove*dMvSpeed, g_pFrm->m_mtrParamGts[iAxisNo].m_Acc*dMvSpeed, FALSE);

	g_pFrm->AddMsg(strTemp);
}

void CCmdRun::Step()
{
	if((m_tSysStatus != K_RUN_STS_STOP))
	{
		AfxMessageBox(("Stop Run Frist To Step!"));
		return;
	}
	if(m_iStepIndex<=0 || m_cmdRunList.GetCount()<0)
	{
		m_cmdParam.reset();
		m_cmdParam.dAccXY = g_pFrm->m_mtrParamGts[X_AXIS].m_Acc;
		m_cmdParam.dAccZ = g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc;

		if(!RunListExpand())
		{
			return;
		}
		m_pController->AxisMove(Z_AXIS,0,200,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc);
		m_iStepIndex=0;
	}
	if(m_cmdRunList.GetCount()<1)
	{
		AfxMessageBox("NO CMD!");
		return;
	}
	if(m_iStepIndex >(m_cmdRunList.GetCount()-1))
	{
		AfxMessageBox("�Ѿ�ִ�е����һ���켣������");
		m_iStepIndex = -1;
		return;
	}
	m_tSysStatus = K_RUN_STS_RUN;
	m_pController->TS_ListSetSystemRun(CRD1);
	SetLamp();///

	CString strText;
	strText.Format("GotoStep:%d",m_iStepIndex);
	g_pFrm->AddMsg(strText);

	//ArrangDcpParamter();
	g_pFrm->m_wndEditBar.SetCmdCurSel(m_iStepIndex);

	RunStep(&m_iStepIndex,0,0);
	//g_pFrm->m_wndEditBar.SetCmdCurSel(m_iStepIndex);

	m_iStepIndex++;
	m_tSysStatus = K_RUN_STS_STOP;
	m_pController->TS_ListSetSystemStop(CRD1);
	SetLamp();////

}

void CCmdRun::UnloadBoard(void)
{
	if (theApp.m_EncryptOp.m_bLockSystem)
	{
		return ;
	}
	if(	m_tSysStatus == K_RUN_STS_RUN || m_tSysStatus == K_RUN_STS_EMERGENCY) 
	{
		return;
	}
	if (m_pController->IsInitOk()==FALSE) 
	{
		AfxMessageBox(_T("System Not Init��"));
		return;
	}

	if (!m_bUnloadFinish)
	{
		AfxMessageBox(_T("����δ���"));
		return;
	}

	m_bUnloadFinish = false;
	m_bUnloadSuccess = false;
	// �����������߳�
	g_pUnloadThread = AfxBeginThread(CCmdRun::UnloadThread, this);
	if(g_pUnloadThread==NULL)
	{
		m_bFlagRunning = FALSE;
		g_pFrm->AddMsg("Run UnLoad Thread Error.");
		return;
	}
}

void CCmdRun::LoadBoard(void)
{
	if (theApp.m_EncryptOp.m_bLockSystem)
	{
		return ;
	}
	if(	m_tSysStatus == K_RUN_STS_RUN || m_tSysStatus == K_RUN_STS_EMERGENCY) 
	{
		return;
	}
	if (m_pController->IsInitOk()==FALSE) 
	{
		AfxMessageBox(_T("System Not Init��"));
		return;
	}

	if (!m_bLoadFinish)
	{
		AfxMessageBox(_T("����δ���"));
		return;
	}

	m_bLoadFinish = false;
	m_bLoadSuccess = false;
	// �����������߳�
	g_pLoadThread = AfxBeginThread(CCmdRun::LoadThread, this);
	if(g_pLoadThread==NULL)
	{
		m_bFlagRunning = FALSE;
		g_pFrm->AddMsg("Run Load Thread Error.");
		return;
	}
}

void CCmdRun::Run()
{
	//////////////////////////////////////////////////////////////////////////
	//							�ж�ϵͳ��������							//
	//////////////////////////////////////////////////////////////////////////
	if (m_tSysStatus == K_RUN_STS_RUN)		return;
	if (m_pController->IsInitOk()==FALSE) 
	{
		AfxMessageBox(_T("�忨û�г�ʼ���ɹ�!"));
		return;
	}
	if (theApp.m_EncryptOp.m_bLockSystem)	
	{
		AfxMessageBox(_T("ϵͳ����,����ϵ��Ӧ�̽��!!!"));
		return ;
	}
	if (m_tSysStatus == K_RUN_STS_EMERGENCY) 
	{	
		AfxMessageBox(_T("��ͣ��ť������,�븴λ��������"));
		return;
	}
	if (g_pFrm->m_wndEditBar.m_rawList.GetCount()<1)
	{
		AfxMessageBox(_T("û��ѧϰ·�����ݣ�"));
		return ;
	}
	if (!m_bHomeSuccess)
	{		
		AfxMessageBox(_T("���Ȼ���"));
		return;
	}		
	if(1 == g_pFrm->m_pRobotParam->m_nIfCheckSafeDoor)
	{	
		if (!m_bDoorSFSts)	// �Ŵ�״̬
		{
			AfxMessageBox(_T("�ű���,����ǰ�źͺ����ź�!"));
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//							�жϽ���									//
	//////////////////////////////////////////////////////////////////////////

	m_bFlagRunning = TRUE;		//ȥ���´��ظ�������־
	m_bIsHandPause = false;

	if (m_tSysStatus==K_RUN_STS_PAUSE)
	{
		g_pFrm->AddMsg("Contiue Run ");

		m_tSysStatus = K_RUN_STS_RUN;
		m_pController->TS_ListSetSystemRun(CRD1);

		SetLamp();	
		return;
	}

	m_bLoadFinish = false;
	m_bLoadEnable = false;
	m_bLoadSuccess = false;
	m_bUnloadEnable = false;
	m_bUnloadFinish = false;
	m_bUnloadSuccess = false;
	m_bDispEnable = false;
	m_nLoadStep = 1;
	m_nUnloadStep = 1;
	g_bStop = FALSE;
	m_bCanWeightAndWash = false;
	//////////////////////////////////////////////////////////////////////////
	// �����������߳�
	if (0 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode)
	{
		g_pLoadThread = AfxBeginThread(CCmdRun::LoadThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		g_pUnloadThread = AfxBeginThread(CCmdRun::UnloadThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	}
	else if (1 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode)
	{
		g_pLoadThread = AfxBeginThread(CCmdRun::LoadThread_3Box, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		g_pUnloadThread = AfxBeginThread(CCmdRun::UnloadThread_3Box, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	}

	if(g_pLoadThread==NULL)
	{
		g_bStop = TRUE;
		m_bFlagRunning = FALSE;
		g_pFrm->AddMsg("Run Load Thread Error.");
		return;
	}
	if(g_pUnloadThread==NULL)
	{
		g_bStop = TRUE;
		m_bFlagRunning = FALSE;
		g_pFrm->AddMsg("Run UnLoad Thread Error.");
		return;
	}
	::SetThreadAffinityMask(g_pLoadThread->m_hThread, g_pFrm->m_nUIRunProcesser);
	::SetThreadAffinityMask(g_pUnloadThread->m_hThread, g_pFrm->m_nUIRunProcesser);
	g_pLoadThread->ResumeThread();
	g_pUnloadThread->ResumeThread();

	// �㽺�߳�
	g_pPlayThread = AfxBeginThread(CCmdRun::PlayThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if(g_pPlayThread==NULL)
	{
		g_bStop = TRUE;
		m_bFlagRunning = FALSE;
		g_pFrm->AddMsg("Creat Run Thread Error.");
		return;
	}
	::SetThreadAffinityMask(g_pLightsetThread->m_hThread, 0x01<<(g_pFrm->m_nProcesserNum-1));
	g_pPlayThread->ResumeThread();
	//////////
}
void CCmdRun::AutoRps()
{
	// ����λ��
	double x = g_pFrm->m_pRobotParam->m_posWeightAdj.x+g_pFrm->m_pSysParam->tOffset[0].x;
	double y = g_pFrm->m_pRobotParam->m_posWeightAdj.y+g_pFrm->m_pSysParam->tOffset[0].y;
	double z = g_pFrm->m_pRobotParam->m_zContactPos-g_pFrm->m_pRobotParam->m_posWeightAdj.z;

	int times = g_pFrm->m_pRobotParam->m_nAutoRpsDots;				// �����������

	// �ƶ���λ������λ��
	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

	DotDisp(times);
	g_pFrm->m_Robot->GotoSafeZpos();

	m_tmAutoRps.TimerStart();
}
void CCmdRun::AutoRPSRun()
{
	if (theApp.m_EncryptOp.m_bLockSystem)
	{
		return ;
	}

	if(	m_tSysStatus == K_RUN_STS_RUN || m_tSysStatus == K_RUN_STS_EMERGENCY) 
		return;

	if (m_pController->IsInitOk()==FALSE) 
	{
		AfxMessageBox(_T("System Not Init��"));
		return;
	}
	g_pAutoRPSThread = AfxBeginThread(CCmdRun::AutoRPSThread, this);//,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	if(g_pAutoRPSThread==NULL)
	{
		g_pFrm->AddMsg(_T("Creat Run Thread Error."));
		return;
	}	
}

// ����ǰ��׼������
void CCmdRun::PrepareHome(void)
{
	// ֹͣ���е����˶�
	m_pController->AxisStop(0xff);
	g_pFrm->m_Robot->m_pController->SetExOutport(0, -1, FALSE);
}

void CCmdRun::Home()
{
	// TODO: Add your control notification handler code here
	if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
	{
		m_lsCtrl.Init();
		m_lsCtrl.Home(smc_mtr_rev);
	}
	if(m_pController->IsInitOk()==FALSE) 
	{
		if (RTN_NONE != Init())
		{
			AfxMessageBox("�忨����ʧ��!���鼱ͣ��ť����������������·��!");
			return;
		}
	}

	if( m_tSysStatus >= K_AXIS_STS_ALM_X && m_tSysStatus <= K_AXIS_STS_ALM_Z)
	{
		long t_sts[MAX_AXIS_NUM];
		for(unsigned short i=0;i<m_pController->g_ExMtnPara->axisNum;i++)
		{
			m_pController->ClrAxisSts(i+1);
			Sleep(2);
		}
		for(unsigned short i=0;i<m_pController->g_ExMtnPara->axisNum;i++)
		{
			m_pController->GetAxisSts(i+1,&t_sts[i]);
			if((t_sts[i] & 0x02) == 0x02)
			{
				AfxMessageBox(_T("����������!"));
				return;
			}
		}
	}

	static bool b = false;

	if(b)	return;

	b = true;

	if(g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_RUN )
	{
		AfxMessageBox("������ֹͣ����!");
		b = false;
		return;
	}
	PrepareHome();

	CFunction::DelaySec(0.05);
	m_tSysStatus = K_RUN_STS_NONE;

	g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(FALSE);
	g_pFrm->m_wndEditBar.m_wndList.EnableWindow(FALSE);
	g_pFrm->m_wndToolBar.EnableWindow(FALSE);
	//	g_pFrm->m_DrawToolBar.EnableWindow(FALSE);
	if (1 == g_pFrm->m_pRobotParam->m_nIfRotateWorkpiece)
	{
		m_bRotateTableHomeFinish = false;
		g_pHomeRotateTable = AfxBeginThread(ThreadRotateTableHome, this);
	}

	if (1 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode)
	{
		m_bLdHomeFinish = false;
		m_bUldHomeFinish = false;
		if (!m_bLdHomeSucceed)
			g_pHomeLdThread = AfxBeginThread(ThreadHomeLd, this);

		if (!m_bUldHomeSucceed)
			g_pHomeUldThread = AfxBeginThread(ThreadHomeUld, this);
	}

	////////////////
	short sTemp = RTN_NONE;

	Stop();
	///////////////
	g_pFrm->SetPanelTextEx(IDS_RUN_STATUS,"Go Home...");
	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,_T("Go Home..."));

	unsigned int bCrdAxis[4];
	double searchRange[4];
	double hCrdVel[4];
	double lCrdVel[4];
	double accCrd[4];
	double offsetCrd[4];

	// �����ٶȡ�ƫ�ƵȲ���
	for(int i=0;i<3;i++)
	{
		bCrdAxis[i]		= TRUE;
		hCrdVel[i]		= g_pFrm->m_mtrParamGts[i].m_Vhome;
		lCrdVel[i]		= g_pFrm->m_mtrParamGts[i].m_VhomeLow;
		accCrd[i]		= g_pFrm->m_mtrParamGts[i].m_AccHome;
		offsetCrd[i]	= g_pFrm->m_mtrParamGts[i].m_HomeReturn; 
		searchRange[i]	= g_pFrm->m_mtrParamGts[i].m_HomeStep;
	}

	sTemp = m_pController->AxisesCrdCaptHome(CRD1,bCrdAxis,searchRange,hCrdVel,lCrdVel,accCrd,offsetCrd,TRUE);
	if(sTemp==RTN_CAPLMT_ERR)
	{
		AfxMessageBox("Go Home Error!");/*ԭ�㿪�ع���*/
		b = false;
		m_tSysStatus = K_RUN_STS_STOP;
		return;
	}
	else if(sTemp==RTN_ESTP)
	{
		m_tSysStatus = K_RUN_STS_EMERGENCY;
		m_pController->SetOutport(K_ALL_PORTS,FALSE);
		SetLamp();
		g_pFrm->AddMsg("--->E-Stop----");
		// �ֶμӹ�ʹ��
		if(g_pFrm->m_pSysParam->nFileSeparaEnable == 1)
			g_pFrm->m_Robot->ExOutput("�ְ�ʹ��",TRUE);
		else
			g_pFrm->m_Robot->ExOutput("�ְ�ʹ��",FALSE);

		b = false;
		return;
	}

	m_tSysStatus = K_RUN_STS_NONE;

	Stop();//��ԭ����ɺ�ֹͣ״̬

	b = false;


	ExOutput("11���赲���׵�ŷ�",0);
	ExOutput("12���赲���׵�ŷ�",0);
	ExOutput("11�Ŷ������׵�ŷ�", 0);
	ExOutput("11�Ų�����׵�ŷ�", 0);

	m_pController->AxisStop(1<<T1_AXIS);
	

	if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
	{
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
	}
	//	g_pFrm->m_DrawToolBar.EnableWindow(TRUE);
	g_pFrm->m_wndToolBar.EnableWindow(TRUE);
	m_bHomeSuccess = true;
}

void CCmdRun::Pause()
{
	if(m_tSysStatus != K_RUN_STS_RUN)	return;

	m_pController->TS_ListSetSystemPause(CRD1);
	m_tSysStatus = K_RUN_STS_PAUSE;

	SetLamp();
	g_pFrm->AddMsg("--->Pause ----");	
}

void CCmdRun::Ready()
{
	if(m_tSysStatus == K_RUN_STS_NONE)	return;

	m_iLoop = 0;
	m_iStepIndex = -1;
	m_iCurIndex = 0;

	m_tSysStatus = K_RUN_STS_NONE;
	SetLamp();

	g_pFrm->m_wndToolBar.EnableWindow(TRUE);

	g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
	g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
	g_pFrm->AddMsg("--->Robot Ready----");	
}

void CCmdRun::Net()
{
	Stop();
	//g_pFrm->SetPanelTextEx(IDS_ONLINE_MODE,"����ģʽ");
//	g_pFrm->m_wndRightBar.m_DlgPosInfo.UpdateNetState(TRUE);
	g_pFrm->AddMsg("--->Online Mode---");	

	if(g_pFrm->m_pSysParam->nAutoMode == 0)
	{
		Stop();
		//g_pFrm->SetPanelTextEx(IDS_ONLINE_MODE,_T("����"));
//		g_pFrm->m_wndRightBar.m_DlgPosInfo.UpdateNetState(TRUE);
		g_pFrm->AddMsg("--->Online Mode----");	
		g_pFrm->m_pSysParam->nAutoMode = 1;

		g_pFrm->m_pSysParam->lRunCounter = 0;

		CString str;
		str.Format("PN(pcs):%ld", g_pFrm->m_pSysParam->lRunCounter);
		g_pFrm->SetPanelTextEx(IDS_RUN_COUNT, str);

		g_pFrm->m_wndToolBar.EnableWindow(FALSE);
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(FALSE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(FALSE);
	}

}

void CCmdRun::UnNet()
{
	if(g_pFrm->m_pSysParam->nAutoMode == 1)
	{
		Stop();
		//g_pFrm->SetPanelTextEx(IDS_ONLINE_MODE,_T("����"));
//		g_pFrm->m_wndRightBar.m_DlgPosInfo.UpdateNetState(FALSE);
		g_pFrm->AddMsg("--->UnNet Mode----");	
		g_pFrm->m_pSysParam->nAutoMode = 0;

		g_pFrm->m_pSysParam->lRunCounter = 0;
		CString str;
		str.Format("PN(pcs):%ld", g_pFrm->m_pSysParam->lRunCounter);
		g_pFrm->SetPanelTextEx(IDS_RUN_COUNT, str);

		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
			g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
		}

		g_pFrm->m_wndToolBar.EnableWindow(TRUE);
	}
}

void CCmdRun::Stop(bool bExitThread)
{
	if (bExitThread)
	{
		g_bStop = TRUE;
	}

	if(m_tSysStatus == K_RUN_STS_STOP) 
		return;

	if(m_tSysStatus == K_RUN_STS_RUN_1_UNIT)
	{
		m_tSysStatus = K_RUN_STS_STOP;
		return;
	}

	m_iLoop = 0;
	m_iStepIndex = -1;
	m_iCurIndex = 0;

	if (g_pFrm->m_Robot->m_structPathInfo.m_bRepeat)
	{	// ������ʱ������һ��unit��ֹͣ����
		m_tSysStatus = K_RUN_STS_RUN_1_UNIT;
	}
	else
	{
		m_tSysStatus = K_RUN_STS_STOP;
		m_pController->TS_ListSetSystemStop(CRD1);
	}
	
	for(int i=0;i<m_pController->g_ExMtnPara->axisNum;i++)
	{
		m_pController->ClrAxisSts((unsigned short)i);
	}

	SetLamp();

	g_pFrm->m_wndToolBar.EnableWindow(TRUE);

	if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
	{
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
	}
	m_bIsHandPause = false;
	g_pFrm->AddMsg("--->Stop----");	
}

void CCmdRun::Emergency()
{
	Stop();

	if(m_tSysStatus == K_RUN_STS_EMERGENCY) 
		return;	

	m_iLoop = 0;
	m_iStepIndex = -1;
	m_iCurIndex = 0;

	for(int t=0;t<m_pController->g_ExMtnPara->axisNum;t++)
		m_GotoOffSet[t]=0.0;

	m_tSysStatus = K_RUN_STS_EMERGENCY;

	m_pController->TS_ListSetSystemEStop(CRD1);

	m_pController->SetOutport(K_ALL_PORTS,FALSE);

	SetLamp();

	m_pController->DetachCard();
	// �ֶμӹ�ʹ��
	// 	if(g_pFrm->m_pSysParam->nFileSeparaEnable == 1)
	// 		g_pFrm->m_Robot->ExOutput("�ְ�ʹ��",TRUE);
	// 	else
	// 		g_pFrm->m_Robot->ExOutput("�ְ�ʹ��",FALSE);

	g_pFrm->AddMsg("--->E-Stop----");
	////////
}

void CCmdRun::SetLamp()
{
	if(m_pController == NULL)return;

	ExOutput(_T("��ɫ��.��"),m_tSysStatus == K_RUN_STS_RUN);
	ExOutput(_T("��ɫ��.��"),m_tSysStatus == K_RUN_STS_PAUSE || m_tSysStatus == K_RUN_STS_STOP);

	if(m_tSysStatus == K_AXIS_STS_ALM_X|| m_tSysStatus == K_AXIS_STS_ALM_Y|| m_tSysStatus == K_AXIS_STS_ALM_Z)
	{
		ExOutput(_T("��ɫ��.��"),TRUE);
		ExOutput(_T("������"),TRUE);
		CFunction::DelaySec(0.3);
		ExOutput(_T("��ɫ��.��"),FALSE);
		ExOutput(_T("������"),FALSE);
		CFunction::DelaySec(0.3);
	}

	ExOutput(_T("��ɫ��.��"),m_tSysStatus == K_RUN_STS_EMERGENCY);

	//     if(g_pFrm->bisWeightError == TRUE)
	//        ExOutput(_T("����ָʾ��"),TRUE);

	g_pFrm->m_wndRightBar.m_DlgPosInfo.UpdateMotionState();
}

int CCmdRun::AfxMessageBox(LPCTSTR lpszText, UINT nType /* = MB_OK  */, UINT nIDHelp /* = 0 */ )
{
	int	nRet = 0;
	K_RUN_STATUS l_oldStatus = m_tSysStatus;
	m_tSysStatus = K_RUN_STS_WARNING;
 	nRet = ::AfxMessageBox(lpszText, nType, nIDHelp);
	m_tSysStatus = l_oldStatus;

	CFunction::WriteLog(__FILE__, __LINE__, lpszText, CFunction::LEV_WARN);
	return nRet;
}

BOOL CCmdRun::FindMark(const tgCmdLine *pCmd)
{
	BOOL bFindOK = TRUE;

	g_pFrm->AddMsg("[SEARCH MARK] --> BEGIN ......");

	m_tOffset.x = 0;
	m_tOffset.y = 0;
	m_dRotateAngle = 0;

	g_pView->ImgLive();

	bFindOK = SubFindMark(pCmd);

	g_pFrm->AddMsg("[SEARCH MARK] --> END");

	if(!bFindOK)
		g_pFrm->AddMsg("Find MARK Error.");

	////
	return bFindOK;
}
BOOL CCmdRun::SubFindMark(const tgCmdLine *pCmd)
{ 
	double dx=0, dy=0;
	double t_vel = g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax;
	double t_acc = g_pFrm->m_mtrParamGts[X_AXIS].m_Acc;
	CString  strtemp;
	for(int i=0; i<g_pFrm->m_pSysParam->iMarkNum; i++)
	{
		g_pFrm->m_Precision.GetCurPosMM();
		double m_curXPos = m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
		double m_curYPos = m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];

		//ͼ��ɨ���Z�ᵽλ
// 		if(((m_curZPos-pCmd->pDblValue[4*i +2])>1)||((m_curZPos-pCmd->pDblValue[4*i +2])<-1))
// 		{
// 			m_pController->AxisMove(Z_AXIS, pCmd->pDblValue[4*i +2], g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, m_cmdParam.dAccZ);
// 		}

		//����ͼ��ɨ���
		if((fabs(m_curXPos-pCmd->pDblValue[4*i +0])>1)||(fabs(m_curYPos-pCmd->pDblValue[4*i +1])>1))
		{
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,pCmd->pDblValue[4*i +0], pCmd->pDblValue[4*i +1],g_pFrm->m_pSysParam->dSpeedImageMoveXY,m_cmdParam.dAccXY);
		}

		//////////////////////////////////
		CFunction::DelaySec(0.3);
		//////////////////////////////////
		g_pFrm->AddMsg("Load Model.");
		if(g_pView->PatLoad(i/*(LPSTR)(LPCSTR)g_pDoc->GetModeFile(i+1)*/))//��1��ʼ��0Ϊ�궨ģ��
		{
			CFunction::DelaySec(0.1);
			if(g_pView->PatFind(i+1, pCmd->pIntValue[i],dx, dy)<1)
			{
				//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
				g_pFrm->m_Precision.GetCurPosMM();
				double xx = m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
				double yy = m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
				double tempx=xx;
				double tempy=yy;

				double ccdMove=g_pFrm->m_pSysParam->m_ccdMoveDis;
				int    ccdMoveCnt=g_pFrm->m_pSysParam->m_ccdMoveCnt;

				for (int m=1;m<=ccdMoveCnt;m++)
				{
					g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx-ccdMove*m, tempy-ccdMove*m,t_vel,t_acc);
					CFunction::DelaySec(0.1);
					if(g_pView->PatFind(i+1, pCmd->pIntValue[i],dx, dy)>=1)
						goto lab;

					g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx, tempy-ccdMove*m,t_vel,t_acc);
					CFunction::DelaySec(0.1);
					if(g_pView->PatFind(i+1, pCmd->pIntValue[i],dx, dy)>=1)
						goto lab;

					g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx+ccdMove*m, tempy-ccdMove*m,t_vel,t_acc);
					CFunction::DelaySec(0.1);
					if(g_pView->PatFind(i+1, pCmd->pIntValue[i],dx, dy)>=1)
						goto lab;

					g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx+ccdMove*m, tempy,t_vel,t_acc);
					CFunction::DelaySec(0.1);
					if(g_pView->PatFind(i+1, pCmd->pIntValue[i],dx, dy)>=1)
						goto lab;

					g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx+ccdMove*m, tempy+ccdMove*m,t_vel,t_acc);
					CFunction::DelaySec(0.1);
					if(g_pView->PatFind(i+1, pCmd->pIntValue[i],dx, dy)>=1)
						goto lab;

					g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx, tempy+ccdMove*m,t_vel,t_acc);
					CFunction::DelaySec(0.1);
					if(g_pView->PatFind(i+1, pCmd->pIntValue[i],dx, dy)>=1)
						goto lab;

					g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx-ccdMove*m, tempy+ccdMove*m,t_vel,t_acc);
					CFunction::DelaySec(0.1);
					if(g_pView->PatFind(i+1, pCmd->pIntValue[i],dx, dy)>=1)
						goto lab;

					g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx-ccdMove*m, tempy,t_vel,t_acc);
					CFunction::DelaySec(0.1);
					if(g_pView->PatFind(i+1, pCmd->pIntValue[i],dx, dy)>=1)
						goto lab;

					if(m>=ccdMoveCnt)
					{
						// �ƶ��Ժ���û�ҵ�
						g_pFrm->AddMsg("Find Mark Error!");
						g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"Find Mark Error!");
						if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
							g_pFrm->m_wndEditBar.EnableWindow(TRUE);

						if(g_pFrm->m_pSysParam->nFindMarkJogMove == 1)
						{//
							if(IDNO == AfxMessageBox(_T("ģ��ƥ��ʧ�ܣ��ֶ�����?"),MB_YESNO))
							{
								// ���ô����ʾ���˳��������ӹ�
								return FALSE;
							}else
							{
								g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"");
								// �����ֶ����������λȷ�����ٴβ��ң����Ҳ�����ֱ���˳�
								CFindmarkJog m_JogXYZ;
								if(IDOK==m_JogXYZ.DoModal())
								{
									dx = IMG_WIDTH*0.5;
									dy = IMG_HEIGHT*0.5;
									goto lab; 
								}else
								{
									return FALSE;
								}
							}
						}else
						{   // ����Ҫ�ֶ�
							return FALSE;
						}
					}
				}
			}
lab:	 
			// ���ҳɹ�
			g_pView->PatCenter(dx,dy);	

			m_tMarkFindPos[i].x = dx;   // ���ҵ���ģ��λ�û�е����
			m_tMarkFindPos[i].y = dy;

			strtemp.Format("Mark��%i��е����:	x[%d],y[%d]", i, m_tMarkFindPos[i].x, m_tMarkFindPos[i].y);
			g_pFrm->AddMsg(strtemp);
		}
		else
		{
			g_pFrm->AddMsg("LoadMark Fail");
			return FALSE;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	double dMarkStdDistace = 0.0;
	dMarkStdDistace = (pCmd->pDblValue[4]-pCmd->pDblValue[0]) * (pCmd->pDblValue[4]-pCmd->pDblValue[0]) +
		(pCmd->pDblValue[5]-pCmd->pDblValue[1]) * (pCmd->pDblValue[5]-pCmd->pDblValue[1]);

	dMarkStdDistace = sqrt(dMarkStdDistace);

	double dSearchDistance = 0.0;
	dSearchDistance = (m_tMarkFindPos[1].x-m_tMarkFindPos[0].x) * (m_tMarkFindPos[1].x-m_tMarkFindPos[0].x) +
		(m_tMarkFindPos[1].y-m_tMarkFindPos[0].y) * (m_tMarkFindPos[1].y-m_tMarkFindPos[0].y);
	dSearchDistance = sqrt(dSearchDistance);

	if (abs(dMarkStdDistace-dSearchDistance)>1)
	{
		AfxMessageBox("�ҵ���Mark���������ģ��ʱMark�����ƫ���1mm!");
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	double dStdAngle=0;   // ��׼ģ��Ƕ�
	double dMarkAngle =0; // ����ģ��Ƕ�

	switch(g_pFrm->m_pSysParam->iMarkNum)
	{
	case 1:
		m_tOffset.x = m_tMarkFindPos[0].x -pCmd->pDblValue[0];  
		m_tOffset.y = m_tMarkFindPos[0].y -pCmd->pDblValue[1];
		break;
	case 2:  // 
		m_tMarkCenter.x = (pCmd->pDblValue[0] + pCmd->pDblValue[4])/2;   // 2���궨Mark��X����
		m_tMarkCenter.y = (pCmd->pDblValue[1] + pCmd->pDblValue[5])/2;   // 2���궨Mark��Y���� 

		// ����ģ�����ĺͱ궨ģ������ƫ��
		m_tOffset.x = ((m_tMarkFindPos[0].x + m_tMarkFindPos[1].x)/2 - m_tMarkCenter.x);
		m_tOffset.y = ((m_tMarkFindPos[0].y + m_tMarkFindPos[1].y)/2 - m_tMarkCenter.y);

		dStdAngle  = atan2(pCmd->pDblValue[1]-pCmd->pDblValue[5],pCmd->pDblValue[0]-pCmd->pDblValue[4]);
		dMarkAngle = atan2(m_tMarkFindPos[0].y-m_tMarkFindPos[1].y, m_tMarkFindPos[0].x-m_tMarkFindPos[1].x);

		// ����Ƕ�ƫ��
		m_dRotateAngle = 0;  
		m_dRotateAngle = fabs(dMarkAngle)-fabs(dStdAngle);
		if(dMarkAngle<0) 
			m_dRotateAngle=-m_dRotateAngle;

		if(fabs(m_dRotateAngle) > M_PI_2)
		{
			if(m_dRotateAngle<0)
				m_dRotateAngle += M_PI;
			else
				m_dRotateAngle -= M_PI;
		}	

#ifdef  DEBUG_SD950
		strtemp.Format("Mark X:%.3f",m_tOffset.x);
		g_pFrm->AddMsg(strtemp);
		strtemp.Format("Mark y:%.3f",m_tOffset.y);
		g_pFrm->AddMsg(strtemp);
		strtemp.Format("Mark A:%.3f",m_dRotateAngle);
		g_pFrm->AddMsg(strtemp);
#endif
		break;
	default:
		NULL;
	}

	return TRUE;
}

BOOL CCmdRun::FindMarkEx(tgCmdLine *pCmd)
{
	BOOL bFindOK = TRUE;

	g_pFrm->AddMsg("[SEARCH MARK] --> BEGIN ......");
	int nGID = pCmd->pIntValue[2];
	m_tOffsetEX[nGID].x = 0;
	m_tOffsetEX[nGID].y = 0;
	m_dRotateAngleEX[nGID] = 0;

	m_MarkGropID = nGID;
	bFindOK = SubFindMarkEx(pCmd);

	g_pFrm->AddMsg("[SEARCH MARK] --> END");

	if(!bFindOK)
		g_pFrm->AddMsg("Find MARK Error.");

	////
	return bFindOK;
}
BOOL CCmdRun::SubFindMarkEx(tgCmdLine *pCmd)
{ 
	double dx=0, dy=0;
	double t_vel = g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax;
	double t_acc = g_pFrm->m_mtrParamGts[X_AXIS].m_Acc;
	CString  strtemp;
	int nGID = pCmd->pIntValue[2];

	for(int i=0; i<g_pFrm->m_pSysParam->iMarkNum; i++)
	{
		//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
		g_pFrm->m_Precision.GetCurPosMM();
		double m_curXPos = m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
		double m_curYPos = m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
		//		double m_curZPos = m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

		// 		//ͼ��ɨ���Z�ᵽλ
		// 		if(((m_curZPos-pCmd->pDblValue[4*i +2])>1)||((m_curZPos-pCmd->pDblValue[4*i +2])<-1))
		// 		{
		// 			m_pController->AxisMove(Z_AXIS, pCmd->pDblValue[4*i +2], g_pFrm->m_pSysParam->dSpeedImageMoveZ, m_cmdParam.dAccZ);
		// 		}

		//����ͼ��ɨ���
		if((fabs(m_curXPos-pCmd->pDblValue[4*i +0])>1)||(fabs(m_curYPos-pCmd->pDblValue[4*i +1])>1))
		{
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,pCmd->pDblValue[4*i +0], pCmd->pDblValue[4*i +1],t_vel, t_acc);
		}

		CFunction::DelaySec(0.1);
		g_pView->m_pCamera->SoftTrigger();
		g_pView->m_pCamera->WaitGrabOneFrame();

		if(g_pView->PatFind(i+1+nGID*2, pCmd->pIntValue[i],dx, dy)<1)
		{
			if (K_RUN_STS_STOP == m_tSysStatus)
			{
				return FALSE;
			}
			//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
			g_pFrm->m_Precision.GetCurPosMM();
			double xx = m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
			double yy = m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
			double tempx=xx;
			double tempy=yy;

			double ccdMove=g_pFrm->m_pSysParam->m_ccdMoveDis;
			int    ccdMoveCnt=g_pFrm->m_pSysParam->m_ccdMoveCnt;

			for (int m=1;m<=ccdMoveCnt;m++)
			{
				g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx-ccdMove*m, tempy-ccdMove*m,t_vel,t_acc);
				CFunction::DelaySec(0.1);
				g_pView->m_pCamera->SoftTrigger();
				g_pView->m_pCamera->WaitGrabOneFrame();
				if(g_pView->PatFind(i+1+nGID*2, pCmd->pIntValue[i],dx, dy)>=1)
					goto lab;

				g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx, tempy-ccdMove*m,t_vel,t_acc);
				CFunction::DelaySec(0.1);
				g_pView->m_pCamera->SoftTrigger();
				g_pView->m_pCamera->WaitGrabOneFrame();
				if(g_pView->PatFind(i+1+nGID*2, pCmd->pIntValue[i],dx, dy)>=1)
					goto lab;

				g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx+ccdMove*m, tempy-ccdMove*m,t_vel,t_acc);
				CFunction::DelaySec(0.1);
				g_pView->m_pCamera->SoftTrigger();
				g_pView->m_pCamera->WaitGrabOneFrame();
				if(g_pView->PatFind(i+1+nGID*2, pCmd->pIntValue[i],dx, dy)>=1)
					goto lab;

				g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx+ccdMove*m, tempy,t_vel,t_acc);
				CFunction::DelaySec(0.1);
				g_pView->m_pCamera->SoftTrigger();
				g_pView->m_pCamera->WaitGrabOneFrame();
				if(g_pView->PatFind(i+1+nGID*2, pCmd->pIntValue[i],dx, dy)>=1)
					goto lab;

				g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx+ccdMove*m, tempy+ccdMove*m,t_vel,t_acc);
				CFunction::DelaySec(0.1);
				g_pView->m_pCamera->SoftTrigger();
				g_pView->m_pCamera->WaitGrabOneFrame();
				if(g_pView->PatFind(i+1+nGID*2, pCmd->pIntValue[i],dx, dy)>=1)
					goto lab;

				g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx, tempy+ccdMove*m,t_vel,t_acc);
				CFunction::DelaySec(0.1);
				g_pView->m_pCamera->SoftTrigger();
				g_pView->m_pCamera->WaitGrabOneFrame();
				if(g_pView->PatFind(i+1+nGID*2, pCmd->pIntValue[i],dx, dy)>=1)
					goto lab;

				g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx-ccdMove*m, tempy+ccdMove*m,t_vel,t_acc);
				CFunction::DelaySec(0.1);
				g_pView->m_pCamera->SoftTrigger();
				g_pView->m_pCamera->WaitGrabOneFrame();
				if(g_pView->PatFind(i+1+nGID*2, pCmd->pIntValue[i],dx, dy)>=1)
					goto lab;

				g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx-ccdMove*m, tempy,t_vel,t_acc);
				CFunction::DelaySec(0.1);
				g_pView->m_pCamera->SoftTrigger();
				g_pView->m_pCamera->WaitGrabOneFrame();
				if(g_pView->PatFind(i+1+nGID*2, pCmd->pIntValue[i],dx, dy)>=1)
					goto lab;

				if(m>=ccdMoveCnt)
				{
					// �ƶ��Ժ���û�ҵ�
					g_pFrm->AddMsg("Find Mark Error!");
					g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"Find Mark Error!");
					if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
						g_pFrm->m_wndEditBar.EnableWindow(TRUE);

					if(IDNO == AfxMessageBox(_T("ģ��ƥ��ʧ�ܣ��ֶ�����?"),MB_YESNO))
					{
						// �ƶ��Ժ���û�ҵ�
						g_pFrm->AddMsg("Find Mark Error!");
						g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"Find Mark Error!");
						m_tOffsetEX[nGID].x = -9999;
						m_tOffsetEX[nGID].y = -9999;
						return FALSE;
					}
					else
					{
						g_pView->m_pCamera->SetTriggerMode(false);
						g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"");
						// �����ֶ����������λȷ�����ٴβ��ң����Ҳ�����ֱ���˳�
						CFindmarkJog m_JogXYZ;
						if(IDOK==m_JogXYZ.DoModal())
						{
							dx = IMG_WIDTH*0.5;
							dy = IMG_HEIGHT*0.5;
							goto lab; 
						}else
						{
							return FALSE;
						}
					}

				}
			}
		}
lab:	 
		// ���ҳɹ�
		g_pFrm->m_Precision.GetCurPosMM();
		g_pView->PatCenter(dx,dy);	

		m_tMarkFindPosEX[nGID][i].x = dx;   // ���ҵ���ģ��λ�û�е����
		m_tMarkFindPosEX[nGID][i].y = dy;   
		//}
		// 		else
		// 		{
		// 			g_pFrm->AddMsg("LoadMark Fail");
		// 			m_tOffsetEX[nGID].x = -9999;
		// 			m_tOffsetEX[nGID].y = -9999;
		// 			return FALSE;
		// 		}
	}

	//////////////////////////////////////////////////////////////////////////
	double dStdAngle=0;   // ��׼ģ��Ƕ�
	double dMarkAngle =0; // ����ģ��Ƕ�

	switch(g_pFrm->m_pSysParam->iMarkNum)
	{
	case 1: 
		m_tOffsetEX[nGID].x = m_tMarkFindPosEX[nGID][0].x -pCmd->pDblValue[0];  
		m_tOffsetEX[nGID].y = m_tMarkFindPosEX[nGID][0].y -pCmd->pDblValue[1];
		break;
	case 2:  // 
		m_tMarkCenterEX[nGID].x = (pCmd->pDblValue[0] + pCmd->pDblValue[4])/2;   // 2���궨Mark��X����
		m_tMarkCenterEX[nGID].y = (pCmd->pDblValue[1] + pCmd->pDblValue[5])/2;   // 2���궨Mark��Y���� 

		// ����ģ�����ĺͱ궨ģ������ƫ��
		m_tOffsetEX[nGID].x = ((m_tMarkFindPosEX[nGID][0].x + m_tMarkFindPosEX[nGID][1].x)/2 - m_tMarkCenterEX[nGID].x);
		m_tOffsetEX[nGID].y = ((m_tMarkFindPosEX[nGID][0].y + m_tMarkFindPosEX[nGID][1].y)/2 - m_tMarkCenterEX[nGID].y);

		dStdAngle  = atan2(pCmd->pDblValue[1]-pCmd->pDblValue[5],pCmd->pDblValue[0]-pCmd->pDblValue[4]);
		dMarkAngle = atan2(m_tMarkFindPosEX[nGID][0].y-m_tMarkFindPosEX[nGID][1].y, m_tMarkFindPosEX[nGID][0].x-m_tMarkFindPosEX[nGID][1].x);

		// ����Ƕ�ƫ��
		m_dRotateAngleEX[nGID] = 0;  
		m_dRotateAngleEX[nGID] = fabs(dMarkAngle)-fabs(dStdAngle);
		if(dMarkAngle<0) 
			m_dRotateAngleEX[nGID]=-m_dRotateAngleEX[nGID];
		
		{
			CString strS;
			strS.Format("Mark	X:[%.4f] y:[%.f]  RotAng:[%.4f]  offx:[%.4f]  offy:[%.4f]", m_tMarkCenterEX[nGID].x, m_tMarkCenterEX[nGID].y, m_dRotateAngleEX[nGID], m_tOffsetEX[nGID].x, m_tOffsetEX[nGID].y);
			g_pFrm->AddMsg(strS);
		}

		if(fabs(m_dRotateAngleEX[nGID]) > M_PI_2)
		{
			if(m_dRotateAngleEX[nGID]<0)
				m_dRotateAngleEX[nGID] += M_PI;
			else
				m_dRotateAngleEX[nGID] -= M_PI;
		}	

		break;
	default:
		NULL;
	}

	return TRUE;
}

BOOL  CCmdRun::ReviewFindMark(tgCmdLine *pCmd)
{ 
	double dx=0, dy=0;
	double t_vel = g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax;
	double t_acc = g_pFrm->m_mtrParamGts[X_AXIS].m_Acc;
	CString  strtemp;
	int nGID = 0;// pCmd->pIntValue[2];
	g_pView->m_pCamera->SetTriggerMode(true);

	for(int i=0; i<g_pFrm->m_pSysParam->iMarkNum; i++)
	{
		g_pFrm->m_Precision.GetCurPosMM();
		double m_curXPos = m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
		double m_curYPos = m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];

// 		//ͼ��ɨ���Z�ᵽλ
// 		if(((m_curZPos-pCmd->pDblValue[4*i +2])>1)||((m_curZPos-pCmd->pDblValue[4*i +2])<-1))
// 		{
// 			m_pController->AxisMove(Z_AXIS, pCmd->pDblValue[4*i +2], g_pFrm->m_pSysParam->dSpeedImageMoveZ, m_cmdParam.dAccZ);
// 		}

		//����ͼ��ɨ���
		if((fabs(m_curXPos-pCmd->pDblValue[4*i +0])>1)||(fabs(m_curYPos-pCmd->pDblValue[4*i +1])>1))
		{
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,pCmd->pDblValue[4*i +0], pCmd->pDblValue[4*i +1],t_vel, t_acc);
		}

		CFunction::DelaySec(0.05);
		g_pView->m_pCamera->SoftTrigger();
		g_pView->m_pCamera->WaitGrabOneFrame();

		if(g_pView->PatFind(i+1+nGID*2, pCmd->pIntValue[i],dx, dy)<1)
		{
			return FALSE;
		}

		// ���ҳɹ�
		g_pFrm->m_Precision.GetCurPosMM();
		g_pView->PatCenter(dx,dy);	

		m_tMarkFindPosEX[nGID][i].x = dx;   // ���ҵ���ģ��λ�û�е����
		m_tMarkFindPosEX[nGID][i].y = dy;   
	}

	//////////////////////////////////////////////////////////////////////////
	double dStdAngle=0;   // ��׼ģ��Ƕ�
	double dMarkAngle =0; // ����ģ��Ƕ�

	switch(g_pFrm->m_pSysParam->iMarkNum)
	{
	case 1: 
		m_tOffsetEX[nGID].x = m_tMarkFindPosEX[nGID][0].x -pCmd->pDblValue[0];  
		m_tOffsetEX[nGID].y = m_tMarkFindPosEX[nGID][0].y -pCmd->pDblValue[1];
		break;
	case 2:  // 
		m_tMarkCenterEX[nGID].x = (pCmd->pDblValue[0] + pCmd->pDblValue[4])/2;   // 2���궨Mark��X����
		m_tMarkCenterEX[nGID].y = (pCmd->pDblValue[1] + pCmd->pDblValue[5])/2;   // 2���궨Mark��Y���� 

		// ����ģ�����ĺͱ궨ģ������ƫ��
		m_tOffsetEX[nGID].x = ((m_tMarkFindPosEX[nGID][0].x + m_tMarkFindPosEX[nGID][1].x)/2 - m_tMarkCenterEX[nGID].x);
		m_tOffsetEX[nGID].y = ((m_tMarkFindPosEX[nGID][0].y + m_tMarkFindPosEX[nGID][1].y)/2 - m_tMarkCenterEX[nGID].y);

		dStdAngle  = atan2(pCmd->pDblValue[1]-pCmd->pDblValue[5],pCmd->pDblValue[0]-pCmd->pDblValue[4]);
		dMarkAngle = atan2(m_tMarkFindPosEX[nGID][0].y-m_tMarkFindPosEX[nGID][1].y, m_tMarkFindPosEX[nGID][0].x-m_tMarkFindPosEX[nGID][1].x);

		// ����Ƕ�ƫ��
		m_dRotateAngleEX[nGID] = 0;  
		m_dRotateAngleEX[nGID] = fabs(dMarkAngle)-fabs(dStdAngle);
		if(dMarkAngle<0) 
			m_dRotateAngleEX[nGID]=-m_dRotateAngleEX[nGID];

		if(fabs(m_dRotateAngleEX[nGID]) > M_PI_2)
		{
			if(m_dRotateAngleEX[nGID]<0)
				m_dRotateAngleEX[nGID] += M_PI;
			else
				m_dRotateAngleEX[nGID] -= M_PI;
		}	

		break;
	default:
		NULL;
	}

	return TRUE;
}

BOOL CCmdRun::GetZCheckCmdPosition(double x, double y, double *pX, double *pY, double dOffsetX, double dOffsetY)
{
	*pX = (x + dOffsetX);
	*pY = (y + dOffsetY);

	*pX += m_tOffset.x;
	*pY += m_tOffset.y;

	return TRUE;
}

BOOL CCmdRun::GetRepeatPosition(tgCmdLine *pCmd, double *pX, double *pY)	// ����ʱ,��ȡ���о���λ�õ�3������
{
	CString strTemp;
	for (int i=0; i<3; i++)
	{
		pX[i] = pCmd->pDblValue[i*4];
		pY[i] = pCmd->pDblValue[i*4+1];

		strTemp.Format("S%d:		x:%.4f	y:%.4f", i+1, pX[i], pY[i]);
		g_pFrm->AddMsg(strTemp);

		pX[i] -= m_tMarkCenterEX[0].x;
		pY[i] -= m_tMarkCenterEX[0].y;

		pX[3] = pX[i]*cos(m_dRotateAngleEX[0]) - pY[i]*sin(m_dRotateAngleEX[0]);
		pY[3] = pY[i]*cos(m_dRotateAngleEX[0]) + pX[i]*sin(m_dRotateAngleEX[0]);

		pX[i] = pX[3] + (m_tMarkFindPosEX[0][0].x + m_tMarkFindPosEX[0][1].x)/2;
		pY[i] = pY[3] + (m_tMarkFindPosEX[0][0].y + m_tMarkFindPosEX[0][1].y)/2;

		strTemp.Format("E%d:		x:%.4f	y:%.4f", i+1, pX[i], pY[i]);
		g_pFrm->AddMsg(strTemp);
	}

	return TRUE;
}

BOOL  CCmdRun::GetCmdCameraPosition(tgCmdLine *pCmd, double *pX, double *pY)
{
	if(!CDlgPathEdit::IsTrackPoint(*pCmd, FALSE))
		return FALSE;

	*pX = (pCmd->pDblValue[0]);
	*pY = (pCmd->pDblValue[1]);

	double xPos[2], yPos[2];

	xPos[0] = pCmd->pDblValue[0];
	yPos[0] = pCmd->pDblValue[1];

	xPos[0] -= m_tMarkCenterEX[0].x;
	yPos[0] -= m_tMarkCenterEX[0].y;

	xPos[1] = xPos[0]*cos(m_dRotateAngleEX[0]) - yPos[0]*sin(m_dRotateAngleEX[0]);
	yPos[1] = yPos[0]*cos(m_dRotateAngleEX[0]) + xPos[0]*sin(m_dRotateAngleEX[0]);

	*pX = xPos[1] + (m_tMarkFindPosEX[0][0].x + m_tMarkFindPosEX[0][1].x)/2;
	*pY = yPos[1] + (m_tMarkFindPosEX[0][0].y + m_tMarkFindPosEX[0][1].y)/2;

	return TRUE;
}

BOOL  CCmdRun::GetCmdPositionEX(tgCmdLine *pCmd, double *pX, double *pY, double *pZ, double dOffsetX, double dOffsetY,int nGID)
{
	if(!CDlgPathEdit::IsTrackPoint(*pCmd, FALSE))
		return FALSE;

	*pX = (pCmd->pDblValue[0] + dOffsetX);
	*pY = (pCmd->pDblValue[1] + dOffsetY);
	if (RunMode_Dry == m_runMode)
	{
		*pZ = g_pFrm->m_pRobotParam->m_zSafePos;
	}
	else
	{
		*pZ = pCmd->pDblValue[2] + m_dFirstHeightOffset;
	}
	
	switch(g_pFrm->m_pSysParam->iMarkNum)
	{
	case 0:
		return TRUE;
	case 1:
		*pX += m_tOffset.x;
		*pY += m_tOffset.y;
		return TRUE;
	default:
		NULL;
	}

	// m_iMarkNum == 2
	if (CMD_ZMETER == pCmd->tType)
	{
		double ccdx = *pX;  
		double ccdy = *pY;

		// Ŀ��λ����ͼ�����������кͱ궨ģ�����ĵĽǶȺͳ���
		double dPointAngle = atan2(double(ccdy-m_tMarkCenterEX[nGID].y), double(ccdx-m_tMarkCenterEX[nGID].x));
		double dPointLen = sqrt(double((ccdx-m_tMarkCenterEX[nGID].x) * (ccdx-m_tMarkCenterEX[nGID].x) +(ccdy-m_tMarkCenterEX[nGID].y) * (ccdy-m_tMarkCenterEX[nGID].y) ) );

		// ����ƫ���� Ŀ���ͱ궨����ϵ�е�λ��ƫ��
		double drx = dPointLen*cos(m_dRotateAngleEX[nGID]+dPointAngle)-dPointLen*cos(dPointAngle);
		double dry = dPointLen*sin(m_dRotateAngleEX[nGID]+dPointAngle)-dPointLen*sin(dPointAngle);
 
		*pX += drx+m_tOffsetEX[nGID].x;  // תΪ����(̽��)������������ 
		*pY += dry+m_tOffsetEX[nGID].y;
	}
	else
	{
		// Ŀ��λ�� תΪͼ����������λ�� 
		double ccdx = *pX-(g_pFrm->m_pSysParam->tOffset[m_CurToolNo].x);  
		double ccdy = *pY-(g_pFrm->m_pSysParam->tOffset[m_CurToolNo].y);

		if (0)
		{
			ccdx -= m_tMarkCenterEX[nGID].x;
			ccdy -= m_tMarkCenterEX[nGID].y;
			double xPos, yPos;

			xPos = ccdx*cos(m_dRotateAngleEX[nGID]) - ccdy*sin(m_dRotateAngleEX[nGID]);
			yPos = ccdy*cos(m_dRotateAngleEX[nGID]) + ccdx*sin(m_dRotateAngleEX[nGID]);

			*pX = xPos + (m_tMarkFindPosEX[nGID][0].x + m_tMarkFindPosEX[nGID][1].x)/2;
			*pY = yPos + (m_tMarkFindPosEX[nGID][0].y + m_tMarkFindPosEX[nGID][1].y)/2;

			*pX += g_pFrm->m_pSysParam->tOffset[m_CurToolNo].x;
			*pY += g_pFrm->m_pSysParam->tOffset[m_CurToolNo].y;
		}
		else
		{
			// Ŀ��λ����ͼ�����������кͱ궨ģ�����ĵĽǶȺͳ���
			double dPointAngle = atan2(double(ccdy-m_tMarkCenterEX[nGID].y), double(ccdx-m_tMarkCenterEX[nGID].x));
			double dPointLen = sqrt(double((ccdx-m_tMarkCenterEX[nGID].x) * (ccdx-m_tMarkCenterEX[nGID].x) +(ccdy-m_tMarkCenterEX[nGID].y) * (ccdy-m_tMarkCenterEX[nGID].y) ) );

			// ����ƫ���� Ŀ���ͱ궨����ϵ�е�λ��ƫ��
			double drx = dPointLen*cos(m_dRotateAngleEX[nGID]+dPointAngle)-dPointLen*cos(dPointAngle);
			double dry = dPointLen*sin(m_dRotateAngleEX[nGID]+dPointAngle)-dPointLen*sin(dPointAngle);

			// ��ƫ�������ӵ�ͼ������������ƫ����
			double dx = m_tOffsetEX[nGID].x+drx;  
			double dy = m_tOffsetEX[nGID].y+dry;

			*pX+=dx;  // תΪ������������ 
			*pY+=dy;
		}
	}

	return TRUE;
}
BOOL  CCmdRun::GetCmdPosition(tgCmdLine *pCmd, double *pX, double *pY, double *pZ, double dOffsetX, double dOffsetY, int iPosIndex)
{
	if(!CDlgPathEdit::IsTrackPoint(*pCmd, FALSE) || iPosIndex<0 || iPosIndex>pCmd->iDotNum-1)
		return FALSE;

	*pX = (pCmd->pDblValue[iPosIndex*4+0] + dOffsetX);
	*pY = (pCmd->pDblValue[iPosIndex*4+1] + dOffsetY);
	*pZ = pCmd->pDblValue[iPosIndex*4+2] + m_dFirstHeightOffset;

	switch(g_pFrm->m_pSysParam->iMarkNum)
	{
	case 0:
		return TRUE;
	case 1:
		*pX += m_tOffset.x;
		*pY += m_tOffset.y;
		return TRUE;
	default:
		NULL;
	}

	// m_iMarkNum == 2
	// Ŀ��λ�� תΪͼ����������λ�� 
	double ccdx = *pX-(g_pFrm->m_pSysParam->tOffset[m_CurToolNo].x);  
	double ccdy = *pY-(g_pFrm->m_pSysParam->tOffset[m_CurToolNo].y);

	// Ŀ��λ����ͼ�����������кͱ궨ģ�����ĵĽǶȺͳ���
	double dPointAngle = atan2(double(ccdy-m_tMarkCenter.y), double(ccdx-m_tMarkCenter.x));
	double dPointLen = sqrt(double((ccdx-m_tMarkCenter.x) * (ccdx-m_tMarkCenter.x) +(ccdy-m_tMarkCenter.y) * (ccdy-m_tMarkCenter.y) ) );

	// ����ƫ���� Ŀ���ͱ궨����ϵ�е�λ��ƫ��
	double drx = dPointLen*cos(m_dRotateAngle+dPointAngle)-dPointLen*cos(dPointAngle);
	double dry = dPointLen*sin(m_dRotateAngle+dPointAngle)-dPointLen*sin(dPointAngle);

#ifdef  DEBUG_SD950
	CString strtemp;
	strtemp.Format("Ŀ��λ��ƫ��dX:%.3f",drx);
	g_pFrm->AddMsg(strtemp);
	strtemp.Format("Ŀ��λ��ƫ��dy:%.3f",dry);
	g_pFrm->AddMsg(strtemp);
#endif
	// ��ƫ�������ӵ�ͼ������������ƫ����
	double dx = m_tOffset.x+drx;  
	double dy = m_tOffset.y+dry;

	*pX+=dx;  // תΪ������������ 
	*pY+=dy;

	return TRUE;
}

BOOL CCmdRun::RunListExpand()
{	
	//������ͷͼ��ƫ��
	if(!m_bModifiedTrack) return TRUE;
	//	g_pView->ImgStop();

	int pCurTempToolNo = -1; //��ǰִ�й��ߺ�
	double dZSetHeight = 10;	// �㽺�߶���Ի�׼�߶�

	CString strTemp;
	tgCmdLine pCmd;	

	if(g_pFrm->m_wndEditBar.m_rawList.GetCount()<1)
	{
		AfxMessageBox("No CMD");
		return FALSE;
	}

	RunListClear();	

	for(int i=0; i<g_pFrm->m_wndEditBar.m_rawList.GetCount(); i++)
	{	
		CFunction::DoEvents();		
		//////////////////////////////////////////////////////////////////////////
		POSITION posIndex=g_pFrm->m_wndEditBar.m_rawList.FindIndex(i);
		pCmd = g_pFrm->m_wndEditBar.m_rawList.GetAt(posIndex);

		if (pCmd.tType==CMD_TOOL)
		{
			pCurTempToolNo = pCmd.pIntValue[0];//���е�ָ��pIntValue[0]��Ϊ����ID����
			dZSetHeight    = pCmd.pDblValue[2];
			bNeedFillCMPList  = TRUE;
		}	

		if (pCurTempToolNo==-1)//û�����÷�
		{
			//��ͼ��ģʽ�˶�
			pCurTempToolNo = 2;
			m_CurToolNo = pCurTempToolNo;
			bNeedFillCMPList = FALSE;
		}
		else
		{
			//����Ҫѡ������Ͳ�������䷧
			if(!RunListOffset(&pCmd, g_pFrm->m_pSysParam->tOffset[pCurTempToolNo].x, g_pFrm->m_pSysParam->tOffset[pCurTempToolNo].y, 0/*g_pFrm->m_pSysParam->tOffset[pCurTempToolNo].z*/))
			{
				AfxMessageBox("�켣��λ��У��ƫ�Ƴ�������Ч�г̷�Χ������������ͼ�����ͷλ�ò���.");
				return FALSE;
			}
		}

		if(pCmd.tType == CMD_POINT			|| pCmd.tType == CMD_LINE_START 
			|| pCmd.tType==CMD_LINE_PASSING || pCmd.tType == CMD_LINE_END
			|| pCmd.tType == CMD_LINE_ARC	|| pCmd.tType == CMD_CIRCLE
			|| pCmd.tType == CMD_POT		|| pCmd.tType == CMD_WCL		||pCmd.tType == CMD_WCRECT
			|| pCmd.tType == CMD_SUPERSTART || pCmd.tType == CMD_SUPERPOINT ||pCmd.tType == CMD_SUPEREND
			|| pCmd.tType == CMD_SUPERDOT)
		{
			for (int j = 0;j<pCmd.iDotNum;j++)
			{
				if(pCurTempToolNo==2)
				{	//ͼ��ģʽ
					//pCmd.pDblValue[j*4+2] = g_pFrm->m_pSysParam->tPixelAdjustPos.z;
					pCmd.pDblValue[j*4+2] = g_pFrm->m_pRobotParam->m_zSafePos;
				}
				else if(pCurTempToolNo==1)
				{	
					pCmd.pDblValue[j*4+2] = dZSetHeight+g_pFrm->m_pSysParam->dCaliNeedleDelta;
				}
				else if(pCurTempToolNo==0)
				{
					pCmd.pDblValue[j*4+2] = g_pFrm->m_pRobotParam->m_zContactPos - dZSetHeight;
					//pCmd.pDblValue[j*4+2] = dZSetHeight+g_pFrm->m_pSysParam->dCaliJetDelta;
				}
			}
		}
		if (pCmd.tType == CMD_FILEDELIMITER)//  
		{
			pCmd.pIntValue[1] = i;
		}
		if (pCmd.tType == CMD_FILESCRPEWORK)//
		{
			pCmd.pIntValue[1] = i;
		}
		m_cmdRunList.AddTail(pCmd);
	}

	g_pFrm->SetFocus();
	g_pFrm->SetForegroundWindow();
	m_bModifiedTrack=FALSE;

	return TRUE;
}

BOOL CCmdRun::RunListOffset(tgCmdLine *pCmd, double dOffsetX, double dOffsetY,double dOffsetZ)
{
	switch(pCmd->tType)//////����ͼ������ͷ��ƫ��ͼ����͸߶ȼ�ⲻ��Ҫ
	{
		//case CMD_BRUSH_AREA:
	case CMD_DUMMY:
	case CMD_POINT:
	case CMD_LINE_START:
	case CMD_LINE_PASSING:
	case CMD_LINE_ARC:
	case CMD_LINE_END:
	case CMD_POT:
	case CMD_SUPERDOT:
	case CMD_SUPERSTART:	// �ɽ���
	case CMD_SUPERPOINT:	// �ɽ���
	case CMD_SUPEREND:		// �ɽ���
		pCmd->pDblValue[0] += dOffsetX;
		pCmd->pDblValue[1] += dOffsetY;
		pCmd->pDblValue[2] += dOffsetZ;

		if(pCmd->pDblValue[0] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[0] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[1] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[1] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[2] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[2] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
		{
			pCmd->pDblValue[0] -= dOffsetX;
			pCmd->pDblValue[1] -= dOffsetY;
			pCmd->pDblValue[2] -= dOffsetZ;
			return FALSE;
		}
		break;
	case CMD_WCL://����ֱ����������
		pCmd->pDblValue[0] += dOffsetX;//mark0
		pCmd->pDblValue[1] += dOffsetY;
		pCmd->pDblValue[2] += dOffsetZ;

		pCmd->pDblValue[4] += dOffsetX;//mark1
		pCmd->pDblValue[5] += dOffsetY;
		pCmd->pDblValue[6] += dOffsetZ;

		if(pCmd->pDblValue[0] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[0] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[1] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[1] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[2] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[2] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
		{
			pCmd->pDblValue[0] -= dOffsetX;
			pCmd->pDblValue[1] -= dOffsetY;
			pCmd->pDblValue[2] -= dOffsetZ;
			return FALSE;
		}

		if(pCmd->pDblValue[4] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[4] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[5] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[5] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[6] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[6] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
		{
			pCmd->pDblValue[4] -= dOffsetX;
			pCmd->pDblValue[5] -= dOffsetY;
			pCmd->pDblValue[6] -= dOffsetZ;
			return FALSE;
		}

		break;
	case CMD_WCRECT:   // ���ؾ���
		pCmd->pDblValue[0] += dOffsetX;//mark0
		pCmd->pDblValue[1] += dOffsetY;
		pCmd->pDblValue[2] += dOffsetZ;

		pCmd->pDblValue[4] += dOffsetX;//mark1
		pCmd->pDblValue[5] += dOffsetY;
		pCmd->pDblValue[6] += dOffsetZ;

		pCmd->pDblValue[8] += dOffsetX;//mark2
		pCmd->pDblValue[9] += dOffsetY;
		pCmd->pDblValue[10] += dOffsetZ;

		pCmd->pDblValue[12] += dOffsetX;//mark3
		pCmd->pDblValue[13] += dOffsetY;
		pCmd->pDblValue[14] += dOffsetZ;

		pCmd->pDblValue[16] += dOffsetX;//mark4
		pCmd->pDblValue[17] += dOffsetY;
		pCmd->pDblValue[18] += dOffsetZ;

		if(pCmd->pDblValue[0] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[0] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[1] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[1] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[2] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[2] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
		{
			pCmd->pDblValue[0] -= dOffsetX;
			pCmd->pDblValue[1] -= dOffsetY;
			pCmd->pDblValue[2] -= dOffsetZ;
			return FALSE;
		}

		if(pCmd->pDblValue[4] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[4] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[5] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[5] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[6] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[6] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
		{
			pCmd->pDblValue[4] -= dOffsetX;
			pCmd->pDblValue[5] -= dOffsetY;
			pCmd->pDblValue[6] -= dOffsetZ;
			return FALSE;
		}

		if(pCmd->pDblValue[8] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[8] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[9] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[9] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[10] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[10] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
		{
			pCmd->pDblValue[8] -= dOffsetX;
			pCmd->pDblValue[9] -= dOffsetY;
			pCmd->pDblValue[10] -= dOffsetZ;
			return FALSE;
		}

		if(pCmd->pDblValue[12] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[12] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[13] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[13] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[14] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[14] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
		{
			pCmd->pDblValue[12] -= dOffsetX;
			pCmd->pDblValue[13] -= dOffsetY;
			pCmd->pDblValue[14] -= dOffsetZ;
			return FALSE;
		}

		if(pCmd->pDblValue[16] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[16] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[17] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[17] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
			||pCmd->pDblValue[18] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[18] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
		{
			pCmd->pDblValue[16] -= dOffsetX;
			pCmd->pDblValue[17] -= dOffsetY;
			pCmd->pDblValue[18] -= dOffsetZ;
			return FALSE;
		}
		break;
	case CMD_CIRCLE:
		if(pCmd->pIntValue[4]==0)
		{//Բ�ģ�ֱ��
			pCmd->pDblValue[0] += dOffsetX;
			pCmd->pDblValue[1] += dOffsetY;
			pCmd->pDblValue[2] += dOffsetZ;

			if(pCmd->pDblValue[0] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[0] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
				||pCmd->pDblValue[1] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[1] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
				||pCmd->pDblValue[2] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[2] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
			{
				pCmd->pDblValue[0] -= dOffsetX;
				pCmd->pDblValue[1] -= dOffsetY;
				pCmd->pDblValue[2] -= dOffsetZ;
				return FALSE;
			}
		}
		else
		{//����
			pCmd->pDblValue[0] += dOffsetX;
			pCmd->pDblValue[1] += dOffsetY;
			pCmd->pDblValue[2] += dOffsetZ;

			if(pCmd->pDblValue[0] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[0] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
				||pCmd->pDblValue[1] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[1] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
				||pCmd->pDblValue[2] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[2] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
			{
				pCmd->pDblValue[0] -= dOffsetX;
				pCmd->pDblValue[1] -= dOffsetY;
				pCmd->pDblValue[2] -= dOffsetZ;
				return FALSE;
			}

			pCmd->pDblValue[4] += dOffsetX;
			pCmd->pDblValue[5] += dOffsetY;
			pCmd->pDblValue[6] += dOffsetZ;

			if(pCmd->pDblValue[4] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[4] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
				||pCmd->pDblValue[5] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[5] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
				||pCmd->pDblValue[6] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[6] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
			{
				pCmd->pDblValue[4] -= dOffsetX;
				pCmd->pDblValue[5] -= dOffsetY;
				pCmd->pDblValue[6] -= dOffsetZ;
				return FALSE;
			}

			pCmd->pDblValue[8] += dOffsetX;
			pCmd->pDblValue[9] += dOffsetY;
			pCmd->pDblValue[10] += dOffsetZ;

			if(pCmd->pDblValue[8] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[8] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
				||pCmd->pDblValue[9] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[9] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
				||pCmd->pDblValue[10] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[10] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
			{
				pCmd->pDblValue[8] -= dOffsetX;
				pCmd->pDblValue[9] -= dOffsetY;
				pCmd->pDblValue[10] -= dOffsetZ;
				return FALSE;
			}

			pCmd->pDblValue[12] += dOffsetX;
			pCmd->pDblValue[13] += dOffsetY;
			pCmd->pDblValue[14] += dOffsetZ;

			if(pCmd->pDblValue[12] < g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative || pCmd->pDblValue[12] > g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive
				||pCmd->pDblValue[13] < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative || pCmd->pDblValue[13] > g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive
				||pCmd->pDblValue[14] < g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelNegative || pCmd->pDblValue[14] > g_pFrm->m_mtrParamGts[Z_AXIS].m_MaxTravelPositive)
			{
				pCmd->pDblValue[12] -= dOffsetX;
				pCmd->pDblValue[13] -= dOffsetY;
				pCmd->pDblValue[14] -= dOffsetZ;
				return FALSE;
			}
		}
	default:break;
	}

	return TRUE;
}
BOOL CCmdRun::RunListOffsetChange(int iStartIndex, int iEndIndex, double dOffsetX, double dOffsetY,double dOffsetZ)
{
	tgCmdLine pCmd;
	CString strTemp;

	if(g_pFrm->m_wndEditBar.m_rawList.GetCount()<1)
	{
		AfxMessageBox("û�й켣����");
		return FALSE;
	}
	if(iStartIndex<0 || iStartIndex>g_pFrm->m_wndEditBar.m_rawList.GetCount()-1)
	{
		AfxMessageBox("��ʼ�����ô���");
		return FALSE;
	}
	if(iStartIndex>iEndIndex || iEndIndex>g_pFrm->m_wndEditBar.m_rawList.GetCount()-1)
	{
		AfxMessageBox("���������ô���");
		return FALSE;
	}

	//	g_pView->ImgStop();

	m_cmdParam.reset();
	m_cmdParam.dAccXY = g_pFrm->m_mtrParamGts[X_AXIS].m_Acc;
	m_cmdParam.dAccZ = g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc;

	for(int i=iStartIndex; i<iEndIndex+1; i++)
	{		
		CFunction::DoEvents();
		//////////////////////////////////////////////////////////////////////////
		POSITION pos = g_pFrm->m_wndEditBar.m_rawList.FindIndex(i);
		if(pos!=NULL)		
		{
			pCmd = g_pFrm->m_wndEditBar.m_rawList.GetAt(pos);
			if(pCmd.tType == CMD_POINT			
				||pCmd.tType == CMD_DUMMY
				||pCmd.tType == CMD_CIRCLE
				||pCmd.tType == CMD_BRUSH_AREA
				||pCmd.tType == CMD_LINE_START
				||pCmd.tType == CMD_LINE_ARC
				||pCmd.tType == CMD_SUPERSTART
				||pCmd.tType == CMD_SUPEREND
				||pCmd.tType == CMD_SUPERPOINT
				||pCmd.tType == CMD_SUPERDOT
				||pCmd.tType == CMD_MARK
				||pCmd.tType == CMD_LINE_PASSING
				||pCmd.tType == CMD_LINE_END
				||pCmd.tType == CMD_WCL
				||pCmd.tType == CMD_WCRECT
				||pCmd.tType == CMD_POT)
			{
				if(!RunListOffset(&pCmd, dOffsetX, dOffsetY,dOffsetZ))
				{
					g_pFrm->m_wndEditBar.SetCmdCurSel(i);
					AfxMessageBox("�켣��λ�ó�������Ч�γɷ�Χ�����������ò���.");
					return FALSE;
				}
				g_pFrm->m_wndEditBar.m_rawList.InsertAfter(pos,pCmd);
				g_pFrm->m_wndEditBar.m_rawList.RemoveAt(pos);
			}
		}
		else 
			return FALSE;
	}

	g_pFrm->SetFocus();
	g_pFrm->SetForegroundWindow();
	m_bModifiedTrack=TRUE;

	return TRUE;
}
void CCmdRun::RunListClear()
{
	m_cmdRunList.RemoveAll();//����Ҫ��ɾ���б������ڴ�
	m_iStepIndex=-1;
}
void CCmdRun::RunListShow()
{
	CString strTemp, strText;
	int iCount = m_cmdRunList.GetCount();
	g_pFrm->AddMsg("Cmd Expand Begin.....");
	for(int i=0; i<iCount; i++)
	{
		strTemp.Format("%s->", szCmdline[m_cmdRunList.GetAt(m_cmdRunList.FindIndex(i)).tType]);
		strText = strTemp;
		for(int j=0; j<m_cmdRunList.GetAt(m_cmdRunList.FindIndex(i)).iDblNum; j++)
		{
			strTemp.Format("%.3f,",m_cmdRunList.GetAt(m_cmdRunList.FindIndex(i)).pDblValue[j]);
			strText += strTemp;
		}
		g_pFrm->AddMsg(strText);
	}
	g_pFrm->AddMsg("Cmd Expand End.....");
}
short CCmdRun::RunDelay(tgCmdLine *pCmd)
{
	short sTemp=RTN_NONE;
	if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
	{
		CFunction::DelayExSec(pCmd->pDblValue[0]);
	}
	sTemp  = m_pController->TS_ListDelay(CRD1,pCmd->pDblValue[0]);
	return sTemp;
}
short CCmdRun::RunPause()
{
	short sTemp  = m_pController->TS_ListPausePoint(CRD1);
	return sTemp;
}
short CCmdRun::RunHome()
{
	t_Base hdp;
	::ZeroMemory(&hdp,sizeof(t_Base));

	double dx,dy,dz,du;

	dx = g_pFrm->m_pSysParam->tDefHomePos.x;//������Ҫͨ���˵������Զ����ԭ��λ��
	dy = g_pFrm->m_pSysParam->tDefHomePos.y;
	dz = g_pFrm->m_pSysParam->tDefHomePos.z;
	du = g_pFrm->m_pSysParam->tDefHomePos.u;

	hdp.dVelXY = m_cmdParam.dVelXY;
	hdp.dVelZ  = m_cmdParam.dVelZ;
	g_pFrm->m_Precision.Measure2Real(dy);
	hdp.pos.x = dx;
	hdp.pos.y = dy;
	hdp.pos.z = dz;
	hdp.pos.a = du;

	short sTemp  = m_pController->TS_ListGoHome(CRD1,&dcp[m_CurToolNo],&hdp);
	return sTemp;
}
short CCmdRun::RunSetIO(tgCmdLine *pCmd)
{
	short sTemp;
	//int uiIndex,tempIndex;

	if(pCmd->pIntValue[0] == 0)//����
	{
		sTemp = m_pController->TS_ListWaitInp(CRD1,(unsigned int)pCmd->pIntValue[1],TRUE);
		if(sTemp == RTN_STP) return RTN_STP;
		if(sTemp == RTN_ESTP) return RTN_ESTP;
	}
	else if(pCmd->pIntValue[0] == 1)//���
	{
		sTemp = m_pController->TS_ListOutp(CRD1,(unsigned int)pCmd->pIntValue[1],pCmd->pIntValue[2]);
		if(sTemp == RTN_STP) return RTN_STP;
		if(sTemp == RTN_ESTP) return RTN_ESTP;
	}

	return RTN_NONE;
}
short CCmdRun::RunDispensor(tgCmdLine *pCmd)
{//�㽺������ //ֻ���ؽ�������������
	m_CurToolNo = pCmd->pIntValue[0];

	short sTemp  = m_pController->TS_ListSetDispBit(CRD1,&dcp[pCmd->pIntValue[0]],pCmd->pIntValue[1]);//pCmd->pIntValue[0] ��ID
	return sTemp;
}
short CCmdRun::RunTool(tgCmdLine *pCmd)
{
	m_CurToolNo = pCmd->pIntValue[0];
	if(pCmd->pIntValue[0]==0)
	{
		dcp[m_CurToolNo].bIsJet = TRUE;
		m_cmdParam.nLowTime = pCmd->pIntValue[1];
		m_cmdParam.nHighTime = pCmd->pIntValue[2];
		m_cmdParam.lPulseNum = (long)pCmd->pDblValue[0];
		m_cmdParam.dZSetHeight = pCmd->pDblValue[1];
		m_DispCYtime = m_cmdParam.nLowTime ;
	}
	else if (pCmd->pIntValue[0]==1)
	{
		dcp[m_CurToolNo].bIsJet = FALSE;
		m_cmdParam.nLowTime = 10;
		m_cmdParam.nHighTime = 10;
		m_cmdParam.lPulseNum = 0;
	}
	return TRUE;
}
short CCmdRun::RunWashNeedle()
{//��ϴ
	t_BathPoint bp;
	::ZeroMemory(&bp,sizeof(t_Base));

	bp.dVelXY = g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax;
	bp.dVelZ  = g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax;

	bp.pos.x = g_pFrm->m_pRobotParam->m_structClean.m_posClean.x+g_pFrm->m_pSysParam->tOffset[0].x;   
	bp.pos.y = g_pFrm->m_pRobotParam->m_structClean.m_posClean.y+g_pFrm->m_pSysParam->tOffset[0].y;
	bp.pos.z = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_structClean.m_posClean.z;
	bp.pos.a = g_pFrm->m_pRobotParam->m_structClean.m_posClean.u;

	bp.nPort     = inWashPort;                     // ��ϴ�ö˿�
	bp.dWaitTime = 0.1;                        
	bp.timers    = g_pFrm->m_pSysParam->iWashNum;  // ��ϴ����
	bp.type      = g_pFrm->m_pSysParam->iWashType;  // ��ϴ����
	bp.dis       = 5;          // ��ϴ�������� 
	short sTemp  = m_pController->TS_ListBath(CRD1,&dcp[m_CurToolNo],&bp);
	return sTemp;
}
short CCmdRun::RunStandBy(tgCmdLine *pCmd,double dOffsetX,double dOffsetY)
{//�ȴ���
	t_Base sdp;
	::ZeroMemory(&sdp,sizeof(t_Base));

	double dx,dy,dz,du;
	int iPosIndex = 0;
	dx = pCmd->pDblValue[iPosIndex*4+0];
	dy = pCmd->pDblValue[iPosIndex*4+1];
	dz = pCmd->pDblValue[iPosIndex*4+2];
	du = pCmd->pDblValue[iPosIndex*4+3];

	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}
	sdp.dVelXY = m_cmdParam.dVelXY;
	sdp.dVelZ  = m_cmdParam.dVelZ;
	g_pFrm->m_Precision.Measure2Real(dy);
	sdp.pos.x = dx;
	sdp.pos.y = dy;
	sdp.pos.z = dz;
	sdp.pos.a = du;

	short sTemp  = m_pController->TS_ListStandBy(CRD1,&dcp[m_CurToolNo],&sdp);//pCmd->pIntValue[0] ��ID
	return sTemp;
}
short CCmdRun::RunDummy(tgCmdLine *pCmd,double dOffsetX,double dOffsetY)
{
	t_Base ddp;
	::ZeroMemory(&ddp,sizeof(t_Base));

	double dx,dy,dz,du;
	int iPosIndex = 0;
	dx = pCmd->pDblValue[iPosIndex*4+0];
	dy = pCmd->pDblValue[iPosIndex*4+1];
	dz = pCmd->pDblValue[iPosIndex*4+2];
	du = pCmd->pDblValue[iPosIndex*4+3];

	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}
	ddp.dVelXY = m_cmdParam.dVelXY;
	ddp.dVelZ  = m_cmdParam.dVelZ;
	ddp.dWorkVel=m_cmdParam.dVelXY;
	g_pFrm->m_Precision.Measure2Real(dy);
	ddp.pos.x = dx;
	ddp.pos.y = dy;
	ddp.pos.z = dz;
	ddp.pos.a = du;

	short sTemp  = m_pController->TS_ListDummy(CRD1,&ddp);//pCmd->pIntValue[0] ��ID
	return sTemp;
}

short CCmdRun::RunPoint(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	UNREFERENCED_PARAMETER(pNextTrackCmd);

	t_Point pdp;
	::ZeroMemory(&pdp,sizeof(t_Point));
	CString strtemp;
	double dx,dy,dz,du;
	int iPosIndex = 0;
	dx = pCmd->pDblValue[iPosIndex*4+0];
	dy = pCmd->pDblValue[iPosIndex*4+1];
	dz = pCmd->pDblValue[iPosIndex*4+2];
	du = pCmd->pDblValue[iPosIndex*4+3];


	// 	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
	// 		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	// 	{
	if(GetCmdPositionEX(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
		return RTN_DATA_OVER;
	if (RunMode_Dry == m_runMode)
	{
		dx -= g_pFrm->m_pSysParam->tOffset[0].x;
		dy -= g_pFrm->m_pSysParam->tOffset[0].y;
	}
	// 	}else
	// 	{
	// 		if(GetCmdPosition(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
	// 			return RTN_DATA_OVER;
	// 	}

#ifdef  DEBUG_SD950
	strtemp.Format("point X:%.3f",dx);
	g_pFrm->AddMsg(strtemp);
	strtemp.Format("point y:%.3f",dy);
	g_pFrm->AddMsg(strtemp);
#endif

	double dDispCyc, dDispFreq;
	g_pFrm->m_comDisp[0].GetCycleTimeAndFreq(dDispCyc, dDispFreq);
	// ������Ƶ㽺��ʱ,�㽺���ĵ㽺Ƶ��
	pdp.dVelA = dDispFreq;
	pdp.dVelXY = m_cmdParam.dVelXY;
	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dZcleaType = m_cmdParam.dZcleaType;
	pdp.dZclearance = m_cmdParam.dZclearance;

	pdp.dZupHVel = m_cmdParam.tPoint.dZupHVel;
	pdp.dZupLVel = m_cmdParam.tPoint.dZupLVel;

	// 
	g_pFrm->m_Precision.Measure2Real(dy);

	pdp.pos.x = dx;
	pdp.pos.y = dy;
	pdp.pos.z = dz;

	pdp.dPreTime = m_cmdParam.tPoint.dPreTime;
	pdp.dZuphigh = m_cmdParam.tPoint.dZupHigh;
	pdp.dNodeTime = m_cmdParam.tPoint.dTailTime;

	//���ط�
	if(pCmd->pIntValue[1]>0 && (RunMode_Wet==m_runMode)) 
		pdp.bOn = TRUE;
	else 
		pdp.bOn = FALSE;

	//������ʱ��ʱ
	if(0 == pCmd->pIntValue[5])  
		pdp.dDispTime = m_cmdParam.tPoint.dDispTime;//s to ms
	else                         
		pdp.dDispTime = pCmd->pDblValue[5];//��ʱ��ʱʱ��

	dcp[m_CurToolNo].bBit = pdp.bOn;
	dcp[m_CurToolNo].lowTime = m_cmdParam.nLowTime;
	dcp[m_CurToolNo].highTime = m_cmdParam.nHighTime;
	dcp[m_CurToolNo].lPulseNum = m_cmdParam.lPulseNum;

	short sTemp  = m_pController->TS_ListPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] ��ID
	if (RTN_NONE != sTemp)
	{
		return sTemp;
	}

	// �����˶�
	sTemp=m_pController->TS_ListWaitStop();
	return sTemp;
}

// ������
short CCmdRun::RunFlyPoint_Dots(tgCmdLine *pCmd,double dOffsetX,double dOffsetY)
{
	if((m_tSysStatus==K_RUN_STS_STOP) || (g_bStop==TRUE && (K_RUN_STS_RUN_1_UNIT!=m_tSysStatus))) 
	return RTN_STP;

	t_HPoint pdp;
	::ZeroMemory(&pdp,sizeof(t_Point));
	CString strTemp;
	double dx,dy,dz,du;
	int iPosIndex = 0;
	dx = pCmd->pDblValue[iPosIndex*4+0];
	dy = pCmd->pDblValue[iPosIndex*4+1];
	dz = pCmd->pDblValue[iPosIndex*4+2];
	du = pCmd->pDblValue[iPosIndex*4+3];

	pdp.iDotNum = Round(du/g_pFrm->m_pRobotParam->m_dAvgWeight);

	if(GetCmdPositionEX(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
		return RTN_DATA_OVER;

	if (RunMode_Dry == m_runMode)
	{
		dz = g_pFrm->m_pRobotParam->m_zSafePos;
		dx -= g_pFrm->m_pSysParam->tOffset[0].x;
		dy -= g_pFrm->m_pSysParam->tOffset[0].y;
	}

	double dDispCyc, dDispFreq;
	g_pFrm->m_comDisp[0].GetCycleTimeAndFreq(dDispCyc, dDispFreq);
	pdp.dDispTime = pdp.iDotNum * dDispCyc;

	pdp.dVelXY = m_cmdParam.dVelXY;    
	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dAccXY = m_cmdParam.dAccXY;
	pdp.dAccZ  = m_cmdParam.dAccZ;

	// �������ֵ
	DOT_PARAM dotParam;
	::ZeroMemory(&dotParam,sizeof(DOT_PARAM));
	dotParam = g_pFrm->m_DotParam[(int)pCmd->pDblValue[PARAM_POS]];
	
//	m_tmLoad.TimerStart();
	//////////////////////////////////////////////////////////////////////////
	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, dx, dy, pdp.dVelXY, pdp.dAccXY);
	m_pController->AxisMove(Z_AXIS, dz, pdp.dVelZ, pdp.dAccZ);
// 	strTemp.Format("Z �ƶ�ʱ��:[%.3f]", m_tmLoad.TimerGetMs());
// 	g_pFrm->AddMsg(strTemp);
	//m_pController->TS_ListDelay(CRD1, 0.02);

//	CFunction::DelayExSec(0.025);
	CFunction::DelayExSec(dotParam.dfSettlingTime);
	DotDisp(pdp.iDotNum);
//	CFunction::DelayExSec(pdp.dDispTime/1000 + 0.01);
	CFunction::DelayExSec(dotParam.dfDwellSecs);
	//CFunction::DelayExSec(0.01);

	return m_pController->TS_ListInit(CRD1, 1);
	//////////////////////////////////////////////////////////////////////////

// 	short sTemp = m_pController->TS_ListHPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] ��ID
// 	if (RTN_NONE != sTemp)
// 	{
// 		return sTemp;
// 	}
// 
// 	// �����˶�
// 	sTemp=m_pController->TS_ListWaitStop();
// 	return sTemp;
}

// �ɽ����
short CCmdRun::RunFlyPoint_StartPos(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	// �����˶����ȴ��˶�ֹͣ
	short sTemp;
	CString strTemp;
	sTemp=m_pController->TS_ListWaitStop();

	// IO���Ʒ�ʽ����IO�źſ���,���źŹط�
	if (0 == g_pFrm->m_pRobotParam->m_nValveCtrlMode)
	{
		if (0 != g_pFrm->m_comDisp[0].m_dispParam.NumOfPul)
		{
			g_pFrm->m_comDisp[0].m_dispParam.NumOfPul = 0;
			if (1 != g_pFrm->m_comDisp[0].HandleParamDispenser(FALSE))
			{
				AfxMessageBox("���õ㽺���������������!");
				return -1;
			}
		}
	}
	else if (1 == g_pFrm->m_pRobotParam->m_nValveCtrlMode)
	{
		if (1 != g_pFrm->m_comDisp[0].m_dispParam.NumOfPul)
		{
			g_pFrm->m_comDisp[0].m_dispParam.NumOfPul = 1;
			if (1 != g_pFrm->m_comDisp[0].HandleParamDispenser(FALSE))
			{
				AfxMessageBox("���õ㽺���������������!");
				return -1;
			}
		}
	}
	else
	{
		strTemp.Format("ʹ�����䷧ʱ,δָ��������ģʽ:[%d]", g_pFrm->m_pRobotParam->m_nValveCtrlMode);
		AfxMessageBox(strTemp);
		return -1;
	}

	t_HPoint pdp; // ���ٳ�����
	::ZeroMemory(&pdp,sizeof(t_HPoint));

	CString strtemp;
	double dx,dy,dz,du, dPreLen;
	double dNxtx, dNxty, dNxtz, dNxtCmdLen;

	dx = pCmd->pDblValue[0];
	dy = pCmd->pDblValue[1];
	dz = pCmd->pDblValue[2];
	du = pCmd->pDblValue[3];
	dPreLen = pCmd->pDblValue[4];
	dNxtx = pNextTrackCmd->pDblValue[0];
	dNxty = pNextTrackCmd->pDblValue[1];
	dNxtz = pNextTrackCmd->pDblValue[2];
	du = pNextTrackCmd->pDblValue[3];
	dNxtCmdLen = pNextTrackCmd->pDblValue[4];

	// �߶γ���MM
	double dLineLenth = sqrt((dNxtx-dx)*(dNxtx-dx) + (dNxty-dy)*(dNxty-dy));

	if(GetCmdPositionEX(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
		return RTN_DATA_OVER;

	if(GetCmdPositionEX(pNextTrackCmd,&dNxtx,&dNxty,&dNxtz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
		return RTN_DATA_OVER;

	if (RunMode_Dry == m_runMode)
	{
		dx -= g_pFrm->m_pSysParam->tOffset[0].x;
		dy -= g_pFrm->m_pSysParam->tOffset[0].y;
		dNxtx -= g_pFrm->m_pSysParam->tOffset[0].x;
		dNxty -= g_pFrm->m_pSysParam->tOffset[0].x;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��׼����
	double dDotWt = g_pFrm->m_pRobotParam->m_dAvgWeight;
	// ����
	if (dDotWt < EPS)		pdp.iDotNum = Round(du/0.5);
	else					pdp.iDotNum = Round(du/dDotWt);
			
	pdp.pos.x = dx + (dx-dNxtx)*(dPreLen)/dLineLenth;
	pdp.pos.y = dy + (dy-dNxty)*(dPreLen)/dLineLenth;
	pdp.pos.z = dz;

	CARTESIAN_POINT ptEnd;
	ptEnd.x = dNxtx + (dNxtx-dx)*(dNxtCmdLen)/dLineLenth;
	ptEnd.y = dNxty + (dNxty-dy)*(dNxtCmdLen)/dLineLenth;


	double dDispCyc, dDispFreq;
	g_pFrm->m_comDisp[0].GetCycleTimeAndFreq(dDispCyc, dDispFreq);

	dLineLenth += dPreLen-dNxtCmdLen;	// �㽺���ȼ���������ĳ���

	pdp.dVelXY = dLineLenth*dDispFreq/pdp.iDotNum; 
//	pdp.dVelXY = m_cmdParam.dVelXY;    
	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dAccXY = m_cmdParam.dAccXY;
	pdp.dAccZ  = m_cmdParam.dAccZ;

	// ���ٵ����ٶεľ���
	double dAccLen = pdp.dVelXY*pdp.dVelXY / m_cmdParam.dAccXY / 2000.0;

	// ����IO����,���㷢�����������
	if (1 == g_pFrm->m_pRobotParam->m_nValveCtrlMode)
	{
		if (0 == pdp.iDotNum)
		{
			pdp.dVelXY = m_cmdParam.dVelXY;
			pdp.dCompareStep = dLineLenth*2;
			pdp.iCompareAxis = 1;
			pdp.dCompareStart = dx + (dx-dNxtx)*(dPreLen)/dLineLenth;

			//////////////////////////////////////////////////////////////////////////
			pdp.pComparePos = new long;
			pdp.pComparePos[0] = long(pdp.dCompareStart + pdp.dCompareStep)*1000;
		}
		else	
		{
			pdp.dDispTime = g_pFrm->m_comDisp[0].m_dispParam.OpenTime; 
			pdp.dVelXY = dLineLenth*dDispFreq/pdp.iDotNum; 
			pdp.pComparePos = new long[pdp.iDotNum];

			if (pdp.dVelXY > Vel_Limit)
			{
				pdp.dVelXY = Vel_Limit;
			}
			//		double dAccLen = 1.333*pdp.dVelXY*pdp.dVelXY / m_cmdParam.dAccXY / 2000.0;

			BOOL bForword = TRUE;	// �Ƿ���������㽺
			double dAng = abs(dx-dNxtx) - abs(dy-dNxty);	// �߶�ƫ��X�����Y��
			if (dAng > 0)	// ƫ��X���߶�
			{
				pdp.iCompareAxis = 1;
				pdp.dCompareStep = (ptEnd.x-pdp.pos.x)/(pdp.iDotNum);
				pdp.dCompareStart = pdp.dCompareStep/2;
			}
			else
			{
				pdp.iCompareAxis = 2;
				pdp.dCompareStep = (ptEnd.y-pdp.pos.y)/(pdp.iDotNum);
				pdp.dCompareStart = pdp.dCompareStep/2;
			}	
			if (pdp.dCompareStep > 0)
				bForword = TRUE;
			else
				bForword = FALSE;

			pdp.pComparePos[0] = (long)(pdp.dCompareStart*1000);
			for (int i=1; i<pdp.iDotNum; i++)
			{
				pdp.pComparePos[i] = long(pdp.pComparePos[0] + pdp.dCompareStep*i*1000);
			}

			//		int nAccDotNums = 0;
			/*		if (dAccLen*2 > dLineLenth)
			{
			nAccDotNums = pdp.iDotNum / 2;
			}
			else
			{
			nAccDotNums = Round(abs((dAccLen+6*pdp.dVelXY/1000) / pdp.dCompareStep));
			nDecDotNums = Round(abs((dAccLen-6*pdp.dVelXY/1000) / pdp.dCompareStep));
			}

			double dRelativeDispPos;// ����λ����Ծ��ȷֲ��Ľ���λ�����λ��
			double dTemp1 = 0.75*6*6*2*pdp.dAccXY/1000; 
			double dTemp2 = 0;
			for (int i=0; i<nAccDotNums; i++)
			{
			if (bForword)	// ��������㽺
			{
			dTemp2 = sqrt(dTemp1*dTemp1+4*dTemp1*(pdp.dCompareStart+i*pdp.dCompareStep));
			dRelativeDispPos = (dTemp2 - dTemp1) / 2;
			pdp.pComparePos[i] = long((pdp.dCompareStart + i*pdp.dCompareStep - dRelativeDispPos) * 1000);
			}
			else	// ��������㽺
			{
			dTemp2 = sqrt(dTemp1*dTemp1-4*dTemp1*(pdp.dCompareStart+i*pdp.dCompareStep));
			dRelativeDispPos = (dTemp2 - dTemp1) / 2;
			pdp.pComparePos[i] = long((pdp.dCompareStart + i*pdp.dCompareStep + dRelativeDispPos) * 1000);
			}
			}
			for (int i=nAccDotNums; i<pdp.iDotNum-nDecDotNums; i++)
			{
			if (bForword)
			{
			pdp.pComparePos[i] = long((pdp.dCompareStart + i*pdp.dCompareStep-pdp.dVelXY*6/1000)*1000);
			}
			else
			{
			pdp.pComparePos[i] = long((pdp.dCompareStart + i*pdp.dCompareStep+pdp.dVelXY*6/1000)*1000);
			}
			}
			for (int i=pdp.iDotNum-nDecDotNums; i<pdp.iDotNum; i++)
			{
			if (bForword)
			{
			dTemp2 = sqrt(dTemp1*dTemp1+4*dTemp1*(pdp.dCompareStart+(pdp.iDotNum-i)*pdp.dCompareStep));
			dRelativeDispPos = (dTemp2 - dTemp1) / 2;
			pdp.pComparePos[i] = long((pdp.dCompareStart + i*pdp.dCompareStep - dRelativeDispPos) * 1000);
			}
			else
			{
			dTemp2 = sqrt(dTemp1*dTemp1-4*dTemp1*(pdp.dCompareStart+(pdp.iDotNum-i)*pdp.dCompareStep));
			dRelativeDispPos = (dTemp2 - dTemp1) / 2;
			pdp.pComparePos[i] = long((pdp.dCompareStart + i*pdp.dCompareStep + dRelativeDispPos) * 1000);
			}
			}

			if (bForword)
			{
			if (0 == pdp.pComparePos[0])
			{
			pdp.pComparePos[0] = 1;
			}
			}
			else
			{
			if (0 == pdp.pComparePos[0])
			{
			pdp.pComparePos[0] = -1;
			}
			} */
		}
	}

	dAccLen *= 1.5;

	// �Խ�λ�õ� 
	pdp.m_assistPos.x = dx + (dx-dNxtx)*(dAccLen+dPreLen)/dLineLenth;
	pdp.m_assistPos.y = dy + (dy-dNxty)*(dAccLen+dPreLen)/dLineLenth;
	pdp.m_assistPos.z = dz;
	pdp.m_assistPos.a = du;

#ifdef  DEBUG_SD950
	strtemp.Format("point X:%.3f",dx);
	g_pFrm->AddMsg(strtemp);
	strtemp.Format("point y:%.3f",dy);
	g_pFrm->AddMsg(strtemp);
#endif
	
	pdp.dVelXY = m_cmdParam.dVelXY;    
	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dAccXY = m_cmdParam.dAccXY;
	pdp.dAccZ  = m_cmdParam.dAccZ;

	dcp[m_CurToolNo].outBit[0] = ExOutputBitNameToNo(_T("1�ŵ��̬����"));

	//����˶�ָ��
	sTemp  = m_pController->TS_ListHStartPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] ��ID
	strtemp.Format("������㣺x[%.3f], y[%.3f]", pdp.pos.x, pdp.pos.y);
	g_pFrm->AddMsg(strtemp);

	if (NULL != pdp.pComparePos)  
	{
		delete []pdp.pComparePos;
		pdp.pComparePos = NULL;
	}

	return sTemp;
}
// �ɽ������
short CCmdRun::RunFlyPoint_MidPos(tgCmdLine *pPreTrackCmd, tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	UNREFERENCED_PARAMETER(pNextTrackCmd);
	if (CMD_LINE_ARC == pPreTrackCmd->tType)	return RTN_NONE;

	t_HPoint pdp; // ���ٳ�����
	::ZeroMemory(&pdp,sizeof(t_HPoint));

	CString strtemp;
	double dx,dy,dz,du;
	double dxx,dyy;// λ��΢��

	dx = pCmd->pDblValue[0];
	dy = pCmd->pDblValue[1];
	dz = pCmd->pDblValue[2];
	du = pCmd->pDblValue[3];

	double dDotWt = g_pFrm->m_pRobotParam->m_dAvgWeight;
	if (dDotWt <EPS)
	{
		pdp.iDotNum = int(du/0.5);
	}
	else
	{
		pdp.iDotNum = int(du/dDotWt);
	}
	double dPrex = pPreTrackCmd->pDblValue[0];
	double dPrey = pPreTrackCmd->pDblValue[1];
	double dLineLenth = sqrt((dPrex-dx)*(dPrex-dx) + (dPrey-dy)*(dPrey-dy));
	double dDispCyc, dDispFreq;
	g_pFrm->m_comDisp[0].GetCycleTimeAndFreq(dDispCyc, dDispFreq);

	if (0 == pdp.iDotNum)		pdp.dVelXY = m_cmdParam.dVelXY;
	else						pdp.dVelXY = dLineLenth*dDispFreq/pdp.iDotNum;  
	CString strTemp;
	strTemp.Format("�������ٶ�:[%.3f]", pdp.dVelXY);
	g_pFrm->AddMsg(strTemp);
	//    
	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dAccXY = m_cmdParam.dAccXY;
	pdp.dAccZ  = m_cmdParam.dAccZ;

	dxx = pCmd->pDblValue[6];
	dyy = pCmd->pDblValue[7];

	// λ��΢��
	dx +=dxx;
	dy +=dyy; 

	// 	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
	// 		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	// 	{
	if(GetCmdPositionEX(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
		return RTN_DATA_OVER;
	if (RunMode_Dry == m_runMode)
	{
		dx -= g_pFrm->m_pSysParam->tOffset[0].x;
		dy -= g_pFrm->m_pSysParam->tOffset[0].y;
	}
	// 	}else
	// 	{
	// 		if(GetCmdPosition(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
	// 			return RTN_DATA_OVER;
	// 	}
#ifdef  DEBUG_SD950
	strtemp.Format("point X:%.3f",dx);
	g_pFrm->AddMsg(strtemp);
	strtemp.Format("point y:%.3f",dy);
	g_pFrm->AddMsg(strtemp);
#endif

	pdp.pos.x = dx;
	pdp.pos.y = dy;
	pdp.pos.z = dz;
	pdp.pos.a = du;

	//����˶�ָ��
	short sTemp  = m_pController->TS_ListHWorkPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] ��ID

	return sTemp;
}
// �����ٶȼ���ƫ����
double CCmdRun::CalcVelFlyOffset(double dvel,double djetdelay)  
{
	double temp;
	//  mm/s

	djetdelay  = djetdelay/1000.;// ms -> s 

	temp = dvel*djetdelay ;
	return temp ; // mm 
}
// �ɽ��յ�
short CCmdRun::RunFlyPoint_EndPos(tgCmdLine *pPreTrackCmd, tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{	
	UNREFERENCED_PARAMETER(pNextTrackCmd);
	if (CMD_SUPEREND == pPreTrackCmd->tType)	return RTN_NONE;

	t_HPoint pdp; // ���ٳ�����
	::ZeroMemory(&pdp,sizeof(t_HPoint));	

	CString strtemp;
	double dPrex, dPrey, dPrez, dPreCmdLen;
	double dx,dy,dz,du, dPreLen;

	dx = pCmd->pDblValue[0];
	dy = pCmd->pDblValue[1];
	dz = pCmd->pDblValue[2];
	du = pCmd->pDblValue[3];
	dPreLen = pCmd->pDblValue[4];

	double dDotWt = g_pFrm->m_pRobotParam->m_dAvgWeight;
	if (dDotWt < EPS)
	{
		pdp.iDotNum = Round(du/0.5);
	}
	else
	{
		pdp.iDotNum = Round(du/dDotWt);
	}
	dPrex = pPreTrackCmd->pDblValue[0];
	dPrey = pPreTrackCmd->pDblValue[1];
	dPrez = pPreTrackCmd->pDblValue[2];
	dPreCmdLen = pPreTrackCmd->pDblValue[4];
	double dLineLenth = sqrt((dPrex-dx)*(dPrex-dx) + (dPrey-dy)*(dPrey-dy));
	double dDispCyc, dDispFreq;
	g_pFrm->m_comDisp[0].GetCycleTimeAndFreq(dDispCyc, dDispFreq);

	dLineLenth += dPreCmdLen-dPreLen;
	CString strTemp;
	if (0 == pdp.iDotNum)		pdp.dVelXY = m_cmdParam.dVelXY;
	else						pdp.dVelXY = dLineLenth*dDispFreq/pdp.iDotNum; 

//	pdp.dVelXY /= 1.2;
	if (pdp.dVelXY > Vel_Limit)	// ����500
	{
		pdp.dVelXY = Vel_Limit;///= 1.2;
	}
	strTemp.Format("�������ٶ�:[%.3f]", pdp.dVelXY);
	g_pFrm->AddMsg(strTemp); 

//	pdp.dVelXY /= 1.5;

	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dAccXY = m_cmdParam.dAccXY;
	pdp.dAccZ  = m_cmdParam.dAccZ;

	if(GetCmdPositionEX(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
		return RTN_DATA_OVER;
	if(GetCmdPositionEX(pPreTrackCmd,&dPrex,&dPrey,&dPrez,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
		return RTN_DATA_OVER;

	if (RunMode_Dry == m_runMode)
	{
		dx -= g_pFrm->m_pSysParam->tOffset[0].x;
		dy -= g_pFrm->m_pSysParam->tOffset[0].y;
		dPrex -= g_pFrm->m_pSysParam->tOffset[0].x;
		dPrey -= g_pFrm->m_pSysParam->tOffset[0].x;
	}

	// ���ٵ����ٶεľ���
	double dAccLen = pdp.dVelXY*pdp.dVelXY / m_cmdParam.dAccXY / 2000.0;
	dAccLen *= 1.5;

	// �Խ�λ�õ� 
	pdp.m_assistPos.x = dx + (dx-dPrex)*(dAccLen-dPreLen)/dLineLenth;
	pdp.m_assistPos.y = dy + (dy-dPrey)*(dAccLen-dPreLen)/dLineLenth;
	pdp.m_assistPos.z = dz;
	pdp.m_assistPos.a = du;

	pdp.pos.x = dx + (dPrex - dx)*(dPreLen)/dLineLenth;
	pdp.pos.y = dy + (dPrey - dy)*(dPreLen)/dLineLenth;
	pdp.pos.z = dz;

	// ��� �߲���
	LINE_PARAM lineParam;
	::ZeroMemory(&lineParam,sizeof(LINE_PARAM));
	lineParam = g_pFrm->m_LineParm[(int)pCmd->pDblValue[PARAM_POS]];

	//����˶�ָ��
	short sTemp  = m_pController->TS_ListHEndPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] ��ID
	strtemp.Format("�����յ㣺x[%.3f], y[%.3f]", pdp.pos.x, pdp.pos.y);
	g_pFrm->AddMsg(strtemp);

	// �����˶�
	sTemp=m_pController->TS_ListWaitStop();

	return sTemp;
}
// ��ȡģ������������
BOOL CCmdRun::GetLaserData(unsigned short &date,int ntime,int nDelay)
{
	////////////////////
	unsigned short pTempValue=0;

	if(ntime>10)   ntime =10;
	if(ntime<0)    ntime = 1;	

	unsigned short db[10];
	for(int i=0;i<ntime;i++)
	{
		g_pFrm->m_Robot->m_pController->GetExtADDAVal(0,(short)g_pFrm->m_pSysParam->nLaserReadPortID,&db[i]);
		CFunction::DelaySec(nDelay); 
		pTempValue += db[i];  // �ۼ�����
	}

	// ȡ��ȡ����ƽ��ֵ
	date = (unsigned short)(pTempValue / ntime) ; 

	date = (unsigned short)((-10.0+(date/65535.0)*20.0)*2); 
	// ������Ҫ���� ���̵Ĳ������Ż����� 

	return TRUE;
}
short CCmdRun::RunZHeight(tgCmdLine *pCmd,double dOffsetX,double dOffsetY,double *dValue)
{
	short sTemp = RTN_NONE;
	CString strTemp;

	t_Point pdp;
	double ZHeightVal;
	::ZeroMemory(&pdp,sizeof(t_Point));

	static int nPushIndex=-1;   

	double dx,dy,dz,du;
	int iPosIndex = 0;
	dx = pCmd->pDblValue[iPosIndex*4+0];
	dy = pCmd->pDblValue[iPosIndex*4+1];
	dz = pCmd->pDblValue[iPosIndex*4+2];
	du = pCmd->pDblValue[iPosIndex*4+3];

	double dStdHeightVal = pCmd->pDblValue[iPosIndex*4+4];
	pdp.dDispTime = pCmd->pDblValue[5];

	//GetZCheckCmdPosition(dx,dy,&dx,&dy,dOffsetX,dOffsetY);

	if(GetCmdPositionEX(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY, 0)==FALSE) 
		return RTN_DATA_OVER;

	pdp.dVelXY = m_cmdParam.dVelXY;
	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dZcleaType = m_cmdParam.dZcleaType;
	pdp.dZclearance = m_cmdParam.dZclearance;

	pdp.dZupHVel = m_cmdParam.tPoint.dZupHVel;
	pdp.dZupLVel = m_cmdParam.tPoint.dZupLVel;

	double doffsetCameraLaserX=0;
	double doffsetCameraLaserY=0;

	if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
	{
		doffsetCameraLaserX = g_pFrm->m_pRobotParam->m_posLaserCross.x - g_pFrm->m_pRobotParam->m_posCamereCross.x;
		doffsetCameraLaserY = g_pFrm->m_pRobotParam->m_posLaserCross.y - g_pFrm->m_pRobotParam->m_posCamereCross.y;
	}
	else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
	{
		doffsetCameraLaserX = g_pFrm->m_pRobotParam->m_posProbeCross.x - g_pFrm->m_pRobotParam->m_posCamereCross.x;
		doffsetCameraLaserY = g_pFrm->m_pRobotParam->m_posProbeCross.y - g_pFrm->m_pRobotParam->m_posCamereCross.y;
	}
	g_pFrm->m_Precision.Measure2Real(dy);
	pdp.pos.x = dx+doffsetCameraLaserX;
	pdp.pos.y = dy+doffsetCameraLaserY;
	pdp.pos.z = dz;
	pdp.pos.a = du;

	if (pdp.dDispTime<0.3)
	{
		pdp.dDispTime=0.3;
	}

	pdp.dZuphigh = m_cmdParam.tPoint.dZupHigh;
	sTemp = m_pController->TS_ListZHMeter(CRD1,&dcp[m_CurToolNo],&pdp);

	///////////////
	sTemp=m_pController->TS_ListWaitStop();
	if(sTemp!=RTN_NONE) return sTemp;

	double dHt = 0;

	if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
	{
		CFunction::DelaySec(pdp.dDispTime);
		if (!g_pFrm->m_comLaser.ReadHeightData(dHt))
		{
			g_pFrm->AddMsg(_T("��ȡ���������ݳ���!"));
			return RTN_ZHeight_Err;
		}
		//////////////////////////////////////////////////////////////////////////
		*dValue = dHt;
		strTemp.Format("�����߶���:[%.3f]", dHt);
		//g_pFrm->AddMsg(strTemp);
		CFunction::WriteLog(__FILE__, __LINE__, strTemp, CFunction::LEV_INFO);

		ZHeightVal = dHt-dStdHeightVal;
		if (abs(ZHeightVal) > 0.5)
		{
			AfxMessageBox(_T("�����ø߶�ƫ���0.5mm!"));
			g_pFrm->AddMsg(_T("������ƫ���0.5mm!"));
			return RTN_ZHeight_Err;
		}

		ZHeightVal = dHt - g_pFrm->m_pRobotParam->m_zLaserBaseVal;
		m_dFirstHeightOffset = ZHeightVal;   // �߶�ƫ���

		strTemp.Format("�����߶���:[%.3f]", dHt);
		g_pFrm->AddMsg(strTemp);
	}
	else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
	{
		//////////////////////////////////////////////////////////////////////////
		g_pFrm->m_Robot->ExOutput(_T("̽�����׵�ŷ�"), TRUE);
		CFunction::DelaySec(1);
		double dPos = dStdHeightVal - g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLHeight;
		sTemp = m_pController->AxisMove(Z_AXIS, dPos, g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeHSpd, 10);
		dPos = g_pFrm->m_pRobotParam->m_zContactPos-5;	// ̽���߼���λ��
		sTemp = m_pController->AxisProbeCheck(Z_AXIS, dPos, g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLSpd, 10, &dHt);
		g_pFrm->m_Robot->ExOutput(_T("̽�����׵�ŷ�"), FALSE);
		GotoSafeZpos();
		CFunction::DelaySec(0.2);

		//////////////////////////////////////////////////////////////////////////
		ZHeightVal = dHt-dStdHeightVal;
		if (abs(ZHeightVal) > 0.5)
		{
			AfxMessageBox(_T("̽���ø߶�ƫ���0.5mm!"));
			g_pFrm->AddMsg(_T("̽����ƫ���0.5mm!"));
			return RTN_ZHeight_Err;
		}
		*dValue = dHt;

		ZHeightVal = dHt - g_pFrm->m_pRobotParam->m_zProbeBaseVal;
		m_dFirstHeightOffset = ZHeightVal;   // �߶�ƫ���

		strTemp.Format("̽���߶���:[%.3f]", dHt);
		g_pFrm->AddMsg(strTemp);
		CFunction::WriteLog(__FILE__, __LINE__, strTemp, CFunction::LEV_INFO);
	}

	return m_pController->TS_ListInit(CRD1,1);



	//////////////////////////////////////////////////////////////////////////
	////////////////////
// 	unsigned short pValue=0;
// 
// 	g_pFrm->m_Robot->m_pController->GetExtADDAVal(0,(short)g_pFrm->m_pSysParam->nLaserReadPortID,&pValue);
// 
// 	ZHeightVal = (-5.0+(pValue/65535.0)*10.0)*2-pCmd->pDblValue[4];
// 
// 	CString Strtemp;
// 	Strtemp.Format("Z�ἤ���߲�: %.3f", ZHeightVal);
// 	g_pFrm->AddMsg(Strtemp);
// 
// 	/////////////////////
// 	tgZMeter tZMeterTemp;
// 
// 	if (m_ZMeterVector.empty())
// 	{   // ������ 
// 		*dValue = ZHeightVal;                // ���ظ߶�ֵ
// 		m_dFirstHeightOffset = ZHeightVal;   // �߶�ƫ���
// 		nPushIndex = 0;                          
// 	}
// 	else
// 	{
// 		nPushIndex++;    
// 		*dValue = 0;
// 	}
// 
// 	tZMeterTemp.index = nPushIndex;
// 	tZMeterTemp.delta = ZHeightVal;
// 	m_ZMeterVector.push_back(tZMeterTemp);  // ����ѹ������
// 
// 	sTemp=m_pController->TS_ListInit(CRD1,1);
// 	if(sTemp!=RTN_NONE) return sTemp;
// 
// 	return sTemp;
}

short CCmdRun::RunLineStart(tgCmdLine *pCmd1,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	t_LineStart lstdp;
	::ZeroMemory(&lstdp,sizeof(t_LineStart));

	lstdp.dVelXY=m_cmdParam.dVelXY;
	lstdp.dVelZ=m_cmdParam.dVelZ;

	double dx,dy,dz,du;
	int iPosIndex = 0;

	dx = pCmd1->pDblValue[iPosIndex*4+0];
	dy = pCmd1->pDblValue[iPosIndex*4+1];
	dz = pCmd1->pDblValue[iPosIndex*4+2];
	du = pCmd1->pDblValue[iPosIndex*4+3];

// 	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
// 		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
// 	{
		if(GetCmdPositionEX(pCmd1,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
		if (RunMode_Dry == m_runMode)
		{
			dx -= g_pFrm->m_pSysParam->tOffset[0].x;
			dy -= g_pFrm->m_pSysParam->tOffset[0].y;
		}
// 	}else
// 	{
// 		if(GetCmdPosition(pCmd1,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
// 			return RTN_DATA_OVER;
// 	}

	//���ط�
	if(pCmd1->pIntValue[1]>0 && (RunMode_Wet==m_runMode)) 
		lstdp.bOn = TRUE;
	else 
		lstdp.bOn = FALSE;

	//������ʱ��ʱ
	if(0 == pCmd1->pIntValue[5]) 
		lstdp.dPreTime=m_cmdParam.tLine.dPreTime;//s to ms
	else	
		lstdp.dPreTime=pCmd1->pDblValue[5];//��ʱ��ʱʱ��

	g_pFrm->m_Precision.Measure2Real(dy);

	lstdp.pos.x = dx;
	lstdp.pos.y = dy;
	lstdp.pos.z = dz;
	lstdp.pos.a = du;
	lstdp.dWorkVel = m_cmdParam.tLine.dVel;

	//dcp.bBitEnable	= TRUE;

// 	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
// 		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
// 	{
		if(GetCmdPositionEX(pNextTrackCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
		if (RunMode_Dry == m_runMode)
		{
			dx -= g_pFrm->m_pSysParam->tOffset[0].x;
			dy -= g_pFrm->m_pSysParam->tOffset[0].y;
		}
// 	}else
// 	{
// 		if(GetCmdPosition(pNextTrackCmd,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
// 			return RTN_DATA_OVER;
// 	}
	lstdp.nextCmdType = TranslateType(pNextTrackCmd->tType);
	lstdp.nextTrackPoint.pos.x = dx;
	lstdp.nextTrackPoint.pos.y = dy;
	lstdp.nextTrackPoint.pos.z = dz;
	lstdp.nextTrackPoint.pos.a = pNextTrackCmd->pDblValue[3];

	//m_CurToolNo = pCmd1->pIntValue[0];

	dcp[m_CurToolNo].bBit = lstdp.bOn;

	dcp[m_CurToolNo].lowTime = m_cmdParam.nLowTime;
	dcp[m_CurToolNo].highTime = m_cmdParam.nHighTime;
	dcp[m_CurToolNo].lPulseNum = m_cmdParam.lPulseNum;

	m_pController->TS_ListWaitStop();
	return m_pController->TS_ListLineStart(CRD1,&dcp[m_CurToolNo],&lstdp);//pCmd->pIntValue[0] ��ID
}

short CCmdRun::RunLinePass(tgCmdLine *pPreTrackCmd,tgCmdLine *pCmd1,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	t_LinePass lpdp;

	double xb,yb,zb;
	double dx,dy,dz,du;
	int iPosIndex = 0;

	//���ػ��� ʱ�˳���ָ��ִ��
	if (pNextTrackCmd->tType==CMD_WCL)
	{
		RunWCL_addSTrack=false;  // �����ӿ�ʼ��
		return RTN_NONE;

	}
	//���ؾ��� ʱ�˳���ָ��ִ��
	if (pNextTrackCmd->tType==CMD_WCRECT)
	{
		RunWCL_addSTrack=false;  // �����ӿ�ʼ��
		return RTN_NONE;
	}

	dx = pCmd1->pDblValue[iPosIndex*4+0];
	dy = pCmd1->pDblValue[iPosIndex*4+1];
	dz = pCmd1->pDblValue[iPosIndex*4+2];
	du = pCmd1->pDblValue[iPosIndex*4+3];

// 	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
// 		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
// 	{
		if(GetCmdPositionEX(pCmd1,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
		if(GetCmdPositionEX(pPreTrackCmd,&xb,&yb,&zb,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
		if (RunMode_Dry == m_runMode)
		{
			dx -= g_pFrm->m_pSysParam->tOffset[0].x;
			dy -= g_pFrm->m_pSysParam->tOffset[0].y;
			xb -= g_pFrm->m_pSysParam->tOffset[0].x;
			yb -= g_pFrm->m_pSysParam->tOffset[0].y;
		}
// 	}else
// 	{
// 		if(GetCmdPosition(pCmd1,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
// 			return RTN_DATA_OVER;
// 		if(GetCmdPosition(pPreTrackCmd,&xb,&yb,&zb,dOffsetX,dOffsetY)==FALSE) 
// 			return RTN_DATA_OVER;
// 	}

	lpdp.preCmdType = TranslateType(pPreTrackCmd->tType);
	lpdp.preTrackPoint.pos.x = xb;
	lpdp.preTrackPoint.pos.y = yb;
	lpdp.preTrackPoint.pos.z = zb;
	lpdp.preTrackPoint.pos.a = pPreTrackCmd->pDblValue[3];

	//nextTrackpoint
// 	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
// 		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
// 	{
		if(GetCmdPositionEX(pNextTrackCmd,&xb,&yb,&zb,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
		if (RunMode_Dry == m_runMode)
		{
			xb -= g_pFrm->m_pSysParam->tOffset[0].x;
			yb -= g_pFrm->m_pSysParam->tOffset[0].y;
		}
// 	}else
// 	{
// 		if(GetCmdPosition(pNextTrackCmd,&xb,&yb,&zb,dOffsetX,dOffsetY)==FALSE) 
// 			return RTN_DATA_OVER;
// 	}
	lpdp.nextCmdType = TranslateType(pNextTrackCmd->tType);
	lpdp.nextTrackPoint.pos.x = xb;
	lpdp.nextTrackPoint.pos.y = yb;
	lpdp.nextTrackPoint.pos.z = zb;
	lpdp.nextTrackPoint.pos.a = pNextTrackCmd->pDblValue[3];

	//������ʱ�ٶ�
	if(0 != pCmd1->pIntValue[4])
		lpdp.dWorkVel = pCmd1->pDblValue[4];
	else
		lpdp.dWorkVel = m_cmdParam.tLine.dVel;

	//������ʱ��ʱ
	if(0 == pCmd1->pIntValue[5])
		lpdp.dNodeTime=m_cmdParam.tLine.dNodeTime;
	else                         
		lpdp.dNodeTime=pCmd1->pDblValue[5];

	//���ط�
	if(pCmd1->pIntValue[1]>0 && RunMode_Wet==m_runMode) 
		lpdp.bOn = TRUE;
	else 
		lpdp.bOn = FALSE;

	g_pFrm->m_Precision.Measure2Real(dy);

	lpdp.pos.x = dx;
	lpdp.pos.y = dy;
	lpdp.pos.z = dz;
	lpdp.pos.a = du;

	//���ǰ뾶
	if(pCmd1->pIntValue[6] == FALSE)  
		lpdp.dRound = 0;
	else                         
		lpdp.dRound = pCmd1->pDblValue[6];

	m_pController->TS_ListLinePassing(CRD1,&dcp[m_CurToolNo],&lpdp,TRUE);

	return RTN_NONE;
}

short CCmdRun::RunLineArc(tgCmdLine *pPreTrackCmd,tgCmdLine *pCmd1,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	tgArc tArc;
	::ZeroMemory(&tArc,sizeof(tArc));

	if (pPreTrackCmd->tType!=CMD_LINE_ARC && pPreTrackCmd->tType!=CMD_LINE_START&&
		pPreTrackCmd->tType!=CMD_LINE_PASSING && pPreTrackCmd->tType!=CMD_SUPERPOINT&&
		pNextTrackCmd->tType!=CMD_SUPEREND && pPreTrackCmd->tType != CMD_SUPERSTART)
		return RTN_PARAMETER_ERR;
	if (pNextTrackCmd->tType!=CMD_LINE_ARC&&pNextTrackCmd->tType!=CMD_LINE_END&&
		pNextTrackCmd->tType!=CMD_LINE_PASSING && pNextTrackCmd->tType!=CMD_SUPEREND && 
		pPreTrackCmd->tType!=CMD_SUPERPOINT && pPreTrackCmd->tType != CMD_SUPERSTART)
		return RTN_PARAMETER_ERR;

	if(!g_pFrm->m_wndEditBar.GetCmdArcInfoEx(pPreTrackCmd, pCmd1,pNextTrackCmd,&tArc))//���Բ���Ƿ�Ϸ�
		return 0;

	double dx,dy,dz,du;
	int iPosIndex = 0;
	dx = pCmd1->pDblValue[iPosIndex*4+0];
	dy = pCmd1->pDblValue[iPosIndex*4+1];
	dz = pCmd1->pDblValue[iPosIndex*4+2];
	du = pCmd1->pDblValue[iPosIndex*4+3];

	// 	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
	// 		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	// 	{
	if(GetCmdPositionEX(pCmd1,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
		return RTN_DATA_OVER;
	// 	}else
	// 	{
	// 		if(GetCmdPosition(pCmd1,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
	// 			return RTN_DATA_OVER;
	// 	}
	t_Arc arcdp;
	::ZeroMemory(&arcdp,sizeof(arcdp));
	arcdp.pos.x		= dx;
	arcdp.pos.y		= dy;
	arcdp.pos.z		= dz;
	arcdp.pos.a		= du;
	arcdp.dVelXY	= 200;
	//	arcdp.dVelXY	= m_cmdParam.dVelXY;
	arcdp.dVelZ		= m_cmdParam.dVelZ;

	t_Base preTrackPoint;
	if(GetCmdPositionEX(pPreTrackCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
		return RTN_DATA_OVER;
	preTrackPoint.pos.x = dx;//pPreTrackCmd->pDblValue[0];//m_pController->g_ExMtnPara->dfBakpos[X_AXIS];
	preTrackPoint.pos.y = dy;//pPreTrackCmd->pDblValue[1];//m_pController->g_ExMtnPara->dfBakpos[Y_AXIS];
	preTrackPoint.pos.z = dz;//pPreTrackCmd->pDblValue[2];//m_pController->g_ExMtnPara->dfBakpos[Z_AXIS];
	if(MAX_AXIS_NUM==4)
		preTrackPoint.pos.a = pPreTrackCmd->pDblValue[3];//m_pController->g_ExMtnPara->dfBakpos[A_AXIS];
	else
		preTrackPoint.pos.a = 0;

	double dDotWt = g_pFrm->m_pRobotParam->m_dAvgWeight;
	if (dDotWt < EPS)	arcdp.iDotNum = int(du/0.5);
	else				arcdp.iDotNum = int(du/dDotWt);

	arcdp.preTrackPoint = &preTrackPoint;


	if(pCmd1->pIntValue[1]>0) 
		arcdp.bOn = TRUE;//���ط�
	else 
		arcdp.bOn = FALSE;

	if(0 != pCmd1->pIntValue[4])//������ʱ�ٶ�
		arcdp.dWorkVel = pCmd1->pDblValue[4];
	else
		arcdp.dWorkVel = m_cmdParam.tLine.dVel;

	if(0 == pCmd1->pIntValue[5])//������ʱʱ��
		arcdp.dNodeTime=m_cmdParam.tLine.dNodeTime;
	else                         
		arcdp.dNodeTime=pCmd1->pDblValue[5];

	t_Base lineNextp;
	::ZeroMemory(&lineNextp,sizeof(lineNextp));

	// 	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
	// 		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	// 	{
	if(GetCmdPositionEX(pNextTrackCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
		return RTN_DATA_OVER;
	// 	}else
	// 	{
	// 		if(GetCmdPosition(pNextTrackCmd,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
	// 			return RTN_DATA_OVER;
	// 	}
	lineNextp.pos.x = dx;
	lineNextp.pos.y = dy;
	lineNextp.pos.z = dz;
	lineNextp.pos.a = pNextTrackCmd->pDblValue[3];
	lineNextp.dWorkVel = m_cmdParam.tLine.dVel;
	arcdp.nextTrackPoint = (t_Base*)(&lineNextp);

	short sTemp=m_pController->TS_ListArc(CRD1,&dcp[m_CurToolNo],&arcdp,TRUE);//����ķ�������Կ��ƵĻ�����Ҫ��ȡǰ�浱ǰ����ID
	return sTemp;
}
short CCmdRun::RunCircle(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	UNREFERENCED_PARAMETER(pNextTrackCmd);

	//double xe=0.0, ye=0.0;
	//double dXc, dYc, dRc;
	double  dx, dy;
	//tgCircle tCircle;
	t_Circle circledp;
	::ZeroMemory(&circledp,sizeof(circledp));

	double dz,du;
	int iPosIndex = 0;
	dx = pCmd->pDblValue[iPosIndex*4+0];
	dy = pCmd->pDblValue[iPosIndex*4+1];
	dz = pCmd->pDblValue[iPosIndex*4+2];
	du = pCmd->pDblValue[iPosIndex*4+3];

	//////////////////////////////////////////////////////////////////////////
	circledp.iDotNum = int(du/0.5);
	double dr = pCmd->pDblValue[16];
	double dCycleLenth = 2*PI*dr;
	double dDispCyc, dDispFreq;
	g_pFrm->m_comDisp[0].GetCycleTimeAndFreq(dDispCyc, dDispFreq);

	circledp.dVelXY = dCycleLenth*dDispFreq/circledp.iDotNum;  
	CString strTemp;
	strTemp.Format("��Բ�������ٶ�:[%.3f]", circledp.dVelXY);
	g_pFrm->AddMsg(strTemp);
	//////////////////////////////////////////////////////////////////////////

	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}
	//circledp.dVelXY		=	m_cmdParam.dVelXY;
	circledp.dVelZ		=	m_cmdParam.dVelZ;
	circledp.dPreTime	=	m_cmdParam.tLine.dPreTime;
	circledp.bOn		=	pCmd->pIntValue[1];
	circledp.dWorkVel	=	m_cmdParam.tLine.dVel;
	circledp.dEndLen	=	m_cmdParam.tLine.dEndLen;
	circledp.iEndType	=	m_cmdParam.tLine.iEndType;
	circledp.dEndZupHigh	= m_cmdParam.tLine.dEndZupHigh;
	circledp.dEndVel		= m_cmdParam.tLine.dEndVel;
	circledp.dTailCloseLen	= m_cmdParam.tLine.dTailCloseLen;
	circledp.dZcleaType		= m_cmdParam.dZcleaType;
	circledp.dZclearance	= m_cmdParam.dZclearance;
	circledp.iEndType		= m_cmdParam.tLine.iEndType;

	circledp.circleType = (short)pCmd->pIntValue[4];
	circledp.pos.x	=	dx;
	circledp.pos.y	=	dy;
	circledp.pos.z	=	dz;
	circledp.pos.a	=	du;
	circledp.dDiameter	= pCmd->pDblValue[16];
	circledp.dOverAngle = pCmd->pDblValue[17];

	//m_CurToolNo = pCmd->pIntValue[0];//��ǰ��

	dcp[m_CurToolNo].bBit = circledp.bOn;

	dcp[m_CurToolNo].lowTime = m_cmdParam.nLowTime;
	dcp[m_CurToolNo].highTime = m_cmdParam.nHighTime;
	dcp[m_CurToolNo].lPulseNum = m_cmdParam.lPulseNum;

	short sTemp=m_pController->TS_ListCircle(CRD1,&dcp[m_CurToolNo],&circledp);

	return sTemp;
}
short CCmdRun::RunLineEnd(tgCmdLine *pPreTrackCmd,tgCmdLine *pCmd1,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	UNREFERENCED_PARAMETER(pNextTrackCmd);
	
	t_LineEnd ledp;
	::ZeroMemory(&ledp,sizeof(ledp));

	double dx,dy,dz,du;
	int iPosIndex = 0;
	dx = pCmd1->pDblValue[iPosIndex*4+0];
	dy = pCmd1->pDblValue[iPosIndex*4+1];
	dz = pCmd1->pDblValue[iPosIndex*4+2];
	du = pCmd1->pDblValue[iPosIndex*4+3];

// 	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
// 		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
// 	{
		if(GetCmdPositionEX(pCmd1,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
		if (RunMode_Dry == m_runMode)
		{
			dx -= g_pFrm->m_pSysParam->tOffset[0].x;
			dy -= g_pFrm->m_pSysParam->tOffset[0].y;
		}
// 	}else
// 	{
// 		if(GetCmdPosition(pCmd1,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
// 			return RTN_DATA_OVER;
// 	}

	if(0 != pCmd1->pIntValue[4])//������ʱ�ٶ�
		ledp.dWorkVel = pCmd1->pDblValue[4];
	else
		ledp.dWorkVel = m_cmdParam.tLine.dVel;


	if(0 != pCmd1->pIntValue[5])//������ʱ�ȴ�ʱ��  
		ledp.dWaitTime = pCmd1->pDblValue[5];
	else                         
		ledp.dWaitTime = 0; //m_cmdParam.tLine.dPreMoveTime;
	//ledp.dEndTime=m_cmdParam.tLine.dTailTime;

	ledp.dVelXY			=   m_cmdParam.dVelXY;
	ledp.dVelZ			=   m_cmdParam.dVelZ;
	ledp.dEndVel		=	m_cmdParam.tLine.dEndVel;
	ledp.dEndZupHigh	=	m_cmdParam.tLine.dEndZupHigh;
	ledp.dEndLen		=	m_cmdParam.tLine.dEndLen;
	ledp.iEndType		=	m_cmdParam.tLine.iEndType;
	ledp.dTailCloseLen	=	m_cmdParam.tLine.dTailCloseLen;
	ledp.dZcleaType		=	m_cmdParam.dZcleaType;
	ledp.dZclearance	=	m_cmdParam.dZclearance;
	ledp.pos.x	=	dx;
	ledp.pos.y	=	dy;
	ledp.pos.z	=	dz;
	ledp.pos.a	=	du;
	ledp.iDotNum = (int)(du/0.5);
	////
	if(pPreTrackCmd->tType==CMD_NONE)
	{
		for(int i=0;i<m_pController->g_ExMtnPara->axisNum;i++)
			pPreTrackCmd->pDblValue[iPosIndex+i] = m_pController->g_ExMtnPara->dfBakpos[i];
	}
	else
	{
// 		if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
// 			(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
// 		{
			if(GetCmdPositionEX(pPreTrackCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
				return RTN_DATA_OVER;
			if (RunMode_Dry == m_runMode)
			{
				dx -= g_pFrm->m_pSysParam->tOffset[0].x;
				dy -= g_pFrm->m_pSysParam->tOffset[0].y;
			}
// 		}else
// 		{
// 			if(GetCmdPosition(pPreTrackCmd,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
// 				return RTN_DATA_OVER;
// 		}
	}
	ledp.preCmdType = TranslateType(pPreTrackCmd->tType);
	ledp.preTrackPoint.pos.x	=	dx;
	ledp.preTrackPoint.pos.y	=	dy;
	ledp.preTrackPoint.pos.z	=	dz;
	ledp.preTrackPoint.pos.a	=	pPreTrackCmd->pDblValue[3];

	dcp[m_CurToolNo].bBit = FALSE;//���������ؽ�

	// ��� �߲���
	LINE_PARAM lineParam;
	::ZeroMemory(&lineParam,sizeof(LINE_PARAM));
	lineParam = g_pFrm->m_LineParm[(int)pCmd1->pDblValue[PARAM_POS]];

	ledp.dTailCloseLen = lineParam.dfShutOffDistance;		// ��ǰ�ؽ�����
	ledp.dEndZupHigh   = lineParam.dfBacktrackGap;			// �ܿ�ʱ̧���߶�
	ledp.dEndVel	   = lineParam.dfRetractSpeed;			// �ܿ��������ٶ�
	ledp.dTailCloseLen = lineParam.dfShutOffDistance;		// ��ǰ�ؽ�����

	short sTemp=m_pController->TS_ListLineEnd(CRD1,&dcp[m_CurToolNo],&ledp);

	return sTemp;
}
/******
// ָ������ 
// tDot0 ------- prior    command
// tDot1 ------- current  command
// tDot2 ------- next     command
// PreTrackPoint  ------- prior track point
// NextTrackPoint ------- next  track point
*****************/
short CCmdRun::RunAdd(int *iCurIndex,tgCmdLine *tDot0,tgCmdLine *tDot1,tgCmdLine *tDot2,tgCmdLine *PreTrackPoint,tgCmdLine *NextTrackPoint,double dOffsetX,double dOffsetY,double dOffsetZ)
{
	UNREFERENCED_PARAMETER(dOffsetZ);
	short sTemp;
	unsigned int uiTemp;
	double dHeight;
	switch(tDot1->tType)
	{
	case CMD_DUMMY:
		// ָ�������е㽺����ʼ��
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp =RunPoint(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_POINT: 
		// ָ�������е㽺����ʼ��
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunPoint(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_ZMETER:
		dHeight = 0;  //��ԭ�߶Ȳ�
		sTemp = RunZHeight(tDot1,dOffsetX,dOffsetY,&dHeight);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_REPEATX_ZH:
		sTemp = RunRepeatZMeter(1,tDot1,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_REPEATY_ZH:
		sTemp = RunRepeatZMeter(2,tDot1,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_WCL:
		sTemp =RunWCLine(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_WCRECT:
		sTemp =RunWCRect(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_POT:  
		sTemp =RunWCPot(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_LINE_START: 
		// ָ�������е㽺����ʼ��
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunLineStart(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_LINE_PASSING: 
		// ָ�������е㽺����ʼ��
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunLinePass(PreTrackPoint,tDot1,tDot2,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;			

	case CMD_DUNNY_SPEED:
		m_cmdParam.m_DummySpeed = tDot1->pDblValue[0];	
		break;
	case CMD_LINE_ARC:
		// ָ�������е㽺����ʼ��
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunLineArc(PreTrackPoint,tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_LINE_END:
		// ָ�������е㽺����ʼ��
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunLineEnd(PreTrackPoint,tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_DISPENSER:
		sTemp = RunDispensor(tDot1);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_TOOL:
		sTemp = RunTool(tDot1);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_WAIT_POINT:
		sTemp = RunStandBy(tDot1,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_BATH:  
		sTemp = RunWashNeedle();
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_GOTO_ADDRESS: 
		sTemp = m_pController->TS_ListWaitStop();
		if(sTemp != RTN_NONE) return sTemp;
		//Z����̧��
		sTemp = m_pController->TS_ListInit(CRD1,0);
		if(sTemp != RTN_NONE) return sTemp;

		if(tDot1->pIntValue[0] == 1)
		{//�����ת
			uiTemp = SearchLabel(tDot1->pszNote);
			if(uiTemp == RTN_PARAMETER_ERR)  break;	
			tDot1->pIntValue[1] = uiTemp;
		}
		sTemp = RunGoto(iCurIndex,*tDot0,*tDot1,*tDot2);
		return sTemp;
	case CMD_BRUSH_AREA:
		break;
	case CMD_DELAY:
		sTemp = RunDelay(tDot1);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_PAUSE:
		sTemp = RunPause();
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_HOME:
		sTemp = RunHome();
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_CIRCLE: 
		// ָ�������е㽺����ʼ��
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunCircle(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_SET_IO:
		// ָ�������е㽺����ʼ��
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunSetIO(tDot1);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case PARAM_LINE_SPEED: 
		m_cmdParam.tLine.dVel = tDot1->pDblValue[0];
		break;
	case PARAM_LINE_SETUP:	
		m_cmdParam.tLine.dPreTime = tDot1->pDblValue[0];			// ����ʱ��
		m_cmdParam.tLine.dNodeTime = tDot1->pDblValue[1];		// ��;ʱ��
		m_cmdParam.tLine.dTailTime = tDot1->pDblValue[3];		// ͣ��ʱ��
		m_cmdParam.tLine.dTailCloseLen = tDot1->pDblValue[2];	// ����������

		if(tDot1->iDblNum>4)
			m_cmdParam.tLine.dPreMoveTime = 0;//tDot1->pDblValue[4];
		else
			m_cmdParam.tLine.dPreMoveTime = 0;

		m_cmdParam.tLine.dPreMoveD=tDot1->pDblValue[4];
		m_cmdParam.tLine.dPresclae=tDot1->pDblValue[5];
		break;
	case PARAM_LINE_END:
		m_cmdParam.tLine.dEndLen = tDot1->pDblValue[0];
		m_cmdParam.tLine.dEndZupHigh = tDot1->pDblValue[1];
		m_cmdParam.tLine.dEndVel = tDot1->pDblValue[2];
		m_cmdParam.tLine.iEndType = tDot1->pIntValue[0];

		break;
	case PARAM_POINT_SETUP:
		m_cmdParam.tPoint.dPreTime = tDot1->pDblValue[0]; // ����ǰ��ʱ��
		m_cmdParam.tPoint.dDispTime = tDot1->pDblValue[1];// ����㽺ʱ��
		m_cmdParam.tPoint.dTailTime = tDot1->pDblValue[2];// ��������ȴ�ʱ��
		m_cmdParam.tPoint.DispUseWParam=tDot1->pIntValue[0];
		m_cmdParam.tPoint.DdispOn_T=tDot1->pIntValue[1];
		m_cmdParam.tPoint.DdispClose_T=tDot1->pIntValue[2];
		m_cmdParam.tPoint.DispCount=tDot1->pIntValue[3];

		break;
	case PARAM_JET_SETUP:
		m_cmdParam.nHighTime = tDot1->pIntValue[0];
		m_cmdParam.nLowTime  = tDot1->pIntValue[1];
		m_cmdParam.lPulseNum = (long)tDot1->pDblValue[0];

		break;
	case PARAM_POINT_END:
		m_cmdParam.tPoint.dZupHVel = tDot1->pDblValue[0];
		m_cmdParam.tPoint.dZupLVel = tDot1->pDblValue[1];
		m_cmdParam.tPoint.dZupHigh = tDot1->pDblValue[2];
		break;
	case PARAM_Z_CLEARANCE:
		m_cmdParam.dZcleaType = tDot1->pIntValue[0];
		m_cmdParam.dZclearance = tDot1->pDblValue[0];
		break;
	case PARAM_Z_WORKPOSSET:
		//m_cmdParam.dZcleaType = tDot1->pIntValue[0];
		//m_cmdParam.dZclearance = tDot1->pDblValue[0];
		break;
	case PARAM_Z_SET_HEIGHT:
		m_cmdParam.dZSetType = tDot1->pIntValue[0];
		m_cmdParam.dZSetHeight = tDot1->pDblValue[0];
		break;
	case PARAM_XY_VEL:
	case PARAM_Z_VEL:
		m_cmdParam.dVelXY = tDot1->pDblValue[0];
		m_cmdParam.dVelZ = tDot1->pDblValue[1];
		m_cmdParam.dAccXY = tDot1->pDblValue[2];
		m_cmdParam.dAccZ = tDot1->pDblValue[3];
		break;
	case CMD_LOOP:
		if(LoopAddCount <= -10)
			LoopAddCount = (long)tDot1->pIntValue[0];
		if(LoopAddCount == 1)
			LoopAddCount = -20;

		if(LoopAddCount > 1)
		{
			//m_pController->g_ExMtnPara->dfBakpos[Z_AXIS] = 0;//Z̧��	
			sTemp = RunGoto(iCurIndex,*tDot0,*tDot1,*tDot2);
			if(sTemp == RTN_GTS_ERROR) return sTemp;
			if(sTemp == RTN_ESTP) return sTemp;
			if(sTemp == RTN_STP) return sTemp;
			LoopAddCount --;
			if(sTemp == RTN_GOTO_ADD) return sTemp;
		}

		break;
	default:  break;
	}
	return RTN_NONE;
}
/******
// ָ������ 
// tDot0 ------- prior    command
// tDot1 ------- current  command
// tDot2 ------- next     command
// PreTrackPoint  ------- prior track point
// NextTrackPoint ------- next  track point
*****************/
short CCmdRun::RunAdd_EX(int *iCurIndex,tgCmdLine *tDot0,tgCmdLine *tDot1,tgCmdLine *tDot2,tgCmdLine *PreTrackPoint,tgCmdLine *NextTrackPoint,double dOffsetX,double dOffsetY,double dOffsetZ)
{
	UNREFERENCED_PARAMETER(dOffsetZ);
	UNREFERENCED_PARAMETER(PreTrackPoint);

	short sTemp;
	unsigned int uiTemp;
	double dHeight;
	switch(tDot1->tType)
	{
	case CMD_DUMMY:
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp =RunPoint(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_SUPERDOT: 
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunFlyPoint_Dots(tDot1,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_SUPERSTART:
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunFlyPoint_StartPos(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_SUPERPOINT:
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunFlyPoint_MidPos(PreTrackPoint, tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_SUPEREND:
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunFlyPoint_EndPos(PreTrackPoint, tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_CIRCLE:
		sTemp = RunCircle(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_ZMETER:
		dHeight = 0;  //��ԭ�߶Ȳ�
		sTemp = RunZHeight(tDot1,dOffsetX,dOffsetY,&dHeight);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_LINE_ARC:
		sTemp = RunLineArc(PreTrackPoint,tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_POT:  
		sTemp =RunWCPot(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;		
	case CMD_DUNNY_SPEED:
		m_cmdParam.m_DummySpeed = tDot1->pDblValue[0];	
		break;
	case CMD_DISPENSER:
		sTemp = RunDispensor(tDot1);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_TOOL:
		sTemp = RunTool(tDot1);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_WAIT_POINT:
		sTemp = RunStandBy(tDot1,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_BATH:  
		sTemp = RunWashNeedle();
		if(sTemp!=RTN_NONE) return sTemp;
		break;  
	case CMD_GOTO_ADDRESS:   // ��תָ��
		sTemp = m_pController->TS_ListWaitStop();
		if(sTemp != RTN_NONE) return sTemp;
		//Z����̧��
		sTemp = m_pController->TS_ListInit(CRD1,1);
		if(sTemp != RTN_NONE) return sTemp;

		if(tDot1->pIntValue[0] == 1)
		{//�����ת
			uiTemp = SearchLabel(tDot1->pszNote);
			if(uiTemp == RTN_PARAMETER_ERR)  break;	
			tDot1->pIntValue[1] = uiTemp;
		}
		PoaIndex=0;
		sTemp = RunGoto(iCurIndex,*tDot0,*tDot1,*tDot2);
		return sTemp;

	case CMD_BRUSH_AREA:
		break;

	case CMD_DELAY:
		sTemp = RunDelay(tDot1);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_PAUSE:
		sTemp = RunPause();
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_HOME:
		sTemp = RunHome();
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case CMD_SET_IO:
		sTemp = RunSetIO(tDot1);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case PARAM_JET_SETUP:
		m_cmdParam.nHighTime = tDot1->pIntValue[0];
		m_cmdParam.nLowTime  = tDot1->pIntValue[1];
		m_cmdParam.lPulseNum = (long)tDot1->pDblValue[0];
		break;
	case PARAM_Z_CLEARANCE:
		m_cmdParam.dZcleaType = tDot1->pIntValue[0];
		m_cmdParam.dZclearance = tDot1->pDblValue[0];
		break;
	case PARAM_Z_WORKPOSSET:
		//m_cmdParam.dZcleaType = tDot1->pIntValue[0];
		//m_cmdParam.dZclearance = tDot1->pDblValue[0];
		break;
	case PARAM_Z_SET_HEIGHT:
		m_cmdParam.dZSetType = tDot1->pIntValue[0];
		m_cmdParam.dZSetHeight = tDot1->pDblValue[0];
		break;
	case PARAM_XY_VEL:
	case PARAM_Z_VEL:
		m_cmdParam.dVelXY = tDot1->pDblValue[0];
		m_cmdParam.dVelZ = tDot1->pDblValue[1];
		m_cmdParam.dAccXY = tDot1->pDblValue[2];
		m_cmdParam.dAccZ = tDot1->pDblValue[3];
		break;
	case CMD_LOOP:
		if(LoopAddCount <= -10)
			LoopAddCount = (long)tDot1->pIntValue[0];
		if(LoopAddCount == 1)
			LoopAddCount = -20;

		if(LoopAddCount > 1)
		{
			sTemp = RunGoto(iCurIndex,*tDot0,*tDot1,*tDot2);
			if(sTemp == RTN_GTS_ERROR) return sTemp;
			if(sTemp == RTN_ESTP) return sTemp;
			if(sTemp == RTN_STP) return sTemp;
			LoopAddCount --;
			if(sTemp == RTN_GOTO_ADD) return sTemp;                                                
		}
		break;
	default:  break;
	}
	return RTN_NONE;
}

// ��������λ��
BOOL CCmdRun::CalcBoradArcData(double &dx, double &dy, double dXDis, double dYDis)
{   
	dx += dXDis;
	dy += dYDis;

	return TRUE;
}

// ����н����ݼ���
BOOL CCmdRun::CalcBoradArcData(double &dx,double &dy,double dCurXdis,double dCurYdis,double dRArc)
{   
	// ����н�
	double darc = dRArc;
	double dSinarc = sin(darc);
	double dCosarc = cos(darc);
	double dSinarcF = sin(-darc);
	double dCosarcF = cos(-darc);

	//////////////////////////////////////////////////////////////////////////
	dx = dx-(g_pFrm->m_pSysParam->tOffset[m_CurToolNo].x);
	dy = dy-(g_pFrm->m_pSysParam->tOffset[m_CurToolNo].y);
	//////////////////////////////////////////////////////////////////////////

	double tempXData =0;
	double tempYData =0;
	double tt1,tt2;
	///////////////////////////////////////// 
	// �Ƚ���ǰ�������ݴӲ���ģ������ϵ(��Ʒ����)ת������׼ģ������ϵ(��е����)
	//X = Cos(-@)*x - Sin(-@)*y;
	//Y = Sin(-@)*x + Cos(-@)*y;
	tempXData = dx *dCosarcF - dSinarcF * dy; 
	tempYData = dx *dSinarcF + dCosarcF * dy; 

	tt1 = tempXData + dCurXdis;
	tt2 = tempYData + dCurYdis;

	// �ٽ���ǰ�������ݴӱ�׼ģ������ϵ(��е����)ת��������ģ������ϵ(��Ʒ����)
	// X = Cos(@)*x - Sin(@)*y;
	// Y = Sin(@)*x + Cos(@)*y;
	tempXData =0;
	tempYData =0;

	tempXData = dCosarc * tt1 - dSinarc * tt2;
	tempYData = dSinarc * tt1 + dCosarc * tt2;

	dx = tempXData;
	dy = tempYData;

	//////////////////////////////////////////////////////////////////////////
	dx = dx+(g_pFrm->m_pSysParam->tOffset[m_CurToolNo].x);
	dy = dy+(g_pFrm->m_pSysParam->tOffset[m_CurToolNo].y);
	//////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////
	/*
	tempXData =0;
	tempYData =0;
	tempXData = dCurRX *dCosarc - dCurRY *dSinarc;
	tempYData = dCurRX *dSinarc + dCurRY *dCosarc;

	dx += tempXData; 
	dy += tempYData; 
	*/
	return TRUE;
}
// ����ִ��
/*������*/
//pCmd->pDblValue[0] // x����
//pCmd->pDblValue[1] // y����
//pCmd->pDblValue[2] // �н�
//pCmd->pIntValue[0] // ��ʼ��ַ
//pCmd->pIntValue[1] // 1:S,2:N
//pCmd->pIntValue[2] // X��Ŀ
//pCmd->pIntValue[3] // Y��Ŀ
//pCmd->pIntValue[4] // Z��߶Ȳ�����־
//m_dFirstHeightOffset = 0;   // ���п�ʼǰʹ��һ�Σ����к�ʹ�ô˱�����
short CCmdRun::RunRepeat(unsigned char type,/*int nRepeatEnd,*/tgCmdLine pCmd,double dOffsetX,double dOffsetY)
{  
	BOOL    bZCheckFlag = FALSE;	// ����߶ȼ���ʾ
	
	long numx,numy;					// X Y ���и���
	numx = pCmd.pIntValue[2];		// X��Ŀ
	numy = pCmd.pIntValue[3];		// Y��Ŀ
	double  offx, offy;				// ���� ���
	double  xSpace=0, ySpace=0;		// XY���м��
	double  xSpaceCorrect=0, ySpaceCorrect=0;	// XY���м������

	if (numx>1)
	{
		xSpace = (pCmd.pDblValue[4]-pCmd.pDblValue[0]) / (numx-1);
		ySpaceCorrect = (pCmd.pDblValue[5]-pCmd.pDblValue[1]) / (numx-1);
	}
	if (numy>1)
	{
		ySpace = (pCmd.pDblValue[9]-pCmd.pDblValue[5]) / (numy-1);
		xSpaceCorrect = (pCmd.pDblValue[8]-pCmd.pDblValue[4]) / (numy-1);
	}
	
	unsigned char StoL = 1; // 1���� 0-�ݼ�

	int nRepeatSt=0, nRepeatEnd=0;		// ������ֹ��ַ
	nRepeatSt = pCmd.pIntValue[0];
	nRepeatEnd = pCmd.pIntValue[6];

	short  sTemp;
	double dZTempHeightValue = 0;

	long num = numx * numy;   // �����ܸ���
	
	tgCmdLine address0;  // ��һ��ָ��
	tgCmdLine address1;  // ��ǰָ��
	tgCmdLine address2;  // ��һ��ָ��
	tgCmdLine tPreTrackCmd;   // ��һ�� ����ָ��
	tgCmdLine tNextTrackCmd;  // ��һ�� ����ָ��

	unsigned char ucRepeatModeS_N;     // �ƶ���ʽ //0:S,1:N
	ucRepeatModeS_N = (unsigned char)pCmd.pIntValue[1]; 
	
	double dCurRArc = atan2((pCmd.pDblValue[5]-pCmd.pDblValue[1]), (pCmd.pDblValue[4]-pCmd.pDblValue[0]));//pCmd.pDblValue[3];  // �н�

	long  lcx, lcy;   // ���з�ʽ�� 
	if(type == 1)     // X ��������
	{   // repeat x 
// 		if (m_structPathInfo.m_bRepeatSetStart)
// 		{
			lcx = pCmd.pIntValue[4]-1;//m_structPathInfo.m_nRepeatStX-1; 
			lcy = pCmd.pIntValue[5]-1;//m_structPathInfo.m_nRepeatStY-1;

			if ((0==ucRepeatModeS_N) && (lcy&1))	// S�͵㽺������ʱ
			{
				StoL = 0;
				num = numx*(numy-lcy)-(numx-lcx);
			}
			else
			{
				num = numx*(numy-lcy)-lcx;
			}
			CString str;
			str.Format("repeat x:%d,   %d", lcx, lcy);
			g_pFrm->AddMsg(str);
// 		}
// 		else
// 		{
// 			lcx = 0;  lcy = 0;
// 		}
	}
	else 
	{	//repeat y
// 		if (m_structPathInfo.m_bRepeatSetStart)
// 		{
			lcx = pCmd.pIntValue[4]-1;//m_structPathInfo.m_nRepeatStX-1; 
			lcy = pCmd.pIntValue[5]-1;//m_structPathInfo.m_nRepeatStY-1;

			if ((0==ucRepeatModeS_N) && (lcx&1))	// S�͵㽺������ʱ
			{
				StoL = 0;
				num = (numx-lcx)*numy-(numy-lcy);
			}
			else
			{
				num = (numx-lcx)*numy-lcy;
			}
			CString str;
			str.Format("repeat x:%d,   %d", lcx, lcy);
			g_pFrm->AddMsg(str);
			//num = (numx-lcx)*numy-lcy;
// 		}
// 		else
// 		{
// 			lcx = 0;  lcy = 0;
// 		}
	}

	//////////////////////////////////////////////////////////////////////////
	if(GetRecord(address1,nRepeatSt)) 
		return RTN_PARAMETER_ERR;
	if (CMD_ZMETER == address1.tType)	// ���в��
	{
		bZCheckFlag = TRUE;

		int num0=num;
		m_ZMeterVector.clear();
		for (; num0>0; num0--)
		{
			//////////// X �������� ////////////////////
			offx = xSpace*lcx + xSpaceCorrect*lcy;  // ����X���
			offy = ySpace*lcy + ySpaceCorrect*lcx;  // ����Y���

			if(type == 1)
			{//repeat x
				if(ucRepeatModeS_N == 0)  // S ��ʽ�ƶ�
				{
					if(StoL) 
						lcx ++;
					else
						lcx --;

					if(lcx >= numx)  // ÿ�����һ�� 
					{
						StoL = 0;  
						lcx = numx - 1;
						lcy ++;
					}
					else if(lcx == -1)
					{
						StoL = 1;
						lcx = 0;
						lcy ++;
					}
				}
				else
				{   //  N
					lcx ++;
					if(lcx >= numx)
					{
						lcx = 0;
						lcy ++;
					}
				}
			}
			else //repeat y
			{
				if(ucRepeatModeS_N == 0)
				{// S
					if(StoL)
						lcy ++;
					else 
						lcy --;
					if(lcy >= numy)
					{
						StoL = 0;
						lcy = numy - 1;
						lcx ++;
					}
					else if(lcy == -1)
					{
						StoL = 1;
						lcy = 0;
						lcx ++;
					}
				}
				else
				{//N
					lcy ++;
					if(lcy >= numy)
					{
						lcy = 0;
						lcx ++;
					}
				}
			}
			if(GetRecord(address1,nRepeatSt)) 
				return RTN_PARAMETER_ERR;
			CalcBoradArcData(address1.pDblValue[0],address1.pDblValue[1],offx+dOffsetX,offy+dOffsetY);

			double dHeight = 0;  //��ԭ�߶Ȳ�
			sTemp = RunZHeight(&address1,dOffsetX,dOffsetY,&dHeight);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// ������ԭ
	if(type == 1)     // X ��������
	{   // repeat x 
		// 		if (m_structPathInfo.m_bRepeatSetStart)
		// 		{
		lcx = pCmd.pIntValue[4]-1;//m_structPathInfo.m_nRepeatStX-1; 
		lcy = pCmd.pIntValue[5]-1;//m_structPathInfo.m_nRepeatStY-1;

		if ((0==ucRepeatModeS_N) && (lcy&1))	// S�͵㽺������ʱ
		{
			StoL = 0;
			num = numx*(numy-lcy)-(numx-lcx);
		}
		else
		{
			num = numx*(numy-lcy)-lcx;
		}
		CString str;
		str.Format("repeat x:%d,   %d", lcx, lcy);
		g_pFrm->AddMsg(str);
		// 		}
		// 		else
		// 		{
		// 			lcx = 0;  lcy = 0;
		// 		}
	}
	else 
	{	//repeat y
		// 		if (m_structPathInfo.m_bRepeatSetStart)
		// 		{
		lcx = pCmd.pIntValue[4]-1;//m_structPathInfo.m_nRepeatStX-1; 
		lcy = pCmd.pIntValue[5]-1;//m_structPathInfo.m_nRepeatStY-1;

		if ((0==ucRepeatModeS_N) && (lcx&1))	// S�͵㽺������ʱ
		{
			StoL = 0;
			num = (numx-lcx)*numy-(numy-lcy);
		}
		else
		{
			num = (numx-lcx)*numy-lcy;
		}
		CString str;
		str.Format("repeat x:%d,   %d", lcx, lcy);
		g_pFrm->AddMsg(str);
		//num = (numx-lcx)*numy-lcy;
		// 		}
		// 		else
		// 		{
		// 			lcx = 0;  lcy = 0;
		// 		}
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	int nNeedDotNum;	// ���úõĵ㽺��Ŀ
	nNeedDotNum = pCmd.pIntValue[7];
	if (0 != nNeedDotNum)
	{
		nNeedDotNum = num -nNeedDotNum;
	}
	nNeedDotNum = max(0, nNeedDotNum);
	// �����ܸ���
	for(/*num = numx*numy-1*/;num>nNeedDotNum;num--)
	{   
		if (K_RUN_STS_RUN_1_UNIT == m_tSysStatus)
		{
			g_bStop = TRUE;
			return RTN_STP;
		}
		m_structPathInfo.m_bRepeatSetStart = false;
		///////////////////////
		dZTempHeightValue = 0;   

// 		if(bZCheckFlag==FALSE)  // ��߱�ʾ
// 		{
// 			for(UINT i=0;i<m_ZMeterVector.size();i++)
// 			{
// 				if((numx*numy-num)==m_ZMeterVector[i].index)
// 				{
// 					dZTempHeightValue = m_ZMeterVector[numx*numy-num].delta;
// 					break;
// 				}
// 			}
// 		}
		//////////// X �������� ////////////////////
 		offx = xSpace*lcx + xSpaceCorrect*lcy;  // ����X���
 		offy = ySpace*lcy + ySpaceCorrect*lcx;  // ����Y���
		
		if(type == 1)
		{//repeat x
			if(ucRepeatModeS_N == 0)  // S ��ʽ�ƶ�
			{
				if(StoL) 
					lcx ++;
				else
					lcx --;

				if(lcx >= numx)  // ÿ�����һ�� 
				{
					StoL = 0;  
					lcx = numx - 1;
					lcy ++;
				}
				else if(lcx == -1)
				{
					StoL = 1;
					lcx = 0;
					lcy ++;
				}
			}
			else
			{   //  N
				lcx ++;
				if(lcx >= numx)
				{
					lcx = 0;
					lcy ++;
				}
			}
		}
		else //repeat y
		{
			if(ucRepeatModeS_N == 0)
			{// S
				if(StoL)
					lcy ++;
				else 
					lcy --;
				if(lcy >= numy)
				{
					StoL = 0;
					lcy = numy - 1;
					lcx ++;
				}
				else if(lcy == -1)
				{
					StoL = 1;
					lcy = 0;
					lcx ++;
				}
			}
			else
			{//N
				lcy ++;
				if(lcy >= numy)
				{
					lcy = 0;
					lcx ++;
				}
			}
		}

		if(nRepeatSt>m_cmdRunList.GetCount()||nRepeatSt<0)
			return RTN_PARAMETER_ERR;//������Ҫ����к��Ƿ�Ϸ�

		for(; nRepeatSt<nRepeatEnd; nRepeatSt++)  
		{
			while (K_RUN_STS_PAUSE == m_tSysStatus)
				Sleep(100);

			if(nRepeatSt==0)
			{   // �ӵ�һ��ָ�ʼ
				address0.tType = CMD_NONE;
				if(GetRecord(address1,0))  
					return RTN_PARAMETER_ERR;
				if(GetRecord(address2,1)) 
					return RTN_PARAMETER_ERR;
			}
			else if(nRepeatSt==nRepeatEnd)
			{   // ֻ��һ��ָ��
				if(GetRecord(address0,nRepeatSt - 1)) 
					return RTN_PARAMETER_ERR;
				if(GetRecord(address1,nRepeatSt))
					return RTN_PARAMETER_ERR;
				address2.tType = CMD_NONE;
			}
			else
			{
				if(GetRecord(address0,nRepeatSt - 1)) 
					return RTN_PARAMETER_ERR;
				if(GetRecord(address1,nRepeatSt)) 
					return RTN_PARAMETER_ERR;
				if(GetRecord(address2,nRepeatSt + 1)) 
					return RTN_PARAMETER_ERR;
			}

			//read prior and next track point			
			int iFirst=GetPreDot(nRepeatSt, tPreTrackCmd);
			int iNext=GetNextDot(nRepeatSt, tNextTrackCmd);

			if(-1!=iFirst)
			{
				switch(tPreTrackCmd.tType)  // �϶���ָ������
				{
				case CMD_POINT:
				case CMD_DUMMY:
				case CMD_LINE_START:
				case CMD_LINE_PASSING:
				case CMD_LINE_ARC: 
				case CMD_LINE_END:
				case CMD_CIRCLE:
				case CMD_ZMETER:
				case CMD_POT:
				case CMD_SUPERDOT:
				case CMD_SUPERSTART:
				case CMD_SUPERPOINT:
				case CMD_SUPEREND:
					//CalcBoradArcData(tPreTrackCmd.pDblValue[0],tPreTrackCmd.pDblValue[1],offx,offy, dCurRArc);
					CalcBoradArcData(tPreTrackCmd.pDblValue[0],tPreTrackCmd.pDblValue[1],offx+dOffsetX,offy+dOffsetY);
					if (bZCheckFlag==FALSE)
					{
						if(tPreTrackCmd.tType!=CMD_ZMETER)//����ҪУ����������ָ��
						{
							tPreTrackCmd.pDblValue[2] += dZTempHeightValue;
						}
					}
					break;

				case CMD_WCL:
					CalcBoradArcData(tPreTrackCmd.pDblValue[0],tPreTrackCmd.pDblValue[1],offx,offy,dCurRArc);
					CalcBoradArcData(tPreTrackCmd.pDblValue[4],tPreTrackCmd.pDblValue[5],offx,offy,dCurRArc);
					if (bZCheckFlag==FALSE)
					{  
						//����ҪУ����������ָ��
						if(tPreTrackCmd.tType!=CMD_ZMETER)
						{
							tPreTrackCmd.pDblValue[2] += dZTempHeightValue;
							tPreTrackCmd.pDblValue[6] += dZTempHeightValue;
						}
					}
					break;

				case CMD_WCRECT:
					CalcBoradArcData(tPreTrackCmd.pDblValue[0],tPreTrackCmd.pDblValue[1],offx,offy,dCurRArc);
					CalcBoradArcData(tPreTrackCmd.pDblValue[4],tPreTrackCmd.pDblValue[5],offx,offy,dCurRArc);
					CalcBoradArcData(tPreTrackCmd.pDblValue[8],tPreTrackCmd.pDblValue[9],offx,offy,dCurRArc);
					CalcBoradArcData(tPreTrackCmd.pDblValue[12],tPreTrackCmd.pDblValue[13],offx,offy,dCurRArc);
					CalcBoradArcData(tPreTrackCmd.pDblValue[16],tPreTrackCmd.pDblValue[17],offx,offy,dCurRArc);

					if (bZCheckFlag==FALSE)
					{  
						//����ҪУ����������ָ��
						if(tPreTrackCmd.tType!=CMD_ZMETER)
						{
							tPreTrackCmd.pDblValue[2] += dZTempHeightValue;
							tPreTrackCmd.pDblValue[6] += dZTempHeightValue;
							tPreTrackCmd.pDblValue[10] += dZTempHeightValue;
							tPreTrackCmd.pDblValue[14] += dZTempHeightValue;
							tPreTrackCmd.pDblValue[18] += dZTempHeightValue;
						}
					}
					break;

				default: break;
				}
			}
			if (-1!=iNext)   
			{   
				switch(tNextTrackCmd.tType) // �¶���ָ������
				{
				case CMD_POINT:
				case CMD_LINE_START:
				case CMD_LINE_PASSING:
				case CMD_DUMMY:
				case CMD_LINE_ARC: 
				case CMD_LINE_END:
				case CMD_CIRCLE:
				case CMD_ZMETER:
				case CMD_POT:
				case CMD_SUPERDOT:
				case CMD_SUPERSTART:
				case CMD_SUPERPOINT:
				case CMD_SUPEREND:
					CalcBoradArcData(tNextTrackCmd.pDblValue[0],tNextTrackCmd.pDblValue[1],offx+dOffsetX,offy+dOffsetY/*, dCurRArc*/);
					if (bZCheckFlag==FALSE)
					{
						if(tNextTrackCmd.tType!=CMD_ZMETER)//����ҪУ����������ָ��
						{
							tNextTrackCmd.pDblValue[2] += dZTempHeightValue;
						}
					}
					break;

				case CMD_WCL:
					CalcBoradArcData(tNextTrackCmd.pDblValue[0],tNextTrackCmd.pDblValue[1],offx,offy,dCurRArc);
					CalcBoradArcData(tNextTrackCmd.pDblValue[4],tNextTrackCmd.pDblValue[5],offx,offy,dCurRArc);
					if (bZCheckFlag==FALSE)
					{
						if(tNextTrackCmd.tType!=CMD_ZMETER)//����ҪУ����������ָ��
						{
							tNextTrackCmd.pDblValue[2] += dZTempHeightValue;
							tNextTrackCmd.pDblValue[6] += dZTempHeightValue;
						}
					}
					break;

				case CMD_WCRECT:
					CalcBoradArcData(tNextTrackCmd.pDblValue[0],tNextTrackCmd.pDblValue[1],offx,offy,dCurRArc);
					CalcBoradArcData(tNextTrackCmd.pDblValue[4],tNextTrackCmd.pDblValue[5],offx,offy,dCurRArc);
					CalcBoradArcData(tNextTrackCmd.pDblValue[8],tNextTrackCmd.pDblValue[9],offx,offy,dCurRArc);
					CalcBoradArcData(tNextTrackCmd.pDblValue[12],tNextTrackCmd.pDblValue[13],offx,offy,dCurRArc);
					CalcBoradArcData(tNextTrackCmd.pDblValue[16],tNextTrackCmd.pDblValue[17],offx,offy,dCurRArc);

					if (bZCheckFlag==FALSE)
					{  
						//����ҪУ����������ָ��
						if(tPreTrackCmd.tType!=CMD_ZMETER)
						{
							tNextTrackCmd.pDblValue[2] += dZTempHeightValue;
							tNextTrackCmd.pDblValue[6] += dZTempHeightValue;
							tNextTrackCmd.pDblValue[10] += dZTempHeightValue;
							tNextTrackCmd.pDblValue[14] += dZTempHeightValue;
							tNextTrackCmd.pDblValue[18] += dZTempHeightValue;
						}
					}
					break;
				default: break;
				}
			}
			//////////  ָ�� ////////
			switch(address0.tType)
			{
			case CMD_POINT:
			case CMD_LINE_START:
			case CMD_LINE_PASSING:
			case CMD_DUMMY:
			case CMD_LINE_ARC: 
			case CMD_LINE_END:
			case CMD_CIRCLE:
			case CMD_ZMETER:
			case CMD_POT:
			case CMD_SUPERDOT:
			case CMD_SUPERSTART:
			case CMD_SUPERPOINT:
			case CMD_SUPEREND:
				CalcBoradArcData(address0.pDblValue[0],address0.pDblValue[1],offx+dOffsetX,offy+dOffsetY/*, dCurRArc*/);
				break;

			case CMD_WCL:
				CalcBoradArcData(address0.pDblValue[0],address0.pDblValue[1],offx,offy,dCurRArc);
				CalcBoradArcData(address0.pDblValue[4],address0.pDblValue[5],offx,offy,dCurRArc);
				break;

			case CMD_WCRECT:
				CalcBoradArcData(address0.pDblValue[0],address0.pDblValue[1],offx,offy,dCurRArc);
				CalcBoradArcData(address0.pDblValue[4],address0.pDblValue[5],offx,offy,dCurRArc);
				CalcBoradArcData(address0.pDblValue[8],address0.pDblValue[9],offx,offy,dCurRArc);
				CalcBoradArcData(address0.pDblValue[12],address0.pDblValue[13],offx,offy,dCurRArc);
				CalcBoradArcData(address0.pDblValue[16],address0.pDblValue[17],offx,offy,dCurRArc);
				break;

			default: break;
			}

			switch(address1.tType)
			{
			case CMD_POINT:
			case CMD_LINE_START:
			case CMD_LINE_PASSING:
			case CMD_DUMMY:
			case CMD_LINE_ARC: 
			case CMD_LINE_END:
			case CMD_CIRCLE:
			case CMD_ZMETER:
			case CMD_POT:
			case CMD_SUPERDOT:
			case CMD_SUPERSTART:
			case CMD_SUPERPOINT:
			case CMD_SUPEREND:
				CalcBoradArcData(address1.pDblValue[0],address1.pDblValue[1],offx,offy/*, dCurRArc*/);
				if (bZCheckFlag==FALSE)
				{
					if(address1.tType!=CMD_ZMETER)//����ҪУ����������ָ��
					{
						address1.pDblValue[2] += dZTempHeightValue;
					}
				}	
				break;

			case CMD_WCL:
				CalcBoradArcData(address1.pDblValue[0],address1.pDblValue[1],offx,offy,dCurRArc);
				CalcBoradArcData(address1.pDblValue[4],address1.pDblValue[5],offx,offy,dCurRArc);

				if (bZCheckFlag==FALSE)
				{
					if(address1.tType!=CMD_ZMETER)//����ҪУ����������ָ��
					{
						address1.pDblValue[6] += dZTempHeightValue;
						address1.pDblValue[2] += dZTempHeightValue;
					}
				}	
				break;

			case CMD_WCRECT:
				CalcBoradArcData(address1.pDblValue[0],address1.pDblValue[1],offx,offy,dCurRArc);
				CalcBoradArcData(address1.pDblValue[4],address1.pDblValue[5],offx,offy,dCurRArc);
				CalcBoradArcData(address1.pDblValue[8],address1.pDblValue[9],offx,offy,dCurRArc);
				CalcBoradArcData(address1.pDblValue[12],address1.pDblValue[13],offx,offy,dCurRArc);
				CalcBoradArcData(address1.pDblValue[16],address1.pDblValue[17],offx,offy,dCurRArc);

				if (bZCheckFlag==FALSE)
				{
					if(address1.tType!=CMD_ZMETER)//����ҪУ����������ָ��
					{
						address1.pDblValue[6] += dZTempHeightValue;
						address1.pDblValue[2] += dZTempHeightValue;
						address1.pDblValue[10] += dZTempHeightValue;
						address1.pDblValue[14] += dZTempHeightValue;
						address1.pDblValue[18] += dZTempHeightValue;
					}
				}	
				break;
			default: break;
			}

			switch(address2.tType)
			{
			case CMD_POINT:
			case CMD_LINE_START:
			case CMD_LINE_PASSING:
			case CMD_DUMMY:
			case CMD_LINE_ARC: 
			case CMD_LINE_END:
			case CMD_CIRCLE:
			case CMD_ZMETER:
			case CMD_POT:
			case CMD_SUPERDOT:
			case CMD_SUPERSTART:
			case CMD_SUPERPOINT:
			case CMD_SUPEREND:
				CalcBoradArcData(address2.pDblValue[0],address2.pDblValue[1],offx+dOffsetX,offy+dOffsetY/*, dCurRArc*/);
				break;

			case CMD_WCL:
				CalcBoradArcData(address2.pDblValue[0],address2.pDblValue[1],offx,offy,dCurRArc);
				CalcBoradArcData(address2.pDblValue[4],address2.pDblValue[5],offx,offy,dCurRArc);
				break;

			case CMD_WCRECT:
				CalcBoradArcData(address2.pDblValue[0],address2.pDblValue[1],offx,offy,dCurRArc);
				CalcBoradArcData(address2.pDblValue[4],address2.pDblValue[5],offx,offy,dCurRArc);
				CalcBoradArcData(address2.pDblValue[8],address2.pDblValue[9],offx,offy,dCurRArc);
				CalcBoradArcData(address2.pDblValue[12],address2.pDblValue[13],offx,offy,dCurRArc);
				CalcBoradArcData(address2.pDblValue[16],address2.pDblValue[17],offx,offy,dCurRArc);
				break;
			default: break;
			}

			//  ִ�е�ǰָ�� 
			switch(address1.tType)
			{
			case CMD_POINT:
			case CMD_LINE_START:
			case CMD_LINE_PASSING:
			case CMD_DUMMY:
			case CMD_LINE_ARC: 
			case CMD_LINE_END:
			case CMD_CIRCLE:
				if ((address1.pDblValue[0]<=m_pController->g_ExMtnPara->dfLength[X_AXIS])
					&&(address1.pDblValue[1]<=m_pController->g_ExMtnPara->dfLength[Y_AXIS]))
				{
					sTemp = RunAdd(&nRepeatSt,&address0,&address1,&address2,&tPreTrackCmd,&tNextTrackCmd,dOffsetX,dOffsetY);
					if(sTemp == RTN_PARAMETER_ERR)	return RTN_PARAMETER_ERR;
					if(sTemp == RTN_ESTP)			return RTN_ESTP;
					if(sTemp == RTN_STP)			return RTN_STP;
					if(sTemp == RTN_TIMEOUT)		return RTN_TIMEOUT;
				}
				break;
			case CMD_BRUSH_AREA:
			case CMD_BATH:
			//case CMD_ZMETER:
			case CMD_WCL:
			case CMD_WCRECT:
			case CMD_POT:
			case CMD_SUPERSTART:
			case CMD_SUPERPOINT:
			case CMD_SUPEREND:
			case CMD_SUPERDOT:
				if ((address1.pDblValue[0]<=m_pController->g_ExMtnPara->dfLength[X_AXIS])
					&&(address1.pDblValue[1]<=m_pController->g_ExMtnPara->dfLength[Y_AXIS]))
				{
					sTemp = RunAdd_EX(&nRepeatSt,&address0,&address1,&address2,&tPreTrackCmd,&tNextTrackCmd,dOffsetX,dOffsetY);
					if(sTemp == RTN_PARAMETER_ERR)	return RTN_PARAMETER_ERR;
					if(sTemp == RTN_ESTP)			return RTN_ESTP;
					if(sTemp == RTN_STP)			return RTN_STP;
					if(sTemp == RTN_TIMEOUT)		return RTN_TIMEOUT;
				}
				break;

			case CMD_REPEATY:
				sTemp = RunRepeat(2, address1, offx+dOffsetX, offy+dOffsetY);
				if(sTemp == RTN_PARAMETER_ERR)	return RTN_PARAMETER_ERR;
				if(sTemp == RTN_ESTP)			return RTN_ESTP;
				if(sTemp == RTN_STP)			return RTN_STP;
				if(sTemp == RTN_TIMEOUT)		return RTN_TIMEOUT;
				address1.pIntValue[4] = 1;
				address1.pIntValue[5] = 1;
				break;

			case CMD_REPEATX:
				sTemp = RunRepeat(1, address1, offx+dOffsetX, offy+dOffsetY);
				if(sTemp == RTN_PARAMETER_ERR)	return RTN_PARAMETER_ERR;
				if(sTemp == RTN_ESTP)			return RTN_ESTP;
				if(sTemp == RTN_STP)			return RTN_STP;
				if(sTemp == RTN_TIMEOUT)		return RTN_TIMEOUT;
				address1.pIntValue[4] = 1;
				address1.pIntValue[5] = 1;
				break;

			case CMD_TOOL:
			case CMD_DISPENSER:				
			case CMD_GOTO_ADDRESS:
			case CMD_SET_IO:
			case CMD_DELAY:
			case CMD_PAUSE:
			case CMD_LOOP:			
			case CMD_DUNNY_SPEED:
			case PARAM_LINE_SPEED:
			case PARAM_LINE_SETUP:
			case PARAM_LINE_END:
			case PARAM_POINT_SETUP:
			case PARAM_JET_SETUP:
			case PARAM_POINT_END:
			case PARAM_Z_CLEARANCE:
			case PARAM_Z_WORKPOSSET:
			case PARAM_Z_SET_HEIGHT:
			case PARAM_XY_VEL:
			case PARAM_Z_VEL:
				sTemp = RunAdd(&nRepeatSt,&address0,&address1,&address2,&tPreTrackCmd,&tNextTrackCmd,dOffsetX,dOffsetY);
				if(sTemp == RTN_PARAMETER_ERR) return RTN_PARAMETER_ERR;
				if(sTemp == RTN_ESTP) return RTN_ESTP;
				if(sTemp == RTN_STP) return RTN_STP;
				if(sTemp == RTN_TIMEOUT) return RTN_TIMEOUT;
				break;
			default:  break;
			}
		}
	}	

	if (0 != nNeedDotNum)	return RTN_STP;

	return RTN_NONE;
}
// ���ؾ���
short CCmdRun::RunWCRect(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	t_WCRectParam     lWclp;   // ���ػ��߲���
	::ZeroMemory(&lWclp,sizeof(t_WCRectParam));

	// ����λ��
	double dxA,dyA,dzA,duA;
	double dxB,dyB,dzB,duB;
	double dxC,dyC,dzC,duC;
	double dxD,dyD,dzD,duD;
	double dxE,dyE,dzE,duE;

	dxA = pCmd->pDblValue[0];
	dyA = pCmd->pDblValue[1];
	dzA = pCmd->pDblValue[2];
	duA = pCmd->pDblValue[3];
	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dxA,&dyA,&dzA,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dxA,&dyA,&dzA,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}
	dxB = pCmd->pDblValue[4];
	dyB = pCmd->pDblValue[5];
	dzB = pCmd->pDblValue[6];
	duB = pCmd->pDblValue[7];
	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dxB,&dyB,&dzB,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dxB,&dyB,&dzB,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}

	dxC = pCmd->pDblValue[8];
	dyC = pCmd->pDblValue[9];
	dzC = pCmd->pDblValue[10];
	duC = pCmd->pDblValue[11];
	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dxC,&dyC,&dzC,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dxC,&dyC,&dzC,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}
	dxD = pCmd->pDblValue[12];
	dyD = pCmd->pDblValue[13];
	dzD = pCmd->pDblValue[14];
	duD = pCmd->pDblValue[15];
	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dxD,&dyD,&dzD,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dxD,&dyD,&dzD,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}
	dxE = pCmd->pDblValue[16];
	dyE = pCmd->pDblValue[17];
	dzE = pCmd->pDblValue[18];
	duE = pCmd->pDblValue[19];
	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dxE,&dyE,&dzE,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dxE,&dyE,&dzE,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}
	lWclp.pos[0].x = dxA;
	lWclp.pos[0].y = dyA;
	lWclp.pos[0].z = dzA;
	lWclp.pos[0].a = duA;    

	lWclp.pos[1].x = dxB;
	lWclp.pos[1].y = dyB;
	lWclp.pos[1].z = dzB;
	lWclp.pos[1].a = duB;   

	lWclp.pos[2].x = dxC;
	lWclp.pos[2].y = dyC;
	lWclp.pos[2].z = dzC;
	lWclp.pos[2].a = duC;   

	lWclp.pos[3].x = dxD;
	lWclp.pos[3].y = dyD;
	lWclp.pos[3].z = dzD;
	lWclp.pos[3].a = duD;   

	lWclp.pos[4].x = dxE;
	lWclp.pos[4].y = dyE;
	lWclp.pos[4].z = dzE;
	lWclp.pos[4].a = duE;   

	// �����ٶ�
	lWclp.dVelXY    = m_cmdParam.dVelXY;   
	lWclp.dVelZ     = m_cmdParam.dVelZ;   

	//�����ٶ� 
	lWclp.dWorkVel  = pCmd->pDblValue[21]; //m_cmdParam.tLine.dVel;

	//���ط�
	lWclp.bOn = TRUE;

	//������ʱ��ʱ 
	lWclp.dPreTime=m_cmdParam.tLine.dPreTime; //s to ms


	lWclp.DispWegiht  = pCmd->pDblValue[20];      // ��������
	lWclp.dEndZupHigh = m_cmdParam.dZclearance;   // Z�������߶�
	lWclp.dZcleaType  = m_cmdParam.dZcleaType;    // Z�����߶�
	lWclp.dEndTime    = m_cmdParam.tLine.dTailTime;  // �㽺����ͣ��ʱ��

	/// ��β����
	lWclp.dEndVel		=	m_cmdParam.tLine.dEndVel;
	lWclp.dEndLen		=	m_cmdParam.tLine.dEndLen;
	lWclp.iEndType		=	m_cmdParam.tLine.iEndType;
	lWclp.dTailCloseLen	=	m_cmdParam.tLine.dTailCloseLen;

	if (pNextTrackCmd->tType==CMD_LINE_PASSING)
		lWclp.AddE_Trace=false;
	else
		lWclp.AddE_Trace=true;

	lWclp.AddS_Trace = RunWCL_addSTrack;

	//��������
	dcp[m_CurToolNo].bBit = lWclp.bOn;  // ����ʹ��
	dcp[m_CurToolNo].lPulseNum = pCmd->pIntValue[3];  // ��������
	dcp[m_CurToolNo].lowTime = m_cmdParam.nLowTime;   // �������� 

	//
	double disx,disy;
	double dis1,dis2,dis3,dis4,dis0;
	double t;

	disx= dxB-dxA ;
	disy = dyB-dyA ;
	dis1=sqrt((disx)*(disx)+(disy)*(disy));    // �߶γ�   mm

	disx= dxC-dxB ;
	disy = dyC-dyB ;
	dis2=sqrt((disx)*(disx)+(disy)*(disy));    // �߶γ�   mm

	disx= dxD-dxC ;
	disy = dyD-dyC ;
	dis3=sqrt((disx)*(disx)+(disy)*(disy));    // �߶γ�   mm

	disx= dxE-dxD ;
	disy = dyE-dyD ;
	dis4=sqrt((disx)*(disx)+(disy)*(disy));    // �߶γ�   mm

	dis0 = dis1+dis2+dis3+dis4;  // ���α߳�

	t= dis0 / lWclp.dWorkVel;             // ��Ҫ��ʱ��    s
	t= 1000*t;                            // ms

	int tdis;
	tdis = (int)( t / (int)pCmd->pIntValue[3]);  // ÿ�����ʱ���� ms
	tdis -= m_cmdParam.nLowTime;  // ѭ�����ڼ��

	dcp[m_CurToolNo].highTime = tdis; // �������ڼ��

	short sTemp=m_pController->TS_ListWCRect(CRD1,&dcp[m_CurToolNo],&lWclp);
	return sTemp;
}
// ���ػ���
short CCmdRun::RunWCLine(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	t_WCLParam     lWclp;   // ���ػ��߲���
	::ZeroMemory(&lWclp,sizeof(t_WCLParam));

	// ����λ��
	double dxA,dyA,dzA,duA;
	double dxB,dyB,dzB,duB;

	dxA = pCmd->pDblValue[0];
	dyA = pCmd->pDblValue[1];
	dzA = pCmd->pDblValue[2];
	duA = pCmd->pDblValue[3];
	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dxA,&dyA,&dzA,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dxA,&dyA,&dzA,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}
	dxB = pCmd->pDblValue[4];
	dyB = pCmd->pDblValue[5];
	dzB = pCmd->pDblValue[6];
	duB = pCmd->pDblValue[7];
	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dxB,&dyB,&dzB,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dxB,&dyB,&dzB,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}

	lWclp.pos[0].x = dxA;
	lWclp.pos[0].y = dyA;
	lWclp.pos[0].z = dzA;
	lWclp.pos[0].a = duA;    

	lWclp.pos[1].x = dxB;
	lWclp.pos[1].y = dyB;
	lWclp.pos[1].z = dzB;
	lWclp.pos[1].a = duB;   

	// �����ٶ�
	lWclp.dVelXY    = m_cmdParam.dVelXY;   
	lWclp.dVelZ     = m_cmdParam.dVelZ;   

	//�����ٶ� 
	lWclp.dWorkVel  = pCmd->pDblValue[9];//m_cmdParam.tLine.dVel;

	//���ط�
	lWclp.bOn = TRUE;

	//������ʱ��ʱ 
	lWclp.dPreTime=m_cmdParam.tLine.dPreTime; //s to ms


	lWclp.DispWegiht  = pCmd->pDblValue[8];      // ��������
	lWclp.dEndZupHigh = m_cmdParam.dZclearance;  // Z�������߶�
	lWclp.dZcleaType  = m_cmdParam.dZcleaType;   // Z�����߶�
	lWclp.dEndTime    = m_cmdParam.tLine.dTailTime;  // �㽺����ͣ��ʱ��

	///��β����
	lWclp.dEndVel		=	m_cmdParam.tLine.dEndVel;
	lWclp.dEndLen		=	m_cmdParam.tLine.dEndLen;
	lWclp.iEndType		=	m_cmdParam.tLine.iEndType;
	lWclp.dTailCloseLen	=	m_cmdParam.tLine.dTailCloseLen;

	if (pNextTrackCmd->tType==CMD_LINE_PASSING)
		lWclp.AddE_Trace=false;
	else
		lWclp.AddE_Trace=true;

	lWclp.AddS_Trace = RunWCL_addSTrack;

	//��������
	dcp[m_CurToolNo].bBit = lWclp.bOn;
	dcp[m_CurToolNo].lPulseNum = pCmd->pIntValue[3];  // ��������
	dcp[m_CurToolNo].lowTime = m_cmdParam.nLowTime;   // �������� 

	//
	double disx = dxB-dxA ;
	double disy = dyB-dyA ;
	double dis=sqrt((disx)*(disx)+(disy)*(disy));// �߶γ�   mm
	double t = dis / lWclp.dWorkVel;             // ��Ҫ��ʱ��    s
	t= 1000*t;                            // ms

	int tdis = (int)( t / (int)pCmd->pIntValue[3]);  // ÿ�����ʱ���� ms
	tdis -= m_cmdParam.nLowTime;  // ѭ�����ڼ��

	dcp[m_CurToolNo].highTime = tdis; // �������ڼ��

	short sTemp=m_pController->TS_ListWCLine(CRD1,&dcp[m_CurToolNo],&lWclp);
	return sTemp;
}

//  ���ش��
short CCmdRun::RunWCPot(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	UNREFERENCED_PARAMETER(pNextTrackCmd);

	t_Point pdp;
	::ZeroMemory(&pdp,sizeof(t_Point));

	double dx,dy,dz;
	dx = pCmd->pDblValue[0];
	dy = pCmd->pDblValue[1];
	dz = pCmd->pDblValue[2];

	if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
		(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
	{
		if(GetCmdPositionEX(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY,m_MarkGropID)==FALSE) 
			return RTN_DATA_OVER;
	}else
	{
		if(GetCmdPosition(pCmd,&dx,&dy,&dz,dOffsetX,dOffsetY)==FALSE) 
			return RTN_DATA_OVER;
	}

	pdp.dVelXY = m_cmdParam.dVelXY;                // �ٶ�
	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dZcleaType = m_cmdParam.dZcleaType;  
	pdp.dZclearance = m_cmdParam.dZclearance; 

	pdp.dZupHVel = m_cmdParam.tPoint.dZupHVel;
	pdp.dZupLVel = m_cmdParam.tPoint.dZupLVel;

	g_pFrm->m_Precision.Measure2Real(dy);

	pdp.pos.x = dx;   // λ��
	pdp.pos.y = dy;
	pdp.pos.z = dz;
	pdp.pos.a = 0;

	pdp.dPreTime = m_cmdParam.tPoint.dPreTime;    // ǰ��ʱ��
	pdp.dZuphigh = m_cmdParam.tPoint.dZupHigh;    // Z̧�߸߶�
	pdp.dNodeTime = m_cmdParam.tPoint.dTailTime;  // �㽺������ͣʱ��

	//���ط�
	if(pCmd->pIntValue[1]>0) 
		pdp.bOn = TRUE;
	else 
		pdp.bOn = FALSE;

	int nDispNum;
	nDispNum=(int) (pCmd->pDblValue[4] / g_pFrm->m_pSysParam->dBCMGPorDOT);   // ���������

	//������ʱ            
	pdp.dDispTime = (pCmd->pIntValue[4])*(m_cmdParam.nLowTime + m_cmdParam.nHighTime) / 1000.;//��ʱ��ʱʱ��

	//dcp.bBitEnable	= TRUE;
	//m_CurToolNo = pCmd->pIntValue[0];

	dcp[m_CurToolNo].bBit =       pdp.bOn;  
	dcp[m_CurToolNo].lowTime =    m_cmdParam.nLowTime;  // ����ʱ��
	dcp[m_CurToolNo].highTime =   m_cmdParam.nHighTime; // �������
	dcp[m_CurToolNo].lPulseNum =  pCmd->pIntValue[4];   // ����

	short sTemp;
	sTemp = m_pController->TS_ListPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] ��ID
	return RTN_NONE;
}
short CCmdRun::RunRepeatZMeter(unsigned char type,tgCmdLine *pCmd,double dOffsetX,double dOffsetY)
{
	unsigned char ucPath,StoL = 1;
	//int uiCount = 0;
	double offx,offy;
	long numx,numy,num,lcx,lcy;
	short sTemp;
	tgZMeter tZMeterTemp;

	/*������*/
	//pCmd->pDblValue[0]//x����
	//pCmd->pDblValue[1]//y����
	//pCmd->pIntValue[0]//��ʼ��ַ
	//pCmd->pIntValue[1]//1:S,2:N
	//pCmd->pIntValue[2]//X��Ŀ
	//pCmd->pIntValue[3]//Y��Ŀ

	double dMyTempX  = pCmd->pDblValue[0];
	DOUBLE dMyTempY  = pCmd->pDblValue[1];

	double m_dCurTempH;

	/////////////////////////////////
	numx = pCmd->pIntValue[2];//X��Ŀ
	numy = pCmd->pIntValue[3];//Y��Ŀ
	ucPath = (unsigned char)pCmd->pIntValue[1];////1:S,2:N

	if(type == 1)
	{//repeat x
		lcx = 1;
		lcy = 0;
	}
	else 
	{//repeat y
		lcx = 0;
		lcy = 1;
	}

	m_ZMeterVector.clear();
	m_dFirstHeightOffset = 0;

	//////////////////////
	m_dCurTempH = 0;
	sTemp = RunZHeight(pCmd,dOffsetX,dOffsetY,&m_dCurTempH);
	if(sTemp == RTN_PARAMETER_ERR) return RTN_PARAMETER_ERR;

	m_dFirstHeightOffset = m_dCurTempH;

	tZMeterTemp.index = 0;
	tZMeterTemp.delta = m_dCurTempH;
	m_ZMeterVector.push_back(tZMeterTemp);

	/////////////////////////
	for(num = numx*numy-1;num>0;num--)
	{
		tZMeterTemp.index = numx*numy-num;

		/////////////////////
		if(type == 1)
		{//repeat x
			offx = pCmd->pDblValue[6] * lcx;
			offy = pCmd->pDblValue[7] * lcy;
			if(ucPath == 0)
			{// S
				if(StoL) lcx ++;
				else lcx --;
				if(lcx == numx)
				{
					StoL = 0;
					lcx = numx - 1;
					lcy ++;
				}
				else if(lcx == -1)
				{
					StoL = 1;
					lcx = 0;
					lcy ++;
				}
			}
			else
			{//N
				lcx ++;
				if(lcx >= numx)
				{
					lcx = 0;
					lcy ++;
				}
			}
		}
		else //repeat y
		{
			offx = pCmd->pDblValue[6] * lcx;
			offy = pCmd->pDblValue[7] * lcy;
			if(ucPath == 0)
			{// S
				if(StoL) lcy ++;
				else lcy --;
				if(lcy == numy)
				{
					StoL = 0;
					lcy = numy - 1;
					lcx ++;
				}
				else if(lcy == -1)
				{
					StoL = 1;
					lcy = 0;
					lcx ++;
				}
			}
			else
			{//N
				lcy ++;
				if(lcy >= numy)
				{
					lcy = 0;
					lcx ++;
				}
			}
		}

		pCmd->pDblValue[0] = dMyTempX + offx;
		pCmd->pDblValue[1] = dMyTempY + offy;		

		m_dCurTempH = 0;
		sTemp = RunZHeight(pCmd,dOffsetX,dOffsetY,&m_dCurTempH);
		if(sTemp == RTN_PARAMETER_ERR) return RTN_PARAMETER_ERR;

		tZMeterTemp.delta = m_dCurTempH;
		m_ZMeterVector.push_back(tZMeterTemp);
	}	
	return RTN_NONE;
}
// ����ִ��
short CCmdRun::RunStep(int *pIndex,double dOffsetX,double dOffsetY)
{
	short sTemp = RTN_NONE;
	int uiCount= *pIndex;;

	tgCmdLine pCmd0;//=NULL;
	tgCmdLine pCmd1;//=NULL;
	tgCmdLine pCmd2;//=NULL;

	tgCmdLine tPreTrackCmd;
	tgCmdLine tNextTrackCmd;

	CARTESIAN_POINT pos;
	::ZeroMemory(&pos,sizeof(pos));

	CString strTemp;
	strTemp.Format("step %d ",uiCount);
	g_pFrm->AddMsg(strTemp);


	//ȷ������ǰ����ͣ����
	sTemp = m_pController->AxisStop(0xFF);
	if(RTN_NONE!=sTemp) return RTN_NONE;

	for(unsigned short p=0;p<m_pController->g_ExMtnPara->axisNum;p++)
	{//�����λ��״̬
		sTemp = m_pController->ClrAxisSts(p+1);
		if(RTN_NONE!=sTemp) return RTN_NONE;
	}

	sTemp = m_pController->TS_ListInit(CRD1,1);
	if(RTN_NONE!=sTemp) return RTN_NONE;

	m_pController->g_ExMtnPara->dfBakpos[X_AXIS] = m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	m_pController->g_ExMtnPara->dfBakpos[Y_AXIS] = m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	m_pController->g_ExMtnPara->dfBakpos[Z_AXIS] = m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
	//////////////////////////////////////////////////////////////////////////
	if(uiCount>m_cmdRunList.GetCount()-1)
		return RTN_PARAMETER_ERR;

	m_iCurIndex = uiCount;
	//////////////////////////////////////////////////////////////////////////
	pCmd0.tType = CMD_NONE;

	if(uiCount==0)
	{
		pCmd0.tType = CMD_NONE;
		if(GetRecord(pCmd1,0)) return RTN_PARAMETER_ERR;
		if(GetRecord(pCmd2,1)) return RTN_PARAMETER_ERR;
	}
	else 
	{
		if(GetRecord(pCmd0,uiCount-1)) return RTN_PARAMETER_ERR;
		if(GetRecord(pCmd1,uiCount)) return RTN_PARAMETER_ERR;
		if(GetRecord(pCmd2,uiCount+1)) return RTN_PARAMETER_ERR;
	}	

	//��ȡǰ���˶�λ�õ�				
	//GetPreLineDot(uiCount, tPreTrackCmd);
	//GetNextLineDot(uiCount, tNextTrackCmd);
	GetPreDot(uiCount, tPreTrackCmd);
	GetNextDot(uiCount, tNextTrackCmd);

	g_pFrm->AddMsg(szCmdline[pCmd1.tType]);

	switch(pCmd1.tType)
	{
	case CMD_DUMMY:
	case CMD_POINT:				
	case CMD_LINE_START:			
	case CMD_LINE_PASSING:			
	case CMD_LINE_ARC:				
	case CMD_CIRCLE:				
	case CMD_LINE_END:
	case CMD_TOOL:
	case CMD_DISPENSER:				
	case CMD_GOTO_ADDRESS:
	case CMD_BRUSH_AREA:
	case CMD_SET_IO:
	case CMD_DELAY:
	case CMD_PAUSE:
	case CMD_HOME:
	case CMD_LOOP:
	case CMD_ZMETER:
	case CMD_REPEATX_ZH:
	case CMD_REPEATY_ZH:
	case CMD_DUNNY_SPEED:
	case PARAM_LINE_SPEED:
	case PARAM_LINE_SETUP:
	case PARAM_LINE_END:
	case PARAM_POINT_SETUP:
	case PARAM_JET_SETUP:
	case PARAM_POINT_END:
	case PARAM_Z_CLEARANCE:
	case PARAM_Z_WORKPOSSET:
	case PARAM_Z_SET_HEIGHT:
	case PARAM_XY_VEL:
	case PARAM_Z_VEL:
		sTemp = RunAdd(&uiCount,&pCmd0,&pCmd1,&pCmd2,&tPreTrackCmd,&tNextTrackCmd,dOffsetX,dOffsetY);
		if(sTemp == RTN_STP) return RTN_STP;
		if(sTemp == RTN_ESTP) return RTN_ESTP;
		if(sTemp == RTN_GTS_ERROR) return RTN_GTS_ERROR;
		if(sTemp == RTN_TIMEOUT) return RTN_TIMEOUT;

		g_pFrm->m_wndEditBar.SetCmdCurSel(uiCount);			
		break;
	case CMD_MARK://Mark����
		sTemp=m_pController->TS_ListWaitStop();
		if (sTemp!=RTN_NONE) return sTemp;

		if(!FindMark(&pCmd1))
		{
			if(m_tSysStatus==K_RUN_STS_RUN)
			{
				g_pFrm->AddMsg("û���ҵ�MARK�㣬��λ����");
				//GotoSafeZpos();
				//m_pController->AxisMove(Z_AXIS,0,200,g_pFrm->m_mtrParam[Z_AXIS].m_Acc);
			}
			return RTN_PARAMETER_ERR;
		}
		m_pController->TS_ListInit(CRD1,1);
		break;

	case CMD_CALL_SUB:
		/*
		for (int j=uiCount;j<m_cmdRunList.GetCount();j++)
		{
		if(m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).tType == CMD_LABEL && (CString)m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).pszNote == (CString)pCmd->pszNote)
		{
		iLabelStart = j;
		}
		}
		for(int j=iLabelStart;j<m_cmdRunList.GetCount();j++)
		{
		if(m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).tType == CMD_END_PROGRAM)
		iLabelEnd = j;
		}
		for(int j=iLabelStart;j<iLabelEnd;j++)
		{
		if(g_pFrm->m_wndEditBar.IsTrackPoint(m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)),FALSE))
		{
		dOffX =pCmd->pDblValue[0]-m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).pDblValue[0];
		dOffY =pCmd->pDblValue[1]-m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).pDblValue[1];
		break;
		}
		}
		RunTrack(iLabelStart,iLabelEnd,dOffX,dOffY,FALSE);*/
		break;
		//case CMD_CALL_PRO:
		//	break;;
	case CMD_END_PROGRAM:
		for(int t=0;t<m_pController->g_ExMtnPara->axisNum;t++)
			m_GotoOffSet[t]=0.0;
		break;
	default:break;
	}

	sTemp=m_pController->TS_ListWaitStop();
	if(sTemp!=RTN_NONE) return sTemp;

	return RTN_NONE;
}
/***********************************/
/*����˵�����Զ�����ִ�к���
/*����˵���� int iStartIndex, ��ʼ�ӹ��к�
/*            int iEndIndex,   ��ֹ�ӹ��к�
/*            double dOffsetX, 
/*            double dOffsetY,
/*            double dOffsetZ
/***********************************/
short CCmdRun::RunSub(int iStartIndex, int iEndIndex, double dOffsetX,double dOffsetY,double dOffsetZ)
{
	tgCmdLine pPreCmd;   // ��һ��ָ��
	tgCmdLine pCurCmd;   // ��ǰָ��
	tgCmdLine pNextCmd;   // ��һ��ָ��
	tgCmdLine tPreTrackCmd;  // ��ǰָ�����һ�����˶���Ϣ��ָ��
	tgCmdLine tNextTrackCmd; // ��ǰָ�����һ�����˶���Ϣ��ָ��

	//	unsigned int SubNoBuff[100];      // �����ӳ���ϵ�λ��
	//	unsigned int SubNoIndex = 0;      // �ӳ����
	//	char         ProgOrSub[200];      // 0:Prog  1:Sub

//	bool    progEnd = true;           // �Ƿ��һ�ε��������
	short   sRtnTemp  = RTN_NONE;     // ִ�з���״̬
	PoaIndex=0;               // �ӳ������������

	// ��ʱ����
	unsigned int uiTemp = 0;         
	CString      strMsgOut;         // ��Ϣ�����ʱ�� 

	// �����кż��
	if(iStartIndex >= iEndIndex)
		return RTN_NONE;

	// �ӹ���Ϣ���
	strMsgOut.Format("From %d to %d",iStartIndex,iEndIndex - 1);
	g_pFrm->AddMsg(strMsgOut);

	//�����״̬
	for(unsigned short p=0;p<m_pController->g_ExMtnPara->axisNum;p++)
	{
		sRtnTemp = m_pController->ClrAxisSts(p+1);
		if(RTN_NONE!=sRtnTemp) 
			return RTN_NONE;
	}

	// ��ʼ��������  ����z��̧�߶�����ӵ�������
	sRtnTemp = m_pController->TS_ListInit(CRD1,1);  
	if(RTN_NONE!=sRtnTemp)
		return RTN_NONE;

	// ��ʼ����תƫ��
	for(int t=0;t<m_pController->g_ExMtnPara->axisNum;t++)
		m_GotoOffSet[t]=0.0;

	// ǰһ��ָ��Z��λ������
	m_pController->g_ExMtnPara->dfBakpos[Z_AXIS] = g_pFrm->m_pRobotParam->m_zSafePos;

	pPreCmd.tType = CMD_NONE;   // ��ʼʱ����һ��ָ��ΪNULL

	LoopAddCount = -20;       // ǰհ����

	// Ԥ������
	if(GetRecord(pCurCmd,iStartIndex))     
		return RTN_PARAMETER_ERR;      

	if(GetRecord(pNextCmd,iStartIndex+1))
		return RTN_PARAMETER_ERR;

	//����ʼ�ӹ�λ���˶����������λ��
	for(int uiCount=iStartIndex; uiCount<iEndIndex; uiCount++)
	{
		m_iCurIndex = uiCount;  // ��¼��ǰִ��ָ���к�

		//��ȡǰ���˶�λ�õ�	   			
		GetPreDot(uiCount, tPreTrackCmd);
		GetNextDot(uiCount, tNextTrackCmd);
		// �����ǰָ����Ϣ
		g_pFrm->AddMsg(szCmdline[pCurCmd.tType]);

		switch(pCurCmd.tType)
		{
		case CMD_DUMMY:
		case CMD_POINT:				
		case CMD_LINE_START:			
		case CMD_LINE_PASSING:			
		case CMD_LINE_ARC:				
		case CMD_CIRCLE:				
		case CMD_LINE_END:				
		case CMD_DISPENSER:
		case CMD_TOOL:
		case CMD_WAIT_POINT:
		case CMD_BATH:
		case CMD_ZMETER:
		case CMD_REPEATX_ZH:
		case CMD_REPEATY_ZH:
		case CMD_GOTO_ADDRESS:
		case CMD_BRUSH_AREA:
		case CMD_SET_IO:
		case CMD_DELAY:
		case CMD_PAUSE:
		case CMD_HOME:
		case CMD_LOOP:		
		case CMD_DUNNY_SPEED:
		case PARAM_LINE_SPEED:
		case PARAM_LINE_SETUP:
		case PARAM_LINE_END:
		case PARAM_POINT_SETUP:
		case PARAM_JET_SETUP:
		case PARAM_POINT_END:
		case PARAM_Z_CLEARANCE:
		case PARAM_Z_WORKPOSSET:
		case PARAM_Z_SET_HEIGHT:
		case PARAM_XY_VEL:
		case PARAM_Z_VEL:
		case CMD_POT:
		case CMD_WCL:
		case CMD_WCRECT:
// 		case CMD_SUPEREND:
// 		case CMD_SUPERPOINT:
// 		case CMD_SUPERSTART:
			sRtnTemp = RunAdd(&uiCount,  
				&pPreCmd,&pCurCmd,&pNextCmd,
				&tPreTrackCmd,&tNextTrackCmd,
				dOffsetX,dOffsetY,dOffsetZ);
			if(sRtnTemp == RTN_STP)                return RTN_STP;
			else if(sRtnTemp == RTN_PARAMETER_ERR) return RTN_PARAMETER_ERR;
			else if(sRtnTemp == RTN_ESTP)          return RTN_ESTP;
			else if(sRtnTemp == RTN_GTS_ERROR)     return RTN_GTS_ERROR;
			else if(sRtnTemp == RTN_TIMEOUT)       return RTN_TIMEOUT;
			else if(sRtnTemp == RTN_ZHeight_Err)   return RTN_ZHeight_Err;
			else if(sRtnTemp == RTN_GOTO_ADD)      continue;
			else  break;
		case CMD_MARK:                 //  Mark����
			// �����˶����ȴ��˶�ֹͣ
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			g_pView->ImgStop();
			if(!FindMarkEx(&pCurCmd))
			{
				return RTN_MARK_ERROR;
			}
			// ���´�������ϵ
			sRtnTemp=m_pController->TS_ListInit(CRD1, 1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;
//			progEnd = true;
			break;


			//////////////////////////////////////////////////////////////////////////

// 			// �����˶����ȴ��˶�ֹͣ  
// 			sRtnTemp=m_pController->TS_ListWaitStop();
// 			if(sRtnTemp!=RTN_NONE) return sRtnTemp;
// 
// 			if((g_pFrm->m_pSysParam->nFileSeparaEnable == 1)||
// 				(g_pFrm->m_pSysParam->nFileScrapeEnable == 1))
// 			{
// 				if(!FindMarkEx(&pCurCmd))
// 				{
// 					return RTN_MARK_ERROR;
// 				}
// 			}
// 			else
// 			{
// 				if(!FindMark(&pCurCmd))
// 				{
// 					return RTN_MARK_ERROR;
// 				}
// 			}
// 
// 			// ���´�������ϵ
// 			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
// 			if(sRtnTemp!=RTN_NONE) 
// 				return sRtnTemp;
// 
// //			progEnd= true;
// 			break;

		case CMD_REPEATARC: 
			sRtnTemp = RunBoardArc(pCurCmd,dOffsetX,dOffsetY);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;
			break;
		case CMD_REPEATX: // X�������� 
			sRtnTemp = RunRepeat(1,/*uiCount,*/pCurCmd,dOffsetX,dOffsetY);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;
			break;
		case CMD_REPEATY: // Y��������
			sRtnTemp = RunRepeat(2,/*uiCount,*/pCurCmd,dOffsetX,dOffsetY);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;
			break;

		case CMD_CALL_SUB: // �����ӳ���
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			//��ȡ�Ӻ�����ʼ��ַ
			if(pCurCmd.pIntValue[0] == 1)  
			{   
				//1 ��� 
				uiTemp = SearchLabel(pCurCmd.pszNote);
				if(uiTemp == RTN_PARAMETER_ERR)  break;	
				pCurCmd.pIntValue[1] = uiTemp;  // ��Ŷ�Ӧ��ʼ��ַ
			}

			PoaIndex ++;//�ӳ����������	
			uiTemp = pCurCmd.pIntValue[1]; //��ַ���߱�� ��Ӧ��ַ

			// ��ȡ��һ�������������
			// ���������ƫ����ݵ�ǰλ�� pCmd1 ��ǰ����������
			GetGotoOffsetEx(uiTemp,pCurCmd); 	
			///////////////////////////////////////

			if(GetRecord(pCurCmd,uiTemp)) return RTN_PARAMETER_ERR;   //���ӳ��򴦿�ʼ ��ȡ����ָ��
			if(GetRecord(pNextCmd,uiTemp+1)) return RTN_PARAMETER_ERR;

			//���ӳ���ʼλ����Ϊ��ǰִ��λ��
			uiCount = uiTemp - 1;  			
			continue;	

		case CMD_FILESCRPEWORK:  // ƴ��ӹ���
			//�������ȴ��˶�ֹͣ
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			g_pFrm->AddMsg("ƴ��ӹ�");
			// ���´�������ϵ
			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;

			//���ӳ���ʼλ����Ϊ��ǰִ��λ��
			uiCount = pCurCmd.pIntValue[1]; 
			if(GetRecord(pCurCmd,uiCount))     
				return RTN_PARAMETER_ERR;      

			if(GetRecord(pNextCmd,uiCount+1))
				return RTN_PARAMETER_ERR;
//			progEnd= true;
			break;
		case CMD_FILEDELIMITER:  // �ֶη�
			//�������ȴ��˶�ֹͣ
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			// ���ͷֶν��������PLC
			ExOutput(_T("�ֶμӹ�����"),TRUE);
			CFunction::DelaySec(0.3);                   // s
			ExOutput(_T("�ֶμӹ�����"),FALSE);

			m_EvtAskSeparaRun.ResetEvent();
			::WaitForSingleObject(m_EvtAskSeparaRun,INFINITE);
			m_EvtAskSeparaRun.ResetEvent();

			g_pFrm->AddMsg("�ֶμӹ���ʼ");
			// ���´�������ϵ
			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;

			//���ӳ���ʼλ����Ϊ��ǰִ��λ��
			uiCount = pCurCmd.pIntValue[1]; 
			if(GetRecord(pCurCmd,uiCount))     
				return RTN_PARAMETER_ERR;      

			if(GetRecord(pNextCmd,uiCount+1))
				return RTN_PARAMETER_ERR;
//			progEnd= true;
			break;

		case CMD_END_PROGRAM:
			for(int t=0;t<m_pController->g_ExMtnPara->axisNum;t++)
				m_GotoOffSet[t]=0.0;

// 			if (progEnd)//��һ�ε�������
// 			{
				PoaIndex=0;
// 			}
// 			progEnd=false;

			// û�С�CMD_CALL_SUB��ָ��
			if (GetFindCallSub(uiCount+1+PoaIndex,pCurCmd)==-1)
			{ 
				// û����תָ��
				if(GetFindCallJump(uiCount+1+PoaIndex,pCurCmd)==-1)
				{
					// û�зֶ�ָ��
					if(GetFindCallFiledLimiter(uiCount+1+PoaIndex,pCurCmd)==-1)
					{
						// û��ƴ��ָ��
						if(GetFindCallFiledScorp(uiCount+1+PoaIndex,pCurCmd)==-1)
						{ 
							m_iCurIndex=0;
							//�����˶��ҵȴ�ֹͣ
							sRtnTemp=m_pController->TS_ListWaitStop();
							return sRtnTemp;
						}else
						{
							if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
								return RTN_PARAMETER_ERR;
						}
					}else  // �зֶ�ָ��
					{
						if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
							return RTN_PARAMETER_ERR;
					}
				}else  // ��תָ���¼���
				{
					// ��ȡ�Ӻ�����һ��ָ��
					if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
						return RTN_PARAMETER_ERR;
				}
			}
			else // ���Ӻ�������ָ��
			{   
				// ��ȡ�Ӻ�����һ��ָ��
				if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
					return RTN_PARAMETER_ERR;
				if(g_pFrm->m_pSysParam->nFileScrapeEnable == 1)
				{
					if(GetRecord(pNextCmd,uiCount+2+PoaIndex)) 
						return RTN_PARAMETER_ERR;

					uiCount = uiCount+PoaIndex+1;  

				}
			}
			continue;
		default:break;
		}
		///
		pPreCmd=pCurCmd;
		pCurCmd=pNextCmd;
		if(uiCount==(iEndIndex-2))	
			pNextCmd.tType = CMD_NONE;
		else if(uiCount<(iEndIndex-2))
		{
			if(GetRecord(pNextCmd,uiCount+2)) return RTN_PARAMETER_ERR;
		}
		////
	}
	return RTN_NONE;
}
short CCmdRun::RunBoardArc(tgCmdLine pCmd,double dOffsetX,double dOffsetY)
{
	UNREFERENCED_PARAMETER(dOffsetX);
	UNREFERENCED_PARAMETER(dOffsetY);
	// ����н�
	m_cmdParam.dBoardArcForRepeat =pCmd.pDblValue[0];

	return RTN_NONE;
}
/***********************************/
/*����˵�����ɽ�ģʽ�Զ�����ִ�к���
/*����˵���� int iStartIndex, ��ʼ�ӹ��к�
/*            int iEndIndex,   ��ֹ�ӹ��к�
/*            double dOffsetX, 
/*            double dOffsetY,
/*            double dOffsetZ
/***********************************/
short CCmdRun::RunSub_EX(int iStartIndex, int iEndIndex, double dOffsetX,double dOffsetY,double dOffsetZ)
{
	tgCmdLine pPreCmd;   // ��һ��ָ��
	tgCmdLine pCurCmd;   // ��ǰָ��
	tgCmdLine pNextCmd;   // ��һ��ָ��
	tgCmdLine tPreTrackCmd;  // ��ǰָ�����һ�����˶���Ϣ��ָ��
	tgCmdLine tNextTrackCmd; // ��ǰָ�����һ�����˶���Ϣ��ָ��

	bool    progEnd = true;           // �Ƿ��һ�ε��������
	short   sRtnTemp = RTN_NONE;      // ִ�з���״̬
	PoaIndex=0;                       // �ӳ������������

	// ��ʱ����
	unsigned int uiTemp = 0;         
	CString      strMsgOut;         // ��Ϣ�����ʱ�� 

	// �����кż��
	if(iStartIndex >= iEndIndex)
		return RTN_NONE;

	// �ӹ���Ϣ���
	strMsgOut.Format("From %d to %d",iStartIndex,iEndIndex - 1);
	g_pFrm->AddMsg(strMsgOut);

	//�����״̬
	for(unsigned short p=0;p<m_pController->g_ExMtnPara->axisNum;p++)
	{
		sRtnTemp = m_pController->ClrAxisSts(p+1);
		if(RTN_NONE!=sRtnTemp) 
			return RTN_NONE;
	}

	// ��ʼ��������  z��̧�߶�����ӵ�������
	sRtnTemp = m_pController->TS_ListInit(CRD1,1);  
	if(RTN_NONE!=sRtnTemp)
		return RTN_NONE;

	// ��ʼ����תƫ��
	for(int t=0;t<m_pController->g_ExMtnPara->axisNum;t++)
		m_GotoOffSet[t]=0.0;

	// ǰһ��ָ��Z��λ������
	m_pController->g_ExMtnPara->dfBakpos[Z_AXIS] = g_pFrm->m_pRobotParam->m_zSafePos;

	pPreCmd.tType = CMD_NONE;   // ��ʼʱ����һ��ָ��ΪNULL

	LoopAddCount = -20;       // ǰհ����

	m_cmdHComperList.RemoveAll(); 
	// Ԥ������
	if(GetRecord(pCurCmd,iStartIndex))     
		return RTN_PARAMETER_ERR;      

	if(GetRecord(pNextCmd,iStartIndex+1))
		return RTN_PARAMETER_ERR;

	//����ʼ�ӹ�λ���˶����������λ��
	for(int uiCount=iStartIndex; uiCount<iEndIndex; uiCount++) 
	{
		while (K_RUN_STS_PAUSE == m_tSysStatus)
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION, "��ͣ");
			Sleep(100);
		}

		m_iCurIndex = uiCount;  // ��¼��ǰִ��ָ���к�

		//��ȡǰ���˶�λ�õ�	   			
		GetPreDot(uiCount, tPreTrackCmd);
		GetNextDot(uiCount, tNextTrackCmd);
		// �����ǰָ����Ϣ
		g_pFrm->AddMsg(szCmdline[pCurCmd.tType]);
		switch(pCurCmd.tType)
		{
		case CMD_DUMMY:
		case CMD_POINT:
		case CMD_DISPENSER:
		case CMD_TOOL:
		case CMD_WAIT_POINT:
		case CMD_GOTO_ADDRESS:
		case CMD_SET_IO:
		case CMD_DELAY:
		case CMD_PAUSE:
		case CMD_HOME:
		case CMD_LOOP:			
		case CMD_CIRCLE:		
		case CMD_DUNNY_SPEED:
		case PARAM_LINE_SPEED:
		case PARAM_LINE_SETUP:
		case PARAM_LINE_END:
		case PARAM_POINT_SETUP:
		case PARAM_JET_SETUP:
		case PARAM_POINT_END:
		case PARAM_Z_CLEARANCE:
		case PARAM_Z_WORKPOSSET:
		case PARAM_Z_SET_HEIGHT:
		case PARAM_XY_VEL:
		case PARAM_Z_VEL:
		case CMD_POT:        // ���ش��
		case CMD_LINE_ARC:
		case CMD_ZMETER:
		case CMD_SUPEREND:
		case CMD_SUPERPOINT:
		case CMD_SUPERSTART:
		case CMD_SUPERDOT:
			sRtnTemp = RunAdd_EX(&uiCount,  
				&pPreCmd,&pCurCmd,&pNextCmd,
				&tPreTrackCmd,&tNextTrackCmd,
				dOffsetX,dOffsetY,dOffsetZ);
			if(sRtnTemp == RTN_STP)      				return RTN_STP;
			else if(sRtnTemp == RTN_PARAMETER_ERR)		return RTN_PARAMETER_ERR;
			else if(sRtnTemp == RTN_ESTP)         		return RTN_ESTP;
			else if(sRtnTemp == RTN_GTS_ERROR)     		return RTN_GTS_ERROR;
			else if(sRtnTemp == RTN_TIMEOUT)      		return RTN_TIMEOUT;
			else if(sRtnTemp == RTN_ZHeight_Err)      	return RTN_ZHeight_Err;
			else if(sRtnTemp == RTN_GOTO_ADD)     		continue;
			else  
				break;

		case CMD_REPEATX: // X�������� 
			sRtnTemp = RunRepeat(1,/*uiCount,*/pCurCmd,dOffsetX,dOffsetY);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;
			break;
		case CMD_REPEATY: // Y��������
			sRtnTemp = RunRepeat(2,/*uiCount,*/pCurCmd,dOffsetX,dOffsetY);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;
			break;

		case CMD_ReverseBoard:
			// �����˶����ȴ��˶�ֹͣ
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			{	
				m_pController->AxisMove(Z_AXIS,g_pFrm->m_pRobotParam->m_zSafePos, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
			
				g_pFrm->m_Precision.LineMoveXY(CRD1,0,g_pFrm->m_pSysParam->tDefHomePos.x,
					g_pFrm->m_pSysParam->tDefHomePos.y,g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax,
					g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			}
			//////////////////////////////////////////////////////////////////////////
			// ���嶯��
			g_pFrm->m_Robot->GotoSafeZpos(FALSE);

			sRtnTemp = (short)ReverseBoard();
			if (1 != sRtnTemp)
			{
				return RTN_Reverse_Err;
			}

			// ���´�������ϵ
			sRtnTemp=m_pController->TS_ListInit(CRD1, 1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;
			break;

		case CMD_MARK:         //  Mark����
			// �����˶����ȴ��˶�ֹͣ
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			g_pView->ImgStop();
			if(!FindMarkEx(&pCurCmd))
			{
				return RTN_MARK_ERROR;
			}
			// ���´�������ϵ
			sRtnTemp=m_pController->TS_ListInit(CRD1, 1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;

			progEnd = true;
			break;

		case CMD_CALL_SUB: // �����ӳ���
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			//��ȡ�Ӻ�����ʼ��ַ
			if(pCurCmd.pIntValue[0] == 1)  
			{   
				//1 ��� 
				uiTemp = SearchLabel(pCurCmd.pszNote);
				if(uiTemp == RTN_PARAMETER_ERR)  break;	
				pCurCmd.pIntValue[1] = uiTemp;  // ��Ŷ�Ӧ��ʼ��ַ
			}

			PoaIndex ++;//�ӳ����������	
			uiTemp = pCurCmd.pIntValue[1]; //��ַ���߱�� ��Ӧ��ַ

			// ��ȡ��һ�������������
			// ���������ƫ����ݵ�ǰλ�� pCmd1 ��ǰ����������
			GetGotoOffsetEx(uiTemp,pCurCmd); 	
			///////////////////////////////////////

			if(GetRecord(pCurCmd,uiTemp)) return RTN_PARAMETER_ERR;   //���ӳ��򴦿�ʼ ��ȡ����ָ��
			if(GetRecord(pNextCmd,uiTemp+1)) return RTN_PARAMETER_ERR;

			//���ӳ���ʼλ����Ϊ��ǰִ��λ��
			uiCount = uiTemp - 1;  			
			continue;	

		case CMD_FILEDELIMITER:  // �ֶη�
			//�������ȴ��˶�ֹͣ
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			// ���ͷֶν��������PLC
			ExOutput(_T("�ֶμӹ�����"),TRUE);
			CFunction::DelaySec(0.3);                   // s
			ExOutput(_T("�ֶμӹ�����"),FALSE);

			m_EvtAskSeparaRun.ResetEvent();
			::WaitForSingleObject(m_EvtAskSeparaRun,INFINITE);
			m_EvtAskSeparaRun.ResetEvent();

			g_pFrm->AddMsg("�ֶμӹ���ʼ");
			// ���´�������ϵ
			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;

			//���ӳ���ʼλ����Ϊ��ǰִ��λ��
			uiCount = pCurCmd.pIntValue[1]; 
			if(GetRecord(pCurCmd,uiCount))     
				return RTN_PARAMETER_ERR;      

			if(GetRecord(pNextCmd,uiCount+1))
				return RTN_PARAMETER_ERR;
			progEnd= true;
			break;	
		case CMD_FILESCRPEWORK:  // ƴ��ӹ���
			//�������ȴ��˶�ֹͣ
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			g_pFrm->AddMsg("ƴ��ӹ�");
			// ���´�������ϵ
			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;

			//���ӳ���ʼλ����Ϊ��ǰִ��λ��
			uiCount = pCurCmd.pIntValue[1]; 
			if(GetRecord(pCurCmd,uiCount))     
				return RTN_PARAMETER_ERR;      

			if(GetRecord(pNextCmd,uiCount+1))
				return RTN_PARAMETER_ERR;
			progEnd= true;
			break;
		case CMD_END_PROGRAM:
			for(int t=0;t<m_pController->g_ExMtnPara->axisNum;t++)
				m_GotoOffSet[t]=0.0;

			if (progEnd)//��һ�ε�������
			{
				PoaIndex=0;
			}
			progEnd=false;

			// �Ƿ��С�CMD_CALL_SUB��ָ��
			if (GetFindCallSub(uiCount+1+PoaIndex,pCurCmd)==-1)
			{  
				if(GetFindCallJump(uiCount+1+PoaIndex,pCurCmd)==-1)
				{
					if(GetFindCallFiledLimiter(uiCount+1+PoaIndex,pCurCmd)==-1)
					{
						// û��ƴ��ָ��
						if(GetFindCallFiledScorp(uiCount+1+PoaIndex,pCurCmd)==-1)
						{ 
							m_iCurIndex=0;
							//�����˶��ҵȴ�ֹͣ
							sRtnTemp=m_pController->TS_ListWaitStop();
							return sRtnTemp;
						}else
						{
							if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
								return RTN_PARAMETER_ERR;
						}
					}else
					{
						if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
							return RTN_PARAMETER_ERR;
					}
				}else
				{
					// ��ȡ�Ӻ�����һ��ָ��
					if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
						return RTN_PARAMETER_ERR;
				}
			}
			else
			{
				// ��ȡ�Ӻ�����һ��ָ��
				if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
					return RTN_PARAMETER_ERR;
				if(g_pFrm->m_pSysParam->nFileScrapeEnable == 1)
				{
					if(GetRecord(pNextCmd,uiCount+2+PoaIndex)) 
						return RTN_PARAMETER_ERR;

					uiCount = uiCount+PoaIndex;  

				}
			}
			continue;
		default:break;
		}
		///
		pPreCmd=pCurCmd;
		pCurCmd=pNextCmd;
		if(uiCount==(iEndIndex-2))	
			pNextCmd.tType = CMD_NONE;
		else if(uiCount<(iEndIndex-2))
		{
			if(GetRecord(pNextCmd,uiCount+2)) return RTN_PARAMETER_ERR;
		}
		////
	}
	return RTN_NONE;
}
/************************************************************************/
/*��ȡ��ǰ����һ�����˶���Ϣ��ָ��                                                            */
/************************************************************************/
int CCmdRun::GetPreDot(int iCurIndex, tgCmdLine &tDot)
{
	for(int i=iCurIndex-1; i>=0 ;i--)
	{
		if(i>m_cmdRunList.GetCount()-1)break;

		POSITION pos=m_cmdRunList.FindIndex(i);
		CMD_TYPE ListType=m_cmdRunList.GetAt(pos).tType;

		if(ListType == CMD_DUMMY			|| ListType == CMD_POINT
			|| ListType == CMD_LINE_PASSING	|| ListType == CMD_LINE_START
			|| ListType == CMD_LINE_ARC		|| ListType == CMD_LINE_END
			|| ListType == CMD_CIRCLE		|| ListType == CMD_BRUSH_AREA
			|| ListType == CMD_WAIT_POINT	|| ListType == CMD_WCL
			|| ListType == CMD_WCRECT		|| ListType == CMD_POT
			|| ListType == CMD_SUPERSTART   //�ɽ����
			|| ListType == CMD_SUPERPOINT   //�ɽ�����
			|| ListType == CMD_SUPEREND    )//�ɽ�������
		{
			tDot = m_cmdRunList.GetAt(pos);
			return i;
		}
	}
	tDot.Clear();
	return -1;
}
int CCmdRun::GetNextDot(int iCurIndex, tgCmdLine &tDot)
{
	for(int i=iCurIndex+1; i<m_cmdRunList.GetSize() ;i++)
	{
		POSITION pos=m_cmdRunList.FindIndex(i);
		CMD_TYPE ListType=m_cmdRunList.GetAt(pos).tType;

		if(ListType == CMD_DUMMY			|| ListType == CMD_POINT
			|| ListType == CMD_LINE_PASSING	|| ListType == CMD_LINE_START
			|| ListType == CMD_LINE_ARC		|| ListType == CMD_LINE_END
			|| ListType == CMD_CIRCLE		|| ListType == CMD_BRUSH_AREA
			|| ListType == CMD_WAIT_POINT	|| ListType == CMD_WCL
			|| ListType == CMD_WCRECT		|| ListType == CMD_POT
			|| ListType == CMD_SUPERSTART	|| ListType == CMD_SUPERDOT
			|| ListType == CMD_SUPERPOINT   || ListType == CMD_SUPEREND)
		{
			tDot = m_cmdRunList.GetAt(pos);
			return i;
		}
	}
	tDot.Clear();
	return -1;
}
int CCmdRun::GetFindCallSub(int iCurIndex, tgCmdLine &tDot)//���ӳ���ָ��
{
	UNREFERENCED_PARAMETER(tDot);
	for(int i=iCurIndex; i<m_cmdRunList.GetSize() ;i++)
	{
		POSITION pos=m_cmdRunList.FindIndex(i);
		CMD_TYPE ListType=m_cmdRunList.GetAt(pos).tType;
		if (ListType == CMD_CALL_SUB)
		{
			return 1;
		}	
	}
	//tDot.Clear();
	return -1;
}
int CCmdRun::GetFindCallJump(int iCurIndex, tgCmdLine &tDot) //����תָ��
{
	UNREFERENCED_PARAMETER(tDot);
	for(int i=iCurIndex; i<m_cmdRunList.GetSize() ;i++)
	{
		POSITION pos=m_cmdRunList.FindIndex(i);
		CMD_TYPE ListType=m_cmdRunList.GetAt(pos).tType;
		if (ListType == CMD_GOTO_ADDRESS)
		{
			return 1;
		}	
	}
	//tDot.Clear();
	return -1;
}

int CCmdRun::GetFindCallFiledLimiter(int iCurIndex, tgCmdLine &tDot) //�зֶ�ָ��
{
	UNREFERENCED_PARAMETER(tDot);
	for(int i=iCurIndex; i<m_cmdRunList.GetSize() ;i++)
	{
		POSITION pos=m_cmdRunList.FindIndex(i);
		CMD_TYPE ListType=m_cmdRunList.GetAt(pos).tType;
		if (ListType == CMD_FILEDELIMITER)
		{
			return 1;
		}	
	}
	//tDot.Clear();
	return -1;
}

int CCmdRun::GetFindCallFiledScorp(int iCurIndex, tgCmdLine &tDot) //��ƴ��ָ��
{
	UNREFERENCED_PARAMETER(tDot);
	for(int i=iCurIndex; i<m_cmdRunList.GetSize() ;i++)
	{
		POSITION pos=m_cmdRunList.FindIndex(i);
		CMD_TYPE ListType=m_cmdRunList.GetAt(pos).tType;
		if (ListType == CMD_FILESCRPEWORK)
		{
			return 1;
		}	
	}
	//tDot.Clear();
	return -1;
}
int CCmdRun::GetPreLineDot(int iCurIndex, tgCmdLine &tDot)
{
	for(int i=iCurIndex-1; i>=0 ;i--)
	{
		if(i>m_cmdRunList.GetCount()-1)break;

		POSITION pos = m_cmdRunList.FindIndex(i);
		CMD_TYPE ListType=m_cmdRunList.GetAt(pos).tType;

		if(ListType == CMD_LINE_PASSING || ListType == CMD_LINE_START || ListType == CMD_LINE_ARC)
		{
			if(GetRecord(tDot,i)) return -1;
			return i;
		}
	}
	return -1;
}
int CCmdRun::GetNextLineDot(int iCurIndex, tgCmdLine &tDot)
{
	for(int i=iCurIndex+1; i<m_cmdRunList.GetSize() ;i++)
	{
		POSITION pos = m_cmdRunList.FindIndex(i);
		CMD_TYPE ListType=m_cmdRunList.GetAt(pos).tType;
		if(ListType == CMD_LINE_PASSING || ListType == CMD_LINE_END	|| ListType == CMD_LINE_ARC)
		{
			if(GetRecord(tDot,i)) return -1;
			return i;
		}
	}
	return -1;
}
// ��ȡһ��ָ��
short CCmdRun::GetRecord(tgCmdLine &address,int Index)
{
	if (m_cmdRunList.IsEmpty()) return 1;

	if (Index>=m_cmdRunList.GetSize()) return 1;

	tgCmdLine pMyTempCmd;

	POSITION posIndex = m_cmdRunList.FindIndex(Index);
	pMyTempCmd = m_cmdRunList.GetAt(posIndex);

	address = pMyTempCmd;

	// ���Ϊ�˶��й�ָ�� ָ��λ�ø���ƫ����
	switch(address.tType)
	{
	case CMD_DUMMY:
	case CMD_POINT:
	case CMD_LINE_START:
	case CMD_LINE_PASSING:
	case CMD_LINE_ARC:
	case CMD_LINE_END:
	case CMD_CIRCLE:
	case CMD_BRUSH_AREA:
	case CMD_POT:
	case CMD_SUPERSTART:
	case CMD_SUPERPOINT:
	case CMD_SUPEREND:
		address.pDblValue[0]+=m_GotoOffSet[X_AXIS];
		address.pDblValue[1]+=m_GotoOffSet[Y_AXIS];
		address.pDblValue[2]+=m_GotoOffSet[Z_AXIS];
		break;
	case CMD_WCL:
		address.pDblValue[0]+=m_GotoOffSet[X_AXIS];
		address.pDblValue[1]+=m_GotoOffSet[Y_AXIS];
		address.pDblValue[2]+=m_GotoOffSet[Z_AXIS];

		address.pDblValue[4]+=m_GotoOffSet[X_AXIS];
		address.pDblValue[5]+=m_GotoOffSet[Y_AXIS];
		address.pDblValue[6]+=m_GotoOffSet[Z_AXIS];
		break;
	case CMD_WCRECT:
		address.pDblValue[0]+=m_GotoOffSet[X_AXIS];
		address.pDblValue[1]+=m_GotoOffSet[Y_AXIS];
		address.pDblValue[2]+=m_GotoOffSet[Z_AXIS];

		address.pDblValue[4]+=m_GotoOffSet[X_AXIS];
		address.pDblValue[5]+=m_GotoOffSet[Y_AXIS];
		address.pDblValue[6]+=m_GotoOffSet[Z_AXIS];

		address.pDblValue[8]+=m_GotoOffSet[X_AXIS];
		address.pDblValue[9]+=m_GotoOffSet[Y_AXIS];
		address.pDblValue[10]+=m_GotoOffSet[Z_AXIS];

		address.pDblValue[12]+=m_GotoOffSet[X_AXIS];
		address.pDblValue[13]+=m_GotoOffSet[Y_AXIS];
		address.pDblValue[14]+=m_GotoOffSet[Z_AXIS];

		address.pDblValue[16]+=m_GotoOffSet[X_AXIS];
		address.pDblValue[17]+=m_GotoOffSet[Y_AXIS];
		address.pDblValue[18]+=m_GotoOffSet[Z_AXIS];
		break;
	default:
		break;
	}
	return 0;
}
TS_CMD_TYPE CCmdRun::TranslateType(CMD_TYPE tType)
{
	switch (tType)
	{
	case CMD_POINT:
		return TS_CMD_POINT;
	case CMD_LINE_START:
		return TS_CMD_LINE_START;
	case CMD_LINE_PASSING:
		return TS_CMD_LINE_PASSING;
	case CMD_LINE_ARC:
		return TS_CMD_LINE_ARC;
	case CMD_LINE_END:
		return TS_CMD_LINE_END;
	case CMD_CIRCLE:
		return TS_CMD_CIRCLE;
	case CMD_END_PROGRAM:
		return TS_CMD_END_PROGRAM;
	default:break;
	}
	return TS_CMD_NONE;
}

short CCmdRun::CheckPos(tgCmdLine *pCmd)
{
	for (int m=0;m<pCmd->iDotNum;m++)
	{
		for(int i=0;i<3;i++)
		{
			if(pCmd->pDblValue[m*4+i]>g_pFrm->m_mtrParamGts[i].m_MaxTravelPositive || pCmd->pDblValue[m*4+i]<g_pFrm->m_mtrParamGts[i].m_MaxTravelNegative) 
				return RTN_DATA_OVER;
		}
	}

	return RTN_NONE;
}

short CCmdRun::SearchLabel(char *note)
{
	for (short j=0; j<m_cmdRunList.GetCount(); j++)
	{
		if(m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).tType == CMD_LABEL && (CString)m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).pszNote == (CString)note)
		{
			return j;
		}
	}
	return RTN_PARAMETER_ERR;
}

short CCmdRun::SearchLabelEnd(char *note)
{
	for (short j=0; j<m_cmdRunList.GetCount(); j++)
	{
		if(m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).tType == CMD_LABEL && (CString)m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).pszNote == (CString)note)
		{
			return j;
		}
	}
	return RTN_PARAMETER_ERR;
}

// ��ȡ��ǰ������ת��֮���λ��ƫ��
void CCmdRun::GetGotoOffset(int uiTemp,tgCmdLine pCmd)
{
	tgCmdLine pCmd0;                // ��ʱ�Խ�ָ�� 

	m_GotoOffSet[X_AXIS] = 0;
	m_GotoOffSet[Y_AXIS] = 0;
	m_GotoOffSet[Z_AXIS] = 0;

	for( ;uiTemp<m_cmdRunList.GetCount();uiTemp++)
	{
		GetRecord(pCmd0,uiTemp);
		switch(pCmd0.tType)
		{
		case CMD_POINT:
		case CMD_LINE_START:
		case CMD_LINE_PASSING:
		case CMD_DUMMY:
		case CMD_LINE_ARC:
		case CMD_LINE_END:
		case CMD_CIRCLE:
		case CMD_POT:
			m_GotoOffSet[X_AXIS] = pCmd.pDblValue[0] - pCmd0.pDblValue[0]-pCmd.pDblValue[4]/*�Ӻ�������΢������*/;
			m_GotoOffSet[Y_AXIS] = pCmd.pDblValue[1] - pCmd0.pDblValue[1]-pCmd.pDblValue[5];
			//m_GotoOffSet[Z_AXIS] = pCmd.pDblValue[2] - pCmd0.pDblValue[2];
			return;
		case CMD_WCL:
			return;
		case CMD_WCRECT:
			return;
		case CMD_END_PROGRAM: 
			return;
		default: break;
		}
	}	
}
// ��ȡ��ǰ������ת��֮���λ��ƫ��
void CCmdRun::GetGotoOffsetEx(int uiTemp,tgCmdLine pCmd)
{
	tgCmdLine pCmd0;                // ��ʱ�Խ�ָ�� 
	tgCmdLine ptempCmd,ptempCmd1;   // ��ʱָ�� 

	m_GotoOffSet[X_AXIS] = 0;
	m_GotoOffSet[Y_AXIS] = 0;
	m_GotoOffSet[Z_AXIS] = 0;

	// ��ȡ��һ�������������
	for( int i = 0 ;i<m_cmdRunList.GetCount(); i++)
	{
		POSITION pos = m_cmdRunList.FindIndex(i);
		CMD_TYPE ListType=m_cmdRunList.GetAt(pos).tType;

		if((i+1)>=m_cmdRunList.GetCount())
			break;

		POSITION pos1 = m_cmdRunList.FindIndex(i+1);
		CMD_TYPE ListType1=m_cmdRunList.GetAt(pos1).tType;

		if(ListType == CMD_SUPERSTART && ListType1 == CMD_SUPERPOINT)
		{
			GetRecord(ptempCmd,i);     // ��ȡ�ɽ��������
			GetRecord(ptempCmd1,i+1);  // ��ȡ�ɽ���һ��������
			break;
		}
	}	
	// �����һ�����͵�һ�������Yƫ��
	double  dYdis;
	dYdis = ptempCmd1.pDblValue[1]-ptempCmd.pDblValue[1];
	if(dYdis <0)
		dYdis = -dYdis;

	// �Ӻ������ô����Y��λ��
	double tempYpos = pCmd.pDblValue[1];  
	pCmd.pDblValue[1] = tempYpos -dYdis;

	for( ;uiTemp<m_cmdRunList.GetCount();uiTemp++)
	{
		GetRecord(pCmd0,uiTemp);
		switch(pCmd0.tType)
		{
		case CMD_POINT:
		case CMD_LINE_START:
		case CMD_LINE_PASSING:
		case CMD_DUMMY:
		case CMD_LINE_ARC:
		case CMD_LINE_END:
		case CMD_CIRCLE:
		case CMD_POT:
		case CMD_SUPERSTART:
		case CMD_SUPERPOINT:
		case CMD_SUPEREND:
			m_GotoOffSet[X_AXIS] = pCmd.pDblValue[0] - pCmd0.pDblValue[0]-pCmd.pDblValue[4];
			m_GotoOffSet[Y_AXIS] = pCmd.pDblValue[1] - pCmd0.pDblValue[1]-pCmd.pDblValue[5];
			//m_GotoOffSet[Z_AXIS] = pCmd.pDblValue[2] - pCmd0.pDblValue[2];
			if(bNeedFillCMPList == TRUE)
			{
				m_GotoOffSet[X_AXIS] += g_pFrm->m_pSysParam->tOffset[0].x;
				m_GotoOffSet[Y_AXIS] += g_pFrm->m_pSysParam->tOffset[0].y;
			}
			return;
		case CMD_WCL:
			return;
		case CMD_WCRECT:
			return;
		case CMD_END_PROGRAM: 
			return;
		default: break;
		}
	}	
}

// ��ת��ָ��λ��ִ��
short CCmdRun::RunGoto(int * uiIndex,tgCmdLine &address0,tgCmdLine &address1,tgCmdLine &address2)
{
	int uiPara;

	uiPara = address1.pIntValue[1]; //��ַ

	if(address1.pIntValue[1] == 0)
	{
		address0.tType = CMD_NONE;
		if(GetRecord(address1,0)) return RTN_PARAMETER_ERR;
		if(GetRecord(address2,1)) return RTN_PARAMETER_ERR;
		* uiIndex = -1;
	}
	else if(address1.pIntValue[1] == (m_cmdRunList.GetCount()-1))
	{
		memcpy(&address0,&address1,sizeof(address1));
		if(GetRecord(address1,uiPara)) return RTN_PARAMETER_ERR;
		address2.tType = CMD_NONE;
		* uiIndex = uiPara-1;
	}
	else if((address1.pIntValue[1] > 0)&&(address1.pIntValue[1] < (m_cmdRunList.GetCount()-1)))
	{
		memcpy(&address0,&address1,sizeof(address1));
		if(GetRecord(address1,uiPara))      return RTN_PARAMETER_ERR;
		if(GetRecord(address2,uiPara + 1))  return RTN_PARAMETER_ERR;
		* uiIndex = uiPara-1;
	}
	return RTN_GOTO_ADD;
}

short CCmdRun::DotDisp(int dotNum, RunMode md)	// ����ָ�������ĵĵ�
{
// 	// �����˶�
 	short sTemp;
// 	sTemp=m_pController->TS_ListWaitStop();

	double dDispCyc, dDispFreq;
	g_pFrm->m_comDisp[0].GetCycleTimeAndFreq(dDispCyc, dDispFreq);
	if (DispCtrlMode_IO == m_pController->m_dispCtrlMode)
	{
		if (dotNum != g_pFrm->m_comDisp[0].m_dispParam.NumOfPul)
		{
			g_pFrm->m_comDisp[0].m_dispParam.NumOfPul = dotNum;
			if (1 != g_pFrm->m_comDisp[0].HandleParamDispenser(FALSE))
			{
				AfxMessageBox("���ش��ʱд��㽺����������!");
				return -1;
			}
		}

		if (RunMode_Wet == m_runMode)
			ExOutput(_T("1�ŵ��̬����"), TRUE);
		double dDispTime = dotNum*dDispCyc;
		dDispCyc *= 1.2;
		CFunction::DelaySec(dDispTime/1000.0);
		ExOutput(_T("1�ŵ��̬����"), FALSE);
	}
	else if (DispCtrlMode_Compare == m_pController->m_dispCtrlMode)
	{
		if (1)
		{
			CKFTimer tmTemp;
			tmTemp.TimerStart();
			for (int j=1; j<=dotNum; j++)
			{
				while (tmTemp.TimerGetMs() < j*dDispCyc)
				{
					//CFunction::DoEvents();
				}

				if (RunMode_Wet == m_runMode)
				{
					sTemp = m_pController->ComparePulse();
					if (sTemp != RTN_NONE)
					{
						return sTemp;
					}
				}
			}
		}
		else
		{
			if (dotNum != g_pFrm->m_comDisp[0].m_dispParam.NumOfPul)
			{
				g_pFrm->m_comDisp[0].m_dispParam.NumOfPul = dotNum;
				if (1 != g_pFrm->m_comDisp[0].HandleParamDispenser(FALSE))
				{
					AfxMessageBox("���ʱд��㽺����������!");
					return -1;
				}

				CFunction::DelaySec(0.3);
			}

			sTemp = m_pController->ComparePulse();
			if (sTemp != RTN_NONE)
			{
				return sTemp;
			}

			double dDispTime = dotNum*dDispCyc;
			dDispTime *= 1.2;
			CFunction::DelaySec(dDispTime/1000.0);
		}

		// ���´�������ϵ
		sTemp=m_pController->TS_ListInit(CRD1, 1);
		return sTemp;
	}

 	short rtn = 0;
// 	rtn = g_pFrm->m_Robot->m_pController->AxisRelativeMove(U_AXIS, dRelPos, dVel, g_pFrm->m_mtrParamGts[U_AXIS].m_Acc);
// 	if (RTN_NONE != rtn)
// 	{
// 		AfxMessageBox("�����ʧ��!");
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	// �����˶����ȴ��˶�ֹͣ
// 	short sRtnTemp;
// 	sRtnTemp=m_pController->TS_ListWaitStop();
// 	if(sRtnTemp!=RTN_NONE) return sRtnTemp;

	return rtn;
}
short CCmdRun::WeightAdjust()
{
	{
		g_pFrm->m_Robot->GotoSafeZpos();	

		double x=0, y=0, z=0;
		x = g_pFrm->m_pRobotParam->m_posWeightAdj.x;
		x += g_pFrm->m_pSysParam->tOffset[0].x;
		y = g_pFrm->m_pRobotParam->m_posWeightAdj.y;
		y += g_pFrm->m_pSysParam->tOffset[0].y;
		z = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_posWeightAdj.z;

		g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc, TRUE);
	}

	CString str;
	double dWtVal[3];	// ���ӳƶ���
	g_pFrm->m_comWeight.GetWeight(dWtVal[0]);
	g_pFrm->m_pRobotParam->m_dWasteGlueCur += dWtVal[0];

	g_pFrm->m_comWeight.SetZero();
	g_pFrm->m_comWeight.SetImdRpt();

	double dAvgWt = 0.0;
	int dotNum = g_pFrm->m_pRobotParam->m_nWeightAdjDotNum;
	if (dotNum <1)
	{
		return -1;
	}
	double dotWt = g_pFrm->m_pRobotParam->m_dAvgWeight;

	for (int i=0; i<3; i++)
	{
		g_pFrm->m_Robot->DotDisp(dotNum);
		Sleep(2000);
		g_pFrm->m_comWeight.GetWeight(dWtVal[i]);
		g_pFrm->m_pRobotParam->m_dWasteGlueCur += dWtVal[i];
		g_pFrm->m_comWeight.SetZero();

		if (dWtVal[i] < EPS)
		{
			AfxMessageBox("���㽺��̫С!");
			return -1;
		}

		dAvgWt = dWtVal[i] / dotNum; 
		if ((dAvgWt > dotWt*2) || (dAvgWt < dotWt/2.0))
		{
			AfxMessageBox("����У������!");
			return -1;
		}
	}

	if (g_pFrm->m_pRobotParam->m_dWasteGlueCur > (g_pFrm->m_pRobotParam->m_dWasteGlueAlarm*1000))
	{
		g_pFrm->m_Robot->ExOutput(_T("��ɫ��.��"),TRUE);
		g_pFrm->m_Robot->ExOutput(_T("������"),TRUE);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.m_bWasteGlueAlarm = true;
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetCurWasteGlue("�Ͻ�������");
	}
	else
	{
		CString strTemp;
		strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_dWasteGlueCur/1000.0);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetCurWasteGlue(strTemp);
	}

	dotWt = (dWtVal[0] + dWtVal[1] + dWtVal[2])/3.0/dotNum;

	g_pFrm->m_pRobotParam->m_dAvgWeight = dotWt;
	// ���浥�㽺��
	CString strFile = CFunction::GetDirectory() + "system\\robotparam.dat";
	CFunction::HandleDouble(FALSE, "AvgWeight", "AvgWeight", g_pFrm->m_pRobotParam->m_dAvgWeight, strFile);	

	// ���ؼ�ʱ
	m_tmAutoWeight.TimerStart();
	return 1;
}

short CCmdRun::DispOpen(bool bOpen)
{
	UNREFERENCED_PARAMETER(bOpen);
	short sTemp;

	double dDispCyc, dDispFreq;
	g_pFrm->m_comDisp[0].GetCycleTimeAndFreq(dDispCyc, dDispFreq);

	CKFTimer tmTemp;
	tmTemp.TimerStart();
	for (int j=1; j<=1000000; j++)
		//while(m_bDispOpen)
	{
		while (tmTemp.TimerGetMs() < j*dDispCyc)
		{
			CFunction::DoEvents();
		}

		sTemp = m_pController->ComparePulse();
		if (!m_bDispOpen)
		{
			break;
		}
		if (sTemp != RTN_NONE)
		{
			return sTemp;
		}
	}

	return RTN_NONE;
}

short CCmdRun::GotoSafeZpos(BOOL bWaitStop)
{
	short rtn = 0;
	if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		rtn = ExOutput(_T("̽�����׵�ŷ�"), FALSE);
	
	m_pController->GetCurPosMM();
	double curZPos = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
	if (curZPos > g_pFrm->m_pRobotParam->m_zSafePos)
	{
		if (bWaitStop)
		{
			return m_pController->AxisMove(Z_AXIS, g_pFrm->m_pRobotParam->m_zSafePos,g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc, TRUE);
		}
		else
		{
			return m_pController->AxisMove(Z_AXIS, g_pFrm->m_pRobotParam->m_zSafePos,g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc, FALSE);
		}
	}
	else
	{
		return rtn;
	}
}

BOOL CCmdRun::Syntax()
{
	BOOL bRet= TRUE;
	CString str("");
	switch (SubSyntax())
	{
	case SYNTAX_POINT_DISPENSE_ERROR:
		AfxMessageBox("���㲻�ܷ����߶ο�ʼ�����߶ν�����֮��!"); 
		bRet = FALSE;
		break;
	case SYNTAX_WCL_PARAERROR:
		AfxMessageBox("���ػ����ٶ����������ʱ�䲻ƥ��");
		bRet = FALSE;
		break;
	case SYNTAX_NO_LINE_END: 
		AfxMessageBox("û���߶ν�����!"); 
		bRet = FALSE;
		break;
	case SYNTAX_NO_LINE_START:
		AfxMessageBox("û���߶ο�ʼ��!");
		bRet = FALSE;
		break;
	case SYNTAX_LINE_PASS_ERR:
		str="�߶��м��ǰ��ֻ������Ϊ:1.�߶ο�ʼ��,2.�߶��м��,3.Բ����,4.����/��!";
		AfxMessageBox(str);
		bRet= FALSE;
		break;
	case SYNTAX_ARC_ERR:
		str="Բ����ǰ��ֻ������Ϊ:1.�߶ο�ʼ��,2.�߶��м��,3.����/��!";
		AfxMessageBox(str);
		bRet= FALSE;
		break;
	case SYNTAX_DATA_VOER: 
		AfxMessageBox("��ֵ���!");
		bRet = FALSE;
		break;
	case SYNTAX_GOTO_ADD_ERR:
		AfxMessageBox("û���ҵ�Ҫ��ת�ĵ�ַ���ţ����ַ�Ƿ�!");
		bRet = FALSE;
		break; 
	case SYNTAX_LINE_END_ERR:
		str="�߶ν�����ǰ��ֻ������Ϊ:1.�߶ο�ʼ��,2.�߶��м��,3.Բ����,4.����/��,5.����Ϳ��!";
		AfxMessageBox(str);
		bRet= FALSE;
		break;	
	case SYNTAX_CIRCLE_ERROR:
		str="Բ�β��ܷ����߶���ʼ���������֮��!";
		AfxMessageBox(str);
		bRet= FALSE;
		break;	
	case SYNTAX_LOOP_ADD_ERR:
		AfxMessageBox("ѭ��ָ�ʼ��ַ���ڵ�ǰ��ַ!");
		bRet = FALSE; 
		break;
	case SYNTAX_NO_END_PROGRAME:
		AfxMessageBox("û�г�ʽ������־!");
		bRet = FALSE;
		break;
	case SYNTAX_BRUSHAREA_ERROR:
		AfxMessageBox("Ϳ��ָ������߶���ʼ�����߶ν�����֮��!"); 
		bRet = FALSE;
		break;
	default:
		break;
	}
	return (bRet);
}

SYNTAX_ERROR_CODE CCmdRun::SubSyntax(void)
{
	short Count;
	unsigned char LineStart = 0;
	unsigned int have_end = 0;

	short checkPosRet;
	m_iDebugErrorLine=0;
	BOOL bFindLabel = TRUE;

	tgCmdLine tempCmdLine;
	tgCmdLine pCmd;

	for(Count=0;Count<m_cmdRunList.GetCount();Count++)
	{
		POSITION posIndex = m_cmdRunList.FindIndex(Count);
		pCmd = m_cmdRunList.GetAt(posIndex);
		m_iDebugErrorLine = Count;
		switch(pCmd.tType)
		{
		case CMD_POT:		//
		case CMD_SUPERDOT:	// ���ٴ��
		case CMD_SUPERPOINT:// �����м��
		case CMD_POINT:		// ����㽺
		case CMD_ZMETER:	// ���
		case CMD_DUMMY:		// �յ�	
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE)	return SYNTAX_DATA_VOER;
			if(LineStart==1) 			return SYNTAX_POINT_DISPENSE_ERROR;//ֱ�߿�ʼ�˻�û����
			break;

		case CMD_SUPEREND:
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE)	return SYNTAX_DATA_VOER;
			if(LineStart==0)			return SYNTAX_NO_LINE_START;//ǰ��ûֱ�����
			else LineStart = 0; 
			break;

		case CMD_SUPERSTART:
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE)
				return SYNTAX_DATA_VOER;
			if(LineStart==1) 	return SYNTAX_POINT_DISPENSE_ERROR;//ֱ�߿�ʼ�˻�û����
			else LineStart = 1; 
			break;
			
		case CMD_LINE_START:	// ֱ�߿�ʼ
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE)	return SYNTAX_DATA_VOER;
			if(LineStart==1)			return SYNTAX_NO_LINE_END;//ֱ�߿�ʼ�˻�û����
			else LineStart = 1;			break;
			
		case CMD_LINE_PASSING:	// ֱ���м��
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE) return SYNTAX_DATA_VOER;
			//if(LineStart==0) return SYNTAX_NO_LINE_START;////ǰ��ûֱ�����
			//GetPreLineDot(Count, tempCmdLine);
			GetPreDot(Count, tempCmdLine);
			if((tempCmdLine.tType!=CMD_LINE_START)
				&&(tempCmdLine.tType!=CMD_LINE_ARC)
				&&(tempCmdLine.tType!=CMD_LINE_PASSING))
				//&&(tempCmdLine.tType!=CMD_DISPENSER))
				return SYNTAX_LINE_PASS_ERR;
			//return SYNTAX_NO_ERROR;
			break;
						
		case CMD_LINE_ARC:	// Բ��
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE) return SYNTAX_DATA_VOER;

			//GetPreLineDot(Count, tempCmdLine);
			GetPreDot(Count, tempCmdLine);
			if((tempCmdLine.tType!=CMD_LINE_START)
				&&(tempCmdLine.tType!=CMD_LINE_PASSING)
				&&(tempCmdLine.tType!=CMD_LINE_ARC)
				&&(tempCmdLine.tType!=CMD_DISPENSER)
				&&(tempCmdLine.tType!=CMD_SUPERPOINT)
				&&(tempCmdLine.tType!=CMD_SUPERSTART)
				&&(tempCmdLine.tType!=CMD_SUPEREND)) 
				return SYNTAX_ARC_ERR;
			break;
			// ֱ�߽�����
		case CMD_LINE_END:
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE) return SYNTAX_DATA_VOER;
			if(LineStart==0) return SYNTAX_NO_LINE_START;//ǰ��ûֱ�����
			else LineStart = 0; 

			//GetPreLineDot(Count, tempCmdLine);
			GetPreDot(Count, tempCmdLine);
			if((tempCmdLine.tType!=CMD_LINE_START)
				&&(tempCmdLine.tType!=CMD_LINE_ARC)
				&&(tempCmdLine.tType!=CMD_LINE_PASSING)
				&&(tempCmdLine.tType!=CMD_DELAY)
				&&(tempCmdLine.tType!=CMD_DISPENSER))
				return SYNTAX_LINE_END_ERR;
			break;
			// Բ�ε㽺
		case CMD_CIRCLE:   
			// �켣������Χ
			if(LineStart==1) return SYNTAX_CIRCLE_ERROR;//ֱ�߿�ʼ�˻�û����

			if(pCmd.pIntValue[1]==0)//Բ�ģ�ֱ��
			{
				for(int i=0;i<m_pController->g_ExMtnPara->axisNum;i++)
				{
					if(pCmd.pDblValue[i]>g_pFrm->m_mtrParamGts[i].m_MaxTravelPositive || pCmd.pDblValue[i]<g_pFrm->m_mtrParamGts[i].m_MaxTravelNegative) 
						return SYNTAX_DATA_VOER;
				}
				if(pCmd.pDblValue[0]+pCmd.pDblValue[4]>g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive || pCmd.pDblValue[0]-pCmd.pDblValue[4]<g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelNegative) 
					return SYNTAX_DATA_VOER;
				if(pCmd.pDblValue[1]+pCmd.pDblValue[4]>g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive || pCmd.pDblValue[1]-pCmd.pDblValue[4]<g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelNegative) 
					return SYNTAX_DATA_VOER;
			}
			else
			{
				checkPosRet = CheckPos(&pCmd);
				if(checkPosRet !=RTN_NONE) return SYNTAX_DATA_VOER;	
			}
			break;	
		case CMD_GOTO_ADDRESS:
			if (pCmd.pIntValue[0]!=0)
			{
				for (int j=0;j<m_cmdRunList.GetCount();j++)
				{
					if(m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).tType == CMD_LABEL && (CString)m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).pszNote == (CString)pCmd.pszNote)
					{
						bFindLabel = TRUE;
						break;
					}
					else
						bFindLabel = FALSE;
				}
				if(!bFindLabel) return SYNTAX_GOTO_ADD_ERR;
			}
			else
			{
				if(pCmd.pIntValue[1]>Count)
					return SYNTAX_GOTO_ADD_ERR;
			}

			break;
		case CMD_CALL_SUB:
			if (pCmd.pIntValue[0]==1)//���
			{
				for (int j=0;j<m_cmdRunList.GetCount();j++)
				{
					if(m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).tType == CMD_LABEL && (CString)m_cmdRunList.GetAt(m_cmdRunList.FindIndex(j)).pszNote == (CString)pCmd.pszNote)
					{
						bFindLabel = TRUE;
						break;
					}
					else
						bFindLabel = FALSE;
				}
				if(!bFindLabel) return SYNTAX_GOTO_ADD_ERR;
			}
			else
			{
				if(pCmd.pIntValue[1]>=m_cmdRunList.GetCount())
					return SYNTAX_GOTO_ADD_ERR;
				if(pCmd.pIntValue[1]<0)
					return SYNTAX_GOTO_ADD_ERR;
			}			
			break;

		case CMD_LOOP:
			if(pCmd.pIntValue[1]>Count)		return SYNTAX_LOOP_ADD_ERR;
			break;

		case CMD_BRUSH_AREA:
			if(LineStart==1)	return SYNTAX_BRUSHAREA_ERROR;//ֱ�߿�ʼ�˻�û����
			break;

		case CMD_MARK:
			m_structPathInfo.m_bFindMark = true;
			m_structPathInfo.m_nMarkNum = 0;
			if (pCmd.pDblValue[0]>EPS || pCmd.pDblValue[1]>EPS)
			{
				g_pFrm->m_pSysParam->tDefHomePos.x = pCmd.pDblValue[0];
				g_pFrm->m_pSysParam->tDefHomePos.y = pCmd.pDblValue[1];
				m_structPathInfo.m_nMarkNum++;
			}
			if (pCmd.pDblValue[4]>EPS || pCmd.pDblValue[5]>EPS)
				m_structPathInfo.m_nMarkNum++;
			break;

			
		case CMD_REPEATX:	// ���м���Ƿ�������м俪ʼ�㽺
		case CMD_REPEATY:
			m_structPathInfo.m_bRepeat = true;
			if (pCmd.pIntValue[4]>1 || pCmd.pIntValue[5]>1)
			{
// 				m_structPathInfo.m_bRepeatSetStart = true;
// 				m_structPathInfo.m_nRepeatStX = pCmd.pIntValue[4];
// 				m_structPathInfo.m_nRepeatStY = pCmd.pIntValue[5];

				tgCmdLine &pCmdTemp = m_cmdRunList.GetAt(posIndex);
				pCmdTemp.pIntValue[4] = 1;	// ���м俪ʼ�㽺��ִֻ��һ��
				pCmdTemp.pIntValue[5] = 1;
				posIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(Count);
				tgCmdLine &pCmdTemp1 = g_pFrm->m_wndEditBar.m_rawList.GetAt(posIndex);
				pCmdTemp1.pIntValue[4] = 1;	// ���м俪ʼ�㽺��ִֻ��һ��
				pCmdTemp1.pIntValue[5] = 1;
				CString str;
				str.Format("st id: %d, %d", m_structPathInfo.m_nRepeatStX, m_structPathInfo.m_nRepeatStY);
				g_pFrm->AddMsg(str);
			}
			break;

			// �������
		case CMD_END_PROGRAM:
			have_end = 1;
			break;
		default:break;
		}
	}

	if(LineStart)		return SYNTAX_NO_LINE_END;
	if(have_end !=1 )	return SYNTAX_NO_END_PROGRAME;
	m_iDebugErrorLine = 0;//���õ�ǰ��Ϊ ��һ��

	return SYNTAX_NO_ERROR;
}
// �����ػ������ݺϷ���
BOOL CCmdRun::CheckWCLData(tgCmdLine *pCmd)
{
	// ����λ��
	double dxA,dyA,dzA,duA;
	double dxB,dyB,dzB,duB;

	dxA = pCmd->pDblValue[0];
	dyA = pCmd->pDblValue[1];
	dzA = pCmd->pDblValue[2];
	duA = pCmd->pDblValue[3];

	//if(GetCmdPosition(pCmd,&dxA,&dyA,&dzA,0,0)==FALSE) 
	//	return RTN_DATA_OVER;

	dxB = pCmd->pDblValue[4];
	dyB = pCmd->pDblValue[5];
	dzB = pCmd->pDblValue[6];
	duB = pCmd->pDblValue[7];

	//if(GetCmdPosition(pCmd,&dxB,&dyB,&dzB,0,0,1)==FALSE) 
	//	return RTN_DATA_OVER;

	//�����ٶ� 
	double  dWorkVel  = m_cmdParam.tLine.dVel;
	//
	double disx = dxB-dxA ;
	double disy = dyB-dyA ;
	double dis=sqrt((disx)*(disx)+(disy)*(disy)); // �߶γ�   mm
	double t = dis / dWorkVel;    // ��Ҫ��ʱ��    s
	t= 1000*t;                    // s -> ms

	int tdis =(int) (t / (int)pCmd->pIntValue[3]);  // ÿ�����ʱ����
	tdis -= m_cmdParam.nLowTime;  // ѭ�����ڼ��

	if(tdis<=0)
	{
		return FALSE;     
	}
	return TRUE;
}

BOOL CCmdRun::GetCircle(const tgCmdLine *tgCmd, double *pX, double *pY, double *pR)
{
	if(tgCmd->tType != CMD_CIRCLE || tgCmd->iDblNum<12)return FALSE;
	double dX[3], dY[3];
	for(int i=0; i<3; i++)
	{
		dX[i] = tgCmd->pDblValue[i*4 + 0];
		dY[i] = tgCmd->pDblValue[i*4 + 1];
	}

	CPointGE pGE;
	pGE.CaclCirclePara(dX[0],dY[0],dX[1],dY[1],dX[2],dY[2],pX, pY, pR);

	return TRUE;
}

// ��ͷ�Խ����� 
void CCmdRun::NeedleTestDisp()
{
	double x=0,y=0,z=0;

	if (g_pFrm->m_pSysParam->nToolType == 2)
	{
		AfxMessageBox(_T("ͼ��ģʽ�²����Խ���������ѡ����ģʽ��"));
		return ;
	}

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		x = g_pFrm->m_pSysParam->tDispTestPos.x;
		y = g_pFrm->m_pSysParam->tDispTestPos.y;
		z = g_pFrm->m_pSysParam->tDispTestPos.z;

		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

		if (g_pFrm->m_pSysParam->nJetPulse==0)//����ģʽ
		{
			AfxMessageBox(_T("Switch times cannot be zero!"));
		}
		else
		{   
			g_pFrm->m_Robot->NeedleDisp(g_pFrm->m_pSysParam->nJetPulse,TRUE);
		}

		g_pFrm->m_Robot->GotoSafeZpos();
	}
	else
	{
		AfxMessageBox(_T("System Init error"));
	}
}

// �Խ����� 
short CCmdRun::TestDisp()
{
	short iRtn = 0;

	if (g_pFrm->m_pSysParam->nToolType == 2)
	{
		AfxMessageBox(_T("ͼ��ģʽ�²����Խ���������ѡ����ģʽ��"));
		return 0;
	}
	g_pFrm->m_wndEditBar.EnableWindow(FALSE);

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		g_pFrm->m_pRobotParam->m_structTestGlue.m_nBdRows = 
			int((g_pFrm->m_pRobotParam->m_structTestGlue.m_posButRight.y - g_pFrm->m_pRobotParam->m_structTestGlue.m_posTopLeft.y - 20) / g_pFrm->m_pRobotParam->m_structTestGlue.m_nGLueSpace);
		g_pFrm->m_pRobotParam->m_structTestGlue.m_nBdColumns = 
			int((g_pFrm->m_pRobotParam->m_structTestGlue.m_posButRight.x - g_pFrm->m_pRobotParam->m_structTestGlue.m_posTopLeft.x - 20) / g_pFrm->m_pRobotParam->m_structTestGlue.m_nGLueSpace);

		if (g_pFrm->m_pRobotParam->m_structTestGlue.m_nCurIndex == g_pFrm->m_pRobotParam->m_structTestGlue.m_nBdColumns*g_pFrm->m_pRobotParam->m_structTestGlue.m_nBdRows)
		{
			AfxMessageBox(_T("�մɰ��Ѿ��Խ���,�����մɰ�����!"));
			g_pFrm->m_pRobotParam->m_structTestGlue.m_nCurIndex = 0;
		}

		double dPosX = g_pFrm->m_pRobotParam->m_structTestGlue.m_posTopLeft.x + 5 + (g_pFrm->m_pRobotParam->m_structTestGlue.m_nCurIndex % g_pFrm->m_pRobotParam->m_structTestGlue.m_nBdColumns)
			* (g_pFrm->m_pRobotParam->m_structTestGlue.m_nGlueLen + g_pFrm->m_pRobotParam->m_structTestGlue.m_nGLueSpace);
		dPosX += g_pFrm->m_pSysParam->tOffset[0].x;
		double dPosY = g_pFrm->m_pRobotParam->m_structTestGlue.m_posTopLeft.y + 5 + (g_pFrm->m_pRobotParam->m_structTestGlue.m_nCurIndex / g_pFrm->m_pRobotParam->m_structTestGlue.m_nBdColumns)
			* g_pFrm->m_pRobotParam->m_structTestGlue.m_nGLueSpace;
		dPosY += g_pFrm->m_pSysParam->tOffset[0].y;

		double dPosZ = 0;
		if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
			dPosZ = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_posJetAdjust.z+g_pFrm->m_pRobotParam->m_zLaserCrossVal-g_pFrm->m_pRobotParam->m_zLaserBaseVal;
		else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
			dPosZ = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_posJetAdjust.z+g_pFrm->m_pRobotParam->m_zProbeCrossVal-g_pFrm->m_pRobotParam->m_zProbeBaseVal;

		iRtn = g_pFrm->m_Robot->GotoSafeZpos();
		iRtn = g_pFrm->m_Robot->m_pController->LineMoveXY(CRD1, 0, dPosX, dPosY, 100, 10);
		iRtn = g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, dPosZ, 20, 10);

		if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
		{
		}
		else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
		{
			dPosX += g_pFrm->m_pRobotParam->m_structTestGlue.m_nGlueLen;
			iRtn = g_pFrm->m_Robot->ExOutput(_T("1�ŵ��̬����"), TRUE);
			iRtn = g_pFrm->m_Robot->m_pController->AxisMove(X_AXIS, dPosX, g_pFrm->m_pRobotParam->m_structTestGlue.m_dWorkVel, 10);
			iRtn = g_pFrm->m_Robot->ExOutput(_T("1�ŵ��̬����"), FALSE);

			g_pFrm->m_pRobotParam->m_structTestGlue.m_nCurIndex++;
		}

		iRtn = g_pFrm->m_Robot->GotoSafeZpos();
	}
	else
	{
		AfxMessageBox(_T("System Init error"));
	}

	if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
	{
		g_pFrm->m_wndEditBar.EnableWindow(TRUE);
	}

	return iRtn;
}
// ��ϴ��ͷ����
void CCmdRun::WashNeedle()
{
	double x=0,y=0,z=0;

	g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(FALSE);
	g_pFrm->m_wndEditBar.m_wndList.EnableWindow(FALSE);

	if(m_pController->IsInitOk())
	{
		x = g_pFrm->m_pRobotParam->m_structClean.m_posClean.x+g_pFrm->m_pSysParam->tOffset[0].x;   
		y = g_pFrm->m_pRobotParam->m_structClean.m_posClean.y+g_pFrm->m_pSysParam->tOffset[0].y;
		z = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_structClean.m_posClean.z;
		double zDown = g_pFrm->m_pRobotParam->m_structClean.m_dCleanDownMM;
		int num = g_pFrm->m_pRobotParam->m_structClean.m_nCleanTimes;
		zDown = min(5, zDown);
			
		// �ƶ���λ
		GotoSafeZpos(); 
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
		// ����ϴ�� 
		ExOutput(_T("��๤��վ��ŷ�"),TRUE);
		for(int i=0;i<num;i++)
		{   
			m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
			CFunction::DelaySec(0.5);
			m_pController->AxisMove(Z_AXIS,z+zDown, g_pFrm->m_mtrParamGts[Z_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
		}
		// �ر���ϴ��
		ExOutput(_T("��๤��վ��ŷ�"),FALSE);
		g_pFrm->m_Robot->GotoSafeZpos();
	}
	else
	{
		AfxMessageBox(_T("�˶����ƿ�δ������ʼ����"));
	}

	if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
	{
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
	}
} 
// ��ϴ���䷧����
void CCmdRun::WashJet(bool bIfDisp)
{
	if (!m_pController->IsInitOk())
	{
		AfxMessageBox(_T("System Init Fail��"));
		return;
	}
	
	double x=0,y=0,z=0;

	if (!bIfDisp)
	{
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(FALSE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(FALSE);
	}
	
	if(bIfDisp || (IDYES == AfxMessageBox("��ȷ���Ƿ���ϴ!", MB_YESNO)))
	{
		x = g_pFrm->m_pRobotParam->m_structClean.m_posClean.x+g_pFrm->m_pSysParam->tOffset[0].x;   
		y = g_pFrm->m_pRobotParam->m_structClean.m_posClean.y+g_pFrm->m_pSysParam->tOffset[0].y;
		z = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_structClean.m_posClean.z;

		GotoSafeZpos();
		// �ƶ���λ
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,m_cmdParam.dVelXY,m_cmdParam.dAccXY);
		m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

		// ��IO
		int num = g_pFrm->m_pRobotParam->m_structClean.m_nCleanTimes;
		ExOutput("��๤��վ��ŷ�",TRUE);
		for (int i=0;i<num;i++)
		{
			// �����ƶ�
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,x-0.8,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove/6,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,x+0.8,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove/6,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			//g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParam[X_AXIS].m_Vmin/2,g_pFrm->m_mtrParam[X_AXIS].m_Acc);
			CFunction::DelaySec(0.05);
			// ǰ���ƶ�
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y+0.8,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove/6,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y-0.8,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove/6,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove/6,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			CFunction::DelaySec(0.05);
		}
		// �ر�IO
		ExOutput("��๤��վ��ŷ�",FALSE);

		GotoSafeZpos();
	}

	if(!bIfDisp && theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
	{
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
	}
}

// ģ����ģ�� ��Ͱ��ѹ����������
short CCmdRun::SetBackPressure(double dVal)
{
	short idata = (short)(dVal / 0.4 * 32767);

	short rtn = m_pController->SetDac(12, &idata);
	if (RTN_NONE != rtn)
	{
		return rtn;
	}

	// ��Ͱ��ѹ
	g_pFrm->m_pRobotParam->m_dBackPress = dVal;
	g_pFrm->RobotParam(FALSE);
	return rtn;
}

short CCmdRun::GetBackPressure(double &dVal)
{
	short uData;
	short rtn = m_pController->GetDac(12, &uData);
	if (RTN_NONE != rtn)
	{
		return rtn;
	}

	dVal = (double)uData*0.4/32767;
	return rtn;
}

short CCmdRun::LoadUnLoad_loadBoxMove(int idx, bool bWaitStop)
{	
	if (!m_pController->IsInitOk())						return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;
	if (idx>2 || idx<0)									return 0;
	if (RTN_NONE != LoadUnLoad_loadCheck())				return 0;

	short rtn = 0;
// 	if (idx == g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId)
// 	{
// 		return RTN_NONE;
// 	}
// 	else
// 	{
		double dPos = g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[idx*2];
		if (bWaitStop)
			rtn = g_pFrm->m_Robot->m_pController->AxisMove(LOAD_AXIS,dPos,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Acc, TRUE);
		else
			rtn = g_pFrm->m_Robot->m_pController->AxisMove(LOAD_AXIS,dPos,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Acc, FALSE);

		if (RTN_NONE != rtn)			return 0;

		g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId = idx;
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId = 0;
//	}
	return rtn;
}
short CCmdRun::LoadUnLoad_loadLayerMove(int idx)
{	
	if (!m_pController->IsInitOk())						return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;
	if (RTN_NONE != LoadUnLoad_loadCheck())				return 0;
	int layerCount = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount;
	if ((idx>layerCount-1) || (idx<0) || (layerCount<2))return 0;

	short rtn;
	if (idx == g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId)
	{
		return RTN_NONE;
	}
	else
	{
		int nCrtBoxId = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId;
		double dPitch = g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[nCrtBoxId*2+1] - g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[nCrtBoxId*2];
		dPitch = dPitch/(layerCount-1);
		
		double dPos = g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[nCrtBoxId*2] + dPitch*idx;
		rtn = g_pFrm->m_Robot->m_pController->AxisMove(LOAD_AXIS,dPos,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Acc, TRUE);
		if (RTN_NONE != rtn)	return 0;

		g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId = idx;
	}
	
	return rtn;
}
short CCmdRun::LoadUnLoad_unloadBoxMove(int idx, bool bWaitStop)
{	
	if (!m_pController->IsInitOk())						return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;
	if (idx>2 || idx<0)									return 0;
	if (RTN_NONE != LoadUnLoad_unloadCheck())			return 0;

	short rtn = 0;
// 	if (idx == g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId)
// 	{
// 		return RTN_NONE;
// 	}
// 	else
// 	{
		double dPos = g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[idx*2];
		if (bWaitStop)
			rtn = g_pFrm->m_Robot->m_pController->AxisMove(UNLOAD_AXIS,dPos,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Acc, TRUE);
		else
			rtn = g_pFrm->m_Robot->m_pController->AxisMove(UNLOAD_AXIS,dPos,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Acc, FALSE);

		if (RTN_NONE != rtn)		return 0;

		g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId = idx;
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId = 0;
//	}

	return rtn;
}
short CCmdRun::LoadUnLoad_unloadLayerMove(int idx)
{	
	if (!m_pController->IsInitOk())						return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;
	if (RTN_NONE != LoadUnLoad_unloadCheck())			return 0;
	int layerCount = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nUldLayerCount;
	if ((idx>layerCount-1) || idx<0 || layerCount<2)	return 0;

	short rtn=0;
	if (idx == g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId)
	{
		return RTN_NONE;
	}
	else
	{
		int nCrtBoxId = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;
		double dPitch = g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[nCrtBoxId*2+1] - g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[nCrtBoxId*2];
		dPitch = dPitch/(layerCount-1);

		double dPos = g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[nCrtBoxId*2] + dPitch*idx;
		rtn = g_pFrm->m_Robot->m_pController->AxisMove(UNLOAD_AXIS,dPos,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Acc, TRUE);
		if (RTN_NONE != rtn)	return 0;
		
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId = idx;
	}

	return rtn;
}

short CCmdRun::LoadUnLoad_loadPushOut(BOOL &bBdDetect, BOOL bToEnd)	//bBdDetect:����Ƿ��⵽����   bToEnd:�Ƿ����ϵ��յ�
{
	if (!m_pController->IsInitOk())						return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;
	
	short iRet = g_pFrm->m_Robot->ExOutput("���ϵ������",TRUE);
	if (iRet != RTN_NONE)	
	{
		iRet = g_pFrm->m_Robot->ExOutput("���ϵ������",TRUE);
		CFunction::DelaySec(2);
		iRet = g_pFrm->m_Robot->ExOutput("���ϵ������",FALSE);
		iRet = g_pFrm->m_Robot->ExOutput("���ϵ������",FALSE);
		AfxMessageBox("���׶�������!");
		return 0;
	}

	BOOL bStatus1 = FALSE, bStatus2 = FALSE;
	iRet = g_pFrm->m_Robot->ExInput("���ϵ��-R", bStatus1);
	CHECK_RTN_RTNVAL(_T("��ȡ[���ϵ��-R]�źų���"), iRet, 0);

	CKFTimer tmTemp;
	tmTemp.TimerStart();
	while (!bStatus1)
	{
		if (tmTemp.TimerGetSecond() > 4)
		{
			g_pFrm->m_Robot->ExOutput("���ϵ������",TRUE);
			CFunction::DelaySec(2);
			g_pFrm->m_Robot->ExOutput("���ϵ������",FALSE);
			g_pFrm->m_Robot->ExOutput("���ϵ������",FALSE);
			AfxMessageBox("���ϵ�����ϵ�λ��ʱ!");
			return 0;
		}
		iRet = g_pFrm->m_Robot->ExInput("���ϵ��-R", bStatus1);
		if (!bStatus2)
		{
			g_pFrm->m_Robot->ExInput("11�Ź⿪", bStatus2);
			if (bStatus2)
			{
				if (!bToEnd)	break;
			}
		}
		CFunction::DelaySec(0.010);
		Sleep(10);
	}

	iRet = g_pFrm->m_Robot->ExOutput("���ϵ������",FALSE);
	iRet = g_pFrm->m_Robot->ExInput("11�Ź⿪", bStatus2);
	if (bStatus1 && !bStatus2)
	{
		tmTemp.TimerStart();
		while (!bStatus2)
		{
			if (tmTemp.TimerGetSecond() > 2)
				break;

			iRet = g_pFrm->m_Robot->ExInput("11�Ź⿪", bStatus2);
		
			CFunction::DelaySec(0.010);
			Sleep(10);
		}
	}
	bBdDetect = bStatus2;
	return iRet;
}
	
short CCmdRun::LoadUnLoad_loadPullBack()
{	
	if (!m_pController->IsInitOk())						return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;

	short iRet = g_pFrm->m_Robot->ExOutput("���ϵ������",TRUE);
	iRet |= g_pFrm->m_Robot->ExOutput("���ϵ������",TRUE);

	BOOL bStatus1 = FALSE;
	iRet |= g_pFrm->m_Robot->ExInput("���ϵ��-L", bStatus1);

	CKFTimer tmTemp;
	tmTemp.TimerStart();
	while (!bStatus1)
	{
		if (tmTemp.TimerGetSecond() > 4)
		{
			iRet = g_pFrm->m_Robot->ExOutput("���ϵ������", FALSE);
			iRet |= g_pFrm->m_Robot->ExOutput("���ϵ������", FALSE);
			AfxMessageBox("���ϵ���˻س�ʱ!");
			return 0;
		}

		CFunction::DelaySec(0.02);
		iRet |= g_pFrm->m_Robot->ExInput("���ϵ��-L", bStatus1);
	}

	iRet |= g_pFrm->m_Robot->ExOutput("���ϵ������",FALSE);
	iRet |= g_pFrm->m_Robot->ExOutput("���ϵ������",FALSE);
	return iRet;
}

short CCmdRun::LoadUnLoad_unloadPushOut()
{
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())	return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;

	short iRet = g_pFrm->m_Robot->ExOutput("2���Ƴ������׵�ŷ�",TRUE);
	if (iRet != RTN_NONE)	
		AfxMessageBox("���׶�������!");

	BOOL bStatus = FALSE;
	iRet &= g_pFrm->m_Robot->ExInput("�Ƴ�������-R", bStatus);
	CKFTimer tmTemp;
	tmTemp.TimerStart();
	while (!bStatus)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			AfxMessageBox("�Ƴ�������2 ��λ��ʱ");
		}

		iRet &= g_pFrm->m_Robot->ExInput("�Ƴ�������-R", bStatus);
		CFunction::DelaySec(0.020);
	}

	iRet &= g_pFrm->m_Robot->ExOutput("1���Ƴ������׵�ŷ�",TRUE);
	iRet &= g_pFrm->m_Robot->ExInput("�Ƴ�������-L", bStatus);
	tmTemp.TimerStart();
	while (!bStatus)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			AfxMessageBox("�Ƴ�������1 ��λ��ʱ");
		}

		iRet &= g_pFrm->m_Robot->ExInput("�Ƴ�������-L", bStatus);
		CFunction::DelaySec(0.020);
	}
	return iRet;
}

short CCmdRun::LoadUnLoad_unloadPullBack()
{	
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())	return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;

	short iRet = g_pFrm->m_Robot->ExOutput("1���Ƴ������׵�ŷ�",FALSE);
	iRet &= g_pFrm->m_Robot->ExOutput("2���Ƴ������׵�ŷ�",FALSE);
	if (iRet != RTN_NONE)	
	{
		AfxMessageBox("���׶�������!");
	}
	return iRet;
}

// ���ϵ�������ƶ�ǰ��״̬ȷ�ϣ�һ��Ҫ�������������ƶ�����ֹ����
short CCmdRun::LoadUnLoad_loadCheck()
{
	BOOL bStatus1=TRUE, bStatus2=FALSE;
	short iRtn = 0;

	iRtn = ExInput("11�Ź⿪", bStatus1);
	if ((iRtn!=RTN_NONE) || (bStatus1))
	{
		AfxMessageBox("��ڴ�������,��������źŻ�����·!");
		return 0;
	}

	iRtn &= ExInput("���ϵ��-L", bStatus2);
	if (!bStatus2)
	{
		iRtn &= LoadUnLoad_loadPullBack();
	}
	return iRtn;
}

// ���ϵ�������ƶ�ǰ��״̬ȷ�ϣ�һ��Ҫ�������������ƶ�����ֹ����
short CCmdRun::LoadUnLoad_unloadCheck()
{
	BOOL bStatus=TRUE;
	short iRtn=0;

	ExOutput("1���Ƴ������׵�ŷ�",FALSE);
	ExOutput("2���Ƴ������׵�ŷ�",FALSE);
	iRtn = ExInput("14�Ź⿪", bStatus);
	if (iRtn!=RTN_NONE || bStatus)
	{
		AfxMessageBox("���ڴ�������,��������źŻ�����·!");
		return 0;
	}
	return iRtn;
}

short CCmdRun::RotateTable_Home()	// ��תƽ̨��ԭ��
{
	if(g_pFrm->m_Robot->m_pController->IsInitOk() == FALSE) 		return 0;
	short rtn = 0;	

	m_bRotateTableHomeFinish = false;
	m_bRotateTableHomeSucceed = false;

	rtn = m_pController->AxisHome(R_AXIS, g_pFrm->m_mtrParamGts[R_AXIS].m_Vhome, g_pFrm->m_mtrParamGts[R_AXIS].m_VhomeLow,
		g_pFrm->m_mtrParamGts[R_AXIS].m_AccHome,  g_pFrm->m_mtrParamGts[R_AXIS].m_HomeReturn);

	m_bRotateTableHomeFinish = true;
	if (RTN_NONE == rtn)
		m_bRotateTableHomeSucceed = true;
	else 
		m_bRotateTableHomeSucceed = false;


	CFunction::DelaySec(0.5);

//	if (m_bRotateTableHomeSucceed)
//		m_pController->AxisMove(R_AXIS, 2, g_pFrm->m_mtrParamGts[R_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[R_AXIS].m_Acc);
	return rtn;
}

short CCmdRun::LoadUnLoad_Home(int mtrIndex)	// �����ϻ�ԭ��
{
	if(g_pFrm->m_Robot->m_pController->IsInitOk() == FALSE) 		return 0;
	short rtn = 0;	

	if (mtrIndex)
	{
		m_bUldHomeSucceed = false;	
		m_bUldHomeFinish = false;	

		rtn = m_pController->AxisHome(UNLOAD_AXIS, g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vhome, g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_VhomeLow,
			g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_AccHome,  g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_HomeReturn);

		m_bUldHomeFinish = true;	
		if (RTN_NONE == rtn)
			m_bUldHomeSucceed = true;	
		else
			m_bUldHomeSucceed = false;

		CFunction::DelaySec(0.5);
		if (m_bUldHomeSucceed)
			rtn = LoadUnLoad_unloadBoxMove(0);
	}
	else
	{
		m_bLdHomeSucceed = false;	
		m_bLdHomeFinish = false;	

		rtn = m_pController->AxisHome(LOAD_AXIS, g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vhome, g_pFrm->m_mtrParamGts[LOAD_AXIS].m_VhomeLow,
			g_pFrm->m_mtrParamGts[LOAD_AXIS].m_AccHome,  g_pFrm->m_mtrParamGts[LOAD_AXIS].m_HomeReturn);

		m_bLdHomeFinish = true;	
		if (RTN_NONE == rtn)
			m_bLdHomeSucceed = true;	
		else
			m_bLdHomeSucceed = false;

		CFunction::DelaySec(0.5);
		if (m_bLdHomeSucceed)
			rtn = LoadUnLoad_loadBoxMove(0);
	}

	return rtn;
}

void CCmdRun::SetRunMode(RunMode md)
{
	m_runMode = md;
	m_pController->SetRunMode(md);
}
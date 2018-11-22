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
CWinThread *g_pMonitorThread = NULL;		 // 检测线程
CWinThread *g_pPlayThread = NULL;			 // 检测线程
CWinThread *g_pUpdateThread = NULL;			 // 检测线程
CWinThread *g_pLineSignStsThread = NULL;	 // 流水线信号状态监测线程
CWinThread *g_pAutoRPSThread = NULL; 
CWinThread *g_pLoadThread = NULL;
CWinThread *g_pLoadReadyThread = NULL;
CWinThread *g_pUnloadThread = NULL;
CWinThread *g_pUnloadReadyThread = NULL;
CWinThread *g_pLightsetThread = NULL;	 
CWinThread *g_pHomeLdThread = NULL;
CWinThread *g_pHomeUldThread = NULL;
CWinThread *g_pHomeRotateTable = NULL;

BOOL g_bStop = TRUE;	// 是否停止点胶
BOOL g_bClose = FALSE;   //是否关闭了控制卡
BOOL g_bUpdate = FALSE;   

double m_GotoOffSet[MAX_AXIS_NUM];//跳转偏移
CEvent m_EvtAskSeparaRun;// = CEvent(FALSE, TRUE);            // 请求分段加工事件 
//////////////////////////////////////////////////////////////////////////
int LoopCount    =- 20;    
int LoopAddCount =- 20;    

int CCmdRun::m_WCBatetID  = 0;   // 空板称重ID
int CCmdRun::m_WCBatetID2 = 0;   // 满板称重ID　

//////////////////////////////////////////////////////////////////////////
//wparam:axis pos double[4]
//lparam:no used
UINT CCmdRun::PlayThread(LPVOID lparam)
{
	CCmdRun *p0 = (CCmdRun*)lparam;
	//Play函数里面不管什么错误需要返回，否则会有线程内存泄漏

	//////////////////////////////////////////////////////////////////////////
	BOOL bInpStatus = FALSE;
	short iRtn = 0;

	// 是否启用胶量报警
// 	if (g_pFrm->m_pRobotParam->m_structGlueWarning.m_nIfGlueWarning)
// 	{	// 胶量检测传感器
// 		if (0 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)
// 		{
// 			iRtn = p0->ExInput(_T("胶量检测1"), bInpStatus);	
// 			if (RTN_NONE != iRtn || !bInpStatus)
// 			{
// 				if (IDOK != p0->AfxMessageBox(_T("胶量已经报警,\n忽略提示继续点胶点:[确定].\n退出程序点:[取消]"), MB_OKCANCEL))
// 				{
// 					return 0;
// 				}
// 			}
// 		}
// 		else if (g_pFrm->m_pRobotParam->m_structGlueWarning.IfGlueUsedOut())
// 		{	// 预设好的点胶时间
// 			if (2 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)
// 			{
// 				if (IDOK != p0->AfxMessageBox(_T("胶水已经过期,\n忽略提示继续点胶点:[确定].\n退出程序点:[取消]"), MB_OKCANCEL))
// 				{
// 					return 0;
// 				}
// 			}
// 			else if (1 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)	// 板数
// 			{
// 				if (IDOK != p0->AfxMessageBox(_T("胶量已经报警,\n忽略提示继续点胶点:[确定].\n退出程序点:[取消]"), MB_OKCANCEL))
// 				{
// 					return 0;
// 				}
// 			}
// 		}
// 	}
	//////////////////////////////////////////////////////////////////////////
	// 启动点胶线程，首先称重.因为停止时很可能是换胶水了
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

		if (p0->m_bCanWeightAndWash)	// 进料线程检测到入口有料
		{		
			// 称重校准时间到了
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

			// 是否启用胶量报警
			if (g_pFrm->m_pRobotParam->m_structGlueWarning.m_nIfGlueWarning)
			{	// 胶量检测传感器
				if (0 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)
				{
					iRtn = p0->ExInput(_T("胶量检测1"), bInpStatus);	
					if (RTN_NONE != iRtn || !bInpStatus)
					{
						if (IDOK != p0->AfxMessageBox(_T("胶量已经报警,\n忽略提示继续点胶点:[确定].\n退出程序点:[取消]"), MB_OKCANCEL))
						{
							p0->Stop();
							return 0;
						}
					}
				}
				else if (g_pFrm->m_pRobotParam->m_structGlueWarning.IfGlueUsedOut())
				{	// 预设好的点胶时间
					if (2 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)
					{
						if (IDOK != p0->AfxMessageBox(_T("胶水已经过期,\n忽略提示继续点胶点:[确定].\n退出程序点:[取消]"), MB_OKCANCEL))
						{
							p0->Stop();
							return 0;
						}
					}
					else if (1 == g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode)	// 板数
					{
						if (IDOK != p0->AfxMessageBox(_T("胶量已经报警,\n忽略提示继续点胶点:[确定].\n退出程序点:[取消]"), MB_OKCANCEL))
						{
							p0->Stop();
							return 0;
						}
					}
				}
			}

			// 清理吸嘴间隔
			if ((0!=g_pFrm->m_pRobotParam->m_structClean.m_nCleanInterval) 
				&& g_pFrm->m_pRobotParam->m_structClean.m_nIfAutoClean)
			{
				if (0 == (g_pFrm->m_pSysParam->lRunCounter%g_pFrm->m_pRobotParam->m_structClean.m_nCleanInterval))
				{
					if (0 == g_pFrm->m_pRobotParam->m_nWorkMode)
					{
						// 清理喷嘴
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

		// 自动清理喷嘴
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
		// 喷嘴防固化清喷计时
		p0->m_tmAutoRps.TimerStart();
	}

	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION, _T("停止"));
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
	// 回零
	p0->Home();

	// 清喷位置
	double x = g_pFrm->m_pRobotParam->m_posWeightAdj.x;
	double y = g_pFrm->m_pRobotParam->m_posWeightAdj.y;
	double z = g_pFrm->m_pRobotParam->m_zContactPos-g_pFrm->m_pRobotParam->m_posWeightAdj.z;

//	double distime = g_pFrm->m_pRobotParam->m_dAutoRpsIntervalMin*60;	// 间隔时间
	int times = g_pFrm->m_pRobotParam->m_nAutoRpsDots;				// 单次清喷次数
// 	double opentimes  =g_pFrm->m_pSysParam->dSPRSTime;       // 开阀时间 ms
// 	double closetimes =g_pFrm->m_pSysParam->dSPRSDelay;      // 关阀时间 ms

	// 移动到位到清喷位置
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
// 			// 更新时间
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

// Io 监听线程
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

// 灯光设置线程
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
			p2->ExOutput(_T("三色灯.红"),FALSE);
			p2->ExOutput(_T("三色灯.黄"),TRUE);
			p2->ExOutput(_T("三色灯.绿"),FALSE);
			p2->ExOutput(_T("蜂鸣器"),FALSE);
			break;
		case K_RUN_STS_RUN:
		case K_RUN_STS_RUN_1_UNIT:	// 阵列点胶时，点完一个unit才停止程序
		case K_RUN_STS_RUN_1_BOARD:	// 点完当前料片就停止程序
			p2->ExOutput(_T("三色灯.红"),FALSE);
			p2->ExOutput(_T("三色灯.黄"),FALSE);
			p2->ExOutput(_T("三色灯.绿"),TRUE);
			p2->ExOutput(_T("蜂鸣器"),FALSE);
			break;
		default:
			p2->ExOutput(_T("三色灯.红"),TRUE);
			p2->ExOutput(_T("三色灯.黄"),FALSE);
			p2->ExOutput(_T("三色灯.绿"),FALSE);
			p2->ExOutput(_T("蜂鸣器"),TRUE);
			Sleep(500);
			p2->ExOutput(_T("三色灯.红"),FALSE);
			p2->ExOutput(_T("三色灯.黄"),FALSE);
			p2->ExOutput(_T("三色灯.绿"),FALSE);
			p2->ExOutput(_T("蜂鸣器"),FALSE);
			break;
		}
		Sleep(500);
	}

	return 1;
}

UINT CCmdRun::LoadThread_3Box(LPVOID lparam)		// 离线式3层料盒上料线程
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
	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("停止");
	p2->m_bLoadFinish = true;

	return 0;
}

UINT CCmdRun::ThreadRotateTableHome(LPVOID lparam)	// 旋转平台回原点
{
	CCmdRun *p4 = (CCmdRun*)lparam;
	p4->RotateTable_Home();
	return 0;
}

UINT CCmdRun::ThreadHomeLd(LPVOID lparam)	// 离线式上料马达回零
{
	CCmdRun *p3 = (CCmdRun*)lparam;
	p3->LoadUnLoad_Home(0);
	return 0;
}
UINT CCmdRun::ThreadHomeUld(LPVOID lparam)	// 离线式下料马达回零
{
	CCmdRun *p3 = (CCmdRun*)lparam;
	p3->LoadUnLoad_Home(1);
	return 0;
}

// 进料线程
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
	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("停止");
	p2->m_bLoadFinish = true;
	return 0;
}

UINT CCmdRun::UnloadThread_3Box(LPVOID lparam)	// 离线式3层料盒下料线程
{
	CCmdRun *p2 = (CCmdRun*)(lparam);
	p2->m_bUnloadFinish = false;
	p2->m_bUnloadSuccess = false;
	short rtn = 0;

	// 检查点胶工位是否有板		
	BOOL bInpStatus[4];
	if (RTN_NONE != p2->ExInput(_T("14号光开"), bInpStatus[0]))
	{
		g_bStop = TRUE;
		return 0;
	}
	if (RTN_NONE != p2->ExInput(_T("13号光开"), bInpStatus[1]))
	{
		g_bStop = TRUE;
		return 0;
	}

	if (bInpStatus[0] || bInpStatus[1])
	{
		if (IDYES == p2->AfxMessageBox(_T("点胶工位有板,是否需要出板!"), MB_YESNO))
		{
			// 松板 准备出板
			p2->ExOutput(_T("12号阻挡气缸电磁阀"), FALSE);
			p2->UnFixBoard();

			p2->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("启动皮带"));

			CKFTimer tmTemp;
			tmTemp.TimerStart();
			while (TRUE)
			{
				rtn = p2->ExInput(_T("14号光开"), bInpStatus[0]);
				if (RTN_NONE != rtn)
				{
					g_bStop = TRUE;
					return 0;
				}
				if (bInpStatus[0])	break;
				else				Sleep(20);

				if (tmTemp.TimerGetSecond() > 6.0)
				{
					p2->AfxMessageBox("点胶工位卡板!出口光开未感应到物料超时!");
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

			rtn = p2->ExInput(_T("14号光开"), bInpStatus[0]);
			if (RTN_NONE != rtn)
			{
				g_bStop = TRUE;
				return 0;
			}
			if (bInpStatus[0])
			{
				p2->AfxMessageBox("推出料后出口光开还感应到物料，出口卡料或者传感器信号有误，流程将停止");
				g_bStop = TRUE;
				return 0;
			}

			// 料盒下移一层
			int crtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId;
			int crtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;

			if ((g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount-1) == crtLayer)
			{
				if (2 == crtBox)
				{
					p2->ExOutput(_T("三色灯.红"),TRUE);
					p2->ExOutput(_T("蜂鸣器"),TRUE);
					rtn = p2->LoadUnLoad_unloadBoxMove(0, false);
					if (RTN_NONE != rtn)	
					{
						g_bStop = TRUE;
						return 0;
					}

					if (IDOK == p2->AfxMessageBox(_T("已到料盒终点，更换好空料盒后按 [确认]  退出流程按 [取消]"), MB_OKCANCEL))
					{
						p2->m_pController->WaitAxisStop(UNLOAD_AXIS);
						p2->ExOutput(_T("三色灯.红"), FALSE);
						p2->ExOutput(_T("蜂鸣器"), FALSE);

						rtn = p2->ExInput("Unload端料盒检测", bInpStatus[0]);
						if (!bInpStatus[0])
						{
							p2->AfxMessageBox(_T("请在第一层放好料盒后继续"));
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
					rtn = p2->ExInput("Unload端料盒检测", bInpStatus[0]);
					if (!bInpStatus[0])
					{
						p2->AfxMessageBox(_T("请在当前层放好料盒后继续"));
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
			p2->ExOutput("12号阻挡气缸电磁阀", TRUE);
			tmTemp.TimerStart();
			while (tmTemp.TimerGetSecond() < 2.0)
			{
				g_pFrm->m_Robot->ExInput("12号阻挡气缸检测", bDiStatus);
				if (bDiStatus)
				{
					break;
				}
			}
			g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax/2, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

			tmTemp.TimerStart();
			while (tmTemp.TimerGetSecond() < 2.0)
			{
				g_pFrm->m_Robot->ExInput("13号光开", bDiStatus);
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

	// 出板完成
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

	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("停止");
	p2->m_bUnloadFinish = true;
	return 0;
}

// 出料线程
UINT CCmdRun::UnloadThread(LPVOID lparam)
{
	CCmdRun *p2 = (CCmdRun*)(lparam);
	p2->m_bUnloadFinish = false;
	p2->m_bUnloadSuccess = false;
	short rtn = 0;

	// 附加翻板模块
	if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
	{
		if (p2->m_lsCtrl.CheckBoard())	
		{
			p2->AfxMessageBox("翻板轨道有板，将在出板后继续流程");

			BOOL bInpStatus = FALSE;	
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("等待下位机要板信号");
			while (TRUE)
			{
				rtn = p2->ExInput(_T("下位机要板信号1"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					p2->AfxMessageBox(_T("读取下位机要板信号出错"));
					g_bStop = TRUE;
					return 0;
				}

				// 有下位机要板信号
				if (!bInpStatus)
				{
					p2->ExOutput("本工位出板信号1", TRUE);
					break;
				}
				Sleep(100);
			}

			p2->m_lsCtrl.PrepareOutBoard();
			if (1 != p2->m_lsCtrl.BackOutBoard())
			{
				p2->ExOutput("本工位出板信号1", FALSE);
				p2->AfxMessageBox("翻板机出板出错!流程停止");
				g_bStop = TRUE;

				return 0;
			}
			p2->ExOutput("本工位出板信号1", FALSE);
		}
	}

	// 检查点胶工位是否有板		
	BOOL bInpStatus[4];
	if (RTN_NONE != p2->ExInput(_T("14号光开"), bInpStatus[0]))
	{
		g_bStop = TRUE;
		return 0;
	}
	if (RTN_NONE != p2->ExInput(_T("13号光开"), bInpStatus[1]))
	{
		g_bStop = TRUE;
		return 0;
	}
	if (bInpStatus[0] || bInpStatus[1])
	{
		if (IDYES == ::AfxMessageBox("点胶工位有板,是否需要出板!", MB_YESNO))
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("等待下位机要板信号");
			while (TRUE)
			{
				rtn = p2->ExInput(_T("下位机要板信号1"), bInpStatus[2]);
				if (RTN_NONE != rtn)
				{
					p2->AfxMessageBox(_T("读取下位机要板信号出错"));
					g_bStop = TRUE;
					return 0;
				}

				// 有下位机要板信号
				if (!bInpStatus[2])
				{
					p2->ExOutput("本工位出板信号1", TRUE);
					break;
				}
				Sleep(100);
			}
			if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
			{
				p2->m_lsCtrl.PrepareOutBoard();
			}
			p2->ExOutput("11号顶升气缸电磁阀", FALSE);	
			p2->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("启动皮带"));

			if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
			{
				if (1 != p2->m_lsCtrl.BackOutBoard())
				{
					p2->ExOutput("本工位出板信号1", FALSE);

					p2->AfxMessageBox("翻板机出板出错!流程停止");
					g_bStop = TRUE;

					return 0;
				}

				p2->ExOutput("本工位出板信号1", FALSE);
			}
			else
			{
				CKFTimer tmTemp;
				tmTemp.TimerStart();
				while (TRUE)
				{
					rtn = p2->ExInput(_T("14号光开"), bInpStatus[0]);
					if (RTN_NONE != rtn)
					{
						g_bStop = TRUE;
						return 0;
					}
					if (bInpStatus[0])	break;
					else				Sleep(10);

					if (tmTemp.TimerGetSecond() > 6.0)
					{
						p2->AfxMessageBox("点胶工位卡板!");
						g_bStop = TRUE;
						return 0;
					}
				}
				tmTemp.TimerStart();
				while (TRUE)
				{
					rtn = p2->ExInput(_T("14号光开"), bInpStatus[0]);
					if (RTN_NONE != rtn)
					{
						p2->ExOutput("本工位出板信号1", FALSE);
						g_bStop = TRUE;
						return 0;
					}
					if (!bInpStatus[0])	break;
					else				Sleep(10);

					if (tmTemp.TimerGetSecond() > 6.0)
					{
						p2->AfxMessageBox("接驳台处卡板!");
						p2->ExOutput("本工位出板信号1", FALSE);
						g_bStop = TRUE;
						return 0;
					}
				}

				p2->ExOutput("本工位出板信号1", FALSE);
			}

			p2->m_pController->AxisStop(T1_AXIS);
		}
	}

	// 出板完成
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

	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("停止");
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
	inEmergecy = -1;//为了快速访问IO口,单独定义

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

	m_bWorkStopCYSign = FALSE;  // 加工区阻挡气缸到位信号
	m_bSqeStopCYSign= FALSE;    // 分段阻挡气缸到位信号
	m_bUpStopCYSign1= FALSE;	// 顶升气缸上位信号
	m_bUpStopCYSign2= FALSE;	// 顶升气缸下位信号

	m_bAutoPreFic = FALSE;		// 仿固话模式

	//////////////////////////////////////////////////////////////////////////
	// Load Unload 变量
	m_nLoadStep = 1;
	m_nUnloadStep = 1;
	m_bLoadFinish = true;
	m_bLoadSuccess = false;
	m_bUnloadFinish = true;
	m_bUnloadSuccess = true;
	m_bCanWeightAndWash = false;

	m_bLdHomeSucceed = false;	// 程序是否已经成功回零上料
	m_bLdHomeFinish = false;	// 程序是否已经完成回零上料
	m_bUldHomeSucceed = false;	// 程序是否已经成功回零下料
	m_bUldHomeFinish = false;	// 程序是否已经完成回零下料

	m_bLoadEnable = false;
	m_bUnloadEnable = false;
	m_bDispEnable = false;

	m_nRePushCount = 0;		// 重复推料计数
	m_nEmptyLayerCount = 0;	// 连续空料层计数
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
	CString IIDInfo = _T("通用IN#"); 
	CString IStsInfo = _T(""); 
	CString strinId,str;

	char temp[MAX_PATH];

	CGIniFile m_IniFile(CFunction::GetDirectory() + IOMAP_FILE_PATH);
// 	if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
// 	{	// 离线进料方式，加载MEMS配置
// 		m_IniFile.SetPath(CFunction::GetDirectory() + IOMAP_FILE_PATH_OFFLINE);
// 	}

	for(int i=0;i<IN_POINT;i++)
	{
		strinId.Format(_T("%d"),i%IN_POINT);    //ID 
		str = IIDInfo+strinId; // 
		m_IniFile.Read(IInfo,str,temp);
		m_InPutCtrl[i].InitIOChannel((WORD)(_ttoi)(strinId),TYPE_INPUT,temp);
	}

	IIDInfo = _T("拓展IN#");
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
	IIDInfo = _T("通用OT#"); 
	IStsInfo = _T(""); 

	for(int j=0;j<OUT_POINT;j++)
	{
		strinId.Format(_T("%d"),j%OUT_POINT);    //ID 
		str = IIDInfo+strinId; // 
		m_IniFile.Read(IInfo,str,temp);
		m_OutPutCtrl[j].InitIOChannel((WORD)(_ttoi)(strinId),TYPE_OUTPUT,temp);
	}

	IIDInfo = _T("拓展OT#");
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
			str.Format(_T("1号点固态电器"));

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

// 针头手动出胶
short CCmdRun::NeedleDisp(int times,BOOL Open)
{
	short dwRet;
	unsigned short uHighTime = (unsigned short)g_pFrm->m_pSysParam->nOpenTime;
	unsigned short uLowTime = (unsigned short)g_pFrm->m_pSysParam->nCloseTime;
	if(Open == TRUE)
	{ 
		if(times == -1)
		{   // 出胶此时为—1时一直打开
			dwRet=m_pController->SetOutport((short)outJetPort,TRUE);
		}
		// 出胶次数
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

// 针头手动出胶
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
// 手动喷射出胶
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
			else//拓展IO
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
	BOOL bInpStatus  = FALSE;	// 输入端口信号
	short rtn		 = 0;		// 返回值		
	CKFTimer tmTemp;			// 定时器

	if (1 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode)
	{
		rtn = ExOutput(_T("11号顶升气缸电磁阀"), TRUE);
		CHECK_RTN_RTNVAL(_T("11号顶升气缸电磁阀打开出错"), rtn, 0);
		rtn = ExOutput(_T("破真空电磁阀"), FALSE);

		rtn = ExOutput(_T("真空吸电磁阀"), TRUE);
		Sleep(200);

		tmTemp.TimerStart();
		while (1)
		{
			rtn = ExInput(_T("11号顶升气缸1检测"), bInpStatus);
			if (RTN_NONE != rtn)
			{
				AfxMessageBox(_T("检测顶升气缸出错!"));
				return 0;
			}

			if (bInpStatus)
				break;

			if (tmTemp.TimerGetSecond() > 4)
			{
				AfxMessageBox(_T("顶升气缸到位超时!"));
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
			ExOutput(_T("11号顶升气缸电磁阀"), TRUE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11号顶升气缸1检测"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("检测顶升气缸出错!"));
					return 0;
				}

				if (!bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("顶升气缸到位超时!"));
					return 0;
				}
			}

			CFunction::DelaySec(0.1);		
			break;

		case FixMode_1Clamp:
			ExOutput(_T("11号侧夹气缸电磁阀"), TRUE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11号侧加气缸检测"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("检测侧夹气缸出错!"));
					return 0;
				}
				if (!bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("侧夹气缸打开到位超时!"));
					return 0;
				}
			}
			CFunction::DelaySec(0.1);

			ExOutput(_T("11号侧夹气缸电磁阀"), FALSE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11号侧加气缸检测"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("检测侧夹气缸出错!"));
					return 0;
				}
				if (bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("侧夹气缸关闭到位超时!"));
					return 0;
				}
			}

			ExOutput(_T("11号顶升气缸电磁阀"), TRUE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11号顶升气缸1检测"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("检测顶升气缸出错!"));
					return 0;
				}

				if (!bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("顶升气缸到位超时!"));
					return 0;
				}
			}

			CFunction::DelaySec(0.1);
			break;

		case FixMode_1Lift:
			ExOutput(_T("11号顶升气缸电磁阀"), TRUE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11号顶升气缸1检测"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("检测顶升气缸出错!"));
					return 0;
				}

				if (!bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("顶升气缸到位超时!"));
					return 0;
				}
			}

			CFunction::DelaySec(0.1);


			ExOutput(_T("11号侧夹气缸电磁阀"), TRUE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11号侧加气缸检测"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("检测侧夹气缸出错!"));
					return 0;
				}
				if (!bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("侧夹气缸打开到位超时!"));
					return 0;
				}
			}
			CFunction::DelaySec(0.1);

			ExOutput(_T("11号侧夹气缸电磁阀"), FALSE);
			tmTemp.TimerStart();
			while (1)
			{
				rtn = ExInput(_T("11号侧加气缸检测"), bInpStatus);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox(_T("检测侧夹气缸出错!"));
					return 0;
				}
				if (bInpStatus)
					break;

				if (tmTemp.TimerGetSecond() > 4)
				{
					AfxMessageBox(_T("侧夹气缸关闭到位超时!"));
					return 0;
				}
			}
			break;
		}
	}
	
	ExOutput(_T("12号阻挡气缸电磁阀"), FALSE);
	ExOutput(_T("11号侧夹气缸电磁阀"), FALSE);


	rtn = ExOutput(_T("真空吸电磁阀"), FALSE);
	Sleep(500);

	rtn = ExOutput(_T("真空吸电磁阀"), TRUE);
	return 1;
}

int  CCmdRun::UnFixBoard(void)
{
	if (1 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode)
	{
		ExOutput(_T("真空吸电磁阀"), FALSE);
		ExOutput(_T("破真空电磁阀"), TRUE);
		Sleep(200);
		ExOutput(_T("破真空电磁阀"), FALSE);
	}

	ExOutput(_T("11号顶升气缸电磁阀"), FALSE);
	return 1;
}

int CCmdRun::ReverseBoard(void)	// 点胶流程中的翻板指令
{
	if (AddOn_Reverse != g_pFrm->m_pRobotParam->m_nAddOn)
	{
		AfxMessageBox("没有配置翻板功能");
		return -1;
	}

	BOOL bInpStatus;
	short rtn = 0;

	ExOutput(_T("11号顶升气缸电磁阀"), FALSE);
	// 等待出板流程成功
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

	// 启动皮带	
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
			AfxMessageBox("背面点胶，至翻板机卡板");
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
			g_pFrm->m_Robot->ExInput("13号光开", bInpStatus);
			if (bInpStatus)
			{
				g_pFrm->m_Robot->m_lsCtrl.AxisStop(smc_mtr_track);
				break;
			}
			if (tmTemp.TimerGetSecond() > 6.0)
			{
				g_pFrm->m_Robot->m_lsCtrl.AxisStop(smc_mtr_track);
				g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
				AfxMessageBox("翻板机到点胶位卡板!");
				return -1;
			}
		}
		tmTemp.TimerStart();
		while (1)
		{
			g_pFrm->m_Robot->ExInput("13号光开", bInpStatus);
			if (!bInpStatus)
			{
				break;
			}
			if (tmTemp.TimerGetSecond() > 4.0)
			{
				AfxMessageBox("翻板机到点胶位时，在点胶工位卡板");
				return -1;
			}
		}

		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, -g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax/4, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		g_pFrm->m_Robot->ExOutput("12号阻挡气缸电磁阀", TRUE);
		tmTemp.TimerStart();
		while (tmTemp.TimerGetSecond() < 2.0)
		{
			g_pFrm->m_Robot->ExInput("12号阻挡气缸检测", bInpStatus);
			if (bInpStatus)
			{
				break;
			}
		}
		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

		tmTemp.TimerStart();
		while (tmTemp.TimerGetSecond() < 2.0)
		{
			g_pFrm->m_Robot->ExInput("13号光开", bInpStatus);
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

int	  CCmdRun::LoadStep_3Box(void)	// 离线式3层料盒上料步骤
{
	BOOL bInpStatus1 = FALSE;	// 输入端口信号
	int  rtn = 0;

	switch (m_nLoadStep)
	{
	case 1:		// 等待进板使能
		if (m_bLoadEnable)
		{
			// 打开阻挡气缸
			if (RTN_NONE != ExOutput(_T("12号阻挡气缸电磁阀"), TRUE))
			{
				g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("打开阻挡气缸出错");
				m_bLoadFinish = true;
				m_bLoadSuccess = false;
				return -1;
			}
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("收到 [允许进板信号]");
			m_bLoadEnable = false;
			m_nRePushCount = 0;		// 重复推料计数
			m_nEmptyLayerCount = 0;	// 连续空料层计数
			// 13感应到板
			rtn = ExInput(_T("13号光开"), bInpStatus1);
			if (RTN_NONE != rtn)
				return -1;
			if (bInpStatus1)	m_nLoadStep = 20;
			else				m_nLoadStep = 10;
		}

		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("等待 [允许进板信号]");
		Sleep(50);
		break;

	case 10:	// 推杆推料
		m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		rtn = LoadUnLoad_loadPushOut(bInpStatus1);
		CHECK_RTN_RTNVAL("推杆推料动作出错",rtn, -1);
		rtn = LoadUnLoad_loadPullBack();
		CHECK_RTN_RTNVAL("推杆退回动作出错",rtn, -1);

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

	case 11:	// 下一料片
		{
			m_nEmptyLayerCount++;
			if (m_nEmptyLayerCount >= g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdEmptyWarning)
			{
				CString str;
				str.Format("连续%d层没有检测到物料,\r\n停止程序点:[终止]\r\n继续下一层点:[重试]\r\n点胶工位已经有板点:[忽略]", m_nEmptyLayerCount);
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
				CHECK_RTN_RTNVAL(_T("进料盒下移一层出错"), rtn, -1);
				m_nLoadStep = 10;
			}
			else
			{	// 下一料盒
				m_nLoadStep = 12;
				break;
			}
		}
		break;

	case 12:	// 料盒下移一层
		{
			int crtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId;
			if (crtBox<2)
			{
				rtn = LoadUnLoad_loadBoxMove(crtBox+1);
				CHECK_RTN_RTNVAL(_T("料盒移动出错!"), rtn, -1);
				m_nLoadStep = 13;
				break;
			}
			else
			{
				LoadUnLoad_loadBoxMove(0, false);
				if (IDOK == AfxMessageBox("料盒用完，换好料盒后点:[确认]\r\n  停止流程点:[取消]"))
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
		rtn = ExInput("Load端料盒检测", bInpStatus1);
		CHECK_RTN_RTNVAL(_T("Load端料盒检测出错"), rtn, -1);

		// 移动到第一层后检测一下料盒
		if (bInpStatus1)
			m_nLoadStep = 10;
		else
			m_nLoadStep = 12;

		break;

	case 15: 
		if (m_tmLoad.TimerGetSecond() > 6)
		{
			if (IDOK == AfxMessageBox(_T("入口进板卡料!重新检测点:[确认], \r\n停止点:[取消]"), MB_OKCANCEL))
			{
				m_tmLoad.TimerStart();
				break;
			}
			else
			{
				return -1;
			}
		}

		// 13感应到板
		m_bCanWeightAndWash = true;
		rtn = ExInput(_T("13号光开"), bInpStatus1);
		if (RTN_NONE != rtn)
			return -1;
		if (bInpStatus1)
		{
			m_nLoadStep = 20;
			break;
		}

		Sleep(50);
		break;

	case 20:	// 皮带低速运行
		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		Sleep(300);
		// 进料完成
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
		m_nLoadStep = 30;
		break;

	case 30:	// 夹板
		rtn = FixBoard();
		if (1 != rtn)
		{
			AfxMessageBox(_T("夹板动作失败"));
			return -1;
		}
		m_bDispEnable = true;
		m_nLoadStep = 40;
		break;

	case 40:	// 下一料片
		{
			int crtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId;
			if (crtLayer < g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount-1)
			{
				LoadUnLoad_loadLayerMove(crtLayer+1);
				m_nLoadStep = 1;
				break;
			}
			else
			{	// 下一料盒
				m_nLoadStep = 42;
				break;
			}
		}
		break;

	case 42:	// 料盒下移一层
		{
			int crtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId;
			if (crtBox<2)
			{
				rtn = LoadUnLoad_loadBoxMove(crtBox+1);
				if (RTN_NONE != rtn)
				{
					AfxMessageBox("上料盒移动出错!");
					g_bStop = TRUE;
					return -1;
				}
				m_nLoadStep = 44;
				break;
			}
			else
			{
				LoadUnLoad_loadBoxMove(0, false);
				if (IDOK == AfxMessageBox("料盒用完，换好料盒后点:[确认]  停止流程点:[取消]"))
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
		rtn = ExInput("Load端料盒检测", bInpStatus1);
		if (RTN_NONE != rtn)
		{
			AfxMessageBox("料盒检测信号出错!");
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
			str.Format("错误的进板动作序号:[%d]", m_nLoadStep);
			AfxMessageBox(str);
		}
		break;
	}

	return 1;
}

int  CCmdRun::LoadStep(void)
{
	BOOL bInpStatus1 = FALSE;	// 输入端口信号
	BOOL bInpStatus2 = FALSE;	// 输入端口信号
	BOOL bInpStatus3 = FALSE;	// 输入端口信号
	int  rtn = 0;
	switch (m_nLoadStep)
	{
	case 1:		// 等待进板使能
		if (m_bLoadEnable)
		{
			// 打开阻挡气缸
			if (RTN_NONE != ExOutput(_T("12号阻挡气缸电磁阀"), TRUE))
			{
				g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("打开阻挡气缸出错");
				m_bLoadFinish = true;
				m_bLoadSuccess = false;
				return -1;
			}
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("进板使能 OK");
			m_bLoadEnable = false;

			// 13感应到板
			rtn = ExInput(_T("13号光开"), bInpStatus1);
			if (RTN_NONE != rtn)
				return -1;
			if (bInpStatus1)	m_nLoadStep = 20;
			else				m_nLoadStep = 10;
		}

		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("等待进板使能");
		Sleep(50);
		break;

	case 10:	// 等待上位机信号和入口光纤有感应
		rtn = ExOutput(_T("本工位要板信号1"), TRUE);
		if (RTN_NONE != rtn)
			return -1;

		rtn = ExInput(_T("上位机出板信号1"), bInpStatus1);
		if (RTN_NONE != rtn)
			return -1;
		rtn = ExInput(_T("11号光开"), bInpStatus2);
		if (RTN_NONE != rtn)
			return -1;

		if (bInpStatus1 || bInpStatus2)
		{	// 启动皮带	
			g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
			m_tmLoad.TimerStart();
			m_nLoadStep = 15;
			m_bCanWeightAndWash = true;
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("进板开始!");
		}
		else
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("等待上位机出板信号!");
			Sleep(20);
		}
		break;

	case 15: 
// 		if (m_tmLoad.TimerGetMs() > g_pFrm->m_pRobotParam->m_dTimeAskPre)
// 		{
// 			ExOutput(_T("本工位要板信号1"), FALSE);
// 		}

		if (m_tmLoad.TimerGetSecond() > 6)
		{
			AfxMessageBox(_T("入口进板超时!"));
			return -1;
		}

		// 13感应到板
		rtn = ExInput(_T("13号光开"), bInpStatus1);
		if (RTN_NONE != rtn)
			return -1;
		if (bInpStatus1)
		{
			rtn = ExOutput(_T("本工位要板信号1"), FALSE);
			if (RTN_NONE != rtn)
				return -1;

			rtn = ExInput(_T("11号光开"), bInpStatus2);
			rtn = ExInput(_T("12号光开"), bInpStatus3);
			if (bInpStatus3||bInpStatus2)
			{
				AfxMessageBox("11或12光电开关与13同时有信号,可能13光电开关误感应!!");
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

	case 20:	// 皮带低速运行
		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax/2.0, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		Sleep(300);
		// 进料完成
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
		m_nLoadStep = 30;
		break;

	case 30:	// 夹板
		rtn = FixBoard();
		if (1 != rtn)
		{
			AfxMessageBox(_T("夹板动作失败"));
			return -1;
		}
		m_bDispEnable = true;
		m_nLoadStep = 1;
		break;
	}

	return 1;
}

int	  CCmdRun::UnloadStep_3Box(void)	// 离线式3层料盒下料步骤
{
	BOOL bInpStatus = FALSE;	// 输入端口信号
	short rtn = 0;

	switch (m_nUnloadStep)
	{
	case 1:	
		if (m_bUnloadEnable)
		{	
			m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("点胶完成，收到出板信号!");
			UnFixBoard();
			m_bUnloadEnable = false;
			m_bUnloadFinish = false;
			m_nUnloadStep = 10;
			m_tmUnload.TimerStart();

			break;			
		}

		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("等待点胶完成,出板信号");
		Sleep(50);
		break;

	case 10:	// 等待出板光开感应到信号
		rtn = ExInput(_T("14号光开"), bInpStatus);
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
				AfxMessageBox("点胶工位卡板!出口光开未感应到物料超时!");
				m_bDownLayerAfterWarning = false;
				m_pController->AxisStop(1<<T1_AXIS);
				g_bStop = TRUE;
				return 0;
			}
			Sleep(10);
		}
		break; 

	case 20:	// 等待料片流出出板光开
		rtn = ExInput(_T("14号光开"), bInpStatus);
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
				AfxMessageBox("出口卡板!出口光开处一直有物料!");
				m_bDownLayerAfterWarning = false;
				m_pController->AxisStop(1<<T1_AXIS);
				g_bStop = TRUE;
				return 0;
			}
			Sleep(10);
		}
		break;

	case 22:	// 延时推出料延时时间
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

	case 30:	// 检测是否卡料
		rtn = ExInput(_T("14号光开"), bInpStatus);
		if (RTN_NONE != rtn)
		{
			g_bStop = TRUE;
			return 0;
		}
		if (bInpStatus)
		{
			AfxMessageBox("推出料后出口光开还感应到物料，出口卡料或者传感器信号有误，请人工处理");
		}
		m_nUnloadStep = 40;
		break;

	case 40:	// 料盒下移一层
		{
			m_bLoadEnable = true;
			int crtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId;
			int crtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;

			if ((g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount-1) == crtLayer)
			{
				if (2 == crtBox)
				{
					LoadUnLoad_unloadBoxMove(0);
					if (IDOK == AfxMessageBox(_T("已到料盒终点，更换好空料盒后按:[确认]  \r\n退出流程按:[取消]"), MB_OKCANCEL))
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

	case 50:// 检测料盒
		rtn = ExInput("Unload端料盒检测", bInpStatus);
		if (!bInpStatus)
		{
			int crtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;
			if (2 == crtBox)
			{
				LoadUnLoad_unloadBoxMove(0, false);

				if (IDOK == AfxMessageBox("最下层没有检测到物料盒,请换好料盒后点:[确认]  停止点:[取消]", MB_OKCANCEL))
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
			str.Format("出料步骤出错:[%d]未定义", m_nUnloadStep);
			AfxMessageBox(str);
		}
		break;
	}	

	return 1;
}

int  CCmdRun::UnLoadStep(void)
{
	BOOL bInpStatus = FALSE;	// 输入端口信号
	short rtn = 0;

	switch (m_nUnloadStep)
	{
	case 1:		// 等待出板
		if (m_bUnloadEnable)
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("等待出板 OK");
			UnFixBoard();
			m_bUnloadEnable = false;
			m_bUnloadFinish = false;
			m_nUnloadStep = 5;
			break;
		}

		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("等待出板");
		Sleep(50);
		break;

	case 5:
		rtn = ExInput(_T("13号光开"), bInpStatus);	
		if (RTN_NONE != rtn)
		{
			m_pController->AxisStop(1<<T1_AXIS);
			AfxMessageBox(_T("出板时读取13号光开出错"));
			return -1;
		}
		if (!bInpStatus)	// 不需要出板
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("点胶位无板!出板完成");
			m_bLoadEnable = true;
			m_bUnloadFinish = true;
			m_bUnloadSuccess = true;
			m_nUnloadStep = 1;
			break;
		}

		ExOutput(_T("12号阻挡气缸电磁阀"), FALSE);
		ExOutput(_T("本工位出板信号1"), TRUE);
		m_nUnloadStep = 10;
		break;

	case 10:	// 等待下位机要板
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("等待下位机要板"));
		rtn = ExInput(_T("下位机要板信号1"), bInpStatus);
		if (RTN_NONE != rtn)
		{
			AfxMessageBox(_T("读取下位机要板信号出错"));
			return -1;
		}
		if (bInpStatus)
		{
			m_nUnloadStep = 20;
		}
		Sleep(20);
		break;

	case 20:		// 启动皮带	
		m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("启动皮带"));
		if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
		{
			m_lsCtrl.PrepareOutBoard();
		}
		m_tmUnload.TimerStart();
		m_nUnloadStep = 30;
		break;

	case 30:	// 14号光开读到板
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("等待板流到14光开"));
		rtn = ExInput(_T("14号光开"), bInpStatus);	
		if (RTN_NONE != rtn)
		{
			m_pController->AxisStop(1<<T1_AXIS);
			AfxMessageBox(_T("出板时读取14号光开出错"));
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
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("14号光开未读到板"));
			m_pController->AxisStop(1<<T1_AXIS);
			AfxMessageBox(_T("出板时卡板"));
			return -1;
		}
		break;

	case 40:	// 板流过14号光开
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("等待板流出14光开"));
		rtn = ExInput(_T("14号光开"), bInpStatus);	
		if (RTN_NONE != rtn)
		{
			g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
			AfxMessageBox(_T("出板时读取14号光开出错"));
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

		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("等待板流出14光开");
		if (m_tmUnload.TimerGetSecond() > 4)
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("14光开卡板"));
			m_pController->AxisStop(1<<T1_AXIS);
			AfxMessageBox(_T("出板时卡板"));
			g_bStop = TRUE;
			return -1;
		}
		break;

	case 50:	// 关闭出板信号
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus("出板完成");
		ExOutput(_T("本工位出板信号1"), FALSE);
		m_pController->AxisStop(1<<T1_AXIS);
		m_bLoadEnable = true;
		m_bUnloadSuccess = true;
		m_bUnloadFinish = true;
		m_nUnloadStep = 1;
		break;

	case 60:	// 翻板机出板
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetUnloadStatus(_T("翻板机出板"));
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
*函数名称：CheckIO
*函数功能：检测控制器输入端口状态
*       m_inputSts=1 ,InTemp=0  sts=1   正常
*       m_inputSts=0 ,InTemp=1  sts=-2  端口发生变化(下降沿)
*       m_inputSts=0 ,InTemp=0  sts=0   端口发生变化(变化保持中)
*       m_inputSts=1 ,InTemp=1  sts=2   端口发生变化(上升沿)
*参数说明：
*返回值：
*版本历史：
====================================*/
void CCmdRun::CheckIO()
{
	static BOOL bEmergecy       = FALSE;   // 设备急停
	static BOOL bDoorSf         = FALSE;   // 安全门
	static long inOldStatus = 0xFFFF;  
	long inTemp = 0xFFFF;

	int sts=0;
	if(m_hMsgReceiveWnd == NULL)	// 还没有信号接收窗口
	{
		return;
	}

	if(m_pController->IsInitOk())
	{
		// 获取当前信号状态
		m_pController->GetInputSts(&m_inputSts);         // 读取板卡输入口状态
		m_pController->GetOutputSts(&m_outputSts);       // 获取板载输出口状态
		m_pController->GetExInputSts(0, &m_exInputSts);	 // 获取扩展口输入状态
		bRobotIsMoving = m_pController->IsMoving(CRD1);

		//急停被按下
		if((m_inputSts&(0x01<<inEmergecy))==(0x01<<inEmergecy))
		{
			bEmergecy = TRUE;		
			::SendMessage(m_hMsgReceiveWnd, WM_MSG_INPUT_EVENT, inEmergecy, TRUE);
		}

		// 机器设定界面
		if(g_pFrm->m_dlgMachineSet.GetSafeHwnd() && g_pFrm->m_dlgMachineSet.IsWindowVisible())
		{
			int index = g_pFrm->m_dlgMachineSet.m_tabCtrl.GetCurSel();
			if (2 == index)
			{
				g_pFrm->m_dlgMachineSet.m_pageLoadUnload.UpdataIOStatus(m_exInputSts);
			}
		}

		// 双阀设定界面
		if (g_pFrm->m_DlgDualValve.GetSafeHwnd() && g_pFrm->m_DlgDualValve.IsWindowVisible())
		{
			g_pFrm->m_DlgDualValve.UpdateInputStatus(m_inputSts);
		}

		// IO 状态显示界面
		if(g_pFrm->m_dlgIOCtrl.GetSafeHwnd() && g_pFrm->m_dlgIOCtrl.IsWindowVisible())
		{
			int index = g_pFrm->m_dlgIOCtrl.m_tabCtrl.GetCurSel();

			if(index == 0)   // 输入口状态
			{  
				//刷新当前输入口状态
				g_pFrm->m_dlgIOCtrl.pageInput.UpdateIOSts(m_inputSts);
				//刷新当前扩展口状态
				g_pFrm->m_dlgIOCtrl.pageInput.UpdateExIOSts(m_exInputSts);
			}
			else if(index == 1)	// 输出口状态
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

					// 根据工具类型设置 Io界面下手动喷射功能状态
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
			else if (2 == index)	// 轨道调试界面
			{
				g_pFrm->m_dlgIOCtrl.pageTrack.UpdataIOState(m_inputSts, m_exInputSts, m_outputSts);
			}
		}

		//急停按下被复位
		if (bEmergecy&&((m_inputSts&(0x01<<inEmergecy))!=(0x01<<inEmergecy)))
		{
			bEmergecy = FALSE;
			::SendMessage(m_hMsgReceiveWnd, WM_MSG_INPUT_EVENT, ExInputBitNameToNo(_T("停止运行")), -2);//
		}

		inTemp = inOldStatus ^ m_inputSts;
		inOldStatus = m_inputSts;

		for(int i=0; i<MAX_I0_NUM; i++)
		{
			if(m_inputSts & (1<<i))
				sts=1;
			else 
				sts=0;
			if(inTemp & (1<<i))//1端口发生改变
			{
				if(m_inputSts & (1<<i))
					sts=2;
				else 
					sts=-2;
			}

			// 不是急停信号
			if ((i != inEmergecy))  
				::SendMessage(m_hMsgReceiveWnd, WM_MSG_INPUT_EVENT, i, sts);
		}
	}
}

// 检测状态
void CCmdRun::CheckStatus()
{
	double dAxisPos[8];
	long   axissts[8];
	CString str;

	if(m_hMsgReceiveWnd == NULL) 
		return;

	// 马达位置刷新
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
	// 双阀设置界面,刷新Z轴坐标
	if (g_pFrm->m_DlgDualValve.GetSafeHwnd() && g_pFrm->m_DlgDualValve.IsWindowVisible())
	{
		g_pFrm->m_DlgDualValve.UpdateZPos(dAxisPos[Z_AXIS]);
	}

	
	//单次运行周期时间
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
	//更新网络发送数据状态
	///当前时间
	CTime time; 
	time=CTime::GetCurrentTime(); 
	str = time.Format("%Y-%m-%d %H:%M:%S ");
	g_pFrm->SetPanelTextEx(IDS_CUR_CLOCK,str); 

	CTimeSpan ctSpan = time-g_pFrm->m_pSysParam->m_SystemBeginTime;
	str = ctSpan.Format(_T("%D天-%H时-%M分-%S秒"));
	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_RUNTIME,str);

	//根据工具类型设置状态栏对应状态
	if (g_pFrm->m_pSysParam->nToolType==0)
	{   
		g_pFrm->SetPanelTextEx(IDS_TOOL_STATUS, "当前模式:喷射阀");
// 		if(g_pFrm->m_pSysParam->nJetType == 0)
// 			g_pFrm->SetPanelTextEx(IDS_TOOL_STATUS, "当前模式:气动式喷射阀");
// 		else
// 			g_pFrm->SetPanelTextEx(IDS_TOOL_STATUS, "当前模式:压电式喷射阀");
	}
	else if(g_pFrm->m_pSysParam->nToolType==1)
	{
		g_pFrm->SetPanelTextEx(IDS_TOOL_STATUS, "当前模式:针筒");
	}
	else if(g_pFrm->m_pSysParam->nToolType==2)
	{
		g_pFrm->SetPanelTextEx(IDS_TOOL_STATUS, "当前模式::图像模式");
	}
}
// 阀体压力设置  （适用Jet8系列）
void CCmdRun::JetPressCtrl(double dDate)
{
	unsigned short temp;
	temp = (unsigned short)(dDate*10.0*65535/6);
	g_pFrm->m_Robot->m_pController->SetExtADDAVal(0,0,temp);
}
// 胶筒压力设置  （适用EFD/Jet8系列）
void CCmdRun::TubePressCtrl(double dDate) 
{  
	unsigned short temp;
	temp = (unsigned short)(dDate*10.0*65535/6);
	g_pFrm->m_Robot->m_pController->SetExtADDAVal(0,1,temp);
}
void CCmdRun::Free()
{
	//释放之前让线程自主return结束
	g_bClose = TRUE;
	g_bStop = TRUE;
	g_bUpdate = TRUE;
	//////////////////////////////
	Sleep(50);

	//////////////////////////////////////////////////////////////////////////
	// 进料线程
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
	// 信号塔线程 
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
	// 出料线程
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
	// IO监控线程
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
	// 界面刷新线程
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
	//释放之前让线程g_pPlayThread自主return结束
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

	// 设置背压值
	SetBackPressure(g_pFrm->m_pRobotParam->m_dBackPress);
	// 打开背压
	ExOutput(_T("1号备压电磁阀"),TRUE);

	RunWCL_addSTrack=true;
	//手动界面禁止操作
	g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(FALSE);
	g_pFrm->m_wndEditBar.m_wndList.EnableWindow(FALSE);

	//清除当前检测到的高度偏差
	m_dFirstHeightOffset = 0;
	m_ZMeterVector.clear();

	//预处理编程路径 叠加喷嘴图像偏移
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

	//检测语法
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

	//记录初始时间点
	CString strTime;
	GetLocalTime(&st1); 
	strTime.Format("%4d-%2d-%2d %d:%d:%d",st1.wYear,st1.wMonth,st1.wDay,st1.wHour,st1.wMinute,st1.wSecond);
	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_STARTTIME,strTime);
	SystemTimeToFileTime(&st1,   &g_pFrm->m_pSysParam->t1.ft); 

	m_pController->TS_ListSetSystemRun(CRD1);

	//更改运行状态
	m_tSysStatus = K_RUN_STS_RUN;

	//Z轴提高到安全高度
	sTemp = GotoSafeZpos();
	//if(RTN_NONE!=sTemp)
	//	return;

	///////////////
	m_iCurIndex = 0;//从头开始运行

	CString strTemp;
	strTemp.Format("StartFrom NO:%04d", m_iCurIndex);
	g_pFrm->AddMsg(strTemp);

	SetLamp();

	m_cmdParam.reset();//这里初始必须要将变量重新复位
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
		g_pFrm->AddMsg("CMD 翻板出错!");
		return -1;
	}
	else if (RTN_ZHeight_Err == sTemp)
	{
		g_pFrm->AddMsg("测高出错!超出范围");
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

		if(IDOK == AfxMessageBox(_T("Find Mark Fail,Try again？"),MB_OKCANCEL))
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

	/////////////  运行完成  /////////////////////////////
	if(m_tSysStatus == K_RUN_STS_RUN)
	{ 
		//这里还需要判断缓冲区是否没有任何指令？
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
//			g_pFrm->AddMsg(_T("请求出料"));

// 			// 发送点胶完成信号给PLC
// 			ExOutput(_T("完成加工请求"),TRUE);
// 			CFunction::DelaySec(0.2);                   //s
// 			ExOutput(_T("完成加工请求"),FALSE);
		}

		g_pFrm->m_pSysParam->lRunCounter++;   // 加工计数 累加
		g_pFrm->m_pRobotParam->m_structGlueWarning.m_nCurPcs++;		// 胶量报警中,已点板数计数

		///////////////////////////////////////////////////////
// 		if(g_pFrm->m_pSysParam->nAutoMode)//自动模式下
// 		{
// 			if(g_pFrm->m_pSysParam->uiWashPinCycle>0)
// 			{
// 				int iTempCounter = g_pFrm->m_pSysParam->lRunCounter - g_pFrm->m_pSysParam->uiWashPinCycle*(g_pFrm->m_pSysParam->lRunCounter/g_pFrm->m_pSysParam->uiWashPinCycle);
// 				if((iTempCounter==0)&&(g_pFrm->m_pSysParam->lRunCounter>1))
// 				{
// 					if(0 == g_pFrm->m_pSysParam->iWashType)
// 					{  // 擦拭式清洗
// 						WashJet(g_pFrm->m_pSysParam->iWashNum);   // 清洗喷嘴
// 					}
// 					else
// 					{  // 单吸式清洗
// 						WashNeedle(g_pFrm->m_pSysParam->iWashNum);   // 清洗喷嘴   
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
		//这里可以移动到加工待机位
		GotoSafeZpos();
		if( 1 == g_pFrm->m_pSysParam->nGoStandByPos)
		{
			//g_pFrm->m_Robot->m_pController->LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParam[X_AXIS].m_Vmin,g_pFrm->m_mtrParam[X_AXIS].m_Acc);
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,g_pFrm->m_pSysParam->tDefHomePos.x,g_pFrm->m_pSysParam->tDefHomePos.y,g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		}

		// 称重报错时不清当前状态
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

	m_pController->SetCardNum(CARDNUM);// 设置卡号
	m_pController->SetToolNum(TOOL_NUM);

	dcp = new t_Tool[TOOL_MAXNUM];

	LoadIOMap();

	m_pController->MapToolStruct(dcp);

	// 3轴机械臂 急停输入设置
	inEmergecy = ExInputBitNameToNo(_T("急停按钮"));
	if (-1==inEmergecy)
		return RTN_PARAMETER_ERR;
	m_pController->MapEStop(inEmergecy);             

	// 真空清洁
	inWashPort = ExOutputBitNameToNo(_T("清洁工作站电磁阀"));  
	if (-1==inWashPort)
		return RTN_PARAMETER_ERR;

	// Z轴针头位置传感器信号
	inSenseCheckZ = ExInputBitNameToNo(_T("点胶针头高度标定"));
	if (-1==inSenseCheckZ)
		return RTN_PARAMETER_ERR;


	outJetPort = ExOutputBitNameToNo(_T("1号点固态电器"));
	if (-1==outJetPort)
		return RTN_PARAMETER_ERR;

	TS_MTN_PARAM tsMtnParam;
	tsMtnParam.axisNum = MAX_AXIS_NUM; // 控制总轴数

	for(int i=1;i<=tsMtnParam.axisNum;i++)
	{
		tsMtnParam.dfLength[i-1] = g_pFrm->m_mtrParamGts[i-1].m_MaxTravelPositive-g_pFrm->m_mtrParamGts[i-1].m_MaxTravelNegative;
		tsMtnParam.dMaxpos[i-1]  = g_pFrm->m_mtrParamGts[i-1].m_MaxTravelPositive;
		tsMtnParam.dMinpos[i-1]  = g_pFrm->m_mtrParamGts[i-1].m_MaxTravelNegative;
		tsMtnParam.dfScale[i-1]	 = g_pFrm->m_mtrParamGts[i-1].m_Resolution; //pulse/mm
	}

	//	tsMtnParam.dzSafePost=m_pSysParam->dzSafePost;//Z轴安全高度
	tsMtnParam.iCardType = CRD_TYPE_GTS;
	tsMtnParam.iLmtSns	= 0x0000;	// 限位传感器激活电平
	tsMtnParam.iHomeSns = 0x0000;

	tsMtnParam.maxVel	= 2000;
	tsMtnParam.maxAcc	= g_pFrm->m_pSysParam->dLookMaxAcc;
	tsMtnParam.startVel	= 0;
	tsMtnParam.stpAcc	= 10000;
	tsMtnParam.synAcc	= g_pFrm->m_pSysParam->dLookAcc;
	tsMtnParam.synVel	= 200;

	//伺服电机，使能报警信号
	for (int j=0;j<3/*tsMtnParam.axisNum*/;j++)
	{
		if (3 >= j)
		{
			tsMtnParam.alarmON[j] = TRUE;
		}
		else
		{
			tsMtnParam.alarmON[j] = FALSE;   //第4轴为点胶阀控制电机
		}
	}

	short m_Ret;
	// 初始化控制系统
	if(g_pFrm->m_pSysParam->nRobotCtrlMode == DefCtrlMode_Pulse)
	{	

		for (int i=0; i<MAX_AXIS_NUM; i++)
		{	// 脉冲模式
			tsMtnParam.iCtrlMode[i] = DefCtrlMode_Pulse;
		}

		g_pFrm->AddMsg("Start AttachCard.....");
		m_Ret = m_pController->AttachCard(&tsMtnParam);
	}
	else
	{  
		// 模拟量
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
		// 设置缓冲匀速运动时间
		m_pController->KF_SetLookEvenTime(g_pFrm->m_pSysParam->m_LookEvenTime);
		// 设置缓冲拐弯降速比
		m_pController->KF_SetLookTunScale(g_pFrm->m_pSysParam->m_LookTunScale);

		//初始化坐标系参数
		if(CRD_TYPE_GTS == tsMtnParam.iCardType)
		{
			t_CrdP crd1;
			memset(&crd1,0,sizeof(t_CrdP));
			crd1.dimension = 3;
			crd1.profile[0]	= 1;//规划器1对应X轴
			crd1.profile[1]	= 2;//规划器2对应Y轴
			crd1.profile[2]	= 3;//规划器3对应Z轴
			m_pController->TS_EX_SetCoordinate(1,&crd1);//建立1号坐标系
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
			g_pFrm->AddMsg("更新线程创建失败");
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
			g_pFrm->AddMsg("监视线程创建失败");
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
			g_pFrm->AddMsg("信号塔线程创建失败");
			return RTN_PARAMETER_ERR;
		}
		::SetThreadAffinityMask(g_pLightsetThread->m_hThread, g_pFrm->m_nUIRunProcesser);
		g_pLightsetThread->ResumeThread();
	}

	//g_pLineSignStsThread = AfxBeginThread(CCmdRun::LineSignStsThread, this);
	//if(g_pLineSignStsThread==NULL)
	//{
	//	g_pFrm->AddMsg("输送线信号监视线程创建失败");
	///	return RTN_PARAMETER_ERR;
	//}

	// 	// 分段加工使能
	//     if(g_pFrm->m_pSysParam->nFileSeparaEnable == 1)
	// 	    g_pFrm->m_Robot->ExOutput("分版使能",TRUE);
	// 	else
	//         g_pFrm->m_Robot->ExOutput("分版使能",FALSE);

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
// 	{ //这里的Z轴dfLength应该为limit
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
		AfxMessageBox("已经执行到最后一条轨迹命令了");
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
		AfxMessageBox(_T("System Not Init！"));
		return;
	}

	if (!m_bUnloadFinish)
	{
		AfxMessageBox(_T("出料未完成"));
		return;
	}

	m_bUnloadFinish = false;
	m_bUnloadSuccess = false;
	// 启动上下料线程
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
		AfxMessageBox(_T("System Not Init！"));
		return;
	}

	if (!m_bLoadFinish)
	{
		AfxMessageBox(_T("进料未完成"));
		return;
	}

	m_bLoadFinish = false;
	m_bLoadSuccess = false;
	// 启动上下料线程
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
	//							判断系统启动条件							//
	//////////////////////////////////////////////////////////////////////////
	if (m_tSysStatus == K_RUN_STS_RUN)		return;
	if (m_pController->IsInitOk()==FALSE) 
	{
		AfxMessageBox(_T("板卡没有初始化成功!"));
		return;
	}
	if (theApp.m_EncryptOp.m_bLockSystem)	
	{
		AfxMessageBox(_T("系统锁定,请联系供应商解决!!!"));
		return ;
	}
	if (m_tSysStatus == K_RUN_STS_EMERGENCY) 
	{	
		AfxMessageBox(_T("急停按钮被按下,请复位后再启动"));
		return;
	}
	if (g_pFrm->m_wndEditBar.m_rawList.GetCount()<1)
	{
		AfxMessageBox(_T("没有学习路径数据！"));
		return ;
	}
	if (!m_bHomeSuccess)
	{		
		AfxMessageBox(_T("请先回零"));
		return;
	}		
	if(1 == g_pFrm->m_pRobotParam->m_nIfCheckSafeDoor)
	{	
		if (!m_bDoorSFSts)	// 门打开状态
		{
			AfxMessageBox(_T("门被打开,请检查前门和后门信号!"));
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//							判断结束									//
	//////////////////////////////////////////////////////////////////////////

	m_bFlagRunning = TRUE;		//去能下次重复触发标志
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
	// 启动上下料线程
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

	// 点胶线程
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
	// 清喷位置
	double x = g_pFrm->m_pRobotParam->m_posWeightAdj.x+g_pFrm->m_pSysParam->tOffset[0].x;
	double y = g_pFrm->m_pRobotParam->m_posWeightAdj.y+g_pFrm->m_pSysParam->tOffset[0].y;
	double z = g_pFrm->m_pRobotParam->m_zContactPos-g_pFrm->m_pRobotParam->m_posWeightAdj.z;

	int times = g_pFrm->m_pRobotParam->m_nAutoRpsDots;				// 单次清喷次数

	// 移动到位到清喷位置
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
		AfxMessageBox(_T("System Not Init！"));
		return;
	}
	g_pAutoRPSThread = AfxBeginThread(CCmdRun::AutoRPSThread, this);//,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	if(g_pAutoRPSThread==NULL)
	{
		g_pFrm->AddMsg(_T("Creat Run Thread Error."));
		return;
	}	
}

// 回零前的准备动作
void CCmdRun::PrepareHome(void)
{
	// 停止所有的轴运动
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
			AfxMessageBox("板卡连接失败!请检查急停按钮、驱动器报警、电路等!");
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
				AfxMessageBox(_T("驱动器报警!"));
				return;
			}
		}
	}

	static bool b = false;

	if(b)	return;

	b = true;

	if(g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_RUN )
	{
		AfxMessageBox("运行中停止回零!");
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

	// 回零速度、偏移等参数
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
		AfxMessageBox("Go Home Error!");/*原点开关故障*/
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
		// 分段加工使能
		if(g_pFrm->m_pSysParam->nFileSeparaEnable == 1)
			g_pFrm->m_Robot->ExOutput("分版使能",TRUE);
		else
			g_pFrm->m_Robot->ExOutput("分版使能",FALSE);

		b = false;
		return;
	}

	m_tSysStatus = K_RUN_STS_NONE;

	Stop();//回原点完成后停止状态

	b = false;


	ExOutput("11号阻挡气缸电磁阀",0);
	ExOutput("12号阻挡气缸电磁阀",0);
	ExOutput("11号顶升气缸电磁阀", 0);
	ExOutput("11号侧夹气缸电磁阀", 0);

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
	//g_pFrm->SetPanelTextEx(IDS_ONLINE_MODE,"在线模式");
//	g_pFrm->m_wndRightBar.m_DlgPosInfo.UpdateNetState(TRUE);
	g_pFrm->AddMsg("--->Online Mode---");	

	if(g_pFrm->m_pSysParam->nAutoMode == 0)
	{
		Stop();
		//g_pFrm->SetPanelTextEx(IDS_ONLINE_MODE,_T("在线"));
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
		//g_pFrm->SetPanelTextEx(IDS_ONLINE_MODE,_T("单机"));
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
	{	// 有阵列时，点完一个unit才停止程序
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
	// 分段加工使能
	// 	if(g_pFrm->m_pSysParam->nFileSeparaEnable == 1)
	// 		g_pFrm->m_Robot->ExOutput("分版使能",TRUE);
	// 	else
	// 		g_pFrm->m_Robot->ExOutput("分版使能",FALSE);

	g_pFrm->AddMsg("--->E-Stop----");
	////////
}

void CCmdRun::SetLamp()
{
	if(m_pController == NULL)return;

	ExOutput(_T("三色灯.绿"),m_tSysStatus == K_RUN_STS_RUN);
	ExOutput(_T("三色灯.黄"),m_tSysStatus == K_RUN_STS_PAUSE || m_tSysStatus == K_RUN_STS_STOP);

	if(m_tSysStatus == K_AXIS_STS_ALM_X|| m_tSysStatus == K_AXIS_STS_ALM_Y|| m_tSysStatus == K_AXIS_STS_ALM_Z)
	{
		ExOutput(_T("三色灯.红"),TRUE);
		ExOutput(_T("蜂鸣器"),TRUE);
		CFunction::DelaySec(0.3);
		ExOutput(_T("三色灯.红"),FALSE);
		ExOutput(_T("蜂鸣器"),FALSE);
		CFunction::DelaySec(0.3);
	}

	ExOutput(_T("三色灯.红"),m_tSysStatus == K_RUN_STS_EMERGENCY);

	//     if(g_pFrm->bisWeightError == TRUE)
	//        ExOutput(_T("报警指示灯"),TRUE);

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

		//图像扫描点Z轴到位
// 		if(((m_curZPos-pCmd->pDblValue[4*i +2])>1)||((m_curZPos-pCmd->pDblValue[4*i +2])<-1))
// 		{
// 			m_pController->AxisMove(Z_AXIS, pCmd->pDblValue[4*i +2], g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, m_cmdParam.dAccZ);
// 		}

		//不在图像扫描点
		if((fabs(m_curXPos-pCmd->pDblValue[4*i +0])>1)||(fabs(m_curYPos-pCmd->pDblValue[4*i +1])>1))
		{
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,pCmd->pDblValue[4*i +0], pCmd->pDblValue[4*i +1],g_pFrm->m_pSysParam->dSpeedImageMoveXY,m_cmdParam.dAccXY);
		}

		//////////////////////////////////
		CFunction::DelaySec(0.3);
		//////////////////////////////////
		g_pFrm->AddMsg("Load Model.");
		if(g_pView->PatLoad(i/*(LPSTR)(LPCSTR)g_pDoc->GetModeFile(i+1)*/))//从1开始，0为标定模板
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
						// 移动以后还是没找到
						g_pFrm->AddMsg("Find Mark Error!");
						g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"Find Mark Error!");
						if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
							g_pFrm->m_wndEditBar.EnableWindow(TRUE);

						if(g_pFrm->m_pSysParam->nFindMarkJogMove == 1)
						{//
							if(IDNO == AfxMessageBox(_T("模板匹配失败！手动查找?"),MB_YESNO))
							{
								// 设置错误标示并退出本工件加工
								return FALSE;
							}else
							{
								g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"");
								// 创建手动界面调整到位确定后再次查找，再找不到，直接退出
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
						{   // 不需要手动
							return FALSE;
						}
					}
				}
			}
lab:	 
			// 查找成功
			g_pView->PatCenter(dx,dy);	

			m_tMarkFindPos[i].x = dx;   // 查找到的模板位置机械坐标
			m_tMarkFindPos[i].y = dy;

			strtemp.Format("Mark点%i机械坐标:	x[%d],y[%d]", i, m_tMarkFindPos[i].x, m_tMarkFindPos[i].y);
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
		AfxMessageBox("找到的Mark点距离与做模板时Mark点距离偏差超过1mm!");
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	double dStdAngle=0;   // 标准模板角度
	double dMarkAngle =0; // 查找模板角度

	switch(g_pFrm->m_pSysParam->iMarkNum)
	{
	case 1:
		m_tOffset.x = m_tMarkFindPos[0].x -pCmd->pDblValue[0];  
		m_tOffset.y = m_tMarkFindPos[0].y -pCmd->pDblValue[1];
		break;
	case 2:  // 
		m_tMarkCenter.x = (pCmd->pDblValue[0] + pCmd->pDblValue[4])/2;   // 2个标定Mark点X中心
		m_tMarkCenter.y = (pCmd->pDblValue[1] + pCmd->pDblValue[5])/2;   // 2个标定Mark点Y中心 

		// 查找模板中心和标定模板中心偏差
		m_tOffset.x = ((m_tMarkFindPos[0].x + m_tMarkFindPos[1].x)/2 - m_tMarkCenter.x);
		m_tOffset.y = ((m_tMarkFindPos[0].y + m_tMarkFindPos[1].y)/2 - m_tMarkCenter.y);

		dStdAngle  = atan2(pCmd->pDblValue[1]-pCmd->pDblValue[5],pCmd->pDblValue[0]-pCmd->pDblValue[4]);
		dMarkAngle = atan2(m_tMarkFindPos[0].y-m_tMarkFindPos[1].y, m_tMarkFindPos[0].x-m_tMarkFindPos[1].x);

		// 计算角度偏差
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

		// 		//图像扫描点Z轴到位
		// 		if(((m_curZPos-pCmd->pDblValue[4*i +2])>1)||((m_curZPos-pCmd->pDblValue[4*i +2])<-1))
		// 		{
		// 			m_pController->AxisMove(Z_AXIS, pCmd->pDblValue[4*i +2], g_pFrm->m_pSysParam->dSpeedImageMoveZ, m_cmdParam.dAccZ);
		// 		}

		//不在图像扫描点
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
					// 移动以后还是没找到
					g_pFrm->AddMsg("Find Mark Error!");
					g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"Find Mark Error!");
					if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
						g_pFrm->m_wndEditBar.EnableWindow(TRUE);

					if(IDNO == AfxMessageBox(_T("模板匹配失败！手动查找?"),MB_YESNO))
					{
						// 移动以后还是没找到
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
						// 创建手动界面调整到位确定后再次查找，再找不到，直接退出
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
		// 查找成功
		g_pFrm->m_Precision.GetCurPosMM();
		g_pView->PatCenter(dx,dy);	

		m_tMarkFindPosEX[nGID][i].x = dx;   // 查找到的模板位置机械坐标
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
	double dStdAngle=0;   // 标准模板角度
	double dMarkAngle =0; // 查找模板角度

	switch(g_pFrm->m_pSysParam->iMarkNum)
	{
	case 1: 
		m_tOffsetEX[nGID].x = m_tMarkFindPosEX[nGID][0].x -pCmd->pDblValue[0];  
		m_tOffsetEX[nGID].y = m_tMarkFindPosEX[nGID][0].y -pCmd->pDblValue[1];
		break;
	case 2:  // 
		m_tMarkCenterEX[nGID].x = (pCmd->pDblValue[0] + pCmd->pDblValue[4])/2;   // 2个标定Mark点X中心
		m_tMarkCenterEX[nGID].y = (pCmd->pDblValue[1] + pCmd->pDblValue[5])/2;   // 2个标定Mark点Y中心 

		// 查找模板中心和标定模板中心偏差
		m_tOffsetEX[nGID].x = ((m_tMarkFindPosEX[nGID][0].x + m_tMarkFindPosEX[nGID][1].x)/2 - m_tMarkCenterEX[nGID].x);
		m_tOffsetEX[nGID].y = ((m_tMarkFindPosEX[nGID][0].y + m_tMarkFindPosEX[nGID][1].y)/2 - m_tMarkCenterEX[nGID].y);

		dStdAngle  = atan2(pCmd->pDblValue[1]-pCmd->pDblValue[5],pCmd->pDblValue[0]-pCmd->pDblValue[4]);
		dMarkAngle = atan2(m_tMarkFindPosEX[nGID][0].y-m_tMarkFindPosEX[nGID][1].y, m_tMarkFindPosEX[nGID][0].x-m_tMarkFindPosEX[nGID][1].x);

		// 计算角度偏差
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

// 		//图像扫描点Z轴到位
// 		if(((m_curZPos-pCmd->pDblValue[4*i +2])>1)||((m_curZPos-pCmd->pDblValue[4*i +2])<-1))
// 		{
// 			m_pController->AxisMove(Z_AXIS, pCmd->pDblValue[4*i +2], g_pFrm->m_pSysParam->dSpeedImageMoveZ, m_cmdParam.dAccZ);
// 		}

		//不在图像扫描点
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

		// 查找成功
		g_pFrm->m_Precision.GetCurPosMM();
		g_pView->PatCenter(dx,dy);	

		m_tMarkFindPosEX[nGID][i].x = dx;   // 查找到的模板位置机械坐标
		m_tMarkFindPosEX[nGID][i].y = dy;   
	}

	//////////////////////////////////////////////////////////////////////////
	double dStdAngle=0;   // 标准模板角度
	double dMarkAngle =0; // 查找模板角度

	switch(g_pFrm->m_pSysParam->iMarkNum)
	{
	case 1: 
		m_tOffsetEX[nGID].x = m_tMarkFindPosEX[nGID][0].x -pCmd->pDblValue[0];  
		m_tOffsetEX[nGID].y = m_tMarkFindPosEX[nGID][0].y -pCmd->pDblValue[1];
		break;
	case 2:  // 
		m_tMarkCenterEX[nGID].x = (pCmd->pDblValue[0] + pCmd->pDblValue[4])/2;   // 2个标定Mark点X中心
		m_tMarkCenterEX[nGID].y = (pCmd->pDblValue[1] + pCmd->pDblValue[5])/2;   // 2个标定Mark点Y中心 

		// 查找模板中心和标定模板中心偏差
		m_tOffsetEX[nGID].x = ((m_tMarkFindPosEX[nGID][0].x + m_tMarkFindPosEX[nGID][1].x)/2 - m_tMarkCenterEX[nGID].x);
		m_tOffsetEX[nGID].y = ((m_tMarkFindPosEX[nGID][0].y + m_tMarkFindPosEX[nGID][1].y)/2 - m_tMarkCenterEX[nGID].y);

		dStdAngle  = atan2(pCmd->pDblValue[1]-pCmd->pDblValue[5],pCmd->pDblValue[0]-pCmd->pDblValue[4]);
		dMarkAngle = atan2(m_tMarkFindPosEX[nGID][0].y-m_tMarkFindPosEX[nGID][1].y, m_tMarkFindPosEX[nGID][0].x-m_tMarkFindPosEX[nGID][1].x);

		// 计算角度偏差
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

BOOL CCmdRun::GetRepeatPosition(tgCmdLine *pCmd, double *pX, double *pY)	// 阵列时,获取阵列矩阵位置的3点坐标
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

		// 目标位置在图像中心坐标中和标定模板中心的角度和长度
		double dPointAngle = atan2(double(ccdy-m_tMarkCenterEX[nGID].y), double(ccdx-m_tMarkCenterEX[nGID].x));
		double dPointLen = sqrt(double((ccdx-m_tMarkCenterEX[nGID].x) * (ccdx-m_tMarkCenterEX[nGID].x) +(ccdy-m_tMarkCenterEX[nGID].y) * (ccdy-m_tMarkCenterEX[nGID].y) ) );

		// 计算偏移量 目标点和标定坐标系中的位置偏差
		double drx = dPointLen*cos(m_dRotateAngleEX[nGID]+dPointAngle)-dPointLen*cos(dPointAngle);
		double dry = dPointLen*sin(m_dRotateAngleEX[nGID]+dPointAngle)-dPointLen*sin(dPointAngle);
 
		*pX += drx+m_tOffsetEX[nGID].x;  // 转为激光(探针)中心中心坐标 
		*pY += dry+m_tOffsetEX[nGID].y;
	}
	else
	{
		// 目标位置 转为图像中心坐标位置 
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
			// 目标位置在图像中心坐标中和标定模板中心的角度和长度
			double dPointAngle = atan2(double(ccdy-m_tMarkCenterEX[nGID].y), double(ccdx-m_tMarkCenterEX[nGID].x));
			double dPointLen = sqrt(double((ccdx-m_tMarkCenterEX[nGID].x) * (ccdx-m_tMarkCenterEX[nGID].x) +(ccdy-m_tMarkCenterEX[nGID].y) * (ccdy-m_tMarkCenterEX[nGID].y) ) );

			// 计算偏移量 目标点和标定坐标系中的位置偏差
			double drx = dPointLen*cos(m_dRotateAngleEX[nGID]+dPointAngle)-dPointLen*cos(dPointAngle);
			double dry = dPointLen*sin(m_dRotateAngleEX[nGID]+dPointAngle)-dPointLen*sin(dPointAngle);

			// 将偏移量叠加到图像与喷射中心偏差上
			double dx = m_tOffsetEX[nGID].x+drx;  
			double dy = m_tOffsetEX[nGID].y+dry;

			*pX+=dx;  // 转为喷射中心坐标 
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
	// 目标位置 转为图像中心坐标位置 
	double ccdx = *pX-(g_pFrm->m_pSysParam->tOffset[m_CurToolNo].x);  
	double ccdy = *pY-(g_pFrm->m_pSysParam->tOffset[m_CurToolNo].y);

	// 目标位置在图像中心坐标中和标定模板中心的角度和长度
	double dPointAngle = atan2(double(ccdy-m_tMarkCenter.y), double(ccdx-m_tMarkCenter.x));
	double dPointLen = sqrt(double((ccdx-m_tMarkCenter.x) * (ccdx-m_tMarkCenter.x) +(ccdy-m_tMarkCenter.y) * (ccdy-m_tMarkCenter.y) ) );

	// 计算偏移量 目标点和标定坐标系中的位置偏差
	double drx = dPointLen*cos(m_dRotateAngle+dPointAngle)-dPointLen*cos(dPointAngle);
	double dry = dPointLen*sin(m_dRotateAngle+dPointAngle)-dPointLen*sin(dPointAngle);

#ifdef  DEBUG_SD950
	CString strtemp;
	strtemp.Format("目标位置偏差dX:%.3f",drx);
	g_pFrm->AddMsg(strtemp);
	strtemp.Format("目标位置偏差dy:%.3f",dry);
	g_pFrm->AddMsg(strtemp);
#endif
	// 将偏移量叠加到图像与喷射中心偏差上
	double dx = m_tOffset.x+drx;  
	double dy = m_tOffset.y+dry;

	*pX+=dx;  // 转为喷射中心坐标 
	*pY+=dy;

	return TRUE;
}

BOOL CCmdRun::RunListExpand()
{	
	//叠加针头图像偏移
	if(!m_bModifiedTrack) return TRUE;
	//	g_pView->ImgStop();

	int pCurTempToolNo = -1; //当前执行工具号
	double dZSetHeight = 10;	// 点胶高度相对基准高度

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
			pCurTempToolNo = pCmd.pIntValue[0];//所有的指令pIntValue[0]都为阀的ID定义
			dZSetHeight    = pCmd.pDblValue[2];
			bNeedFillCMPList  = TRUE;
		}	

		if (pCurTempToolNo==-1)//没有设置阀
		{
			//用图像模式运动
			pCurTempToolNo = 2;
			m_CurToolNo = pCurTempToolNo;
			bNeedFillCMPList = FALSE;
		}
		else
		{
			//这里要选择是针筒还是喷射阀
			if(!RunListOffset(&pCmd, g_pFrm->m_pSysParam->tOffset[pCurTempToolNo].x, g_pFrm->m_pSysParam->tOffset[pCurTempToolNo].y, 0/*g_pFrm->m_pSysParam->tOffset[pCurTempToolNo].z*/))
			{
				AfxMessageBox("轨迹点位置校正偏移超出了有效行程范围，请重新设置图像和针头位置参数.");
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
				{	//图像模式
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
	switch(pCmd->tType)//////修正图像与针头的偏差图像检测和高度检测不需要
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
	case CMD_SUPERSTART:	// 飞胶点
	case CMD_SUPERPOINT:	// 飞胶点
	case CMD_SUPEREND:		// 飞胶点
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
	case CMD_WCL://称重直线两点坐标
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
	case CMD_WCRECT:   // 称重矩形
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
		{//圆心＋直径
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
		{//三点
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
		AfxMessageBox("没有轨迹命令");
		return FALSE;
	}
	if(iStartIndex<0 || iStartIndex>g_pFrm->m_wndEditBar.m_rawList.GetCount()-1)
	{
		AfxMessageBox("开始行设置错误");
		return FALSE;
	}
	if(iStartIndex>iEndIndex || iEndIndex>g_pFrm->m_wndEditBar.m_rawList.GetCount()-1)
	{
		AfxMessageBox("结束行设置错误");
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
					AfxMessageBox("轨迹点位置超出了有效形成范围，请重新设置参数.");
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
	m_cmdRunList.RemoveAll();//这里要先删除列表后清除内存
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

	dx = g_pFrm->m_pSysParam->tDefHomePos.x;//这里需要通过菜单设置自定义的原点位置
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

	if(pCmd->pIntValue[0] == 0)//输入
	{
		sTemp = m_pController->TS_ListWaitInp(CRD1,(unsigned int)pCmd->pIntValue[1],TRUE);
		if(sTemp == RTN_STP) return RTN_STP;
		if(sTemp == RTN_ESTP) return RTN_ESTP;
	}
	else if(pCmd->pIntValue[0] == 1)//输出
	{
		sTemp = m_pController->TS_ListOutp(CRD1,(unsigned int)pCmd->pIntValue[1],pCmd->pIntValue[2]);
		if(sTemp == RTN_STP) return RTN_STP;
		if(sTemp == RTN_ESTP) return RTN_ESTP;
	}

	return RTN_NONE;
}
short CCmdRun::RunDispensor(tgCmdLine *pCmd)
{//点胶阀命令 //只开关胶，不动作气缸
	m_CurToolNo = pCmd->pIntValue[0];

	short sTemp  = m_pController->TS_ListSetDispBit(CRD1,&dcp[pCmd->pIntValue[0]],pCmd->pIntValue[1]);//pCmd->pIntValue[0] 阀ID
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
{//清洗
	t_BathPoint bp;
	::ZeroMemory(&bp,sizeof(t_Base));

	bp.dVelXY = g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax;
	bp.dVelZ  = g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax;

	bp.pos.x = g_pFrm->m_pRobotParam->m_structClean.m_posClean.x+g_pFrm->m_pSysParam->tOffset[0].x;   
	bp.pos.y = g_pFrm->m_pRobotParam->m_structClean.m_posClean.y+g_pFrm->m_pSysParam->tOffset[0].y;
	bp.pos.z = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_structClean.m_posClean.z;
	bp.pos.a = g_pFrm->m_pRobotParam->m_structClean.m_posClean.u;

	bp.nPort     = inWashPort;                     // 清洗泵端口
	bp.dWaitTime = 0.1;                        
	bp.timers    = g_pFrm->m_pSysParam->iWashNum;  // 清洗次数
	bp.type      = g_pFrm->m_pSysParam->iWashType;  // 清洗类型
	bp.dis       = 5;          // 清洗动作幅度 
	short sTemp  = m_pController->TS_ListBath(CRD1,&dcp[m_CurToolNo],&bp);
	return sTemp;
}
short CCmdRun::RunStandBy(tgCmdLine *pCmd,double dOffsetX,double dOffsetY)
{//等待点
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

	short sTemp  = m_pController->TS_ListStandBy(CRD1,&dcp[m_CurToolNo],&sdp);//pCmd->pIntValue[0] 阀ID
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

	short sTemp  = m_pController->TS_ListDummy(CRD1,&ddp);//pCmd->pIntValue[0] 阀ID
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
	// 脉冲控制点胶阀时,点胶阀的点胶频率
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

	//开关阀
	if(pCmd->pIntValue[1]>0 && (RunMode_Wet==m_runMode)) 
		pdp.bOn = TRUE;
	else 
		pdp.bOn = FALSE;

	//开关临时延时
	if(0 == pCmd->pIntValue[5])  
		pdp.dDispTime = m_cmdParam.tPoint.dDispTime;//s to ms
	else                         
		pdp.dDispTime = pCmd->pDblValue[5];//临时延时时间

	dcp[m_CurToolNo].bBit = pdp.bOn;
	dcp[m_CurToolNo].lowTime = m_cmdParam.nLowTime;
	dcp[m_CurToolNo].highTime = m_cmdParam.nHighTime;
	dcp[m_CurToolNo].lPulseNum = m_cmdParam.lPulseNum;

	short sTemp  = m_pController->TS_ListPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] 阀ID
	if (RTN_NONE != sTemp)
	{
		return sTemp;
	}

	// 启动运动
	sTemp=m_pController->TS_ListWaitStop();
	return sTemp;
}

// 喷射打点
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

	// 点参数赋值
	DOT_PARAM dotParam;
	::ZeroMemory(&dotParam,sizeof(DOT_PARAM));
	dotParam = g_pFrm->m_DotParam[(int)pCmd->pDblValue[PARAM_POS]];
	
//	m_tmLoad.TimerStart();
	//////////////////////////////////////////////////////////////////////////
	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, dx, dy, pdp.dVelXY, pdp.dAccXY);
	m_pController->AxisMove(Z_AXIS, dz, pdp.dVelZ, pdp.dAccZ);
// 	strTemp.Format("Z 移动时间:[%.3f]", m_tmLoad.TimerGetMs());
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

// 	short sTemp = m_pController->TS_ListHPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] 阀ID
// 	if (RTN_NONE != sTemp)
// 	{
// 		return sTemp;
// 	}
// 
// 	// 启动运动
// 	sTemp=m_pController->TS_ListWaitStop();
// 	return sTemp;
}

// 飞胶起点
short CCmdRun::RunFlyPoint_StartPos(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	// 启动运动并等待运动停止
	short sTemp;
	CString strTemp;
	sTemp=m_pController->TS_ListWaitStop();

	// IO控制方式，有IO信号开阀,无信号关阀
	if (0 == g_pFrm->m_pRobotParam->m_nValveCtrlMode)
	{
		if (0 != g_pFrm->m_comDisp[0].m_dispParam.NumOfPul)
		{
			g_pFrm->m_comDisp[0].m_dispParam.NumOfPul = 0;
			if (1 != g_pFrm->m_comDisp[0].HandleParamDispenser(FALSE))
			{
				AfxMessageBox("设置点胶阀及保存参数出错!");
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
				AfxMessageBox("设置点胶阀及保存参数出错!");
				return -1;
			}
		}
	}
	else
	{
		strTemp.Format("使用喷射阀时,未指定阀控制模式:[%d]", g_pFrm->m_pRobotParam->m_nValveCtrlMode);
		AfxMessageBox(strTemp);
		return -1;
	}

	t_HPoint pdp; // 高速出胶点
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

	// 线段长度MM
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
	// 标准胶重
	double dDotWt = g_pFrm->m_pRobotParam->m_dAvgWeight;
	// 点数
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

	dLineLenth += dPreLen-dNxtCmdLen;	// 点胶长度加上修正后的长度

	pdp.dVelXY = dLineLenth*dDispFreq/pdp.iDotNum; 
//	pdp.dVelXY = m_cmdParam.dVelXY;    
	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dAccXY = m_cmdParam.dAccXY;
	pdp.dAccZ  = m_cmdParam.dAccZ;

	// 加速到匀速段的距离
	double dAccLen = pdp.dVelXY*pdp.dVelXY / m_cmdParam.dAccXY / 2000.0;

	// 高速IO控制,计算发送脉冲的坐标
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

			BOOL bForword = TRUE;	// 是否往正方向点胶
			double dAng = abs(dx-dNxtx) - abs(dy-dNxty);	// 线段偏向X轴或者Y轴
			if (dAng > 0)	// 偏向X的线段
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

			double dRelativeDispPos;// 开阀位置相对均匀分布的胶点位的相对位置
			double dTemp1 = 0.75*6*6*2*pdp.dAccXY/1000; 
			double dTemp2 = 0;
			for (int i=0; i<nAccDotNums; i++)
			{
			if (bForword)	// 往正方向点胶
			{
			dTemp2 = sqrt(dTemp1*dTemp1+4*dTemp1*(pdp.dCompareStart+i*pdp.dCompareStep));
			dRelativeDispPos = (dTemp2 - dTemp1) / 2;
			pdp.pComparePos[i] = long((pdp.dCompareStart + i*pdp.dCompareStep - dRelativeDispPos) * 1000);
			}
			else	// 往负方向点胶
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

	// 试教位置点 
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

	dcp[m_CurToolNo].outBit[0] = ExOutputBitNameToNo(_T("1号点固态电器"));

	//添加运动指令
	sTemp  = m_pController->TS_ListHStartPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] 阀ID
	strtemp.Format("喷射起点：x[%.3f], y[%.3f]", pdp.pos.x, pdp.pos.y);
	g_pFrm->AddMsg(strtemp);

	if (NULL != pdp.pComparePos)  
	{
		delete []pdp.pComparePos;
		pdp.pComparePos = NULL;
	}

	return sTemp;
}
// 飞胶喷射点
short CCmdRun::RunFlyPoint_MidPos(tgCmdLine *pPreTrackCmd, tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	UNREFERENCED_PARAMETER(pNextTrackCmd);
	if (CMD_LINE_ARC == pPreTrackCmd->tType)	return RTN_NONE;

	t_HPoint pdp; // 高速出胶点
	::ZeroMemory(&pdp,sizeof(t_HPoint));

	CString strtemp;
	double dx,dy,dz,du;
	double dxx,dyy;// 位置微调

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
	strTemp.Format("喷射线速度:[%.3f]", pdp.dVelXY);
	g_pFrm->AddMsg(strTemp);
	//    
	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dAccXY = m_cmdParam.dAccXY;
	pdp.dAccZ  = m_cmdParam.dAccZ;

	dxx = pCmd->pDblValue[6];
	dyy = pCmd->pDblValue[7];

	// 位置微调
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

	//添加运动指令
	short sTemp  = m_pController->TS_ListHWorkPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] 阀ID

	return sTemp;
}
// 根据速度计算偏移量
double CCmdRun::CalcVelFlyOffset(double dvel,double djetdelay)  
{
	double temp;
	//  mm/s

	djetdelay  = djetdelay/1000.;// ms -> s 

	temp = dvel*djetdelay ;
	return temp ; // mm 
}
// 飞胶终点
short CCmdRun::RunFlyPoint_EndPos(tgCmdLine *pPreTrackCmd, tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{	
	UNREFERENCED_PARAMETER(pNextTrackCmd);
	if (CMD_SUPEREND == pPreTrackCmd->tType)	return RTN_NONE;

	t_HPoint pdp; // 高速出胶点
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
	if (pdp.dVelXY > Vel_Limit)	// 限速500
	{
		pdp.dVelXY = Vel_Limit;///= 1.2;
	}
	strTemp.Format("喷射线速度:[%.3f]", pdp.dVelXY);
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

	// 减速到匀速段的距离
	double dAccLen = pdp.dVelXY*pdp.dVelXY / m_cmdParam.dAccXY / 2000.0;
	dAccLen *= 1.5;

	// 试教位置点 
	pdp.m_assistPos.x = dx + (dx-dPrex)*(dAccLen-dPreLen)/dLineLenth;
	pdp.m_assistPos.y = dy + (dy-dPrey)*(dAccLen-dPreLen)/dLineLenth;
	pdp.m_assistPos.z = dz;
	pdp.m_assistPos.a = du;

	pdp.pos.x = dx + (dPrex - dx)*(dPreLen)/dLineLenth;
	pdp.pos.y = dy + (dPrey - dy)*(dPreLen)/dLineLenth;
	pdp.pos.z = dz;

	// 添加 线参数
	LINE_PARAM lineParam;
	::ZeroMemory(&lineParam,sizeof(LINE_PARAM));
	lineParam = g_pFrm->m_LineParm[(int)pCmd->pDblValue[PARAM_POS]];

	//添加运动指令
	short sTemp  = m_pController->TS_ListHEndPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] 阀ID
	strtemp.Format("喷射终点：x[%.3f], y[%.3f]", pdp.pos.x, pdp.pos.y);
	g_pFrm->AddMsg(strtemp);

	// 启动运动
	sTemp=m_pController->TS_ListWaitStop();

	return sTemp;
}
// 获取模拟量激光数据
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
		pTempValue += db[i];  // 累计数据
	}

	// 取获取数据平均值
	date = (unsigned short)(pTempValue / ntime) ; 

	date = (unsigned short)((-10.0+(date/65535.0)*20.0)*2); 
	// 这里需要进行 量程的参数化优化处理 

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
			g_pFrm->AddMsg(_T("读取激光测高数据出错!"));
			return RTN_ZHeight_Err;
		}
		//////////////////////////////////////////////////////////////////////////
		*dValue = dHt;
		strTemp.Format("激光测高读数:[%.3f]", dHt);
		//g_pFrm->AddMsg(strTemp);
		CFunction::WriteLog(__FILE__, __LINE__, strTemp, CFunction::LEV_INFO);

		ZHeightVal = dHt-dStdHeightVal;
		if (abs(ZHeightVal) > 0.5)
		{
			AfxMessageBox(_T("激光测得高度偏差超过0.5mm!"));
			g_pFrm->AddMsg(_T("激光测高偏差超过0.5mm!"));
			return RTN_ZHeight_Err;
		}

		ZHeightVal = dHt - g_pFrm->m_pRobotParam->m_zLaserBaseVal;
		m_dFirstHeightOffset = ZHeightVal;   // 高度偏差保存

		strTemp.Format("激光测高读数:[%.3f]", dHt);
		g_pFrm->AddMsg(strTemp);
	}
	else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
	{
		//////////////////////////////////////////////////////////////////////////
		g_pFrm->m_Robot->ExOutput(_T("探针气缸电磁阀"), TRUE);
		CFunction::DelaySec(1);
		double dPos = dStdHeightVal - g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLHeight;
		sTemp = m_pController->AxisMove(Z_AXIS, dPos, g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeHSpd, 10);
		dPos = g_pFrm->m_pRobotParam->m_zContactPos-5;	// 探针测高极限位置
		sTemp = m_pController->AxisProbeCheck(Z_AXIS, dPos, g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLSpd, 10, &dHt);
		g_pFrm->m_Robot->ExOutput(_T("探针气缸电磁阀"), FALSE);
		GotoSafeZpos();
		CFunction::DelaySec(0.2);

		//////////////////////////////////////////////////////////////////////////
		ZHeightVal = dHt-dStdHeightVal;
		if (abs(ZHeightVal) > 0.5)
		{
			AfxMessageBox(_T("探针测得高度偏差超过0.5mm!"));
			g_pFrm->AddMsg(_T("探针测高偏差超过0.5mm!"));
			return RTN_ZHeight_Err;
		}
		*dValue = dHt;

		ZHeightVal = dHt - g_pFrm->m_pRobotParam->m_zProbeBaseVal;
		m_dFirstHeightOffset = ZHeightVal;   // 高度偏差保存

		strTemp.Format("探针测高读数:[%.3f]", dHt);
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
// 	Strtemp.Format("Z轴激光测高差: %.3f", ZHeightVal);
// 	g_pFrm->AddMsg(Strtemp);
// 
// 	/////////////////////
// 	tgZMeter tZMeterTemp;
// 
// 	if (m_ZMeterVector.empty())
// 	{   // 容器空 
// 		*dValue = ZHeightVal;                // 返回高度值
// 		m_dFirstHeightOffset = ZHeightVal;   // 高度偏差保存
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
// 	m_ZMeterVector.push_back(tZMeterTemp);  // 数据压入容器
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

	//开关阀
	if(pCmd1->pIntValue[1]>0 && (RunMode_Wet==m_runMode)) 
		lstdp.bOn = TRUE;
	else 
		lstdp.bOn = FALSE;

	//开关临时延时
	if(0 == pCmd1->pIntValue[5]) 
		lstdp.dPreTime=m_cmdParam.tLine.dPreTime;//s to ms
	else	
		lstdp.dPreTime=pCmd1->pDblValue[5];//临时延时时间

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
	return m_pController->TS_ListLineStart(CRD1,&dcp[m_CurToolNo],&lstdp);//pCmd->pIntValue[0] 阀ID
}

short CCmdRun::RunLinePass(tgCmdLine *pPreTrackCmd,tgCmdLine *pCmd1,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	t_LinePass lpdp;

	double xb,yb,zb;
	double dx,dy,dz,du;
	int iPosIndex = 0;

	//称重画线 时退出此指令执行
	if (pNextTrackCmd->tType==CMD_WCL)
	{
		RunWCL_addSTrack=false;  // 不增加开始线
		return RTN_NONE;

	}
	//称重矩形 时退出此指令执行
	if (pNextTrackCmd->tType==CMD_WCRECT)
	{
		RunWCL_addSTrack=false;  // 不增加开始线
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

	//开关临时速度
	if(0 != pCmd1->pIntValue[4])
		lpdp.dWorkVel = pCmd1->pDblValue[4];
	else
		lpdp.dWorkVel = m_cmdParam.tLine.dVel;

	//开关临时延时
	if(0 == pCmd1->pIntValue[5])
		lpdp.dNodeTime=m_cmdParam.tLine.dNodeTime;
	else                         
		lpdp.dNodeTime=pCmd1->pDblValue[5];

	//开关阀
	if(pCmd1->pIntValue[1]>0 && RunMode_Wet==m_runMode) 
		lpdp.bOn = TRUE;
	else 
		lpdp.bOn = FALSE;

	g_pFrm->m_Precision.Measure2Real(dy);

	lpdp.pos.x = dx;
	lpdp.pos.y = dy;
	lpdp.pos.z = dz;
	lpdp.pos.a = du;

	//倒角半径
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

	if(!g_pFrm->m_wndEditBar.GetCmdArcInfoEx(pPreTrackCmd, pCmd1,pNextTrackCmd,&tArc))//检查圆弧是否合法
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
		arcdp.bOn = TRUE;//开关阀
	else 
		arcdp.bOn = FALSE;

	if(0 != pCmd1->pIntValue[4])//开关临时速度
		arcdp.dWorkVel = pCmd1->pDblValue[4];
	else
		arcdp.dWorkVel = m_cmdParam.tLine.dVel;

	if(0 == pCmd1->pIntValue[5])//开关临时时间
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

	short sTemp=m_pController->TS_ListArc(CRD1,&dcp[m_CurToolNo],&arcdp,TRUE);//这里的阀如果可以控制的话，需要获取前面当前阀的ID
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
	strTemp.Format("画圆喷射线速度:[%.3f]", circledp.dVelXY);
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

	//m_CurToolNo = pCmd->pIntValue[0];//当前阀

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

	if(0 != pCmd1->pIntValue[4])//开关临时速度
		ledp.dWorkVel = pCmd1->pDblValue[4];
	else
		ledp.dWorkVel = m_cmdParam.tLine.dVel;


	if(0 != pCmd1->pIntValue[5])//开关临时等待时间  
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

	dcp[m_CurToolNo].bBit = FALSE;//结束点必须关胶

	// 添加 线参数
	LINE_PARAM lineParam;
	::ZeroMemory(&lineParam,sizeof(LINE_PARAM));
	lineParam = g_pFrm->m_LineParm[(int)pCmd1->pDblValue[PARAM_POS]];

	ledp.dTailCloseLen = lineParam.dfShutOffDistance;		// 提前关胶距离
	ledp.dEndZupHigh   = lineParam.dfBacktrackGap;			// 避开时抬升高度
	ledp.dEndVel	   = lineParam.dfRetractSpeed;			// 避开是上升速度
	ledp.dTailCloseLen = lineParam.dfShutOffDistance;		// 提前关胶距离

	short sTemp=m_pController->TS_ListLineEnd(CRD1,&dcp[m_CurToolNo],&ledp);

	return sTemp;
}
/******
// 指令运行 
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
		// 指定了阵列点胶的起始点
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp =RunPoint(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_POINT: 
		// 指定了阵列点胶的起始点
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunPoint(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_ZMETER:
		dHeight = 0;  //清原高度差
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
		// 指定了阵列点胶的起始点
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunLineStart(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_LINE_PASSING: 
		// 指定了阵列点胶的起始点
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunLinePass(PreTrackPoint,tDot1,tDot2,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;			

	case CMD_DUNNY_SPEED:
		m_cmdParam.m_DummySpeed = tDot1->pDblValue[0];	
		break;
	case CMD_LINE_ARC:
		// 指定了阵列点胶的起始点
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunLineArc(PreTrackPoint,tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_LINE_END:
		// 指定了阵列点胶的起始点
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
		//Z轴须抬起
		sTemp = m_pController->TS_ListInit(CRD1,0);
		if(sTemp != RTN_NONE) return sTemp;

		if(tDot1->pIntValue[0] == 1)
		{//标号跳转
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
		// 指定了阵列点胶的起始点
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunCircle(tDot1,NextTrackPoint,dOffsetX,dOffsetY);
		if(sTemp!=RTN_NONE) return sTemp;
		break;

	case CMD_SET_IO:
		// 指定了阵列点胶的起始点
// 		if (m_structPathInfo.m_bRepeat && m_structPathInfo.m_bRepeatSetStart)
// 			break;
		sTemp = RunSetIO(tDot1);
		if(sTemp!=RTN_NONE) return sTemp;
		break;
	case PARAM_LINE_SPEED: 
		m_cmdParam.tLine.dVel = tDot1->pDblValue[0];
		break;
	case PARAM_LINE_SETUP:	
		m_cmdParam.tLine.dPreTime = tDot1->pDblValue[0];			// 出胶时间
		m_cmdParam.tLine.dNodeTime = tDot1->pDblValue[1];		// 中途时间
		m_cmdParam.tLine.dTailTime = tDot1->pDblValue[3];		// 停胶时间
		m_cmdParam.tLine.dTailCloseLen = tDot1->pDblValue[2];	// 不出胶距离

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
		m_cmdParam.tPoint.dPreTime = tDot1->pDblValue[0]; // 单点前延时间
		m_cmdParam.tPoint.dDispTime = tDot1->pDblValue[1];// 单点点胶时间
		m_cmdParam.tPoint.dTailTime = tDot1->pDblValue[2];// 单点结束等待时间
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
			//m_pController->g_ExMtnPara->dfBakpos[Z_AXIS] = 0;//Z抬高	
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
// 指令运行 
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
		dHeight = 0;  //清原高度差
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
	case CMD_GOTO_ADDRESS:   // 跳转指令
		sTemp = m_pController->TS_ListWaitStop();
		if(sTemp != RTN_NONE) return sTemp;
		//Z轴须抬起
		sTemp = m_pController->TS_ListInit(CRD1,1);
		if(sTemp != RTN_NONE) return sTemp;

		if(tDot1->pIntValue[0] == 1)
		{//标号跳转
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

// 计算阵列位置
BOOL CCmdRun::CalcBoradArcData(double &dx, double &dy, double dXDis, double dYDis)
{   
	dx += dXDis;
	dy += dYDis;

	return TRUE;
}

// 基板夹角数据计算
BOOL CCmdRun::CalcBoradArcData(double &dx,double &dy,double dCurXdis,double dCurYdis,double dRArc)
{   
	// 基板夹角
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
	// 先将当前坐标数据从查找模板坐标系(产品坐标)转换到标准模板坐标系(机械坐标)
	//X = Cos(-@)*x - Sin(-@)*y;
	//Y = Sin(-@)*x + Cos(-@)*y;
	tempXData = dx *dCosarcF - dSinarcF * dy; 
	tempYData = dx *dSinarcF + dCosarcF * dy; 

	tt1 = tempXData + dCurXdis;
	tt2 = tempYData + dCurYdis;

	// 再将当前坐标数据从标准模板坐标系(机械坐标)转换到查找模板坐标系(产品坐标)
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
// 阵列执行
/*阵列行*/
//pCmd->pDblValue[0] // x坐标
//pCmd->pDblValue[1] // y坐标
//pCmd->pDblValue[2] // 夹角
//pCmd->pIntValue[0] // 开始地址
//pCmd->pIntValue[1] // 1:S,2:N
//pCmd->pIntValue[2] // X数目
//pCmd->pIntValue[3] // Y数目
//pCmd->pIntValue[4] // Z轴高度补偿标志
//m_dFirstHeightOffset = 0;   // 阵列开始前使用一次，阵列后不使用此变量了
short CCmdRun::RunRepeat(unsigned char type,/*int nRepeatEnd,*/tgCmdLine pCmd,double dOffsetX,double dOffsetY)
{  
	BOOL    bZCheckFlag = FALSE;	// 激光高度检测标示
	
	long numx,numy;					// X Y 阵列个数
	numx = pCmd.pIntValue[2];		// X数目
	numy = pCmd.pIntValue[3];		// Y数目
	double  offx, offy;				// 阵列 间隔
	double  xSpace=0, ySpace=0;		// XY阵列间距
	double  xSpaceCorrect=0, ySpaceCorrect=0;	// XY阵列间距修正

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
	
	unsigned char StoL = 1; // 1递增 0-递减

	int nRepeatSt=0, nRepeatEnd=0;		// 阵列起止地址
	nRepeatSt = pCmd.pIntValue[0];
	nRepeatEnd = pCmd.pIntValue[6];

	short  sTemp;
	double dZTempHeightValue = 0;

	long num = numx * numy;   // 阵列总个数
	
	tgCmdLine address0;  // 上一条指令
	tgCmdLine address1;  // 当前指令
	tgCmdLine address2;  // 下一条指令
	tgCmdLine tPreTrackCmd;   // 上一个 动作指令
	tgCmdLine tNextTrackCmd;  // 下一个 动作指令

	unsigned char ucRepeatModeS_N;     // 移动方式 //0:S,1:N
	ucRepeatModeS_N = (unsigned char)pCmd.pIntValue[1]; 
	
	double dCurRArc = atan2((pCmd.pDblValue[5]-pCmd.pDblValue[1]), (pCmd.pDblValue[4]-pCmd.pDblValue[0]));//pCmd.pDblValue[3];  // 夹角

	long  lcx, lcy;   // 阵列方式　 
	if(type == 1)     // X 方向阵列
	{   // repeat x 
// 		if (m_structPathInfo.m_bRepeatSetStart)
// 		{
			lcx = pCmd.pIntValue[4]-1;//m_structPathInfo.m_nRepeatStX-1; 
			lcy = pCmd.pIntValue[5]-1;//m_structPathInfo.m_nRepeatStY-1;

			if ((0==ucRepeatModeS_N) && (lcy&1))	// S型点胶往回走时
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

			if ((0==ucRepeatModeS_N) && (lcx&1))	// S型点胶往回走时
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
	if (CMD_ZMETER == address1.tType)	// 阵列测高
	{
		bZCheckFlag = TRUE;

		int num0=num;
		m_ZMeterVector.clear();
		for (; num0>0; num0--)
		{
			//////////// X 方向阵列 ////////////////////
			offx = xSpace*lcx + xSpaceCorrect*lcy;  // 阵列X间隔
			offy = ySpace*lcy + ySpaceCorrect*lcx;  // 阵列Y间隔

			if(type == 1)
			{//repeat x
				if(ucRepeatModeS_N == 0)  // S 方式移动
				{
					if(StoL) 
						lcx ++;
					else
						lcx --;

					if(lcx >= numx)  // 每行最后一个 
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

			double dHeight = 0;  //清原高度差
			sTemp = RunZHeight(&address1,dOffsetX,dOffsetY,&dHeight);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 参数还原
	if(type == 1)     // X 方向阵列
	{   // repeat x 
		// 		if (m_structPathInfo.m_bRepeatSetStart)
		// 		{
		lcx = pCmd.pIntValue[4]-1;//m_structPathInfo.m_nRepeatStX-1; 
		lcy = pCmd.pIntValue[5]-1;//m_structPathInfo.m_nRepeatStY-1;

		if ((0==ucRepeatModeS_N) && (lcy&1))	// S型点胶往回走时
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

		if ((0==ucRepeatModeS_N) && (lcx&1))	// S型点胶往回走时
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

	int nNeedDotNum;	// 设置好的点胶数目
	nNeedDotNum = pCmd.pIntValue[7];
	if (0 != nNeedDotNum)
	{
		nNeedDotNum = num -nNeedDotNum;
	}
	nNeedDotNum = max(0, nNeedDotNum);
	// 阵列总个数
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

// 		if(bZCheckFlag==FALSE)  // 测高标示
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
		//////////// X 方向阵列 ////////////////////
 		offx = xSpace*lcx + xSpaceCorrect*lcy;  // 阵列X间隔
 		offy = ySpace*lcy + ySpaceCorrect*lcx;  // 阵列Y间隔
		
		if(type == 1)
		{//repeat x
			if(ucRepeatModeS_N == 0)  // S 方式移动
			{
				if(StoL) 
					lcx ++;
				else
					lcx --;

				if(lcx >= numx)  // 每行最后一个 
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
			return RTN_PARAMETER_ERR;//这里需要检查行号是否合法

		for(; nRepeatSt<nRepeatEnd; nRepeatSt++)  
		{
			while (K_RUN_STS_PAUSE == m_tSysStatus)
				Sleep(100);

			if(nRepeatSt==0)
			{   // 从第一条指令开始
				address0.tType = CMD_NONE;
				if(GetRecord(address1,0))  
					return RTN_PARAMETER_ERR;
				if(GetRecord(address2,1)) 
					return RTN_PARAMETER_ERR;
			}
			else if(nRepeatSt==nRepeatEnd)
			{   // 只有一条指令
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
				switch(tPreTrackCmd.tType)  // 上动作指令类型
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
						if(tPreTrackCmd.tType!=CMD_ZMETER)//不需要校正单激光测高指令
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
						//不需要校正单激光测高指令
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
						//不需要校正单激光测高指令
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
				switch(tNextTrackCmd.tType) // 下动作指令类型
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
						if(tNextTrackCmd.tType!=CMD_ZMETER)//不需要校正单激光测高指令
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
						if(tNextTrackCmd.tType!=CMD_ZMETER)//不需要校正单激光测高指令
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
						//不需要校正单激光测高指令
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
			//////////  指令 ////////
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
					if(address1.tType!=CMD_ZMETER)//不需要校正单激光测高指令
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
					if(address1.tType!=CMD_ZMETER)//不需要校正单激光测高指令
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
					if(address1.tType!=CMD_ZMETER)//不需要校正单激光测高指令
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

			//  执行当前指令 
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
// 称重矩形
short CCmdRun::RunWCRect(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	t_WCRectParam     lWclp;   // 称重划线参数
	::ZeroMemory(&lWclp,sizeof(t_WCRectParam));

	// 矩形位置
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

	// 空跑速度
	lWclp.dVelXY    = m_cmdParam.dVelXY;   
	lWclp.dVelZ     = m_cmdParam.dVelZ;   

	//出胶速度 
	lWclp.dWorkVel  = pCmd->pDblValue[21]; //m_cmdParam.tLine.dVel;

	//开关阀
	lWclp.bOn = TRUE;

	//开关临时延时 
	lWclp.dPreTime=m_cmdParam.tLine.dPreTime; //s to ms


	lWclp.DispWegiht  = pCmd->pDblValue[20];      // 出胶重量
	lWclp.dEndZupHigh = m_cmdParam.dZclearance;   // Z轴提升高度
	lWclp.dZcleaType  = m_cmdParam.dZcleaType;    // Z提升高度
	lWclp.dEndTime    = m_cmdParam.tLine.dTailTime;  // 点胶结束停胶时间

	/// 收尾动作
	lWclp.dEndVel		=	m_cmdParam.tLine.dEndVel;
	lWclp.dEndLen		=	m_cmdParam.tLine.dEndLen;
	lWclp.iEndType		=	m_cmdParam.tLine.iEndType;
	lWclp.dTailCloseLen	=	m_cmdParam.tLine.dTailCloseLen;

	if (pNextTrackCmd->tType==CMD_LINE_PASSING)
		lWclp.AddE_Trace=false;
	else
		lWclp.AddE_Trace=true;

	lWclp.AddS_Trace = RunWCL_addSTrack;

	//工具设置
	dcp[m_CurToolNo].bBit = lWclp.bOn;  // 出胶使能
	dcp[m_CurToolNo].lPulseNum = pCmd->pIntValue[3];  // 出胶次数
	dcp[m_CurToolNo].lowTime = m_cmdParam.nLowTime;   // 出胶周期 

	//
	double disx,disy;
	double dis1,dis2,dis3,dis4,dis0;
	double t;

	disx= dxB-dxA ;
	disy = dyB-dyA ;
	dis1=sqrt((disx)*(disx)+(disy)*(disy));    // 线段长   mm

	disx= dxC-dxB ;
	disy = dyC-dyB ;
	dis2=sqrt((disx)*(disx)+(disy)*(disy));    // 线段长   mm

	disx= dxD-dxC ;
	disy = dyD-dyC ;
	dis3=sqrt((disx)*(disx)+(disy)*(disy));    // 线段长   mm

	disx= dxE-dxD ;
	disy = dyE-dyD ;
	dis4=sqrt((disx)*(disx)+(disy)*(disy));    // 线段长   mm

	dis0 = dis1+dis2+dis3+dis4;  // 矩形边长

	t= dis0 / lWclp.dWorkVel;             // 需要的时间    s
	t= 1000*t;                            // ms

	int tdis;
	tdis = (int)( t / (int)pCmd->pIntValue[3]);  // 每个点的时间间隔 ms
	tdis -= m_cmdParam.nLowTime;  // 循环周期间隔

	dcp[m_CurToolNo].highTime = tdis; // 出胶周期间隔

	short sTemp=m_pController->TS_ListWCRect(CRD1,&dcp[m_CurToolNo],&lWclp);
	return sTemp;
}
// 称重划线
short CCmdRun::RunWCLine(tgCmdLine *pCmd,tgCmdLine *pNextTrackCmd,double dOffsetX,double dOffsetY)
{
	t_WCLParam     lWclp;   // 称重划线参数
	::ZeroMemory(&lWclp,sizeof(t_WCLParam));

	// 划线位置
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

	// 空跑速度
	lWclp.dVelXY    = m_cmdParam.dVelXY;   
	lWclp.dVelZ     = m_cmdParam.dVelZ;   

	//出胶速度 
	lWclp.dWorkVel  = pCmd->pDblValue[9];//m_cmdParam.tLine.dVel;

	//开关阀
	lWclp.bOn = TRUE;

	//开关临时延时 
	lWclp.dPreTime=m_cmdParam.tLine.dPreTime; //s to ms


	lWclp.DispWegiht  = pCmd->pDblValue[8];      // 出胶重量
	lWclp.dEndZupHigh = m_cmdParam.dZclearance;  // Z轴提升高度
	lWclp.dZcleaType  = m_cmdParam.dZcleaType;   // Z提升高度
	lWclp.dEndTime    = m_cmdParam.tLine.dTailTime;  // 点胶结束停胶时间

	///收尾动作
	lWclp.dEndVel		=	m_cmdParam.tLine.dEndVel;
	lWclp.dEndLen		=	m_cmdParam.tLine.dEndLen;
	lWclp.iEndType		=	m_cmdParam.tLine.iEndType;
	lWclp.dTailCloseLen	=	m_cmdParam.tLine.dTailCloseLen;

	if (pNextTrackCmd->tType==CMD_LINE_PASSING)
		lWclp.AddE_Trace=false;
	else
		lWclp.AddE_Trace=true;

	lWclp.AddS_Trace = RunWCL_addSTrack;

	//工具设置
	dcp[m_CurToolNo].bBit = lWclp.bOn;
	dcp[m_CurToolNo].lPulseNum = pCmd->pIntValue[3];  // 出胶次数
	dcp[m_CurToolNo].lowTime = m_cmdParam.nLowTime;   // 出胶周期 

	//
	double disx = dxB-dxA ;
	double disy = dyB-dyA ;
	double dis=sqrt((disx)*(disx)+(disy)*(disy));// 线段长   mm
	double t = dis / lWclp.dWorkVel;             // 需要的时间    s
	t= 1000*t;                            // ms

	int tdis = (int)( t / (int)pCmd->pIntValue[3]);  // 每个点的时间间隔 ms
	tdis -= m_cmdParam.nLowTime;  // 循环周期间隔

	dcp[m_CurToolNo].highTime = tdis; // 出胶周期间隔

	short sTemp=m_pController->TS_ListWCLine(CRD1,&dcp[m_CurToolNo],&lWclp);
	return sTemp;
}

//  称重打点
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

	pdp.dVelXY = m_cmdParam.dVelXY;                // 速度
	pdp.dVelZ  = m_cmdParam.dVelZ;
	pdp.dZcleaType = m_cmdParam.dZcleaType;  
	pdp.dZclearance = m_cmdParam.dZclearance; 

	pdp.dZupHVel = m_cmdParam.tPoint.dZupHVel;
	pdp.dZupLVel = m_cmdParam.tPoint.dZupLVel;

	g_pFrm->m_Precision.Measure2Real(dy);

	pdp.pos.x = dx;   // 位置
	pdp.pos.y = dy;
	pdp.pos.z = dz;
	pdp.pos.a = 0;

	pdp.dPreTime = m_cmdParam.tPoint.dPreTime;    // 前延时间
	pdp.dZuphigh = m_cmdParam.tPoint.dZupHigh;    // Z抬高高度
	pdp.dNodeTime = m_cmdParam.tPoint.dTailTime;  // 点胶结束暂停时间

	//开关阀
	if(pCmd->pIntValue[1]>0) 
		pdp.bOn = TRUE;
	else 
		pdp.bOn = FALSE;

	int nDispNum;
	nDispNum=(int) (pCmd->pDblValue[4] / g_pFrm->m_pSysParam->dBCMGPorDOT);   // 计算打点个数

	//出胶延时            
	pdp.dDispTime = (pCmd->pIntValue[4])*(m_cmdParam.nLowTime + m_cmdParam.nHighTime) / 1000.;//临时延时时间

	//dcp.bBitEnable	= TRUE;
	//m_CurToolNo = pCmd->pIntValue[0];

	dcp[m_CurToolNo].bBit =       pdp.bOn;  
	dcp[m_CurToolNo].lowTime =    m_cmdParam.nLowTime;  // 出胶时间
	dcp[m_CurToolNo].highTime =   m_cmdParam.nHighTime; // 出胶间隔
	dcp[m_CurToolNo].lPulseNum =  pCmd->pIntValue[4];   // 次数

	short sTemp;
	sTemp = m_pController->TS_ListPoint(CRD1,&dcp[m_CurToolNo],&pdp);//pCmd->pIntValue[0] 阀ID
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

	/*阵列行*/
	//pCmd->pDblValue[0]//x坐标
	//pCmd->pDblValue[1]//y坐标
	//pCmd->pIntValue[0]//开始地址
	//pCmd->pIntValue[1]//1:S,2:N
	//pCmd->pIntValue[2]//X数目
	//pCmd->pIntValue[3]//Y数目

	double dMyTempX  = pCmd->pDblValue[0];
	DOUBLE dMyTempY  = pCmd->pDblValue[1];

	double m_dCurTempH;

	/////////////////////////////////
	numx = pCmd->pIntValue[2];//X数目
	numy = pCmd->pIntValue[3];//Y数目
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
// 单步执行
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


	//确保运行前机器停下来
	sTemp = m_pController->AxisStop(0xFF);
	if(RTN_NONE!=sTemp) return RTN_NONE;

	for(unsigned short p=0;p<m_pController->g_ExMtnPara->axisNum;p++)
	{//清除限位等状态
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

	//读取前后运动位置点				
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
	case CMD_MARK://Mark设置
		sTemp=m_pController->TS_ListWaitStop();
		if (sTemp!=RTN_NONE) return sTemp;

		if(!FindMark(&pCmd1))
		{
			if(m_tSysStatus==K_RUN_STS_RUN)
			{
				g_pFrm->AddMsg("没有找到MARK点，定位错误！");
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
/*函数说明：自动运行执行函数
/*参数说明： int iStartIndex, 起始加工行号
/*            int iEndIndex,   终止加工行号
/*            double dOffsetX, 
/*            double dOffsetY,
/*            double dOffsetZ
/***********************************/
short CCmdRun::RunSub(int iStartIndex, int iEndIndex, double dOffsetX,double dOffsetY,double dOffsetZ)
{
	tgCmdLine pPreCmd;   // 上一条指令
	tgCmdLine pCurCmd;   // 当前指令
	tgCmdLine pNextCmd;   // 下一条指令
	tgCmdLine tPreTrackCmd;  // 当前指令的上一条有运动信息的指令
	tgCmdLine tNextTrackCmd; // 当前指令的下一条有运动信息的指令

	//	unsigned int SubNoBuff[100];      // 调用子程序断点位置
	//	unsigned int SubNoIndex = 0;      // 子程序号
	//	char         ProgOrSub[200];      // 0:Prog  1:Sub

//	bool    progEnd = true;           // 是否第一次到程序结束
	short   sRtnTemp  = RTN_NONE;     // 执行返回状态
	PoaIndex=0;               // 子程序组序号清零

	// 临时变量
	unsigned int uiTemp = 0;         
	CString      strMsgOut;         // 信息输出临时量 

	// 运行行号检测
	if(iStartIndex >= iEndIndex)
		return RTN_NONE;

	// 加工信息输出
	strMsgOut.Format("From %d to %d",iStartIndex,iEndIndex - 1);
	g_pFrm->AddMsg(strMsgOut);

	//清除轴状态
	for(unsigned short p=0;p<m_pController->g_ExMtnPara->axisNum;p++)
	{
		sRtnTemp = m_pController->ClrAxisSts(p+1);
		if(RTN_NONE!=sRtnTemp) 
			return RTN_NONE;
	}

	// 初始化缓冲区  屏蔽z轴抬高动作添加到缓冲区
	sRtnTemp = m_pController->TS_ListInit(CRD1,1);  
	if(RTN_NONE!=sRtnTemp)
		return RTN_NONE;

	// 初始化跳转偏移
	for(int t=0;t<m_pController->g_ExMtnPara->axisNum;t++)
		m_GotoOffSet[t]=0.0;

	// 前一条指令Z轴位置清零
	m_pController->g_ExMtnPara->dfBakpos[Z_AXIS] = g_pFrm->m_pRobotParam->m_zSafePos;

	pPreCmd.tType = CMD_NONE;   // 起始时，上一条指令为NULL

	LoopAddCount = -20;       // 前瞻个数

	// 预读两条
	if(GetRecord(pCurCmd,iStartIndex))     
		return RTN_PARAMETER_ERR;      

	if(GetRecord(pNextCmd,iStartIndex+1))
		return RTN_PARAMETER_ERR;

	//从起始加工位置运动到程序结束位置
	for(int uiCount=iStartIndex; uiCount<iEndIndex; uiCount++)
	{
		m_iCurIndex = uiCount;  // 记录当前执行指令行号

		//读取前后运动位置点	   			
		GetPreDot(uiCount, tPreTrackCmd);
		GetNextDot(uiCount, tNextTrackCmd);
		// 输出当前指令信息
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
		case CMD_MARK:                 //  Mark设置
			// 启动运动并等待运动停止
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			g_pView->ImgStop();
			if(!FindMarkEx(&pCurCmd))
			{
				return RTN_MARK_ERROR;
			}
			// 重新创建坐标系
			sRtnTemp=m_pController->TS_ListInit(CRD1, 1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;
//			progEnd = true;
			break;


			//////////////////////////////////////////////////////////////////////////

// 			// 启动运动并等待运动停止  
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
// 			// 重新创建坐标系
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
		case CMD_REPEATX: // X方向阵列 
			sRtnTemp = RunRepeat(1,/*uiCount,*/pCurCmd,dOffsetX,dOffsetY);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;
			break;
		case CMD_REPEATY: // Y方向阵列
			sRtnTemp = RunRepeat(2,/*uiCount,*/pCurCmd,dOffsetX,dOffsetY);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;
			break;

		case CMD_CALL_SUB: // 调用子程序
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			//获取子函数开始地址
			if(pCurCmd.pIntValue[0] == 1)  
			{   
				//1 标号 
				uiTemp = SearchLabel(pCurCmd.pszNote);
				if(uiTemp == RTN_PARAMETER_ERR)  break;	
				pCurCmd.pIntValue[1] = uiTemp;  // 标号对应开始地址
			}

			PoaIndex ++;//子程序组的序数	
			uiTemp = pCurCmd.pIntValue[1]; //地址或者标号 对应地址

			// 获取第一个喷射起点坐标
			// 算两起点间的偏差，备份当前位置 pCmd1 当前调用命令行
			GetGotoOffsetEx(uiTemp,pCurCmd); 	
			///////////////////////////////////////

			if(GetRecord(pCurCmd,uiTemp)) return RTN_PARAMETER_ERR;   //从子程序处开始 读取两条指令
			if(GetRecord(pNextCmd,uiTemp+1)) return RTN_PARAMETER_ERR;

			//将子程序开始位置设为当前执行位置
			uiCount = uiTemp - 1;  			
			continue;	

		case CMD_FILESCRPEWORK:  // 拼版加工符
			//启动并等待运动停止
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			g_pFrm->AddMsg("拼版加工");
			// 重新创建坐标系
			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;

			//将子程序开始位置设为当前执行位置
			uiCount = pCurCmd.pIntValue[1]; 
			if(GetRecord(pCurCmd,uiCount))     
				return RTN_PARAMETER_ERR;      

			if(GetRecord(pNextCmd,uiCount+1))
				return RTN_PARAMETER_ERR;
//			progEnd= true;
			break;
		case CMD_FILEDELIMITER:  // 分段符
			//启动并等待运动停止
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			// 发送分段进料请求给PLC
			ExOutput(_T("分段加工请求"),TRUE);
			CFunction::DelaySec(0.3);                   // s
			ExOutput(_T("分段加工请求"),FALSE);

			m_EvtAskSeparaRun.ResetEvent();
			::WaitForSingleObject(m_EvtAskSeparaRun,INFINITE);
			m_EvtAskSeparaRun.ResetEvent();

			g_pFrm->AddMsg("分段加工开始");
			// 重新创建坐标系
			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;

			//将子程序开始位置设为当前执行位置
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

// 			if (progEnd)//第一次到结束点
// 			{
				PoaIndex=0;
// 			}
// 			progEnd=false;

			// 没有“CMD_CALL_SUB”指令
			if (GetFindCallSub(uiCount+1+PoaIndex,pCurCmd)==-1)
			{ 
				// 没有跳转指令
				if(GetFindCallJump(uiCount+1+PoaIndex,pCurCmd)==-1)
				{
					// 没有分段指令
					if(GetFindCallFiledLimiter(uiCount+1+PoaIndex,pCurCmd)==-1)
					{
						// 没有拼版指令
						if(GetFindCallFiledScorp(uiCount+1+PoaIndex,pCurCmd)==-1)
						{ 
							m_iCurIndex=0;
							//启动运动且等待停止
							sRtnTemp=m_pController->TS_ListWaitStop();
							return sRtnTemp;
						}else
						{
							if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
								return RTN_PARAMETER_ERR;
						}
					}else  // 有分段指令
					{
						if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
							return RTN_PARAMETER_ERR;
					}
				}else  // 跳转指令下继续
				{
					// 获取子函数第一条指令
					if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
						return RTN_PARAMETER_ERR;
				}
			}
			else // 有子函数调用指令
			{   
				// 获取子函数第一条指令
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
	// 基板夹角
	m_cmdParam.dBoardArcForRepeat =pCmd.pDblValue[0];

	return RTN_NONE;
}
/***********************************/
/*函数说明：飞胶模式自动运行执行函数
/*参数说明： int iStartIndex, 起始加工行号
/*            int iEndIndex,   终止加工行号
/*            double dOffsetX, 
/*            double dOffsetY,
/*            double dOffsetZ
/***********************************/
short CCmdRun::RunSub_EX(int iStartIndex, int iEndIndex, double dOffsetX,double dOffsetY,double dOffsetZ)
{
	tgCmdLine pPreCmd;   // 上一条指令
	tgCmdLine pCurCmd;   // 当前指令
	tgCmdLine pNextCmd;   // 下一条指令
	tgCmdLine tPreTrackCmd;  // 当前指令的上一条有运动信息的指令
	tgCmdLine tNextTrackCmd; // 当前指令的下一条有运动信息的指令

	bool    progEnd = true;           // 是否第一次到程序结束
	short   sRtnTemp = RTN_NONE;      // 执行返回状态
	PoaIndex=0;                       // 子程序组序号清零

	// 临时变量
	unsigned int uiTemp = 0;         
	CString      strMsgOut;         // 信息输出临时量 

	// 运行行号检测
	if(iStartIndex >= iEndIndex)
		return RTN_NONE;

	// 加工信息输出
	strMsgOut.Format("From %d to %d",iStartIndex,iEndIndex - 1);
	g_pFrm->AddMsg(strMsgOut);

	//清除轴状态
	for(unsigned short p=0;p<m_pController->g_ExMtnPara->axisNum;p++)
	{
		sRtnTemp = m_pController->ClrAxisSts(p+1);
		if(RTN_NONE!=sRtnTemp) 
			return RTN_NONE;
	}

	// 初始化缓冲区  z轴抬高动作添加到缓冲区
	sRtnTemp = m_pController->TS_ListInit(CRD1,1);  
	if(RTN_NONE!=sRtnTemp)
		return RTN_NONE;

	// 初始化跳转偏移
	for(int t=0;t<m_pController->g_ExMtnPara->axisNum;t++)
		m_GotoOffSet[t]=0.0;

	// 前一条指令Z轴位置清零
	m_pController->g_ExMtnPara->dfBakpos[Z_AXIS] = g_pFrm->m_pRobotParam->m_zSafePos;

	pPreCmd.tType = CMD_NONE;   // 起始时，上一条指令为NULL

	LoopAddCount = -20;       // 前瞻个数

	m_cmdHComperList.RemoveAll(); 
	// 预读两条
	if(GetRecord(pCurCmd,iStartIndex))     
		return RTN_PARAMETER_ERR;      

	if(GetRecord(pNextCmd,iStartIndex+1))
		return RTN_PARAMETER_ERR;

	//从起始加工位置运动到程序结束位置
	for(int uiCount=iStartIndex; uiCount<iEndIndex; uiCount++) 
	{
		while (K_RUN_STS_PAUSE == m_tSysStatus)
		{
			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION, "暂停");
			Sleep(100);
		}

		m_iCurIndex = uiCount;  // 记录当前执行指令行号

		//读取前后运动位置点	   			
		GetPreDot(uiCount, tPreTrackCmd);
		GetNextDot(uiCount, tNextTrackCmd);
		// 输出当前指令信息
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
		case CMD_POT:        // 称重打点
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

		case CMD_REPEATX: // X方向阵列 
			sRtnTemp = RunRepeat(1,/*uiCount,*/pCurCmd,dOffsetX,dOffsetY);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;
			break;
		case CMD_REPEATY: // Y方向阵列
			sRtnTemp = RunRepeat(2,/*uiCount,*/pCurCmd,dOffsetX,dOffsetY);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;
			break;

		case CMD_ReverseBoard:
			// 启动运动并等待运动停止
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			{	
				m_pController->AxisMove(Z_AXIS,g_pFrm->m_pRobotParam->m_zSafePos, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
			
				g_pFrm->m_Precision.LineMoveXY(CRD1,0,g_pFrm->m_pSysParam->tDefHomePos.x,
					g_pFrm->m_pSysParam->tDefHomePos.y,g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax,
					g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			}
			//////////////////////////////////////////////////////////////////////////
			// 翻板动作
			g_pFrm->m_Robot->GotoSafeZpos(FALSE);

			sRtnTemp = (short)ReverseBoard();
			if (1 != sRtnTemp)
			{
				return RTN_Reverse_Err;
			}

			// 重新创建坐标系
			sRtnTemp=m_pController->TS_ListInit(CRD1, 1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;
			break;

		case CMD_MARK:         //  Mark设置
			// 启动运动并等待运动停止
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			g_pView->ImgStop();
			if(!FindMarkEx(&pCurCmd))
			{
				return RTN_MARK_ERROR;
			}
			// 重新创建坐标系
			sRtnTemp=m_pController->TS_ListInit(CRD1, 1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;

			progEnd = true;
			break;

		case CMD_CALL_SUB: // 调用子程序
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			//获取子函数开始地址
			if(pCurCmd.pIntValue[0] == 1)  
			{   
				//1 标号 
				uiTemp = SearchLabel(pCurCmd.pszNote);
				if(uiTemp == RTN_PARAMETER_ERR)  break;	
				pCurCmd.pIntValue[1] = uiTemp;  // 标号对应开始地址
			}

			PoaIndex ++;//子程序组的序数	
			uiTemp = pCurCmd.pIntValue[1]; //地址或者标号 对应地址

			// 获取第一个喷射起点坐标
			// 算两起点间的偏差，备份当前位置 pCmd1 当前调用命令行
			GetGotoOffsetEx(uiTemp,pCurCmd); 	
			///////////////////////////////////////

			if(GetRecord(pCurCmd,uiTemp)) return RTN_PARAMETER_ERR;   //从子程序处开始 读取两条指令
			if(GetRecord(pNextCmd,uiTemp+1)) return RTN_PARAMETER_ERR;

			//将子程序开始位置设为当前执行位置
			uiCount = uiTemp - 1;  			
			continue;	

		case CMD_FILEDELIMITER:  // 分段符
			//启动并等待运动停止
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			// 发送分段进料请求给PLC
			ExOutput(_T("分段加工请求"),TRUE);
			CFunction::DelaySec(0.3);                   // s
			ExOutput(_T("分段加工请求"),FALSE);

			m_EvtAskSeparaRun.ResetEvent();
			::WaitForSingleObject(m_EvtAskSeparaRun,INFINITE);
			m_EvtAskSeparaRun.ResetEvent();

			g_pFrm->AddMsg("分段加工开始");
			// 重新创建坐标系
			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;

			//将子程序开始位置设为当前执行位置
			uiCount = pCurCmd.pIntValue[1]; 
			if(GetRecord(pCurCmd,uiCount))     
				return RTN_PARAMETER_ERR;      

			if(GetRecord(pNextCmd,uiCount+1))
				return RTN_PARAMETER_ERR;
			progEnd= true;
			break;	
		case CMD_FILESCRPEWORK:  // 拼版加工符
			//启动并等待运动停止
			sRtnTemp=m_pController->TS_ListWaitStop();
			if(sRtnTemp!=RTN_NONE) return sRtnTemp;

			g_pFrm->AddMsg("拼版加工");
			// 重新创建坐标系
			sRtnTemp=m_pController->TS_ListInit(CRD1,1);
			if(sRtnTemp!=RTN_NONE) 
				return sRtnTemp;

			//将子程序开始位置设为当前执行位置
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

			if (progEnd)//第一次到结束点
			{
				PoaIndex=0;
			}
			progEnd=false;

			// 是否有“CMD_CALL_SUB”指令
			if (GetFindCallSub(uiCount+1+PoaIndex,pCurCmd)==-1)
			{  
				if(GetFindCallJump(uiCount+1+PoaIndex,pCurCmd)==-1)
				{
					if(GetFindCallFiledLimiter(uiCount+1+PoaIndex,pCurCmd)==-1)
					{
						// 没有拼版指令
						if(GetFindCallFiledScorp(uiCount+1+PoaIndex,pCurCmd)==-1)
						{ 
							m_iCurIndex=0;
							//启动运动且等待停止
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
					// 获取子函数第一条指令
					if(GetRecord(pCurCmd,uiCount+1+PoaIndex)) 
						return RTN_PARAMETER_ERR;
				}
			}
			else
			{
				// 获取子函数第一条指令
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
/*获取当前行上一条有运动信息的指令                                                            */
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
			|| ListType == CMD_SUPERSTART   //飞胶起点
			|| ListType == CMD_SUPERPOINT   //飞胶单点
			|| ListType == CMD_SUPEREND    )//飞胶结束点
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
int CCmdRun::GetFindCallSub(int iCurIndex, tgCmdLine &tDot)//有子程序指令
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
int CCmdRun::GetFindCallJump(int iCurIndex, tgCmdLine &tDot) //有跳转指令
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

int CCmdRun::GetFindCallFiledLimiter(int iCurIndex, tgCmdLine &tDot) //有分段指令
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

int CCmdRun::GetFindCallFiledScorp(int iCurIndex, tgCmdLine &tDot) //有拼版指令
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
// 获取一条指令
short CCmdRun::GetRecord(tgCmdLine &address,int Index)
{
	if (m_cmdRunList.IsEmpty()) return 1;

	if (Index>=m_cmdRunList.GetSize()) return 1;

	tgCmdLine pMyTempCmd;

	POSITION posIndex = m_cmdRunList.FindIndex(Index);
	pMyTempCmd = m_cmdRunList.GetAt(posIndex);

	address = pMyTempCmd;

	// 如果为运动有关指令 指令位置附加偏移量
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

// 获取当前点与跳转点之间的位置偏差
void CCmdRun::GetGotoOffset(int uiTemp,tgCmdLine pCmd)
{
	tgCmdLine pCmd0;                // 临时试教指令 

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
			m_GotoOffSet[X_AXIS] = pCmd.pDblValue[0] - pCmd0.pDblValue[0]-pCmd.pDblValue[4]/*子函数调用微调距离*/;
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
// 获取当前点与跳转点之间的位置偏差
void CCmdRun::GetGotoOffsetEx(int uiTemp,tgCmdLine pCmd)
{
	tgCmdLine pCmd0;                // 临时试教指令 
	tgCmdLine ptempCmd,ptempCmd1;   // 临时指令 

	m_GotoOffSet[X_AXIS] = 0;
	m_GotoOffSet[Y_AXIS] = 0;
	m_GotoOffSet[Z_AXIS] = 0;

	// 获取第一个飞射起点坐标
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
			GetRecord(ptempCmd,i);     // 获取飞胶起点坐标
			GetRecord(ptempCmd1,i+1);  // 获取飞胶第一个点坐标
			break;
		}
	}	
	// 计算第一个起点和第一个喷射点Y偏差
	double  dYdis;
	dYdis = ptempCmd1.pDblValue[1]-ptempCmd.pDblValue[1];
	if(dYdis <0)
		dYdis = -dYdis;

	// 子函数调用处起点Y轴位置
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

// 跳转到指定位置执行
short CCmdRun::RunGoto(int * uiIndex,tgCmdLine &address0,tgCmdLine &address1,tgCmdLine &address2)
{
	int uiPara;

	uiPara = address1.pIntValue[1]; //地址

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

short CCmdRun::DotDisp(int dotNum, RunMode md)	// 喷射指定数量的的点
{
// 	// 启动运动
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
				AfxMessageBox("称重打点时写入点胶阀参数出错!");
				return -1;
			}
		}

		if (RunMode_Wet == m_runMode)
			ExOutput(_T("1号点固态电器"), TRUE);
		double dDispTime = dotNum*dDispCyc;
		dDispCyc *= 1.2;
		CFunction::DelaySec(dDispTime/1000.0);
		ExOutput(_T("1号点固态电器"), FALSE);
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
					AfxMessageBox("打点时写入点胶阀参数出错!");
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

		// 重新创建坐标系
		sTemp=m_pController->TS_ListInit(CRD1, 1);
		return sTemp;
	}

 	short rtn = 0;
// 	rtn = g_pFrm->m_Robot->m_pController->AxisRelativeMove(U_AXIS, dRelPos, dVel, g_pFrm->m_mtrParamGts[U_AXIS].m_Acc);
// 	if (RTN_NONE != rtn)
// 	{
// 		AfxMessageBox("喷射点失败!");
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	// 启动运动并等待运动停止
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
	double dWtVal[3];	// 电子称读数
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
			AfxMessageBox("单点胶重太小!");
			return -1;
		}

		dAvgWt = dWtVal[i] / dotNum; 
		if ((dAvgWt > dotWt*2) || (dAvgWt < dotWt/2.0))
		{
			AfxMessageBox("称重校正出错!");
			return -1;
		}
	}

	if (g_pFrm->m_pRobotParam->m_dWasteGlueCur > (g_pFrm->m_pRobotParam->m_dWasteGlueAlarm*1000))
	{
		g_pFrm->m_Robot->ExOutput(_T("三色灯.红"),TRUE);
		g_pFrm->m_Robot->ExOutput(_T("蜂鸣器"),TRUE);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.m_bWasteGlueAlarm = true;
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetCurWasteGlue("废胶盒已满");
	}
	else
	{
		CString strTemp;
		strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_dWasteGlueCur/1000.0);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetCurWasteGlue(strTemp);
	}

	dotWt = (dWtVal[0] + dWtVal[1] + dWtVal[2])/3.0/dotNum;

	g_pFrm->m_pRobotParam->m_dAvgWeight = dotWt;
	// 保存单点胶重
	CString strFile = CFunction::GetDirectory() + "system\\robotparam.dat";
	CFunction::HandleDouble(FALSE, "AvgWeight", "AvgWeight", g_pFrm->m_pRobotParam->m_dAvgWeight, strFile);	

	// 称重计时
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
		rtn = ExOutput(_T("探针气缸电磁阀"), FALSE);
	
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
		AfxMessageBox("单点不能放在线段开始点与线段结束点之间!"); 
		bRet = FALSE;
		break;
	case SYNTAX_WCL_PARAERROR:
		AfxMessageBox("称重划线速度与出胶周期时间不匹配");
		bRet = FALSE;
		break;
	case SYNTAX_NO_LINE_END: 
		AfxMessageBox("没有线段结束点!"); 
		bRet = FALSE;
		break;
	case SYNTAX_NO_LINE_START:
		AfxMessageBox("没有线段开始点!");
		bRet = FALSE;
		break;
	case SYNTAX_LINE_PASS_ERR:
		str="线段中间点前面只能设置为:1.线段开始点,2.线段中间点,3.圆弧点,4.阀开/关!";
		AfxMessageBox(str);
		bRet= FALSE;
		break;
	case SYNTAX_ARC_ERR:
		str="圆弧点前面只能设置为:1.线段开始点,2.线段中间点,3.阀开/关!";
		AfxMessageBox(str);
		bRet= FALSE;
		break;
	case SYNTAX_DATA_VOER: 
		AfxMessageBox("数值溢出!");
		bRet = FALSE;
		break;
	case SYNTAX_GOTO_ADD_ERR:
		AfxMessageBox("没有找到要跳转的地址或标号，或地址非法!");
		bRet = FALSE;
		break; 
	case SYNTAX_LINE_END_ERR:
		str="线段结束点前面只能设置为:1.线段开始点,2.线段中间点,3.圆弧点,4.阀开/关,5.区域涂布!";
		AfxMessageBox(str);
		bRet= FALSE;
		break;	
	case SYNTAX_CIRCLE_ERROR:
		str="圆形不能放在线段起始点与结束点之间!";
		AfxMessageBox(str);
		bRet= FALSE;
		break;	
	case SYNTAX_LOOP_ADD_ERR:
		AfxMessageBox("循环指令开始地址大于当前地址!");
		bRet = FALSE; 
		break;
	case SYNTAX_NO_END_PROGRAME:
		AfxMessageBox("没有程式结束标志!");
		bRet = FALSE;
		break;
	case SYNTAX_BRUSHAREA_ERROR:
		AfxMessageBox("涂覆指令不能在线段起始点与线段结束点之间!"); 
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
		case CMD_SUPERDOT:	// 高速打点
		case CMD_SUPERPOINT:// 高速中间点
		case CMD_POINT:		// 单点点胶
		case CMD_ZMETER:	// 测高
		case CMD_DUMMY:		// 空点	
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE)	return SYNTAX_DATA_VOER;
			if(LineStart==1) 			return SYNTAX_POINT_DISPENSE_ERROR;//直线开始了还没结束
			break;

		case CMD_SUPEREND:
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE)	return SYNTAX_DATA_VOER;
			if(LineStart==0)			return SYNTAX_NO_LINE_START;//前面没直线起点
			else LineStart = 0; 
			break;

		case CMD_SUPERSTART:
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE)
				return SYNTAX_DATA_VOER;
			if(LineStart==1) 	return SYNTAX_POINT_DISPENSE_ERROR;//直线开始了还没结束
			else LineStart = 1; 
			break;
			
		case CMD_LINE_START:	// 直线开始
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE)	return SYNTAX_DATA_VOER;
			if(LineStart==1)			return SYNTAX_NO_LINE_END;//直线开始了还没结束
			else LineStart = 1;			break;
			
		case CMD_LINE_PASSING:	// 直线中间点
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE) return SYNTAX_DATA_VOER;
			//if(LineStart==0) return SYNTAX_NO_LINE_START;////前面没直线起点
			//GetPreLineDot(Count, tempCmdLine);
			GetPreDot(Count, tempCmdLine);
			if((tempCmdLine.tType!=CMD_LINE_START)
				&&(tempCmdLine.tType!=CMD_LINE_ARC)
				&&(tempCmdLine.tType!=CMD_LINE_PASSING))
				//&&(tempCmdLine.tType!=CMD_DISPENSER))
				return SYNTAX_LINE_PASS_ERR;
			//return SYNTAX_NO_ERROR;
			break;
						
		case CMD_LINE_ARC:	// 圆弧
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
			// 直线结束点
		case CMD_LINE_END:
			checkPosRet = CheckPos(&pCmd);
			if(checkPosRet !=RTN_NONE) return SYNTAX_DATA_VOER;
			if(LineStart==0) return SYNTAX_NO_LINE_START;//前面没直线起点
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
			// 圆形点胶
		case CMD_CIRCLE:   
			// 轨迹超出范围
			if(LineStart==1) return SYNTAX_CIRCLE_ERROR;//直线开始了还没结束

			if(pCmd.pIntValue[1]==0)//圆心＋直径
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
			if (pCmd.pIntValue[0]==1)//标号
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
			if(LineStart==1)	return SYNTAX_BRUSHAREA_ERROR;//直线开始了还没结束
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

			
		case CMD_REPEATX:	// 阵列检查是否从阵列中间开始点胶
		case CMD_REPEATY:
			m_structPathInfo.m_bRepeat = true;
			if (pCmd.pIntValue[4]>1 || pCmd.pIntValue[5]>1)
			{
// 				m_structPathInfo.m_bRepeatSetStart = true;
// 				m_structPathInfo.m_nRepeatStX = pCmd.pIntValue[4];
// 				m_structPathInfo.m_nRepeatStY = pCmd.pIntValue[5];

				tgCmdLine &pCmdTemp = m_cmdRunList.GetAt(posIndex);
				pCmdTemp.pIntValue[4] = 1;	// 从中间开始点胶，只执行一次
				pCmdTemp.pIntValue[5] = 1;
				posIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(Count);
				tgCmdLine &pCmdTemp1 = g_pFrm->m_wndEditBar.m_rawList.GetAt(posIndex);
				pCmdTemp1.pIntValue[4] = 1;	// 从中间开始点胶，只执行一次
				pCmdTemp1.pIntValue[5] = 1;
				CString str;
				str.Format("st id: %d, %d", m_structPathInfo.m_nRepeatStX, m_structPathInfo.m_nRepeatStY);
				g_pFrm->AddMsg(str);
			}
			break;

			// 程序结束
		case CMD_END_PROGRAM:
			have_end = 1;
			break;
		default:break;
		}
	}

	if(LineStart)		return SYNTAX_NO_LINE_END;
	if(have_end !=1 )	return SYNTAX_NO_END_PROGRAME;
	m_iDebugErrorLine = 0;//设置当前行为 第一行

	return SYNTAX_NO_ERROR;
}
// 检测称重划线数据合法性
BOOL CCmdRun::CheckWCLData(tgCmdLine *pCmd)
{
	// 划线位置
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

	//出胶速度 
	double  dWorkVel  = m_cmdParam.tLine.dVel;
	//
	double disx = dxB-dxA ;
	double disy = dyB-dyA ;
	double dis=sqrt((disx)*(disx)+(disy)*(disy)); // 线段长   mm
	double t = dis / dWorkVel;    // 需要的时间    s
	t= 1000*t;                    // s -> ms

	int tdis =(int) (t / (int)pCmd->pIntValue[3]);  // 每个点的时间间隔
	tdis -= m_cmdParam.nLowTime;  // 循环周期间隔

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

// 针头试胶功能 
void CCmdRun::NeedleTestDisp()
{
	double x=0,y=0,z=0;

	if (g_pFrm->m_pSysParam->nToolType == 2)
	{
		AfxMessageBox(_T("图像模式下不能试胶，请重新选择工作模式！"));
		return ;
	}

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		x = g_pFrm->m_pSysParam->tDispTestPos.x;
		y = g_pFrm->m_pSysParam->tDispTestPos.y;
		z = g_pFrm->m_pSysParam->tDispTestPos.z;

		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

		if (g_pFrm->m_pSysParam->nJetPulse==0)//跟随模式
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

// 试胶功能 
short CCmdRun::TestDisp()
{
	short iRtn = 0;

	if (g_pFrm->m_pSysParam->nToolType == 2)
	{
		AfxMessageBox(_T("图像模式下不能试胶，请重新选择工作模式！"));
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
			AfxMessageBox(_T("陶瓷板已经试胶满,清理陶瓷板后继续!"));
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
			iRtn = g_pFrm->m_Robot->ExOutput(_T("1号点固态电器"), TRUE);
			iRtn = g_pFrm->m_Robot->m_pController->AxisMove(X_AXIS, dPosX, g_pFrm->m_pRobotParam->m_structTestGlue.m_dWorkVel, 10);
			iRtn = g_pFrm->m_Robot->ExOutput(_T("1号点固态电器"), FALSE);

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
// 清洗针头动作
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
			
		// 移动到位
		GotoSafeZpos(); 
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
		// 打开清洗泵 
		ExOutput(_T("清洁工作站电磁阀"),TRUE);
		for(int i=0;i<num;i++)
		{   
			m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
			CFunction::DelaySec(0.5);
			m_pController->AxisMove(Z_AXIS,z+zDown, g_pFrm->m_mtrParamGts[Z_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
		}
		// 关闭清洗泵
		ExOutput(_T("清洁工作站电磁阀"),FALSE);
		g_pFrm->m_Robot->GotoSafeZpos();
	}
	else
	{
		AfxMessageBox(_T("运动控制卡未正常初始化！"));
	}

	if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
	{
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
	}
} 
// 清洗喷射阀动作
void CCmdRun::WashJet(bool bIfDisp)
{
	if (!m_pController->IsInitOk())
	{
		AfxMessageBox(_T("System Init Fail！"));
		return;
	}
	
	double x=0,y=0,z=0;

	if (!bIfDisp)
	{
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(FALSE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(FALSE);
	}
	
	if(bIfDisp || (IDYES == AfxMessageBox("请确认是否清洗!", MB_YESNO)))
	{
		x = g_pFrm->m_pRobotParam->m_structClean.m_posClean.x+g_pFrm->m_pSysParam->tOffset[0].x;   
		y = g_pFrm->m_pRobotParam->m_structClean.m_posClean.y+g_pFrm->m_pSysParam->tOffset[0].y;
		z = g_pFrm->m_pRobotParam->m_zContactPos - g_pFrm->m_pRobotParam->m_structClean.m_posClean.z;

		GotoSafeZpos();
		// 移动到位
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,m_cmdParam.dVelXY,m_cmdParam.dAccXY);
		m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

		// 打开IO
		int num = g_pFrm->m_pRobotParam->m_structClean.m_nCleanTimes;
		ExOutput("清洁工作站电磁阀",TRUE);
		for (int i=0;i<num;i++)
		{
			// 左右移动
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,x-0.8,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove/6,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,x+0.8,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove/6,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			//g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParam[X_AXIS].m_Vmin/2,g_pFrm->m_mtrParam[X_AXIS].m_Acc);
			CFunction::DelaySec(0.05);
			// 前后移动
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y+0.8,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove/6,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y-0.8,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove/6,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove/6,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			CFunction::DelaySec(0.05);
		}
		// 关闭IO
		ExOutput("清洁工作站电磁阀",FALSE);

		GotoSafeZpos();
	}

	if(!bIfDisp && theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
	{
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
	}
}

// 模拟量模块 胶桶背压比例阀操作
short CCmdRun::SetBackPressure(double dVal)
{
	short idata = (short)(dVal / 0.4 * 32767);

	short rtn = m_pController->SetDac(12, &idata);
	if (RTN_NONE != rtn)
	{
		return rtn;
	}

	// 胶桶背压
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

short CCmdRun::LoadUnLoad_loadPushOut(BOOL &bBdDetect, BOOL bToEnd)	//bBdDetect:入口是否检测到物料   bToEnd:是否推料到终点
{
	if (!m_pController->IsInitOk())						return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;
	
	short iRet = g_pFrm->m_Robot->ExOutput("进料电机脉冲",TRUE);
	if (iRet != RTN_NONE)	
	{
		iRet = g_pFrm->m_Robot->ExOutput("进料电机方向",TRUE);
		CFunction::DelaySec(2);
		iRet = g_pFrm->m_Robot->ExOutput("进料电机脉冲",FALSE);
		iRet = g_pFrm->m_Robot->ExOutput("进料电机方向",FALSE);
		AfxMessageBox("气缸动作出错!");
		return 0;
	}

	BOOL bStatus1 = FALSE, bStatus2 = FALSE;
	iRet = g_pFrm->m_Robot->ExInput("进料电机-R", bStatus1);
	CHECK_RTN_RTNVAL(_T("读取[进料电机-R]信号出错"), iRet, 0);

	CKFTimer tmTemp;
	tmTemp.TimerStart();
	while (!bStatus1)
	{
		if (tmTemp.TimerGetSecond() > 4)
		{
			g_pFrm->m_Robot->ExOutput("进料电机方向",TRUE);
			CFunction::DelaySec(2);
			g_pFrm->m_Robot->ExOutput("进料电机脉冲",FALSE);
			g_pFrm->m_Robot->ExOutput("进料电机方向",FALSE);
			AfxMessageBox("进料电机推料到位超时!");
			return 0;
		}
		iRet = g_pFrm->m_Robot->ExInput("进料电机-R", bStatus1);
		if (!bStatus2)
		{
			g_pFrm->m_Robot->ExInput("11号光开", bStatus2);
			if (bStatus2)
			{
				if (!bToEnd)	break;
			}
		}
		CFunction::DelaySec(0.010);
		Sleep(10);
	}

	iRet = g_pFrm->m_Robot->ExOutput("进料电机方向",FALSE);
	iRet = g_pFrm->m_Robot->ExInput("11号光开", bStatus2);
	if (bStatus1 && !bStatus2)
	{
		tmTemp.TimerStart();
		while (!bStatus2)
		{
			if (tmTemp.TimerGetSecond() > 2)
				break;

			iRet = g_pFrm->m_Robot->ExInput("11号光开", bStatus2);
		
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

	short iRet = g_pFrm->m_Robot->ExOutput("进料电机方向",TRUE);
	iRet |= g_pFrm->m_Robot->ExOutput("进料电机脉冲",TRUE);

	BOOL bStatus1 = FALSE;
	iRet |= g_pFrm->m_Robot->ExInput("进料电机-L", bStatus1);

	CKFTimer tmTemp;
	tmTemp.TimerStart();
	while (!bStatus1)
	{
		if (tmTemp.TimerGetSecond() > 4)
		{
			iRet = g_pFrm->m_Robot->ExOutput("进料电机方向", FALSE);
			iRet |= g_pFrm->m_Robot->ExOutput("进料电机脉冲", FALSE);
			AfxMessageBox("推料电机退回超时!");
			return 0;
		}

		CFunction::DelaySec(0.02);
		iRet |= g_pFrm->m_Robot->ExInput("进料电机-L", bStatus1);
	}

	iRet |= g_pFrm->m_Robot->ExOutput("进料电机脉冲",FALSE);
	iRet |= g_pFrm->m_Robot->ExOutput("进料电机方向",FALSE);
	return iRet;
}

short CCmdRun::LoadUnLoad_unloadPushOut()
{
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())	return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;

	short iRet = g_pFrm->m_Robot->ExOutput("2号推出料气缸电磁阀",TRUE);
	if (iRet != RTN_NONE)	
		AfxMessageBox("气缸动作出错!");

	BOOL bStatus = FALSE;
	iRet &= g_pFrm->m_Robot->ExInput("推出料气缸-R", bStatus);
	CKFTimer tmTemp;
	tmTemp.TimerStart();
	while (!bStatus)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			AfxMessageBox("推出料气缸2 到位超时");
		}

		iRet &= g_pFrm->m_Robot->ExInput("推出料气缸-R", bStatus);
		CFunction::DelaySec(0.020);
	}

	iRet &= g_pFrm->m_Robot->ExOutput("1号推出料气缸电磁阀",TRUE);
	iRet &= g_pFrm->m_Robot->ExInput("推出料气缸-L", bStatus);
	tmTemp.TimerStart();
	while (!bStatus)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			AfxMessageBox("推出料气缸1 到位超时");
		}

		iRet &= g_pFrm->m_Robot->ExInput("推出料气缸-L", bStatus);
		CFunction::DelaySec(0.020);
	}
	return iRet;
}

short CCmdRun::LoadUnLoad_unloadPullBack()
{	
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())	return 0;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)	return 0;

	short iRet = g_pFrm->m_Robot->ExOutput("1号推出料气缸电磁阀",FALSE);
	iRet &= g_pFrm->m_Robot->ExOutput("2号推出料气缸电磁阀",FALSE);
	if (iRet != RTN_NONE)	
	{
		AfxMessageBox("气缸动作出错!");
	}
	return iRet;
}

// 进料电机上下移动前的状态确认，一定要满足条件才能移动，防止卡料
short CCmdRun::LoadUnLoad_loadCheck()
{
	BOOL bStatus1=TRUE, bStatus2=FALSE;
	short iRtn = 0;

	iRtn = ExInput("11号光开", bStatus1);
	if ((iRtn!=RTN_NONE) || (bStatus1))
	{
		AfxMessageBox("入口处有物料,请检测入口信号或者线路!");
		return 0;
	}

	iRtn &= ExInput("进料电机-L", bStatus2);
	if (!bStatus2)
	{
		iRtn &= LoadUnLoad_loadPullBack();
	}
	return iRtn;
}

// 下料电机上下移动前的状态确认，一定要满足条件才能移动，防止卡料
short CCmdRun::LoadUnLoad_unloadCheck()
{
	BOOL bStatus=TRUE;
	short iRtn=0;

	ExOutput("1号推出料气缸电磁阀",FALSE);
	ExOutput("2号推出料气缸电磁阀",FALSE);
	iRtn = ExInput("14号光开", bStatus);
	if (iRtn!=RTN_NONE || bStatus)
	{
		AfxMessageBox("出口处有物料,请检测出口信号或者线路!");
		return 0;
	}
	return iRtn;
}

short CCmdRun::RotateTable_Home()	// 旋转平台回原点
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

short CCmdRun::LoadUnLoad_Home(int mtrIndex)	// 上下料回原点
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
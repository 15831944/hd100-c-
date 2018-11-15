#include "StdAfx.h"
#include "RedrawTrack.h"
#include "TSCtrlSys.h"
#define _USE_MATH_DEFINES
#include <math.h>

//CEvent g_trackevent(0,1);
//CEvent g_drawevent(0,1);

CCriticalSection g_tk;

CRedrawTrack::CRedrawTrack(void)
{
	m_OldInfo	= NULL;
	m_Thread	= NULL;
	m_bexit		= false;
	m_InterDr	= 0.5;
	m_bValveOut = FALSE;
	m_bCurLineSt = false;
	m_iCurLine	= -1;
	m_bDataUpdate = false;
	g_exitdrawevent.SetEvent();

	m_dFLeft	= 10000;
	m_dFRight	= 0;
	m_dFTop		= 10000;
	m_dFBottom	= 0;
	m_dFScale	= 1;

	m_dBLeft	= 10000;
	m_dBRight	= 0;
	m_dBTop		= 10000;
	m_dBBottom	= 0;
	m_dBScale	= 0;
}

CRedrawTrack::~CRedrawTrack(void)
{
	m_bexit = true;
	::WaitForSingleObject(g_exitdrawevent,INFINITE);
}

bool CRedrawTrack::LoadTrack()
{
	int iCount=g_pFrm->m_wndEditBar.m_rawList.GetCount();
	m_bDataUpdate = true;
	m_FrontData.RemoveAll();
	m_BackData.RemoveAll();
	LoadTrack(0,iCount);
	m_bDataUpdate = false;
	return TRUE;
}

bool CRedrawTrack::LoadTrack(int iStartIndex, int iEndIndex, double dScale/* =1 */, double dOffsetX/* =0 */, double dOffsetY/* =0 */)
{
	int i=0, /*j=0,*/ m=0, n=0;
	tgArc tArc;
	TPOINT p1, p2,p3;
	double dTemp=0;
	double dXc, dYc, dRc;
	CString strTemp;
	double x=0, y=0;
	//m_lXlen = rect.Width();
	int nx=0, ny=0;
	TRACKDATA TmpTkD;
	TmpTkD.iType = -1;
	tgCmdLine pPaintCmd;
	double dCurx = 0,dCury = 0;
	CollPos(&g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[0],
		&g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[1]);

	BOOL bFront = TRUE;	// 是否正面，默认正面，翻板指令后则为反面
	//////////////////////////////////////////////////////////////////////////
	for(i=iStartIndex; i<iEndIndex; i++)
	{
		POSITION posIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(i);
		pPaintCmd = g_pFrm->m_wndEditBar.m_rawList.GetAt(posIndex);

		switch(pPaintCmd.tType)
		{
		case CMD_NONE:
			break;

		case CMD_ReverseBoard:
			bFront = !bFront;
			break;

		case CMD_MARK:
			p1 = GetPoint(0,pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dScale, dOffsetX, dOffsetY);
			TmpTkD.iType = T_MARK;
			TmpTkD.dPos[0] = p1.x;
			TmpTkD.dPos[1] = p1.y;
			p2 = GetPoint(0,pPaintCmd.pDblValue[4], pPaintCmd.pDblValue[5], dScale, dOffsetX, dOffsetY);
			TmpTkD.dPos[2] = p2.x;
			TmpTkD.dPos[3] = p2.y;
			if (bFront)
			{
				m_FrontData.Add(TmpTkD);
				if (p1.x>m_dFRight)		m_dFRight	= p1.x;
				if (p1.x<m_dFLeft)		m_dFLeft	= p1.x;
				if (p1.y>m_dFBottom)	m_dFBottom	= p1.y;
				if (p1.y<m_dFTop)		m_dFTop		= p1.y;

				if (p2.x>m_dFRight)		m_dFRight	= p2.x;
				if (p2.x<m_dFLeft)		m_dFLeft	= p2.x;
				if (p2.y>m_dFBottom)	m_dFBottom	= p2.y;
				if (p2.y<m_dFTop)		m_dFTop		= p2.y;
			}
			else
			{
				m_BackData.Add(TmpTkD);
				if (p1.x>m_dBRight)		m_dBRight	= p1.x;
				if (p1.x<m_dBLeft)		m_dBLeft	= p1.x;
				if (p1.y>m_dBBottom)	m_dBBottom	= p1.y;
				if (p1.y<m_dBTop)		m_dBTop		= p1.y;

				if (p2.x>m_dBRight)		m_dBRight	= p2.x;
				if (p2.x<m_dBLeft)		m_dBLeft	= p2.x;
				if (p2.y>m_dBBottom)	m_dBBottom	= p2.y;
				if (p2.y<m_dBTop)		m_dBTop		= p2.y;
			}
			break;

		case CMD_POINT:
		case CMD_SUPERDOT:
			p1 = GetPoint(0,pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dScale, dOffsetX, dOffsetY);
			TmpTkD.iType = T_POINT;
			TmpTkD.dPos[0] = p1.x;
			TmpTkD.dPos[1] = p1.y;
			if (bFront)
			{
				m_FrontData.Add(TmpTkD);
				if (p1.x>m_dFRight)		m_dFRight	= p1.x;
				if (p1.x<m_dFLeft)		m_dFLeft	= p1.x;
				if (p1.y>m_dFBottom)	m_dFBottom	= p1.y;
				if (p1.y<m_dFTop)		m_dFTop		= p1.y;
			}
			else
			{
				m_BackData.Add(TmpTkD);
				if (p1.x>m_dBRight)		m_dBRight	= p1.x;
				if (p1.x<m_dBLeft)		m_dBLeft	= p1.x;
				if (p1.y>m_dBBottom)	m_dBBottom	= p1.y;
				if (p1.y<m_dBTop)		m_dBTop		= p1.y;
			}
			break;//单点点胶,tgPos[1]

		case CMD_LINE_START:
		case CMD_SUPERSTART:
			p1 = GetPoint(0,pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dScale, dOffsetX, dOffsetY);
			dCurx = p1.x;
			dCury = p1.y;
			if (bFront)
			{
				if (p1.x>m_dFRight)		m_dFRight	= p1.x;
				if (p1.x<m_dFLeft)		m_dFLeft	= p1.x;
				if (p1.y>m_dFBottom)	m_dFBottom	= p1.y;
				if (p1.y<m_dFTop)		m_dFTop		= p1.y;
			}
			else
			{
				if (p1.x>m_dBRight)		m_dBRight	= p1.x;
				if (p1.x<m_dBLeft)		m_dBLeft	= p1.x;
				if (p1.y>m_dBBottom)	m_dBBottom	= p1.y;
				if (p1.y<m_dBTop)		m_dBTop		= p1.y;
			}
			break;//线段点胶起点tgPos[1]

		case CMD_LINE_PASSING:
		case CMD_SUPERPOINT:
			if(pPaintCmd.pIntValue[6] && pPaintCmd.pDblValue[6]>1)
			{
				tgCmdLine tPreDotEx,tNextDotEx;
				int m0=-1, m2=-1;
				m0 = g_pFrm->m_wndEditBar.GetPreTrackPoint(i, TRUE);
				m2 = g_pFrm->m_wndEditBar.GetNextTrackPoint(i, TRUE);
				if(m2<0) 
					goto lab;

				POSITION PreposIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(m0);
				tPreDotEx = g_pFrm->m_wndEditBar.m_rawList.GetAt(PreposIndex);

				POSITION NextposIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(m2);
				tNextDotEx = g_pFrm->m_wndEditBar.m_rawList.GetAt(NextposIndex);

				double m_CirX=0,m_CirY=0;
				BOOL dir;
				BOOL IsRetOK=g_pFrm->m_wndEditBar.TrackArcHandle(tPreDotEx,pPaintCmd,tNextDotEx,pPaintCmd.pDblValue[6],m_CirX,m_CirY,dir);
				if (IsRetOK==TRUE)
				{
					p1 = GetPoint(0,tPreDotEx.pDblValue[0], tPreDotEx.pDblValue[1], dScale, dOffsetX, dOffsetY);
					p2 = GetPoint(0,m_CirX, m_CirY, dScale, dOffsetX, dOffsetY);//圆心
					p3 = GetPoint(0,tNextDotEx.pDblValue[0], tNextDotEx.pDblValue[1], dScale, dOffsetX, dOffsetY);//圆心
					dTemp = pPaintCmd.pDblValue[6]* dScale;

					TmpTkD.iType = T_LINE;
					TmpTkD.dPos[0] = dCurx;
					TmpTkD.dPos[1] = dCury;
					TmpTkD.dPos[2] = p1.x;
					TmpTkD.dPos[3] = p1.y;
					dCurx = p1.x;
					dCury = p1.y;
					m_FrontData.Add(TmpTkD);

					if(dir==TRUE)//逆时针
					{
						TmpTkD.iType = T_WCIRCLE;
						TmpTkD.dPos[0] = p1.x;
						TmpTkD.dPos[1] = p1.y;
						TmpTkD.dPos[2] = p3.x;
						TmpTkD.dPos[3] = p3.y;
						TmpTkD.dPos[4] = p2.x;
						TmpTkD.dPos[5] = p2.y;
						TmpTkD.dPos[6] = dTemp;
						dCurx = p3.x;
						dCury = p3.y;
						m_FrontData.Add(TmpTkD);
					}
					else
					{
						TmpTkD.iType = T_DCIRCLE;
						TmpTkD.dPos[0] = p3.x;
						TmpTkD.dPos[1] = p3.y;
						TmpTkD.dPos[2] = p1.x;
						TmpTkD.dPos[3] = p1.y;
						TmpTkD.dPos[4] = p2.x;
						TmpTkD.dPos[5] = p2.y;
						TmpTkD.dPos[6] = dTemp;
						dCurx = p1.x;
						dCury = p1.y;
						m_FrontData.Add(TmpTkD);
					}
					break;
				}
			lab:p1 = GetPoint(0,pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dScale, dOffsetX, dOffsetY);
				TmpTkD.iType = T_LINE;
				TmpTkD.dPos[0] = dCurx;
				TmpTkD.dPos[1] = dCury;
				TmpTkD.dPos[2] = p1.x;
				TmpTkD.dPos[3] = p1.y;
				dCurx = p1.x;
				dCury = p1.y;
				m_FrontData.Add(TmpTkD);
			}
			else
			{
				p1 = GetPoint(0,pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dScale, dOffsetX, dOffsetY);
				TmpTkD.iType = T_LINE;
				TmpTkD.dPos[0] = dCurx;
				TmpTkD.dPos[1] = dCury;
				TmpTkD.dPos[2] = p1.x;
				TmpTkD.dPos[3] = p1.y;
				dCurx = p1.x;
				dCury = p1.y;
				if (bFront)
				{
					m_FrontData.Add(TmpTkD);
					if (p1.x>m_dFRight)	m_dFRight	= p1.x;
					if (p1.x<m_dFLeft)		m_dFLeft	= p1.x;
					if (p1.y>m_dFBottom)	m_dFBottom	= p1.y;
					if (p1.y<m_dFTop)		m_dFTop		= p1.y;
				}
				else
				{
					m_BackData.Add(TmpTkD);
					if (p1.x>m_dBRight)	m_dBRight	= p1.x;
					if (p1.x<m_dBLeft)		m_dBLeft	= p1.x;
					if (p1.y>m_dBBottom)	m_dBBottom	= p1.y;
					if (p1.y<m_dBTop)		m_dBTop		= p1.y;
				}
			}
			break;// 线段点胶中间点tgPos[1]
		case CMD_LINE_ARC:
			if(!g_pFrm->m_wndEditBar.GetCmdArcInfo(pPaintCmd, i, &tArc))
				return FALSE;
			p1 = GetPoint(0,tArc.dx[0], tArc.dy[0], dScale, dOffsetX, dOffsetY);
			p2 = GetPoint(0,tArc.x, tArc.y, dScale, dOffsetX, dOffsetY);//圆心
			p3 = GetPoint(0,tArc.dx[2], tArc.dy[2], dScale, dOffsetX, dOffsetY);//当前点
			dTemp = tArc.r * dScale;
			if(tArc.angle<0)
			{
				TmpTkD.iType = T_DCIRCLE;
				TmpTkD.dPos[0] = p1.x;
				TmpTkD.dPos[1] = p1.y;
				TmpTkD.dPos[2] = p3.x;
				TmpTkD.dPos[3] = p3.y;
				TmpTkD.dPos[4] = p2.x;
				TmpTkD.dPos[5] = p2.y;
				TmpTkD.dPos[6] = dTemp;
				dCurx = p3.x;
				dCury = p3.y;
				m_FrontData.Add(TmpTkD);
			}
			else
			{
				TmpTkD.iType = T_WCIRCLE;
				TmpTkD.dPos[0] = p1.x;
				TmpTkD.dPos[1] = p1.y;
				TmpTkD.dPos[2] = p3.x;
				TmpTkD.dPos[3] = p3.y;
				TmpTkD.dPos[4] = p2.x;
				TmpTkD.dPos[5] = p2.y;
				TmpTkD.dPos[6] = dTemp;
				dCurx = p3.x;
				dCury = p3.y;
				m_FrontData.Add(TmpTkD);
			}
			//pDC->MoveTo(p3);
			break;//圆弧点胶 tgPos[1]
		case CMD_CIRCLE:
			if(pPaintCmd.pIntValue[4]==0)//圆心＋直径
			{
				p1 = GetPoint(0,pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dScale, dOffsetX, dOffsetY);
				dTemp = pPaintCmd.pDblValue[4]*dScale*0.5;
				if(dTemp<1)dTemp = 1;
				TPOINT tP1,tP2;
				bool bDr = 0;
				tP1.x = p1.x;
				tP1.y = p1.y;
				GetCircleInfo(tP1,dTemp,&tP2,&bDr);
				TmpTkD.iType = T_DCIRCLE + bDr;
				TmpTkD.dPos[0] = tP2.x;
				TmpTkD.dPos[1] = tP2.y;
				TmpTkD.dPos[2] = tP2.x;
				TmpTkD.dPos[3] = tP2.y;
				TmpTkD.dPos[4] = tP1.x;
				TmpTkD.dPos[5] = tP1.y;
				TmpTkD.dPos[6] = dTemp;
				dCurx = TmpTkD.dPos[2];
				dCury = TmpTkD.dPos[3];
				m_FrontData.Add(TmpTkD);
			}
			else//三点
			{
				TPOINT dTmpPos[3];
				CDlgPathEdit::GetCircle(&pPaintCmd, &dXc, &dYc, &dRc);
				p1 = GetPoint(0,dXc, dYc, dScale, dOffsetX, dOffsetY);
				dTemp = (dRc *dScale);
				if(dTemp<1) dTemp=1;
				TPOINT tP1,tP2;
				bool bDr = 0;
				tP1.x = p1.x;
				tP1.y = p1.y;
				GetCircleInfo(tP1,dTemp,&tP2,&bDr);
				TmpTkD.iType = T_DCIRCLE + bDr;
				TmpTkD.dPos[0] = tP2.x;
				TmpTkD.dPos[1] = tP2.y;
				TmpTkD.dPos[2] = tP2.x;
				TmpTkD.dPos[3] = tP2.y;
				TmpTkD.dPos[4] = tP1.x;
				TmpTkD.dPos[5] = tP1.y;
				TmpTkD.dPos[6] = dTemp;
				dCurx = TmpTkD.dPos[2];
				dCury = TmpTkD.dPos[3];
				m_FrontData.Add(TmpTkD);
			}
			break;//圆形点胶 1:break;圆心+直径, tgPos[1], double diameter, 2:break;3点画圆,tgPos[3]

		case CMD_LINE_END:
		case CMD_SUPEREND:
			if(g_pFrm->m_wndEditBar.GetEndPos(&pPaintCmd, i, m_cmdParam.tLine.dTailCloseLen, &x, &y))
			{
				p1 = GetPoint(0,x, y, dScale, dOffsetX, dOffsetY);
				//DrawCircle(pDC, p1, 3,FALSE);
			}
			p1 = GetPoint(0,pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dScale, dOffsetX, dOffsetY);
			//pDC->LineTo(p1);
			TmpTkD.iType = T_LINE;
			TmpTkD.dPos[0] = dCurx;
			TmpTkD.dPos[1] = dCury;
			TmpTkD.dPos[2] = p1.x;
			TmpTkD.dPos[3] = p1.y;
			dCurx = p1.x;
			dCury = p1.y;
			if (bFront)
			{
				m_FrontData.Add(TmpTkD);
				if (p1.x>m_dFRight)	m_dFRight	= p1.x;
				if (p1.x<m_dFLeft)		m_dFLeft	= p1.x;
				if (p1.y>m_dFBottom)	m_dFBottom	= p1.y;
				if (p1.y<m_dFTop)		m_dFTop		= p1.y;
			}
			else
			{
				m_BackData.Add(TmpTkD);
				if (p1.x>m_dBRight)	m_dBRight	= p1.x;
				if (p1.x<m_dBLeft)		m_dBLeft	= p1.x;
				if (p1.y>m_dBBottom)	m_dBBottom	= p1.y;
				if (p1.y<m_dBTop)		m_dBTop		= p1.y;
			}
			break;//线段点胶结束点,tgPos[1]
		case CMD_DISPENSER:
			break;//点胶阀开/关 BOOL On/Off
		case CMD_GOTO_ADDRESS:
			break;//跳转指令 int iIndex
		case CMD_REPEATX://X方向阵列 int iIndex:break; int stepType(S/N):break; int iXNum:break; int iYNum:break;  double xSpace:break; double ySpace
			{
				double dx, dy;
				dx = (pPaintCmd.pDblValue[4]-pPaintCmd.pDblValue[0]) / (pPaintCmd.pIntValue[2]-1);
				dy = (pPaintCmd.pDblValue[9]-pPaintCmd.pDblValue[5]) / (pPaintCmd.pIntValue[3]-1);
				for(m=0; m<pPaintCmd.pIntValue[3]; m++)//y
				{
					for(n=0; n<pPaintCmd.pIntValue[2]; n++)//x
					{
						if(m==0 && n==0)continue;
						if(pPaintCmd.pIntValue[1] ==0)//S
						{
							if((m%2) == 0) nx = n;
							else nx = pPaintCmd.pIntValue[2] -1 - n;
						}
						else
						{
							nx = n;
						}
						ny = m;

						//LoadTrack(pPaintCmd.pIntValue[0], i, dScale,pPaintCmd.pDblValue[0]*nx+dOffsetX, pPaintCmd.pDblValue[1]*ny+dOffsetY);
						LoadTrack(pPaintCmd.pIntValue[0], i, dScale,dx*nx+dOffsetX, dy*ny+dOffsetY);
					}
				}
				break;
			}
		case CMD_REPEATY://Y方向阵列 int iIndex:break; int stepType(S/N):break; int iXNum:break; int iYNum:break;  double xSpace:break; double ySpace
			{
				double dx, dy;
				dx = (pPaintCmd.pDblValue[4]-pPaintCmd.pDblValue[0]) / (pPaintCmd.pIntValue[2]-1);
				dy = (pPaintCmd.pDblValue[9]-pPaintCmd.pDblValue[5]) / (pPaintCmd.pIntValue[3]-1);

				for(m=0; m<pPaintCmd.pIntValue[2]; m++)//x
				{
					for(n=0; n<pPaintCmd.pIntValue[3]; n++)//y
					{
						if(m==0 && n==0)continue;
						nx = m;
						if(pPaintCmd.pIntValue[1] ==0)//S
						{
							if((m%2) == 0) ny = n;
							else ny = pPaintCmd.pIntValue[3] -1 - n;
						}
						else
						{
							ny = n;
						}

						//LoadTrack( pPaintCmd.pIntValue[0], i, dScale,pPaintCmd.pDblValue[0]*nx+dOffsetX, pPaintCmd.pDblValue[1]*ny+dOffsetY);
						LoadTrack( pPaintCmd.pIntValue[0], i, dScale,dx*nx+dOffsetX, dy*ny+dOffsetY);
					}
				}
				break;
			}
		case CMD_BRUSH_AREA:			
			if(pPaintCmd.pIntValue[5] == 0)//矩形
			{
				vector <tgBrushEx> brushRunList;
				tgBrushEx brushLine;				
				tgBrush tArea;
				bool bSet = FALSE;

				tArea.nDir = pPaintCmd.pIntValue[4]==0?0:1;//0--X方向涂覆  1--Y方向涂覆

				if(g_pFrm->m_wndEditBar.GetCmdBrushInfo(pPaintCmd,&tArea)==FALSE) break;

				brushLine.x=pPaintCmd.pDblValue[0];
				brushLine.y=pPaintCmd.pDblValue[1];
				brushLine.tType=CMD_LINE_START;
				brushRunList.push_back(brushLine);
	
				for (int i=0;i<(2*tArea.nNum+1);i++)
				{//加入圆弧倒角功能
					////step1
					brushLine.x = pPaintCmd.pDblValue[0] + (tArea.nDir==0?0:1)*((i/2)%2==0?1:0)*tArea.dx + (tArea.nDir==0?1:0)*((i/2)*tArea.dx +((i%2)==0?0:1)*tArea.dx);
					brushLine.y = pPaintCmd.pDblValue[1] + (tArea.nDir==0?1:0)*((i/2)%2==0?1:0)*tArea.dy + (tArea.nDir==0?0:1)*((i/2)*tArea.dy +((i%2)==0?0:1)*tArea.dy);

					bSet = (i%2)==0?TRUE:FALSE;//偶数点位置还判断是否停止压人指令
					if(bSet==TRUE)
					{
						if (tArea.nDir==0)
						{//tArea.nDir==0 X方向涂覆						
							if(fabs(pPaintCmd.pDblValue[4] - brushLine.x)<fabs(tArea.dx))
							{
								brushLine.tType=CMD_LINE_END;
								brushRunList.push_back(brushLine);
								break;
							}
						}
						else
						{//tArea.nDir==1 Y方向涂覆
							if(fabs(pPaintCmd.pDblValue[5] - brushLine.y)<fabs(tArea.dy))
							{
								brushLine.tType=CMD_LINE_END;
								brushRunList.push_back(brushLine);
								break;
							}
						}
					}
					brushLine.tType=CMD_LINE_PASSING;
					brushRunList.push_back(brushLine);
				}
				////////////////////////////////////////////////////////////////////////
				for(UINT i=0;i<brushRunList.size();i++)
				{
					if(brushRunList[i].tType==CMD_LINE_START)
					{
						p1 = GetPoint(0,brushRunList[i].x,brushRunList[i].y, dScale, dOffsetX, dOffsetY);
						dCurx = p1.x;
						dCury = p1.y;
					}
					else if(brushRunList[i].tType==CMD_LINE_PASSING)
					{	
						tgBrushEx brush[3];
						brush[0]=brushRunList[i-1];
						brush[1]=brushRunList[i];
						brush[2]=brushRunList[i+1];

						tgCmdLine tPreDotEx ,tCurDoxEx,tNextDotEx;
						tPreDotEx.pDblValue[0] = brush[0].x;
						tPreDotEx.pDblValue[1] = brush[0].y;
						tCurDoxEx.pDblValue[0] = brush[1].x;
						tCurDoxEx.pDblValue[1] = brush[1].y;
						tNextDotEx.pDblValue[0] = brush[2].x;
						tNextDotEx.pDblValue[1] = brush[2].y;

						if(pPaintCmd.pIntValue[6]==2)//插入圆弧
						{
							double m_CirX=0,m_CirY=0;
							BOOL dir=FALSE;
							BOOL  bRet=g_pFrm->m_wndEditBar.TrackArcHandle(tPreDotEx,tCurDoxEx,tNextDotEx,pPaintCmd.pDblValue[9],m_CirX,m_CirY,dir);
							if (bRet==TRUE)
							{
								p1 = GetPoint(0,tPreDotEx.pDblValue[0], tPreDotEx.pDblValue[1], dScale, dOffsetX, dOffsetY);
								p2 = GetPoint(0,m_CirX, m_CirY, dScale, dOffsetX, dOffsetY);//圆心
								p3 = GetPoint(0,tNextDotEx.pDblValue[0], tNextDotEx.pDblValue[1], dScale, dOffsetX, dOffsetY);//圆心
								dTemp = pPaintCmd.pDblValue[9]* dScale;
								TmpTkD.iType = T_LINE;
								TmpTkD.dPos[0] = dCurx;
								TmpTkD.dPos[1] = dCury;
								TmpTkD.dPos[2] = p1.x;
								TmpTkD.dPos[3] = p1.y;
								dCurx = p1.x;
								dCury = p1.y;
								m_FrontData.Add(TmpTkD);
								if(dir==TRUE)//逆时针
								{
									TmpTkD.iType = T_DCIRCLE/*T_WCIRCLE*/;
									TmpTkD.dPos[0] = p1.x;
									TmpTkD.dPos[1] = p1.y;
									TmpTkD.dPos[2] = p3.x;
									TmpTkD.dPos[3] = p3.y;
									TmpTkD.dPos[4] = p2.x;
									TmpTkD.dPos[5] = p2.y;
									TmpTkD.dPos[6] = dTemp;
									dCurx = p3.x;
									dCury = p3.y;
									m_FrontData.Add(TmpTkD);
								}
								else
								{
									TmpTkD.iType = T_WCIRCLE;
									TmpTkD.dPos[0] = p1.x;
									TmpTkD.dPos[1] = p1.y;
									TmpTkD.dPos[2] = p3.x;
									TmpTkD.dPos[3] = p3.y;
									TmpTkD.dPos[4] = p2.x;
									TmpTkD.dPos[5] = p2.y;
									TmpTkD.dPos[6] = dTemp;
									dCurx = p3.x;
									dCury = p3.y;
									m_FrontData.Add(TmpTkD);
								}
							}
							else
							{
								p1 = GetPoint(0,tCurDoxEx.pDblValue[0],tCurDoxEx.pDblValue[1], dScale, dOffsetX, dOffsetY);
								TmpTkD.iType = T_LINE;
								TmpTkD.dPos[0] = dCurx;
								TmpTkD.dPos[1] = dCury;
								TmpTkD.dPos[2] = p1.x;
								TmpTkD.dPos[3] = p1.y;
								dCurx = p1.x;
								dCury = p1.y;
								m_FrontData.Add(TmpTkD);
							}
						}						
						else
						{
							p1 = GetPoint(0,tCurDoxEx.pDblValue[0],tCurDoxEx.pDblValue[1], dScale, dOffsetX, dOffsetY);
							TmpTkD.iType = T_LINE;
							TmpTkD.dPos[0] = dCurx;
							TmpTkD.dPos[1] = dCury;
							TmpTkD.dPos[2] = p1.x;
							TmpTkD.dPos[3] = p1.y;
							dCurx = p1.x;
							dCury = p1.y;
							m_FrontData.Add(TmpTkD);
						}
					}
					else if(brushRunList[i].tType==CMD_LINE_END)
					{
						p1 = GetPoint(0,brushRunList[i].x,brushRunList[i].y, dScale, dOffsetX, dOffsetY);
						TmpTkD.iType = T_LINE;
						TmpTkD.dPos[0] = dCurx;
						TmpTkD.dPos[1] = dCury;
						TmpTkD.dPos[2] = p1.x;
						TmpTkD.dPos[3] = p1.y;
						dCurx = p1.x;
						dCury = p1.y;
						m_FrontData.Add(TmpTkD);
					}
				}
			}
			else//圆形
			{
				tgBrush tArea;

				if(g_pFrm->m_wndEditBar.GetCmdBrushInfo(pPaintCmd,&tArea)==FALSE) break;

				cux=tArea.x;
				cuy=tArea.y;
				double dRadius=tArea.dRadius;

				//////////////////////////////////////////////////////////////////////////
				double tx = 0,ty = 0;
				while(tArea.dAngle <0)  tArea.dAngle+=360;
				CString tmpstr;
				while(dRadius>tArea.dStep)
				{
					tArea.dAngle += 9;//分成40等份
					if(tArea.dAngle>=360) tArea.dAngle -= 360;

					if(dRadius>(tArea.dStep/40))
						dRadius = dRadius - (tArea.dStep/40);

					tArea.dfCx[1] = dRadius * cos(tArea.dAngle*M_PI/180) + tArea.dfCx[0];//下一圆弧点
					tArea.dfCy[1] = dRadius * sin(tArea.dAngle*M_PI/180) + tArea.dfCy[0];//下一圆弧点
					TPOINT sp,ep,tp1,tp2;
					TPOINT tmp1,tmp2;
					CPoint dop,dsp,dep;
					sp.x = cux;
					sp.y = cuy;
					ep.x = tArea.dfCx[1];
					ep.y = tArea.dfCy[1];
					if(1 == m_Geo.InsCC(sp,dRadius,ep,dRadius))
					{
						m_Geo.Intersection_circle_circle(sp,dRadius,ep,dRadius,tp1,tp2);
						dsp = GetPoint(sp.x, sp.y, dScale, dOffsetX, dOffsetY, TRUE);
						dep = GetPoint(ep.x, ep.y, dScale, dOffsetX, dOffsetY, TRUE);
						if(m_Geo.GetDrlinedot(sp,ep,tp1) == 2)
						{
							dop = GetPoint(tp1.x, tp1.y, dScale, dOffsetX, dOffsetY, TRUE);	
							TmpTkD.iType = T_WCIRCLE;
							TmpTkD.dPos[0] = sp.x;
							TmpTkD.dPos[1] = sp.y;
							TmpTkD.dPos[2] = ep.x;
							TmpTkD.dPos[3] = ep.y;
							TmpTkD.dPos[4] = tp1.x;
							TmpTkD.dPos[5] = tp1.y;
							TmpTkD.dPos[6] = dRadius;
							m_FrontData.Add(TmpTkD);
						}
						else if(m_Geo.GetDrlinedot(sp,ep,tp2) == 2)
						{
							dop = GetPoint(tp2.x, tp2.y, dScale, dOffsetX, dOffsetY, TRUE);
							TmpTkD.iType = T_WCIRCLE;
							TmpTkD.dPos[0] = sp.x;
							TmpTkD.dPos[1] = sp.y;
							TmpTkD.dPos[2] = ep.x;
							TmpTkD.dPos[3] = ep.y;
							TmpTkD.dPos[4] = tp2.x;
							TmpTkD.dPos[5] = tp2.y;
							TmpTkD.dPos[6] = dRadius;
							m_FrontData.Add(TmpTkD);
						}
						else
						{
							ASSERT(0);
						}
					}
					tx = tArea.dfCx[1];
					ty = tArea.dfCy[1];
					//if(j < tArea.nNum)
					{
						cux=tArea.dfCx[1];//ep.x;
						cuy=tArea.dfCy[1];//ep.y;
					}
				}
				//////////////////////////////////////////////////////////////////////////
				p1 = GetPoint(0,tArea.dfCx[0], tArea.dfCy[0], dScale, dOffsetX, dOffsetY);
				p3 = GetPoint(0,tArea.dfCx[1], tArea.dfCy[1], dScale, dOffsetX, dOffsetY);
				dCurx = p3.x;
				dCury = p3.y;
				TmpTkD.iType = T_LINE;
				TmpTkD.dPos[0] = dCurx;
				TmpTkD.dPos[1] = dCury;
				TmpTkD.dPos[2] = p1.x;
				TmpTkD.dPos[3] = p1.y;
				dCurx = p1.x;
				dCury = p1.y;
				m_FrontData.Add(TmpTkD);
			}
			break;
		case CMD_CALL_SUB:
			break;
		case CMD_LABEL:
			break;
		case CMD_NULL_LINE:
			break;
		case CMD_DELAY:
			break;
		case CMD_PAUSE:
			break;
		case CMD_HOME:
			break;
		case CMD_LOOP:
			break;
		case CMD_DUMMY:
			break;
		case CMD_NOTE:
			break;
		case CMD_END_PROGRAM:
			return FALSE;
			break;//程序结束命令
		default:
			NULL;
		}
	}
	return TRUE;
}

CPoint CRedrawTrack::GetPoint(double dX, double dY, double dScale, double dOffsetX, double dOffsetY, bool bTrack)
{
	CPoint dot;
	dot.x = static_cast<long>((dX-dOffsetX) * dScale);
	dot.y = static_cast<long>((dY-dOffsetY) * dScale);
	if(bTrack)
	{
// 		dot.x += m_tTrackDrawOffset.x;
// 		dot.y += m_tTrackDrawOffset.y;
	}

	return dot;
}

TPOINT CRedrawTrack::GetPoint(int pType, double dX, double dY, double dScale, double dOffsetX, double dOffsetY)
{
	UNREFERENCED_PARAMETER(pType);
	TPOINT dot;
	dot.x = (dX+dOffsetX)*dScale;
	dot.y = (dY+dOffsetY)*dScale;

	return dot;
}

void CRedrawTrack::SetOffset(int dx,int dy)
{
	m_tTrackDrawOffset.x = dx;
	m_tTrackDrawOffset.y = dy;
}

bool CRedrawTrack::GetCircleInfo(TPOINT op,double r,TPOINT *sp,bool *dir)
{
	sp->x = op.x - sin(PI/4)*r;
	sp->y = op.y - cos(PI/4)*r;
	*dir = 1;
	return TRUE;
}

int CRedrawTrack::P3ToSEO(TPOINT *p,int iNum)
{
	if(iNum < 3) return -4;
	double px[3],py[3];
	double dx,dy,r;
	double a[5];
	TPOINT dTmp;
	for (int i=0;i<3;i++)
	{
		px[i]=p[i].x;
		py[i]=p[i].y;
	}
	g_pFrm->m_wndEditBar.GetCircle(px,py,&dx,&dy,&r);
	for (int i=0;i<3;i++)
	{
		dTmp.x =p[i].x - dx;
		dTmp.y =p[i].y - dy;
		m_Geo.Getagforpoint(dTmp,&a[i]);
	}
	p[1].x = p[2].x;
	p[1].y = p[2].y;
	p[2].x = dx;
	p[2].y = dy;
	a[3] = a[1]-a[0];
	a[4] = a[2]-a[0];
	while(a[3]<0 || a[4]<0 || a[3]>360 || a[4]>360)
	{
		a[3] = a[3]<0?(a[3]+360):a[3];
		a[4] = a[4]<0?(a[4]+360):a[4];
		a[3] = a[3]>360?(a[3]-360):a[3];
		a[4] = a[4]>360?(a[4]-360):a[4];
	}
	if(a[3]<a[4])
	{
		return T_WCIRCLE;
	}
	else
		return T_DCIRCLE;
}

bool CRedrawTrack::Start()
{
	if(m_Thread != NULL)
		return FALSE;
	//g_trackevent.ResetEvent();
	m_iCurLine = -1;
	m_bCurLineSt = false;
	m_bexit = false;
// 	m_Thread = AfxBeginThread(TrackThread,this);
// 	if (!m_Thread)
// 		return FALSE;
	return TRUE;
}

bool CRedrawTrack::Stop()
{
	if(!m_Thread) return FALSE;
	m_bexit = true;
	//g_trackevent.SetEvent();
	m_Thread->ResumeThread();
	m_Thread = NULL;
	return TRUE;
}

bool CRedrawTrack::Pause()
{
	m_Thread->SuspendThread();
	return TRUE;
}

bool CRedrawTrack::Restart()
{
	m_Thread->ResumeThread();
	return TRUE;
}

UINT CRedrawTrack::TrackThread(LPVOID pParam)
{
	CRedrawTrack *pTk = (CRedrawTrack*)pParam;
	TRACKINFO lInfo,rInfo,Info1,Info2,lold;
	TRACKDATA lKd;
	TRACKDATA rKd;
	int iLn = pTk->m_FrontData.GetCount();
	if(iLn>0)
	{
		lKd = pTk->m_FrontData.GetAt(0);
		lInfo.line = 0;
		lInfo.p.x = lKd.dPos[0];
		lInfo.p.y = lKd.dPos[1];
	}
	else return 1;

	lold = lInfo;

	pTk->m_OldLine = 0;
	pTk->g_exitdrawevent.ResetEvent();
	pTk->m_OldInfo = &lold;
	while(!pTk->m_bexit)//test
	{
		Info1 = pTk->GetTrackInfo(lInfo);
		if(fabs(lInfo.p.x - Info1.p.x)<pTk->m_InterDr
			&& fabs(lInfo.p.y - Info1.p.y) < pTk->m_InterDr)
		{
			NULL;
		}
		else
		{
			lold = Info1;
			lInfo = Info1;
		}
		Sleep(20);
	}
	pTk->m_OldInfo = NULL;
	pTk->m_Thread = NULL;
	pTk->g_exitdrawevent.SetEvent();
	return 0;
}

TRACKINFO CRedrawTrack::GetTrackInfo(TRACKINFO info)
{
	double kpos[2];
	kpos[0] = *m_dCurX;
	kpos[1] = *m_dCurY;
	TRACKINFO Tmpinfo = info;
	if(fabs(Tmpinfo.p.x-kpos[0]) < m_InterDr &&
		fabs(Tmpinfo.p.y-kpos[1]) < m_InterDr)
	{
		return Tmpinfo;
	}
	if(false == m_bValveOut)
	{
		return Tmpinfo;
	}
	if(m_bDataUpdate)
	{
		return Tmpinfo;
	}
	TRACKINFO tInfo;
	TRACKDATA tk;
	TPOINT	dp[4];
	int iLn = m_FrontData.GetCount();
	for(int i=m_OldLine;i<iLn;i++)
	{
		if(m_bDataUpdate)
		{
			return Tmpinfo;
		}
		tk = m_FrontData.GetAt(i);
		switch(tk.iType)
		{
		case T_MARK:
			break;
		case T_WAIT:
		case T_HOME:
		case T_POINT:
			{
				if(fabs(kpos[0] - tk.dPos[0])<m_InterDr &&
					fabs(kpos[1] - tk.dPos[1])<m_InterDr)
				{
					m_OldLine = i;
					dp[0].x = tk.dPos[0];
					dp[0].y = tk.dPos[1];
					tInfo.line = i;
					tInfo.p = dp[0];
					return tInfo;
				}
			}
			break;
		case T_LINE:
			{
				dp[0].x = kpos[0];
				dp[0].y = kpos[1];
				dp[1].x = tk.dPos[0];
				dp[1].y = tk.dPos[1];
				dp[2].x = tk.dPos[2];
				dp[2].y = tk.dPos[3];
				if(m_Geo.IsInsLC(dp[0],m_InterDr,dp[1],dp[2]))
				{
					if(Tmpinfo.line == i && m_Geo.Distance(dp[1],dp[0])<m_Geo.Distance(dp[1],Tmpinfo.p))
						return Tmpinfo;	//为了解决线段回画

					if(m_Geo.Distance(dp[0],dp[1]) < 1)
						return Tmpinfo;

					m_OldLine = i;
					tInfo.line = i;
					tInfo.p = dp[0];
					m_dCurLinePos = dp[0];
					return tInfo;
				}
			}
			break;
		case T_DCIRCLE:
			{
				//SEOR
				double angle[6];
				dp[1].x = kpos[0];
				dp[1].y = kpos[1];
				dp[2].x = tk.dPos[4];
				dp[2].y = tk.dPos[5];
				dp[0].x = dp[1].x - dp[2].x;
				dp[0].y = dp[1].y - dp[2].y;
				int co = m_Geo.InsCC(dp[1],m_InterDr,dp[2],tk.dPos[6]);
				if(co)
				{
					m_Geo.Getagforpoint(dp[0],&angle[1]);//当前角
					dp[0].x = tk.dPos[2] - dp[2].x;
					dp[0].y = tk.dPos[3] - dp[2].y;
					m_Geo.Getagforpoint(dp[0],&angle[2]);//DC:目标角
					dp[0].x = tk.dPos[0] - dp[2].x;
					dp[0].y = tk.dPos[1] - dp[2].y;
					m_Geo.Getagforpoint(dp[0],&angle[3]);//DC:起始角
					angle[4] = angle[2] - angle[3];
					angle[5] = angle[1] - angle[3];
					while(angle[4] <= 0) angle[4] += 360;
					while(angle[4] > 360) angle[4] -= 360;
					while(angle[5] <= 0) angle[5] += 360;
					while(angle[5] > 360) angle[5] -= 360;
					if(angle[4] < angle[5])
					{
						tInfo.line = i;
						tInfo.p = dp[1];
						m_AData.line = i;
						m_AData.angle = angle[2];
						m_OldLine = i;
						return tInfo;
					}
					else if(i<iLn)
					{
						continue;
					}
					else
						return Tmpinfo;
				}
			}
			break;
		case T_WCIRCLE:
			{
				//SEOR
				double angle[6];
				dp[1].x = kpos[0];
				dp[1].y = kpos[1];
				dp[2].x = tk.dPos[4];
				dp[2].y = tk.dPos[5];
				dp[0].x = dp[1].x - dp[2].x;
				dp[0].y = dp[1].y - dp[2].y;
				int co = m_Geo.InsCC(dp[1],m_InterDr,dp[2],tk.dPos[6]);
				if(co)
				{
					m_Geo.Getagforpoint(dp[0],&angle[1]);//当前角
					dp[0].x = tk.dPos[2] - dp[2].x;
					dp[0].y = tk.dPos[3] - dp[2].y;
					m_Geo.Getagforpoint(dp[0],&angle[2]);//DC:目标角
					dp[0].x = tk.dPos[0] - dp[2].x;
					dp[0].y = tk.dPos[1] - dp[2].y;
					m_Geo.Getagforpoint(dp[0],&angle[3]);//DC:起始角
					angle[4] = angle[2] - angle[3];
					angle[5] = angle[1] - angle[3];
					while(angle[4] <= 0) angle[4] += 360;
					while(angle[4] > 360) angle[4] -= 360;
					while(angle[5] <= 0) angle[5] += 360;
					while(angle[5] > 360) angle[5] -= 360;
					if(angle[4] > angle[5])
					{
						tInfo.line = i;
						tInfo.p = dp[1];
						m_AData.line = i;
						m_AData.angle = angle[2];
						m_OldLine = i;
						return tInfo;
					}
					else if(i<iLn)
					{
						continue;
					}
					else
						return Tmpinfo;
				}
			}
			break;
		default:
			break;
		}
	}
	return Tmpinfo;
}

void CRedrawTrack::Paint(CDC *pDC, CRect rect, BOOL bFront, double dOffsetX/* =0 */, double dOffsetY/* =0 */)
{
	if(NULL == pDC)
		return;
	if(m_bDataUpdate)
		return;
	TRACKDATA tCurPt;

	if (bFront)
	{
		CBrush brush(RGB(192, 255, 192));
		pDC->FillRect(rect, &brush);
	}
	else
	{
		CBrush brush(RGB(255, 255, 192));
		pDC->FillRect(rect, &brush);
	}

	double	dPt[7];
	TPOINT dSt;
	CPoint p1,p2,p3;
	int iTemp;
	int iLn = 0;

	double dStartX, dStartY; // 绘图的左上角 对应的机械坐标
	double dScale = 1;
	double dTrackWidth, dTrackHeight;
	if (bFront) // 正面轨迹数据
	{
		dTrackWidth = abs(m_dFRight - m_dFLeft);
		dTrackHeight = abs(m_dFBottom - m_dFTop);
		dTrackHeight = max(1, dTrackHeight);
		dTrackWidth = max(1, dTrackWidth);
		dTrackWidth *= 1.2;
		dTrackHeight *= 1.2;
		double dscalex = rect.Width() / dTrackWidth;
		double dscaley = rect.Height() / dTrackHeight;
		dScale = min(dscalex, dscaley);
		if (dScale<EPS)
		{
			return;
		}

		dStartX = (m_dFLeft+m_dFRight)/2 - rect.Width()/2/dScale;
		dStartY = (m_dFTop+m_dFBottom)/2 - rect.Height()/2/dScale;

		iLn = m_FrontData.GetCount();
	}
	else	// 背面轨迹数据
	{
		dTrackWidth = abs(m_dBRight - m_dBLeft);
		dTrackHeight = abs(m_dBBottom - m_dBTop);
		dTrackHeight = max(1, dTrackHeight);
		dTrackWidth = max(1, dTrackWidth);
		dTrackWidth *= 1.2;
		dTrackHeight *= 1.2;
		double dscalex = rect.Width() / dTrackWidth;
		double dscaley = rect.Height() / dTrackHeight;
		dScale = min(dscalex, dscaley);
		if (dScale<EPS)
		{
			return;
		}

		dStartX = (m_dBLeft+m_dBRight)/2 - rect.Width()/2/dScale;
		dStartY = (m_dBTop+m_dBBottom)/2 - rect.Height()/2/dScale;

		iLn = m_BackData.GetCount();
	}

	
	for (int i=0;i<iLn/*iltmpinfo.line*/;i++)
	{
		if(m_bDataUpdate)
			return;
		tCurPt = bFront ? m_FrontData.GetAt(i):m_BackData.GetAt(i);
		if(i == iLn/*tmpinfo.line*/)
		{	//最后一段取末端点
// 			if(tCurPt.iType == T_POINT)
// 			{
// 				tCurPt.dPos[0] = tmpinfo.p.x;
// 				tCurPt.dPos[1] = tmpinfo.p.y;
// 			}
// 			else if(tCurPt.iType == T_LINE)
// 			{
// 				tCurPt.dPos[2] = tmpinfo.p.x;
// 				tCurPt.dPos[3] = tmpinfo.p.y;
// 			}
// 			else if(tCurPt.iType == T_DCIRCLE)
// 			{
// 				tCurPt.dPos[2] = tmpinfo.p.x;
// 				tCurPt.dPos[3] = tmpinfo.p.y;
// 				if(fabs(double(tCurPt.dPos[0] - tCurPt.dPos[2]))>0.01 || fabs(double(tCurPt.dPos[1] - tCurPt.dPos[3]))>0.05)
// 				{
// 					m_bCurLineSt = true;
// 					m_iCurLine = tmpinfo.line;
// 				}
// 				else if(m_iCurLine != tmpinfo.line)
// 				{
// 					m_bCurLineSt = false;
// 					break;
// 				}
// 			}
// 			else if(tCurPt.iType == T_WCIRCLE)
// 			{
// 				tCurPt.dPos[2] = tmpinfo.p.x;
// 				tCurPt.dPos[3] = tmpinfo.p.y;
// 				if(fabs(double(tCurPt.dPos[0] - tCurPt.dPos[2]))>0.05 || fabs(double(tCurPt.dPos[1] - tCurPt.dPos[3]))>0.05)
// 				{
// 					m_bCurLineSt = true;
// 					m_iCurLine = tmpinfo.line;
// 				}
// 				else if(m_iCurLine != tmpinfo.line)
// 				{
// 					m_bCurLineSt = false;
// 					break;
// 				}
// 			}
// 			else
// 			{
// 				break;
// 			}
		}
		for(int i=0;i<7;i++)
		{
			dPt[i] = tCurPt.dPos[i];
		}
		switch(tCurPt.iType)
		{
		case T_MARK:
			{
				p1 = GetPoint(dPt[0], dPt[1], dScale, dStartX, dStartY, TRUE);
				DrawCircleEx(pDC,COLOR(240,0,0),p1,5);
				DrawCross(pDC, COLOR(0, 0, 0), p1, 10);
				p1 = GetPoint(dPt[2], dPt[3], dScale, dStartX, dStartY, TRUE);
				DrawCircleEx(pDC,COLOR(240,0,0),p1,5);
				DrawCross(pDC, COLOR(0, 0, 0), p1, 10);
			}
			break;

		case T_POINT:
			{
				p1 = GetPoint(dPt[0], dPt[1], dScale, dStartX, dStartY, TRUE);
				DrawCircleEx(pDC,COLOR(240,0,0),p1,3);
			}
			break;
		case T_LINE:
			{
				p1 = GetPoint(dPt[0], dPt[1], dScale, dStartX, dStartY, TRUE);
				p2 = GetPoint(dPt[2], dPt[3], dScale, dStartX, dStartY, TRUE);
				pDC->MoveTo(p1);
				pDC->LineTo(p2);
			}
			break;
		case T_DCIRCLE:
			{
				p1 = GetPoint(dPt[0], dPt[1], dScale, dOffsetX, dOffsetY, TRUE);
				p2 = GetPoint(dPt[2], dPt[3], dScale, dOffsetX, dOffsetY, TRUE);
				p3 = GetPoint(dPt[4], dPt[5], dScale, dOffsetX, dOffsetY, TRUE);
				iTemp = static_cast<int>(dPt[6]* dScale);
				pDC->MoveTo(p1);
				pDC->Arc(CRect(p3.x - iTemp,p3.y - iTemp,p3.x + iTemp,p3.y + iTemp),p1,p2);
			}
			break;
		case T_WCIRCLE:
			{
				p1 = GetPoint(dPt[0], dPt[1], dScale, dOffsetX, dOffsetY, TRUE);
				p2 = GetPoint(dPt[2], dPt[3], dScale, dOffsetX, dOffsetY, TRUE);
				p3 = GetPoint(dPt[4], dPt[5], dScale, dOffsetX, dOffsetY, TRUE);
				iTemp = static_cast<int>(dPt[6]* dScale);
				pDC->MoveTo(p2);
				pDC->Arc(CRect(p3.x - iTemp,p3.y - iTemp,p3.x + iTemp,p3.y + iTemp),p2,p1);
			}
			break;
		default:
			break;
		}
	}
}

void CRedrawTrack::DrawCircleEx(CDC *pDC,COLOR clr, CPoint point, int r, bool bFull)
{
	CBrush brush(RGB(clr.R,clr.G,clr.B));
	CBrush *pOldBrush = pDC->SelectObject(&brush);
	if(!bFull)
	{
		CGdiObject *pOldGdi = pDC->SelectStockObject(NULL_BRUSH);
		pDC->Ellipse(point.x-r, point.y-r, point.x+r, point.y+r);
		pDC->SelectObject(pOldGdi);
	}
	else
	{
		pDC->Ellipse(point.x-r, point.y-r, point.x+r, point.y+r);
	}
	pDC->SelectObject(pOldBrush);
}

void CRedrawTrack::DrawCross(CDC *pDC,COLOR clr, CPoint point, int r)
{
	CPen pen(PS_SOLID, 1, RGB(clr.R, clr.G, clr.B));
	CPen *pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(point.x-r, point.y);
	pDC->LineTo(point.x+r, point.y);
	pDC->MoveTo(point.x, point.y-r);
	pDC->LineTo(point.x, point.y+r);
	
	pDC->SelectObject(pOldPen);
}

void CRedrawTrack::CollPos(double *px,double *py)
{
	m_dCurX = px;
	m_dCurY = py;
}

void CRedrawTrack::CollValve(bool *bFlag)
{
	m_bValveOut = *bFlag;
}

void CRedrawTrack::SetValve(bool bFlag)
{
	CString str;
	m_bValveOut = bFlag;
}

bool CRedrawTrack::FreeTrackData()
{
	while(m_bDataUpdate)
		Sleep(5);
	if(m_FrontData.GetCount()<0) return false;
	m_bDataUpdate = true; 
	m_FrontData.RemoveAll();
	m_bDataUpdate = false;
	return true;
}

bool CRedrawTrack::GetValveSt()
{
	return m_bValveOut;
}

TRACKDATA CRedrawTrack::GetCurLineData(int n)
{
	if(m_bDataUpdate)
	{
		TRACKDATA tmp;
		memset(&tmp,0,sizeof(tmp));
		tmp.iType = -1;
		return tmp;

	}
	else if(n<0 || n>= m_FrontData.GetCount())
	{
		TRACKDATA tmp;
		memset(&tmp,0,sizeof(tmp));
		tmp.iType = -1;
		return tmp;
	}
	return m_FrontData.GetAt(n);
}

int CRedrawTrack::GetSize()
{
	if(m_bDataUpdate) return 0;
	return m_FrontData.GetCount();
}
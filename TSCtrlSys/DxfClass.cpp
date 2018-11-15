#include "StdAfx.h"
#include "DxfClass.h"
#include "paramdef.h"
#include <math.h>

#define  DXF_PI			3.141592653589793
#define  DXF_2PI		6.283185307179586
#define  DXF_PI_HALF	1.5707963267948965

CDxfClass::CDxfClass(void)
{

}

CDxfClass::~CDxfClass(void)
{
}

BOOL CDxfClass::OpenDxfFile(CString strDxfFile, CString strCmdFile)
{
	CString strReadLine;	
	CStdioFile sdFile;
	BOOL bExit = FALSE;
	if(!sdFile.Open(strDxfFile, CFile::modeRead)
		|| !m_strCmdFile.Open(strCmdFile, CFile::modeCreate|CFile::modeWrite))
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	while(sdFile.ReadString(strReadLine) && !bExit)
	{
		//Min
		if(strReadLine == "$EXTMIN")
		{
			/*
			sdFile.ReadString(strReadLine);//10
			sdFile.ReadString(strReadLine);//x
			CDotType::m_dotmin.x = atof(strReadLine);

			sdFile.ReadString(strReadLine);//20
			sdFile.ReadString(strReadLine);//y
			CDotType::m_dotmin.y = atof(strReadLine);

			sdFile.ReadString(strReadLine);//30
			sdFile.ReadString(strReadLine);//z
			CDotType::m_dotmin.z = atof(strReadLine);
			*/
		}
		//Max
		else if(strReadLine == "$EXTMAX")
		{
			/*
			sdFile.ReadString(strReadLine);//10
			sdFile.ReadString(strReadLine);//x
			CDotType::m_dotmax.x = atof(strReadLine);

			sdFile.ReadString(strReadLine);//20
			sdFile.ReadString(strReadLine);//y
			CDotType::m_dotmax.y = atof(strReadLine);

			sdFile.ReadString(strReadLine);//30
			sdFile.ReadString(strReadLine);//z
			CDotType::m_dotmax.z = atof(strReadLine);
			*/
		}
		//Shape
		else if(strReadLine == "ENTITIES")
		{
			while(sdFile.ReadString(strReadLine))
			{
				if(strReadLine == "ENDSEC")
				{	
					bExit = TRUE;
					break;
				}
				else if(strReadLine == "LINE")
				{
					HandleLine(sdFile);
				}
				else if(strReadLine == "CIRCLE")
				{
					HandleCircle(sdFile);
				}
				else if(strReadLine == "ARC")
				{
					HandleArc(sdFile);
				}
				else if(strReadLine == "LWPOLYLINE")
				{
					HandlePlyLine(sdFile);
				}
				else if(strReadLine == "POINT")
				{
					HandlePoint(sdFile);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	sdFile.Close();
	m_strCmdFile.Close();
	return TRUE;

}

void CDxfClass::HandleLine(CStdioFile &sdFile)
{
	CString strReadLine, strText;
	tgPos dot;
	do
	{
		sdFile.ReadString(strReadLine);
		strReadLine.TrimLeft();
		strReadLine.TrimRight();
	}while(strReadLine != "10");

	//sdFile.ReadString(strReadLine);//10
	sdFile.ReadString(strReadLine);//xz
	dot.x = atof(strReadLine);

	sdFile.ReadString(strReadLine);//20
	sdFile.ReadString(strReadLine);//y1
	dot.y = atof(strReadLine);

	sdFile.ReadString(strReadLine);//30
	sdFile.ReadString(strReadLine);//z1
	dot.z = atof(strReadLine);
	
	strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n", CMD_LINE_START, dot.x, dot.y, dot.z, dot.u);
	m_strCmdFile.WriteString(strText);
	
	//
	sdFile.ReadString(strReadLine);//11
	sdFile.ReadString(strReadLine);//x
	dot.x = atof(strReadLine);

	sdFile.ReadString(strReadLine);//21
	sdFile.ReadString(strReadLine);//y2
	dot.y = atof(strReadLine);

	sdFile.ReadString(strReadLine);//31
	sdFile.ReadString(strReadLine);//z2
	dot.z = atof(strReadLine);

	strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n", CMD_LINE_END, dot.x, dot.y, dot.z, dot.u);
	m_strCmdFile.WriteString(strText);
}

void CDxfClass::HandlePoint(CStdioFile &sdFile)
{
	CString strReadLine, strText;
	tgPos dot;
	do
	{
		sdFile.ReadString(strReadLine);
		strReadLine.TrimLeft();
		strReadLine.TrimRight();
	}while(strReadLine != "10");

	//sdFile.ReadString(strReadLine);//10
	sdFile.ReadString(strReadLine);//x
	dot.x = atof(strReadLine);

	sdFile.ReadString(strReadLine);//20
	sdFile.ReadString(strReadLine);//y1
	dot.y = atof(strReadLine);

	sdFile.ReadString(strReadLine);//30
	sdFile.ReadString(strReadLine);//z1
	dot.z = atof(strReadLine);

	strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n", CMD_POINT, dot.x, dot.y, dot.z, dot.u);
	m_strCmdFile.WriteString(strText);;
}

void CDxfClass::HandlePlyLine(CStdioFile &sdFile)
{
	CString strReadLine, strText;
	tgPos dot, predot, startdot, dotc, enddot, dotmiddle;

	//////////////////////////////////////////////////////////////////////////
	int idotCount =0, i=0;
	BOOL bLoop=0;

	do {
		sdFile.ReadString(strReadLine);
		strReadLine.TrimLeft();
		strReadLine.TrimRight();
		if(strReadLine == "90")//节点数
		{
			sdFile.ReadString(strReadLine);
			idotCount = atoi(strReadLine);
		}
		else if(strReadLine == "70")//是否封闭
		{
			sdFile.ReadString(strReadLine);
			bLoop = atoi(strReadLine);
		}
		else if(strReadLine == "42")//圆弧
		{
			/*			
			弧长计算公式为：L=πRθ，
			弦长计算公式为: b=2Rsin(θ/2)，
			式中: L为弧长，b为弦长，
			R为圆弧半径，θ为该圆弧的圆心角，按弧度计。

			若知圆弧端点坐标 x1,y1,x2,y2
			弦长 = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
			*/
			double dBulk = 0;
			sdFile.ReadString(strReadLine);
			dBulk = atof(strReadLine);//弧段弓弦比 = 弓高/(弦长/2)

			sdFile.ReadString(strReadLine);//10 //终点
			sdFile.ReadString(strReadLine);//x
			enddot.x = atof(strReadLine);

			sdFile.ReadString(strReadLine);//20
			sdFile.ReadString(strReadLine);//y
			enddot.y = atof(strReadLine);

			//终点->起点角度
			double dAngle=atan2(enddot.y-predot.y, enddot.x-predot.x);
			
			//弦中心点
			dotmiddle.x = (predot.x+enddot.x)/2;
			dotmiddle.y = (predot.y+enddot.y)/2;

			//1/2弦长
			double dBow = sqrt((enddot.y-predot.y)*(enddot.y-predot.y) + (enddot.x-predot.x)*(enddot.x-predot.x))/2;
			double dBowAngle = fabs(atan(dBulk));

			//1/2圆心角
			double halfAngle=fabs(2.0*dBowAngle);
			
			//1/2圆心角弦长
			double dBowHalfAngle = dBow/cos(dBowAngle);

			//半径
			double dRadius=dBow / sin(halfAngle);

			//弦高
			double dBowHigh = dBowHalfAngle  * sin(dBowAngle);
			//
			if(dBulk>0)//逆圆弧
			{
				//圆弧中心点
// 				dot.x = predot.x + dBowHalfAngle *cos(dAngle - dBowAngle);
// 				dot.y = predot.y + dBowHalfAngle *sin(dAngle - dBowAngle);
				dot.x = dotmiddle.x + dBowHigh * cos(dAngle -dBowAngle);
				dot.y = dotmiddle.y + dBowHigh * sin(dAngle -dBowAngle);

				//圆心点
// 				dotc.x = predot.x + dRadius*cos(DXF_PI_HALF-halfAngle+dAngle);
// 				dotc.y = predot.y + dRadius*sin(DXF_PI_HALF-halfAngle+dAngle);
				dotc.x = dotmiddle.x + (dRadius-dBowHigh) * cos(dAngle +(DXF_PI_HALF-dBowAngle));
				dotc.y = dotmiddle.y + (dRadius-dBowHigh) * sin(dAngle +(DXF_PI_HALF-dBowAngle));

			}
			else//顺圆弧
			{
// 				dot.x = predot.x + dBowHalfAngle *cos(dAngle + dBowAngle);
// 				dot.y = predot.y + dBowHalfAngle *sin(dAngle + dBowAngle);
				dot.x = dotmiddle.x + dBowHigh * cos(dAngle +dBowAngle);
				dot.y = dotmiddle.y + dBowHigh * sin(dAngle +dBowAngle);

// 				dotc.x = predot.x + dRadius*cos(DXF_PI_HALF-halfAngle-dAngle);
// 				dotc.y = predot.y - dRadius*sin(DXF_PI_HALF-halfAngle-dAngle);
				dotc.x = dotmiddle.x + (dRadius-dBowHigh) * cos(dAngle -(DXF_PI_HALF-dBowAngle));
				dotc.y = dotmiddle.y + (dRadius-dBowHigh) * sin(dAngle -(DXF_PI_HALF-dBowAngle));
			}

			//
			strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n", CMD_LINE_ARC, dot.x, dot.y, dot.z, dot.u);
			m_strCmdFile.WriteString(strText);

// 			strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n", CMD_POINT_DISPENSE, dotc.x, dotc.y, dotc.z, dotc.u);
// 			m_strCmdFile.WriteString(strText);

			strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n", 
				((i== idotCount-1&&!bLoop) ? CMD_LINE_END :CMD_LINE_PASSING), enddot.x, enddot.y, enddot.z, enddot.u);
			m_strCmdFile.WriteString(strText);

			if(i==0 && bLoop) startdot = dot;
			predot = dot;
			i++;
		}
		else if(strReadLine == "10")
		{
			sdFile.ReadString(strReadLine);
			dot.x = atof(strReadLine);						
		}
		else if(strReadLine == "20")
		{
			sdFile.ReadString(strReadLine);
			dot.y = atof(strReadLine);

			//
			if(i==idotCount-1 && !bLoop)
				strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n",CMD_LINE_END, dot.x, dot.y, dot.z, dot.u);
			else
				strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n",(i==0 ? CMD_LINE_START: CMD_LINE_PASSING), dot.x, dot.y, dot.z, dot.u);

			m_strCmdFile.WriteString(strText);;

			if(i==0 && bLoop) startdot = dot;
			predot = dot;
			i++;
		}
	} while(i<idotCount || i<1);

	if(bLoop)	
	{
		dot = startdot;
		strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n",CMD_LINE_END, dot.x, dot.y, dot.z, dot.u);
		m_strCmdFile.WriteString(strText);
	}
}

void CDxfClass::HandleArc(CStdioFile &sdFile)
{
	CString strReadLine, strText;
	tgPos d, dot;
	double dRound;
	do
	{
		sdFile.ReadString(strReadLine);
		strReadLine.TrimLeft();
		strReadLine.TrimRight();
	}while(strReadLine != "10");

	sdFile.ReadString(strReadLine);//x圆心
	d.x = atof(strReadLine);

	sdFile.ReadString(strReadLine);//20
	sdFile.ReadString(strReadLine);//y
	d.y = atof(strReadLine);

	sdFile.ReadString(strReadLine);//30
	sdFile.ReadString(strReadLine);//z
	d.z = atof(strReadLine);

	sdFile.ReadString(strReadLine);//40
	sdFile.ReadString(strReadLine);//round
	dRound = atof(strReadLine);
	do
	{
		sdFile.ReadString(strReadLine);
		strReadLine.TrimLeft();
		strReadLine.TrimRight();
	}while(strReadLine != "50");
	sdFile.ReadString(strReadLine);//start dAngle[degree]
	double dStartAngle = atof(strReadLine);

	sdFile.ReadString(strReadLine);//51
	sdFile.ReadString(strReadLine);//end dAngle[degree]
	double dEndAngle = atof(strReadLine);
	if(dEndAngle<dStartAngle) dEndAngle += 360;
	dStartAngle *= DXF_PI/180;
	dEndAngle *= DXF_PI/180;

	dot.x = d.x + dRound * cos(dStartAngle);
	dot.y = d.y + dRound * sin(dStartAngle);
	dot.z = d.z;
	strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n",CMD_LINE_START, dot.x, dot.y, dot.z, dot.u);
	m_strCmdFile.WriteString(strText);

	dot.x = d.x + dRound * cos((dEndAngle + dStartAngle)/2);
	dot.y = d.y + dRound * sin((dEndAngle + dStartAngle)/2);
	strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n",CMD_LINE_ARC, dot.x, dot.y, dot.z, dot.u);
	m_strCmdFile.WriteString(strText);

	dot.x = d.x + dRound * cos(dEndAngle);
	dot.y = d.y + dRound * sin(dEndAngle );
	strText.Format("%%%d,1,4,0,%8.5fF,%8.5fF,%8.5fF,%8.5fF*%%\r\n",CMD_LINE_END, dot.x, dot.y, dot.z, dot.u);
	m_strCmdFile.WriteString(strText);
}

void CDxfClass::HandleCircle(CStdioFile &sdFile)
{
	CString strReadLine, strText, strTemp;
	tgPos d;
	double dRound = 0;
	do
	{
		sdFile.ReadString(strReadLine);
		strReadLine.TrimLeft();
		strReadLine.TrimRight();
	}while(strReadLine != "10");

	sdFile.ReadString(strReadLine);//x center
	d.x = atof(strReadLine);

	sdFile.ReadString(strReadLine);//20 
	sdFile.ReadString(strReadLine);//y  center
	d.y = atof(strReadLine);

	sdFile.ReadString(strReadLine);//30
	sdFile.ReadString(strReadLine);//z
	d.z = atof(strReadLine);

	sdFile.ReadString(strReadLine);//40
	sdFile.ReadString(strReadLine);//round
	dRound = atof(strReadLine);

	strTemp.Format("%%%d,1,12,1", CMD_CIRCLE);
	strText = strTemp;

	strTemp.Format(",%8.5fF,%8.5fF,%8.5fF,%8.5fF", d.x, d.y, d.z, d.u);
	strText += strTemp;
	strTemp.Format(",%8.5fF,%8.5fF,%8.5fF,%8.5fF", dRound, 0.0f, 0.0f, 0.0f);
	strText += strTemp;
	strTemp.Format(",%8.5fF,%8.5fF,%8.5fF,%8.5fF", 0.0f, 0.0f, 0.0f, 0.0f);
	strText += strTemp;

	strText += ",0D*%\r\n";
	m_strCmdFile.WriteString(strText);
}

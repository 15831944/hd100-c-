#include "StdAfx.h"
#include "TSCtrlSys.h"
#include "Function.h"
#include "Precision.h"

CPrecision::CPrecision(void)
{
}

CPrecision::~CPrecision(void)
{
}
void CPrecision::PrecisionFile(BOOL Read)
{
	CString strFile = CFunction::GetDirectory() + "system\\ModeY.pre";
	CString strTemp, strKey;

	CFunction::HandleInt(Read, "BASE", "DateNUM", m_posBase.num, strFile);
	CFunction::HandleDouble(Read, "BASE", "MMPerStep", m_posBase.stepmm, strFile);

	//精度补偿数据
	for(int i=0;i<m_posBase.num;i++)
	{
		strKey.Format("%d",i);
		CFunction::HandleDouble(Read, strKey,"MeasPos", m_pPosInfo[i].measurePos, strFile);

		strKey.Format("%d",i);
		CFunction::HandleDouble(Read, strKey,"RealPos", m_pPosInfo[i].realPos, strFile);

	}

}

// 查找正向最大偏差，负向最大偏差，总体偏差绝对值
double CPrecision::GetMaxDeviation(POSINFO *pPosInfo,double *dTempNegative,double *dTempPositive)
{
	double dTemp;
	*dTempNegative = 0;
	*dTempPositive = 0;

	for(int i=0;i<m_posBase.num;i++)
	{
		if(pPosInfo[i].measurePos<pPosInfo[i].realPos)
		{
			dTemp = pPosInfo[i].measurePos - pPosInfo[i].realPos;
			//寻找最小的负数
			if(dTemp<*dTempNegative)
				*dTempNegative = dTemp;
		}
		else
		{
			dTemp = pPosInfo[i].measurePos - pPosInfo[i].realPos;
			//寻找最大的正数
			if(dTemp>*dTempPositive)
				*dTempPositive = dTemp;
		}
	}

	return (*dTempPositive - *dTempNegative);
}


//由线性测量坐标->非线性真实坐标
// measure mm -> real mm,单轴
double CPrecision::measureMM2realMM(double measureMM)
{
	double realMM;
	double realTemp,measureTemp;
	short mmStepNo; 

	// measure MM ->real MM
	if(0==m_posBase.num || measureMM<=m_pPosInfo[0].measurePos)
	{
		realMM = measureMM;
	}
	else
	{
		for(mmStepNo=0;mmStepNo<m_posBase.num;mmStepNo++)
		{
			if(measureMM<m_pPosInfo[mmStepNo].measurePos)
				break;
		}

		//如果数据超出表格范围，则使用表格中最后一段的比例系数作为参考系
		if(mmStepNo==m_posBase.num)
		{
			mmStepNo--;
		}

		//获取表格内的位置数据
		//根据表格内的相邻两个点的"线性"斜率关系，求出任意位置的实际坐标(realMM)
		//  real - real--     realMM - real--
		// --------------- = -----------------
		//  meas - meas--     mealMM - meas--

		realTemp = (double)m_pPosInfo[mmStepNo].realPos-(double)m_pPosInfo[mmStepNo-1].realPos;
		measureTemp = (double)m_pPosInfo[mmStepNo].measurePos-(double)m_pPosInfo[mmStepNo-1].measurePos;

		realMM = m_pPosInfo[mmStepNo-1].realPos
			+ realTemp/measureTemp*(measureMM-m_pPosInfo[mmStepNo-1].measurePos);
	}

	return realMM;
}


//由非线性真实坐标->线性测量坐标
// real mm -> measure mm, 单轴
double CPrecision::realMM2measureMM(double realMM)
{
	double measureMM;
	double realTemp,measureTemp;
	short mmStepNo;

	// real MM -> measure MM
	if(0==m_posBase.num || realMM<=m_pPosInfo[0].realPos)
	{  
		// 当前位置小于开始校正的位置
		measureMM = realMM;
	}
	else
	{
		for(mmStepNo=0;mmStepNo<m_posBase.num;mmStepNo++)
		{  
			// 找出目标位置所在校正表范围
			if(realMM < m_pPosInfo[mmStepNo].realPos)
				break;
		}

		//如果数据超出表格范围，则使用表格中最后一段的比例系数作为参考系
		if(mmStepNo==m_posBase.num)
		{
			mmStepNo--;
		}

		//获取表格内的位置数据
		//根据表格内的相邻两个点的"线性"斜率关系，求出任意位置的测量坐标(measureMM)
		//  real - real--     realMM - real--
		// --------------- = -----------------
		//  meas - meas--     mealMM - meas--

		realTemp = (double)m_pPosInfo[mmStepNo].realPos
			-(double)m_pPosInfo[mmStepNo-1].realPos;

		measureTemp = (double)m_pPosInfo[mmStepNo].measurePos
			-(double)m_pPosInfo[mmStepNo-1].measurePos;

		measureMM = m_pPosInfo[mmStepNo-1].measurePos
			+ (realMM-m_pPosInfo[mmStepNo-1].realPos)/(realTemp/measureTemp);
	}

	return measureMM;
}
 BOOL CPrecision::GetCurPosMM()
 {
    g_pFrm->m_Robot->m_pController->GetCurPosMM();

	if(g_pFrm->m_pSysParam->bEnablePrecFun)
	{
	  g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[1] = 
		  realMM2measureMM(g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[1]);
	}
	 //double -> long
	 //作用1:对小数点后的数据,做四舍五入.
	 //作用2:防止double到long 数据丢失

	 //(long)(1.400*1000) = 1399
	 // long(1.400*1000+5*1e-4) = 1400
	 // (long)(100.0f*(realMM2measureMM(drealmmTemp,j)+0.005f));
	return TRUE;
 }
 BOOL CPrecision::LineMoveXY(short crd,short fifo,double x,double y,double vel,double acc,BOOL bWaitStop)
 {
    if(g_pFrm->m_pSysParam->bEnablePrecFun == TRUE)
	{
		y  = measureMM2realMM(y);  
	}
	 g_pFrm->m_Robot->m_pController->LineMoveXY(crd,fifo,x,	y, vel,acc, bWaitStop);

	 return TRUE;
 }

 BOOL CPrecision::Measure2Real(double &y)
 {
	 if(g_pFrm->m_pSysParam->bEnablePrecFun == TRUE)
	 {
		 y  = measureMM2realMM(y);  
	 }
	 return TRUE;
 }
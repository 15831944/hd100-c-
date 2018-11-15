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

	//���Ȳ�������
	for(int i=0;i<m_posBase.num;i++)
	{
		strKey.Format("%d",i);
		CFunction::HandleDouble(Read, strKey,"MeasPos", m_pPosInfo[i].measurePos, strFile);

		strKey.Format("%d",i);
		CFunction::HandleDouble(Read, strKey,"RealPos", m_pPosInfo[i].realPos, strFile);

	}

}

// �����������ƫ��������ƫ�����ƫ�����ֵ
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
			//Ѱ����С�ĸ���
			if(dTemp<*dTempNegative)
				*dTempNegative = dTemp;
		}
		else
		{
			dTemp = pPosInfo[i].measurePos - pPosInfo[i].realPos;
			//Ѱ����������
			if(dTemp>*dTempPositive)
				*dTempPositive = dTemp;
		}
	}

	return (*dTempPositive - *dTempNegative);
}


//�����Բ�������->��������ʵ����
// measure mm -> real mm,����
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

		//������ݳ������Χ����ʹ�ñ�������һ�εı���ϵ����Ϊ�ο�ϵ
		if(mmStepNo==m_posBase.num)
		{
			mmStepNo--;
		}

		//��ȡ����ڵ�λ������
		//���ݱ���ڵ������������"����"б�ʹ�ϵ���������λ�õ�ʵ������(realMM)
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


//�ɷ�������ʵ����->���Բ�������
// real mm -> measure mm, ����
double CPrecision::realMM2measureMM(double realMM)
{
	double measureMM;
	double realTemp,measureTemp;
	short mmStepNo;

	// real MM -> measure MM
	if(0==m_posBase.num || realMM<=m_pPosInfo[0].realPos)
	{  
		// ��ǰλ��С�ڿ�ʼУ����λ��
		measureMM = realMM;
	}
	else
	{
		for(mmStepNo=0;mmStepNo<m_posBase.num;mmStepNo++)
		{  
			// �ҳ�Ŀ��λ������У����Χ
			if(realMM < m_pPosInfo[mmStepNo].realPos)
				break;
		}

		//������ݳ������Χ����ʹ�ñ�������һ�εı���ϵ����Ϊ�ο�ϵ
		if(mmStepNo==m_posBase.num)
		{
			mmStepNo--;
		}

		//��ȡ����ڵ�λ������
		//���ݱ���ڵ������������"����"б�ʹ�ϵ���������λ�õĲ�������(measureMM)
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
	 //����1:��С����������,����������.
	 //����2:��ֹdouble��long ���ݶ�ʧ

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
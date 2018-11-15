#pragma once

#define  MAXPREC_NUM  54
typedef struct _posBase
{
	double stepmm;       //��������(mm)
	int num;          //λ�õ�����

}POSBASE;

typedef struct _posInfo
{
	double measurePos;  // ����ֵ   
	double realPos;     // ��ʵ��У����ֵ

}POSINFO;

class CPrecision
{
public:
	CPrecision(void);
	~CPrecision(void);
public:
	 POSBASE m_posBase;               //  ���ݻ�������
	 POSINFO m_pPosInfo[MAXPREC_NUM]; // ���Ȳ��������б�

     void PrecisionFile(BOOL Read);
    
	 double GetMaxDeviation(POSINFO *m_pPosInfo,double *dTempNegative,double *dTempPositive);
	 double measureMM2realMM(double measureMM);
	 double realMM2measureMM(double realMM);

     BOOL GetCurPosMM();
	 BOOL LineMoveXY(
		 short crd,						//�˶�����ϵ
		 short fifo,						//��������
		 double x,						//Xλ��,��λmm
		 double y,						//Yλ��,��λmm
		 double vel=20,					//�ٶ�,mm/s
		 double acc=0.5,					//���ٶ�,mm/(s*s)
		 BOOL bWaitStop=TRUE				//�Ƿ�ȴ��˶����
		 );

	  BOOL Measure2Real(double &y);
};

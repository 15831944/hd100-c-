#pragma once

#define  MAXPREC_NUM  54
typedef struct _posBase
{
	double stepmm;       //步进距离(mm)
	int num;          //位置点数量

}POSBASE;

typedef struct _posInfo
{
	double measurePos;  // 理论值   
	double realPos;     // 真实（校正）值

}POSINFO;

class CPrecision
{
public:
	CPrecision(void);
	~CPrecision(void);
public:
	 POSBASE m_posBase;               //  数据基本属性
	 POSINFO m_pPosInfo[MAXPREC_NUM]; // 精度补偿数据列表

     void PrecisionFile(BOOL Read);
    
	 double GetMaxDeviation(POSINFO *m_pPosInfo,double *dTempNegative,double *dTempPositive);
	 double measureMM2realMM(double measureMM);
	 double realMM2measureMM(double realMM);

     BOOL GetCurPosMM();
	 BOOL LineMoveXY(
		 short crd,						//运动坐标系
		 short fifo,						//缓冲区号
		 double x,						//X位置,单位mm
		 double y,						//Y位置,单位mm
		 double vel=20,					//速度,mm/s
		 double acc=0.5,					//加速度,mm/(s*s)
		 BOOL bWaitStop=TRUE				//是否等待运动完成
		 );

	  BOOL Measure2Real(double &y);
};

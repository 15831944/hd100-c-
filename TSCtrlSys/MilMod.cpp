#include "StdAfx.h"
#include "TSCtrlSys.h"
#include "MilMod.h"
#include <io.h>
//#include <unistd.h>

CMilMod::CMilMod(void)
{
	MilSearchContext= M_NULL;		/* Search context	*/  
	MilModMaskImg	= M_NULL;		/* ModMask Image	*/
	MilResult		= M_NULL;		/* Result identifier*/
	MilModImage		= M_NULL;

	MilDisplay = M_NULL;
	MilOverlay = M_NULL;

	m_modId = -1;
	m_acceptanceScore = 60;		// 可接受最低匹配分数(1~100)
	m_certainScore = 95;		// 确信匹配分数,达到此分数后停止搜索(1~100)
}

CMilMod::~CMilMod(void)
{
	FreeMem();
}

void CMilMod::FreeMem()			// 释放内存,模板有更改时用到
{
	if (M_NULL != MilModImage)
	{
		MbufFree(MilModImage);
		MilModImage = M_NULL;
	}

	if (M_NULL != MilModMaskImg)
	{
		MbufFree(MilModMaskImg);
		MilModMaskImg = M_NULL;
	}

	if (M_NULL != MilSearchContext)
	{
		MmodFree(MilSearchContext);
		MilSearchContext = M_NULL;
	}

	if (M_NULL != MilResult)
	{
		MmodFree(MilResult);
		MilResult = M_NULL;
	}

	if (M_NULL != MilDisplay)
	{
		MdispFree(MilDisplay);
		MilDisplay = M_NULL;
	}
}

void CMilMod::CreateMod(MIL_ID srcImage, int xStart, int yStart, int width, int height)
{
	FreeMem();

	/* Allocate a Geometric Model Finder context. */
	MmodAlloc(g_pView->MilSystem, M_GEOMETRIC_CONTROLLED, M_DEFAULT, &MilSearchContext);

	/* Allocate a result buffer. */
	if (M_NULL == MilResult)
	{
		MmodAllocResult(g_pView->MilSystem, M_DEFAULT, &MilResult);
	}

	/* Define the model. */
	MmodDefine(MilSearchContext, M_IMAGE, g_pView->MilImage,
		xStart, yStart, width, height);

	// Mod Mask
	MbufAlloc2d(g_pView->MilSystem, width, height, 8+M_UNSIGNED,
		M_IMAGE+M_PROC+M_DISP, &MilModMaskImg);
	MbufClear(MilModMaskImg, 0);
	MmodMask(MilSearchContext, 0, MilModMaskImg, M_DONT_CARES, M_DEFAULT);

	// Mod Image
	MbufAlloc2d(g_pView->MilSystem, width, height, 8+M_UNSIGNED,
		M_IMAGE+M_PROC+M_DISP, &MilModImage);
	MbufCopyClip(srcImage, MilModImage, -xStart, -yStart);
	
	/* Modify the acceptance and the certainty for all the models that were defined. */
	MmodControl(MilSearchContext, M_DEFAULT, M_ACCEPTANCE, m_acceptanceScore);
	MmodControl(MilSearchContext, M_DEFAULT, M_CERTAINTY,  m_certainScore);

	/* Set the number of occurrences to 2 for all the models that were defined. */
	MmodControl(MilSearchContext, M_DEFAULT, M_NUMBER, 1);
	/* Set the search speed. */
	MmodControl(MilSearchContext, M_CONTEXT, M_SPEED, M_VERY_HIGH); 

	/* Increase the smoothness for the edge extraction in the search context. */
	MmodControl(MilSearchContext, M_CONTEXT, M_SMOOTHNESS, 75);

	/* Preprocess the search context. */
	MmodPreprocess(MilSearchContext, M_DEFAULT);

	m_nWidth = width;
	m_nHeight = height;
}

static int n_imgSaveId = 0;
static int n_srcImgId = 0;
int CMilMod::FindMod(const double dAcceptance, double &x, double &y)
{
	if (0 == _access("Cap", 0))
	{
		CString filePath;
		filePath.Format("Cap\\src_img%d.bmp", n_imgSaveId);
		MbufExport(filePath.GetBuffer(filePath.GetLength()), M_BMP, g_pView->MilImage);
		n_imgSaveId++;
	}

	UNREFERENCED_PARAMETER(dAcceptance);
	const int MODEL_MAX_OCCURRENCES=1;
	long   Model[MODEL_MAX_OCCURRENCES],     /* Model index.            */
		ModelDrawColor = M_COLOR_GREEN;		 /* Model draw color.       */

	long   NumResults  = 0L;                 /* Number of results found.*/   
	double Score[MODEL_MAX_OCCURRENCES],     /* Model correlation score.*/
		XPosition[MODEL_MAX_OCCURRENCES], /* Model X position.       */
		YPosition[MODEL_MAX_OCCURRENCES], /* Model Y position.       */
		Angle[MODEL_MAX_OCCURRENCES],     /* Model occurrence angle. */
		Scale[MODEL_MAX_OCCURRENCES];     /* Model occurrence scale. */
//		Time = 0.0;                       /* Bench variable.         */  

	// 	/* Dummy first find operation for better function timing accuracy (model cache effet,...). */
	// 	MmodFind(MilSearchContext, MilChildImageShow, MilResult);

	/* Allocate a result buffer. */
	if (M_NULL == MilResult)
	{
		MmodAllocResult(g_pView->MilSystem, M_DEFAULT, &MilResult);
	}

	CString str;
// 	/* Reset the timer. */
// 	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	/* Preprocess the search context. */
	MmodPreprocess(MilSearchContext, M_DEFAULT);
// 	/* Read the find time. */
// 	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &Time);
// 	str.Format("模板预处理时间：%.3f", Time);
// 	g_pFrm->AddMsg(str);

// 	/* Reset the timer. */
// 	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	/* Find the model. */
	MmodFind(MilSearchContext, g_pView->MilImage, MilResult);
// 	/* Read the find time. */
// 	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &Time);
// 	str.Format("模板查找时间：%.3f", Time);
// 	g_pFrm->AddMsg(str);

	/* Get the number of models found. */
	MmodGetResult(MilResult, M_DEFAULT, M_NUMBER+M_TYPE_LONG, &NumResults);

	/* If a model was found above the acceptance threshold. */
	if ( (NumResults >= 1) && (NumResults <= MODEL_MAX_OCCURRENCES) )
	{
		/* Get the results of the single model. */
		MmodGetResult(MilResult, M_DEFAULT, M_INDEX+M_TYPE_LONG, Model);
		MmodGetResult(MilResult, M_DEFAULT, M_POSITION_X, XPosition);
		MmodGetResult(MilResult, M_DEFAULT, M_POSITION_Y, YPosition);
		MmodGetResult(MilResult, M_DEFAULT, M_ANGLE, Angle);
		MmodGetResult(MilResult, M_DEFAULT, M_SCALE, Scale);
		MmodGetResult(MilResult, M_DEFAULT, M_SCORE, Score);
		
		x = XPosition[0];
		y = YPosition[0];

		/* Draw edges, position and box over the occurrences that were found. */
		for (int i=0; i<NumResults; i++)
		{         
			MgraColor(M_DEFAULT, ModelDrawColor);
			MmodDraw(M_DEFAULT,  MilResult, g_pView->MilOverlayImage,//MilSubOverlay,
				M_DRAW_EDGES+M_DRAW_BOX+M_DRAW_POSITION, i, M_DEFAULT);
			str.Format("score:[%f], angle:[%f]", Score[0], Angle[0]);
			MgraBackColor(M_DEFAULT, M_TRANSPARENT);
			MgraText(M_DEFAULT, g_pView->MilOverlayImage, (long)x, (long)y, str.GetBuffer(str.GetLength()));

			if (0 == _access("Cap", 0))
			{
				CString filePath;
				MmodDraw(M_DEFAULT,  MilResult, g_pView->MilImage,//MilSubOverlay,
					M_DRAW_EDGES+M_DRAW_BOX+M_DRAW_POSITION, 0, M_DEFAULT);
				filePath.Format("Cap\\dst_img%d.bmp", n_imgSaveId);
				MbufExport(filePath.GetBuffer(filePath.GetLength()), M_BMP, g_pView->MilImage);
				//n_imgSaveId++;
			}
		}
		if (Score[0] < m_acceptanceScore || 
			Scale[0] < 0.9 || Scale[0] > 1.1)
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// 保存图片
	{
// 		if (0 == _access("Cap", 0))
// 		{
// 			CString filePath;
// 			MmodDraw(M_DEFAULT,  MilResult, g_pView->MilImage,//MilSubOverlay,
// 				M_DRAW_EDGES+M_DRAW_BOX+M_DRAW_POSITION, 0, M_DEFAULT);
// 			filePath.Format("Cap\\src_img%d.bmp", n_imgSaveId);
// 			MbufExport(filePath.GetBuffer(filePath.GetLength()), M_BMP, g_pView->MilImage);
// 			n_imgSaveId++;
// 		}
	}

	return 1;
}

void CMilMod::LoadMod()
{
	FreeMem();

	CString filePath;
	CString strTemp;
	strTemp.Format("%sproduct\\%s\\", CFunction::GetDirectory(), g_AllProgInfo[0].m_curProgName);
	filePath.Format("Model_Mod_%d.mod", m_modId);
	filePath = strTemp + filePath;
	if (-1 == _access(filePath, 0))
	{
		return;
	}

	MmodRestore(filePath.GetBuffer(filePath.GetLength()), g_pView->MilSystem, M_DEFAULT, &MilSearchContext);
	filePath.ReleaseBuffer();
	if (M_NULL == MilSearchContext)
	{
		AfxMessageBox("加载模板出错!!!");
		return;
	}

// 	/* Preprocess the search context. */
// 	MmodPreprocess(MilSearchContext, M_DEFAULT);

	filePath.Format("Model_Img_%d.bmp", m_modId);
	filePath = strTemp + filePath;
	MbufRestore(filePath.GetBuffer(filePath.GetLength()), g_pView->MilSystem, &MilModImage);
	filePath.ReleaseBuffer();
	if (M_NULL == MilModImage)
	{
		AfxMessageBox("加载模板出错!!!");
		return;
	}

	MbufInquire(MilModImage, M_SIZE_X, &m_nWidth);
	MbufInquire(MilModImage, M_SIZE_Y, &m_nHeight);

	// Mod Mask
	MbufAlloc2d(g_pView->MilSystem, m_nWidth, m_nHeight, 8+M_UNSIGNED,
		M_IMAGE+M_PROC+M_DISP, &MilModMaskImg);
	MbufClear(MilModMaskImg, 0);

	if (M_NULL == MilDisplay)
	{
		MdispAlloc(g_pView->MilSystem, M_DEFAULT, M_DEF_DISPLAY_FORMAT, M_DEFAULT, &MilDisplay);
	}
}

void CMilMod::SaveMod()			// 保存模板
{
	if (M_NULL == MilModImage)
	{
		return;
	}
	CString filePath;
	CString strTemp;
	strTemp.Format("%sproduct\\%s\\", CFunction::GetDirectory(), g_AllProgInfo[0].m_curProgName);
	filePath.Format("Model_Mod_%d.mod", m_modId);
	filePath = strTemp + filePath;
	
	MmodSave(filePath.GetBuffer(filePath.GetLength()), MilSearchContext, M_DEFAULT);
	filePath.ReleaseBuffer();

	filePath.Format("Model_Img_%d.bmp", m_modId);
	filePath = strTemp + filePath;
	MbufExport(filePath.GetBuffer(filePath.GetLength()), M_BMP, MilModImage);
	filePath.ReleaseBuffer();
}

void CMilMod::ShowMod(HWND hWnd)
{
	if (M_NULL == MilDisplay)
	{
		MdispAlloc(g_pView->MilSystem, M_DEFAULT, M_DEF_DISPLAY_FORMAT, M_DEFAULT, &MilDisplay);
	}
	if (MilModImage == M_NULL)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	MdispSelectWindow(MilDisplay, MilModImage/*MilModMaskImg*/, hWnd);
	/* Enable the display of overlay annotations. */
	MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
	/* Inquire the overlay buffer associated with the display. */
	MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlay);
	/* Clear the overlay to transparent. */
	MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
	// Now that overlay buffer is correctly prepared, display it [CALL TO MIL]
	MdispControl(MilDisplay, M_OVERLAY_SHOW, M_ENABLE); 
	//////////////////////////////////////////////////////////////////////////

//	MmodMask(MilSearchContext, 0, MilModMaskImg, M_DONT_CARES, M_DEFAULT);
	MgraColor(M_DEFAULT, M_COLOR_RED);
	MmodDraw(M_DEFAULT,  MilSearchContext, MilOverlay,
		M_DRAW_EDGES/*+M_DRAW_BOX+M_DRAW_POSITION*/, M_ALL, M_DEFAULT);
}

void CMilMod::SetAcceptance(int accept)
{
	m_acceptanceScore = accept;
}

void CMilMod::ClearOverlay()
{
	/* Clear the overlay to transparent. */
	MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
	// Now that overlay buffer is correctly prepared, display it [CALL TO MIL]
	MdispControl(MilDisplay, M_OVERLAY_SHOW, M_ENABLE); 
}


void CMilMod::EditMod(CPoint point)
{
	MgraColor(M_DEFAULT, M_COLOR_WHITE);
	MgraRectFill(M_DEFAULT, MilModMaskImg, 
		point.x-3, point.y-3,
		point.x+3, point.y+3
		);
	MmodMask(MilSearchContext, 0, MilModMaskImg, M_DONT_CARES, M_DEFAULT);

	/* Preprocess the search context. */
	MmodPreprocess(MilSearchContext, M_DEFAULT);

	/* Clear the overlay to transparent. */
	MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

	MgraColor(M_DEFAULT, M_COLOR_RED);
	MmodDraw(M_DEFAULT,  MilSearchContext, MilOverlay,
		M_DRAW_EDGES/*+M_DRAW_BOX+M_DRAW_POSITION*/, M_ALL, M_DEFAULT);
}
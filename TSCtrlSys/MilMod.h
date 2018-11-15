#pragma once

class CMilMod
{
public:
	CMilMod(void);
	~CMilMod(void);

	MIL_ID MilSearchContext,             /* Search context		*/
		MilModImage,					 /* Mod Image buffer	*/
		MilModMaskImg,					 /* ModMask Image		*/
		MilResult;                       /* Result identifier	*/

	//////////////////////////////////////////////////////////////////////////
	// Display
	MIL_ID MilDisplay,
		MilOverlay;

	int m_modId;
	int m_acceptanceScore;	// 可接受最低匹配分数(1~100)
	int m_certainScore;		// 确信匹配分数,达到此分数后停止搜索(1~100)

	long m_nWidth;
	long m_nHeight;

	void FreeMem();			// 释放内存,模板有更改时用到
	void LoadMod();			// 加载模板
	void SaveMod();			// 保存模板

	void ShowMod(HWND hWnd);

	void CreateMod(MIL_ID srcImage, int xStart, int yStart, int width, int height);
	int FindMod(const double dAcceptance, double &x, double &y);

	void ClearOverlay();
	void EditMod(CPoint point);

	void SetAcceptance(int accept);
};


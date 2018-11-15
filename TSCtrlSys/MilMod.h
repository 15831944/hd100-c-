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
	int m_acceptanceScore;	// �ɽ������ƥ�����(1~100)
	int m_certainScore;		// ȷ��ƥ�����,�ﵽ�˷�����ֹͣ����(1~100)

	long m_nWidth;
	long m_nHeight;

	void FreeMem();			// �ͷ��ڴ�,ģ���и���ʱ�õ�
	void LoadMod();			// ����ģ��
	void SaveMod();			// ����ģ��

	void ShowMod(HWND hWnd);

	void CreateMod(MIL_ID srcImage, int xStart, int yStart, int width, int height);
	int FindMod(const double dAcceptance, double &x, double &y);

	void ClearOverlay();
	void EditMod(CPoint point);

	void SetAcceptance(int accept);
};


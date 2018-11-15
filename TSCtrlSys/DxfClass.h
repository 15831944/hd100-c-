#pragma once

class CDxfClass
{
public:
	CDxfClass(void);
	~CDxfClass(void);

	//////////////////////////////////////////////////////////////////////////
	BOOL OpenDxfFile(CString strDxfFile, CString strCmdFile);

private:
	void HandleCircle(CStdioFile &sdFile);
	void HandleArc(CStdioFile &sdFile);
	void HandlePlyLine(CStdioFile &sdFile);
	void HandlePoint(CStdioFile &sdFile);
	void HandleLine(CStdioFile &File);

protected:
	CStdioFile m_strCmdFile;
};

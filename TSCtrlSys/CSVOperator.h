#pragma once
#include "StringParser.h"

class CCSVOperator
{
public:
    CCSVOperator(){};
    ~CCSVOperator(){};
    CCSVOperator(const char* path);

    bool LoadCSV(const char* path);
    bool SaveCSV(const char* path = NULL);

    bool GetInt(u32 uiLine, u32 uiRow, int& iValue);
	bool GetFloat(u32 uiLine, u32 uiRow, float& fValue);
	bool GetDouble(u32 uiLine, u32 uiRow, double& dValue);

    std::string* GetString(u32 uiLine, u32 uiRow);

    bool SetNumber(u32 uiLine, u32 uiRow, int iValue);
	bool SetNumber(u32 uiLine, u32 uiRow, float fValue);
	bool SetNumber(u32 uiLine, u32 uiRow, double dValue);

    bool SetString(u32 uiLine, u32 uiRow, const char* pStr);
    std::map<u32, std::map<u32, std::string> >& GetCSVMap(){return m_StringKeyMap;}

protected:
    std::string m_CSVName;
    std::map<u32, std::map<u32, std::string> > m_StringKeyMap;
};


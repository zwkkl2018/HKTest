
/**
 * 用于输出log文件的类.
 */

#ifndef LOG_H  
#define LOG_H  


#include <fstream>  
#include <string>  
#include <ctime>
//#include "Ctrol.h"
//#include "CameraConfig.h"
#pragma once

//#define LOG_API __declspec(dllexport)
#pragma warning(disable:4996)
using namespace std;



class CmLog
{
	CRITICAL_SECTION     m_cs;
	//	vector<camCtr>*      m_pCameraV;
public:
	string logfileName;
	CmLog(string filename)
	{
		logfileName = filename;
		InitializeCriticalSection(&m_cs);

	}
	~CmLog()
	{
		DeleteCriticalSection(&m_cs);
	}
	//wchar_t* C2W(const char* str)
	//{
	//	int len = MultiByteToWideChar(CP_OEMCP, 0, str, -1/*null terminated*/, NULL, 0);
	//	wchar_t* wstr = new wchar_t[len + 1];
	//	MultiByteToWideChar(CP_OEMCP, 0, str, -1/*null terminated*/, wstr, len);
	//	return wstr;
	//}
	void setlogfile(string logFile)
	{
		string ss = logFile;
		logfileName = ss;

		ifstream ifile;
		ifile.open(logfileName.c_str());
		if (!ifile.is_open())
		{
			FILE *pfile = fopen(logfileName.c_str(), "wb+");
			if (pfile == NULL)
			{
				return;
			}
			fclose(pfile);
			ifile.open(logfileName.c_str());
		}
		ifile.close();
	}

	void logString(int caminfoid, string logStr)
	{


		SYSTEMTIME sys;
		GetLocalTime(&sys);
		char timeStrID[200];
		sprintf(timeStrID, "[%4d-%02d-%02d %02d:%02d:%02d.%03d] Cam:%d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, caminfoid);
		std::ofstream of;
		EnterCriticalSection(&m_cs);
		of.open(logfileName, std::ios::app);
		if (of.is_open())
		{
			of << timeStrID << "\t" << logStr << std::endl;
		}
		LeaveCriticalSection(&m_cs);
	}


	void logString(int caminfoid, vector<string> logStrVecotr)
	{


		SYSTEMTIME sys;
		GetLocalTime(&sys);
		char timeStrID[200];
		sprintf(timeStrID, "[%4d-%02d-%02d %02d:%02d:%02d.%03d] Cam:%d info:", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, caminfoid);
		std::ofstream of;
		EnterCriticalSection(&m_cs);
		of.open(logfileName, std::ios::app);
		for (int i = 0; i < logStrVecotr.size();i++)
		{			
			if (of.is_open())
			{
				of << timeStrID << "\t" << logStrVecotr[i] << std::endl;
			}
		}
		LeaveCriticalSection(&m_cs);
	}


};

#endif  
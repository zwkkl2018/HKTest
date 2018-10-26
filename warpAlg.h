#pragma  once
#include "TissueAlg.h"
#include "CtextPro.h"
#include "CameraConfig.h"
#include <iostream>
#include <functional>
using namespace std;
//#include "CameraConfig.h"
enum MyAlgerrEnum
{
	err_polyVPoint = -4,
	err_getROImat =-5,
	err_initpara = -10,
};


enum algEnum
{
	algNULL,
	topTopBV,
	topOtherPara, 

	lsNorm,
	lsOtherPara,

	ss0Norm,
	ss1Norm,
};
 

class warpAlg
{
public:
	warpAlg();
	~warpAlg();
	bool initAlg();
	int algProcess(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG);
	int algSetParaProcess(algEnum algStep, int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG, bool resetPara = false);
	string appPath;

	cv::Mat llSNormMatback;
	cv::Mat ss0SNormMatback;
	cv::Mat ss1SNormMatback;


	cv::Mat llSNormMat;
	cv::Mat ss0SNormMat;
	cv::Mat ss1SNormMat;

private:
	int algTopP(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG);
	int algwithoutTopSP(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG);


	int algLSP(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG);
	int algSSP_0(int camID, int* para[], const  cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG);
	int algSSP_1(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG);

	int _algTopPSetPara(algEnum algStep, int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG, bool resetPara = false);
	int _algLSPSetPara(algEnum algStep, int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG, bool resetPara = false);
	int _algSSP_0SetPara(algEnum algStep, int camID, int* para[], const  cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG, bool resetPara = false);
	int _algSSP_1SetPara(algEnum algStep, int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG, bool resetPara = false);

	bool getMapValue(ParaKey parkey, double& value);
	void setInfo(string info, int ret, vector<string>& outInfo, vector<string>& outInfoLOG);

	void LogString(int camIDinfo, string str);
	int  enterHelp(int camIdinfo);
	int  returnHelp(int camIdinfo, int ret);

public:
	string app_NormPath;

	string llSNormPath;
	string ss0SNormPath;
	string ss1SNormPath;

	//cv::Mat llSNormMat;
	//cv::Mat ss0SNormMat;
	//cv::Mat ss1SNormMat;

	CRITICAL_SECTION  _criticaTOPP;
	CRITICAL_SECTION  _criticalLSP;
	CRITICAL_SECTION  _criticaSS0SP;
	CRITICAL_SECTION  _criticaSS1SP;

public:
	CameraConfig*	m_pCameraConfig;
	TissueAlg*      ppTopAlg[maxCamcnt];
	CtextPro*       ptextPro[maxCamcnt];
	map<ParaKey, int>*	     p_MapParaKey;

	//CmLog*   pCmLog;
public:
	std::map<int, std::function<Mat(int, int)>>   algFunctiontable;
private:
};

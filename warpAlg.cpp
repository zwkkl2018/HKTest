#include "stdafx.h"
#include "warpAlg.h"

warpAlg::warpAlg()
{
}

warpAlg::~warpAlg()
{
	DeleteCriticalSection(&_criticalLSP);
	DeleteCriticalSection(&_criticaTOPP);

	DeleteCriticalSection(&_criticaSS0SP);

	DeleteCriticalSection(&_criticaSS1SP);

}
bool warpAlg::initAlg()
{
	for (size_t i = 0; i < maxCamcnt/*m_pCameraConfig->m_MySEQ.size()*/; i++)
	{
		ptextPro[i] = new CtextPro;
		ppTopAlg[i] = new TissueAlg;
		if (ppTopAlg == NULL || ptextPro == NULL) return false;
	}
	app_NormPath = appPath + ("\\用户数据\\Admin\\出厂参数");

	InitializeCriticalSection(&_criticalLSP);
	InitializeCriticalSection(&_criticaTOPP);
	InitializeCriticalSection(&_criticaSS0SP);
	InitializeCriticalSection(&_criticaSS1SP);

	return true;
}



int warpAlg::algProcess(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG)
{

	if (camID == CamIDType::CAM_TOP)			return algTopP(camID, para, inImg, outImg, outInfo, outInfoLOG);
	else
	{
		return algwithoutTopSP(camID, para, inImg, outImg, outInfo, outInfoLOG);
	}
	//if (camID == CamIDType::CAM_LONGSIDE)		return algLSP(CAM_LONGSIDE, para, inImg, outImg, outInfo,  outInfoLOG);
	//if (camID == CamIDType::CAM_SHORTSIDE_0)	return algSSP_0(CAM_SHORTSIDE_0, para, inImg, outImg, outInfo,outInfoLOG);
	//if (camID == CamIDType::CAM_SHORTSIDE_1)	return algSSP_1(CAM_SHORTSIDE_1, para, inImg, outImg, outInfo,  outInfoLOG);

}

void warpAlg::LogString(int camIDinfo, string str)
{
	/*for (size_t i = 0; i < m_pCameraConfig->m_MySEQ.size(); i++)
	{
	}*/
	double value = 0;
	ParaKey saveDataLog(camIDinfo, "相机参数", "保存数据");
	getMapValue(saveDataLog, value);
	if (value == 0) return;

	LogInstance()->logString(camIDinfo, str);
}

int warpAlg::algTopP(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG)
{
	int ret = 0;
	//EnterCriticalSection(&_criticaTOPP);
	enterHelp(camID);
	ptextPro[camID]->camIdinfo = camID;
	ppTopAlg[camID]->camIdinfo = camID;
	//LOGString("enter algTopP");
	string info;
	char charTmp[200] = { 0 };
	int val0 = 50;
	if (para == NULL)
	{
		ParaKey parkey(camID, "二值化阈值", "阈值");
		auto m = p_MapParaKey->find(parkey);
		if (m != p_MapParaKey->end()) val0 = m->second;
	}

	double value = 0;
	double totalAreaThv = 922496;
	double areaThreshole = 10;
	double areaLengthThv = 3;
	if (para == NULL)
	{
		ParaKey parkeyArea(camID, "轮廓参数", "面积");
		if (getMapValue(parkeyArea, value))totalAreaThv = value;

		ParaKey parkeyEdgeArea(camID, "轮廓参数", "边缘面积");
		if (getMapValue(parkeyEdgeArea, value))areaThreshole = value;
		ParaKey parkeyEdgeAreaLength(camID, "轮廓参数", "面积/长度");
		if (getMapValue(parkeyEdgeAreaLength, value))areaLengthThv = value;

	}

	cv::Mat outcvt2BinColor;
	cv::Rect roiRect = cv::Rect(0, 0, inImg.cols, inImg.rows);

	LogString(camID, "enter processThresholdtransform");
	ppTopAlg[camID]->processThresholdtransform(inImg, outImg, outcvt2BinColor, val0, 0, roiRect);


	vector<vector<cv::Point> > polyVPoint;
	vector<geometricFeatureItem> geometricFeature;

	LogString(camID, "enter processapproxPolyDP");

	double maxArea = 0.0;
	ret = ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 4, maxArea);
	sprintf_s(charTmp, "leave processapproxPolyDP _ maxArea = %f", maxArea);

	LogString(camID, charTmp);
	outInfo.push_back(charTmp);

	pointLineFeature  pLFeature;
	if (polyVPoint.size() == 0 || ret < 0)
	{
		LogString(camID, "enter polyVPoint.size()");
		setInfo("pointLineFeature", -4, outInfo, outInfoLOG);
		LeaveCriticalSection(&_criticaTOPP);
		return err_polyVPoint;
	}
	LogString(camID, "enter processPointLineFeature");

	ret = ppTopAlg[camID]->processPointLineFeature(outImg, polyVPoint.at(0), pLFeature, areaThreshole, areaLengthThv, totalAreaThv);
	setInfo("ppTopAlg", ret, outInfo, outInfoLOG);
	LogString(camID, "Leave algTopP");

	//LeaveCriticalSection(&_criticaTOPP);
	return   returnHelp(camID,  ret);
}

int  warpAlg::enterHelp(int camIdinfo)
{
	//if (camIdinfo == CamIDType::CAM_TOP)			EnterCriticalSection(&_criticaTOPP);
	//if (camIdinfo == CamIDType::CAM_LONGSIDE)		EnterCriticalSection(&_criticalLSP); //return algLSP(CAM_LONGSIDE, para, inImg, outImg, outInfo, outInfoLOG);
	//if (camIdinfo == CamIDType::CAM_SHORTSIDE_0)	EnterCriticalSection(&_criticaSS0SP); //return algSSP_0(CAM_SHORTSIDE_0, para, inImg, outImg, outInfo, outInfoLOG);
	//if (camIdinfo == CamIDType::CAM_SHORTSIDE_1)    EnterCriticalSection(&_criticaSS1SP);//
	//LogString(camIdinfo, "enter enterHelp");

	return 0;
}
int  warpAlg::returnHelp(int camIdinfo ,int ret)
{
	//if (camIdinfo == CamIDType::CAM_TOP)			LeaveCriticalSection(&_criticaTOPP);
	//if (camIdinfo == CamIDType::CAM_LONGSIDE)		LeaveCriticalSection(&_criticalLSP); //return algLSP(CAM_LONGSIDE, para, inImg, outImg, outInfo, outInfoLOG);
	//if (camIdinfo == CamIDType::CAM_SHORTSIDE_0)	LeaveCriticalSection(&_criticaSS0SP); //return algSSP_0(CAM_SHORTSIDE_0, para, inImg, outImg, outInfo, outInfoLOG);
	//if (camIdinfo == CamIDType::CAM_SHORTSIDE_1)    LeaveCriticalSection(&_criticaSS1SP);//  LeaveCriticalSection(&_criticalLSP);
	//LogString(camIdinfo, "return returnHelp");

	return  ret;
}
int warpAlg::algwithoutTopSP(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG)
{
	enterHelp(camID);
	if (camID==2)
	{
		int kk = 90;
	}
	//EnterCriticalSection(&_criticalLSP);
	ptextPro[camID]->camIdinfo = camID;
	ppTopAlg[camID]->camIdinfo = camID;
	int ret = 0;
	static bool init = false;
	bool  setNorMatFlg = false;
	cv::Mat NOrmMat;
	if (camID == CAM_LONGSIDE)
	{
		if (llSNormMat.empty() || !init)	{ llSNormPath = app_NormPath + "\\llSNormPath.jpg";	llSNormMat = cv::imread(llSNormPath); setNorMatFlg = true; }
		NOrmMat = llSNormMat;
		if (NOrmMat.empty()){ outInfo.push_back("请设置侧封相机参数"); return returnHelp(camID, err_initpara); }
		 
	}

	if (camID == CAM_SHORTSIDE_0)
	{
		if (ss0SNormMat.empty() || !init){ ss0SNormPath = app_NormPath + "\\SS0SNormPath.jpg"; ss0SNormMat = cv::imread(ss0SNormPath); setNorMatFlg = true; }
		NOrmMat = ss0SNormMat;
		if (NOrmMat.empty()){ outInfo.push_back("请设置端封0相机参数");  return returnHelp(camID, err_initpara);  }


	}
	if (camID == CAM_SHORTSIDE_1)
	{
		if (ss1SNormMat.empty() || !init){ ss1SNormPath = app_NormPath + "\\SS1SNormPath.jpg"; ss1SNormMat = cv::imread(ss1SNormPath); setNorMatFlg = true; }
		NOrmMat = ss1SNormMat;
		if (NOrmMat.empty()){ outInfo.push_back("请设置端封1相机参数"); return returnHelp(camID, err_initpara); }

	}

	if (setNorMatFlg )
	{
		ptextPro[camID]->setImgHSVNormP(NOrmMat);

		cv::Mat textNorm;
		ptextPro[camID]->imgHSVNorm(NOrmMat, textNorm);
		Mat   outcvt2BinColor;
		ppTopAlg[camID]->processThresholdtransform(NOrmMat, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)


		vector<vector<cv::Point> > polyVPoint;
		//vector<geometricFeatureItem> geometricFeature;
		double max_Area;
		ret = ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_Area);//top(16)
		if (ret != E_SUCEES)
		{
			LogString(camID, "err polyVPoint.size()");
			setInfo("pointLineFeature", err_polyVPoint, outInfo, outInfoLOG);
			return returnHelp(camID, err_polyVPoint);

		}
		 
		ret = ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, true);
		if (ret != 0)
		{
			LogString(camID, "err getROImat");
			setInfo("getROImat", ret, outInfo, outInfoLOG);
			return returnHelp(camID, ret);

		}
		Mat outimgtF;
		ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8


		cv::Point psize50(50, 50);
		cv::Point psize20(20, 20);

		cv::Point psize3(15, 15);
		cv::Point psize10(5, 5);

		vector<cv::Point> recccSize;
		recccSize.push_back(psize50);
		recccSize.push_back(psize20);
		recccSize.push_back(psize3);
		recccSize.push_back(psize10);

		ptextPro[camID]->setImgDensityNormP(outimgtF, recccSize, 1);
		setInfo("lsNorm:", 0, outInfo, outInfoLOG);
 
		init = true;	 
		LogString(camID, "err polyVPoint.size()");
		setInfo("设置相机成功", camID, outInfo, outInfoLOG);

		return returnHelp(camID, 0);


	}
 

	if (!init)
	{
		setInfo("init : false ", 0, outInfo, outInfoLOG);
		//LeaveCriticalSection(&_criticalLSP);
		return returnHelp(camID, 0);

	}


	cv::Mat textNorm;
	ptextPro[camID]->imgHSVNorm(inImg, textNorm);
	//	ptextPro[camID]->imgHSVNorm(inImg, outImg);
	Mat   outcvt2BinColor;
	ppTopAlg[camID]->processThresholdtransform(inImg, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)
	vector<vector<cv::Point> > polyVPoint;
	vector<geometricFeatureItem> geometricFeature;
	double max_Area;
	ret = ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_Area);//top(16)

	if (polyVPoint.size() == 0 || ret < 0)
	{
		LogString(camID, "err polyVPoint.size()");

		setInfo("pointLineFeature", err_polyVPoint, outInfo, outInfoLOG);
		//LeaveCriticalSection(&_criticalLSP);
		return returnHelp(camID, err_polyVPoint);

	}
	ret = ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, false);
	if (ret != 0)
	{
		//LeaveCriticalSection(&_criticalLSP);
		
		return returnHelp(camID, err_getROImat);
	}
	Mat outimgtF;
	ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8

	cv::Point psize50(50, 50);
	cv::Point psize20(20, 20);

	cv::Point psize3(15, 15);
	cv::Point psize10(5, 5);

	vector<cv::Point> recccSize;
	recccSize.push_back(psize50);
	recccSize.push_back(psize20);
	recccSize.push_back(psize3);
	recccSize.push_back(psize10);


	ptextPro[camID]->getImgDensity(outimgtF);
	ptextPro[camID]->pDFeatureCompare();


	ptextPro[camID]->getOriDensityFeature();
	ptextPro[camID]->getComperDensityFeature();


	vector<double>  score = ptextPro[camID]->getHashSiminal();
	for (int i = 0; i < score.size(); i++)
	{
		char scoretmp[200] = { 0 };
		sprintf_s(scoretmp, "hash siminal:%f", score[i]);
		setInfo(scoretmp, 0, outInfo, outInfoLOG);
	}

	setInfo("lsOtherPara:", 0, outInfo, outInfoLOG);
	outImg = outimgtF;
	//LeaveCriticalSection(&_criticalLSP);
	return returnHelp(camID, 0);

	 
}

int warpAlg::algLSP(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG)
{

	EnterCriticalSection(&_criticalLSP);
	ptextPro[camID]->camIdinfo = camID;
	ppTopAlg[camID]->camIdinfo = camID;

	int ret = 0;
	static bool init = false;
	if (llSNormMat.empty())
	{
		llSNormPath = app_NormPath + "\\llSNormPath.jpg";
		llSNormMat = cv::imread(llSNormPath);

		if (llSNormMat.empty())
		{

			outInfo.push_back("请设置侧封相机参数");
			outInfoLOG.push_back("请设置侧封相机参数");
			LeaveCriticalSection(&_criticalLSP);
			return err_initpara;

		}

		ptextPro[camID]->setImgHSVNormP(llSNormMat);

		cv::Mat textNorm;
		ptextPro[camID]->imgHSVNorm(llSNormMat, textNorm);
		Mat   outcvt2BinColor;
		ppTopAlg[camID]->processThresholdtransform(llSNormMat, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)


		vector<vector<cv::Point> > polyVPoint;
		//vector<geometricFeatureItem> geometricFeature;
		double max_Area;
		ret = ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_Area);//top(16)
		if (polyVPoint.size() == 0 || ret < 0)
		{
			LogString(camID, "err polyVPoint.size()");

			setInfo("pointLineFeature", err_polyVPoint, outInfo, outInfoLOG);
			LeaveCriticalSection(&_criticalLSP);
			return err_polyVPoint;
		}
		ret = ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, true);
		if (ret != 0)
		{
			LeaveCriticalSection(&_criticalLSP);
			return err_getROImat;
		}
		Mat outimgtF;
		ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8


		cv::Point psize50(50, 50);
		cv::Point psize20(20, 20);

		cv::Point psize3(15, 15);
		cv::Point psize10(5, 5);

		vector<cv::Point> recccSize;
		recccSize.push_back(psize50);
		recccSize.push_back(psize20);
		recccSize.push_back(psize3);
		recccSize.push_back(psize10);

		ptextPro[camID]->setImgDensityNormP(outimgtF, recccSize, 1);
		setInfo("lsNorm:", 0, outInfo, outInfoLOG);

		//	llSNormPath = app_NormPath + "\\llSNormPath.jpg";
		//	cv::imwrite(llSNormPath, inImg);
		//	llSNormMat = inImg;
		//	outImg = outimgtF;
		init = true;
		LeaveCriticalSection(&_criticalLSP);

		return 0;

	}


	if (!init)
	{
		setInfo("init : false ", 0, outInfo, outInfoLOG);
		LeaveCriticalSection(&_criticalLSP);
		return 0;
	}


	cv::Mat textNorm;
	ptextPro[camID]->imgHSVNorm(inImg, textNorm);
	//	ptextPro[camID]->imgHSVNorm(inImg, outImg);
	Mat   outcvt2BinColor;
	ppTopAlg[camID]->processThresholdtransform(inImg, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)
	vector<vector<cv::Point> > polyVPoint;
	vector<geometricFeatureItem> geometricFeature;
	double max_Area;
	ret = ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_Area);//top(16)

	if (polyVPoint.size() == 0 || ret < 0)
	{
		LogString(camID, "err polyVPoint.size()");

		setInfo("pointLineFeature", err_polyVPoint, outInfo, outInfoLOG);
		LeaveCriticalSection(&_criticalLSP);
		return err_polyVPoint;
	}
	ret = ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, false);
	if (ret != 0)
	{
		LeaveCriticalSection(&_criticalLSP);
		return err_getROImat;
	}
	Mat outimgtF;
	ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8

	cv::Point psize50(50, 50);
	cv::Point psize20(20, 20);

	cv::Point psize3(15, 15);
	cv::Point psize10(5, 5);

	vector<cv::Point> recccSize;
	recccSize.push_back(psize50);
	recccSize.push_back(psize20);
	recccSize.push_back(psize3);
	recccSize.push_back(psize10);


	ptextPro[camID]->getImgDensity(outimgtF);
	ptextPro[camID]->pDFeatureCompare();


	ptextPro[camID]->getOriDensityFeature();
	ptextPro[camID]->getComperDensityFeature();


	vector<double>  score = ptextPro[camID]->getHashSiminal();
	cout << "/n" << endl;
	for (int i = 0; i < score.size(); i++)
	{
		//cout << "hash siminal :" << score[i] << endl;

		char scoretmp[200] = { 0 };
		sprintf_s(scoretmp, "hash siminal:%f", score[i]);
		setInfo(scoretmp, 0, outInfo, outInfoLOG);
	}

	setInfo("lsOtherPara:", 0, outInfo, outInfoLOG);
	outImg = outimgtF;
	LeaveCriticalSection(&_criticalLSP);

	return 0;
}
int warpAlg::algSSP_0(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG)
{
	//return 0;
	EnterCriticalSection(&_criticaSS0SP);
	ptextPro[camID]->camIdinfo = camID;
	ppTopAlg[camID]->camIdinfo = camID;
	int ret = 0;
	char charTmp[256] = { 0 };
	static bool init = false;
	if (ss0SNormMat.empty())
	{
		ss0SNormPath = app_NormPath + "\\SS0SNormPath.jpg";
		ss0SNormMat = cv::imread(ss0SNormPath);

		if (ss0SNormMat.empty())
		{
			//AfxMessageBox(TEXT("请设置端封0相机参数"));
			outInfo.push_back("请设置端封0相机参数");
			LogString(camID, "请设置端封0相机参数");
			LeaveCriticalSection(&_criticaSS0SP);
			return err_initpara;
		}

		ptextPro[camID]->setImgHSVNormP(ss0SNormMat);

		cv::Mat textNorm;
		ptextPro[camID]->imgHSVNorm(ss0SNormMat, textNorm);
		Mat   outcvt2BinColor;
		ppTopAlg[camID]->processThresholdtransform(ss0SNormMat, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)


		vector<vector<cv::Point> > polyVPoint;
		//vector<geometricFeatureItem> geometricFeature;
		double max_Area;
		ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_Area);//top(16)

		ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, true);

		Mat outimgtF;
		ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8


		cv::Point psize50(50, 50);
		cv::Point psize20(20, 20);

		cv::Point psize3(15, 15);
		cv::Point psize10(5, 5);

		vector<cv::Point> recccSize;
		recccSize.push_back(psize50);
		recccSize.push_back(psize20);
		recccSize.push_back(psize3);
		recccSize.push_back(psize10);

		ptextPro[camID]->setImgDensityNormP(outimgtF, recccSize, 1);
		setInfo("lsNorm:", 0, outInfo, outInfoLOG);

		//	llSNormPath = app_NormPath + "\\llSNormPath.jpg";
		//	cv::imwrite(llSNormPath, inImg);
		//	llSNormMat = inImg;
		//	outImg = outimgtF;
		init = true;
		LeaveCriticalSection(&_criticaSS0SP);

		return 0;

	}

	if (!init)
	{

		outInfo.push_back("init : false 请设置端封0相机参数");
		outInfoLOG.push_back("init : false 请设置端封0相机参数");

		LeaveCriticalSection(&_criticaSS0SP);
		return err_initpara;

	}


	cv::Mat textNorm;
	ptextPro[camID]->imgHSVNorm(inImg, textNorm);
	//	ptextPro[camID]->imgHSVNorm(inImg, outImg);
	Mat   outcvt2BinColor;
	LogString(camID, "enter processThresholdtransform");
	ppTopAlg[camID]->processThresholdtransform(inImg, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)
	vector<vector<cv::Point> > polyVPoint;
	vector<geometricFeatureItem> geometricFeature;
	double max_Area;

	LogString(camID, "enter processapproxPolyDP");

	ret = ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_Area);//top(16)
	sprintf_s(charTmp, "leave processapproxPolyDP _ maxArea = %f", max_Area);
	LogString(camID, charTmp);

	outInfo.push_back(charTmp);
	if (polyVPoint.size() == 0 || ret < 0)
	{
		outInfoLOG.push_back("err_polyVPoint.size()");
		setInfo("pointLineFeature", err_polyVPoint, outInfo, outInfoLOG);
		LeaveCriticalSection(&_criticaSS0SP);
		return err_polyVPoint;
	}
	LogString(camID, "enter getROImat");

	ret = ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, false);
	if (ret != 0)
	{
		LogString(camID, " err getROImat");
		LeaveCriticalSection(&_criticaSS0SP);
		return err_getROImat;
	}
	Mat outimgtF;
	LogString(camID, "enter imgFileter");

	ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8

	cv::Point psize50(50, 50);
	cv::Point psize20(20, 20);

	cv::Point psize3(15, 15);
	cv::Point psize10(5, 5);

	vector<cv::Point> recccSize;
	recccSize.push_back(psize50);
	recccSize.push_back(psize20);
	recccSize.push_back(psize3);
	recccSize.push_back(psize10);


	ptextPro[camID]->getImgDensity(outimgtF);
	ptextPro[camID]->pDFeatureCompare();


	ptextPro[camID]->getOriDensityFeature();
	ptextPro[camID]->getComperDensityFeature();


	vector<double>  score = ptextPro[camID]->getHashSiminal();
	cout << "/n" << endl;
	for (int i = 0; i < score.size(); i++)
	{
		//cout << "hash siminal :" << score[i] << endl;

		char scoretmp[200] = { 0 };
		sprintf_s(scoretmp, "hash siminal:%f", score[i]);
		setInfo(scoretmp, 0, outInfo, outInfoLOG);
	}

	setInfo("lsOtherPara:", camID, outInfo, outInfoLOG);
	outImg = outimgtF;


	LeaveCriticalSection(&_criticaSS0SP);

	return 0;
}
int warpAlg::algSSP_1(int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG)
{
	EnterCriticalSection(&_criticaSS1SP);
	ptextPro[camID]->camIdinfo = camID;
	ppTopAlg[camID]->camIdinfo = camID;
	int ret = 0;
	static bool init = false;
	if (ss1SNormMat.empty() || !init)
	{
		ss1SNormPath = app_NormPath + "\\SS1SNormPath.jpg";
		ss1SNormMat = cv::imread(ss1SNormPath);

		if (ss1SNormMat.empty())
		{
			outInfo.push_back("请设置端封相机1参数");
			LogString(camID, "请设置端封相机1参数");

			LeaveCriticalSection(&_criticaSS1SP);
			return err_initpara;

		}

		ptextPro[camID]->setImgHSVNormP(ss1SNormMat);

		cv::Mat textNorm;
		ptextPro[camID]->imgHSVNorm(ss1SNormMat, textNorm);
		Mat   outcvt2BinColor;
		ppTopAlg[camID]->processThresholdtransform(ss1SNormMat, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)


		vector<vector<cv::Point> > polyVPoint;
		//vector<geometricFeatureItem> geometricFeature;
		double max_Area;
		ret = ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_Area);//top(16)
		if (polyVPoint.size() == 0 || ret < 0)
		{
			LogString(camID, "err polyVPoint.size()");
			setInfo("pointLineFeature", err_polyVPoint, outInfo, outInfoLOG);
			LeaveCriticalSection(&_criticaSS1SP);
			return err_polyVPoint;
		}
		ret = ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, true);
		if (ret != 0)
		{
			LogString(camID, "err getROImat");

			LeaveCriticalSection(&_criticaSS1SP);
			return err_getROImat;
		}
		Mat outimgtF;
		ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8


		cv::Point psize50(50, 50);
		cv::Point psize20(20, 20);

		cv::Point psize3(15, 15);
		cv::Point psize10(5, 5);

		vector<cv::Point> recccSize;
		recccSize.push_back(psize50);
		recccSize.push_back(psize20);
		recccSize.push_back(psize3);
		recccSize.push_back(psize10);

		ptextPro[camID]->setImgDensityNormP(outimgtF, recccSize, 1);
		setInfo("lsNorm:", camID, outInfo, outInfoLOG);

		init = true;
		LeaveCriticalSection(&_criticaSS1SP);

		return 0;

	}

	if (!init)
	{
		setInfo("init : false ", camID, outInfo, outInfoLOG);
		outInfo.push_back("请设置端封1相机参数");
		LogString(camID, "请设置端封1相机参数");

		LeaveCriticalSection(&_criticaSS1SP);
		return err_initpara;
	}


	cv::Mat textNorm;
	ptextPro[camID]->imgHSVNorm(inImg, textNorm);
	//	ptextPro[camID]->imgHSVNorm(inImg, outImg);
	Mat   outcvt2BinColor;
	ppTopAlg[camID]->processThresholdtransform(inImg, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)
	vector<vector<cv::Point> > polyVPoint;
	vector<geometricFeatureItem> geometricFeature;
	double max_Area;
	ret = ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_Area);//top(16)
	if (polyVPoint.size() == 0 || ret < 0)
	{
		LogString(camID, "err polyVPoint.size()");
		setInfo("pointLineFeature", err_polyVPoint, outInfo, outInfoLOG);
		LeaveCriticalSection(&_criticaSS1SP);
		return err_polyVPoint;
	}
	ret = ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, false);
	if (ret != 0)
	{
		LogString(camID, "err getROImat");
		LeaveCriticalSection(&_criticaSS1SP);
		return err_getROImat;
	}
	Mat outimgtF;
	ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8

	cv::Point psize50(50, 50);
	cv::Point psize20(20, 20);

	cv::Point psize3(15, 15);
	cv::Point psize10(5, 5);

	vector<cv::Point> recccSize;
	recccSize.push_back(psize50);
	recccSize.push_back(psize20);
	recccSize.push_back(psize3);
	recccSize.push_back(psize10);


	ptextPro[camID]->getImgDensity(outimgtF);
	ptextPro[camID]->pDFeatureCompare();


	ptextPro[camID]->getOriDensityFeature();
	ptextPro[camID]->getComperDensityFeature();


	vector<double>  score = ptextPro[camID]->getHashSiminal();

	double hashValue = 0.0;
	for (int i = 0; i < score.size(); i++)
	{
		char scoretmp[200] = { 0 };
		sprintf_s(scoretmp, "hash siminal:%f", score[i]);
		setInfo(scoretmp, camID, outInfo, outInfoLOG);
	}

	outImg = outimgtF;
	LeaveCriticalSection(&_criticaSS1SP);

	return 0;
}

int warpAlg::algSetParaProcess(algEnum algStep, int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG, bool resetPara)
{
	if (algStep == algEnum::algNULL)
	{
		setInfo("algStep err", -1, outInfo, outInfoLOG);
		return -1;
	}

	//return _algTopPSetPara(algStep, camID, para, inImg, outImg, outInfo);
	if (camID == CamIDType::CAM_TOP)			return _algTopPSetPara(algStep, CAM_TOP, para, inImg, outImg, outInfo, outInfoLOG);
	if (camID == CamIDType::CAM_LONGSIDE)		return _algLSPSetPara(algStep, CAM_LONGSIDE, para, inImg, outImg, outInfo, outInfoLOG, resetPara);
	if (camID == CamIDType::CAM_SHORTSIDE_0)	return _algSSP_0SetPara(algStep, CAM_SHORTSIDE_0, para, inImg, outImg, outInfo, outInfoLOG, resetPara);
	if (camID == CamIDType::CAM_SHORTSIDE_1)	return _algSSP_1SetPara(algStep, CAM_SHORTSIDE_1, para, inImg, outImg, outInfo, outInfoLOG, resetPara);

}
bool warpAlg::getMapValue(ParaKey parkey, double& value)
{
	auto m = p_MapParaKey->find(parkey);
	if (m != p_MapParaKey->end())
	{
		value = m->second;
		return true;
	}
	value = 0;
	return false;
}


void warpAlg::setInfo(string info, int ret, vector<string>& outInfo, vector<string>& outInfoLOG)
{
	char charTmp[100] = { 0 };
	itoa(ret, charTmp, 10);
	string infotmp = charTmp;
	info = info + ": " + infotmp;
	if (outInfo.size() != 0)outInfo.push_back(info);
	if (outInfoLOG.size() != 0)outInfoLOG.push_back(info);
}


int  warpAlg::_algTopPSetPara(algEnum algStep, int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG, bool resetPara)
{
	ptextPro[camID]->camIdinfo = camID;
	ppTopAlg[camID]->camIdinfo = camID;
	string info;
	double value = 0;
	char charTmp[200] = { 0 };

	int val0 = 50;
	if (para == NULL)
	{
		ParaKey parkey(camID, "二值化阈值", "阈值");
		if (getMapValue(parkey, value))val0 = value;
	}
	setInfo("二值化阈值", val0, outInfo, outInfoLOG);
	cv::Mat outcvt2BinColor;
	cv::Rect roiRect = cv::Rect(0, 0, inImg.cols, inImg.rows);
	ppTopAlg[camID]->processThresholdtransform(inImg, outImg, outcvt2BinColor, val0, 0, roiRect);
	if (algStep == topTopBV) return 0;


	double areaThreshole = 10;
	double areaLengthThv = 3;
	double totalAreaThv = 922496;
	if (para == NULL)
	{
		ParaKey parkeyArea(camID, "轮廓参数", "面积");
		if (getMapValue(parkeyArea, value))totalAreaThv = value;

		ParaKey parkeyEdgeArea(camID, "轮廓参数", "边缘面积");
		if (getMapValue(parkeyEdgeArea, value))areaThreshole = value;
		ParaKey parkeyEdgeAreaLength(camID, "轮廓参数", "面积/长度");
		if (getMapValue(parkeyEdgeAreaLength, value))areaLengthThv = value;

	}
	setInfo("面积", totalAreaThv, outInfo, outInfoLOG);
	setInfo("边缘面积", areaThreshole, outInfo, outInfoLOG);
	setInfo("面积/长度", areaLengthThv, outInfo, outInfoLOG);


	vector<vector<cv::Point> > polyVPoint;
	vector<geometricFeatureItem> geometricFeature;
	double maxArea = 0.0;
	ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 4, maxArea);
	sprintf_s(charTmp, "leave processapproxPolyDP _ maxArea = %f", maxArea);
	outInfo.push_back(charTmp);
	LogString(camID, charTmp);

	pointLineFeature  pLFeature;
	if (polyVPoint.size() == 0)
	{
		setInfo("pointLineFeature:", -4, outInfo, outInfoLOG);
		LogString(camID, "pointLineFeature:-4");
		return -4;
	}

	int ret = ppTopAlg[camID]->processPointLineFeature(outImg, polyVPoint.at(0), pLFeature, areaThreshole, areaLengthThv, totalAreaThv);
	setInfo("Check Out  ret:", ret, outInfo, outInfoLOG);
	return ret;
}
int  warpAlg::_algLSPSetPara(algEnum algStep, int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG, bool resetPara)
{
	ptextPro[camID]->camIdinfo = camID;
	ppTopAlg[camID]->camIdinfo = camID;
	if (algStep == lsNorm)	{

		cv::Mat textNorm, outcvt2BinColor, normMat;
		if (resetPara)
		{
			normMat = llSNormMat;
		}
		else
		{
			normMat = inImg;
		}

		ptextPro[camID]->setImgHSVNormP(normMat);
		ptextPro[camID]->imgHSVNorm(normMat, textNorm);
		ppTopAlg[camID]->processThresholdtransform(normMat, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)


		vector<vector<cv::Point> > polyVPoint;
		//vector<geometricFeatureItem> geometricFeature;
		double max_area = 0;
		ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_area);//top(16)

		ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, true);

		Mat outimgtF;
		ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8


		cv::Point psize50(50, 50);
		cv::Point psize20(20, 20);

		cv::Point psize3(15, 15);
		cv::Point psize10(5, 5);

		vector<cv::Point> recccSize;
		recccSize.push_back(psize50);
		recccSize.push_back(psize20);
		recccSize.push_back(psize3);
		recccSize.push_back(psize10);

		ptextPro[camID]->setImgDensityNormP(outimgtF, recccSize, 1);
		setInfo("lsNorm:", 0, outInfo, outInfoLOG);

		llSNormPath = app_NormPath + "\\llSNormPath.jpg";
		cv::imwrite(llSNormPath, inImg);
		llSNormMat = normMat;
		outImg = outimgtF;

		return 0;
	}

	if (algStep == lsOtherPara){

		if (llSNormMat.empty())
		{
			llSNormPath = app_NormPath + "\\llSNormPath.jpg";
			llSNormMat = cv::imread(llSNormPath);
			ptextPro[camID]->setImgHSVNormP(llSNormMat);

			cv::Mat textNorm;
			ptextPro[camID]->imgHSVNorm(llSNormMat, textNorm);
			Mat   outcvt2BinColor;
			ppTopAlg[camID]->processThresholdtransform(llSNormMat, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)


			vector<vector<cv::Point> > polyVPoint;
			//vector<geometricFeatureItem> geometricFeature;
			double max_area = 0;
			ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_area);//top(16)

			ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, true);

			Mat outimgtF;
			ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8


			cv::Point psize50(50, 50);
			cv::Point psize20(20, 20);

			cv::Point psize3(15, 15);
			cv::Point psize10(5, 5);

			vector<cv::Point> recccSize;
			recccSize.push_back(psize50);
			recccSize.push_back(psize20);
			recccSize.push_back(psize3);
			recccSize.push_back(psize10);

			ptextPro[camID]->setImgDensityNormP(outimgtF, recccSize, 1);
			setInfo("lsNorm:", 0, outInfo, outInfoLOG);

			//	llSNormPath = app_NormPath + "\\llSNormPath.jpg";
			//	cv::imwrite(llSNormPath, inImg);
			//	llSNormMat = inImg;
			//	outImg = outimgtF;

		}



		cv::Mat textNorm;
		ptextPro[camID]->imgHSVNorm(inImg, textNorm);
		//	ptextPro[camID]->imgHSVNorm(inImg, outImg);
		Mat   outcvt2BinColor;
		ppTopAlg[camID]->processThresholdtransform(inImg, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)
		vector<vector<cv::Point> > polyVPoint;
		vector<geometricFeatureItem> geometricFeature;
		double max_area = 0;
		ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_area);//top(16)

		ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, false);

		Mat outimgtF;
		ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8

		cv::Point psize50(50, 50);
		cv::Point psize20(20, 20);

		cv::Point psize3(15, 15);
		cv::Point psize10(5, 5);

		vector<cv::Point> recccSize;
		recccSize.push_back(psize50);
		recccSize.push_back(psize20);
		recccSize.push_back(psize3);
		recccSize.push_back(psize10);


		ptextPro[camID]->getImgDensity(outimgtF);
		ptextPro[camID]->pDFeatureCompare();


		ptextPro[camID]->getOriDensityFeature();
		ptextPro[camID]->getComperDensityFeature();


		vector<double>  score = ptextPro[camID]->getHashSiminal();
		cout << "/n" << endl;
		for (int i = 0; i < score.size(); i++)
		{
			//cout << "hash siminal :" << score[i] << endl;

			char scoretmp[200] = { 0 };
			sprintf_s(scoretmp, "hash siminal:%f", score[i]);
			setInfo(scoretmp, 0, outInfo, outInfoLOG);
		}

		setInfo("lsOtherPara", 0, outInfo, outInfoLOG);
		outImg = outimgtF;
		return 0;
	}



	return 0;
}
int  warpAlg::_algSSP_0SetPara(algEnum algStep, int camID, int* para[], const  cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG, bool resetPara)
{
	int ret = 0;
	ptextPro[camID]->camIdinfo = camID;
	ppTopAlg[camID]->camIdinfo = camID;
	if (algStep == lsNorm)	{
		cv::Mat textNorm, outcvt2BinColor, normMAt;
		if (resetPara == true)
		{
			normMAt = ss0SNormMat;
		}
		else
		{
			normMAt = inImg;
		}
		ptextPro[camID]->setImgHSVNormP(normMAt);
		ptextPro[camID]->imgHSVNorm(normMAt, textNorm);

		LogInstance()->logString(camID, "enterSetpara processThresholdtransform");

		ppTopAlg[camID]->processThresholdtransform(normMAt, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)


		vector<vector<cv::Point> > polyVPoint;
		//vector<geometricFeatureItem> geometricFeature;
		double max_area = 0;
		LogString(camID, "enter processapproxPolyDP");
		ret=ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_area);//top(16)
		if (ret != E_SUCEES)
		{
			return ret;
		}

		outInfoLOG.push_back("enterSetpara getROImat");
		LogString(camID, "enter  getROImat");

		ret=ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, true);
		if (ret != E_SUCEES)
		{
			return ret;
		}

		Mat outimgtF;
		ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8


		cv::Point psize50(50, 50);
		cv::Point psize20(20, 20);

		cv::Point psize3(15, 15);
		cv::Point psize10(5, 5);

		vector<cv::Point> recccSize;
		recccSize.push_back(psize50);
		recccSize.push_back(psize20);
		recccSize.push_back(psize3);
		recccSize.push_back(psize10);

		ptextPro[camID]->setImgDensityNormP(outimgtF, recccSize, 1);
		setInfo("lsNorm:", 0, outInfo, outInfoLOG);

		ss0SNormPath = app_NormPath + "\\SS0SNormPath.jpg";
		cv::imwrite(ss0SNormPath, inImg);
		ss0SNormMat = normMAt;
		outImg = outimgtF;

		return 0;
	}

	if (algStep == lsOtherPara){

		if (ss0SNormMat.empty())
		{
			ss0SNormPath = app_NormPath + "\\SS0SNormPath.jpg";
			ss0SNormMat = cv::imread(llSNormPath);
			ptextPro[camID]->setImgHSVNormP(ss0SNormMat);

			cv::Mat textNorm;
			ptextPro[camID]->imgHSVNorm(ss0SNormMat, textNorm);
			Mat   outcvt2BinColor;
			ppTopAlg[camID]->processThresholdtransform(ss0SNormMat, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)


			vector<vector<cv::Point> > polyVPoint;
			//vector<geometricFeatureItem> geometricFeature;
			double max_area = 0;
			ret=ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_area);//top(16)
			if (ret != E_SUCEES)
			{
				return ret;
			}
			ret=ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, true);
			if (ret != E_SUCEES)
			{
				return ret;
			}
			Mat outimgtF;
			ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8


			cv::Point psize50(50, 50);
			cv::Point psize20(20, 20);

			cv::Point psize3(15, 15);
			cv::Point psize10(5, 5);

			vector<cv::Point> recccSize;
			recccSize.push_back(psize50);
			recccSize.push_back(psize20);
			recccSize.push_back(psize3);
			recccSize.push_back(psize10);

			ptextPro[camID]->setImgDensityNormP(outimgtF, recccSize, 1);
			setInfo("lsNorm:", camID, outInfo, outInfoLOG);

			//	llSNormPath = app_NormPath + "\\llSNormPath.jpg";
			//	cv::imwrite(llSNormPath, inImg);
			//	llSNormMat = inImg;
			//	outImg = outimgtF;

		}



		cv::Mat textNorm;
		ptextPro[camID]->imgHSVNorm(inImg, textNorm);
		//	ptextPro[camID]->imgHSVNorm(inImg, outImg);
		Mat   outcvt2BinColor;
		ppTopAlg[camID]->processThresholdtransform(inImg, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)
		vector<vector<cv::Point> > polyVPoint;
		vector<geometricFeatureItem> geometricFeature;
		double max_area = 0;
		ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_area);//top(16)

		ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, false);

		Mat outimgtF;
		ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8

		cv::Point psize50(50, 50);
		cv::Point psize20(20, 20);

		cv::Point psize3(15, 15);
		cv::Point psize10(5, 5);

		vector<cv::Point> recccSize;
		recccSize.push_back(psize50);
		recccSize.push_back(psize20);
		recccSize.push_back(psize3);
		recccSize.push_back(psize10);


		ptextPro[camID]->getImgDensity(outimgtF);
		ptextPro[camID]->pDFeatureCompare();


		ptextPro[camID]->getOriDensityFeature();
		ptextPro[camID]->getComperDensityFeature();


		vector<double>  score = ptextPro[camID]->getHashSiminal();
		cout << "/n" << endl;
		for (int i = 0; i < score.size(); i++)
		{
			//cout << "hash siminal :" << score[i] << endl;

			char scoretmp[200] = { 0 };
			sprintf_s(scoretmp, "hash siminal:%f", score[i]);
			setInfo(scoretmp, camID, outInfo, outInfoLOG);
		}

		setInfo("lsOtherPara", camID, outInfo, outInfoLOG);
		outImg = outimgtF;
		return 0;
	}



	return 0;
}
int  warpAlg::_algSSP_1SetPara(algEnum algStep, int camID, int* para[], const cv::Mat inImg, cv::Mat& outImg, vector<string>& outInfo, vector<string>& outInfoLOG, bool resetPara)
{
	ptextPro[camID]->camIdinfo = camID;
	ppTopAlg[camID]->camIdinfo = camID;
	if (algStep == lsNorm)	{
		cv::Mat textNorm, outcvt2BinColor, NormMat;
		if (resetPara)
		{
			NormMat = ss1SNormMat;
		}
		else
		{
			NormMat = inImg;
		}
		ptextPro[camID]->setImgHSVNormP(NormMat);

		ptextPro[camID]->imgHSVNorm(NormMat, textNorm);

		LogString(camID, "enter processThresholdtransform");
		ppTopAlg[camID]->processThresholdtransform(NormMat, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)


		vector<vector<cv::Point> > polyVPoint;
		//vector<geometricFeatureItem> geometricFeature;
		double max_area = 0;
		LogString(camID, "enter processapproxPolyDP");
		ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_area);//top(16)
		LogString(camID, "enter getROImat");


		ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, true);

		Mat outimgtF;
		ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8


		cv::Point psize50(50, 50);
		cv::Point psize20(20, 20);

		cv::Point psize3(15, 15);
		cv::Point psize10(5, 5);

		vector<cv::Point> recccSize;
		recccSize.push_back(psize50);
		recccSize.push_back(psize20);
		recccSize.push_back(psize3);
		recccSize.push_back(psize10);

		ptextPro[camID]->setImgDensityNormP(outimgtF, recccSize, 1);
		setInfo("lsNorm:", 0, outInfo, outInfoLOG);

		ss1SNormPath = app_NormPath + "\\SS1SNormPath.jpg";
		cv::imwrite(ss1SNormPath, inImg);
		ss0SNormMat = NormMat;
		outImg = outimgtF;

		return 0;
	}

	if (algStep == lsOtherPara){

		if (ss1SNormMat.empty())
		{
			ss1SNormPath = app_NormPath + "\\SS0SNormPath.jpg";
			ss1SNormMat = cv::imread(ss1SNormPath);
			ptextPro[camID]->setImgHSVNormP(ss1SNormMat);

			cv::Mat textNorm;
			ptextPro[camID]->imgHSVNorm(ss1SNormMat, textNorm);
			Mat   outcvt2BinColor;
			ppTopAlg[camID]->processThresholdtransform(ss1SNormMat, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)


			vector<vector<cv::Point> > polyVPoint;
			//vector<geometricFeatureItem> geometricFeature;
			double max_area = 0;
			ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_area);//top(16)

			ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, true);

			Mat outimgtF;
			ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8


			cv::Point psize50(50, 50);
			cv::Point psize20(20, 20);

			cv::Point psize3(15, 15);
			cv::Point psize10(5, 5);

			vector<cv::Point> recccSize;
			recccSize.push_back(psize50);
			recccSize.push_back(psize20);
			recccSize.push_back(psize3);
			recccSize.push_back(psize10);

			ptextPro[camID]->setImgDensityNormP(outimgtF, recccSize, 1);
			setInfo("lsNorm:", camID, outInfo, outInfoLOG);

			//	llSNormPath = app_NormPath + "\\llSNormPath.jpg";
			//	cv::imwrite(llSNormPath, inImg);
			//	llSNormMat = inImg;
			//	outImg = outimgtF;

		}



		cv::Mat textNorm;
		ptextPro[camID]->imgHSVNorm(inImg, textNorm);
		//	ptextPro[camID]->imgHSVNorm(inImg, outImg);
		Mat   outcvt2BinColor;
		ppTopAlg[camID]->processThresholdtransform(inImg, outImg, outcvt2BinColor, 21, 0);// option 1,thresh 为奇数  top(51,1),left(81,1)
		vector<vector<cv::Point> > polyVPoint;
		vector<geometricFeatureItem> geometricFeature;
		double max_area = 0;
		ppTopAlg[camID]->processapproxPolyDP(outImg, outImg, polyVPoint, 1, max_area);//top(16)

		ptextPro[camID]->getROImat(textNorm, outImg, polyVPoint, false);

		Mat outimgtF;
		ptextPro[camID]->imgFileter(outImg, outimgtF, 5, 0);//8

		cv::Point psize50(50, 50);
		cv::Point psize20(20, 20);

		cv::Point psize3(15, 15);
		cv::Point psize10(5, 5);

		vector<cv::Point> recccSize;
		recccSize.push_back(psize50);
		recccSize.push_back(psize20);
		recccSize.push_back(psize3);
		recccSize.push_back(psize10);


		ptextPro[camID]->getImgDensity(outimgtF);
		ptextPro[camID]->pDFeatureCompare();


		ptextPro[camID]->getOriDensityFeature();
		ptextPro[camID]->getComperDensityFeature();


		vector<double>  score = ptextPro[camID]->getHashSiminal();
		cout << "/n" << endl;
		for (int i = 0; i < score.size(); i++)
		{
			//cout << "hash siminal :" << score[i] << endl;

			char scoretmp[200] = { 0 };
			sprintf_s(scoretmp, "hash siminal:%f", score[i]);
			setInfo(scoretmp, camID, outInfo, outInfoLOG);
		}

		setInfo("lsOtherPara", camID, outInfo, outInfoLOG);
		outImg = outimgtF;
		return 0;
	}



	return 0;

}
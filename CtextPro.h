#pragma once
#include "global.h"

class imgregFeaturebase
{
public:
	double  m_density;
	cv::Point   m_imgRectPoint;
	cv::Rect    m_oriReg;
};

class imgregFeature
{
public:
	imgregFeaturebase m_regMaxDensity;
	cv::Point m_orImgTable;
};



class CtextPro
{
public:
	int camIdinfo;
public:
	CtextPro();
	~CtextPro();
	cv::Point    m_oriImgSize;
	cv::Point    m_oriImgDensity;
		 
	vector<float> m_imgNormMeanP;
	vector<vector<Mat>>  m_imgDensityPNorm;//密度标准

	
	vector<vector<Mat>>  m_imgDensity;//存储图片密度vector: vector<vector<Mat>> 

	vector<vector<Mat>>  m_imgDensityComperFeature;//比对结果

	vector<imgregFeature> m_ComperDensityFeature;
	vector<imgregFeature> m_OriDensityFeature;

private:
	int     m_oneDensityPCount;//default 3, ？？？
	
	vector<string>  m_hashresStr;
	vector<string>  m_hashresStrNorm;
	vector<double>  m_hashSiminal;
public:

	int  desityArr[200][200];

	vector<cv::Point> m_desitytableRC;//全局变量，存储参数，存储设置密度vector量，eg.【Point psize20(50, 50);vector<Point> recccSize;】

	cv::Point        ROImatTemplate; //中心点，向上向下ROWS 单边= ROImatTemplate.y ,中心点，向左向右COlS 单边 = ROImatTemplate.x
public:

	//void creatMemoey(void* p,int blockSize);
	//void releaseMemoey(void* p, int blockSize);


	void setImgHSVNormP(const Mat inImg);
	void imgHSVNorm(const Mat inImg, Mat& outImg);
	void imgFileter(const Mat inImg, Mat& outImg, int fileterpra, int filterType);

	void setImgDensityNormP(const Mat inImg, vector <cv::Point> rectSize, int oneDensityPCount);
	void getImgDensity(const Mat inImg);

	//in:inImg;in:rectSize ,inImg被均分成多少方格; in:typeP ,0 标准处理，1 判断处理
	void getImgDensityTable(const Mat inImg, vector <cv::Point>rectSize, int typeP);

	void getCImgFSaveDensity(const Mat inImg, cv::Point rectSize, vector<Mat>& outImg);
	void getDensityCore(const Mat inImg, cv::Point rectSize, Mat& outImg);

	inline int twoPointsDistance(cv::Point point1, cv::Point point2);
	int  getMinDispoint(cv::Point sourcePoint, cv::Point& minDPoint, vector<cv::Point> pointVector);
	int  getXYmaxmin(int*  array, vector<cv::Point> pointVector);
	int  getROImat(const Mat img, Mat& outImg, const vector<vector<cv::Point>> polyVPoint, bool isTemplate);


	int  imgDensityCompare(vector<Mat>& imgNorm, vector<Mat>& src, int comperType = 0);
	//int  imgDensityComper(vector<Mat>& src1, vector<Mat>& src2, int comperType = 0);
	int  pImgDensityCompare(vector<Mat> src);
	int  pDFeatureCompare();

	int  getComperDensityFeature();
	int  getOriDensityFeature();
	int  densityMaxROI(const Mat src, imgregFeature& iFeature);
	
public:
	vector<string> getHashV(vector<Mat> inCImg);
	vector<double> getHashSiminal();

private:
	Mat   m_ColorNormMat;
};


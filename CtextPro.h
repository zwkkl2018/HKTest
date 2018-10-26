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
	vector<vector<Mat>>  m_imgDensityPNorm;//�ܶȱ�׼

	
	vector<vector<Mat>>  m_imgDensity;//�洢ͼƬ�ܶ�vector: vector<vector<Mat>> 

	vector<vector<Mat>>  m_imgDensityComperFeature;//�ȶԽ��

	vector<imgregFeature> m_ComperDensityFeature;
	vector<imgregFeature> m_OriDensityFeature;

private:
	int     m_oneDensityPCount;//default 3, ������
	
	vector<string>  m_hashresStr;
	vector<string>  m_hashresStrNorm;
	vector<double>  m_hashSiminal;
public:

	int  desityArr[200][200];

	vector<cv::Point> m_desitytableRC;//ȫ�ֱ������洢�������洢�����ܶ�vector����eg.��Point psize20(50, 50);vector<Point> recccSize;��

	cv::Point        ROImatTemplate; //���ĵ㣬��������ROWS ����= ROImatTemplate.y ,���ĵ㣬��������COlS ���� = ROImatTemplate.x
public:

	//void creatMemoey(void* p,int blockSize);
	//void releaseMemoey(void* p, int blockSize);


	void setImgHSVNormP(const Mat inImg);
	void imgHSVNorm(const Mat inImg, Mat& outImg);
	void imgFileter(const Mat inImg, Mat& outImg, int fileterpra, int filterType);

	void setImgDensityNormP(const Mat inImg, vector <cv::Point> rectSize, int oneDensityPCount);
	void getImgDensity(const Mat inImg);

	//in:inImg;in:rectSize ,inImg�����ֳɶ��ٷ���; in:typeP ,0 ��׼����1 �жϴ���
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


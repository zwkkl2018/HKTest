#include "stdafx.h"
#include "CtextPro.h"
#include <algorithm>
#include <functional>

CtextPro::CtextPro()
{
	memset(desityArr, 0, sizeof(desityArr));
	m_imgDensity.clear();
	m_imgDensityPNorm.clear();
	m_desitytableRC.clear();


	m_imgDensityComperFeature.clear();
	m_ComperDensityFeature.clear();
	m_OriDensityFeature.clear();
	m_oneDensityPCount = 3;
//	m_avghash2Simal = true;
	m_oriImgSize = cv::Point(0, 0);
	m_oriImgDensity = cv::Point(0, 0);
	camIdinfo = -1;

}


CtextPro::~CtextPro()
{
	m_imgDensity.clear();
	m_imgDensityPNorm.clear();
	m_desitytableRC.clear();


	m_imgDensityComperFeature.clear();
	m_ComperDensityFeature.clear();
	m_OriDensityFeature.clear();
}

void CtextPro::setImgHSVNormP(const Mat inImg)
{

	m_oriImgSize.x = inImg.cols;
	m_oriImgSize.y = inImg.rows;


	std::vector<cv::Mat> channels;

	float meanff = 0.0;
	Scalar tempVal;
	cv::split(inImg, channels);
	// channels[0] is the Hue
	// channels[1] is the Saturation
	// channels[2] is the Value

	for (int iindex = 0; iindex < 3; iindex++)
	{
		tempVal = cv::mean(channels[iindex]);
		m_imgNormMeanP.push_back(tempVal.val[0]);
	}
	m_ColorNormMat = inImg;

}
void CtextPro::imgHSVNorm(const Mat inImg, Mat& outImg)
{
	//cv::imwrite("inImg.jpg", inImg);
	std::vector<cv::Mat> channels;
	//std::vector<float>   imgNormMeanP;
	float par = 0.0;
	float meanff = 0.0;
	Scalar tempVal;
	split(inImg, channels);
	// channels[0] is the Hue
	// channels[1] is the Saturation
	// channels[2] is the Value
	for (int iindex = 0; iindex < 3; iindex++)
	{
		tempVal = cv::mean(channels[iindex]);
		par = tempVal.val[0];
		channels[iindex] = (m_imgNormMeanP.at(iindex) / par)* channels[iindex];
	}
	merge(channels, outImg);
	 

}
void CtextPro::imgFileter(const Mat inImg, Mat& outImg, int fileterpra, int filterType = 0)
{
	if (filterType == 0)
	{
		bilateralFilter(inImg, outImg, fileterpra, fileterpra * 2, fileterpra / 2);
		//GaussianBlur(inImg, outImg, Size(3,3), 1.5);
	}
	//	imshow("GaussianBlur", outImg);
	cvtColor(outImg, outImg, COLOR_BGR2GRAY);
	//Canny(outImg, outImg, 80, 120);
	//	imshow("cvtColor", outImg);
	Canny(outImg, outImg, 80, 120);

}

void CtextPro::setImgDensityNormP(const Mat inImg, vector <cv::Point>rectSize, int oneDensityPCount)
{

	m_imgDensityPNorm.clear();
	m_desitytableRC = rectSize;
	m_oneDensityPCount = oneDensityPCount;
	getImgDensityTable(inImg, m_desitytableRC, 0);
#if 0

	for (int ii = 0; ii < m_imgDensityPNorm.size(); ii++)
	{
		for (int jj = 0; jj < m_imgDensityPNorm.at(ii).size(); jj++)
		{
			if (jj == 0)
			{
				//imshow("m_imgDensityP0", m_imgDensityPNorm.at(ii).at(jj));
			}
			if (jj == 1)
			{
				//imshow("m_imgDensityP1", m_imgDensityPNorm.at(ii).at(jj));
			}
			if (jj == 2)
			{
				//imshow("m_imgDensityP2", m_imgDensityPNorm.at(ii).at(jj));
			}
		}
	}
#endif

}
void CtextPro::getImgDensity(const Mat inImg)
{
	m_oriImgDensity.x = inImg.cols;
	m_oriImgDensity.y = inImg.rows;

	m_imgDensity.clear();
	getImgDensityTable(inImg, m_desitytableRC, 1);
#if 0

	for (int ii = 0; ii < m_imgDensity.size(); ii++)
	{
		for (int jj = 0; jj < m_imgDensity.at(ii).size(); jj++)
		{
			if (jj == 0)
			{
				//imshow("m_imgDensityP0", m_imgDensity.at(ii).at(jj));
				//imwrite("m_imgDensityP0.bmp", m_imgDensity.at(ii).at(jj));
			}
			if (jj == 1)
			{
				//imshow("m_imgDensityP1", m_imgDensity.at(ii).at(jj));
				//imwrite("m_imgDensityP1.bmp", m_imgDensity.at(ii).at(jj));
			}
			if (jj == 2)
			{
				//imshow("m_imgDensityP2", m_imgDensity.at(ii).at(jj));
				//imwrite("m_imgDensityP2.bmp", m_imgDensity.at(ii).at(jj));
			}
		}
	}
#endif
}

void  CtextPro::getImgDensityTable(const Mat inImg, vector <cv::Point>rectSize, int typeP)
{
	if (typeP == 1)//m_imgDensity
	{
		m_hashresStr.clear();
	}
	if (typeP==0)
	{
		m_hashresStrNorm.clear();
	}

	for (int rectSizeindex = 0; rectSizeindex < rectSize.size(); rectSizeindex++) //eg.【50,50】，【40,40】，【10,10】
	{
		vector<Mat>   imgDensityP;
		imgDensityP.clear();

		for (int matIndex = 0; matIndex < m_oneDensityPCount; matIndex++)//err, eg.[50,50]-->top_left, top_left+left+1/2,top_left+top+1/2: m_oneDensityPCount default =3
		{
			Mat tmpImg;
			tmpImg.create(m_desitytableRC.at(rectSizeindex), CV_8UC1);
			imgDensityP.push_back(tmpImg);

			if (typeP == 0) //m_imgDensityPNorm
			{
				//m_imgDensityPNorm.clear();
				m_imgDensityPNorm.push_back(imgDensityP);
				getCImgFSaveDensity(inImg, m_desitytableRC.at(rectSizeindex), m_imgDensityPNorm.at(rectSizeindex));
				 
				m_hashresStrNorm.push_back(getHashV(m_imgDensityPNorm.at(rectSizeindex))[0]);

			}
			if (typeP == 1)//m_imgDensity
			{

				m_imgDensity.push_back(imgDensityP);
				getCImgFSaveDensity(inImg, m_desitytableRC.at(rectSizeindex), m_imgDensity.at(rectSizeindex));
				
				m_hashresStr.push_back(getHashV(m_imgDensity.at(rectSizeindex))[0]);
			}
		}
	}

}
vector<string> CtextPro::getHashV(vector<Mat> inCImg)
{
	vector<string> vString;
	for (int indexx = 0; indexx < inCImg.size(); indexx++)
	{
		Mat tmpImg = inCImg[indexx];	

		string resStr(tmpImg.rows*tmpImg.cols, '\0');
		int index = 0;
		double avg = mean(tmpImg)[0];

		for (int rw = 0; rw < tmpImg.rows; rw++)
		{
			uchar *data = (uchar*)tmpImg.ptr<uchar>(rw);
			for (int coll = 0; coll < tmpImg.cols; coll++)
			{
				if (data[coll] > avg)
				{
					resStr[index++] = '1';
				}
				else
				{
					resStr[index++] = '0';
				}
			}
		}	
		vString.push_back(resStr);
	}
	return vString;
}

vector<double> CtextPro::getHashSiminal()
{
	vector<double> simiall;
	
	for (int index = 0; index < m_hashresStrNorm.size(); index++)
	{
		double similarity = 1.0;
		int stringLen = m_hashresStrNorm[index].size();
		for (int i = 0; i <stringLen; i++)
		{
			char c1 = m_hashresStrNorm[index][i];
			char c2 = m_hashresStr[index][i];
			if (c1 != c2)
				similarity = similarity - 1.0 / stringLen;
		}
		simiall.push_back(similarity);
	}
	return simiall;
}

void CtextPro::getCImgFSaveDensity(const Mat inImg, cv::Point rectSize, vector<Mat>& outImg)
{

	int xgap = inImg.cols / rectSize.x;
	int ygap = inImg.rows / rectSize.y;
	Mat imgg;
	for (int indexx = 0; indexx < outImg.size(); indexx++)
	{
		if (indexx == 0)
		{
			imgg = inImg;
		}
		if (indexx == 1)
		{
			imgg = inImg(cv::Rect(0, ygap / 2, inImg.cols, inImg.rows - ygap / 2));

		}
		if (indexx == 2)
		{
			imgg = inImg(cv::Rect(xgap / 2, 0, inImg.cols - xgap / 2, inImg.rows - ygap / 2));
		}
		getDensityCore(imgg, rectSize, outImg.at(indexx));

#if 0
		if (indexx == 0)
		{
			//imshow("getDensity_0", outImg.at(indexx));
		}
		if (indexx == 1)
		{
			//imshow("getDensity_1", outImg.at(indexx));
		}
		if (indexx == 2)
		{
			//imshow("getDensity_2", outImg.at(indexx));
		}
#endif

	}
}
void CtextPro::getDensityCore(const Mat inImg, cv::Point rectSize, Mat& outImg)
{
	memset(desityArr, 0, sizeof(desityArr));
	int xgap = inImg.cols / rectSize.x;
	int ygap = inImg.rows / rectSize.y;

	for (int indexRows = 0; indexRows < inImg.rows; indexRows++) // 行循环
	{
		uchar *data = (uchar*)inImg.ptr<uchar>(indexRows);
		for (int indexCols = 0; indexCols < inImg.cols; indexCols++) //列循环
		{
			int m = indexCols / xgap;
			int n = indexRows / ygap;
			desityArr[n][m] += data[indexCols];
		}
	}

	for (int rw = 0; rw < rectSize.y; rw++)
	{
		uchar *data = (uchar*)outImg.ptr<uchar>(rw);
		for (int coll = 0; coll < rectSize.x; coll++)
		{
			//desityArr[rw][coll] = desityArr[rw][coll] / (rectSize.x*rectSize.y);
			desityArr[rw][coll] = desityArr[rw][coll] / (xgap*ygap);

			data[coll] = desityArr[rw][coll];
		}
	}

}



inline int CtextPro::twoPointsDistance(cv::Point point1, cv::Point point2)
{
	return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
}

int  CtextPro::getMinDispoint(cv::Point sourcePoint, cv::Point& minDPoint, vector<cv::Point> pointVector)
{
	double minD = INFINITE;
	double temD = INFINITE;

	for (int i = 0; i < pointVector.size(); i++)
	{
		temD = twoPointsDistance(sourcePoint, pointVector.at(i));
		if (temD < minD)
		{
			minD = temD;
			minDPoint = pointVector.at(i);
		}
	}
	return E_SUCEES;
}

int  CtextPro::getXYmaxmin(int*  array, vector<cv::Point> pointVector)
{
	int xMin, yMin, xMax, yMax;
	xMin = yMin = INFINITE;
	xMax = yMax = -999;

	vector<int> xtmpVector, ytmpVector;
	for (int i = 0; i < pointVector.size(); i++)
	{
		xtmpVector.push_back(pointVector.at(i).x);
		ytmpVector.push_back(pointVector.at(i).y);
	}
	sort(xtmpVector.begin(), xtmpVector.end());
	sort(ytmpVector.begin(), ytmpVector.end());

	*array++ = xtmpVector.at(0);
	*array++ = xtmpVector.at(pointVector.size() - 1);

	*array++ = ytmpVector.at(0);
	*array = ytmpVector.at(pointVector.size() - 1);

	return E_SUCEES;

}
int  CtextPro::getROImat(const Mat img, Mat& outImg, const vector<vector<cv::Point>> polyVPoint, bool isTemplate)
{

	vector<cv::Point>points;
	for (int ii = 0; ii < polyVPoint.size(); ii++)
	{
		for (int jj = 0; jj < polyVPoint.at(ii).size(); jj++)
		{
			points.push_back(polyVPoint.at(ii).at(jj));
		}
	}
	int arry[4] = { 0 };
	 
	LogInstance()->logString(camIdinfo, "enter getXYmaxmin");
	getXYmaxmin(arry, points);
	cv::Point centre(0, 0);
	centre.x = arry[0] + (arry[1] - arry[0]) / 2;
	centre.y = arry[2] + (arry[3] - arry[2]) / 2;

	if (true == isTemplate)
	{
		ROImatTemplate.y = ((arry[3] - arry[2]) / 2)*0.8;
		ROImatTemplate.x = ((arry[1] - arry[0]) / 2)*0.8;
	}
	//else if (false == isTemplate)
	//{
	//	
	//	//if (mtype == 1)
	//	//{
	//	//	int imgHight = img.rows;
	//	//	int imgW = img.cols;
	//	//	Point2f imgV[4];

	//	//	imgV[0] = Point2f(0, 0);
	//	//	imgV[1] = Point2f(0, imgHight);
	//	//	imgV[2] = Point2f(imgW, imgHight);
	//	//	imgV[3] = Point2f(imgW, 0);
	//	//	Point leftup, leftdown, rightdown, rightup;
	//	//	getMinDispoint(imgV[0], leftup, points);
	//	//	getMinDispoint(imgV[1], leftdown, points);
	//	//	getMinDispoint(imgV[2], rightdown, points);
	//	//	getMinDispoint(imgV[3], rightup, points);
	//	//	//Mat roi = img(Rect(500, 200, 100, 300));//Rect四个形参分别是：x坐标，y坐标，长，高；注意(x,y)指的是矩形的左上角点
	//	//	outImg = img(Rect(leftup.x, leftup.y, rightdown.x - leftdown.x, rightdown.y - rightup.y));
	//	//}
	//	//else
	//	//{

	//	

	//	outImg = img(Rect(arry[0], arry[2], arry[1] - arry[0], arry[3] - arry[2]));

	//}
	char chartmp[256] = {0};
	int tmpp = (ROImatTemplate.y * 2 / 4) * 4+4;
	int tmppp = (ROImatTemplate.x * 2 / 4) * 4 + 4;

	sprintf_s(chartmp, "img: %d*%d  rect: %d,%d,%d,%d",img.cols,img.rows, centre.x - ROImatTemplate.x, centre.y - ROImatTemplate.y, tmppp, tmpp);
	LogInstance()->logString(camIdinfo, chartmp);
	LogInstance()->logString(camIdinfo, "enter outImg =img(cv::rect())");

	if (img.cols<0|| img.rows<0|| centre.x - ROImatTemplate.x<0||centre.y - ROImatTemplate.y<0|| tmppp<0||tmpp<0)
	{
		return E_beyongMat;
	}
 
	if (centre.x - ROImatTemplate.x + tmppp>img.cols || centre.y - ROImatTemplate.y + tmpp>img.rows || centre.x - ROImatTemplate.x<0 || centre.y - ROImatTemplate.y<0)
	{
		return E_beyongMat;
	}
	outImg = img(cv::Rect(centre.x - ROImatTemplate.x, centre.y - ROImatTemplate.y, tmppp, tmpp));

	return E_SUCEES;
	//
	//Point2f tr[4];
	//	box.points(tr);
	//	RotatedRect box = minAreaRect(Mat(points));//点集的最小外接旋转矩形
	//for (int vindex = 0; vindex < 4; vindex++)
	//{
	//	for (int pindex = 0; pindex < 4; pindex++)
	//	{

	//	}
	//}




	//for (int i = 0; i < 4; i++)
	//{
	//	//line(outimg, tr[i], tr[(i + 1) % 4], Scalar(0, 255, 0), 1, CV_AA);
	//}
}

int  CtextPro::imgDensityCompare(vector<Mat>& imgNorm, vector<Mat>& src, int comperType)
{
	int comperC = min(imgNorm.size(), src.size());
	vector<Mat> tmppVec;
	int dataaa = 0;
	for (int indexx = 0; indexx < comperC; indexx++)
	{
		Mat tmp = src.at(indexx);// abs(imgNorm.at(indexx) - src.at(indexx));
		//imshow("NormC", imgNorm.at(indexx));
		for (int rowindex = 0; rowindex < tmp.rows; rowindex++)
		{
			uchar *dataNorm = (uchar*)imgNorm.at(indexx).ptr<uchar>(rowindex);
			uchar *dataSrc = (uchar*)tmp.ptr<uchar>(rowindex);

			for (int colindex = 0; colindex < tmp.cols; colindex++)
			{
				dataaa = dataSrc[colindex] - dataNorm[colindex];
				//dataSrc[colindex] = dataaa < 0 ? 0 : dataaa;	
				dataSrc[colindex] = abs(dataaa);// < 0 ? 0 : dataaa;

			}
		}

		tmppVec.push_back(tmp);
		//dst->push_back(tmp);
		//dst.push_back(tmp);
	}
	m_imgDensityComperFeature.push_back(tmppVec);
	return 0;
}
int ccc = 0;
int  CtextPro::pImgDensityCompare(vector<Mat> src)
{
	char windowname[20];
	ccc++;
	for (int i = 0; i < src.size(); i++)
	{
		sprintf(windowname, "%d_C_%d", ccc, i);
		//imshow(windowname, src.at(i));
	}

	return 0;
}

int  CtextPro::pDFeatureCompare()
{
	m_imgDensityComperFeature.clear();
	m_ComperDensityFeature.clear();

	int comperC = min(m_imgDensityPNorm.size(), m_imgDensity.size());
	for (int i = 0; i < comperC; i++)
	{
		imgDensityCompare(m_imgDensityPNorm.at(i), m_imgDensity.at(i), 0);
	}
	ccc = 0;
	for (int ii = 0; ii < m_imgDensityComperFeature.size(); ii++)
	{
		pImgDensityCompare(m_imgDensityComperFeature.at(ii));
	}
	return 0;

}
int  CtextPro::densityMaxROI(const Mat src, imgregFeature& iFeature)
{
	int Va = 0;
	//imwrite("densityFeatureP.bmp", src);
	//imshow("densityFeatureP.bmp", src);
	for (int indexRows = 0; indexRows < src.rows; indexRows++) // 行循环
	{
		uchar *data = (uchar*)src.ptr<uchar>(indexRows);
		for (int indexCols = 0; indexCols < src.cols; indexCols++) //列循环
		{
			Va = data[indexCols];
			if (Va > iFeature.m_regMaxDensity.m_density)
			{
				iFeature.m_regMaxDensity.m_density = data[indexCols];
				iFeature.m_regMaxDensity.m_imgRectPoint = cv::Point(indexRows, indexCols);
			}
		}
	}

	return 0;
}
int  CtextPro::getComperDensityFeature()
{
	m_ComperDensityFeature.clear();
	for (int iii = 0; iii < m_imgDensityComperFeature.size(); iii++)
	{
		cv::Point sss = m_desitytableRC.at(iii);
		for (int indexx = 0; indexx < m_imgDensityComperFeature.at(iii).size(); indexx++)
		{
			imgregFeature tmpimgregFeature;

			densityMaxROI(m_imgDensityComperFeature.at(iii).at(indexx), tmpimgregFeature);
			tmpimgregFeature.m_orImgTable = sss;



			tmpimgregFeature.m_regMaxDensity.m_oriReg.x = (m_oriImgDensity.x / sss.x)*tmpimgregFeature.m_regMaxDensity.m_imgRectPoint.x;
			tmpimgregFeature.m_regMaxDensity.m_oriReg.y = (m_oriImgDensity.y / sss.y)*tmpimgregFeature.m_regMaxDensity.m_imgRectPoint.y;

			tmpimgregFeature.m_regMaxDensity.m_oriReg.width = m_oriImgDensity.x / sss.x;
			tmpimgregFeature.m_regMaxDensity.m_oriReg.height = m_oriImgDensity.y / sss.y;
			m_ComperDensityFeature.push_back(tmpimgregFeature);
		}

	}
	return 0;
}
int  CtextPro::getOriDensityFeature()
{
	m_OriDensityFeature.clear();
	for (int iii = 0; iii < m_imgDensity.size(); iii++)
	{
		cv::Point sss = m_desitytableRC.at(iii);
		for (int indexx = 0; indexx < m_imgDensity.at(iii).size(); indexx++)
		{
			imgregFeature tmpimgregFeature;

			densityMaxROI(m_imgDensity.at(iii).at(indexx), tmpimgregFeature);


			tmpimgregFeature.m_orImgTable = sss;



			int k = tmpimgregFeature.m_regMaxDensity.m_oriReg.x = (m_oriImgDensity.x / sss.x)*tmpimgregFeature.m_regMaxDensity.m_imgRectPoint.x;
			int n = tmpimgregFeature.m_regMaxDensity.m_oriReg.y = (m_oriImgDensity.y / sss.y)*tmpimgregFeature.m_regMaxDensity.m_imgRectPoint.y;

			int z = tmpimgregFeature.m_regMaxDensity.m_oriReg.width = m_oriImgDensity.x / sss.x;
			int m = tmpimgregFeature.m_regMaxDensity.m_oriReg.height = m_oriImgDensity.y / sss.y;
			//TRACE("x = %d and y = %d\n", x, y); x是列，y是行
			//printf("tmpimgregFeature. %d,  %d, %d, %d\n",k,n,z,m );
			//printf("tmpimgregFeature..m_density %f,  %d, %d\n", tmpimgregFeature.m_regMaxDensity.m_density, tmpimgregFeature.m_regMaxDensity.m_imgRectPoint.x, tmpimgregFeature.m_regMaxDensity.m_imgRectPoint.y);
			m_OriDensityFeature.push_back(tmpimgregFeature);
		}

	}
	return 0;
}



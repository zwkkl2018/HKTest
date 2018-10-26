

#pragma  once
#include "global.h"

class pointLineFeatureBase
{
public:
	class pairPoint
	{
	public:
		cv::Point point1;
		cv::Point point2;
		double lineFunctionB;
		double slope;
		double length;


		cv::Point crossPoint2maxlengthPair;
		cv::Point project2maxlengthPair;

		int indexInLinePointsets;//index In LinePointsets  与index In  projection2MlL一样

		//vector<Point> LinePointsets;
		//vector<Point> projection2MlL;
	};
	class pairPointZone
	{
	public:
		vector<pairPoint> pairsPoint;
		double area;
		bool isthisLineOut;//在本线的内侧还是外侧， 外侧
	};

	pointLineFeatureBase()
	{
		LinePointsets.clear();
		slopeForMaxLengthline = 0.0;
		percentForMaxLengthline = 0.0;
		maxlengthIndex = 0;
		lineFunctionB = 0.0;

		area = 0.0;
		thv = 0.0;
		length = 0;
		tDtceThTotalLPairpoint = 0.0;
		totalErrarea = 0.0;
	}
	~pointLineFeatureBase()
	{
		LinePointsets.clear();
	}
public:
	inline int twoPointsDistance(cv::Point point1, cv::Point point2)
	{
		return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
	}
	void getLineFeature(vector<cv::Point>& lPointsets, int linetype)
	{
		if (lPointsets.size() < 2)
		{
			return;
		}
		int totalLength = 0;
		int maxLength = 0;

		for (int pointIndex = 0; pointIndex < lPointsets.size() - 1; pointIndex++)
		{
			pairPoint tmppair;

			tmppair.point1 = lPointsets.at(pointIndex);
			tmppair.point2 = lPointsets.at(pointIndex + 1);
			tmppair.length = twoPointsDistance(tmppair.point1, tmppair.point2);
			totalLength += tmppair.length;

			//line : y = kx+b .   y = slopeForMaxLengthline .x +b
			tmppair.slope = (double)(tmppair.point1.y - tmppair.point2.y) / (double)(tmppair.point1.x - tmppair.point2.x);
			tmppair.lineFunctionB = tmppair.point1.y - tmppair.slope*(tmppair.point1.x);

			if (tmppair.length > maxLength)
			{
				maxLength = tmppair.length;
				maxlengthIndex = pointIndex;
				slopeForMaxLengthline = tmppair.slope;
				lineFunctionB = tmppair.lineFunctionB;


				maxlengthPairpoint = tmppair;
			}

			tmppair.indexInLinePointsets = pointIndex;

			pairsPoint.push_back(tmppair);

		}

		for (int i = 0; i < pairsPoint.size(); i++)
		{
			if (pairsPoint.at(i).length> totalLength *0.2)
			{
				//pairsPoint.at(i).indexInLinePointsets = i;
				thTotalLPairpoint.push_back(pairsPoint.at(i));
			}

			if (i == maxlengthIndex)
			{
				pairsPoint.at(i).crossPoint2maxlengthPair.x = -2;
				pairsPoint.at(i).crossPoint2maxlengthPair.y = -2;
				continue;
			}
			pairsPoint.at(i).crossPoint2maxlengthPair = getCrosspoint(pairsPoint.at(i), maxlengthPairpoint);
		}

		getProjectionPoints(lPointsets, maxlengthPairpoint, projection2MlL);


		if (thTotalLPairpoint.size() > 1)//1/3 线最近点投影距离，及角度
		{
			// thppairSmall,thpairBigger 需要排序，mark

			if (thTotalLPairpoint.at(0).length <= thTotalLPairpoint.at(1).length)
			{
				thppairSmall = thTotalLPairpoint.at(0);
				thpairBigger = thTotalLPairpoint.at(1);
			}
			else
			{
				thppairSmall = thTotalLPairpoint.at(1);
				thpairBigger = thTotalLPairpoint.at(0);
			}

			if (thppairSmall.indexInLinePointsets < thpairBigger.indexInLinePointsets)
			{
				//tmppairSmall.point2,  
				tDtceThTotalLPairpoint = twoPointsDistance(thppairSmall.point2, projection2MlL.at(thppairSmall.indexInLinePointsets + 1)); //投影				 
			}
			else
			{
				tDtceThTotalLPairpoint = twoPointsDistance(thppairSmall.point1, projection2MlL.at(thppairSmall.indexInLinePointsets));
			}
			getProjectionPoints(lPointsets, thppairSmall, projection2MlLSecond);
		}
		else
		{
			thpairBigger = maxlengthPairpoint;
		}

		//projection2MlLSecond

		double deltaaMax = 0.0;
		area = getBeyongLineArea(deltaaMax,linetype);
		length = totalLength - maxLength;		
		thv = area / length;
		thv = max(thv, deltaaMax);
		
	}

	double getmaxlengthPairpointRote()
	{
		return maxlengthPairpoint.slope;
	}
	void getProjectionPoints(const vector<cv::Point> lPointsets, pairPoint xLine, vector<cv::Point> &projection2)
	{
		cv::Point projectionPoint;
		for (int indexpoint = 0; indexpoint < lPointsets.size(); indexpoint++)
		{
			double m = xLine.slope;
			double b = xLine.lineFunctionB;
			int x0 = lPointsets.at(indexpoint).x;
			int y0 = lPointsets.at(indexpoint).y;
			projectionPoint.x = (m*y0 + x0 - m*b) / (pow(m, 2) + 1);
			projectionPoint.y = (y0*pow(m, 2) + m*x0 + b) / (pow(m, 2) + 1);

			projection2.push_back(projectionPoint);
		}

	}
	cv::Point getCrosspoint(pairPoint twoPointLine, pairPoint maxLengthTwoPointLine)
	{
		cv::Point crossPoint;
		double D = -twoPointLine.slope + maxLengthTwoPointLine.slope;
		if (D == 0)
		{
			crossPoint.x = -1;
			crossPoint.y = -1;
			return crossPoint;
		}
		crossPoint.x = (twoPointLine.lineFunctionB - maxLengthTwoPointLine.lineFunctionB) / D;
		crossPoint.y = (twoPointLine.slope*(-maxLengthTwoPointLine.lineFunctionB) - (maxLengthTwoPointLine.slope*(-twoPointLine.lineFunctionB))) / D;
		return crossPoint;
	}


	double getBeyongLineAreahelp(const vector<cv::Point> projection, int begain, int end, double& deltaa, int linetype)
	{
		vector<cv::Point> contour1;

		for (int i = begain; i < end + 1; i++)
		{

			contour1.push_back(projection.at(i));
			contour1.push_back(LinePointsets.at(i));
		}


		int lengthh = twoPointsDistance(projection.at(begain), projection.at(end));
		vector<cv::Point> approx;
		approxPolyDP(contour1, approx, 1, true);
		double area = contourArea(approx);
		deltaa = area / lengthh;
		

		int ret1, ret2;
		ret1 = ret2 = 0;
		bool isOutply = true;
		switch (linetype)
		{
		case LEFTLINE_TYPE:
			ret1= projection.at(begain).x - LinePointsets.at(begain).x;
			ret2= projection.at(end).x - LinePointsets.at(end).x;
			if (ret1>0||ret2>0)
			{
				isOutply = true;
			}
			else
			{
				isOutply = false;
			}
			break;
		case DOWNLINE_TYPE:
			ret1 = projection.at(begain).y - LinePointsets.at(begain).y;
			ret2 = projection.at(end).y - LinePointsets.at(end).y;
			if (ret1 > 0 || ret2 > 0)
			{
				isOutply = false;
			}
			else
			{
				isOutply = true;
			}
			break;
		case RIGHTLINE_TYPE:
			ret1 = projection.at(begain).x - LinePointsets.at(begain).x;
			ret2 = projection.at(end).x - LinePointsets.at(end).x;
			if (ret1 > 0 || ret2 > 0)
			{
				isOutply = false;
			}
			else
			{
				isOutply = true;
			}
			break;
		case UPLINE_TYPE:
			ret1 = projection.at(begain).y - LinePointsets.at(begain).y;
			ret2 = projection.at(end).y - LinePointsets.at(end).y;
			if (ret1 > 0 || ret2 > 0)
			{
				isOutply = true;
			}
			else
			{
				isOutply = false;
			}
			break;
		default:
			break;
		}

		 
		if (isOutply == false && area < 200)
		{
			deltaa = 0;
			area = 0;
		}
		
		return area;

	}

	double getgetBeyongLineAreaF(const vector<cv::Point> projection, int indexP, double& deltaa, int linetype)
	{
		double area1DL;// , area2DL, area1DTt;
		area1DL = 0.0;// area2DL = area1DTt = 0.0;
		area1DL = getBeyongLineAreahelp(projection, indexP, indexP + 1, deltaa, linetype);

		return area1DL;
#if 0

		if (indexP > 0)
		{

			area1DL = getBeyongLineAreahelp(projection, 0, indexP, deltaa);
			area2DL = getBeyongLineAreahelp(projection, indexP + 1, projection.size() - 1, deltaa);

			totalErrarea = area1DL + area2DL;

		}
		else
		{
			totalErrarea = getBeyongLineAreahelp(projection, indexP + 1, projection.size() - 1, deltaa);
		}


		if (thTotalLPairpoint.size() >= 2)
		{
			area1DTt = getBeyongLineAreahelp(projection, thppairSmall.indexInLinePointsets, thppairSmall.indexInLinePointsets + 1, deltaa);
		}


		totalErrarea = totalErrarea - area1DTt;
		return totalErrarea;
#endif
	}
	double getBeyongLineArea(double& deltaaMax,int linetype)
	{

		double areak = 0.0;
		int sSMall = 0;
		int sBIgger = 0;		 
		double deltaa = 0.0;
		if (thTotalLPairpoint.size() > 1)
		{
			for (int index = 0; index < pairsPoint.size(); index++)
			{
				int de = pairsPoint.at(index).indexInLinePointsets;
				sSMall = abs(thppairSmall.indexInLinePointsets - de);
				sBIgger = abs(thpairBigger.indexInLinePointsets - de);
				if (sSMall < sBIgger)
				{
					areak += getgetBeyongLineAreaF(projection2MlLSecond, de, deltaa,linetype);					
				}
				else
				{
					areak += getgetBeyongLineAreaF(projection2MlL, de, deltaa, linetype);

				}
				if (deltaa > deltaaMax)
				{
					deltaaMax = deltaa;
				}
			}
		}
		else
		{
			for (int index = 0; index < pairsPoint.size(); index++)
			{
				int de = pairsPoint.at(index).indexInLinePointsets;
				areak += getgetBeyongLineAreaF(projection2MlL, de, deltaa, linetype);
				if (deltaa > deltaaMax)
				{
					deltaaMax = deltaa;
				}
			}
		}
		return areak;

	}

public:
	vector<cv::Point> LinePointsets;
	vector<cv::Point> projection2MlL;// projection2MaxlengthLinePointsets
	vector<cv::Point> projection2MlLSecond;// projection2MaxlengthLinePointsets

	double slopeForMaxLengthline;
	double percentForMaxLengthline;
	int maxlengthIndex;
	double lineFunctionB;//B , y = kx+B

	double area;
	double thv;
	int length;
	vector<pairPoint> pairsPoint; //直线点对
	pairPoint    maxlengthPairpoint;
	vector<pairPoint>    thTotalLPairpoint;// 大于总长1/3的直线
	pairPoint thppairSmall, thpairBigger;
	double tDtceThTotalLPairpoint;
	double totalErrarea;

protected:
private:
};
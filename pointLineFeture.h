
#pragma  once
#include "global.h"
#include "pointLineFeatureBase.h"
class pointLineFeature
{
public:
	pointLineFeature()
	{
		cv::Point tmp(0, 0);
		luPoint = ldPoint = rdPoint = ruPoint = tmp;
		luIndex = ldIndex = rdIndex = ruIndex = 0;
	}

	void getFor1linePoints(const  vector<cv::Point> inpoint2Lengthsets, vector<cv::Point>& outLines, int beginIndex, int endIndex)
	{
		if (endIndex < beginIndex)
		{
			endIndex = endIndex + inpoint2Lengthsets.size() / 2;
		}
		for (int i = beginIndex; i < endIndex + 1; i++)
		{
			outLines.push_back(inpoint2Lengthsets.at(i));
		}

	}
	void get4LinesPoints()
	{
		if (polyVPoint.size() < 4)
		{
			return;
		}
		vector<cv::Point> tmpVPoint;
		tmpVPoint = polyVPoint;
		tmpVPoint.insert(tmpVPoint.end(), polyVPoint.begin(), polyVPoint.end());

		getFor1linePoints(tmpVPoint, leftLinePoints.LinePointsets, luIndex, ldIndex);
		getFor1linePoints(tmpVPoint, downLinePoints.LinePointsets, ldIndex, rdIndex);
		getFor1linePoints(tmpVPoint, rightLinePoints.LinePointsets, rdIndex, ruIndex);
		getFor1linePoints(tmpVPoint, upLinePoints.LinePointsets, ruIndex, luIndex);

	}

	double getLineMaxlengthSlope(pointLineFeatureBase lineFeatureBase)
	{
		return lineFeatureBase.getmaxlengthPairpointRote();
	}
	void getLineAreaFeature()
	{

		leftLinePoints.getLineFeature(leftLinePoints.LinePointsets, LEFTLINE_TYPE);
		downLinePoints.getLineFeature(downLinePoints.LinePointsets, DOWNLINE_TYPE);
		rightLinePoints.getLineFeature(rightLinePoints.LinePointsets, RIGHTLINE_TYPE);
		upLinePoints.getLineFeature(upLinePoints.LinePointsets, UPLINE_TYPE);
	}
	/*
	 para  tdVV 2个最长边夹角最近距离
	*/
	ErrCode getLineAreaFeatureResult(double areaThreshole, double aDlthresh,int tdVV)//area / lenth Threshold
	{
		
		ErrCode ret = E_SUCEES;
		if (
			((leftLinePoints.area > areaThreshole) && leftLinePoints.thv > aDlthresh) ||
			((downLinePoints.area > areaThreshole) && downLinePoints.thv > aDlthresh) ||
			((rightLinePoints.area > areaThreshole) && rightLinePoints.thv > aDlthresh) ||
			((upLinePoints.area > areaThreshole) && upLinePoints.thv > aDlthresh)
			)
		{
			ret = E_AreaDlengT;
		}

	
		if (leftLinePoints.tDtceThTotalLPairpoint >tdVV ||
			downLinePoints.tDtceThTotalLPairpoint>tdVV ||
			rightLinePoints.tDtceThTotalLPairpoint>tdVV ||
			upLinePoints.tDtceThTotalLPairpoint>tdVV )
		{
			ret = E_tDtceThTotalLPairpoint;
		}
		return ret;
	}

public:
	cv::Point luPoint;
	int luIndex;

	cv::Point ldPoint;
	int ldIndex;

	cv::Point rdPoint;
	int rdIndex;


	cv::Point ruPoint;
	int ruIndex;

	//include eg. luPoint...4 points
	pointLineFeatureBase leftLinePoints;
	pointLineFeatureBase downLinePoints;
	pointLineFeatureBase rightLinePoints;
	pointLineFeatureBase upLinePoints;



	vector<cv::Point> polyVPoint;
protected:
private:
};
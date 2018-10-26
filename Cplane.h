#pragma once
#include "pointLineFeture.h"
class Cplane :public pointLineFeature
{
public:
	 Cplane();
	~Cplane();
	Mat selectImg;
	void getOriImg(const Mat inColourImg, vector<vector<cv::Point>> inVPoint);
	 
};


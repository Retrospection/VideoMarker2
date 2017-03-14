#pragma once


#include <afx.h>
#include <opencv2/core/core.hpp>

class CShapeHelper
{
public:
	CShapeHelper() = delete; 
	~CShapeHelper() = delete;

public:
	static cv::Point ToOpenCVPoint(const CPoint& cpoint);
	static cv::Point ToOpenCVPoint(int x, int y);

	cv::Point ConvertFromCPoint(const CPoint& point);
};


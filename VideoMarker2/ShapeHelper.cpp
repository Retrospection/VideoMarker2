#include "stdafx.h"
#include "ShapeHelper.h"


cv::Point CShapeHelper::ToOpenCVPoint(const CPoint& cpoint)
{
	return{};
}

cv::Point CShapeHelper::ToOpenCVPoint(int x, int y)
{
	return{};
}


cv::Point CShapeHelper::ConvertFromCPoint(const CPoint& point)
{
	return{ point.x, point.y };
}

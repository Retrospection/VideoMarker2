#pragma once

#include <opencv2/core/core.hpp>

#include <vector>

class IVideoMarker2
{
public:
	IVideoMarker2(){}
	
public:
	virtual std::vector<cv::Rect> GetUnsavedBox() = 0;
};
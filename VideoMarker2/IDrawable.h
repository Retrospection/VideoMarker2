#pragma once

#include <opencv2/core/core.hpp>

class IDrawable
{
public:
	IDrawable(){}
	virtual ~IDrawable(){}

public:
	virtual void Draw(cv::Mat& mat) = 0;
};
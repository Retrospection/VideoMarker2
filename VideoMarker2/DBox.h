#pragma once

#include "IDrawable.h"

class DBox :
	public IDrawable
{
public:
	DBox(const cv::Rect& rect, cv::Scalar color)
		: m_rect(rect), m_color(color)
	{}

public:
	virtual void Draw(cv::Mat& mat)
	{
		cv::rectangle(mat, m_rect, m_color, 3, 4, 0);
	}


private:
	const cv::Rect m_rect;
	const cv::Scalar m_color;
};
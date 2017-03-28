#pragma once

#include "IDrawable.h"

class DEditBox :
	public IDrawable
{
public:
	DEditBox(const cv::Rect& rect, const std::vector<cv::Rect>& editPoints, cv::Scalar color)
		: m_rect(rect), m_color(color), m_EditPoints(editPoints)
	{}

public:
	virtual void Draw(cv::Mat& mat)
	{
		cv::rectangle(mat, m_rect, m_color, 3, 4, 0);
		for (auto& rc : m_EditPoints)
		{
			cv::rectangle(mat, rc, m_color, 3, 4, 0);
		}
	}


private:
	const cv::Rect m_rect;
	const std::vector<cv::Rect> m_EditPoints;
	const cv::Scalar m_color;
};
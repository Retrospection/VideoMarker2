#pragma once

#include "IDrawable.h"

class DEditBox :
	public IDrawable
{
public:
	DEditBox(const cv::Rect& rect, const std::vector<cv::Rect>& editPoints, const std::string& str, const cv::Point& org, cv::Scalar color)
		: m_rect(rect), m_color(color), m_EditPoints(editPoints), m_str(str), m_org(org)
	{}

public:
	virtual void Draw(cv::Mat& mat)
	{
		cv::rectangle(mat, m_rect, m_color, 3, 4, 0);
		for (auto& rc : m_EditPoints)
		{
			cv::rectangle(mat, rc, m_color, 3, 4, 0);
		}
		cv::putText(mat, m_str, m_org, cv::FONT_HERSHEY_COMPLEX, 1, m_color);
	}


private:
	const cv::Rect m_rect;
	const std::vector<cv::Rect> m_EditPoints;
	const std::string m_str;
	const cv::Point m_org;
	const cv::Scalar m_color;
};
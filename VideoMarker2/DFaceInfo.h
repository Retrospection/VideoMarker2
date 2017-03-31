#pragma once

#include "IDrawable.h"
#include "FrameInfo.h"

class DFaceInfo :
	public IDrawable
{
public:
	DFaceInfo(const FaceInfo& faceInfo, cv::Scalar color)
		: m_rect(faceInfo.box), m_str(faceInfo.strPersonName), m_color(color)
	{}

public:
	virtual void Draw(cv::Mat& mat)
	{
		cv::rectangle(mat, m_rect, m_color, 3, 4, 0);
		cv::putText(mat, m_str,m_rect.tl(), cv::FONT_HERSHEY_COMPLEX, 1, m_color);
	}


private:
	const cv::Rect m_rect;
	const std::string m_str;
	const cv::Scalar m_color;
};
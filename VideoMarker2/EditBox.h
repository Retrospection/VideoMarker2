#pragma once

#include <vector>
#include <opencv2/core/core.hpp>

class RectEx;


// EditBox 中存放的点所处的坐标系必须为 Raw 坐标系
class CEditBox
{
public:
	explicit CEditBox(cv::Rect& rc);
	~CEditBox();

	int Hit(const cv::Point& point) const;
	void UpdateLocation(int nType, const cv::Point& newPoint);

	const cv::Rect& GetBox() const;
	const std::vector<cv::Rect>& GetEditRects() const;
	
private:
	static cv::Rect GetEditMarkRect(const cv::Point& point);

private:
	void UpdateEditRects();
	std::vector<cv::Point> CalculateEditPoints();

private:
	RectEx& m_Box;
	std::vector<cv::Rect> m_EditRects;
};


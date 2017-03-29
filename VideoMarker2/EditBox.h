#pragma once

#include <vector>
#include <opencv2/core/core.hpp>



// EditBox 中存放的点所处的坐标系必须为 Raw 坐标系
class CEditBox
{
public:
	explicit CEditBox(cv::Rect* pRc);
	~CEditBox();

	bool SetEditPointIndex(const cv::Point& point);

	void Change(const cv::Point& newPoint);

	cv::Rect* GetBox()const;
	std::vector<cv::Rect> GetEditPoints() const;
	int GetEditPointIndex()const;
	

private:
	void CalculateEditPoints();



private:
	cv::Rect* m_pBox;
	std::vector<cv::Rect> m_EditPoints;
	int m_nEditPointIndex;
};


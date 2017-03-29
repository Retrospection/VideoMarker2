#pragma once

#include <vector>
#include <opencv2/core/core.hpp>



// EditBox �д�ŵĵ�����������ϵ����Ϊ Raw ����ϵ
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


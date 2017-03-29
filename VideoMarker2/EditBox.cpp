#include "stdafx.h"
#include "EditBox.h"

#include <iostream>


CEditBox::CEditBox(cv::Rect* pRc)
	:m_pBox(pRc), m_nEditPointIndex(-1)
{
	CalculateEditPoints();
}


CEditBox::~CEditBox()
{
}

void CEditBox::CalculateEditPoints()
{
	m_EditPoints.clear();
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			m_EditPoints.push_back({ m_pBox->tl() + cv::Point(i*(m_pBox->width / 2), j*(m_pBox->height / 2)) - cv::Point(20, 20), m_pBox->tl() + cv::Point(i*(m_pBox->width / 2), j*(m_pBox->height / 2)) + cv::Point(20, 20) });
		}
	}
}

cv::Rect* CEditBox::GetBox() const
{
	return m_pBox;
}

std::vector<cv::Rect> CEditBox::GetEditPoints() const
{
	return m_EditPoints;
}

int CEditBox::GetEditPointIndex() const
{
	return m_nEditPointIndex;
}

bool CEditBox::SetEditPointIndex(const cv::Point& point)
{
	auto iter = std::find_if(m_EditPoints.begin(), m_EditPoints.end(), [&](const cv::Rect& rc){ return rc.contains(point); });
	if (iter == m_EditPoints.end())
	{
		m_nEditPointIndex = -1;
		return false;
	}
	m_nEditPointIndex = iter - m_EditPoints.begin();
	std::cout << "EditPoint index: " << m_nEditPointIndex << std::endl;
	return true;
}

void CEditBox::Change(const cv::Point& newPoint)
{
	switch (m_nEditPointIndex)
	{
	case 0:
	{
		cv::Point br = m_pBox->br();
		*m_pBox = { newPoint, br };
		CalculateEditPoints();
		break;
	}
	default:
		break;
	}
}

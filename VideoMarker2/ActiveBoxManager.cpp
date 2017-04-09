#include "stdafx.h"
#include "ActiveBoxManager.h"
#include "Transformer.h"


const cv::Point CActiveBoxManager::INIT_POINT{ -1, -1 };

CActiveBoxManager::CActiveBoxManager(Transformer* ptr)
	:m_pTrans(ptr)
{
	m_ActivePoints[0] = INIT_POINT; 
	m_ActivePoints[1] = INIT_POINT;
}


CActiveBoxManager::~CActiveBoxManager()
{
}

bool CActiveBoxManager::GetActiveBoxInRoi(cv::Rect& activeBox) const
{
	if (m_ActivePoints[0] == INIT_POINT || m_ActivePoints[1] == INIT_POINT)
	{
		return false;
	}

	if (m_ActivePoints[0] == m_ActivePoints[1])
	{
		return false;
	}
	activeBox = cv::Rect(m_pTrans->Trans({ m_ActivePoints[0], m_ActivePoints[1] }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Roi));
	cv::Rect RoiRect = m_pTrans->GetRoiRect();
	return (RoiRect & activeBox) == activeBox;
}

void CActiveBoxManager::SetStartActivePoint(const cv::Point& pt)
{
	m_ActivePoints[0] = ((m_pTrans->GetRoiRect().contains(pt)) ? pt : INIT_POINT);
}

void CActiveBoxManager::SetEndActivePoint(const cv::Point& pt)
{
	if (!m_pTrans->GetRoiRect().contains(pt))
	{
		ResetActiveBox();
	}
	else
	{
		m_ActivePoints[1] = pt;
	}
}

void CActiveBoxManager::ResetActiveBox()
{
	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;
}



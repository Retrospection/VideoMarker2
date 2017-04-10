#include "stdafx.h"
#include "ActiveBoxManager.h"
#include "Transformer.h"

#include "constant.h"
#include "DBox.h"


#include <iostream>



const cv::Point CActiveBoxManager::INIT_POINT{ -1, -1 };

CActiveBoxManager::CActiveBoxManager(Transformer* ptr)
	:m_pTrans(ptr), m_bChanged(true)
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
	cv::Point point = ((m_pTrans->GetRoiRect().contains(pt)) ? pt : INIT_POINT);
	m_bChanged |= (point != m_ActivePoints[0]);
	m_ActivePoints[0] = point;
}

void CActiveBoxManager::SetEndActivePoint(const cv::Point& pt)
{
	if (!m_pTrans->GetRoiRect().contains(pt))
	{
		ResetActiveBox();
	}
	else
	{
		m_bChanged |= (m_ActivePoints[1] != pt);
		m_ActivePoints[1] = pt;
	}
}

void CActiveBoxManager::ResetActiveBox()
{
	m_bChanged |= ((m_ActivePoints[0] != INIT_POINT) || (m_ActivePoints[1] != INIT_POINT));
	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;
}

void CActiveBoxManager::UpdateDrawableActiveBox(std::vector<IDrawable*>& toBeUpdated)
{
	if (!m_bChanged)
	{
		std::cout << "-----------------------" << std::endl;
		std::cout << "ActiveBox Unchanged!" << std::endl;
		return;
	}
	for (auto drawable : toBeUpdated)
	{
		delete drawable;
	}
	toBeUpdated.clear();
	cv::Rect rc;
	if (GetActiveBoxInRoi(rc))
	{
		toBeUpdated.push_back(new DBox(rc, ColorUnsaved));
	}
	m_bChanged = false;
}



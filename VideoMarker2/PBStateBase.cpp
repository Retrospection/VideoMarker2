#include "stdafx.h"
#include "PBStateBase.h"

#include "PictureBox.h"
#include "FaceInfoManager.h"
#include "Transformer.h"

#include "VideoMarker2Dlg.h"


class FaceInfoManager;

const cv::Point INIT_POINT = { -1, -1 };


CPBStateBase::CPBStateBase(CPictureBox* ptr)
	:m_pPictureBox(ptr)
{
}


CPBStateBase::~CPBStateBase()
{
}

void CPBStateBase::OnLButtonDown(UINT nFlags, CPoint point)
{

}

void CPBStateBase::OnLButtonDown2(UINT nFlags, CPoint point)
{
	m_pPictureBox->OnLButtonDown2(nFlags, point);
}

void CPBStateBase::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void CPBStateBase::OnMouseMove(UINT nFlags, CPoint point)
{

}


void CPBStateBase::DrawActiveBox()
{

}



bool CPBStateBase::IsDrawing() const
{
	return m_pPictureBox->m_bDrawing;
}

int CPBStateBase::GetEditType() const
{
	return m_pPictureBox->m_nEditType;
}

bool CPBStateBase::IsInRoi(const CPoint& pt) const
{
	return m_pPictureBox->IsInRoi(pt);
}

void CPBStateBase::SetDrawing(bool bDrawing)
{
	m_pPictureBox->m_bDrawing = bDrawing;
}

void CPBStateBase::SetActivePoint(const cv::Point& pt, bool bBegin)
{
	m_pPictureBox->m_ActivePoints[bBegin ? 0 : 1] = pt;
}

void CPBStateBase::ResetActivePoints()
{
	SetActivePoint(INIT_POINT, ACTIVE_POINT_BEGIN);
	SetActivePoint(INIT_POINT, ACTIVE_POINT_END);
}

bool CPBStateBase::IsDrawable() const
{
	return m_pPictureBox->m_bDrawable;
}

bool CPBStateBase::HasChosenEditPoint() const
{
	return m_pPictureBox->m_nModifiedFaceInfoIndex != -1;
}

void CPBStateBase::MoveEditPoint(const cv::Point& pt)
{
	m_pPictureBox->m_pFaceInfoManager->Move(pt);
}

cv::Rect CPBStateBase::Trans(const cv::Rect& rc, int src, int dst)
{
	return m_pPictureBox->m_Trans.Trans(rc, src, dst);
}

bool CPBStateBase::GetUnsavedName(std::string& ret)
{
	return ((CVideoMarker2Dlg*)m_pPictureBox->GetParent())->GetUnsavedName2(ret);
}

bool CPBStateBase::GetActiveBox(cv::Rect& rc)
{
	return m_pPictureBox->GetActiveBox(rc);
}

void CPBStateBase::AddFaceInfo(const FaceInfo& faceInfo)
{
	m_pPictureBox->m_pFaceInfoManager->Add(faceInfo);
}

unsigned int CPBStateBase::ValidateFaceInfo()
{
	return m_pPictureBox->ValidateFaceInfo();
}

void CPBStateBase::HandleInvalidFaceInfo(unsigned int result)
{
	m_pPictureBox->MessageBox(m_pPictureBox->m_AlertMessage[result]);
	m_pPictureBox->Undo();
}

void CPBStateBase::SelectBox()
{
	m_pPictureBox->SelectBox();
}

void CPBStateBase::AddDrawables(IDrawable* drawable)
{
	m_pPictureBox->m_drawables.push_back(drawable);
}

bool CPBStateBase::GetActiveBoxEx(cv::Rect& rc)
{
	return m_pPictureBox->GetActiveBoxEx(rc);
}

void CPBStateBase::ClearSelectEditPoint()
{
	m_pPictureBox->ClearSelectEditPoint();
}

bool CPBStateBase::GetActiveBoxFromActiveBoxManager(cv::Rect& rc)
{
	return m_pPictureBox->m_ActiveBoxManager.GetActiveBoxInRoi(rc);
}

void CPBStateBase::SetStartActivePoint(const cv::Point& pt)
{
	m_pPictureBox->m_ActiveBoxManager.SetStartActivePoint(pt);
}

void CPBStateBase::SetEndActivePoint(const cv::Point& pt)
{
	m_pPictureBox->m_ActiveBoxManager.SetEndActivePoint(pt);
}





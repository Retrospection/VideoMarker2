#include "stdafx.h"
#include "PBModifyState.h"
#include "PictureBox.h"


CPBModifyState::CPBModifyState(CPictureBox* ptr)
	:CPBStateBase(ptr)
{
}


CPBModifyState::~CPBModifyState()
{
}

void CPBModifyState::OnLButtonDown(UINT nFlags, CPoint point)
{
	OnLButtonDown2(nFlags, point);
}

void CPBModifyState::OnLButtonUp(UINT nFlags, CPoint point)
{
//	SetActivePoint({ point.x, point.y }, ACTIVE_POINT_END);
	SetEndActivePoint({ point.x, point.y });
	unsigned int result = ValidateFaceInfo();
	if (result != 0)
	{
		HandleInvalidFaceInfo(result);
	}
	ClearSelectEditPoint();
}

void CPBModifyState::OnMouseMove(UINT nFlags, CPoint point)
{
	cv::Point _point = Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl();
	MoveEditPoint(_point);
}



#include "stdafx.h"
#include "PBSelectState.h"
#include "DBox.h"
#include "PictureBox.h"

#include <iostream>


const cv::Scalar Green{ 0, 255, 0 };
const cv::Scalar Red{ 0, 0, 255 };
const cv::Scalar Black{ 0, 0, 0 };
const cv::Scalar Blue{ 255, 0, 0 };

const cv::Scalar ColorUnsaved = Red;
const cv::Scalar ColorSaved = Red;
const cv::Scalar ColorHighLight = Green;
const cv::Scalar ColorIllegal = Blue;


CPBSelectState::CPBSelectState(CPictureBox* ptr)
	:CPBStateBase(ptr)
{
}


CPBSelectState::~CPBSelectState()
{
}

void CPBSelectState::OnLButtonDown(UINT nFlags, CPoint point)
{
	OnLButtonDown2(nFlags,point);
}

void CPBSelectState::OnLButtonUp(UINT nFlags, CPoint point)
{
	assert(IsDrawable());
//	SetActivePoint({ point.x, point.y }, ACTIVE_POINT_END);
	SetEndActivePoint({ point.x, point.y });
	SelectBox();
}

void CPBSelectState::OnMouseMove(UINT nFlags, CPoint point)
{
	assert(IsDrawable());
	assert(GetEditType() == CPictureBox::SELECT_MARK);
// 	if (!IsInRoi(point))
// 	{
// 		ResetActivePoints();
// 		return;
// 	}
// 
// 	SetActivePoint({ point.x, point.y }, ACTIVE_POINT_END);
	SetEndActivePoint({ point.x, point.y });
}

void CPBSelectState::DrawActiveBox()
{
// 	if (!HasChosenEditPoint())
// 	{
// 		cv::Rect rc;
// // 		if (GetActiveBoxEx(rc))
// // 		{
// // 			AddDrawables(new DBox(rc, ColorUnsaved));
// // 		}
// 		if (GetActiveBoxFromActiveBoxManager(rc))
// 		{
// 			AddDrawables(new DBox(rc, ColorUnsaved));
// 		}
// 	}

	assert(!HasChosenEditPoint());
	UpdateDrawableActiveBox();

}

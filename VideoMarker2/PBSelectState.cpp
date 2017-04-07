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


	SetActivePoint({ point.x, point.y }, ACTIVE_POINT_END);
//	SetDrawing(false);

	if (!HasChosenEditPoint())
	{
		// 当前为选择模式
		SelectBox();
	}
	else
	{
		unsigned int result = ValidateFaceInfo();
		if (result != 0)
		{
			HandleInvalidFaceInfo(result);
		}
		ClearSelectEditPoint();
	}
	

}

void CPBSelectState::OnMouseMove(UINT nFlags, CPoint point)
{
	assert(IsDrawable());
// 	if (!IsDrawing())
// 	{
// 		return;
// 	}
	assert(GetEditType() == CPictureBox::SELECT_MARK);
	if (!IsInRoi(point))
	{
		ResetActivePoints();
		return;
	}
	if (HasChosenEditPoint())
	{
		cv::Point _point = Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl();
		MoveEditPoint(_point);
	}

	SetActivePoint({ point.x, point.y }, ACTIVE_POINT_END);
}

void CPBSelectState::DrawActiveBox()
{
	if (!HasChosenEditPoint())
	{
		cv::Rect rc;
		if (GetActiveBoxEx(rc))
		{
			AddDrawables(new DBox(rc, ColorUnsaved));
		}
	}
}

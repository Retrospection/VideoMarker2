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
const cv::Scalar ColorSelected = Blue;


CPBSelectState::CPBSelectState(CPictureBox* ptr)
	:CPBStateBase(ptr)
{
}


CPBSelectState::~CPBSelectState()
{
}

void CPBSelectState::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetDrawing(true);
	cv::Point _point{ Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl() };
	if (SelectEditPoint(_point))
	{
		SetState(CPictureBox::MODIFY_MARK_STATE);
	}
	SetStartActivePoint(_point);
}

void CPBSelectState::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetEndActivePoint({ point.x, point.y });
	SelectBox();
}

void CPBSelectState::OnMouseMove(UINT nFlags, CPoint point)
{
	SetEndActivePoint({ point.x, point.y });
}

void CPBSelectState::DrawActiveBox()
{
	assert(!HasChosenEditPoint());
	UpdateDrawableActiveBox();
}

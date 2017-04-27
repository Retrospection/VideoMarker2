#include "stdafx.h"
#include "PBAddState.h"

#include "PictureBox.h"
#include "DBox.h"
#include "constant.h"

#include <iostream>

CPBAddState::CPBAddState(CPictureBox* ptr)
	:CPBStateBase(ptr)
{
}


CPBAddState::~CPBAddState()
{
}

void CPBAddState::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetDrawing(true);
	cv::Point _point{ Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl() };
	SetStartActivePoint(_point);
}

void CPBAddState::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetEndActivePoint({ point.x, point.y });
	cv::Rect activeBox;
	std::string strPersonName;
	if (!GetActiveBoxFromActiveBoxManager(activeBox) || !GetUnsavedName(strPersonName))
	{
		return;
	}

	activeBox = Trans(activeBox, Transformer::Coordinate::Roi, Transformer::Coordinate::Raw);
	AddFaceInfoToPictureBox({ strPersonName, activeBox });
	unsigned int result = ValidateFaceInfo();
	if (result != 0)
	{
		HandleInvalidFaceInfo(result);
	}
	UpdateDlgFrameInfo();
}

void CPBAddState::OnMouseMove(UINT nFlags, CPoint point)
{
	SetEndActivePoint({ point.x, point.y });
}


void CPBAddState::DrawActiveBox()
{
	UpdateDrawableActiveBox();
}



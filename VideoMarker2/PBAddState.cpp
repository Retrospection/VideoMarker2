#include "stdafx.h"
#include "PBAddState.h"

#include "PictureBox.h"
#include "DBox.h"

#include <iostream>



const cv::Scalar Green{ 0, 255, 0 };
const cv::Scalar Red{ 0, 0, 255 };
const cv::Scalar Black{ 0, 0, 0 };
const cv::Scalar Blue{ 255, 0, 0 };

const cv::Scalar ColorUnsaved = Red;
const cv::Scalar ColorSaved = Red;
const cv::Scalar ColorHighLight = Green;
const cv::Scalar ColorSelected = Blue;



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
	AddFaceInfo({ strPersonName, activeBox });
	unsigned int result = ValidateFaceInfo();
	if (result != 0)
	{
		HandleInvalidFaceInfo(result);
	}

}

void CPBAddState::OnMouseMove(UINT nFlags, CPoint point)
{
	SetEndActivePoint({ point.x, point.y });
}


void CPBAddState::DrawActiveBox()
{
	UpdateDrawableActiveBox();
}



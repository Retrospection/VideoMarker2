// PictureBox.cpp : 实现文件


#include "stdafx.h"
#include "VideoMarker2.h"
#include "PictureBox.h"
#include "NameInputDialog.h"

#include "CvvImage.h"
#include "DBox.h" 
#include "DText.h"
#include "DEditBox.h"
#include "DFaceInfo.h"

#include "StringHelper.h"

#include "VideoMarker2Dlg.h"

#include "PBInitState.h"
#include "PBAddState.h"
#include "PBSelectState.h"

#include "FaceInfoManager.h"

#include <iostream>
#include <functional>

#include <deque>



const cv::Point INIT_POINT = { -1, -1 };

const cv::Scalar Green{ 0, 255, 0 };
const cv::Scalar Red{ 0, 0, 255 };
const cv::Scalar Black{ 0, 0, 0 };
const cv::Scalar Blue{ 255, 0, 0 };

const cv::Scalar ColorUnsaved = Red;
const cv::Scalar ColorSaved = Red;
const cv::Scalar ColorHighLight = Green;
const cv::Scalar ColorIllegal = Blue;

class ScopeGuard
{
public:
	ScopeGuard(std::function<void()> f)
		:m_function(f), m_bDismissed(false)
	{
	}
	~ScopeGuard()
	{
		if (!m_bDismissed)
		{
			m_function();
		}
	}

	void Dismiss()
	{
		m_bDismissed = true;
	}

private:
	std::function<void()> m_function;
	bool m_bDismissed;
};




// CPictureBox

IMPLEMENT_DYNAMIC(CPictureBox, CStatic)


const wchar_t* CPictureBox::m_AlertMessage[] = { L"", L"包围盒面积过小！", L"当前帧存在同名标注！", L"IOU过大！" };




CPictureBox::CPictureBox() 
	:CStatic(), m_Trans(Transformer::Default()), m_bDrawable(false)

{
	m_bDrawing = false;
	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;

	m_nModifiedFaceInfoIndex = -1;
	m_pFaceInfoManager = new FaceInfoManager();
	m_pState = new CPBInitState(this);
}

CPictureBox::~CPictureBox()
{
	delete m_pFaceInfoManager;
	delete m_pState;
}

BEGIN_MESSAGE_MAP(CPictureBox, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CPictureBox 消息处理程序


void CPictureBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pState->OnLButtonDown(nFlags, point);
//	if (!m_bDrawable)
//	{
//		return;
//	}
//	if (m_bDrawing)
//	{
//		m_ActivePoints[1] = { point.x, point.y };
//		Invalidate(FALSE); 
//		CStatic::OnLButtonDown(nFlags, point);
//	}
//	else
//	{
//		m_bDrawing = true;
//		cv::Point _point{ m_Trans.Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl() };
//		m_nModifiedFaceInfoIndex = m_pFaceInfoManager->SelectEditPoint(_point);
//// 		if (m_nModifiedFaceInfoIndex == -1)
//// 		{
//// 			m_pFaceInfoManager->ResetSelected();
//// 		}
//		std::cout << "EditBoxes Index: " << m_nModifiedFaceInfoIndex << std::endl;
//		m_ActivePoints[0] = { point.x, point.y };
//		Invalidate(FALSE);
//		CStatic::OnLButtonDown(nFlags, point);
//	}
}

void CPictureBox::OnLButtonDown2(UINT nFlags, CPoint point)
{
	assert(m_bDrawable);

// 	if (m_bDrawing)
// 	{
// 		m_ActivePoints[1] = { point.x, point.y };
// 	}
// 	else
// 	{
		m_bDrawing = true;
		cv::Point _point{ m_Trans.Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl() };
		m_nModifiedFaceInfoIndex = m_pFaceInfoManager->SelectEditPoint(_point);

		std::cout << "EditBoxes Index: " << m_nModifiedFaceInfoIndex << std::endl;
		m_ActivePoints[0] = { point.x, point.y };
//	}
	Invalidate(FALSE);
	CStatic::OnLButtonDown(nFlags, point);
}



void CPictureBox::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pState->OnMouseMove(nFlags, point);
	Invalidate(FALSE);
	CStatic::OnMouseMove(nFlags, point);
// 	if (!m_bDrawable)
// 	{
// 		return;
// 	}
// 	if (!m_bDrawing)
// 	{
// 		return;
// 	}
// 	cv::Rect RoiRect = m_Trans.GetRoiRect();
// 	if (m_nEditType == SELECT_MARK)
// 	{
// 		// 改变选中的box
// 		if (m_nModifiedFaceInfoIndex != -1)
// 		{
// 			cv::Point _point = m_Trans.Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl();
// 			m_pFaceInfoManager->Move(_point);
// 		}
// 	}
// 	else
// 	{
// 		if ((point.x > (RoiRect.x + RoiRect.width)) || (point.y > RoiRect.height))
// 		{
// 			m_bDrawing = false;
// 			m_ActivePoints[0] = INIT_POINT;
// 			m_ActivePoints[1] = INIT_POINT;
// 			Invalidate(FALSE);
// 			CStatic::OnMouseMove(nFlags, point);
// 			return;
// 		}
// 	}
// 	m_ActivePoints[1] = { point.x, point.y };
// 	Invalidate(FALSE);
// 	CStatic::OnMouseMove(nFlags, point);
}

// void CPictureBox::OnMouseMove2(UINT nFlags, CPoint point)
// {
// 	
// // 	if (!m_bDrawing)
// // 	{
// // 		return;
// // 	}
// 	
// 
// //	assert(m_nEditType != SELECT_MARK);
// // 	if (!IsInRoi(point))
// // 	{
// // 		m_bDrawing = false;
// //		m_ActivePoints[0] = INIT_POINT;
// //		m_ActivePoints[1] = INIT_POINT;
// // 	}
// // 	else
// // 	{
// // 		m_ActivePoints[1] = { point.x, point.y };
// // 	}
// 
// }
// void CPictureBox::OnMouseMoveSelect(UINT nFlags, CPoint point)
// {
// // 	std::cout << "select!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
// // 	assert(m_bDrawable);
// // 	if (!m_bDrawing)
// // 	{
// // 		return;
// // 	}
// //	cv::Rect RoiRect = m_Trans.GetRoiRect();
// // 	assert(m_nEditType == SELECT_MARK);
// // 	if (m_nModifiedFaceInfoIndex != -1)
// // 	{
// // 		cv::Point _point = m_Trans.Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl();
// // 		m_pFaceInfoManager->Move(_point);
// // 	}
// // 	m_ActivePoints[1] = { point.x, point.y };
// // 	Invalidate(FALSE);
// // 	CStatic::OnMouseMove(nFlags, point);
// }

void CPictureBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_pState->OnLButtonUp(nFlags, point);
	Invalidate(FALSE);
	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;
	CStatic::OnLButtonUp(nFlags, point);

// 	if (!m_bDrawable)
// 	{
// 		return;
// 	}
// 	if (!m_bDrawing)
// 	{
// 		return;
// 	}
// 	m_ActivePoints[1] = {point.x, point.y};
// 	m_bDrawing = false;
// 	switch (m_nEditType)
// 	{
// 	case ADD_MARK_TYPE:
// 	{
// 		cv::Rect activeBox;
// 		std::string strPersonName;
// 		((CVideoMarker2Dlg*)GetParent())->GetUnsavedName2(strPersonName);
// 		GetActiveBox(activeBox);
// 		activeBox = m_Trans.Trans(activeBox, Transformer::Coordinate::Roi, Transformer::Coordinate::Raw);
// 		m_pFaceInfoManager->Add({ strPersonName, activeBox });
// 		unsigned int result = ValidateFaceInfo();
// 		if (result != 0)
// 		{
// 			MessageBox(m_AlertMessage[result]);
// 			Undo();
// 		}
// 		break;
// 	}
// 	case DELETE_MAKR_TYPE:
// 	{
// 		m_pFaceInfoManager->DeleteSelected();
// 		break;
// 	}
// 	case SELECT_MARK:
// 	{
// 		if (m_nModifiedFaceInfoIndex == -1)
// 		{
// 			// 当前为选择模式
// 			SelectBox();
// 		}
// 		else
// 		{
// 			// 鼠标点击区域不在 EditPoint 区域
// 			unsigned int result = ValidateFaceInfo();
// 			if (result != 0)
// 			{
// 				MessageBox(m_AlertMessage[result]);
// 				Undo();
// 			}
// 		}
// 		break;
// 	}
// 	default:
// 		break;
// 	}
// 
// 	Invalidate(FALSE);
// 	m_ActivePoints[0] = INIT_POINT;
// 	m_ActivePoints[1] = INIT_POINT;
// 
// 	CStatic::OnLButtonUp(nFlags, point);
}

// void CPictureBox::OnLButtonUp2(UINT nFlags, CPoint point)
// {
// // 	assert(m_bDrawable);
// // 	if (!m_bDrawing)
// // 	{
// // 		return;
// // 	}
// //	m_ActivePoints[1] = { point.x, point.y };
// // 	m_bDrawing = false;
// // 	cv::Rect activeBox;
// // 	std::string strPersonName;
// // 	((CVideoMarker2Dlg*)GetParent())->GetUnsavedName2(strPersonName);
// // 	GetActiveBox(activeBox);
// // 	activeBox = m_Trans.Trans(activeBox, Transformer::Coordinate::Roi, Transformer::Coordinate::Raw);
// // 	m_pFaceInfoManager->Add({ strPersonName, activeBox }); 
// // 	unsigned int result = ValidateFaceInfo();
// // 	if (result != 0)
// // 	{
// // 		MessageBox(m_AlertMessage[result]);
// // 		Undo();
// // 	}
// 
// 
// 
// }


// void CPictureBox::OnLButtonUpSelect(UINT nFlags, CPoint point)
// {
// // 	assert(m_bDrawable);
// // 	if (!m_bDrawing)
// // 	{
// // 		return;
// // 	}
// // 	m_ActivePoints[1] = { point.x, point.y };
// // 	m_bDrawing = false;
// 
// // 	if (m_nModifiedFaceInfoIndex == -1)
// // 	{
// // 		// 当前为选择模式
// // 		SelectBox();
// // 	}
// // 	else
// // 	{
// // 		// 鼠标点击区域不在 EditPoint 区域
// // 		unsigned int result = ValidateFaceInfo();
// // 		if (result != 0)
// // 		{
// // 			MessageBox(m_AlertMessage[result]);
// // 			Undo();
// // 		}
// // 	}
// 
// 
// 
// // 	Invalidate(FALSE);
// // 	m_ActivePoints[0] = INIT_POINT;
// // 	m_ActivePoints[1] = INIT_POINT;
// // 
// // 	CStatic::OnLButtonUp(nFlags, point);
// 
// }

FrameInfo CPictureBox::GetUnsavedFrameInfo() const
{
	FrameInfo ret;
	ret.facesInfo = m_pFaceInfoManager->GetUnsavedFacesInfo();
	return std::move(ret);
}


void CPictureBox::ClearUnsavedFaceInfo()
{
	m_pFaceInfoManager->ClearUnsavedFacesInfo();
}



void CPictureBox::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	if (m_image.empty())
	{
		return;
	}
	cv::Mat preparedImage = m_image.clone();
	DrawFrameInfo(preparedImage(m_Trans.GetRoiRect()));
	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(&IplImage(preparedImage));
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}

void CPictureBox::SetFrameInfo(const FrameInfo& frameInfo)
{
	m_pFaceInfoManager->SetFrameInfo(frameInfo);
	Invalidate(FALSE);
}

void CPictureBox::SetHighLight(size_t nIndex)
{
	m_pFaceInfoManager->SetHighLight(nIndex);
	Invalidate(FALSE);
}

bool CPictureBox::GetActiveBox(cv::Rect& activeBox) const
{
	if (m_ActivePoints[0] == INIT_POINT || m_ActivePoints[1] == INIT_POINT)
	{
		return false;
	}

	if (m_ActivePoints[0] == m_ActivePoints[1] )
	{
		return false;
	}
	activeBox = cv::Rect(m_Trans.Trans({ m_ActivePoints[0], m_ActivePoints[1] }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Roi));
	return true;
}

// std::vector<size_t> CPictureBox::GetDeleteFrameInfo() const
// {
// 	return std::move(m_DeleteFaceInfoIndexes);
// }
// 
// void CPictureBox::CacheDeleteFrameInfo(const std::vector<size_t>& deletedFaceInfoIndex)
// {
// 	m_DeleteFaceInfoIndexes = deletedFaceInfoIndex;
// }
// void CPictureBox::CacheDeleteArea()
// {
// 	m_DeleteArea = cv::Rect(m_Trans.Trans({ m_ActivePoints[0], m_ActivePoints[1] }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw));
// 	std::cout << "Delete Area:" << m_DeleteArea << std::endl;
// }

void CPictureBox::SetImage(const cv::Mat& image)
{
	if (m_image.empty())
	{
		CRect rc;
		this->GetWindowRect(&rc);
		m_image = cv::Mat(rc.Height(), rc.Width(), CV_8UC3);
	}
	if (m_image.size() != image.size())
	{
		m_Trans = Transformer::Make(m_image.size(), image.size());
		m_image = cv::Mat(m_image.size(),CV_8UC3, Black);
	}
	cv::resize(image, m_image(m_Trans.GetRoiRect()), m_Trans.GetRoiRect().size());
}

void CPictureBox::PreSubclassWindow()
{
	DWORD dwStyle = GetStyle();
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_OWNERDRAW);
	CStatic::PreSubclassWindow();
}

void CPictureBox::DrawFrameInfo(cv::Mat& img)
{
	std::vector<FaceInfoEx> facesInfo = m_pFaceInfoManager->GetFacesInfoEx();
// 	if (m_nModifiedFaceInfoIndex == -1)
// 	{
// 		cv::Rect rc;
// 		if (GetActiveBox(rc))
// 		{
// 			m_drawables.push_back(new DBox(rc, ColorUnsaved));
// 		}
// 	}
	m_pState->DrawActiveBox();
	for (auto& faceInfo:facesInfo)
	{
		if (!faceInfo.bIsSelected && !faceInfo.bIsHighLight && faceInfo.bSaved)
		{
			m_drawables.push_back(new DFaceInfo(FaceInfo{ faceInfo.GetFaceInfo().strPersonName, m_Trans.Trans(faceInfo.GetFaceInfo().box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi) }, ColorSaved));
		}
		else if (!faceInfo.bIsSelected && !faceInfo.bIsHighLight && !faceInfo.bSaved)
		{
			m_drawables.push_back(new DFaceInfo(FaceInfo{ faceInfo.GetFaceInfo().strPersonName, m_Trans.Trans(faceInfo.GetFaceInfo().box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi) }, ColorUnsaved));
		}
		else if (faceInfo.bIsSelected && !faceInfo.bIsHighLight)
		{
			FaceInfoEx ex{ faceInfo.GetFaceInfo().strPersonName, m_Trans.Trans(faceInfo.GetFaceInfo().box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi), true, false, true };
			m_drawables.push_back(new DEditBox(ex.GetEditBox().rc, ex.GetEditBox().editMark,ex.GetFaceInfo().strPersonName,ex.GetFaceInfo().box.tl(), ColorIllegal));
		}
		else if (faceInfo.bIsHighLight)
		{
			m_drawables.push_back(new DFaceInfo(faceInfo.GetFaceInfo(), ColorHighLight));
		}
	}
	for (auto& drawable : m_drawables)
	{
		drawable->Draw(img);
		delete drawable;
	}
	m_drawables.clear();
}

void CPictureBox::Undo()
{
	m_pFaceInfoManager->Undo();
	Invalidate(FALSE);
}

void CPictureBox::Redo()
{
	m_pFaceInfoManager->Redo();
	Invalidate(FALSE);

}

void CPictureBox::SetDrawable(bool drawable)
{
	m_bDrawable = drawable;
	if (!m_bDrawable)
	{
		SetState("INIT");
	}
	else
	{
		SetState("AddType");
	}
	std::cout << "drawable? " << m_bDrawable << std::endl;
}

void CPictureBox::SetEditType(size_t nEditType)
{ 
	m_nEditType = nEditType;
	if (m_nEditType == SELECT_MARK)
	{
		SetState("Select");
	}
	else
	{
		SetState("AddType");
	}
	m_bDrawable = true;
}

void CPictureBox::HighLightDeleteFaceInfo()
{

}





void CPictureBox::SelectBox()
{
	cv::Rect square = m_Trans.Trans({ m_ActivePoints[0], m_ActivePoints[1] }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw);
	m_pFaceInfoManager->Select(square);
}

void CPictureBox::DeleteSelectedFacesInfo()
{
	m_pFaceInfoManager->DeleteSelected();
}

unsigned int CPictureBox::ValidateFaceInfo()
{
	return ((CVideoMarker2Dlg*)GetParent())->ValidateFaceInfo();
}

FrameInfo CPictureBox::GetFrameInfo() const
{
	return m_pFaceInfoManager->GetFrameInfo();
}

void CPictureBox::ClearHighLight()
{
	m_pFaceInfoManager->ClearHighLight();
}

void CPictureBox::SetState(const std::string& state)
{
	if (state == "INIT")
	{
		m_pState = new CPBInitState(this);
	}
	else if (state == "AddType")
	{
		m_pState = new CPBAddState(this);
	}
	else if (state == "Select")
	{
		m_pState = new CPBSelectState(this);
	}

	std::cout << "Current State::::::" << state << std::endl;
}

bool CPictureBox::IsInRoi(const CPoint& point)
{
	cv::Rect RoiRect = m_Trans.GetRoiRect();
	return (point.x <= (RoiRect.x + RoiRect.width)) && (point.y <= RoiRect.height);
}

bool CPictureBox::GetActiveBoxEx(cv::Rect& activeBox) const
{
	if (!GetActiveBox(activeBox))
	{
		return false;
	}
	cv::Rect RoiRect = m_Trans.GetRoiRect();
	return (RoiRect & activeBox) == activeBox;
}

void CPictureBox::ClearSelectEditPoint()
{
	m_nModifiedFaceInfoIndex = -1;
	m_pFaceInfoManager->ClearSelectEditPoint();
}




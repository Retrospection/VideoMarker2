// PictureBox.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoMarker2.h"
#include "PictureBox.h"
#include "NameInputDialog.h"

#include "CvvImage.h"
#include "DBox.h"
#include "DText.h"

#include "StringHelper.h"

//#include "Transformer.h"

// CPictureBox

IMPLEMENT_DYNAMIC(CPictureBox, CStatic)


const cv::Point INIT_POINT = { -1, -1 };

const cv::Scalar Green{ 0, 255, 0 };
const cv::Scalar Red{ 0, 0, 255 };
const cv::Scalar Black{ 0, 0, 0 };

const cv::Scalar ColorUnsaved = Red;
const cv::Scalar ColorSaved = Red;
const cv::Scalar ColorHighLight = Green;

CPictureBox::CPictureBox(CStateBase* pState) :CStatic(), m_Trans(Transformer::Default())
{
	m_pState = pState;
	m_bDrawing = false;

	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;
}

CPictureBox::~CPictureBox()
{
// 	for (auto& item:m_drawables)
// 	{
// 		delete item;
// 	}
}

BEGIN_MESSAGE_MAP(CPictureBox, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//	ON_WM_PAINT()
END_MESSAGE_MAP()

// CPictureBox 消息处理程序

void CPictureBox::SetState(CStateBase* pState)
{
	m_pState = pState; 
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


void CPictureBox::SetImage(const cv::Mat& image)
{
//	m_pTrans = &((CVideoMarker2Dlg*)GetParent())->m_Trans;

	if (m_image.empty())
	{
		CRect rc;
		this->GetWindowRect(&rc);
		m_image = cv::Mat(rc.Height(), rc.Width(), CV_8UC3);
	}
//	assert(m_image.size() == image.size());
	if (m_image.size() != image.size())
	{
		m_Trans = Transformer::Make(m_image.size(), image.size());
		m_image = cv::Mat(m_image.size(),CV_8UC3, Black);
	}
	cv::resize(image, m_image(m_Trans.GetRoiRect()), m_Trans.GetRoiRect().size());
}

std::vector<cv::Rect> CPictureBox::GetUnsavedBoxesInRaw()
{
	std::vector<cv::Rect> ret;
	for (const auto& rect : m_boxes)
	{
		ret.push_back(m_Trans.Trans(rect,Transformer::Coordinate::Roi, Transformer::Coordinate::Raw));
	}
	return std::move(ret);
}

// std::vector<cv::Rect> CPictureBox::GetUnsavedBoxesInRoi()
// {
// 	return m_boxes;
// }



void CPictureBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bDrawing)
	{
		return;
	} 

	m_bDrawing = false;

	// 此处保证了位于 m_boxes 中的所有盒子所处的坐标系为 roi 坐标系
// 	cv::Rect activeBox;
// 	if (GetActiveBox(activeBox) && ((CVideoMarker2Dlg*)GetParent())->OnNameSaved())
// 	{
// 		m_boxes.push_back(activeBox);
// 	}


	CNameInputDialog dlg;
	cv::Rect activeBox;
	if (dlg.DoModal() == IDOK && GetActiveBox(activeBox))
	{
		m_boxes.push_back(activeBox);
		m_UnsavedNames.push_back(CStringHelper::ConvertCStringToString(dlg.m_strPersonName));
	}


	Invalidate(FALSE);
	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;
	CStatic::OnLButtonUp(nFlags, point);
}





void CPictureBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bDrawing)
	{
		return;
	}
	m_ActivePoints[1] = { point.x, point.y };
//	((CVideoMarker2Dlg*)GetParent())->PrepareImage();
	Invalidate(FALSE);
	CStatic::OnMouseMove(nFlags, point);
}

void CPictureBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDrawing = true;
	m_ActivePoints[0] = { point.x, point.y };
	Invalidate(FALSE);
	CStatic::OnLButtonDown(nFlags, point);
}


void CPictureBox::PreSubclassWindow()
{
	DWORD dwStyle = GetStyle();
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_OWNERDRAW);
	CStatic::PreSubclassWindow();
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

void CPictureBox::DrawFrameInfo(cv::Mat& img)
{

//	m_FrameInfo = ((CVideoMarker2Dlg*)GetParent())->m_FrameInfo;

//	m_HighLights = { ((CVideoMarker2Dlg*)GetParent())->m_HighLight };


	for (const auto& faceInfo : m_FrameInfo.facesInfo)
	{
		cv::Rect rc = m_Trans.Trans(faceInfo.box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi);
		m_drawables.push_back(new DBox(rc, ColorSaved));
		m_drawables.push_back(new DText(faceInfo.strPersonName, { rc.x, rc.y }, ColorSaved));
	}



// 	for (auto& box : m_boxes)
// 	{
// 		m_drawables.push_back(new DBox(box, ColorUnsaved));
// 	}
// 
// 	for (const auto& unsavedName : m_UnsavedNames)
// 	{

// 		m_drawables.push_back()
// 	}

	assert(m_boxes.size() == m_UnsavedNames.size());
	for (size_t i = 0; i < m_boxes.size();++i)
	{
		m_drawables.push_back(new DBox(m_boxes[i], ColorUnsaved));
		m_drawables.push_back(new DText(m_UnsavedNames[i], { m_boxes[i].x, m_boxes[i].y }, ColorSaved));
	}


	// active box
	cv::Rect rc;
	if (GetActiveBox(rc))
	{
		m_drawables.push_back(new DBox(rc, ColorUnsaved));
	}



	for (auto& highlight:m_HighLights)
	{
		m_drawables.push_back(new DBox(m_Trans.Trans(highlight,Transformer::Coordinate::Raw, Transformer::Coordinate::Roi), ColorHighLight));
	}

	for (auto& drawable : m_drawables)
	{
		drawable->Draw(img);
		delete drawable;
	}
	
	m_drawables.clear();
}

void CPictureBox::SetHighLight(const std::vector<cv::Rect>& highLight)
{
	m_HighLights = highLight;
	Invalidate(FALSE);
}

void CPictureBox::SetFrameInfo(const FrameInfo& frameInfo)
{
	m_FrameInfo = frameInfo;
	Invalidate(FALSE);
}

void CPictureBox::ClearUnsavedBoxes()
{
	m_boxes.clear();
}

std::vector<std::string> CPictureBox::GetUnsavedNames() const
{
	return m_UnsavedNames;
}

void CPictureBox::ClearUnsavedNames()
{
	m_UnsavedNames.clear();
}



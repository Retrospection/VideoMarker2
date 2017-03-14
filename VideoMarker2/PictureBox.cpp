// PictureBox.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoMarker2.h"
#include "PictureBox.h"
#include "NameInputDialog.h"

#include "CvvImage.h"

#include "Transformer.h"

// CPictureBox

IMPLEMENT_DYNAMIC(CPictureBox, CStatic)

CPictureBox::CPictureBox(CStateBase* pState) :CStatic()
{
	m_pState = pState;
	m_bDrawing = false;
	m_pTrans = nullptr;
}

CPictureBox::~CPictureBox()
{
}

BEGIN_MESSAGE_MAP(CPictureBox, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//	ON_WM_PAINT()
END_MESSAGE_MAP()

// CPictureBox 消息处理程序

void CPictureBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDrawing = true;
	m_ActivePoints[0] = { point.x, point.y };
	Invalidate(FALSE);
	CStatic::OnLButtonDown(nFlags, point);
}


void CPictureBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bDrawing)
	{
		return;
	}
	m_ActivePoints[1] = { point.x, point.y };
	Invalidate(FALSE);
	CStatic::OnMouseMove(nFlags, point);
}

void CPictureBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bDrawing)
	{
		return;
	}

	m_bDrawing = false;

	CNameInputDialog dlg;
	if (dlg.DoModal() == IDCANCEL)
	{
		m_ActivePoints[0] = {};
		m_ActivePoints[1] = {};
		Invalidate(FALSE);
		CStatic::OnLButtonUp(nFlags, point);
		return;
	}
	((CVideoMarker2Dlg*)GetParent())->m_AddPersonName.push_back(dlg.m_strPersonName);
	m_boxes.emplace_back(m_ActivePoints[0], m_ActivePoints[1]);
	Invalidate(FALSE);
	CStatic::OnLButtonUp(nFlags, point);
}



cv::Point CPictureBox::ConvertFromCPoint(const CPoint& point)
{
	return{ point.x, point.y };
}

void CPictureBox::SetState(CStateBase* pState)
{
	m_pState = pState; 
}

cv::Point CPictureBox::ConvertMousePointToPicturePoint(const cv::Point& point)
{
	cv::Point ret;
// 	ret.x = point.x - ((CVideoMarker2Dlg*)GetParent())->GetROIRect().x;
// 	ret.y = point.y - ((CVideoMarker2Dlg*)GetParent())->GetROIRect().y;
	return ret;
}

const cv::Point* CPictureBox::GetActivePoints() const
{
	if (m_ActivePoints[0].x == 0 && m_ActivePoints[0].y == 0 && m_ActivePoints[1].x == 0 && m_ActivePoints[1].y == 0)
	{
		return nullptr;
	}

	return m_ActivePoints;
}


const cv::Rect* CPictureBox::GetActiveRect() const
{

}


void CPictureBox::SetImage(const cv::Mat& image)
{
	if (m_image.empty())
	{
		CRect rc;
		this->GetWindowRect(&rc);
		m_image = cv::Mat(rc.Height(), rc.Width(), CV_8UC3);
	}
	assert(m_image.size() == image.size());
	m_image = image;
}

std::vector<cv::Rect> CPictureBox::GetUnsavedBoxes()
{
	
	std::vector<cv::Rect> ret;
	for (const auto& rect : m_boxes)
	{
		ret.push_back(m_pTrans->Trans(rect,Transformer::Coordinate::PictureBox, Transformer::Coordinate::Roi));
	}
	return std::move(ret);
}


void CPictureBox::PreSubclassWindow()
{
	// TODO:  在此添加专用代码和/或调用基类
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
	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(&IplImage(m_image));
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}

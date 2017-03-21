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


// ScopeGuard

#include <functional>
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


const cv::Point INIT_POINT = { -1, -1 };

const cv::Scalar Green{ 0, 255, 0 };
const cv::Scalar Red{ 0, 0, 255 };
const cv::Scalar Black{ 0, 0, 0 };
const cv::Scalar Blue{ 255, 0, 0 };

const cv::Scalar ColorUnsaved = Red;
const cv::Scalar ColorSaved = Red;
const cv::Scalar ColorHighLight = Green;
const cv::Scalar ColorIllegal = Blue;


const wchar_t* CPictureBox::m_AlertMessage[] = { L"", L"包围盒面积过小！", L"当前帧存在同名标注！", L"IOU过大！" };


CPictureBox::CPictureBox(CStateBase* pState) 
	:CStatic(), m_Trans(Transformer::Default()), m_nEndIndexOfUnsavedDrawables(0)

{
	m_pState = pState;
	m_bDrawing = false;
	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;
	
}

CPictureBox::~CPictureBox()
{

}

BEGIN_MESSAGE_MAP(CPictureBox, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
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

std::vector<cv::Rect> CPictureBox::GetUnsavedBoxesInRaw()
{
	std::vector<cv::Rect> ret;
	for (const auto& rect : m_UnsavedBoxes)
	{
		ret.push_back(m_Trans.Trans(rect,Transformer::Coordinate::Roi, Transformer::Coordinate::Raw));
	}
	return std::move(ret);
}



void CPictureBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bDrawing)
	{
		return;
	}
	m_ActivePoints[1] = {point.x, point.y};
	m_bDrawing = false;
	CNameInputDialog dlg;
	cv::Rect activeBox;


	if (dlg.DoModal() == IDOK && GetActiveBox(activeBox))
	{
		std::string strPersonName = CStringHelper::ConvertCStringToString(dlg.m_strPersonName);
		SaveFaceInfo({ strPersonName, m_Trans.Trans(activeBox,Transformer::Coordinate::Roi, Transformer::Coordinate::Raw )});
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

	cv::Rect RoiRect = m_Trans.GetRoiRect();

	if ((point.x > (RoiRect.x + RoiRect.width)) || (point.y >RoiRect.height))
	{
		m_bDrawing = false;
		m_ActivePoints[0] = INIT_POINT;
		m_ActivePoints[1] = INIT_POINT;
		Invalidate(FALSE);
		CStatic::OnMouseMove(nFlags, point);
		return;
	}

	m_ActivePoints[1] = { point.x, point.y };
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
	for (const auto& faceInfo : m_FrameInfo.facesInfo)
	{
		cv::Rect rc = m_Trans.Trans(faceInfo.box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi);
		m_drawables.push_back(new DBox(rc, ColorSaved));
		m_drawables.push_back(new DText(faceInfo.strPersonName, { rc.x, rc.y }, ColorSaved));
	}

// 	assert(m_UnsavedBoxes.size() == m_UnsavedNames.size());
// 	for (size_t i = 0; i < m_UnsavedBoxes.size();++i)
// 	{
// 		m_drawables.push_back(new DBox(m_UnsavedBoxes[i], ColorUnsaved));
// 		m_drawables.push_back(new DText(m_UnsavedNames[i], { m_UnsavedBoxes[i].x, m_UnsavedBoxes[i].y }, ColorSaved));
// 	}

	for (size_t i = 0; i < m_nEndIndexOfUnsavedDrawables; ++i)
	{
		m_drawables.push_back(new DBox(m_UnsavedBoxes[i], ColorUnsaved));
		m_drawables.push_back(new DText(m_UnsavedNames[i], { m_UnsavedBoxes[i].x, m_UnsavedBoxes[i].y }, ColorSaved));
	}

	cv::Rect rc;
	if (GetActiveBox(rc))
	{
		m_drawables.push_back(new DBox(rc, ColorUnsaved));
	}

	for (auto& illegal: m_IllegalFaceInfo)
	{
		m_drawables.push_back(new DBox(m_Trans.Trans(illegal.box,Transformer::Coordinate::Raw,Transformer::Coordinate::Roi), ColorIllegal));
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
	m_UnsavedBoxes.clear();
	m_nEndIndexOfUnsavedDrawables = 0;
}

std::vector<std::string> CPictureBox::GetUnsavedNames() const
{
	return m_UnsavedNames;
}

void CPictureBox::ClearUnsavedNames()
{
	m_UnsavedNames.clear();
	m_nEndIndexOfUnsavedDrawables = 0;
}

void CPictureBox::Undo()
{
	if (m_nEndIndexOfUnsavedDrawables == 0)
	{
		return;
	}
	--m_nEndIndexOfUnsavedDrawables;
	Invalidate(FALSE);
}

void CPictureBox::Redo()
{
	assert(m_UnsavedNames.size() == m_UnsavedBoxes.size());
	if (m_nEndIndexOfUnsavedDrawables == m_UnsavedBoxes.size())
	{
		return;
	}
	++m_nEndIndexOfUnsavedDrawables;
	Invalidate(FALSE);
}

void CPictureBox::SetIllegal(const FaceInfo& info, size_t index)
{
	m_IllegalFaceInfo.push_back(info);
	m_IllegalIndex.push_back(index);
}

void CPictureBox::DecreaseEndIndex()
{
	assert(m_nEndIndexOfUnsavedDrawables > 0);
	--m_nEndIndexOfUnsavedDrawables;
}

void CPictureBox::SaveFaceInfo(const FaceInfo& faceInfo)
{
	unsigned int validateResult = ((CVideoMarker2Dlg*)GetParent())->ValidateFaceInfo(faceInfo);
	assert(validateResult < NUMBER_OF_VALIDATOR_TYPES);
	if (validateResult != 0)
	{
		MessageBox(m_AlertMessage[validateResult]);
		return;
	}

	if (m_nEndIndexOfUnsavedDrawables != m_UnsavedBoxes.size())
	{
		m_UnsavedBoxes.resize(m_nEndIndexOfUnsavedDrawables);
		m_UnsavedNames.resize(m_nEndIndexOfUnsavedDrawables);
	}
	m_UnsavedBoxes.push_back(m_Trans.Trans(faceInfo.box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi));
	m_UnsavedNames.push_back(faceInfo.strPersonName);
	++m_nEndIndexOfUnsavedDrawables;

}

// bool CPictureBox::GetActiveBox2(RRect& rr) const
// {
// 	return true;
// }



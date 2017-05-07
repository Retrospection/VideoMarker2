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
#include "PBModifyState.h"

#include "FaceInfoManager.h"

#include <iostream>
#include <functional>

#include <deque>



const cv::Point INIT_POINT = { -1, -1 };



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
	:CStatic(), m_Trans(Transformer::Default()), m_ActiveBoxManager(&m_Trans), m_pState(nullptr)
{
	m_bDrawing = false;
	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;
	
	m_nModifiedFaceInfoIndex = -1;
	m_pFaceInfoManager = new FaceInfoManager(&m_Trans);
	SetState("INIT");
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
	Invalidate(FALSE);
	CStatic::OnLButtonDown(nFlags, point);
}


void CPictureBox::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pState->OnMouseMove(nFlags, point);
	Invalidate(FALSE);
	CStatic::OnMouseMove(nFlags, point);
}

void CPictureBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_pState->OnLButtonUp(nFlags, point);
	((CVideoMarker2Dlg*)GetParent())->Refresh();
	m_ActiveBoxManager.ResetActiveBox();
	CStatic::OnLButtonUp(nFlags, point);
}

FrameInfo CPictureBox::GetUnsavedFrameInfo() const
{
	FrameInfo ret;
	ret.facesInfo = m_pFaceInfoManager->GetUnsavedFacesInfo();
	return ret;
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
	// TODO : 更新DLG中的FrameInfo，从此函数更新的FrameInfo，两个flag都为false
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
	m_pState->DrawActiveBox();
	m_pState->DrawSavedFacesInfo();
	m_pState->DrawSelectedFacesInfo();
	m_pState->DrawHighlightFacesInfo();
	for (auto& drawable : m_DrawableActiveBox)
	{
		drawable->Draw(img);
	}

	for (auto& drawable : m_DrawableSavedFacesInfo)
	{
		drawable->Draw(img);
	}

	for (auto& drawable : m_DrawableSelectedFacesInfo)
	{
		drawable->Draw(img);
	}

	for (auto& drawable : m_DrawableHighlightFacesInfo)
	{
		drawable->Draw(img);
	}
}

void CPictureBox::Undo()
{
	m_pFaceInfoManager->Undo();
	//Invalidate(FALSE);
}

void CPictureBox::Redo()
{
	m_pFaceInfoManager->Redo();
	//Invalidate(FALSE);

}


void CPictureBox::SetEditType(size_t nEditType)
{
	m_pState->SetState(nEditType);
}


void CPictureBox::SelectBox()
{
	cv::Rect square;
	if (m_ActiveBoxManager.GetActiveBoxInRoi(square))
	{
		square = m_Trans.Trans(square, Transformer::Coordinate::Roi, Transformer::Coordinate::Raw);
	}
	m_pFaceInfoManager->SelectBBox(square);
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
	printf("set state to %s\n", state.c_str());
	m_pFaceInfoManager->ResetSelect();
	SAFE_DELETE(m_pState);
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
	else if (state == "Modify")
	{
		m_pState = new CPBModifyState(this);
	}
	
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
	m_pFaceInfoManager->ResetSelectedEditPoint();
}




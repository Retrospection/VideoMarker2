// PictureBox.cpp : 实现文件


#include "stdafx.h"
#include "VideoMarker2.h"
#include "PictureBox.h"
#include "NameInputDialog.h"

#include "CvvImage.h"
#include "DBox.h" 
#include "DText.h"
#include "DEditBox.h"

#include "StringHelper.h"

#include "VideoMarker2Dlg.h"
//#include "Transformer.h"

#include <iostream>

// ScopeGuard

#include <functional>



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


class SelectItemManager
{
public:
	SelectItemManager(){}

public:
	void Clear()
	{
		m_SelectedItems.clear();
	}

	void Select(cv::Rect& rc)
	{
		assert(m_SelectedItems.empty());
		m_SelectedItems.emplace_back(rc);
	}

	void Select(std::vector<FaceInfo>& facesInfo, std::vector<cv::Rect>& m_UnsavedBoxes, const cv::Rect& square)
	{
		m_SelectedItems.clear();
		for (auto iter = facesInfo.begin(); iter != facesInfo.end(); ++iter)
		{
			if ((iter->box & square).area() > 0)
			{
				m_SelectedItems.emplace_back(iter->box);
			}
		}
		for (size_t i = 0; i < m_UnsavedBoxes.size(); ++i)
		{
			if ((m_UnsavedBoxes[i] & square).area() > 0)
			{
				m_SelectedItems.emplace_back(m_UnsavedBoxes[i]);
			}
		}
	}

	int SelectEditPoint(const cv::Point& point)
	{
		lastPos = point;
		nEditType = -1;
		for (const auto& box : m_SelectedItems)
		{
			if ((nEditType = box.Hit(point)) != -1)
			{
				break;
			}
		}

		return nEditType;
	}

	void Move(const cv::Point& point)
	{
		cv::Point offset = point - lastPos;
		for (auto& box : m_SelectedItems)
		{
			box.UpdateLocation(nEditType, offset);
		}
		lastPos = point;
	}

	std::vector<IDrawable*> GetIDrawable(const Transformer* pTrans) const
	{
		std::vector<IDrawable*> ret;
		for (auto& item: m_SelectedItems)
		{
			std::vector<cv::Rect> roiRect;
			for (auto& rc : item.GetEditRects())
			{
				roiRect.push_back(pTrans->Trans(rc, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi));
			}
			ret.push_back(new DEditBox(pTrans->Trans(item.GetBox(), Transformer::Coordinate::Raw, Transformer::Coordinate::Roi), roiRect, ColorIllegal));
		}
		return ret;
	}

private:
	cv::Point lastPos;
	int nEditType = -1;
	std::vector<CEditBox> m_SelectedItems;
};

// CPictureBox

IMPLEMENT_DYNAMIC(CPictureBox, CStatic)





const wchar_t* CPictureBox::m_AlertMessage[] = { L"", L"包围盒面积过小！", L"当前帧存在同名标注！", L"IOU过大！" };


CPictureBox::CPictureBox(CStateBase* pState) 
	:CStatic(), m_Trans(Transformer::Default()), m_nEndIndexOfUnsavedDrawables(0), m_bDrawable(false)

{
	m_pState = pState;
	m_bDrawing = false;
	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;
	m_DeleteArea = {};
	m_nEditPointIndex = -1;
	m_nModifiedFaceInfoIndex = -1;

	m_pSelectItemManager = new SelectItemManager();
}

CPictureBox::~CPictureBox()
{
	delete m_pSelectItemManager;
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
	if (!m_bDrawable)
	{
		return;
	}
	((CVideoMarker2Dlg*)GetParent())->OnPictureBoxLBtnUp();
	if (!m_bDrawing)
	{
		return;
	}
	m_ActivePoints[1] = {point.x, point.y};
	m_bDrawing = false;
//	CNameInputDialog dlg;
	switch (m_nEditType)
	{
	case ADD_MARK_TYPE:
	{
		cv::Rect activeBox;
		std::string strPersonName;
		if (((CVideoMarker2Dlg*)GetParent())->GetUnsavedName2(strPersonName) && GetActiveBox(activeBox))
		{
			CacheUnsaveFaceInfo({ strPersonName, m_Trans.Trans(activeBox, Transformer::Coordinate::Roi, Transformer::Coordinate::Raw) });
			//m_nEditType = CHANGE_MARK;
			//m_nModifiedFaceInfoIndex = 0;
			//m_pSelectItemManager->Select(xxx);
		}
		break;
	}
	case DELETE_MAKR_TYPE:
	{
		CacheDeleteArea();
		HighLightDeleteFaceInfo();
		break;
	}
	case CHANGE_MARK:
	{
		if (m_nModifiedFaceInfoIndex == -1)
		{
			// 当前为选择模式
			PrepareEdit();
		}
		else
		{
			// 鼠标点击区域不在 EditPoint 区域
		}
		break;
	}
	default:
		break;
	}

	Invalidate(FALSE);
	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;

	CStatic::OnLButtonUp(nFlags, point);

}

void CPictureBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bDrawable)
	{
		return;
	}
	if (!m_bDrawing)
	{
		return;
	}

	cv::Rect RoiRect = m_Trans.GetRoiRect();


	if (m_nEditType == CHANGE_MARK)
	{
// 		if (m_ModifiedFaceInfo.empty())
// 		{
// 			m_ModifiedFaceInfo.push_back(&m_FrameInfo.facesInfo[0]);
// 		}
// 
// 		m_ModifiedFaceInfo[0]->box.width += 3;
// 		m_ModifiedFaceInfo[0]->box.height += 3; 		

		// 改变选中的box

		if (m_nModifiedFaceInfoIndex != -1)
		{
			cv::Point _point = m_Trans.Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl();
			m_pSelectItemManager->Move(_point);
		}
	}
	else
	{

		if ((point.x > (RoiRect.x + RoiRect.width)) || (point.y > RoiRect.height))
		{
			m_bDrawing = false;
			m_ActivePoints[0] = INIT_POINT;
			m_ActivePoints[1] = INIT_POINT;
			Invalidate(FALSE);
			CStatic::OnMouseMove(nFlags, point);
			return;
		}
	}

	m_ActivePoints[1] = { point.x, point.y };
	Invalidate(FALSE);
	CStatic::OnMouseMove(nFlags, point);
}

void CPictureBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bDrawable)
	{
		return;
	}
	((CVideoMarker2Dlg*)GetParent())->OnPictureBoxLBtnDown();
	if (m_bDrawing)
	{
		m_ActivePoints[1] = { point.x, point.y };
		Invalidate(FALSE); 
		CStatic::OnLButtonDown(nFlags, point);
	}
	else
	{
		m_bDrawing = true;
		
		// 选中EditPoint
		cv::Point _point{ m_Trans.Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl() };
		m_nModifiedFaceInfoIndex = m_pSelectItemManager->SelectEditPoint(_point);
		std::cout << "EditBoxes Index: " << m_nModifiedFaceInfoIndex << std::endl;
	
// 		if (!SetEditPoint({ point.x, point.y }))
// 		{
// 			// 鼠标点击区域不在 EditPoint 区域
// 			m_ModifiedFaceInfo.clear();
// 			m_EditPoints.clear();
// 		}
// 		else
// 		{
// 			// 鼠标点击区域位于 EditPoint 区域
// 
// 
// 		}

		m_ActivePoints[0] = { point.x, point.y };
		Invalidate(FALSE);
		CStatic::OnLButtonDown(nFlags, point);
	}
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

	if (m_nModifiedFaceInfoIndex == -1)
	{
		cv::Rect rc;
		if (GetActiveBox(rc))
		{
			m_drawables.push_back(new DBox(rc, ColorUnsaved));
		}
	}

	std::vector<IDrawable*> drawables = m_pSelectItemManager->GetIDrawable(&m_Trans);
	m_drawables.insert(m_drawables.end(), drawables.begin(), drawables.end());

	for (auto& deleted: m_ToBeDeleteFaceInfo)
	{
		m_drawables.push_back(new DBox(m_Trans.Trans(deleted.box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi), ColorIllegal));
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
// 	m_IllegalFaceInfo.push_back(info);
// 	m_IllegalIndex.push_back(index);
}

void CPictureBox::DecreaseEndIndex()
{
	assert(m_nEndIndexOfUnsavedDrawables > 0);
	--m_nEndIndexOfUnsavedDrawables;
}

void CPictureBox::CacheUnsaveFaceInfo(const FaceInfo& faceInfo)
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

void CPictureBox::SetDrawable(bool drawable)
{
	m_bDrawable = drawable;
	std::cout << "drawable? " << m_bDrawable << std::endl;
}

std::vector<size_t> CPictureBox::GetDeleteFrameInfo() const
{
	return std::move(m_DeleteFaceInfoIndexes);
}

void CPictureBox::CalculateDeleteFrameInfoIndex()
{
	m_DeleteFaceInfoIndexes.clear();
	m_DeleteUnsavedFaceInfoIndexes.clear();
	for (size_t i = 0; i < m_FrameInfo.facesInfo.size(); ++i)
	{
		if ((m_DeleteArea & m_FrameInfo.facesInfo[i].box).area() > 0)
		{
			m_DeleteFaceInfoIndexes.push_back(i);
		}
	}
	for (size_t i = 0; i < m_UnsavedBoxes.size(); ++i)
	{
		if ((m_DeleteArea & m_UnsavedBoxes[i]).area() > 0)
		{
			m_DeleteUnsavedFaceInfoIndexes.push_back(i);
		}
	}
}

void CPictureBox::CacheDeleteFrameInfo(const std::vector<size_t>& deletedFaceInfoIndex)
{
	m_DeleteFaceInfoIndexes = deletedFaceInfoIndex;
}

void CPictureBox::ClearDeleteFrameInfo()
{
	m_DeleteFaceInfoIndexes.clear();
	m_DeleteUnsavedFaceInfoIndexes.clear();
}

void CPictureBox::SetEditType(size_t nEditType)
{
	m_nEditType = nEditType;
}

void CPictureBox::CacheDeleteArea()
{
	m_DeleteArea = cv::Rect(m_Trans.Trans({ m_ActivePoints[0], m_ActivePoints[1] }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw));
	std::cout << "Delete Area:" << m_DeleteArea << std::endl;
}

void CPictureBox::DeleteUnsavedFaceInfo()
{
// 	std::vector<cv::Rect> newUnsavedBoxes;
// 	std::vector<std::string> newUnsavedNames;
// 	for (size_t i = 0; i < m_UnsavedBoxes.size(); ++i)
// 	{
// 		if (std::none_of(m_DeleteUnsavedFaceInfoIndexes.begin(), m_DeleteUnsavedFaceInfoIndexes.end(), [&](int index){return index == i; }))
// 		{
// 			newUnsavedBoxes.push_back(m_UnsavedBoxes[i]);
// 			newUnsavedNames.push_back(m_UnsavedNames[i]);
// 		}
// 	}
// 	m_UnsavedBoxes = newUnsavedBoxes;
// 	m_UnsavedNames = newUnsavedNames;

	std::sort(m_DeleteUnsavedFaceInfoIndexes.begin(), m_DeleteUnsavedFaceInfoIndexes.end(), std::greater<size_t>());
	for (auto index:m_DeleteUnsavedFaceInfoIndexes)
	{
		m_UnsavedBoxes.erase(m_UnsavedBoxes.begin() + index);
		m_UnsavedNames.erase(m_UnsavedNames.begin() + index);
		DecreaseEndIndex();
	}

}

void CPictureBox::HighLightDeleteFaceInfo()
{
	CalculateDeleteFrameInfoIndex();
	for (auto index : m_DeleteFaceInfoIndexes)
	{
		m_ToBeDeleteFaceInfo.push_back(m_FrameInfo.facesInfo[index]);
	}
	for (auto index : m_DeleteUnsavedFaceInfoIndexes)
	{
		m_ToBeDeleteFaceInfo.push_back({ m_UnsavedNames[index], m_UnsavedBoxes[index] });
	}
}

void CPictureBox::ClearToBeDeleted()
{
	m_ToBeDeleteFaceInfo.clear();
}

std::vector<FaceInfo> CPictureBox::GetModifiedFacesInfo() const
{
	return{};
}


void CPictureBox::PrepareEdit()
{
// 	m_ModifiedFaceInfo.clear();
// 	m_EditPoints.clear();
// 	cv::Rect square = m_Trans.Trans({ m_ActivePoints[0], m_ActivePoints[1] }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw);
// 	for (auto iter = m_FrameInfo.facesInfo.begin(); iter != m_FrameInfo.facesInfo.end(); ++iter)
// 	{
// 		if ((iter->box & square).area() > 0)
// 		{
// 			m_ModifiedFaceInfo.push_back(&(*iter));
// 			CalculateEditPoints(iter->box);
// 		}
// 	}
// 	for (size_t i = 0; i < m_UnsavedBoxes.size(); ++i)
// 	{
// 		if ((m_UnsavedBoxes[i] & square).area() > 0)
// 		{
// 			m_ModifiedFaceInfo.push_back()
// 		}
// 	}

	cv::Rect square = m_Trans.Trans({ m_ActivePoints[0], m_ActivePoints[1] }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw);
	m_pSelectItemManager->Select(m_FrameInfo.facesInfo, m_UnsavedBoxes, square);
}

FrameInfo CPictureBox::GetUnsavedFrameInfo() const
{
	FrameInfo ret;
	for (size_t i = 0; i < m_UnsavedBoxes.size(); ++i)
	{
		ret.facesInfo.push_back({ m_UnsavedNames[i], m_UnsavedBoxes[i] });
	}
	return std::move(ret);
}

void CPictureBox::ClearEditBoxes()
{
	//auto& m_EditBoxes = Getm_EditBoxes();
	//m_EditBoxes.clear();
	m_pSelectItemManager->Clear();
}

//std::vector<CEditBox>& CPictureBox::Getm_EditBoxes()
//{
//	return m_pSelectItemManager->m_SelectedItems;
//}











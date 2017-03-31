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

class FaceInfoEx
{
public:


	int Hit(const cv::Point& pt) const { return -1; }
	void UpdateLocation(int editType, const cv::Point& offset){}
	FaceInfo GetFaceInfo() const{ return info; }
	CEditBox GetEditBox() const  
	{
		cv::Rect box = info.box;
		CEditBox ret(box);
		return ret; 
	}

	FaceInfo info;
	bool bIsSelected;
	//	bool bIsHighLight;

};

class FaceInfoManager
{
public:
	FaceInfoManager()
		:m_nPos(0)
	{
		m_FacesInfo.push_back({});
	}

	~FaceInfoManager(){}

	void SetFacesInfo(const FrameInfo& frameInfo)
	{
		m_FacesInfo.clear();
		m_nPos = 0;
		std::vector<FaceInfoEx> temp(frameInfo.facesInfo.size());
		std::transform(frameInfo.facesInfo.begin(), frameInfo.facesInfo.end(), temp.begin(), [](const FaceInfo& info){ return FaceInfoEx{  info , false }; });
		m_FacesInfo.push_back(temp);

	}

	std::vector<IDrawable*> GetIDrawables() const
	{
		std::vector<IDrawable*> ret;
		for (const auto& faceInfo : m_FacesInfo[m_nPos])
		{
			if (!faceInfo.bIsSelected /*&& !faceInfo.bIsHighLight*/ )
			{
				ret.push_back(new DFaceInfo(faceInfo.GetFaceInfo(), ColorSaved));
			}
			else if (faceInfo.bIsSelected /*&& !faceInfo.bIsHighLight*/)
			{
				ret.push_back(new DEditBox(faceInfo.GetEditBox().GetBox(), faceInfo.GetEditBox().GetEditRects(), ColorIllegal));
			}
			else
			{
				ret.push_back(new DFaceInfo(faceInfo.GetFaceInfo(), ColorHighLight));
			}
		}
		return ret;
	}

	unsigned int Add(const FaceInfo& faceInfo)
	{
		SnapShot();
		m_FacesInfo.rbegin()->push_back({ faceInfo });
		std::cout << ToString();
		return 0;
	}

	void Delete()
	{
		SnapShot();
		m_FacesInfo[m_nPos].erase(std::remove_if(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), [](const FaceInfoEx& info){ return info.bIsSelected; }), m_FacesInfo[m_nPos].end());
	}



	void Select(const cv::Rect& rc)
	{
		for (auto& faceInfo:m_FacesInfo[m_nPos])
		{
			if (IsOverlapping(rc,faceInfo.GetFaceInfo().box))
			{
				faceInfo.bIsSelected = true;
			}
			else
			{
				faceInfo.bIsSelected = false;
			}
		}
	}

	int SelectEditPoint(const cv::Point& point)
	{
		m_LastPos = point;
		m_nEditType = -1;
		for (const auto& box : m_FacesInfo[m_nPos])
		{
			if (!box.bIsSelected)
			{
				continue;
			}
			if ((m_nEditType = box.Hit(point)) != -1)
			{
				break;
			}
		}
		return m_nEditType;
	}


	void MoveFinished(const cv::Point& point)
	{
 		SnapShot();
	}

	void Move(const cv::Point& point)
	{
		cv::Point offset = point - m_LastPos;
		for (auto& box : m_FacesInfo[m_nPos])
		{
			if (box.bIsSelected)
			{
				box.UpdateLocation(m_nEditType, offset);
			}
		}
		m_LastPos = point;
	}

	void Undo()
	{
		if (m_nPos == 0)
		{
			return;
		}
		--m_nPos;
		std::cout << ToString();
	}

	void Redo()
	{
		if (m_nPos == m_FacesInfo.size() - 1)
		{
			return;
		}
		++m_nPos;
		std::cout << ToString();
	}

private:
	std::string ToString() const
	{
		std::stringstream ss;
		ss << "当前有 " << m_FacesInfo[m_nPos].size() << " 个 faceinfo， 游标位于 " << m_nPos << " 快照总长度： " << m_FacesInfo.size() << std::endl;
		return ss.str();
	}
	void SnapShot()
	{
		if (m_nPos != m_FacesInfo.size() - 1)
		{
			m_FacesInfo.resize(1 + m_nPos);
		}
		if (m_FacesInfo.size() >= 5)
		{
			m_FacesInfo.erase(m_FacesInfo.begin());
			--m_nPos;
		}
		m_FacesInfo.push_back(m_FacesInfo[m_nPos]);
		++m_nPos;
	}

	bool IsOverlapping(const cv::Rect& rc1, const cv::Rect& rc2)const
	{
		return true;
	}


	int m_nEditType;
	cv::Point m_LastPos;

	std::vector<std::vector<FaceInfoEx>> m_FacesInfo;
	size_t m_nPos;

};


CPictureBox::CPictureBox(CStateBase* pState) 
	:CStatic(), m_Trans(Transformer::Default()), m_bDrawable(false)

{
	m_bDrawing = false;
	m_ActivePoints[0] = INIT_POINT;
	m_ActivePoints[1] = INIT_POINT;
	m_DeleteArea = {};
	m_nModifiedFaceInfoIndex = -1;
	m_pSelectItemManager = new SelectItemManager();
	m_pFaceInfoManager = new FaceInfoManager();
}

CPictureBox::~CPictureBox()
{
	delete m_pSelectItemManager;
	delete m_pFaceInfoManager;
}

BEGIN_MESSAGE_MAP(CPictureBox, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CPictureBox 消息处理程序


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
		//m_nModifiedFaceInfoIndex = m_pSelectItemManager->SelectEditPoint(_point);
		m_nModifiedFaceInfoIndex = m_pFaceInfoManager->SelectEditPoint(_point);
		std::cout << "EditBoxes Index: " << m_nModifiedFaceInfoIndex << std::endl;
	
		m_ActivePoints[0] = { point.x, point.y };
		Invalidate(FALSE);
		CStatic::OnLButtonDown(nFlags, point);
	}
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
		// 改变选中的box
		if (m_nModifiedFaceInfoIndex != -1)
		{
			cv::Point _point = m_Trans.Trans({ point.x, point.y, 1, 1 }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw).tl();
			//m_pSelectItemManager->Move(_point);
			m_pFaceInfoManager->Move(_point);
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
	switch (m_nEditType)
	{
	case ADD_MARK_TYPE:
	{
		cv::Rect activeBox;
		std::string strPersonName;

// 		if (((CVideoMarker2Dlg*)GetParent())->GetUnsavedName2(strPersonName) && GetActiveBox(activeBox))
// 		{
// 			CacheUnsaveFaceInfo({ strPersonName, m_Trans.Trans(activeBox, Transformer::Coordinate::Roi, Transformer::Coordinate::Raw) });
// 		}
		((CVideoMarker2Dlg*)GetParent())->GetUnsavedName2(strPersonName);
		GetActiveBox(activeBox);
		m_pFaceInfoManager->Add({ strPersonName, activeBox });

		break;
	}
	case DELETE_MAKR_TYPE:
	{
// 		CacheDeleteArea();
// 		HighLightDeleteFaceInfo();
		m_pFaceInfoManager->Delete();
		break;
	}
	case CHANGE_MARK:
	{
		if (m_nModifiedFaceInfoIndex == -1)
		{
			// 当前为选择模式
			SelectBox();
		}
		else
		{
			// 鼠标点击区域不在 EditPoint 区域
			m_pFaceInfoManager->MoveFinished({ point.x,point.y });
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

std::vector<cv::Rect> CPictureBox::GetUnsavedBoxesInRaw()
{
	std::vector<cv::Rect> ret;
	for (const auto& rect : m_UnsavedBoxes)
	{
		ret.push_back(m_Trans.Trans(rect,Transformer::Coordinate::Roi, Transformer::Coordinate::Raw));
	}
	return std::move(ret);
}


std::vector<std::string> CPictureBox::GetUnsavedNames() const
{
	return m_UnsavedNames;
}

void CPictureBox::DeleteUnsavedFaceInfo()
{
	std::sort(m_DeleteUnsavedFaceInfoIndexes.begin(), m_DeleteUnsavedFaceInfoIndexes.end(), std::greater<size_t>());
	for (auto index:m_DeleteUnsavedFaceInfoIndexes)
	{
		m_UnsavedBoxes.erase(m_UnsavedBoxes.begin() + index);
		m_UnsavedNames.erase(m_UnsavedNames.begin() + index);
	}
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

void CPictureBox::CacheUnsaveFaceInfo(const FaceInfo& faceInfo)
 {
	unsigned int validateResult = ((CVideoMarker2Dlg*)GetParent())->ValidateFaceInfo(faceInfo);
	assert(validateResult < NUMBER_OF_VALIDATOR_TYPES);
	if (validateResult != 0)
	{
		MessageBox(m_AlertMessage[validateResult]);
		return;
	}

	m_UnsavedBoxes.push_back(m_Trans.Trans(faceInfo.box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi));
	m_UnsavedNames.push_back(faceInfo.strPersonName);

}

void CPictureBox::ClearUnsavedBoxes()
{
	m_UnsavedBoxes.clear();
}

void CPictureBox::ClearUnsavedNames()
{
	m_UnsavedNames.clear();
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
	m_FrameInfo = frameInfo;
	Invalidate(FALSE);
}

void CPictureBox::SetHighLight(const std::vector<cv::Rect>& highLight)
{
	m_HighLights = highLight;
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

std::vector<size_t> CPictureBox::GetDeleteFrameInfo() const
{
	return std::move(m_DeleteFaceInfoIndexes);
}

void CPictureBox::CacheDeleteFrameInfo(const std::vector<size_t>& deletedFaceInfoIndex)
{
	m_DeleteFaceInfoIndexes = deletedFaceInfoIndex;
}

void CPictureBox::CacheDeleteArea()
{
	m_DeleteArea = cv::Rect(m_Trans.Trans({ m_ActivePoints[0], m_ActivePoints[1] }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw));
	std::cout << "Delete Area:" << m_DeleteArea << std::endl;
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
	for (const auto& faceInfo : m_FrameInfo.facesInfo)
	{
		cv::Rect rc = m_Trans.Trans(faceInfo.box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi);
		m_drawables.push_back(new DBox(rc, ColorSaved));
		m_drawables.push_back(new DText(faceInfo.strPersonName, { rc.x, rc.y }, ColorSaved));
	}

	for (size_t i = 0; i < m_UnsavedBoxes.size(); ++i)
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
	std::cout << "drawable? " << m_bDrawable << std::endl;
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

void CPictureBox::SetEditType(size_t nEditType)
{
	m_nEditType = nEditType;
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

void CPictureBox::ClearDeleteFrameInfo()
{
	m_DeleteFaceInfoIndexes.clear();
	m_DeleteUnsavedFaceInfoIndexes.clear();
}

void CPictureBox::ClearToBeDeleted()
{
	m_ToBeDeleteFaceInfo.clear();
}



void CPictureBox::SelectBox()
{
	cv::Rect square = m_Trans.Trans({ m_ActivePoints[0], m_ActivePoints[1] }, Transformer::Coordinate::PictureBox, Transformer::Coordinate::Raw);
	//m_pSelectItemManager->Select(m_FrameInfo.facesInfo, m_UnsavedBoxes, square);
	m_pFaceInfoManager->Select(square);
}

void CPictureBox::ClearSelectedBoxes()
{
	m_pSelectItemManager->Clear();
}

unsigned int CPictureBox::ValidateFaceInfo(const FaceInfo& faceInfo)
{
	return ((CVideoMarker2Dlg*)GetParent())->ValidateFaceInfo(faceInfo);
}


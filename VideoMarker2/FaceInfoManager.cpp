
#include "stdafx.h"
#include "FaceInfoManager.h"

////////////////////////////////////////  FaceInfoEx  ///////////////////////////////////////



// Constructor
FaceInfoEx::FaceInfoEx() :m_Box(), m_strPersonName()
{
	UpdateEditRects();
}

FaceInfoEx::FaceInfoEx(const std::string& strPersonName, const cv::Rect& rc,
	bool isSelected, bool isHighLight, bool isSaved, bool isNew, bool deleted)
	: m_strPersonName(strPersonName), m_Box(*reinterpret_cast<const RectEx*>(&rc)), 
	bIsSelected(isSelected), bIsHighLight(isHighLight), bSaved(isSaved), bNewInfo(isNew), bDeleted(deleted)
{

}

// FaceInfoEx::FaceInfoEx(const FaceInfo& faceInfo) : m_Box(*reinterpret_cast<const RectEx*>(&faceInfo.box)), m_strPersonName(faceInfo.strPersonName)
// {
// 	UpdateEditRects();
// }
// 
// FaceInfoEx::FaceInfoEx(const std::string& strPersonName, const cv::Rect& rc, bool isSelected, bool isHighLight, bool isSaved) : m_Box(*reinterpret_cast<const RectEx*>(&rc)), m_strPersonName(strPersonName)
// {
// 	UpdateEditRects();
// }
// 
// FaceInfoEx::FaceInfoEx(const FaceInfo& faceInfo, bool isSelected, bool isHighLight, bool isSaved) : m_Box(*reinterpret_cast<const RectEx*>(&faceInfo.box)), m_strPersonName(faceInfo.strPersonName), bIsSelected(isSelected), bIsHighLight(isHighLight), bSaved(isSaved)
// {
// 	UpdateEditRects();
// }

int FaceInfoEx::Hit(const cv::Point& pt) const
{
	for (size_t i = 0; i < m_EditRects.size(); ++i)
	{
		if (m_EditRects[i].contains(pt))
		{
			return i;
		}
	}
	return -1;
}

void FaceInfoEx::UpdateLocation(int editType, const cv::Point& offset)
{
	m_Box.Move(editType, offset);
	UpdateEditRects();
}

FaceInfo FaceInfoEx::GetFaceInfo() const
{
	return{ m_strPersonName, *reinterpret_cast<const cv::Rect*>(&m_Box) };
}

CEditBox FaceInfoEx::GetEditBox() const
{
	CEditBox ret{ m_EditRects, m_Box };
	return ret;
}

void FaceInfoEx::UpdateEditRects()
{
	m_EditRects.clear();
	auto points = CalculateEditRects();
	for (auto& pt : points)
	{
		m_EditRects.push_back(GetEditMarkRect(pt));
	}
}

std::vector<cv::Point> FaceInfoEx::CalculateEditRects()
{
	return{ m_Box.TopLeft(), m_Box.TopMid(), m_Box.TopRight(),
		m_Box.MidLeft(), m_Box.MidRight(),
		m_Box.BottomLeft(), m_Box.BottomMid(), m_Box.BottomRight()
	};
}

cv::Rect FaceInfoEx::GetEditMarkRect(const cv::Point& point)
{
	int hw = 10;
	int hh = hw;
	cv::Point tl(point.x - hw, point.y - hh);
	cv::Point br(point.x + hw, point.y + hh);
	return{ tl, br };
}



//////////////////////////////////////////  FaceInfoManager  /////////////////////////////////////////////////////


FaceInfoManager::FaceInfoManager(Transformer* ptr) :m_nPos(0), m_bFirstMove(true), m_pTrans(ptr), m_bSavedChanged(true), m_bSelectedChanged(true), m_bHighlightChanged(true)
{
	m_FacesInfo.push_back({});
}

std::vector<FaceInfo> FaceInfoManager::GetUnsavedFacesInfo() const
{
	std::vector<FaceInfo> ret;
	for (auto& faceInfo : m_FacesInfo[m_nPos])
	{
		if (!faceInfo.bSaved)
		{
			ret.push_back(faceInfo.GetFaceInfo());
		}
	}
	return ret;
}

void FaceInfoManager::ClearUnsavedFacesInfo()
{
	SnapShot();
	auto pred = [](const FaceInfoEx& fiex){ return !fiex.bSaved; };
	m_FacesInfo[m_nPos].erase(std::remove_if(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), pred), m_FacesInfo[m_nPos].end());
}

std::vector<FaceInfo> FaceInfoManager::GetFacesInfo()
{
	std::vector<FaceInfo> ret(m_FacesInfo[m_nPos].size());
	auto fn = [](const FaceInfoEx& fiex){ return fiex.GetFaceInfo(); };
	std::transform(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), ret.begin(), fn);
	return ret;
}

std::vector<FaceInfoEx> FaceInfoManager::GetFacesInfoEx() const
{
	return m_FacesInfo[m_nPos];
}

FrameInfo FaceInfoManager::GetFrameInfo()
{
	FrameInfo ret;
	ret.facesInfo = GetFacesInfo();
	return std::move(ret);
}

void FaceInfoManager::SetFrameInfo(const FrameInfo& frameInfo)
{
	m_FacesInfo.clear();
	m_nPos = 0;
	m_bSavedChanged = true;

	std::vector<FaceInfoEx> temp(frameInfo.facesInfo.size());
	std::transform(frameInfo.facesInfo.begin(), frameInfo.facesInfo.end(), temp.begin(), [](const FaceInfo& info){ return FaceInfoEx{ info.strPersonName, info.box, false, false, true, false, false }; });
	m_FacesInfo.push_back(temp);
	SnapShot();
}

unsigned int FaceInfoManager::AddFaceInfo(const FaceInfo& faceInfo)
{
	SnapShot();
	m_FacesInfo.rbegin()->push_back({ faceInfo.strPersonName, faceInfo.box, false, false, false, true, false });
	m_bSavedChanged = true;

	return 0;
}

void FaceInfoManager::DeleteSelected()
{
	// 先检查是否有被选中的 FaceInfo，如果没有直接返回，如果有则执行删除
	if (m_FacesInfo[m_nPos].end() == std::find_if(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), [](const FaceInfoEx& info){ return info.bIsSelected; }))
	{
		return;
	}
	SnapShot();


// 	m_FacesInfo[m_nPos].erase(std::remove_if(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), [](const FaceInfoEx& info){ return info.bIsSelected; }), m_FacesInfo[m_nPos].end());
// 	m_bSelectedChanged = true;
// 	m_bSavedChanged = true;


	// 把所有选中了的 FaceInfo 标记为 Deleted。
	for (auto& faceInfo : m_FacesInfo[m_nPos])
	{
		if (faceInfo.bIsSelected)
		{
			faceInfo.bDeleted = true;
		}
	}
	m_bSelectedChanged = true;
	m_bSavedChanged = true;
}

void FaceInfoManager::SelectBBox(const cv::Rect& rc)
{
	for (auto& faceInfo : m_FacesInfo[m_nPos])
	{
		if (IsOverlapping(rc, faceInfo.GetFaceInfo().box))
		{
			if (!faceInfo.bIsSelected)
			{
				faceInfo.bIsSelected = true;
				m_bSelectedChanged = true;
			}
		}
		else
		{
			if (faceInfo.bIsSelected)
			{
				faceInfo.bIsSelected = false;
				m_bSelectedChanged = true;
			}
		}
	}
}

int FaceInfoManager::SelectEditPoint(const cv::Point& point)
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
			m_bFirstMove = true;
			break;
		}
	}
	if (m_nEditType == -1)
	{
		ResetSelected();
	}
	return m_nEditType;
}

void FaceInfoManager::ResetSelectedEditPoint()
{
	m_nEditType = -1;
}

void FaceInfoManager::Move(const cv::Point& point)
{
	if (m_bFirstMove)
	{
		SnapShot();
		m_bFirstMove = false;
	}
	cv::Point offset = point - m_LastPos;
	for (auto& box : m_FacesInfo[m_nPos])
	{
		if (box.bIsSelected)
		{
			box.UpdateLocation(m_nEditType, offset);
			m_bSavedChanged = true;
			m_bSelectedChanged = true;
		}
	}
	m_LastPos = point;
}

void FaceInfoManager::Undo()
{
	if (m_nPos == 0)
	{
		return;
	}
	--m_nPos;
}

void FaceInfoManager::Redo()
{
	if (m_nPos == m_FacesInfo.size() - 1)
	{
		return;
	}
	++m_nPos;
}

void FaceInfoManager::SetHighLight(size_t nIndex)
{
	ClearHighLight();
	m_bHighlightChanged = true;
	m_FacesInfo[m_nPos][nIndex].bIsHighLight = true;
}

void FaceInfoManager::ClearHighLight()
{
	for (auto& faceinfoEx : m_FacesInfo[m_nPos])
	{
		faceinfoEx.bIsHighLight = false;
		m_bHighlightChanged = true;
	}
}

void FaceInfoManager::UpdateDrawableSavedFacesInfo(std::vector<IDrawable*>& toBeUpdated)
{
	if (!m_bSavedChanged)
	{
		return;
	}
	for (auto drawable : toBeUpdated)
	{
		delete drawable;
	}
	toBeUpdated.clear();
	cv::Rect rc;
	for (auto faceinfoex : m_FacesInfo[m_nPos])
	{
		toBeUpdated.push_back(new DFaceInfo(FaceInfo{ faceinfoex.GetFaceInfo().strPersonName, m_pTrans->Trans(faceinfoex.GetFaceInfo().box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi) }, ColorSaved));
	}
	m_bSavedChanged = false;
}

void FaceInfoManager::UpdateDrawableSelectedFacesInfo(std::vector<IDrawable*>& toBeUpdated)
{
	if (!m_bSelectedChanged)
	{
		return;
	}
	for (auto drawable : toBeUpdated)
	{
		delete drawable;
	}
	toBeUpdated.clear();
	cv::Rect rc;
	for (auto faceinfoex : m_FacesInfo[m_nPos])
	{
		if (faceinfoex.bIsSelected)
		{
			FaceInfoEx ex{ faceinfoex.GetFaceInfo().strPersonName, m_pTrans->Trans(faceinfoex.GetFaceInfo().box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi), true, false, true,false, false };
			toBeUpdated.push_back(new DEditBox(ex.GetEditBox().rc, ex.GetEditBox().editMark, ex.GetFaceInfo().strPersonName, ex.GetFaceInfo().box.tl(), ColorSelected));
		}
	}
	m_bSelectedChanged = false;
	//printf("Selected faces have been updated....\n");
}

void FaceInfoManager::UpdateDrawableHighlightFacesInfo(std::vector<IDrawable*>& toBeUpdated)
{
	if (!m_bHighlightChanged)
	{
		return;
	}
	for (auto drawable : toBeUpdated)
	{
		delete drawable;
	}
	toBeUpdated.clear();
	cv::Rect rc;
	for (auto faceinfoex : m_FacesInfo[m_nPos])
	{
		if (faceinfoex.bIsHighLight)
		{
			toBeUpdated.push_back(new DFaceInfo(FaceInfo{ faceinfoex.GetFaceInfo().strPersonName, m_pTrans->Trans(faceinfoex.GetFaceInfo().box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi) }, ColorHighLight));
		}
	}
	m_bHighlightChanged = false;
	//printf("Highlight faces have been updated....\n");
}

void FaceInfoManager::ResetSelect()
{
	m_bSelectedChanged = true;
	for (auto faceinfoex : m_FacesInfo[m_nPos])
	{
		faceinfoex.bIsSelected = false;
	}
}

std::string FaceInfoManager::ToString() const
{
	std::stringstream ss;
	ss << "当前有 " << m_FacesInfo[m_nPos].size() << " 个 faceinfo， 游标位于 " << m_nPos << " 快照总长度： " << m_FacesInfo.size() << std::endl;
	return ss.str();
}

void FaceInfoManager::SnapShot()
{
//	std::cout << "--------------------------------------------------" << std::endl;
	if (m_nPos != m_FacesInfo.size() - 1)
	{
		m_FacesInfo.resize(1 + m_nPos);
	}
	if (m_FacesInfo.size() >= 20)
	{
		m_FacesInfo.erase(m_FacesInfo.begin());
		--m_nPos;
	}
	m_FacesInfo.push_back(m_FacesInfo[m_nPos]);
	++m_nPos;
}

void FaceInfoManager::ResetSelected()
{
	std::for_each(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), [](FaceInfoEx& fiex){ fiex.bIsSelected = false; });
	m_bSelectedChanged = true;
}

bool FaceInfoManager::IsOverlapping(const cv::Rect& rc1, const cv::Rect& rc2) const
{
	return ((rc1 & rc2).area() > 0);
}

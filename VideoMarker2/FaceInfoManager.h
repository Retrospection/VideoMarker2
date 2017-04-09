#pragma once

#include "FrameInfo.h"
#include "RectEx.h"



#include <string>
#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>


struct CEditBox
{
	std::vector<cv::Rect> editMark;
	RectEx rc;
};

class FaceInfoEx
{
public:
	FaceInfoEx()
		:m_Box(), m_strPersonName()
	{
		UpdateEditRects();
	}
	FaceInfoEx(const FaceInfo& faceInfo)
		: m_Box(*reinterpret_cast<const RectEx*>(&faceInfo.box)), m_strPersonName(faceInfo.strPersonName)
	{
		UpdateEditRects();
	}
	FaceInfoEx(const std::string& strPersonName, const cv::Rect& rc, bool isSelected, bool isHighLight, bool isSaved)
		: m_Box(*reinterpret_cast<const RectEx*>(&rc)), m_strPersonName(strPersonName)
	{
		UpdateEditRects();
	}
	FaceInfoEx(const FaceInfo& faceInfo, bool isSelected, bool isHighLight, bool isSaved)
		: m_Box(*reinterpret_cast<const RectEx*>(&faceInfo.box)), m_strPersonName(faceInfo.strPersonName), bIsSelected(isSelected), bIsHighLight(isHighLight), bSaved(isSaved)
	{
		UpdateEditRects();
	}
	int Hit(const cv::Point& pt) const
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
	void UpdateLocation(int editType, const cv::Point& offset)
	{
		m_Box.Move(editType, offset);
		UpdateEditRects();
	}
	FaceInfo GetFaceInfo() const
	{
		return{ m_strPersonName, *reinterpret_cast<const cv::Rect*>(&m_Box) };
	}
	CEditBox GetEditBox() const
	{
		CEditBox ret{ m_EditRects, m_Box };
		return ret;
	}


	std::string m_strPersonName;
	RectEx m_Box;
	std::vector<cv::Rect> m_EditRects;
	bool bIsSelected;
	bool bIsHighLight;
	bool bSaved;

private:
	void UpdateEditRects()
	{
		m_EditRects.clear();
		auto points = CalculateEditRects();
		for (auto& pt : points)
		{
			m_EditRects.push_back(GetEditMarkRect(pt));
		}
	}

	std::vector<cv::Point> CalculateEditRects()
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
};



class FaceInfoManager
{
public:
	FaceInfoManager()
		:m_nPos(0), m_bFirstMove(true)
	{
		m_FacesInfo.push_back({});
	}

	~FaceInfoManager(){}

	std::vector<FaceInfo> GetUnsavedFacesInfo() const
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
	void ClearUnsavedFacesInfo()
	{
		SnapShot();
		auto pred = [](const FaceInfoEx& fiex){ return !fiex.bSaved; };
		m_FacesInfo[m_nPos].erase(std::remove_if(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), pred), m_FacesInfo[m_nPos].end());
	}

	std::vector<FaceInfo> GetFacesInfo() const
	{
		std::vector<FaceInfo> ret(m_FacesInfo[m_nPos].size());
		auto fn = [](const FaceInfoEx& fiex){ return fiex.GetFaceInfo(); };
		std::transform(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), ret.begin(), fn);
		return ret;
	}

	std::vector<FaceInfoEx> GetFacesInfoEx() const
	{
		return m_FacesInfo[m_nPos];
	}



	FrameInfo GetFrameInfo() const
	{
		FrameInfo ret;
		ret.facesInfo = GetFacesInfo();
		return std::move(ret);
	}


	void SetFrameInfo(const FrameInfo& frameInfo)
	{
		m_FacesInfo.clear();
		m_nPos = 0;
		std::vector<FaceInfoEx> temp(frameInfo.facesInfo.size());
		std::transform(frameInfo.facesInfo.begin(), frameInfo.facesInfo.end(), temp.begin(), [](const FaceInfo& info){ return FaceInfoEx{ info, false, false, true }; });
		m_FacesInfo.push_back(temp);
		SnapShot();
	}

	unsigned int Add(const FaceInfo& faceInfo)
	{
		SnapShot();
		m_FacesInfo.rbegin()->push_back({ faceInfo, false, false, false });
		std::cout << ToString();
		return 0;
	}
	void DeleteSelected()
	{
		if (m_FacesInfo[m_nPos].end() == std::find_if(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), [](const FaceInfoEx& info){ return info.bIsSelected; }))
		{
			return;
		}
		SnapShot();
		m_FacesInfo[m_nPos].erase(std::remove_if(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), [](const FaceInfoEx& info){ return info.bIsSelected; }), m_FacesInfo[m_nPos].end());
	}

	void Select(const cv::Rect& rc)
	{
		for (auto& faceInfo : m_FacesInfo[m_nPos])
		{
			if (IsOverlapping(rc, faceInfo.GetFaceInfo().box))
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

	void ClearSelectEditPoint()
	{
		m_nEditType = -1;
	}

	// 	void MoveFinished()
	// 	{
	// 		m_bFirstMove = true;
	// 	}

	void Move(const cv::Point& point)
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
			}
		}
		m_LastPos = point;
	}

	// 	void MoveStart()
	// 	{
	// 		SnapShot();
	// 	}

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

	void SetHighLight(size_t nIndex)
	{
		ClearHighLight();
		m_FacesInfo[m_nPos][nIndex].bIsHighLight = true;
	}

	void ClearHighLight()
	{
		for (auto& faceinfoEx : m_FacesInfo[m_nPos])
		{
			faceinfoEx.bIsHighLight = false;
		}
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
		std::cout << "--------------------------------------------------" << std::endl;
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

	void ResetSelected()
	{
		std::for_each(m_FacesInfo[m_nPos].begin(), m_FacesInfo[m_nPos].end(), [](FaceInfoEx& fiex){ fiex.bIsSelected = false; });
	}


	bool IsOverlapping(const cv::Rect& rc1, const cv::Rect& rc2)const
	{
		return ((rc1 & rc2).area() > 0);
	}


	int m_nEditType;
	cv::Point m_LastPos;
	bool m_bFirstMove;

	std::vector<std::vector<FaceInfoEx>> m_FacesInfo;
	size_t m_nPos;

};
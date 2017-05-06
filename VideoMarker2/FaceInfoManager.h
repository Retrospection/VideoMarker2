#pragma once

#include "FrameInfo.h"
#include "RectEx.h"
#include "constant.h"
#include "DFaceInfo.h"
#include "DEditBox.h "
#include "Transformer.h"




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
	
	FaceInfoEx();

	FaceInfoEx(const std::string& strPersonName, const cv::Rect& rc,
		bool isSelected, bool isHighLight, bool isSaved, bool isNew, bool deleted);

// 	FaceInfoEx(const FaceInfo& faceInfo);
// 	FaceInfoEx(const std::string& strPersonName, const cv::Rect& rc, bool isSelected, bool isHighLight, bool isSaved);
// 	FaceInfoEx(const FaceInfo& faceInfo, bool isSelected, bool isHighLight, bool isSaved);

	// 根据 pt 找出被选中的 EditBox 是哪一个
	// 返回值： EditBox 的索引
	int Hit(const cv::Point& pt) const;

	// 改变 FaceInfo 矩形框的大小
	void UpdateLocation(int editType, const cv::Point& offset);

	// 返回 FaceInfo
	FaceInfo GetFaceInfo() const;

	// 返回 EditBox
	CEditBox GetEditBox() const;


	// 属性
	std::string m_strPersonName;					// 人名
	RectEx m_Box;									// 包围盒
	std::vector<cv::Rect> m_EditRects;				// 编辑框

	bool bIsSelected;
	bool bIsHighLight;
	bool bSaved;
	bool bNewInfo;
	bool bDeleted;

private:

	// 根据 FaceInfo 更新八个 EditBox
	void UpdateEditRects();

	// 计算 EditRects
	std::vector<cv::Point> CalculateEditRects();

	// 根据 point 计算 EditMark 的矩形框
	cv::Rect GetEditMarkRect(const cv::Point& point);
};


class FaceInfoManager
{
public:
	FaceInfoManager(Transformer* ptr);
	~FaceInfoManager(){}


	std::vector<FaceInfo> GetUnsavedFacesInfo() const;
	void ClearUnsavedFacesInfo();
	std::vector<FaceInfo> GetFacesInfo();
	std::vector<FaceInfoEx> GetFacesInfoEx() const;
	FrameInfo GetFrameInfo(); 
	void SetFrameInfo(const FrameInfo& frameInfo);

	unsigned int AddFaceInfo(const FaceInfo& faceInfo);

	void DeleteSelected();
	void SelectBBox(const cv::Rect& rc);
	int SelectEditPoint(const cv::Point& point);
	void ResetSelectedEditPoint();
	void Move(const cv::Point& point);
	void Undo();
	void Redo();
	void SetHighLight(size_t nIndex);
	void ClearHighLight();
	void UpdateDrawableSavedFacesInfo(std::vector<IDrawable*>& toBeUpdated);
	void UpdateDrawableSelectedFacesInfo(std::vector<IDrawable*>& toBeUpdated);
	void UpdateDrawableHighlightFacesInfo(std::vector<IDrawable*>& toBeUpdated);

	void ResetSelect();


private:
	std::string ToString() const;
	void SnapShot();
	void ResetSelected();
	bool IsOverlapping(const cv::Rect& rc1, const cv::Rect& rc2)const;

	int m_nEditType;
	cv::Point m_LastPos;
	bool m_bFirstMove;

	std::vector<std::vector<FaceInfoEx>> m_FacesInfo;

	bool m_bSavedChanged;
	bool m_bSelectedChanged;
	bool m_bHighlightChanged;
	
	Transformer* m_pTrans;
	size_t m_nPos;

};
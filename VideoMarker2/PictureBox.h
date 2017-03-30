#pragma once

#include <core/core.hpp>
#include "StateBase.h"
#include "Transformer.h"

#include "FrameInfo.h"
#include "EditBox.h"

class CS5;
class CS12;
class CStateBase;
class IDrawable;
class SelectItemManager;

class CPictureBox : public CStatic
{
	friend CS5;
	friend CS12;
	DECLARE_DYNAMIC(CPictureBox)

public:
	CPictureBox(CStateBase* pState);
	virtual ~CPictureBox();

public:
	void SetDrawable(bool drawable);
	void SetEditType(size_t nEditType);


public:
	void SetImage(const cv::Mat& image);
	void SetFrameInfo(const FrameInfo& frameInfo);
	void SetHighLight(const std::vector<cv::Rect>& highLight);
	std::vector<cv::Rect> GetUnsavedBoxesInRaw();
	std::vector<std::string> GetUnsavedNames() const;

	FrameInfo GetUnsavedFrameInfo() const;

	void ClearUnsavedBoxes();
	void ClearUnsavedNames();
	void ClearEditBoxes();

	void Undo();
	void Redo();


	std::vector<size_t> GetDeleteFrameInfo() const;
	void ClearDeleteFrameInfo();

	std::vector<FaceInfo> GetModifiedFacesInfo() const;


	static const size_t ADD_MARK_TYPE = 1;
	static const size_t DELETE_MAKR_TYPE = 2;
	static const size_t CHANGE_MARK = 3;

private:
	void DrawFrameInfo(cv::Mat& img);
	bool GetActiveBox(cv::Rect& activeBox) const;
	void CacheUnsaveFaceInfo(const FaceInfo& faceInfo);
	void HighLightDeleteFaceInfo();




public:
	void CalculateDeleteFrameInfoIndex();
	void CacheDeleteFrameInfo(const std::vector<size_t>& deletedFaceInfoIndex);
	void DeleteUnsavedFaceInfo();
	void ClearToBeDeleted();

private:
	void CacheDeleteArea();
	void PrepareEdit();

private:
	Transformer m_Trans;


private:
	cv::Point m_ActivePoints[2];


	FrameInfo m_FrameInfo;
	std::vector<cv::Rect> m_HighLights;
	std::vector<std::string> m_UnsavedNames;
	std::vector<cv::Rect> m_UnsavedBoxes;
	std::vector<size_t> m_DeleteFaceInfoIndexes;
	std::vector<size_t> m_DeleteUnsavedFaceInfoIndexes;
	std::vector<FaceInfo> m_ToBeDeleteFaceInfo;
	std::vector<size_t> m_IllegalIndex;
	std::vector<FaceInfo*> m_ModifiedFaceInfo;
	std::vector<std::vector<cv::Rect>> m_EditPoints;
	cv::Rect m_DeleteArea;

	int m_nEditPointIndex;
	int m_nModifiedFaceInfoIndex;

	bool m_bDrawable;
	bool m_bDrawing;
	cv::Mat m_image;

	std::vector<IDrawable*> m_drawables;

	static const wchar_t* m_AlertMessage[4];
	static const size_t NUMBER_OF_VALIDATOR_TYPES = sizeof(m_AlertMessage) / sizeof(m_AlertMessage[0]);


	size_t m_nEditType;

	
	SelectItemManager* m_pSelectItemManager;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT);
};



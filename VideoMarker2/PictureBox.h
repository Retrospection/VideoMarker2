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
class CUnsavedFrameInfo;
class FaceInfoManager;

class CPictureBox : public CStatic
{

	friend CS5;
	friend CS12;
	DECLARE_DYNAMIC(CPictureBox)

public:
	CPictureBox(CStateBase* pState);
	virtual ~CPictureBox();

	unsigned int ValidateFaceInfo(const FaceInfo& faceInfo);

///////////////////////// Unsaved ///////////////////////////////
public:
	std::vector<cv::Rect> GetUnsavedBoxesInRaw();
	std::vector<std::string> GetUnsavedNames() const;
	FrameInfo GetUnsavedFrameInfo() const;
	void CacheUnsaveFaceInfo(const FaceInfo& faceInfo);
	void ClearUnsavedBoxes();
	void ClearUnsavedNames();

private:
	std::vector<std::string> m_UnsavedNames;
	std::vector<cv::Rect> m_UnsavedBoxes;
	CUnsavedFrameInfo* m_pUnsavedFrameInfo;

/////////////////////////////////////////////////////////////////

/////////////////////// Deleted /////////////////////////////////
public:
	std::vector<size_t> GetDeleteFrameInfo() const;
	void ClearDeleteFrameInfo();
	void CalculateDeleteFrameInfoIndex();
	void CacheDeleteFrameInfo(const std::vector<size_t>& deletedFaceInfoIndex);
	void DeleteUnsavedFaceInfo();
	void ClearToBeDeleted();

private:
	void CacheDeleteArea();
	cv::Rect m_DeleteArea;
	std::vector<size_t> m_DeleteFaceInfoIndexes;
	std::vector<size_t> m_DeleteUnsavedFaceInfoIndexes;
	std::vector<FaceInfo> m_ToBeDeleteFaceInfo;

/////////////////////////////////////////////////////////////////


/////////////////// ActiveBox ///////////////////////////////////
private:
	bool GetActiveBox(cv::Rect& activeBox) const;
	cv::Point m_ActivePoints[2];

/////////////////////////////////////////////////////////////////

/////////////////// HighLight ///////////////////////////////////
public:
	void SetHighLight(const std::vector<cv::Rect>& highLight);
	void HighLightDeleteFaceInfo();
private:
	std::vector<cv::Rect> m_HighLights;

/////////////////////////////////////////////////////////////////

/////////////////// HighLight ///////////////////////////////////

public:
	void SetFrameInfo(const FrameInfo& frameInfo);
private:
	void DrawFrameInfo(cv::Mat& img);
	FrameInfo m_FrameInfo;


/////////////////////////////////////////////////////////////////

//////////////////// Frame //////////////////////////////////////
public:
	void SetDrawable(bool drawable);
	void SetImage(const cv::Mat& image);
private:
	bool m_bDrawable;
	bool m_bDrawing;
	cv::Mat m_image;

/////////////////////////////////////////////////////////////////

public:
	void SetEditType(size_t nEditType);


public:
	void ClearSelectedBoxes();
	void Undo();
	void Redo();
	static const size_t ADD_MARK_TYPE = 1;
	static const size_t DELETE_MAKR_TYPE = 2;
	static const size_t CHANGE_MARK = 3;

private:
	void SelectBox();

private:
	Transformer m_Trans;
	std::vector<IDrawable*> m_drawables;
	int m_nModifiedFaceInfoIndex;
	static const wchar_t* m_AlertMessage[4];
	static const size_t NUMBER_OF_VALIDATOR_TYPES = sizeof(m_AlertMessage) / sizeof(m_AlertMessage[0]);
	size_t m_nEditType;
	SelectItemManager* m_pSelectItemManager;
	FaceInfoManager* m_pFaceInfoManager;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT);
};



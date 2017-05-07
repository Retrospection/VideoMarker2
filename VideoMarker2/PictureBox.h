#pragma once

#include <core/core.hpp>
#include "StateBase.h"
#include "Transformer.h"
#include "ActiveBoxManager.h"

#include "FrameInfo.h"
#include "RectEx.h"


class CS5;
class CS12;
class CStateBase;
class IDrawable;
class SelectItemManager;
class CUnsavedFrameInfo;
class FaceInfoManager;
class FaceInfoEx;
class CPBStateBase;

class CPictureBox : public CStatic
{

	friend CPBStateBase;
	DECLARE_DYNAMIC(CPictureBox)

public:
	CPictureBox(/*CStateBase* pState*/);
	virtual ~CPictureBox();

	unsigned int ValidateFaceInfo();
	FrameInfo GetFrameInfo() const;
	std::vector<FaceInfoEx> GetFaceInfoEx() const;

	void SetState(const std::string& state);

	bool IsInRoi(const CPoint& pt);
	void ClearSelectEditPoint();

///////////////////////// Unsaved ///////////////////////////////
public:
	FrameInfo GetUnsavedFrameInfo() const;
	void ClearUnsavedFaceInfo();



/////////////////// ActiveBox ///////////////////////////////////
private:
	bool GetActiveBox(cv::Rect& activeBox) const;
	bool GetActiveBoxEx(cv::Rect& activeBox) const;
	cv::Point m_ActivePoints[2];


/////////////////// HighLight ///////////////////////////////////
public:
	void SetHighLight(size_t nIndex);
	void ClearHighLight();
private:
	std::vector<cv::Rect> m_HighLights;


public:
	// 打开工程，前进后退，播放改变当前帧时改变FrameInfo使用的函数。
	void SetFrameInfo(const FrameInfo& frameInfo);
private:

	void DrawFrameInfo(cv::Mat& img);
	FrameInfo m_FrameInfo;



//////////////////// Frame //////////////////////////////////////
public:
	void SetImage(const cv::Mat& image);
private:
	bool m_bDrawing;
	cv::Mat m_image;

public:
	void SetEditType(size_t nEditType);

public:
	void DeleteSelectedFacesInfo();
	void Undo();
	void Redo();
	static const size_t INIT_STATE = 1;
	static const size_t ADD_MARK_STATE = 2;
	static const size_t SELECT_MARK_STATE = 3;
	static const size_t MODIFY_MARK_STATE = 4;

private:
	void SelectBox();

private:
	int m_nModifiedFaceInfoIndex;

	Transformer m_Trans;
	std::vector<IDrawable*> m_drawables;
	FaceInfoManager* m_pFaceInfoManager;
	CPBStateBase* m_pState;

	CActiveBoxManager m_ActiveBoxManager;

	std::vector<IDrawable*> m_DrawableActiveBox;
	std::vector<IDrawable*> m_DrawableSavedFacesInfo;
	std::vector<IDrawable*> m_DrawableSelectedFacesInfo;
	std::vector<IDrawable*> m_DrawableHighlightFacesInfo;

private:
	static const wchar_t* m_AlertMessage[4];
	static const size_t NUMBER_OF_VALIDATOR_TYPES = sizeof(m_AlertMessage) / sizeof(m_AlertMessage[0]);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT);
};



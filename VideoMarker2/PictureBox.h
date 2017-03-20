#pragma once

#include <core/core.hpp>
#include "StateBase.h"
#include "Transformer.h"


class CStateBase;

class IDrawable;

class CPictureBox : public CStatic
{
	DECLARE_DYNAMIC(CPictureBox)

public:
	CPictureBox(CStateBase* pState);
	virtual ~CPictureBox();

	void SetState(CStateBase* pState);



public:
	void SetImage(const cv::Mat& image);
	void SetFrameInfo(const FrameInfo& frameInfo);
	void SetHighLight(const std::vector<cv::Rect>& highLight);
	std::vector<cv::Rect> GetUnsavedBoxesInRaw();
	std::vector<std::string> GetUnsavedNames()const;

	void ClearUnsavedBoxes();
	void ClearUnsavedNames();

	void Undo();
	void Redo();

	void SetIllegal(const FaceInfo& info, size_t index);

	void DecreaseEndIndex();


private:
	void DrawFrameInfo(cv::Mat& img);
	bool GetActiveBox(cv::Rect& activeBox) const;


	Transformer m_Trans;
	
	CStateBase* m_pState;

	cv::Point m_ActivePoints[2];
	std::vector<cv::Rect> m_HighLights;
	FrameInfo m_FrameInfo;
	std::vector<std::string> m_UnsavedNames;
	std::vector<cv::Rect> m_UnsavedBoxes;


	std::vector<FaceInfo> m_IllegalFaceInfo;
	std::vector<size_t> m_IllegalIndex;


	size_t m_nEndIndexOfUnsavedDrawables;

	bool m_bDrawing;
	cv::Mat m_image;

	std::vector<IDrawable*> m_drawables;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT);
};



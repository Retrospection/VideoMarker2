#pragma once

#include <core/core.hpp>
#include "StateBase.h"

class Transformer;

class CStateBase;

class CPictureBox : public CStatic
{
	DECLARE_DYNAMIC(CPictureBox)

public:
	CPictureBox(CStateBase* pState);
	virtual ~CPictureBox();

	void SetState(CStateBase* pState);



public:
	void SetImage(const cv::Mat& image);
	const cv::Rect* GetActiveBox() const;
	std::vector<cv::Rect> GetUnsavedBoxesInRaw();
	std::vector<cv::Rect> GetUnsavedBoxesInRoi();

public:
	Transformer* m_pTrans;
	std::vector<cv::Rect> m_boxes;
	CStateBase* m_pState;
	cv::Point m_ActivePoints[2];
	bool m_bDrawing;
	cv::Mat m_image;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT);
};



#pragma once
#include "PBStateBase.h"


class CPBSelectState :
	public CPBStateBase
{
public:
	CPBSelectState(CPictureBox* ptr);
	virtual ~CPBSelectState();

	virtual void OnLButtonDown(UINT nFlags, CPoint point) override;
	virtual void OnLButtonUp(UINT nFlags, CPoint point) override;
	virtual void OnMouseMove(UINT nFlags, CPoint point) override;
	virtual void DrawActiveBox() override;
};


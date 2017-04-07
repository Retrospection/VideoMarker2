#pragma once
#include "PBStateBase.h"
class CPBAddState :
	public CPBStateBase
{
public:
	CPBAddState(CPictureBox* ptr);
	~CPBAddState();
	virtual void OnLButtonDown(UINT nFlags, CPoint point) override;
	virtual void OnLButtonUp(UINT nFlags, CPoint point) override;
	virtual void OnMouseMove(UINT nFlags, CPoint point) override;
	virtual void DrawActiveBox() override;
};


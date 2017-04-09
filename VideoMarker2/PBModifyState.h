#pragma once
#include "PBStateBase.h"




class CPBModifyState :
	public CPBStateBase
{
public:
	CPBModifyState(CPictureBox* ptr);
	virtual ~CPBModifyState();

	virtual void OnLButtonDown(UINT nFlags, CPoint point) override;
	virtual void OnLButtonUp(UINT nFlags, CPoint point) override;
	virtual void OnMouseMove(UINT nFlags, CPoint point) override;
};


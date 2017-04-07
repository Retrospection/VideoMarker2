#pragma once
#include "PBStateBase.h"
class CPBInitState :
	public CPBStateBase
{
public:
	CPBInitState(CPictureBox* ptr);
	virtual ~CPBInitState();

	virtual void OnLButtonDown(UINT nFlags, CPoint point) override;
	virtual void OnLButtonUp(UINT nFlags, CPoint point) override;
	virtual void OnMouseMove(UINT nFlags, CPoint point) override;
};


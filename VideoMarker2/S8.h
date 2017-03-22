#pragma once
#include "StateBase.h"

class CS8 :
	public CStateBase
{
public:
	CS8(CVideoMarker2Dlg* pDlg, const UIConfig& config);
	virtual ~CS8();

	virtual void RefreshButton() override;

	virtual void OnLButtonDown(int nFlags, cv::Point point) override;

};


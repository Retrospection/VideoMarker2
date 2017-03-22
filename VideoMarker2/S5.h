#pragma once
#include "StateBase.h"

class CS5 :
	public CStateBase
{
public:
	CS5(CVideoMarker2Dlg* pDlg);
	virtual ~CS5();

	virtual void RefreshButton() override;

	virtual void OpenProject() override;

	virtual void AddMark() override;

	virtual void OnLButtonDown(int nFlags, cv::Point point) override;



};


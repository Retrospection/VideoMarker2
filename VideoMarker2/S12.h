#pragma once
#include "StateBase.h"
class CS12 :
	public CStateBase
{
public:
	CS12(CVideoMarker2Dlg* pDlg);
	virtual ~CS12();


	virtual void OnLButtonUp(int nFlags, cv::Point point) override;


};


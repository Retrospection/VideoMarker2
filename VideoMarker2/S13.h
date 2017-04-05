#pragma once
#include "StateBase.h"
class CS13 :
	public CStateBase
{
public:
	CS13(CVideoMarker2Dlg* pDlg, const UIConfig& config);
	virtual ~CS13();

	virtual void RefreshButton() override;


	virtual void SelectMarkBtnClicked() override;

	virtual void DeleteMarkBtnClicked() override;

};


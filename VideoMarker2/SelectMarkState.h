#pragma once
#include "StateBase.h"
class CSelectMarkState :
	public CStateBase
{
public:
	CSelectMarkState(CVideoMarker2Dlg* pDlg, const UIConfig& config);
	virtual ~CSelectMarkState();

	virtual void RefreshButton() override;


	virtual void SelectMarkBtnClicked() override;

	virtual void DeleteMarkBtnClicked() override;

};


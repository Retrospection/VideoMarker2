#pragma once
#include "StateBase.h"

class CAddMarkState :
	public CStateBase
{
public:
	CAddMarkState(CVideoMarker2Dlg* pDlg, const UIConfig& config);
	virtual ~CAddMarkState();

	virtual void RefreshButton() override;

	virtual bool CanDraw() const override;

	virtual void AddSaveMarkBtnClicked() override;

};


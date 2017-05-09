#pragma once

#include "StateBase.h"

class CProjectOpenedState : public CStateBase
{
public:
	CProjectOpenedState(CVideoMarker2Dlg* pDlg, const UIConfig& config);
	virtual ~CProjectOpenedState();

	virtual void RefreshButton() override;

	virtual void AddSaveMarkBtnClicked() override;

	virtual void Undo() override;
	
	virtual void Redo() override;

	virtual void SelectMarkBtnClicked() override;

	virtual void OnLbnSelchangeList1() override;

	virtual void Play() override;

	virtual void Stop() override;

};


#pragma once
#include "StateBase.h"
class CPauseState :
	public CStateBase
{
public:
	CPauseState(CVideoMarker2Dlg* pDlg, const UIConfig& config);
	~CPauseState();

	virtual void RefreshButton() override;

	virtual void Play() override;

	virtual void Stop() override;

	virtual void AddSaveMarkBtnClicked() override;

	virtual void Undo() override;

	virtual void Redo() override;

	virtual void SelectMarkBtnClicked() override;

	virtual void OnLbnSelchangeList1() override;

};


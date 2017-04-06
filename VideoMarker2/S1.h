#pragma once

#include "StateBase.h"

class CS1 : public CStateBase
{
public:
	CS1(CVideoMarker2Dlg* pDlg, const UIConfig& config);
	virtual ~CS1();

	virtual void RefreshButton() override;

	virtual void AddSaveMarkBtnClicked() override;

	virtual void Undo() override;
	
	virtual void Redo() override;

	virtual void SelectMarkBtnClicked() override;

	virtual void OnLbnSelchangeList1() override;


};


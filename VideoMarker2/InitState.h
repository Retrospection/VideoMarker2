#pragma once
#include "StateBase.h"


class CInitState :public CStateBase
{
public:
	CInitState(CVideoMarker2Dlg* pDlg, const UIConfig& config);
	~CInitState();

	virtual void RefreshButton() override;


	virtual void Undo() override;

	virtual void Redo() override;

};


#pragma once
#include "StateBase.h"
class CPauseState :
	public CStateBase
{
public:
	CPauseState(CVideoMarker2Dlg* pDlg);
	~CPauseState();

	virtual void RefreshButton() override;

	virtual void Play() override;

	virtual void Stop() override;

};


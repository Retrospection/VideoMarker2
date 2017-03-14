#pragma once
#include "StateBase.h"
class CPlayState :
	public CStateBase
{
public:
	CPlayState(CVideoMarker2Dlg* pDlg);
	~CPlayState();

public:

	virtual void Pause() override;

	virtual void Stop() override;

	virtual void RefreshButton() override;


};


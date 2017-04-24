#pragma once
#include "StateBase.h"
class CPlayState :
	public CStateBase
{
public:
	CPlayState(CVideoMarker2Dlg* pDlg, const UIConfig& config);
	~CPlayState();

public:

	virtual void Pause() override;

	virtual void Stop() override;

	virtual void RefreshButton() override;

//	virtual void Play() override;


};


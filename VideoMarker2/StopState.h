#pragma once
#include "StateBase.h"
class CStopState :
	public CStateBase
{
public:
	CStopState(CVideoMarker2Dlg* pDlg);
	~CStopState();

	virtual void RefreshButton() override;

//	virtual void Play() override;

	virtual void Open() override;

	virtual void OpenTextFile() override;

};


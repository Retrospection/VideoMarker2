#pragma once
#include "StateBase.h"


class CInitState :public CStateBase
{
public:
	CInitState(CVideoMarker2Dlg* pDlg);
	~CInitState();

	virtual void RefreshButton() override;
	
// ֻ������ļ�
	virtual void Open() override;
	

};


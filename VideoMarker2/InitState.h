#pragma once
#include "StateBase.h"


class CInitState :public CStateBase
{
public:
	CInitState(CVideoMarker2Dlg* pDlg);
	~CInitState();

	virtual void RefreshButton() override;
	
// 只允许打开文件
	virtual void Open() override;
	

};


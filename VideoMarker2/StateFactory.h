#pragma once

#include "StateBase.h"

class CStateFactory
{
private:
	CStateFactory();
	~CStateFactory();

public:
	static CStateFactory& GetInstance();
	CStateBase* Create(const std::string& state, CVideoMarker2Dlg* pDlg, const UIConfig& config);
};


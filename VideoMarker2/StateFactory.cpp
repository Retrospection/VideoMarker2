#include "stdafx.h"
#include "StateFactory.h"
#include "InitState.h"
#include "StopState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "EditMarkState.h"
#include "VideoMarker2Dlg.h"
#include "ProjectOpenedState.h"
#include "AddMarkState.h"
#include "SelectMarkState.h"


CStateFactory::CStateFactory()
{
}


CStateFactory::~CStateFactory()
{
}

CStateFactory& CStateFactory::GetInstance()
{
	static CStateFactory instance;
	return instance;
}

CStateBase* CStateFactory::Create(const std::string& state, CVideoMarker2Dlg* pDlg, const UIConfig& config)
{

	if (state == PLAY)
	{
		return new CPlayState(pDlg, config);
	}
// 	else if (state == STOP)
// 	{
// 		return  new CStopState(pDlg);
// 	}
	else if (state == INIT)
	{
		return new CInitState(pDlg, config);
	}
	else if (state == PAUSE)
	{
		return new CPauseState(pDlg, config);
	}
// 	else if (state == EDIT_MARK)
// 	{
// 		return new CEditMarkState(pDlg);
// 	}
	else if (state == ProjectOpened)
	{
		return new CProjectOpenedState(pDlg,config);
	}
	else if (state == AddMark)
	{
		return new CAddMarkState(pDlg, config);
	}
	else if (state == SelectMark)
	{
		return new CSelectMarkState(pDlg, config);
	}
	return nullptr;
}

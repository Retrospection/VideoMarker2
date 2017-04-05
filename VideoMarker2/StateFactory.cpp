#include "stdafx.h"
#include "StateFactory.h"
#include "InitState.h"
#include "StopState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "EditMarkState.h"
#include "VideoMarker2Dlg.h"
#include "S1.h"
#include "S5.h"
#include "S13.h"


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

// 	if (state == PLAY)
// 	{
// 		return new CPlayState(pDlg);
// 	}
// 	else if (state == STOP)
// 	{
// 		return  new CStopState(pDlg);
// 	}
/*	else */if (state == INIT)
	{
		return new CInitState(pDlg, config);
	}
// 	else if (state == PAUSE)
// 	{
// 		return new CPauseState(pDlg);
// 	}
// 	else if (state == EDIT_MARK)
// 	{
// 		return new CEditMarkState(pDlg);
// 	}
	else if (state == S1)
	{
		return new CS1(pDlg,config);
	}
	else if (state == S5)
	{
		return new CS5(pDlg, config);
	}
	else if (state == S13)
	{
		return new CS13(pDlg, config);
	}
// 	else if (state == S12)
// 	{
// 		return new CS12(pDlg, config);
// 	}
// 	else if (state == S8)
// 	{
// 		return new CS8(pDlg, config);
// 	}
	return nullptr;
}

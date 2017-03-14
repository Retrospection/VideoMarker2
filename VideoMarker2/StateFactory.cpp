#include "stdafx.h"
#include "StateFactory.h"
#include "InitState.h"
#include "StopState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "OpenTextFileState.h"
#include "EditMarkState.h"
#include "VideoMarker2Dlg.h"

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

CStateBase* CStateFactory::Create(const std::string& state, CVideoMarker2Dlg* pDlg)
{
	if (state == PLAY)
	{
		return new CPlayState(pDlg);
	}
	else if (state == STOP)
	{
		return  new CStopState(pDlg);
	}
	else if (state == INIT)
	{
		return new CInitState(pDlg);
	}
	else if (state == PAUSE)
	{
		return new CPauseState(pDlg);
	}
// 	else if (state == OPEN_TEXT_FILE)
// 	{
// 		return new COpenTextFileState(pDlg);
// 	}
	else if (state == EDIT_MARK)
	{
		return new CEditMarkState(pDlg);
	}
	return nullptr;
}

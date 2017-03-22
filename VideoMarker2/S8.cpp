#include "stdafx.h"
#include "S8.h"
#include "State.h"


CS8::CS8(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg, config)
{
}


CS8::~CS8()
{
}

void CS8::RefreshButton()
{
	m_ui.RefreshButton();
}

void CS8::OnLButtonDown(int nFlags, cv::Point point)
{
	SetState(S12);
}

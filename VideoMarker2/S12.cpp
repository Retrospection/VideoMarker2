#include "stdafx.h"
#include "S12.h"

#include "State.h"


CS12::CS12(CVideoMarker2Dlg* pDlg) :CStateBase(pDlg)
{
}


CS12::~CS12()
{
}

void CS12::OnLButtonUp(int nFlags, cv::Point point)
{
	SetState(S8);
}

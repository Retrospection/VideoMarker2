#include "stdafx.h"
#include "S1.h"

#include "VideoMarker2Dlg.h"

CS1::CS1(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg,config)
{
}


CS1::~CS1()
{
}

void CS1::RefreshButton()
{
	m_ui.RefreshButton();
	GetPictureBox()->SetDrawable(false);
}

void CS1::SaveMark()
{

}

void CS1::Undo()
{

}

void CS1::Redo()
{

}


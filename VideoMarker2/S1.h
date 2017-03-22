#pragma once

#include "StateBase.h"

class CS1 : public CStateBase
{
public:
	CS1(CVideoMarker2Dlg* pDlg);
	virtual ~CS1();

	virtual void RefreshButton() override;


	virtual void SaveMark() override;

	virtual void Undo() override;
	
	virtual void Redo() override;



};


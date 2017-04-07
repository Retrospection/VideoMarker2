#include "stdafx.h"
#include "PBInitState.h"


#include <iostream>


CPBInitState::CPBInitState(CPictureBox* ptr)
	:CPBStateBase(ptr)
{
}


CPBInitState::~CPBInitState()
{
}

void CPBInitState::OnLButtonDown(UINT nFlags, CPoint point)
{
	std::cout << "PBINIT: LButton DOWN!" << std::endl;
}

void CPBInitState::OnLButtonUp(UINT nFlags, CPoint point)
{
	std::cout << "PBINIT: LButton UP!" << std::endl;
}

void CPBInitState::OnMouseMove(UINT nFlags, CPoint point)
{
	std::cout << "PBINIT: MOVE!" << std::endl;
}

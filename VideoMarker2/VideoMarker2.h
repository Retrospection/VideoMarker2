
// VideoMarker2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVideoMarker2App: 
// �йش����ʵ�֣������ VideoMarker2.cpp
//

class CVideoMarker2App : public CWinApp
{
public:
	CVideoMarker2App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��


	DECLARE_MESSAGE_MAP()
};

extern CVideoMarker2App theApp;
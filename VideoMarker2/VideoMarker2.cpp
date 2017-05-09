
// VideoMarker2.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "VideoMarker2.h"
#include "VideoMarker2Dlg.h"

#include "VideoMarkerPresenter.h"
#include "VideoPlayer.h"

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoMarker2App

BEGIN_MESSAGE_MAP(CVideoMarker2App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVideoMarker2App 构造

CVideoMarker2App::CVideoMarker2App()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}


// 唯一的一个 CVideoMarker2App 对象

CVideoMarker2App theApp;


// CVideoMarker2App 初始化

BOOL CVideoMarker2App::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();
	CShellManager *pShellManager = new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	// self init.
	CVideoMarker2Dlg dlg;
	m_pMainWnd = &dlg;
	CVideoMarkerPresenter presenter(&dlg);
	INT_PTR nResponse = dlg.DoModal();

	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	return FALSE;
}


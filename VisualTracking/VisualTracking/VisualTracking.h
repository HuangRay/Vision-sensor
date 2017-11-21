
// VisualTracking.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CVisualTrackingApp:
// See VisualTracking.cpp for the implementation of this class
//

class CVisualTrackingApp : public CWinApp
{
public:
	CVisualTrackingApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CVisualTrackingApp theApp;

// 1012090.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMy1012090App:
// See 1012090.cpp for the implementation of this class
//

class CMy1012090App : public CWinApp
{
public:
	CMy1012090App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMy1012090App theApp;
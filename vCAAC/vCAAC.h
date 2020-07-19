// vCAAC.h : main header file for the vCAAC DLL
//

#pragma once
#include "pch.h"
#include "EuroScopePlugIn.h"
#include "VirtualCAACPlugin.h"

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CvCAACApp
// See vCAAC.cpp for the implementation of this class
//

class CvCAACApp : public CWinApp
{
public:
	CvCAACApp();

// Overrides
public:
	virtual BOOL InitInstance();
	CVirtualCAACPlugin * gpMyPlugin = NULL;
	DECLARE_MESSAGE_MAP()
};

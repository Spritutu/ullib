#pragma once

#include "..\..\ULLib\Include\ULLib.h"

class CMainFrame:
	public ULFrames::CULFrameWnd
{
public:
	CMainFrame(void);
	virtual ~CMainFrame(void);
protected:
///////////////WM_...///////////////////////////
	virtual LRESULT OnCreate(WPARAM /*wParam*/,LPARAM /*lParam*/);
};

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "srcdbpg.h"

/////////////////////////////////////////////////////////////////////////////
// CSRCDBPG

IMPLEMENT_DYNCREATE(CSRCDBPG, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CSRCDBPG properties

CString CSRCDBPG::GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}

void CSRCDBPG::SetCaption(LPCTSTR propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}

BOOL CSRCDBPG::GetGotoFirstEnabled()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}

void CSRCDBPG::SetGotoFirstEnabled(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}

BOOL CSRCDBPG::GetGotoLastEnabled()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CSRCDBPG::SetGotoLastEnabled(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

BOOL CSRCDBPG::GetGotoPrevEnabled()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CSRCDBPG::SetGotoPrevEnabled(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

BOOL CSRCDBPG::GetGotoNextEnabled()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void CSRCDBPG::SetGotoNextEnabled(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL CSRCDBPG::GetGotoNewEnabled()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void CSRCDBPG::SetGotoNewEnabled(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

short CSRCDBPG::GetCaptionWidth()
{
	short result;
	GetProperty(0x6, VT_I2, (void*)&result);
	return result;
}

void CSRCDBPG::SetCaptionWidth(short propVal)
{
	SetProperty(0x6, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CSRCDBPG operations

void CSRCDBPG::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
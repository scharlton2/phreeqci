// OCKSUserPunch.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "OCKSUserPunch.h"
#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COCKSUserPunch

IMPLEMENT_DYNAMIC(COCKSUserPunch, baseOCKSUserPunch)

COCKSUserPunch::COCKSUserPunch(CWnd* pWndParent)
	 : baseOCKSUserPunch(IDS_PROPSHT_CAPTION17, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

COCKSUserPunch::~COCKSUserPunch()
{
}


BEGIN_MESSAGE_MAP(COCKSUserPunch, baseOCKSUserPunch)
	//{{AFX_MSG_MAP(COCKSUserPunch)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COCKSUserPunch message handlers
/////////////////////////////////////////////////////////////////////////////
// COCKSRates message handlers

CString COCKSUserPunch::GetString()
{
	/*
	Line 0: USER_PUNCH
	Line 1:      -headings Na+ Mg+2 Pairs Rxn_increment
	Line 2:      -start
	Basic:  10 REM convert to ppm
	Basic:  20 PUNCH MOL("Na+")* 22.99 * 1000
	Basic:  30 PUNCH MOL("Mg+2")* 24.3 * 1000
	Basic:  40 pairs = MOL("NaCO3-") + MOL("MgCO3")
	Basic:  50 PUNCH pairs
	Basic:  60 REM punch reaction increment
	Basic:  70 PUNCH RXN
	Line 3:      -end
	*/

	CString strLines;
	CString strFormat;

	// Line 0
	strLines = _T("USER_PUNCH");

	// Line 1
	strFormat.Format(_T("%s-headings %s"),
		(LPCTSTR)s_strNewLine,
		(LPCTSTR)m_Page1.m_strHead
		);
	strLines += strFormat;

	// Line 2
	strFormat.Format(_T("%s-start"),
		(LPCTSTR)s_strNewLine
		);
	strLines += strFormat;

	// Basic
	std::list<basic_command>::const_iterator command_iter = m_Page1.m_listCommands.begin();
	for ( ; command_iter != m_Page1.m_listCommands.end(); ++command_iter)
	{
		strFormat.Format(_T("%s%d %s"),
			(LPCTSTR)s_strNewLine,
			(*command_iter).nLine,
			(LPCTSTR)(*command_iter).strCommand
			);
		strLines += strFormat;
	}

	// Line 2
	strFormat.Format(_T("%s-end"),
		(LPCTSTR)s_strNewLine
		);
	strLines += strFormat;

	return strLines + s_strNewLine;
}

void COCKSUserPunch::Edit(CString& rStr)
{
	CKeywordLoader2 keywordLoader2(rStr);

	ASSERT(user_punch != NULL);
	CString strDelim = _T(" ");

	// -headings
	if (user_punch_count_headings > 0 )
	{
		m_Page1.m_strHead = user_punch_headings[0];
		for (int i = 1; i < user_punch_count_headings; ++i)
		{		
			m_Page1.m_strHead += strDelim + user_punch_headings[i];
		}
	}

	CRate rate(user_punch);

	m_Page1.m_listCommands.assign(rate.m_listCommands.begin(), rate.m_listCommands.end());
}



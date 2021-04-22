// KSSolutionSpecies.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KSSolutionSpecies.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSSolutionSpecies

IMPLEMENT_DYNAMIC(CKSSolutionSpecies, baseCKSSolutionSpecies)

CKSSolutionSpecies::CKSSolutionSpecies(CWnd* pWndParent)
	 : baseCKSSolutionSpecies(IDS_PROPSHT_CAPTION26, pWndParent)
{
	m_bNeedDatabase = false;
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

CKSSolutionSpecies::~CKSSolutionSpecies()
{
}


BEGIN_MESSAGE_MAP(CKSSolutionSpecies, baseCKSSolutionSpecies)
	//{{AFX_MSG_MAP(CKSSolutionSpecies)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSSolutionSpecies message handlers

CString CKSSolutionSpecies::GetString()
{
	/*
	Line 0:  SOLUTION_SPECIES 
	Line 1a: SO4-2 = SO4-2
	Line 2a:      log_k     0.0
	Line 5a:      -gamma    5.0     -0.04
	Line 1b: SO4-2 + 9H+ + 8e- = HS- + 4H2O
	Line 2b:      log_k     33.652
	Line 3b:      delta_h   -40.14
	Line 5b:      -gamma    3.5     0.0
	Line 1c: H2O = OH- + H+
	Line 2c:      log_k     -14.000
	Line 3c:      delta_h   13.362  kcal 
	Line 4c:      -a_e -283.971 -0.05069842 13323.0  102.24447 -1119669.0
	Line 5c:      -gamma    3.5     0.0
	Line 1d: HS-  = S2-2 + H+
	Line 2d:      log_k     -14.528
	Line 3d:      delta_h   11.4
	Line 6:       -no_check 
	Line 7d:      -mole_balance    S(-2)2
	*/

	// Line 0
	CString strLines = _T("SOLUTION_SPECIES");
	CString strFormat;

	std::list<CSpecies>::const_iterator cIter = m_Page1.m_listSpecies.begin();
	for (; cIter != m_Page1.m_listSpecies.end(); ++cIter)
	{
		// Line 1 rxn
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			(LPCTSTR)cIter->m_strEqn
			);
		strLines += strFormat;

		// Line 2 log_k
		if (cIter->m_dLogK == cIter->m_dLogK)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("log_k"),
				DBL_DIG,
				cIter->m_dLogK
				);
			strLines += strFormat;
		}

		// Line 3 delta_h
		if (cIter->m_dDeltaH == cIter->m_dDeltaH)
		{
			CString strUnits;
			switch (cIter->m_nDeltaHUnits)
			{
			case kcal:
				strUnits = _T("kcal");
				break;
			case cal:
				strUnits = _T("cal");
				break;
			case kjoules:
				strUnits = _T("kJ");
				break;
			case joules:
				strUnits = _T("J");
				break;
			}
			strFormat.Format(_T("%s%4c%-9s %.*g %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("delta_h"),
				DBL_DIG,
				cIter->m_dDeltaH,
				(LPCTSTR)strUnits
				);
			strLines += strFormat;
		}

		// Line 4 analytical_expression
		if (cIter->m_bHasAnalExp)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g %.*g %.*g %.*g %.*g %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-analytical_expression"),
				DBL_DIG,
				cIter->m_dA1,
				DBL_DIG,
				cIter->m_dA2,
				DBL_DIG,
				cIter->m_dA3,
				DBL_DIG,
				cIter->m_dA4,
				DBL_DIG,
				cIter->m_dA5,
				DBL_DIG,
				cIter->m_dA6
				);
			strLines += strFormat;
		}

		// Line 5 -gamma
		if (cIter->m_nActType == CSpecies::AT_DEBYE_HUCKEL)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-gamma"),
				DBL_DIG,
				cIter->m_dDHa,
				DBL_DIG,
				cIter->m_dDHb
				);
			strLines += strFormat;
		}

		// llnl_gamma
		if (cIter->m_nActType == CSpecies::AT_LLNL_DH)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-llnl_gamma"),
				DBL_DIG,
				cIter->m_dDHa
				);
			strLines += strFormat;
		}

		// co2_llnl_gamma
		if (cIter->m_nActType == CSpecies::AT_LLNL_DH_CO2)
		{
			strFormat.Format(_T("%s%4c%-9s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-co2_llnl_gamma"),
				DBL_DIG,
				cIter->m_dDHa
				);
			strLines += strFormat;
		}

		// -dw
		if (cIter->m_dw == cIter->m_dw && cIter->m_dw > 0)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-dw"),
				DBL_DIG,
				cIter->m_dw
				);
			strLines += strFormat;
		}

		// -erm_ddl
		if (cIter->m_erm_ddl == cIter->m_erm_ddl && cIter->m_erm_ddl != 1.0)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-erm_ddl"),
				DBL_DIG,
				cIter->m_erm_ddl
				);
			strLines += strFormat;
		}

		// -millero
		if (cIter->m_bHasMillero)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g %.*g %.*g %.*g %.*g %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-millero"),
				DBL_DIG,
				cIter->m_millero[0],
				DBL_DIG,
				cIter->m_millero[1],
				DBL_DIG,
				cIter->m_millero[2],
				DBL_DIG,
				cIter->m_millero[3],
				DBL_DIG,
				cIter->m_millero[4],
				DBL_DIG,
				cIter->m_millero[5]
				);
			strLines += strFormat;
		}

		// Line 6 -no_check
		if (!cIter->m_bCheckEqn)
		{
			strFormat.Format(_T("%s%4c%-9s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-no_check")
				);
			strLines += strFormat;

			// Line 7 -mole_balance
// COMMENT: {8/9/2001 2:20:54 PM}			ASSERT(!cIter->m_strMoleBalance.IsEmpty());
			if (!cIter->m_strMoleBalance.IsEmpty())
			{
				strFormat.Format(_T("%s%4c%-9s"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					_T("-mole_balance"),
					(LPCTSTR)cIter->m_strMoleBalance
					);
				strLines += strFormat;
			}
		}
	}

	return strLines + s_strNewLine;
}

CString WriteSpeciesReaction(class species *species_ptr);

void CKSSolutionSpecies::Edit(CString& rStr)
{
	try
	{
		PhreeqcI p(rStr);
		p.GetData(this);
	}
	catch (...)
	{
		CString strResource;
		strResource.LoadString(IDS_EXCEPTION_ACCESS_VIOLATION);
		::MessageBox(NULL, strResource, _T("Unhandled Exception"), MB_OK|MB_ICONERROR);
	}
}

CString WriteSpeciesReaction(class species *species_ptr)
{
	CString strEqn;
	CString strFormat;
	bool bFirst;
	int j;

	//
	// reaction
	//

	// left hand side
	bFirst = true;
	for (j = 0; species_ptr->rxn.token[j].s != NULL; ++j)
	{
		if (species_ptr->rxn.token[j].coef < 0) continue;
		if(!bFirst)
		{
			strEqn += _T(" + ");
		}
		bFirst = false;
		if (species_ptr->rxn.token[j].coef != 1)
		{
			strFormat.Format("%g%s", species_ptr->rxn.token[j].coef, species_ptr->rxn.token[j].s->name);
		}
		else
		{
			strFormat.Format("%s", species_ptr->rxn.token[j].s->name);
		}
		strEqn += strFormat;
	}

	// right hand side
	strEqn += _T(" = ");
	bFirst = true;
	for (j = 0; species_ptr->rxn.token[j].s != NULL; ++j)
	{
		if (species_ptr->rxn.token[j].coef > 0) continue;
		if(!bFirst)
		{
			strEqn += _T(" + ");
		} 
		bFirst = false;
		if (-species_ptr->rxn.token[j].coef != 1)
		{
			strFormat.Format("%g%s", -species_ptr->rxn.token[j].coef, species_ptr->rxn.token[j].s->name);
		}
		else
		{
			strFormat.Format("%s", species_ptr->rxn.token[j].s->name);
		}
		strEqn += strFormat;
	}

	return strEqn;
}

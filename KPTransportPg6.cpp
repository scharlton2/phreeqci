// KPTransportPg6.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "phrqtype.h"          // LDBLE
#include "KPTransportPg6.h"

// COMMENT: {11/19/2009 7:34:13 PM}#include "OCKSTransport.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPTransportPg6, baseCKPTransportPg6)


/////////////////////////////////////////////////////////////////////////////
// CKPTransportPg6 property page

CKPTransportPg6::CKPTransportPg6() : baseCKPTransportPg6(CKPTransportPg6::IDD)
, m_bUseMCD(false)
, m_bUseID(false)
, m_default_Dw(1e-9)
, m_multi_Dpor(0.3)
, m_multi_Dpor_lim(0.0)
, m_multi_Dn(1.0)
, m_mcd_substeps(1.0)
, m_interlayer_Dpor(0.1)
, m_interlayer_Dpor_lim(0.0)
, m_interlayer_tortf(100.0)
{
}

CKPTransportPg6::~CKPTransportPg6()
{
}

void CKPTransportPg6::DoDataExchange(CDataExchange* pDX)
{
	baseCKPTransportPg6::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_MULTI_D, m_ctrlUseMCD);
	DDX_Control(pDX, IDC_CHECK_INTERLAYER_D, m_ctrlUseID);
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);

	if (m_bFirstSetActive)
	{
	}

	if (pDX->m_bSaveAndValidate)
	{
		// multi_d
		if (this->m_ctrlUseMCD.GetCheck() == BST_CHECKED)
		{
			this->m_bUseMCD = true;

			// interlayer_D
			if (this->m_ctrlUseID.GetCheck() == BST_CHECKED)
			{
				this->m_bUseID = true;
			}
			else
			{
				this->m_bUseID = false;
			}
		}
		else
		{
			this->m_bUseMCD = false;
			this->m_bUseID  = false;
		}

		if (this->m_bUseMCD)
		{
			CString str;
			::DDX_Text(pDX, IDC_EDIT_DIFF_COEF, str);
			if (str.IsEmpty())
			{
				::AfxMessageBox(_T("Expected default species diffusion coefficient in water at 25�C, m^2/s."));
				pDX->Fail();
			}
			::DDX_Text(pDX, IDC_EDIT_POROSITY, str);
			if (str.IsEmpty())
			{
				::AfxMessageBox(_T("Expected porosity to calculate diffusion coefficient."));
				pDX->Fail();
			}
			::DDX_Text(pDX, IDC_EDIT_POROSITY_LIMIT, str);
			if (str.IsEmpty())
			{
				::AfxMessageBox(_T("Expected porosity limit for diffusive transport."));
				pDX->Fail();
			}
			::DDX_Text(pDX, IDC_EDIT_POROSITY_EXP, str);
			if (str.IsEmpty())
			{
				::AfxMessageBox(_T("Expected exponent for porosity reduction of diffusion coefficient (Dp = Dw * (por)^n)."));
				pDX->Fail();
			}
			::DDX_Text(pDX, IDC_EDIT_TIME_SUBSTEPS, str);
			if (str.IsEmpty())
			{
				::AfxMessageBox(_T("Expected substep factor for MCD."));
				pDX->Fail();
			}

			// multi_d
			::DDX_Text(pDX, IDC_EDIT_DIFF_COEF,      this->m_default_Dw);
			::DDX_Text(pDX, IDC_EDIT_POROSITY,       this->m_multi_Dpor);
			::DDX_Text(pDX, IDC_EDIT_POROSITY_LIMIT, this->m_multi_Dpor_lim);
			::DDX_Text(pDX, IDC_EDIT_POROSITY_EXP,   this->m_multi_Dn);
			::DDX_Text(pDX, IDC_EDIT_TIME_SUBSTEPS,  this->m_mcd_substeps);
			if (this->m_mcd_substeps < 1)
			{
				::AfxMessageBox(_T("Substep factor in MCD must be >= 1.0"));
				pDX->Fail();
			}

			if (this->m_bUseID)
			{

				::DDX_Text(pDX, IDC_EDIT_POROSITY_ID, str);
				if (str.IsEmpty())
				{
					::AfxMessageBox(_T("Expected interlayer porosity."));
					pDX->Fail();
				}
				::DDX_Text(pDX, IDC_EDIT_POROSITY_MIN, str);
				if (str.IsEmpty())
				{
					::AfxMessageBox(_T("Expected interlayer porosity limit for diffusive transport."));
					pDX->Fail();
				}
				::DDX_Text(pDX, IDC_EDIT_TORTUOSITY, str);
				if (str.IsEmpty())
				{
					::AfxMessageBox(_T("Expected interlayer tortuosity factor (Dp = Dw /t_f)."));
					pDX->Fail();
				}

				// interlayer_D
				::DDX_Text(pDX, IDC_EDIT_POROSITY_ID,  this->m_interlayer_Dpor);
				::DDX_Text(pDX, IDC_EDIT_POROSITY_MIN, this->m_interlayer_Dpor_lim);
				::DDX_Text(pDX, IDC_EDIT_TORTUOSITY,   this->m_interlayer_tortf);
			}
		}
	}
	else
	{
		if (this->m_bUseMCD)
		{
			this->m_ctrlUseMCD.SetCheck(BST_CHECKED);
		}
		else
		{
			this->m_ctrlUseMCD.SetCheck(BST_UNCHECKED);
		}

		if (this->m_bUseID)
		{
			this->m_ctrlUseID.SetCheck(BST_CHECKED);
		}
		else
		{
			this->m_ctrlUseID.SetCheck(BST_UNCHECKED);
		}

		::DDX_Text(pDX, IDC_EDIT_DIFF_COEF,      this->m_default_Dw);
		::DDX_Text(pDX, IDC_EDIT_POROSITY,       this->m_multi_Dpor);
		::DDX_Text(pDX, IDC_EDIT_POROSITY_LIMIT, this->m_multi_Dpor_lim);
		::DDX_Text(pDX, IDC_EDIT_POROSITY_EXP,   this->m_multi_Dn);
		::DDX_Text(pDX, IDC_EDIT_TIME_SUBSTEPS,  this->m_mcd_substeps);

		::DDX_Text(pDX, IDC_EDIT_POROSITY_ID,  this->m_interlayer_Dpor);
		::DDX_Text(pDX, IDC_EDIT_POROSITY_MIN, this->m_interlayer_Dpor_lim);
		::DDX_Text(pDX, IDC_EDIT_TORTUOSITY,   this->m_interlayer_tortf);

		this->UpdateMCD();
		this->UpdateID();
	}
}


BEGIN_MESSAGE_MAP(CKPTransportPg6, baseCKPTransportPg6)
	ON_BN_SETFOCUS(IDC_CHECK_MULTI_D, &CKPTransportPg6::OnBnSetfocusCheckMultiD)
	ON_BN_CLICKED(IDC_CHECK_MULTI_D, &CKPTransportPg6::OnBnClickedCheckMultiD)
	ON_BN_SETFOCUS(IDC_CHECK_INTERLAYER_D, &CKPTransportPg6::OnBnSetfocusCheckInterlayerD)
	ON_BN_CLICKED(IDC_CHECK_INTERLAYER_D, &CKPTransportPg6::OnBnClickedCheckInterlayerD)
	ON_EN_SETFOCUS(IDC_EDIT_DIFF_COEF, &CKPTransportPg6::OnEnSetfocusEditDiffCoef)
	ON_EN_SETFOCUS(IDC_EDIT_POROSITY, &CKPTransportPg6::OnEnSetfocusEditPorosity)
	ON_EN_SETFOCUS(IDC_EDIT_POROSITY_LIMIT, &CKPTransportPg6::OnEnSetfocusEditPorosityLimit)
	ON_EN_SETFOCUS(IDC_EDIT_POROSITY_EXP, &CKPTransportPg6::OnEnSetfocusEditPorosityExp)
	ON_EN_SETFOCUS(IDC_EDIT_POROSITY_ID, &CKPTransportPg6::OnEnSetfocusEditPorosityId)
	ON_EN_SETFOCUS(IDC_EDIT_POROSITY_MIN, &CKPTransportPg6::OnEnSetfocusEditPorosityMin)
	ON_EN_SETFOCUS(IDC_EDIT_TORTUOSITY, &CKPTransportPg6::OnEnSetfocusEditTortuosity)
	ON_WM_HELPINFO()
	ON_EN_SETFOCUS(IDC_EDIT_TIME_SUBSTEPS, &CKPTransportPg6::OnEnSetfocusEditTimeSubsteps)
END_MESSAGE_MAP()

BOOL CKPTransportPg6::OnInitDialog()
{
	baseCKPTransportPg6::OnInitDialog();

	// set layout
	CreateRoot(VERTICAL, 5, 6)
			<< (paneCtrl(IDC_GB_MCD_PARAMS, HORIZONTAL, GREEDY, 0, 18, 8, 86)
				<< (pane(VERTICAL, GREEDY, 0, 0, 0)
					//<< itemFixed(VERTICAL, 10)
					<< item(IDC_CHECK_MULTI_D, ABSOLUTE_VERT)
					<< itemFixed(VERTICAL, 10)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< itemFixed(HORIZONTAL, 25)
						<< item(IDC_STATIC_DIFF_COEF, ABSOLUTE_HORZ)
						<< itemFixed(HORIZONTAL, 14)
						<< item(IDC_EDIT_DIFF_COEF, ABSOLUTE_VERT)
						<< itemGrowing(HORIZONTAL)
						)
					<< itemFixed(VERTICAL, 10)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< itemFixed(HORIZONTAL, 25)
						<< item(IDC_STATIC_POROSITY, ABSOLUTE_HORZ)
						<< itemFixed(HORIZONTAL, 14)
						<< item(IDC_EDIT_POROSITY, ABSOLUTE_VERT)
						<< itemGrowing(HORIZONTAL)
						)
					<< itemFixed(VERTICAL, 10)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< itemFixed(HORIZONTAL, 25)
						<< item(IDC_STATIC_POROSITY_LIMIT, ABSOLUTE_HORZ)
						<< itemFixed(HORIZONTAL, 14)
						<< item(IDC_EDIT_POROSITY_LIMIT, ABSOLUTE_VERT)
						<< itemGrowing(HORIZONTAL)
						)
					<< itemFixed(VERTICAL, 10)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< itemFixed(HORIZONTAL, 25)
						<< item(IDC_STATIC_POROSITY_EXP, ABSOLUTE_HORZ)
						<< itemFixed(HORIZONTAL, 14)
						<< item(IDC_EDIT_POROSITY_EXP, ABSOLUTE_VERT)
						<< itemGrowing(HORIZONTAL)
						)
					//{{
					<< itemFixed(VERTICAL, 10)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< itemFixed(HORIZONTAL, 25)
						<< item(IDC_STATIC_TIME_SUBSTEPS, ABSOLUTE_HORZ)
						<< itemFixed(HORIZONTAL, 14)
						<< item(IDC_EDIT_TIME_SUBSTEPS, ABSOLUTE_VERT)
						<< itemGrowing(HORIZONTAL)
						)
					//}}
					<< itemFixed(VERTICAL, 10)
					<< item(IDC_CHECK_INTERLAYER_D, ABSOLUTE_VERT)
					<< itemFixed(VERTICAL, 10)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< itemFixed(HORIZONTAL, 25)
						<< item(IDC_STATIC_POROSITY_ID, ABSOLUTE_HORZ)
						<< itemFixed(HORIZONTAL, 14)
						<< item(IDC_EDIT_POROSITY_ID, ABSOLUTE_VERT)
						<< itemGrowing(HORIZONTAL)
						)
					<< itemFixed(VERTICAL, 10)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< itemFixed(HORIZONTAL, 25)
						<< item(IDC_STATIC_POROSITY_MIN, ABSOLUTE_HORZ)
						<< itemFixed(HORIZONTAL, 14)
						<< item(IDC_EDIT_POROSITY_MIN, ABSOLUTE_VERT)
						<< itemGrowing(HORIZONTAL)
						)
					<< itemFixed(VERTICAL, 10)
					<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
						<< itemFixed(HORIZONTAL, 25)
						<< item(IDC_STATIC_TORTUOSITY, ABSOLUTE_HORZ)
						<< itemFixed(HORIZONTAL, 14)
						<< item(IDC_EDIT_TORTUOSITY, ABSOLUTE_VERT)
						<< itemGrowing(HORIZONTAL)
						)
					)
				)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10, 30)
				<< item(IDC_DESCRIPTION, GREEDY)
				)
	;
	UpdateLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKPTransportPg6::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));

	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0


	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;
	switch (pHelpInfo->iCtrlId)
	{
	case IDC_CHECK_MULTI_D:
		strRes.LoadString(IDS_STRING664);
		break;
	case IDC_CHECK_INTERLAYER_D:
		strRes.LoadString(IDS_STRING665);
		break;
	case IDC_EDIT_DIFF_COEF: case IDC_STATIC_DIFF_COEF:
		strRes.LoadString(IDS_STRING666);
		break;
	case IDC_EDIT_POROSITY: case IDC_STATIC_POROSITY:
		strRes.LoadString(IDS_STRING667);
		break;
	case IDC_EDIT_POROSITY_LIMIT: case IDC_STATIC_POROSITY_LIMIT:
		strRes.LoadString(IDS_STRING668);
		break;
	case IDC_EDIT_POROSITY_EXP: case IDC_STATIC_POROSITY_EXP:
		strRes.LoadString(IDS_STRING669);
		break;
	case IDC_EDIT_POROSITY_ID: case IDC_STATIC_POROSITY_ID:
		strRes.LoadString(IDS_STRING670);
		break;
	case IDC_EDIT_POROSITY_MIN: case IDC_STATIC_POROSITY_MIN:
		strRes.LoadString(IDS_STRING671);
		break;
	case IDC_EDIT_TORTUOSITY: case IDC_STATIC_TORTUOSITY:
		strRes.LoadString(IDS_STRING672);
		break;
	default:
		// No help topic is associated with this item.
		ASSERT(FALSE);
		strRes.LoadString(IDS_STRING441);
		// return baseCKPAdvectionPg1::OnHelpInfo(pHelpInfo);
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}


void CKPTransportPg6::UpdateMCD()
{
	BOOL bEnable = (BST_CHECKED == this->m_ctrlUseMCD.GetCheck());

	if (CWnd* pWnd = this->GetDlgItem(IDC_STATIC_DIFF_COEF))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_DIFF_COEF))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_STATIC_POROSITY))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_POROSITY))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_STATIC_POROSITY_LIMIT))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_POROSITY_LIMIT))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_STATIC_POROSITY_EXP))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_POROSITY_EXP))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_STATIC_TIME_SUBSTEPS))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_TIME_SUBSTEPS))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_CHECK_INTERLAYER_D))
	{
		pWnd->EnableWindow(bEnable);
		this->UpdateID();
	}
}

void CKPTransportPg6::UpdateID()
{
	BOOL bEnable = (BST_CHECKED == this->m_ctrlUseID.GetCheck() && this->m_ctrlUseID.IsWindowEnabled());

	if (CWnd* pWnd = this->GetDlgItem(IDC_STATIC_POROSITY_ID))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_POROSITY_ID))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_STATIC_POROSITY_MIN))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_POROSITY_MIN))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_STATIC_TORTUOSITY))
	{
		pWnd->EnableWindow(bEnable);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_TORTUOSITY))
	{
		pWnd->EnableWindow(bEnable);
	}
}

void CKPTransportPg6::OnBnSetfocusCheckMultiD()
{
	CString strRes;
	strRes.LoadString(IDS_STRING664);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg6::OnBnClickedCheckMultiD()
{
	this->UpdateMCD();
}

void CKPTransportPg6::OnBnSetfocusCheckInterlayerD()
{
	CString strRes;
	strRes.LoadString(IDS_STRING665);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg6::OnBnClickedCheckInterlayerD()
{
	this->UpdateID();
}

void CKPTransportPg6::OnEnSetfocusEditDiffCoef()
{
	CString strRes;
	strRes.LoadString(IDS_STRING666);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg6::OnEnSetfocusEditPorosity()
{
	CString strRes;
	strRes.LoadString(IDS_STRING667);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg6::OnEnSetfocusEditPorosityLimit()
{
	CString strRes;
	strRes.LoadString(IDS_STRING668);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg6::OnEnSetfocusEditPorosityExp()
{
	CString strRes;
	strRes.LoadString(IDS_STRING669);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg6::OnEnSetfocusEditPorosityId()
{
	CString strRes;
	strRes.LoadString(IDS_STRING670);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg6::OnEnSetfocusEditPorosityMin()
{
	CString strRes;
	strRes.LoadString(IDS_STRING671);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg6::OnEnSetfocusEditTortuosity()
{
	CString strRes;
	strRes.LoadString(IDS_STRING672);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg6::OnEnSetfocusEditTimeSubsteps()
{
	CString strRes;
	strRes.LoadString(IDS_STRING703);
	m_eInputDesc.SetWindowText(strRes);
}

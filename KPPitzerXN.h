#pragma once

#include <afxctl.h> // for class CPictureHolder
#include "KeywordPage.h"
#include "EditGrid.h"
#include "afxwin.h"
#include "PitzParam.h"
#include "Database.h"

// CKPPitzerXN dialog

#ifndef baseCKPPitzerXN
#define baseCKPPitzerXN CKeywordPage
#endif

class CKPPitzerXN : public baseCKPPitzerXN
{
	DECLARE_DYNCREATE(CKPPitzerXN)

public:
	CKPPitzerXN(UINT nIDTemplate = CKPPitzerXN::IDD, UINT nIDCaption = 0);
	virtual ~CKPPitzerXN();

// Dialog Data
	enum { IDD = IDD_KEY_PITZER_XN };

protected:
	bool m_bIgnoreChanges;
	HWND m_hWndLastControl;    // last control used
	BOOL m_bEditLastControl;   // last control was an edit item
	CPictureHolder m_pictHolderRightArrow;
	CPictureHolder m_pictHolderStar;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void InitGrid(CDataExchange* pDX, int nIDC);
	void ResetFocus();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditA0();
	afx_msg void OnEnChangeEditA1();
	afx_msg void OnEnChangeEditA2();
	afx_msg void OnEnChangeEditA3();
	afx_msg void OnEnChangeEditA4();
	afx_msg void OnEnChangeEditA5();

	afx_msg void OnEnSetfocusEditA0();
	afx_msg void OnEnSetfocusEditA1();
	afx_msg void OnEnSetfocusEditA2();
	afx_msg void OnEnSetfocusEditA3();
	afx_msg void OnEnSetfocusEditA4();
	afx_msg void OnEnSetfocusEditA5();

	afx_msg void OnEnKillfocusEditA0();
	afx_msg void OnEnKillfocusEditA1();
	afx_msg void OnEnKillfocusEditA2();
	afx_msg void OnEnKillfocusEditA3();
	afx_msg void OnEnKillfocusEditA4();
	afx_msg void OnEnKillfocusEditA5();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowInsertRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInsertRow(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowDeleteRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteRow(WPARAM wParam, LPARAM lParam);

	CEdit       m_ctrlA0;
	CEdit       m_ctrlA1;
	CEdit       m_ctrlA2;
	CEdit       m_ctrlA3;
	CEdit       m_ctrlA4;
	CEdit       m_ctrlA5;
	CSRCDBPG	m_pager;
	CEditGrid	m_ctrlGrid;
	CEdit	    m_eInputDesc;

	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void OnRowColChangeGrid();
	void OnEnterCellGrid();
	void OnLeaveCellGrid();
	void OnGotoFirstClicked();
	void OnGotoLastClicked();
	void OnGotoNewClicked();
	void OnGotoNextClicked();
	void OnGotoPrevClicked();
	void OnDblClickGrid();
	void OnKeyDownGrid(short* KeyCode, short Shift);
	CComboBox m_ctrlX; // IDC_CB_X
	CComboBox m_ctrlN; // IDC_CB_N

public:
	std::list<CPitzParam> m_listParams;

	afx_msg void OnCbnEditchangeCbX1();
	afx_msg void OnCbnSelchangeCbX1();
	afx_msg void OnCbnEditchangeCbX2();
	afx_msg void OnCbnSelchangeCbX2();
	afx_msg void OnCbnSetfocusCbX();
	afx_msg void OnCbnSetfocusCbN();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

// BasicObj.cpp: implementation of the CBasicObj class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "BasicObj.h"

#include "DDX_DDV.h"
#include "EditGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define tokvar          0
#define toknum          1
#define tokstr          2
#define toksnerr        3
#define tokplus         4
#define tokminus        5
#define toktimes        6
#define tokdiv          7
#define tokup           8
#define toklp           9
#define tokrp           10
#define tokcomma        11
#define toksemi         12
#define tokcolon        13
#define tokeq           14
#define toklt           15
#define tokgt           16
#define tokle           17
#define tokge           18
#define tokne           19
#define tokand          20
#define tokor           21
#define tokxor          22
#define tokmod          23
#define toknot          24
#define toksqr          25
#define toksqrt         26
#define toksin          27
#define tokcos          28
#define toktan          29
#define tokarctan       30
#define toklog          31
#define tokexp          32
#define tokabs          33
#define toksgn          34
#define tokstr_         35
#define tokval          36
#define tokchr_         37
#define tokasc          38
#define toklen          39
#define tokmid_         40
#define tokpeek         41
#define tokrem          42
#define toklet          43
#define tokprint        44
#define tokinput        45
#define tokgoto         46
#define tokif           47
#define tokend          48
#define tokstop         49
#define tokfor          50
#define toknext         51
#define tokwhile        52
#define tokwend         53
#define tokgosub        54
#define tokreturn       55
#define tokread         56
#define tokdata         57
#define tokrestore      58
#define tokgotoxy       59
#define tokon           60
#define tokdim          61
#define tokpoke         62
#define toklist         63
#define tokrun          64
#define toknew          65
#define tokload         66
#define tokmerge        67
#define toksave         68
#define tokbye          69
#define tokdel          70
#define tokrenum        71
#define tokthen         72
#define tokelse         73
#define tokto           74
#define tokstep         75
#define toktc           76
#define tokm0           77
#define tokm            78
#define tokparm         79
#define tokact          80
#define tokmol          81
#define tokla           82
#define toklm           83
#define toksr           84
#define toksi           85
#define toktot          86
#define toktk           87
#define toktime         88
#define toklog10        89
#define toksim_time     90
#define tokequi         91
#define tokgas          92
#define tokpunch        93
#define tokkin          94
#define toks_s          95
#define tokmu           96
#define tokalk          97
#define tokrxn          98
#define tokdist         99
#define tokmisc1        100
#define tokmisc2        101
#define tokedl          102
#define tokstep_no      103
#define toksim_no       104
#define toktotal_time   105
#define tokput          106
#define tokget          107
#define tokcharge_balance  109
#define tokpercent_error   110
#ifdef PHREEQ98
#define tokgraph_x        111
#define tokgraph_y        112
#define tokgraph_sy       113
#endif
#define tokcell_no      114
#define tokexists       115
#define toksurf         116
#define toklk_species   117
#define toklk_named     118
#define toklk_phase     119
#define toksum_species  120
#define toksum_gas      121
#define toksum_s_s      122
#define tokcalc_value   123
//{{ 2.9 added functions
#define tokdescription  124
#define toksys          125
#define tokinstr        126
#define tokltrim        127
#define tokrtrim        128
#define toktrim         129
#define tokpad          130
//{{ 2.13 
#define tokchange_por   131
#define tokget_por    	132
#define tokosmotic    	133
#define tokchange_surf  134
//}} 2.13
//}} 2.9 added functions



#define Char        char      /* Characters (not bytes) */
#define varnamelen      20
#define maxdims         4

typedef Char varnamestring[varnamelen + 1];

typedef struct tokenrec {
  struct tokenrec *next;
  int kind;
  union {
    struct varrec *vp;
    double num;
    Char *sp;
    Char snch;
  } UU;
#ifdef PHREEQCI_GUI
  size_t n_sz;
  Char *sz_num;
#endif
} tokenrec;

struct linerec {
  long num, num2;
  tokenrec *txt;
  struct linerec *next;
};

struct varrec {
  varnamestring name;
  struct varrec *next;
  long dims[maxdims];
  char numdims;
  boolean stringvar;
  union {
    struct {
      double *arr;
      double *val, rv;
    } U0;
    struct {
      Char **sarr;
      Char **sval, *sv;
    } U1;
  } UU;
};

// static inline functions
static inline linerec* FindLine(linerec* linebase, long n)
{
	linerec* l;
	l = linebase;
	while (l != NULL && l->num != n)
		l = l->next;
	return l;
}

extern "C"
{
#define EXTERNAL extern
#include "phreeqc/src/global.h"
#include "phreeqc/src/output.h"	
	int basic_renumber_1(char *commands, void **lnbase, void **vbase, void **lpbase);

	int basic_compile_1(char *commands, void **lnbase, void **vbase, void **lpbase, int parse_whole_program_flag);

	int basic_run_1(char *commands, void *lnbase, void *vbase, void *lpbase, int parse_whole_program_flag, HANDLE hInfiniteLoop);
	void basic_free(void *lnbase, void *vbase, void *lpbase);

	void cmd_initialize_1(void);
	void cmd_free_1(void);

	void initialize(void);
	int clean_up(void);

	extern int P_escapecode;
	extern UINT g_nIDErrPrompt;
	extern int g_nErrLineNumber;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicObj::CBasicObj()
{
	ASSERT(P_escapecode     == 0);
	ASSERT(g_nIDErrPrompt   == 0);
	ASSERT(g_nErrLineNumber == 0);

#if defined(SAVE_TEMP_FILE_EXAMPLE)
	// init files for phreeqc
	OpenTempFiles();
#endif

	initialize();

	ASSERT(P_escapecode == 0);
	// init basic hash table
	cmd_initialize_1();
}

CBasicObj::~CBasicObj()
{
	// remove basic hash table
	cmd_free_1();

	clean_up();

// COMMENT: {10/21/2004 2:04:16 PM}	// clean up temp files
// COMMENT: {10/21/2004 2:04:16 PM}	RemoveTempFiles();

	P_escapecode     = 0;
	g_nIDErrPrompt   = 0;
	g_nErrLineNumber = 0;
}

#if defined(SAVE_TEMP_FILE_EXAMPLE)

bool CBasicObj::OpenTempFiles()
{
	// sanity check
	_ASSERTE( input      == NULL );
	_ASSERTE( output     == NULL );
	_ASSERTE( log_file   == NULL );
	_ASSERTE( punch_file == NULL );
	_ASSERTE( dump_file  == NULL );
	_ASSERTE( error_file == NULL );
#ifdef _DEBUG
	// get temp directory
	TCHAR szTempDirectory[MAX_PATH];
	GetTempPath(MAX_PATH, szTempDirectory);

	// open temp files
	TCHAR szFile[MAX_PATH];

	GetTempFileName(szTempDirectory, _T("err"), 0, szFile);	
	VERIFY( m_errFile.Open(szFile, CFile::modeWrite | CFile::modeCreate) );

	GetTempFileName(szTempDirectory, _T("log"), 0, szFile);	
	VERIFY( m_logFile.Open(szFile, CFile::modeWrite | CFile::modeCreate) );

	GetTempFileName(szTempDirectory, _T("out"), 0, szFile);	
	VERIFY( m_outFile.Open(szFile, CFile::modeWrite | CFile::modeCreate) );
#else
	// Don't actually create files in release build
	VERIFY( m_errFile.Open(_T("NUL"), CFile::modeWrite | CFile::modeCreate) );
	VERIFY( m_logFile.Open(_T("NUL"), CFile::modeWrite | CFile::modeCreate) );
	VERIFY( m_outFile.Open(_T("NUL"), CFile::modeWrite | CFile::modeCreate) );
#endif	

	ASSERT(m_errFile.m_pStream != NULL);
	ASSERT(m_logFile.m_pStream != NULL);
	ASSERT(m_outFile.m_pStream != NULL);

	// set phreeqc vars
	error_file = m_errFile.m_pStream;
	log_file   = m_logFile.m_pStream;
	output     = m_outFile.m_pStream;
	input      = NULL;

	return true;
}
bool CBasicObj::RemoveTempFiles()
{
	CFile* arrfilesToRemove[] = { &m_errFile, &m_logFile, &m_outFile };

	for (int i = 0; i < sizeof(arrfilesToRemove) / sizeof(arrfilesToRemove[0]); ++i)
	{
		arrfilesToRemove[i]->Abort();

#ifdef _DEBUG
		// attempt to delete temp files
		try
		{
			CFile::Remove(arrfilesToRemove[i]->GetFilePath());
		}
		catch (CFileException* e)
		{
			e->ReportError();
			_ASSERTE(FALSE);
			e->Delete();
		}
#endif
	}

	// reset phreeqc vars
	input      = NULL;
	output     = NULL;
	log_file   = NULL;
	punch_file = NULL;
	dump_file  = NULL;
	error_file = NULL;

	return true;
}
#endif // SAVE_TEMP_FILE_EXAMPLE

UINT CBasicObj::ThreadProc(LPVOID pParam)
{
	ASSERT(pParam != NULL);

	struct XThreadParam* pThreadParam = (struct XThreadParam*)pParam;

	ASSERT(pThreadParam->hInfiniteLoop != NULL);
	ASSERT(pThreadParam->szCommand != NULL);
	ASSERT(pThreadParam->rate_ptr != NULL);

	int escapeCode = ::basic_run_1(
		pThreadParam->szCommand,
		pThreadParam->rate_ptr->linebase,
		pThreadParam->rate_ptr->varbase,
		pThreadParam->rate_ptr->loopbase,
		TRUE,
		pThreadParam->hInfiniteLoop
		);

	if (g_nIDErrPrompt != 0)
	{
		ASSERT(escapeCode != 0);
		return BASIC_ERRORS;
	}
	return BASIC_OK;
}

//
// nCol should contain the line number and nCol + 1 should contain the basic command
//
void CBasicObj::DDX_BasicCommand(CDataExchange* pDX, int nIDC, long nRow, long nCol, basic_command& value)
{
	if (pDX->m_bSaveAndValidate)
	{
		DDX_GridText(pDX, nIDC, nRow, nCol + 1, value.strCommand);
		CString strCommand = value.strCommand;
		strCommand.TrimRight();

		if (strCommand.IsEmpty())
		{
			// empty command ok (ignore line number)
			value.strCommand = _T("");
		}
		else
		{
			// get line number
			DDX_GridText(pDX, nIDC, nRow, nCol, value.nLine);
			if (value.nLine <= 0)
			{
				DDX_GridFail(pDX, _T("Invalid line number."));
			}

			// re-retrieve command to reset grid row and col
			DDX_GridText(pDX, nIDC, nRow, nCol + 1, value.strCommand);
			value.strCommand.TrimRight();

			// format command
			strCommand.Format(_T("%ld %s"), value.nLine, (LPCTSTR)value.strCommand);
			struct rate command = {0};
			command.commands = strCommand.GetBuffer(strCommand.GetLength() + 4);

			// check command
			ASSERT(P_escapecode == 0);

			// make enough space for inbuf
			int max_line_save = max_line;
			max_line = max(max_line, strCommand.GetLength() + 4);

			if (basic_compile_1(command.commands, &command.linebase, &command.varbase, &command.loopbase, FALSE) != 0)
			{
				// command contains errors 

				// restore
				max_line = max_line_save;

				// clean up BASIC before throwing
				basic_free(command.linebase, command.varbase, command.loopbase);
				strCommand.ReleaseBuffer();
				ASSERT(g_nIDErrPrompt != 0);

				DDX_GridFail(pDX, g_nIDErrPrompt, IDR_MAINFRAME);
			}
			else
			{
				// restore
				max_line = max_line_save;

				// command doesn't contain errors
				ASSERT(P_escapecode == 0);
				ASSERT(g_nIDErrPrompt == 0);
				strCommand.ReleaseBuffer();
				basic_free(command.linebase, command.varbase, command.loopbase);
			}
		}

	}
	else
	{
		// line number
		ASSERT(value.nLine > 0);
		DDX_GridText(pDX, nIDC, nRow, nCol, value.nLine);

		// command
		DDX_GridText(pDX, nIDC, nRow, nCol + 1, value.strCommand);
	}
}

// nCol should contain the line number and nCol + 1 should contain the basic command
//
void CBasicObj::DDV_BasicCommands(CDataExchange* pDX, int nIDC, std::list<basic_command>& r_listCommands, bool bRenumbering /* false */)
{
	if (pDX->m_bSaveAndValidate)
	{
		// verify state of globals
		ASSERT(P_escapecode     == 0);
		ASSERT(g_nIDErrPrompt   == 0);
		ASSERT(g_nErrLineNumber == 0);

		// display hourglass
		CWaitCursor wait;

		// concatenate commands into one string separated by semicolons
		//
		CString str = _T(";");
		std::list<basic_command>::iterator commandIter = r_listCommands.begin();
		CString strLine;
		for (; commandIter != r_listCommands.end(); ++commandIter)
		{
			strLine.Format(_T("%ld %s;"),
				(*commandIter).nLine,
				(*commandIter).strCommand
				);
			str += strLine;
		}

		// set default exit code (assume basic_compile_1 failure)
		DWORD dwExitCode = BASIC_ERRORS;

		// compile commands and if no errors run
		//
		struct rate command = {0};
		command.commands = str.GetBuffer(str.GetLength() + 4);

		// make enough space for inbuf
		int max_line_save = max_line;
		max_line = max(max_line, str.GetLength() + 4);

		if (basic_compile_1(command.commands, &command.linebase, &command.varbase, &command.loopbase, TRUE) == 0)
		{
			// restore
			max_line = max_line_save;

			// verify state of globals after successful compile
			ASSERT(P_escapecode == 0);
			ASSERT(g_nIDErrPrompt == 0);
			ASSERT(g_nErrLineNumber == 0);

			//{{{{{{{{{{{{{{{{{{{{{{
			// iterate over each line
			struct linerec *l = (struct linerec *)command.linebase;
			while (l != NULL)
			{
				// iterate over each token
				struct tokenrec *buf = l->txt;
				while(buf != NULL)
				{
					if (buf->kind == tokgoto || buf->kind == tokgosub || buf->kind == tokrestore)
					{
						while (buf && buf->next && buf->next->kind == toknum)
						{
							buf = buf->next;
							long nLine = (long)floor(buf->UU.num + 0.5);
							if (FindLine((struct linerec *)command.linebase, nLine) == NULL)
							{
								SetErrorCell(pDX, nIDC, l->num);
								str.Format(_T("Undefined line %ld in line %ld\n"), nLine, l->num);
								// cleanup BASIC
								basic_free(command.linebase, command.varbase, command.loopbase);
								DDX_GridFail(pDX, str);
							}
							// skip possible commas
							if (buf) buf = buf->next;
						}
					}
					if (buf) buf = buf->next;
				}
				l = l->next;
			}
			//}}}}}}}}}}}}}}}}}}}}}}

			// create event to notify thread to stop and return
			CEvent eventInfiniteLoop;
			eventInfiniteLoop.ResetEvent();

			// init thread param
			struct XThreadParam basicCheck;
			basicCheck.szCommand = "run";
			basicCheck.rate_ptr = &command;
			basicCheck.hInfiniteLoop = eventInfiniteLoop.m_hObject;

			// create suspended thread in order to set m_bAutoDelete
			CWinThread* pThread = ::AfxBeginThread(ThreadProc, &basicCheck, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			pThread->m_bAutoDelete = FALSE;

			// start thread
			pThread->ResumeThread();

			// wait 1.1 seconds for thread to finish
			DWORD dwWait = ::WaitForSingleObject(pThread->m_hThread, 1100);
			if (dwWait == WAIT_TIMEOUT)
			{
				// signal thread to return
				TRACE("Signaling thread\n");
				VERIFY(eventInfiniteLoop.SetEvent());

				// wait a maximum of 2 seconds for thread to finish
				for (int i = 0; i < 20; ++i)
				{
					ASSERT(pThread);
					dwWait = ::WaitForSingleObject(pThread->m_hThread, 100);
					if (dwWait != WAIT_TIMEOUT)
					{
						TRACE("Thread has returned\n");
						break;
					}
				}
			}

			// get exit status of thread (return value of BasicCheckProc())
			dwExitCode = BASIC_OK;
			VERIFY(::GetExitCodeThread(pThread->m_hThread, &dwExitCode));

			if (dwExitCode == STILL_ACTIVE)
			{
				ASSERT(dwWait == WAIT_TIMEOUT);
				::AfxMessageBox(_T("An unrecoverable error has occured.  The program will now exit"), MB_ICONSTOP);
				::TerminateThread(pThread->m_hThread, 1);
				basic_free(command.linebase, command.varbase, command.loopbase);
				delete pThread;
				AfxAbort(); //::PostQuitMessage(1);
				return;
			}

			// destroy thread
			delete pThread;
		}
		else
		{
			// restore
			max_line = max_line_save;

			// unable to compile
			// verify error state
			ASSERT(P_escapecode     != 0);
			ASSERT(g_nIDErrPrompt   != 0);
		}

		// cleanup BASIC
		basic_free(command.linebase, command.varbase, command.loopbase);

		// check for errors
		if (dwExitCode == BASIC_ERRORS)
		{
			ASSERT(g_nIDErrPrompt != 0);

			SetErrorCell(pDX, nIDC, g_nErrLineNumber);

			// save error prompt before resetting
			UINT nPrompt = g_nIDErrPrompt;

			// reset globals
			g_nIDErrPrompt   = 0;
			P_escapecode     = 0;
			g_nErrLineNumber = 0;

			if (nPrompt == IDS_ERR_INFINITE_LOOP)
			{
				// force current cell to be visible
				CEditGrid* pGrid = (CEditGrid*)PrepareEditGridCtrl(pDX, nIDC);
				pGrid->GetCellTop();

				// let user see grid while msgbox is modal
				ASSERT(pGrid->GetHighLight() == flexHighlightWithFocus);
				pGrid->SetHighLight(flexHighlightAlways);
				pGrid->SetRedraw(TRUE);

				if (bRenumbering)
				{
					// use alternative when renumbering
					if (::AfxMessageBox(_T("Warning: Possible infinite loop detected.  Do you want to continue renumbering?"), MB_ICONEXCLAMATION | MB_YESNO) != IDYES)
					{
						// reset grid
						pGrid->SetHighLight(flexHighlightWithFocus);
						pDX->Fail(); // throws exception
					}
					// reset grid
					pGrid->SetHighLight(flexHighlightWithFocus);
				}
				else
				{
					if (::AfxMessageBox(IDS_ERR_INFINITE_LOOP, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
					{
						// reset grid
						pGrid->SetHighLight(flexHighlightWithFocus);
						pDX->Fail(); // throws exception
					}
					// reset grid
					pGrid->SetHighLight(flexHighlightWithFocus);
				}
			}
			else
			{
				DDX_GridFail(pDX, nPrompt, IDR_MAINFRAME);
			}
		}
		else
		{
			// verify error state
			ASSERT(P_escapecode     == 0);
			ASSERT(g_nErrLineNumber == 0);
			ASSERT(g_nIDErrPrompt   == 0);
		}
	}
}

BOOL CBasicObj::Renumber(std::list<basic_command>& r_listCommands)
{
	CString str = _T(";");
	std::list<basic_command>::iterator commandIter = r_listCommands.begin();
	CString strLine;
	for (; commandIter != r_listCommands.end(); ++commandIter)
	{
		strLine.Format(_T("%ld %s;"),
			(*commandIter).nLine,
			(*commandIter).strCommand
			);
		str += strLine;
	}

	// set default exit code (assume basic_compile_1 failure)
	DWORD dwExitCode = BASIC_ERRORS;

	// compile commands and if no errors run
	//
	struct rate command = {0};
	command.commands = str.GetBuffer(str.GetLength() + 4);

	// make enough space for inbuf
	int max_line_save = max_line;
	max_line = max(max_line, str.GetLength() + 4);
	if (basic_compile_1(command.commands, &command.linebase, &command.varbase, &command.loopbase, TRUE) == 0)
	{
		// restore
		max_line = max_line_save;

		// verify state of globals after successful compile
		ASSERT(P_escapecode == 0);
		ASSERT(g_nIDErrPrompt == 0);
		ASSERT(g_nErrLineNumber == 0);

		//{{ RUN THREAD
		// create event to notify thread to stop and return
		CEvent eventInfiniteLoop;
		eventInfiniteLoop.ResetEvent();

		// init thread param
		struct XThreadParam basicCheck;
		basicCheck.szCommand = "renum";
		basicCheck.rate_ptr = &command;
		basicCheck.hInfiniteLoop = eventInfiniteLoop.m_hObject;

		// create suspended thread in order to set m_bAutoDelete
		CWinThread* pThread =
			::AfxBeginThread(ThreadProc, &basicCheck, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		pThread->m_bAutoDelete = FALSE;

		// start thread
		pThread->ResumeThread();

		// wait 1.1 seconds for thread to finish
		DWORD dwWait = ::WaitForSingleObject(pThread->m_hThread, 1100);
		if (dwWait == WAIT_TIMEOUT)
		{
			// signal thread to return
			TRACE("Signaling thread\n");
			VERIFY(eventInfiniteLoop.SetEvent());

			// wait a maximum of 2 seconds for thread to finish
			for (int i = 0; i < 20; ++i)
			{
				ASSERT(pThread);
				dwWait = ::WaitForSingleObject(pThread->m_hThread, 100);
				if (dwWait != WAIT_TIMEOUT)
				{
					TRACE("Thread has returned\n");
					break;
				}
			}
		}

		// get exit status of thread (return value of BasicCheckProc())
		dwExitCode = BASIC_OK;
		VERIFY(::GetExitCodeThread(pThread->m_hThread, &dwExitCode));

		if (dwExitCode == STILL_ACTIVE)
		{
			ASSERT(dwWait == WAIT_TIMEOUT);
			::AfxMessageBox(_T("An unrecoverable error has occured.  The program will now exit"), MB_ICONSTOP);
			::TerminateThread(pThread->m_hThread, 1);
			basic_free(command.linebase, command.varbase, command.loopbase);
			delete pThread;
			::PostQuitMessage(1);
			return FALSE;	// failure
		}

		// destroy thread
		delete pThread;
		//}} RUN THREAD

		if (dwExitCode == BASIC_OK)
		{
			r_listCommands.clear();
			basic_command basCommand;
			struct linerec *l = (struct linerec *)command.linebase;
			while (l != NULL)
			{
				basCommand.nLine = l->num;
				basCommand.strCommand = ListTokens(l->txt);
				r_listCommands.push_back(basCommand);
				l = l->next;
			}

			// clean up BASIC
			basic_free(command.linebase, command.varbase, command.loopbase);
			return TRUE; // success
		}
		else
		{
			ASSERT(FALSE);
			::AfxMessageBox("BASIC failed to compile", MB_ICONSTOP);
		}
	}
	else
	{
		// restore
		max_line = max_line_save;

		ASSERT(FALSE);
		::AfxMessageBox("BASIC failed to compile", MB_ICONSTOP);
	}

	return FALSE; // failure
}

CString CBasicObj::ListTokens(void* pVoid)
{
	struct tokenrec *buf = (struct tokenrec *)pVoid;

	CString strReturn(_T(""));
	CString strFprintf(_T(""));

	bool ltr;
	// Char STR1[256];

	ltr = false;
	
	while (buf != NULL)
	{
		if ((buf->kind >= (long)toknot && buf->kind <= (long)tokrenum) || 
			buf->kind == (long)toknum || buf->kind == (long)tokvar ||
			buf->kind >= (long)toktc)
		{
			if (ltr)
			{
				strReturn += _T(' '); // putc(' ', f);
			}
			ltr = (buf->kind != toknot);
			
		}
		else
		{
			ltr = false;
		}
		switch (buf->kind)
		{
		case tokvar:
			strReturn += buf->UU.vp->name; // fputs(buf->UU.vp->name, f);
			break;
		
		case toknum:
			strReturn += buf->sz_num; // fputs(buf->sz_num, f);
			////fputs(numtostr(STR1, buf->UU.num), f);
			break;

		case tokstr:
			strFprintf.Format("\"%s\"", buf->UU.sp); // fprintf(f, "\"%s\"", buf->UU.sp);
			strReturn += strFprintf;
			break;

		case toksnerr:
			strFprintf.Format("{%c}", buf->UU.snch); // fprintf(f, "{%c}", buf->UU.snch);
			strReturn += strFprintf;
			break;
		
		case tokplus:
			strReturn += '+'; // putc('+', f);
			break;

		case tokminus:
			strReturn += '-'; // putc('-', f);
			break;
			
		case toktimes:
			strReturn += '*'; // putc('*', f);
			break;
			
		case tokdiv:
			strReturn += '/'; // putc('/', f);
			break;
			
		case tokup:
			strReturn += '^'; // putc('^', f);
			break;
			
		case toklp:
			strReturn += '('; // putc('(', f);
			break;
			
		case tokrp:
			strReturn += ')'; // putc(')', f);
			break;
			
		case tokcomma:
			///strReturn += ','; // putc(',', f);
			strReturn += ", ";
			break;
			
		case toksemi:
			strReturn += ';'; // putc(';', f);
			break;
			
		case tokcolon:
			strReturn += " : "; // fprintf(f, " : ");
			break;
			
		case tokeq:
			strReturn += " = "; // fprintf(f, " = ");
			break;
			
		case toklt:
			strReturn += " < "; // fprintf(f, " < ");
			break;
			
		case tokgt:
			strReturn += " > "; // fprintf(f, " > ");
			break;
			
		case tokle:
			strReturn += " <= "; // fprintf(f, " <= ");
			break;
			
		case tokge:
			strReturn += " >= "; // fprintf(f, " >= ");
			break;
			
		case tokne:
			strReturn += " <> "; // fprintf(f, " <> ");
			break;
			
		case tokand:
			strReturn += " AND "; // fprintf(f, " AND ");
			break;
			
		case tokor:
			strReturn += " OR "; // fprintf(f, " OR ");
			break;
			
		case tokxor:
			strReturn += " XOR "; // fprintf(f, " XOR ");
			break;
			
		case tokmod:
			strReturn += " MOD "; // fprintf(f, " MOD ");
			break;
			
		case toknot:
			strReturn += "NOT "; // fprintf(f, "NOT ");
			break;
			
		case toksqr:
			strReturn += "SQR"; // fprintf(f, "SQR");
			break;
			
		case toksqrt:
			strReturn += "SQRT"; // fprintf(f, "SQRT");
			break;
			
		case toksin:
			strReturn += "SIN"; // fprintf(f, "SIN");
			break;
			
		case tokcos:
			strReturn += "COS"; // fprintf(f, "COS");
			break;
			
		case toktan:
			strReturn += "TAN"; // fprintf(f, "TAN");
			break;
			
		case tokarctan:
			strReturn += "ARCTAN"; // fprintf(f, "ARCTAN");
			break;
			
		case toklog:
			strReturn += "LOG"; // fprintf(f, "LOG");
			break;
			
		case tokexp:
			strReturn += "EXP"; // fprintf(f, "EXP");
			break;
			
		case tokabs:
			strReturn += "ABS"; // fprintf(f, "ABS");
			break;
			
		case toksgn:
			strReturn += "SGN"; // fprintf(f, "SGN");
			break;
			
		case tokstr_:
			strReturn += "STR$"; // fprintf(f, "STR$");
			break;
			
		case tokval:
			strReturn += "STR$"; // fprintf(f, "VAL");
			break;
			
		case tokchr_:
			strReturn += "CHR$"; // fprintf(f, "CHR$");
			break;
			
		case tokasc:
			strReturn += "ASC"; // fprintf(f, "ASC");
			break;
			
		case toklen:
			strReturn += "LEN"; // fprintf(f, "LEN");
			break;
			
		case tokmid_:
			strReturn += "MID$"; // fprintf(f, "MID$");
			break;
			
		case tokpeek:
			strReturn += "PEEK"; // fprintf(f, "PEEK");
			break;
			
		case tokrem:
			strFprintf.Format("REM%s", buf->UU.sp); // fprintf(f, "REM%s", buf->UU.sp);
			strReturn += strFprintf;
			break;
			
		case toklet:
			strReturn += "LET"; // fprintf(f, "LET");
			break;
			
		case tokprint:
			strReturn += "PRINT"; // fprintf(f, "PRINT");
			break;
			
		case tokinput:
			strReturn += "INPUT"; // fprintf(f, "INPUT");
			break;
			
		case tokgoto:
			strReturn += "GOTO"; // fprintf(f, "GOTO");
			break;
			
		case tokif:
			strReturn += "IF"; // fprintf(f, "IF");
			break;
			
		case tokend:
			strReturn += "END"; // fprintf(f, "END");
			break;
			
		case tokstop:
			strReturn += "STOP"; // fprintf(f, "STOP");
			break;
			
		case tokfor:
			strReturn += "FOR"; // fprintf(f, "FOR");
			break;
			
		case toknext:
			strReturn += "NEXT"; // fprintf(f, "NEXT");
			break;
			
		case tokwhile:
			strReturn += "WHILE"; // fprintf(f, "WHILE");
			break;
			
		case tokwend:
			strReturn += "WEND"; // fprintf(f, "WEND");
			break;
			
		case tokgosub:
			strReturn += "GOSUB"; // fprintf(f, "GOSUB");
			break;
			
		case tokreturn:
			strReturn += "RETURN"; // fprintf(f, "RETURN");
			break;
			
		case tokread:
			strReturn += "READ"; // fprintf(f, "READ");
			break;
			
		case tokdata:
			strReturn += "DATA"; // fprintf(f, "DATA");
			break;
			
		case tokrestore:
			strReturn += "RESTORE"; // fprintf(f, "RESTORE");
			break;
			
		case tokgotoxy:
			strReturn += "GOTOXY"; // fprintf(f, "GOTOXY");
			break;
			
		case tokon:
			strReturn += "ON"; // fprintf(f, "ON");
			break;
			
		case tokdim:
			strReturn += "DIM"; // fprintf(f, "DIM");
			break;
			
		case tokpoke:
			strReturn += "POKE"; // fprintf(f, "POKE");
			break;
			
		case toklist:
			strReturn += "LIST"; // fprintf(f, "LIST");
			break;
			
		case tokrun:
			strReturn += "RUN"; // fprintf(f, "RUN");
			break;
			
		case toknew:
			strReturn += "NEW"; // fprintf(f, "NEW");
			break;
			
		case tokload:
			strReturn += "LOAD"; // fprintf(f, "LOAD");
			break;
			
		case tokmerge:
			strReturn += "MERGE"; // fprintf(f, "MERGE");
			break;
			
		case toksave:
			strReturn += "SAVE"; // fprintf(f, "SAVE");
			break;
			
		case tokbye:
			strReturn += "BYE"; // fprintf(f, "BYE");
			break;
			
		case tokdel:
			strReturn += "DEL"; // fprintf(f, "DEL");
			break;
			
		case tokrenum:
			strReturn += "RENUM"; // fprintf(f, "RENUM");
			break;
			
		case tokthen:
			strReturn += " THEN "; // fprintf(f, " THEN ");
			break;
			
		case tokelse:
			strReturn += " ELSE "; // fprintf(f, " ELSE ");
			break;
			
		case tokto:
			strReturn += " TO "; // fprintf(f, " TO ");
			break;
			
		case tokstep:
			strReturn += " STEP "; // fprintf(f, " STEP ");
			break;
			
		case toktc:
			strReturn += "TC"; // fprintf(f, "TC");
			break;
			
		case tokm0:
			strReturn += "M0"; // fprintf(f, "M0");
			break;
			
		case tokm:
			strReturn += "M"; // fprintf(f, "M");
			break;
			
		case tokparm:
			strReturn += "PARM"; // fprintf(f, "PARM");
			break;
			
		case tokact:
			strReturn += "ACT"; // fprintf(f, "ACT");
			break;
			
		case tokmol:
			strReturn += "MOL"; // fprintf(f, "MOL");
			break;
			
		case tokla:
			strReturn += "LA"; // fprintf(f, "LA");
			break;
			
		case toklm:
			strReturn += "LM"; // fprintf(f, "LM");
			break;
			
		case toksr:
			strReturn += "SR"; // fprintf(f, "SR");
			break;
			
		case toksi:
			strReturn += "SI"; // fprintf(f, "SI");
			break;
			
		case toktot:
			strReturn += "TOT"; // fprintf(f, "TOT");
			break;
			
		case toktk:
			strReturn += "TK"; // fprintf(f, "TK");
			break;
			
		case toktime:
			strReturn += "TIME"; // fprintf(f, "TIME");
			break;
			
		case toklog10:
			strReturn += "LOG10"; // fprintf(f, "LOG10");
			break;
			
		case toksim_time:
			strReturn += "SIM_TIME"; // fprintf(f, "SIM_TIME");
			break;
			
		case tokequi:
			strReturn += "EQUI"; // fprintf(f, "EQUI");
			break;
			
		case tokgas:
			strReturn += "GAS"; // fprintf(f, "GAS");
			break;
			
		case tokpunch:
			strReturn += "PUNCH"; // fprintf(f, "PUNCH");
			break;
			
		case tokkin:
			strReturn += "KIN"; // fprintf(f, "KIN");
			break;
			
		case toks_s:
			strReturn += "S_S"; // fprintf(f, "S_S");
			break;
			
		case tokmu:
			strReturn += "MU"; // fprintf(f, "MU");
			break;
			
		case tokalk:
			strReturn += "ALK"; // fprintf(f, "ALK");
			break;
//{{
		case toklk_species:
			strReturn += "LK_SPECIES";
			break;
			
		case toklk_named:
			strReturn += "LK_NAMED";
			break;
			
		case toklk_phase:
			strReturn += "LK_PHASE";
			break;
			
		case toksum_species:
			strReturn += "SUM_SPECIES";
			break;
			
		case toksum_gas:
			strReturn += "SUM_GAS";
			break;
			
		case toksum_s_s:
			strReturn += "SUM_s_s";
			break;
			
		case tokcalc_value:
			strReturn += "CALC_VALUE";
			break;
//}}
		case tokrxn:
			strReturn += "RXN"; // fprintf(f, "RXN");
			break;
			
		case tokdist:
			strReturn += "DIST"; // fprintf(f, "DIST");
			break;
			
		case tokmisc1:
			strReturn += "MISC1"; // fprintf(f, "MISC1");
			break;
			
		case tokmisc2:
			strReturn += "MISC2"; // fprintf(f, "MISC2");
			break;
			
		case tokedl:
			strReturn += "EDL"; // fprintf(f, "EDL");
			break;
			
		case tokstep_no:
			strReturn += "STEP_NO"; // fprintf(f, "STEP_NO");
			break;
			
		case toksurf:
			strReturn += "SURF";
			break;
			
		case toksim_no:
			strReturn += "SIM_NO"; // fprintf(f, "SIM_NO");
			break;
			
		case toktotal_time:
			strReturn += "TOTAL_TIME"; // fprintf(f, "TOTAL_TIME");
			break;
			
		case tokput:
			strReturn += "PUT"; // fprintf(f, "PUT");
			break;
			
		case tokget:
			strReturn += "GET"; // fprintf(f, "GET");
			break;
			
		case tokcharge_balance:
			strReturn += "CHARGE_BALANCE"; // fprintf(f, "CHARGE_BALANCE");
			break;
			
		case tokpercent_error:
			strReturn += "PERCENT_ERROR"; // fprintf(f, "PERCENT_ERROR");
			break;
			
#ifdef PHREEQ98
		case tokgraph_x:
			strReturn += "GRAPH_X"; // fprintf(f, "GRAPH_X");
			break;
			
		case tokgraph_y:
			strReturn += "GRAPH_Y"; // fprintf(f, "GRAPH_Y");
			break;
			
		case tokgraph_sy:
			strReturn += "GRAPH_SY"; // fprintf(f, "GRAPH_SY");
			break;
#endif
			
		case tokcell_no:
			strReturn += "CELL_NO"; // fprintf(f, "CELL_NO");
			break;
			
		case tokexists:
			strReturn += "EXISTS"; // fprintf(f, "EXISTS");
			break;

//{{ 2.9 added functions
		case tokdescription:
			strReturn += "DESCRIPTION"; // output_msg(OUTPUT_BASIC, "DESCRIPTION");
			break;

		case toksys:
			strReturn += "SYS"; // output_msg(OUTPUT_BASIC, "SYS");
			break;

		case tokinstr:
			strReturn += "INSTR"; // output_msg(OUTPUT_BASIC, "INSTR");
			break;

		case tokltrim:
			strReturn += "LTRIM"; // output_msg(OUTPUT_BASIC, "LTRIM");
			break;

		case tokrtrim:
			strReturn += "RTRIM"; // output_msg(OUTPUT_BASIC, "RTRIM");
			break;

		case toktrim:
			strReturn += "TRIM"; // output_msg(OUTPUT_BASIC, "TRIM");
			break;

		case tokpad:
			strReturn += "PAD"; // output_msg(OUTPUT_BASIC, "PAD");
			break;

		case tokchange_por:
			strReturn += "CHANGE_POR";
			break;

		case tokget_por:
			strReturn += "GET_POR";
			break;

		case tokosmotic:
			strReturn += "OSMOTIC";
			break;

		case tokchange_surf:
			strReturn += "CHANGE_SURF";
			break;

//}} 2.9 added functions
			
    }
    buf = buf->next;
  }
  return strReturn;
}

void CBasicObj::DDV_ContainsSave(CDataExchange* pDX, int nIDC, std::list<basic_command>& r_listCommands)
{
	UNUSED(nIDC);

	CString str = _T(";");
	std::list<basic_command>::iterator commandIter = r_listCommands.begin();
	CString strLine;
	for (; commandIter != r_listCommands.end(); ++commandIter)
	{
		strLine.Format(_T("%ld %s;"),
			(*commandIter).nLine,
			(*commandIter).strCommand
			);
		str += strLine;
	}


	// compile commands
	struct rate command = {0};
	command.commands = str.GetBuffer(str.GetLength() + 4);
	if (basic_compile_1(command.commands, &command.linebase, &command.varbase, &command.loopbase, TRUE) == 0)
	{
		// verify state of globals after successful compile
		ASSERT(P_escapecode == 0);
		ASSERT(g_nIDErrPrompt == 0);
		ASSERT(g_nErrLineNumber == 0);

		bool bSaveFound = false;

		// iterate over each line
		struct linerec *l = (struct linerec *)command.linebase;
		while (l != NULL && !bSaveFound)
		{
			// iterate over each token
			struct tokenrec *buf = l->txt;
			while(buf != NULL)
			{
				if (buf->kind == toksave)
				{
					bSaveFound = true;
					break;
				}
				buf = buf->next;
			}
			l = l->next;
		}

		// clean up BASIC
		basic_free(command.linebase, command.varbase, command.loopbase);

		if (!bSaveFound)
		{
			DDX_GridFail(pDX, _T("SAVE statement not found.  Each rate must contain \"SAVE expression\" where the\n"
				"value of expression is the moles of reaction that occur during time subinterval TIME."));
		}
	}
	else
	{
		ASSERT(FALSE);
		::AfxMessageBox("BASIC failed to compile", MB_ICONSTOP);
		// clean up BASIC
		basic_free(command.linebase, command.varbase, command.loopbase);
	}
}

void CBasicObj::SetErrorCell(CDataExchange* pDX, int nIDC, long nLine)
{
	// place selection on first line 
	CEditGrid* pGrid = (CEditGrid*)PrepareEditGridCtrl(pDX, nIDC);
	ASSERT_VALID(pGrid);
	pGrid->SetRow(0);
	pGrid->SetCol(1);

	if (nLine != 0)
	{
		// find row error occured on (assume line numbers are in column 0)
		CString strNum;
		strNum.Format(_T("%ld"), nLine);
		for (long nRow = pGrid->GetFixedRows(); nRow < pGrid->GetRows(); ++nRow)
		{
			if (strNum.Compare(pGrid->GetTextMatrix(nRow, 0)) == 0)
			{
				pGrid->SetRow(nRow);
				break;
			}
		}
	}
}
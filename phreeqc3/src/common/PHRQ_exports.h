#ifndef INC_PHRQ_EXPORTS_H
#define INC_PHRQ_EXPORTS_H

#if defined(WIN32)
#  if defined(PHREEQCI_GUI)
#    if _MSC_VER == 1400   // (Visual Studio 2005 version 8.0)
#      ifndef WINVER
#        define WINVER 0x0400
#      endif
#    endif
#    if _MSC_VER == 1500   // (Visual Studio 2008 version 9.0)
#      ifndef WINVER
#        define WINVER 0x0600
#      endif
#    endif
#    include <afx.h>
#  endif
#  include <windows.h>
#endif

#if defined(_WINDLL) && defined(IPhreeqc_EXPORTS)
#  define IPQ_DLL_EXPORT __declspec(dllexport)
#else
#  define IPQ_DLL_EXPORT
#endif

#endif // INC_PHRQ_EXPORTS_H

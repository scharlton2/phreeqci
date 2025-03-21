if (MSVC_VERSION GREATER_EQUAL 1500)
  cmake_minimum_required(VERSION 3.16) # target_precompile_headers VS_DPI_AWARE
# cmake_minimum_required(VERSION 3.12) # COMMON_LANGUAGE_RUNTIME (VS 2010 and above)
else()
  # VS2005
  cmake_minimum_required(VERSION 3.11)
endif()

project(phreeqci VERSION 0.1.0 LANGUAGES C CXX)

# copy necessary files into build directory
configure_file(phreeqc3/database/phreeqc.dat database/phreeqc.dat COPYONLY)
configure_file(phreeqci.eng phreeqci.eng COPYONLY)

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "Phreeqc Interactive must be compiled as 32-bit (Use -A Win32)")
endif()

# Find the MFC libraries
find_package(MFC REQUIRED)

# Enable Microsoft Foundation Classes (MFC)
# 1 -> static MFC library
# 2 -> shared MFC library
set(CMAKE_MFC_FLAG 2)

# Check for ATL
include(CheckIncludeFileCXX)
check_include_file_cxx("atlbase.h" HAVE_ATL)

if(HAVE_ATL)
  message(STATUS "ATL is installed.")
else()
  message(FATAL_ERROR "ATL is not installed.")
endif()

if(MSVC_VERSION VERSION_LESS 1600)
  message(WARNING "CMake support for ZedGraph (CLR) requires Visual Studio 2010 or higher")
else()
  # Enable Zedgraph by default
  option(ENABLE_ZEDGRAPH "Enable ZedGraph (Requires CLR)" ON)
endif()

add_executable(phreeqci)

# Build an executable with a WinMain entry point on windows.
set_target_properties(phreeqci PROPERTIES WIN32_EXECUTABLE TRUE)

set(RESOURCE_FILES
  res/bitmap1.bmp
  res/checks.bmp
  res/ErrorItems.bmp
  res/ico00001.ico
  res/ico00002.ico
  res/icon1.ico
  res/idr_db.ico
  res/input.ico
  res/InputItems.bmp
  res/litebulb.bmp
  res/litebulb2.bmp
  res/output.ico
  res/OutputItems.bmp
  res/phreeqci.exe.manifest
  res/phreeqci2.ico
  res/phreeqci2.rc2
  res/resize_column.cur
  res/resize_row.cur
  res/RichDocIn.ico
  res/Splsh16.bmp
  res/TabItems.bmp
  res/Toolbar.bmp
  res/toolbar1.bmp
  res/ToolbarForward.bmp
  res/ToolbarInitCond.bmp
  res/ToolbarPrint.bmp
  res/ToolbarRun.bmp
)

target_sources(phreeqci
  PRIVATE
    AccelPropertySheet.cpp
    AccelPropertySheet.h
    BasicDesc2.cpp
    BasicDesc2.h
    BasicEditGrid.cpp
    BasicEditGrid.h
    BasicObj.cpp
    BasicObj.h
    CalculateValues.cpp
    CalculateValuesPg1.h
    CalculateValues.h
    CalculateValuesPg1.cpp
    CEPExchangePg1.cpp
    CEPExchangePg1.h
    CheckListCtrl.cpp
    CheckListCtrl.h
    ChildFrame.cpp
    ChildFrame.h
    CKPEquilibrium_PhasesPg1.cpp
    CKPEquilibrium_PhasesPg1.h
    CKPGas_PhasePg1.cpp
    CKPGas_PhasePg1.h
    CKPKineticsPg1.cpp
    CKPKineticsPg1.h
    CKPMixPg1.cpp
    CKPMixPg1.h
    CKPReaction_TemperaturePg1.cpp
    CKPReaction_TemperaturePg1.h
    CKPReactionPg1.cpp
    CKPReactionPg1.h
    CKPSolid_SolutionsPg1.cpp
    CKPSolid_SolutionsPg1.h
    CKSEquilibrium_Phases.cpp
    CKSEquilibrium_Phases.h
    CKSExchange.cpp
    CKSExchange.h
    CKSGas_Phase.cpp
    CKSGas_Phase.h
    CKSInverse.cpp
    CKSInverse.h
    CKSMix.cpp
    CKSMix.h
    CKSReaction_Temperature.cpp
    CKSReaction_Temperature.h
    CKSReaction.cpp
    CKSReaction.h
    CKSSolid_Solutions.cpp
    CKSSolid_Solutions.h
    CKSSolution.cpp
    CKSSolution.h
    # Class Layout.txt
    ClipListBox.cpp
    ClipListBox.h
    ClipTreeCtrl.cpp
    ClipTreeCtrl.h
    CMakeLists.txt
    # Command Routing.txt
    CommonKeywordPage.cpp
    CommonKeywordPage.h
    CommonKeywordSheet.cpp
    CommonKeywordSheet.h
    CStringLineParser.cpp
    CStringLineParser.h
    CustomizePage1.cpp
    CustomizePage1.h
    CustomizeSheet.cpp
    CustomizeSheet.h
    Database.cpp
    Database.h
    DBObject.cpp
    DBObject.h
    DDX_DDV.cpp
    DDX_DDV.h
    DefinedRanges.cpp
    DefinedRanges.h
    DelayRedraw.h
    DelayUpdate.cpp
    DelayUpdate.h
    Delete.cpp
    Delete.h
    DeletePg1.cpp
    DeletePg1.h
    Dump.cpp
    Dump.h
    DumpPg1.cpp
    DumpPg1.h
    # EditGrid Combo Handling.txt
    EditGrid.cpp
    EditGrid.h
    EditReadOnly.cpp
    EditReadOnly.h
    # EqnChecker.cpp
    # EqnChecker.h
    EqnChecker2.cpp
    EqnChecker2.h
    # ErrorChecker2.cpp
    # ErrorChecker2.h
    ErrorChecker3.cpp
    ErrorChecker3.h
    FixedTabCtrl.cpp
    FixedTabCtrl.h
    font.cpp
    font.h
    GridListDoc.cpp
    GridListDoc.h
    Include.cpp
    Include.h
    IncludePg1.cpp
    IncludePg1.h
    InputTreeOleDropTarget.cpp
    InputTreeOleDropTarget.h
    # jenkins-dist-build.ps1
    Keyword.cpp
    Keyword.h
    KeywordPage.cpp
    KeywordPage.h
    KeywordPageListItems.cpp
    KeywordPageListItems.h
    KeywordParser.cpp
    KeywordParser.h
    KeywordSheet.cpp
    KeywordSheet.h
    KPAdvectionPg1.cpp
    KPAdvectionPg1.h
    KPCopyPg1.cpp
    KPCopyPg1.h
    KPExchangeMasterSpeciesPg1.cpp
    KPExchangeMasterSpeciesPg1.h
    KPExchangeSpeciesPg1.cpp
    KPExchangeSpeciesPg1.h
    KPIncrementPg1.cpp
    KPIncrementPg1.h
    KPInversePg1.cpp
    KPInversePg1.h
    KPKnobsPg1.cpp
    KPKnobsPg1.h
    KPPhasesPg1.cpp
    KPPhasesPg1.h
    KPPitzerAlphas.cpp
    KPPitzerAlphas.h
    KPPitzerCA.cpp
    KPPitzerCA.h
    KPPitzerCAN.cpp
    KPPitzerCAN.h
    KPPitzerGeneral.cpp
    KPPitzerGeneral.h
    KPPitzerNNV.cpp
    KPPitzerNNV.h
    KPPitzerNXX.cpp
    KPPitzerNXX.h
    KPPitzerXN.cpp
    KPPitzerXN.h
    KPPitzerXX.cpp
    KPPitzerXX.h
    KPPitzerXXY.cpp
    KPPitzerXXY.h
    KPPrintPg1.cpp
    KPPrintPg1.h
    KPRatesPg1.cpp
    KPRatesPg1.h
    KPSavePg1.cpp
    KPSavePg1.h
    KPSelectedOutputPg1.cpp
    KPSelectedOutputPg1.h
    KPSelectedOutputPg8.cpp
    KPSelectedOutputPg8.h
    KPSelectedOutputPg9.cpp
    KPSelectedOutputPg9.h
    KPSITEpsilon.cpp
    KPSITEpsilon.h
    KPSolution_SpreadPg1.cpp
    KPSolution_SpreadPg1.h
    KPSolutionMasterSpeciesPg1.cpp
    KPSolutionMasterSpeciesPg1.h
    KPSolutionPg1.cpp
    KPSolutionPg1.h
    KPSolutionSpeciesPg1.cpp
    KPSolutionSpeciesPg1.h
    KPSurfaceMasterSpeciesPg1.cpp
    KPSurfaceMasterSpeciesPg1.h
    KPSurfaceSpeciesPg1.cpp
    KPSurfaceSpeciesPg1.h
    KPTitlePg1.cpp
    KPTitlePg1.h
    KPTransportPg1.cpp
    KPTransportPg1.h
    KPTransportPg6.cpp
    KPTransportPg6.h
    KPUsePg1.cpp
    KPUsePg1.h
    KPUserPrintPg1.cpp
    KPUserPrintPg1.h
    KPUserPunchPg1.cpp
    KPUserPunchPg1.h
    KSExchangeMasterSpecies.cpp
    KSExchangeMasterSpecies.h
    KSExchangeSpecies.cpp
    KSExchangeSpecies.h
    KSIncrement.cpp
    KSIncrement.h
    KSKinetics.cpp
    KSKinetics.h
    KSKnobs.cpp
    KSKnobs.h
    KSPhases.cpp
    KSPhases.h
    KSPitzer.cpp
    KSPitzer.h
    KSPrint.cpp
    KSPrint.h
    KSSelectedOutput.cpp
    KSSelectedOutput.h
    KSSIT.cpp
    KSSIT.h
    KSSolutionMasterSpecies.cpp
    KSSolutionMasterSpecies.h
    KSSolutionSpecies.cpp
    KSSolutionSpecies.h
    KSSurfaceMasterSpecies.cpp
    KSSurfaceMasterSpecies.h
    KSSurfaceSpecies.cpp
    KSSurfaceSpecies.h
    KSTitle.cpp
    KSTitle.h
    LineParser.cpp
    LineParser.h
    LocalException.cpp
    LocalException.h
    LockWindowUpdate.h
    LogicalLineParser.cpp
    LogicalLineParser.h
    MainFrame.cpp
    MainFrame.h
    # MakeHelp.bat
    mshflexgrid.cpp
    mshflexgrid.h
    MultiDocTemplateIn.cpp
    MultiDocTemplateIn.h
    MultiDocTemplateOut.cpp
    MultiDocTemplateOut.h
    MyPropertyPage1.cpp
    MyPropertyPage1.h
    MyPropertySheet.cpp
    MyPropertySheet.h
    MyRecentFileList.cpp
    MyRecentFileList.h
    NodeObject.cpp
    NodeObject.h
    # Notes.txt
    OCKSAdvection.cpp
    OCKSAdvection.h
    OCKSCopy.cpp
    OCKSCopy.h
    OCKSRates.cpp
    OCKSRates.h
    OCKSSave.cpp
    OCKSSave.h
    OCKSSolution_Spread.cpp
    OCKSSolution_Spread.h
    OCKSTransport.cpp
    OCKSTransport.h
    OCKSUse.cpp
    OCKSUse.h
    OCKSUserPrint.cpp
    OCKSUserPrint.h
    OCKSUserPunch.cpp
    OCKSUserPunch.h
    OleCtlKeywordSheet.cpp
    OleCtlKeywordSheet.h
    phreeqci_files.h
    # phreeqci_gui.c
    # phreeqci_gui.h
    PhreeqcI.cpp
    # phreeqci.eng
    PhreeqcI.h
    # phreeqci2.aps
    phreeqci2.cpp
    # phreeqci2.dsp
    # phreeqci2.dsw
    phreeqci2.h
    phreeqci2.rc
    # phreeqci2.reg
    # phreeqci2.sln
    # phreeqci2.vcproj
    # phreeqci2.vcproj.GS.charlton.user
    # phreeqci2.vcxproj
    # phreeqci2.vcxproj.filters
    # phreeqci2.vcxproj.user
    PhreeqciDocManager.cpp
    PhreeqciDocManager.h
    picture.cpp
    picture.h
    PitzParam.cpp
    PitzParam.h
    Reaction_Pressure.cpp
    Reaction_Pressure.h
    Reaction_PressurePg1.cpp
    Reaction_PressurePg1.h
    # ReadMe.txt
    recordset.cpp
    recordset.h
    # resource.fd
    resource.h
    # resource.hm
    RichDocDB.cpp
    RichDocDB.h
    RichDocIn.cpp
    RichDocIn.h
    RichDocInPages.cpp
    RichDocInPages.h
    RichDocInSheet.cpp
    RichDocInSheet.h
    RichDocOut.cpp
    RichDocOut.h
    RichEditLineParser.cpp
    RichEditLineParser.h
    RichEditLineParserSafe.cpp
    RichEditLineParserSafe.h
    RichInCntrItem.cpp
    RichInCntrItem.h
    RichKeywordParser.cpp
    RichKeywordParser.h
    RichLogicalLineParser.cpp
    RichLogicalLineParser.h
    RichOutCntrItem.cpp
    RichOutCntrItem.h
    RichViewDB.cpp
    RichViewDB.h
    RichViewIn.cpp
    RichViewIn.h
    RichViewOut.cpp
    RichViewOut.h
    RunCells.cpp
    RunCells.h
    RunCellsPg1.cpp
    RunCellsPg1.h
    RunDlg.cpp
    RunDlg.h
    RunDlg2.cpp
    RunDlg2.h
    SaveCurrentDirectory.cpp
    SaveCurrentDirectory.h
    SeException.cpp
    SeException.h
    SelectedOutputPg10.cpp
    SelectedOutputPg10.h
    SelectElementsDlg.cpp
    SelectElementsDlg.h
    Splash.cpp
    Splash.h
    srcdbpg.cpp
    srcdbpg.h
    SRCDBPGCH.cpp
    SRCDBPGCH.h
    # StdAfx.cpp
    StdAfx.h
    StdioFileLineParser.cpp
    StdioFileLineParser.h
    SurfacePage.cpp
    SurfacePage.h
    SurfaceSheet.cpp
    SurfaceSheet.h
    SuspendUndo.cpp
    SuspendUndo.h
    TipDlg.cpp
    TipDlg.h
    Tree.h
    Tree.inl
    TreeCtrlDB.cpp
    TreeCtrlDB.h
    TreeCtrlErr.cpp
    TreeCtrlErr.h
    TreeCtrlEx.cpp
    TreeCtrlIn.cpp
    TreeCtrlIn.h
    TreeCtrlKeyword.cpp
    TreeCtrlKeyword.h
    TreeCtrlNode.cpp
    TreeCtrlOut.cpp
    TreeCtrlOut.h
    TreeCtrlPfw.cpp
    TreeCtrlPfw.h
    UpdateObject.cpp
    UpdateObject.h
    # UpgradeLog.htm
    UserGraph.cpp
    UserGraph.h
    UserGraphPg1.cpp
    UserGraphPg1.h
    UserGraphPg2.cpp
    UserGraphPg2.h
    Util.cpp
    Util.h
    WorkspaceBar.cpp
    WorkspaceBar.h
    gridctrl/CellRange.h
    gridctrl/DropDownButton.cpp
    gridctrl/DropDownButton.h
    gridctrl/GridCtrl.cpp
    gridctrl/GridCtrl.h
    gridctrl/GridDropTarget.cpp
    gridctrl/GridDropTarget.h
    gridctrl/InPlaceEdit.cpp
    gridctrl/InPlaceEdit.h
    gridctrl/InPlaceList.cpp
    gridctrl/InPlaceList.h
    gridctrl/ListBoxDrop.cpp
    gridctrl/ListBoxDrop.h
    gridctrl/MemDC.h
    gridctrl/ModGridCtrl.cpp
    gridctrl/ModGridCtrl.h
    gridctrl/ModGridCtrlEx.cpp
    gridctrl/ModGridCtrlEx.h
    gridctrl/TitleTip.cpp
    gridctrl/TitleTip.h
    # Layout
    Layout/ETSLayout.cpp
    Layout/ETSLayout.h
    # phreeqc3/src
    phreeqc3/src/advection.cpp
    phreeqc3/src/basicsubs.cpp
    #phreeqc3/src/chart_icon.gif
    phreeqc3/src/ChartHandler.cpp
    phreeqc3/src/ChartHandler.h
    phreeqc3/src/ChartObject.cpp
    phreeqc3/src/ChartObject.h
    phreeqc3/src/cl1.cpp
    phreeqc3/src/class_main.cpp
    phreeqc3/src/CurveObject.cpp
    phreeqc3/src/CurveObject.h
    phreeqc3/src/cvdense.cpp
    phreeqc3/src/cvdense.h
    phreeqc3/src/cvode.cpp
    phreeqc3/src/cvode.h
    phreeqc3/src/cxxKinetics.cxx
    phreeqc3/src/cxxKinetics.h
    phreeqc3/src/cxxMix.cxx
    phreeqc3/src/cxxMix.h
    phreeqc3/src/dense.cpp
    phreeqc3/src/dense.h
    phreeqc3/src/Dictionary.cpp
    phreeqc3/src/Dictionary.h
    phreeqc3/src/dumper.cpp
    phreeqc3/src/dumper.h
    phreeqc3/src/Exchange.cxx
    phreeqc3/src/Exchange.h
    phreeqc3/src/ExchComp.cxx
    phreeqc3/src/ExchComp.h
    #{{
    # phreeqc3/src/Form1.2005.resX
    # phreeqc3/src/Form1.h
    # phreeqc3/src/Form1.resX
    #}}
    phreeqc3/src/GasComp.cxx
    phreeqc3/src/GasComp.h
    phreeqc3/src/gases.cpp
    phreeqc3/src/GasPhase.cxx
    phreeqc3/src/GasPhase.h
    phreeqc3/src/global_structures.h
    phreeqc3/src/input.cpp
    phreeqc3/src/integrate.cpp
    phreeqc3/src/inverse.cpp
    phreeqc3/src/ISolution.cxx
    phreeqc3/src/ISolution.h
    phreeqc3/src/ISolutionComp.cxx
    phreeqc3/src/ISolutionComp.h
    phreeqc3/src/isotopes.cpp
    phreeqc3/src/kinetics.cpp
    phreeqc3/src/KineticsComp.cxx
    phreeqc3/src/KineticsComp.h
    phreeqc3/src/mainsubs.cpp
    phreeqc3/src/model.cpp
    phreeqc3/src/NA.h
    phreeqc3/src/NameDouble.cxx
    phreeqc3/src/NameDouble.h
    phreeqc3/src/NumKeyword.cxx
    phreeqc3/src/NumKeyword.h
    phreeqc3/src/nvector_serial.cpp
    phreeqc3/src/nvector_serial.h
    phreeqc3/src/nvector.cpp
    phreeqc3/src/nvector.h
    phreeqc3/src/parse.cpp
    phreeqc3/src/PBasic.cpp
    phreeqc3/src/PBasic.h
    phreeqc3/src/phqalloc.cpp
    phreeqc3/src/phqalloc.h
    phreeqc3/src/Phreeqc.cpp
    phreeqc3/src/Phreeqc.h
    #phreeqc3/src/phreex.ico
    phreeqc3/src/PHRQ_io_output.cpp
    phreeqc3/src/pitzer_structures.cpp
    phreeqc3/src/pitzer.cpp
    phreeqc3/src/PPassemblage.cxx
    phreeqc3/src/PPassemblage.h
    phreeqc3/src/PPassemblageComp.cxx
    phreeqc3/src/PPassemblageComp.h
    phreeqc3/src/prep.cpp
    phreeqc3/src/Pressure.cxx
    phreeqc3/src/Pressure.h
    phreeqc3/src/print.cpp
    phreeqc3/src/Reaction.cxx
    phreeqc3/src/Reaction.h
    phreeqc3/src/read.cpp
    phreeqc3/src/ReadClass.cxx
    phreeqc3/src/readtr.cpp
    phreeqc3/src/runner.cpp
    phreeqc3/src/runner.h
    phreeqc3/src/SelectedOutput.cpp
    phreeqc3/src/SelectedOutput.h
    phreeqc3/src/Serializer.cxx
    phreeqc3/src/Serializer.h
    phreeqc3/src/sit.cpp
    phreeqc3/src/smalldense.cpp
    phreeqc3/src/smalldense.h
    phreeqc3/src/Solution.cxx
    phreeqc3/src/Solution.h
    phreeqc3/src/SolutionIsotope.cxx
    phreeqc3/src/SolutionIsotope.h
    phreeqc3/src/spread.cpp
    phreeqc3/src/SS.cxx
    phreeqc3/src/SS.h
    phreeqc3/src/SSassemblage.cxx
    phreeqc3/src/SSassemblage.h
    phreeqc3/src/SScomp.cxx
    phreeqc3/src/SScomp.h
    phreeqc3/src/step.cpp
    phreeqc3/src/StorageBin.cxx
    phreeqc3/src/StorageBin.h
    phreeqc3/src/StorageBinList.cpp
    phreeqc3/src/StorageBinList.h
    phreeqc3/src/structures.cpp
    phreeqc3/src/sundialsmath.cpp
    phreeqc3/src/sundialsmath.h
    phreeqc3/src/sundialstypes.h
    phreeqc3/src/Surface.cxx
    phreeqc3/src/Surface.h
    phreeqc3/src/SurfaceCharge.cxx
    phreeqc3/src/SurfaceCharge.h
    phreeqc3/src/SurfaceComp.cxx
    phreeqc3/src/SurfaceComp.h
    phreeqc3/src/System.cxx
    phreeqc3/src/System.h
    phreeqc3/src/tally.cpp
    phreeqc3/src/Temperature.cxx
    phreeqc3/src/Temperature.h
    phreeqc3/src/tidy.cpp
    phreeqc3/src/transport.cpp
    phreeqc3/src/Use.cpp
    phreeqc3/src/Use.h
    phreeqc3/src/UserPunch.cpp
    phreeqc3/src/UserPunch.h
    phreeqc3/src/utilities.cpp
    # phreeqc3/src/common
    phreeqc3/src/common/Parser.cxx
    phreeqc3/src/common/Parser.h
    phreeqc3/src/common/PHRQ_base.cxx
    phreeqc3/src/common/PHRQ_base.h
    phreeqc3/src/common/PHRQ_exports.h
    phreeqc3/src/common/PHRQ_io.cpp
    phreeqc3/src/common/PHRQ_io.h
    phreeqc3/src/common/phrqtype.h
    phreeqc3/src/common/Utils.cxx
    phreeqc3/src/common/Utils.h
    # phreeqc3/src/PhreeqcKeywords
    phreeqc3/src/PhreeqcKeywords/Keywords.cpp
    phreeqc3/src/PhreeqcKeywords/Keywords.h
    # Posea
    Posea/sizecbar.cpp
    Posea/sizecbar.h
    # res
    ${RESOURCE_FILES}
)

source_group("Resource Files" FILES ${RESOURCE_FILES})

target_compile_definitions(phreeqci
  PRIVATE
    _AFXDLL
    NO_UTF8_ENCODING
    PHREEQCI_GUI
    # WINVER=0x0A00
    # _WIN32_WINNT=0x0A00
)

if (ENABLE_ZEDGRAPH)
  target_compile_definitions(phreeqci
    PRIVATE
      MULTICHART
  )
endif()

# Precompiled headers
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
  # Setup precompiled headers automatically
  target_precompile_headers(phreeqci
  PRIVATE
    StdAfx.h
  )
else()
  # Setup precompiled headers manually
  target_compile_options(phreeqci
    PRIVATE
      "/YuStdAfx.h"
  )
  target_sources(phreeqci
    PRIVATE
      StdAfx.cpp
      StdAfx.h
  )
  target_compile_options(phreeqci
    PRIVATE
      "/FIStdAfx.h"
  )
  set_source_files_properties(StdAfx.cpp PROPERTIES COMPILE_FLAGS "/YcStdAfx.h")
endif()

target_include_directories(phreeqci
  PRIVATE
    # $<BUILD_INTERFACE:${phreeqci2_SOURCE_DIR}>
    # $<BUILD_INTERFACE:${phreeqci2_SOURCE_DIR}/src>
    # $<BUILD_INTERFACE:${phreeqci2_SOURCE_DIR}/src/common>
    # $<BUILD_INTERFACE:${phreeqci2_SOURCE_DIR}/src/PhreeqcKeywords>
    ${CMAKE_CURRENT_LIST_DIR}
    phreeqc3/src
    phreeqc3/src/common
    phreeqc3/src/PhreeqcKeywords
)

# Might need to use find_package for HtmlHelp
target_link_libraries(phreeqci
  PRIVATE
    HtmlHelp.lib
)

# Fix blurry display on high DPI screens (Reqs CMake 3.16 or higher)
set_property(TARGET phreeqci PROPERTY VS_DPI_AWARE "PerMonitor")

# Setup references for /CLR
if (ENABLE_ZEDGRAPH)
  set_target_properties(phreeqci PROPERTIES COMMON_LANGUAGE_RUNTIME "")
  set_target_properties(phreeqci PROPERTIES VS_DOTNET_REFERENCES "System;System.Drawing;System.Windows.Forms;${CMAKE_CURRENT_SOURCE_DIR}/phreeqc3/src/ZedGraph.dll")
endif()

# <CONFIG>_POSTFIX
# Postfix to append to the target file name for configuration <CONFIG>.
# Properties->Configuration Properties->General->Target Name
set_target_properties(phreeqci PROPERTIES MINSIZEREL_POSTFIX m)
set_target_properties(phreeqci PROPERTIES RELWITHDEBINFO_POSTFIX rd)
set_target_properties(phreeqci PROPERTIES DEBUG_POSTFIX d)

# Set the character set to Unicode
# Properties->Configuration Properties->Advanced->Character Set=Use Unicode Character Set
# target_compile_definitions(phreeqci PRIVATE UNICODE _UNICODE)

# Add a post-build command to copy the file
add_custom_command(
    TARGET phreeqci POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "phreeqci.eng" "$<TARGET_FILE_DIR:phreeqci>"
)


# Specify the default startup project in a Visual Studio solution.
# (So that starting a debugger works as expected)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT phreeqci)

# Microsoft Developer Studio Project File - Name="CE1CHScore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (WCE SH) Dynamic-Link Library" 0x0902
# TARGTYPE "Win32 (WCE x86em) Dynamic-Link Library" 0x0b02
# TARGTYPE "Win32 (WCE MIPS) Dynamic-Link Library" 0x0a02
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CE1CHScore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CE1CHScore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CE1CHScore.mak" CFG="CE1CHScore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CE1CHScore - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CE1CHScore - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CE1CHScore - Win32 (WCE MIPS) Release" (based on\
 "Win32 (WCE MIPS) Dynamic-Link Library")
!MESSAGE "CE1CHScore - Win32 (WCE MIPS) Debug" (based on\
 "Win32 (WCE MIPS) Dynamic-Link Library")
!MESSAGE "CE1CHScore - Win32 (WCE SH) Release" (based on\
 "Win32 (WCE SH) Dynamic-Link Library")
!MESSAGE "CE1CHScore - Win32 (WCE SH) Debug" (based on\
 "Win32 (WCE SH) Dynamic-Link Library")
!MESSAGE "CE1CHScore - Win32 (WCE x86em) Release" (based on\
 "Win32 (WCE x86em) Dynamic-Link Library")
!MESSAGE "CE1CHScore - Win32 (WCE x86em) Debug" (based on\
 "Win32 (WCE x86em) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "CE1CHScore - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
RSC=rc.exe
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
RSC=rc.exe
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WMIPSRel"
# PROP BASE Intermediate_Dir "WMIPSRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WMIPSRel"
# PROP Intermediate_Dir "WMIPSRel"
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /ML /W3 /O2 /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D "_WIN32_WCE" /D "UNICODE" /YX /QMRWCE /c
# ADD CPP /nologo /ML /W3 /O2 /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D "_WIN32_WCE" /D "UNICODE" /YX /QMRWCE /c
RSC=rc.exe
# ADD BASE RSC /l 0x419 /r /d "MIPS" /d "_MIPS_" /d "_WIN32_WCE" /d "UNICODE" /d "NDEBUG"
# ADD RSC /l 0x419 /r /d "MIPS" /d "_MIPS_" /d "_WIN32_WCE" /d "UNICODE" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /dll /machine:MIPS /subsystem:windowsce
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /dll /machine:MIPS /subsystem:windowsce
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WMIPSDbg"
# PROP BASE Intermediate_Dir "WMIPSDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WMIPSDbg"
# PROP Intermediate_Dir "WMIPSDbg"
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /MLd /W3 /Zi /Od /D "DEBUG" /D "MIPS" /D "_MIPS_" /D "_WIN32_WCE" /D "UNICODE" /YX /QMRWCE /c
# ADD CPP /nologo /MLd /W3 /Zi /Od /D "DEBUG" /D "MIPS" /D "_MIPS_" /D "_WIN32_WCE" /D "UNICODE" /YX /QMRWCE /c
RSC=rc.exe
# ADD BASE RSC /l 0x419 /r /d "MIPS" /d "_MIPS_" /d "_WIN32_WCE" /d "UNICODE" /d "DEBUG"
# ADD RSC /l 0x419 /r /d "MIPS" /d "_MIPS_" /d "_WIN32_WCE" /d "UNICODE" /d "DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /dll /debug /machine:MIPS /subsystem:windowsce
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /dll /debug /machine:MIPS /subsystem:windowsce
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WCESHRel"
# PROP BASE Intermediate_Dir "WCESHRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WCESHRel"
# PROP Intermediate_Dir "WCESHRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /ML /W3 /O2 /D "NDEBUG" /D "SH3" /D "_SH3_" /D "_WIN32_WCE" /D "UNICODE" /YX /c
# ADD CPP /nologo /ML /W3 /O2 /D "NDEBUG" /D "SH3" /D "_SH3_" /D "_WIN32_WCE" /D "UNICODE" /YX /c
RSC=rc.exe
# ADD BASE RSC /l 0x419 /r /d "SH3" /d "_SH3_" /d "_WIN32_WCE" /d "UNICODE" /d "NDEBUG"
# ADD RSC /l 0x419 /r /d "SH3" /d "_SH3_" /d "_WIN32_WCE" /d "UNICODE" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /dll /machine:SH3 /subsystem:windowsce
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /dll /machine:SH3 /def:"CE1CHSCore.def" /subsystem:windowsce
# SUBTRACT LINK32 /pdb:none
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WCESHDbg"
# PROP BASE Intermediate_Dir "WCESHDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCESHDbg"
# PROP Intermediate_Dir "WCESHDbg"
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /MLd /W3 /Zi /Od /D "DEBUG" /D "SH3" /D "_SH3_" /D "_WIN32_WCE" /D "UNICODE" /YX /c
# ADD CPP /nologo /MLd /W3 /Zi /Od /D "DEBUG" /D "SH3" /D "_SH3_" /D "_WIN32_WCE" /D "UNICODE" /YX /c
RSC=rc.exe
# ADD BASE RSC /l 0x419 /r /d "SH3" /d "_SH3_" /d "_WIN32_WCE" /d "UNICODE" /d "DEBUG"
# ADD RSC /l 0x419 /r /d "SH3" /d "_SH3_" /d "_WIN32_WCE" /d "UNICODE" /d "DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /dll /debug /machine:SH3 /subsystem:windowsce
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /dll /debug /machine:SH3 /subsystem:windowsce
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x86emRel"
# PROP BASE Intermediate_Dir "x86emRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x86emRel"
# PROP Intermediate_Dir "x86emRel"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /O2 /D "WIN32" /D "_WIN32" /D "NDEBUG" /D "_WINDOWS" /D "STRICT" /D "_WIN32_WCE" /D "UNDER_NT" /D "_WIN32_WCE_EMULATION" /D "UNICODE" /D "_UNICODE" /D "_X86_" /YX /c
# ADD CPP /nologo /W3 /O2 /D "WIN32" /D "_WIN32" /D "NDEBUG" /D "_WINDOWS" /D "STRICT" /D "_WIN32_WCE" /D "UNDER_NT" /D "_WIN32_WCE_EMULATION" /D "UNICODE" /D "_UNICODE" /D "_X86_" /YX /c
RSC=rc.exe
# ADD BASE RSC /l 0x419 /d "_WIN32_WCE" /d "UNDER_NT" /d "_WIN32_WCE_EMULATION" /d "UNICODE" /d "NDEBUG"
# ADD RSC /l 0x419 /d "_WIN32_WCE" /d "UNDER_NT" /d "_WIN32_WCE_EMULATION" /d "UNICODE" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib coredll.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /windowsce:emulation
# ADD LINK32 comctl32.lib coredll.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /windowsce:emulation
EMPFILE=empfile.exe
# ADD BASE EMPFILE -NOSHELL -COPY
# ADD EMPFILE -NOSHELL -COPY

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x86emDbg"
# PROP BASE Intermediate_Dir "x86emDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x86emDbg"
# PROP Intermediate_Dir "x86emDbg"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /Zi /Od /D "WIN32" /D "_WIN32" /D "_DEBUG" /D "_WINDOWS" /D "STRICT" /D "_WIN32_WCE" /D "UNDER_NT" /D "_WIN32_WCE_EMULATION" /D "UNICODE" /D "_UNICODE" /D "_X86_" /YX /c
# ADD CPP /nologo /W3 /Gm /Zi /Od /D "WIN32" /D "_WIN32" /D "_DEBUG" /D "_WINDOWS" /D "STRICT" /D "_WIN32_WCE" /D "UNDER_NT" /D "_WIN32_WCE_EMULATION" /D "UNICODE" /D "_UNICODE" /D "_X86_" /YX /c
RSC=rc.exe
# ADD BASE RSC /l 0x419 /d "_WIN32_WCE" /d "UNDER_NT" /d "_WIN32_WCE_EMULATION" /d "UNICODE" /d "_DEBUG"
# ADD RSC /l 0x419 /d "_WIN32_WCE" /d "UNDER_NT" /d "_WIN32_WCE_EMULATION" /d "UNICODE" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib coredll.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /windowsce:emulation
# ADD LINK32 comctl32.lib coredll.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /windowsce:emulation
EMPFILE=empfile.exe
# ADD BASE EMPFILE -NOSHELL -COPY
# ADD EMPFILE -NOSHELL -COPY

!ENDIF 

# Begin Target

# Name "CE1CHScore - Win32 Release"
# Name "CE1CHScore - Win32 Debug"
# Name "CE1CHScore - Win32 (WCE MIPS) Release"
# Name "CE1CHScore - Win32 (WCE MIPS) Debug"
# Name "CE1CHScore - Win32 (WCE SH) Release"
# Name "CE1CHScore - Win32 (WCE SH) Debug"
# Name "CE1CHScore - Win32 (WCE x86em) Release"
# Name "CE1CHScore - Win32 (WCE x86em) Debug"
# Begin Source File

SOURCE=.\ascii8x16.h
# End Source File
# Begin Source File

SOURCE=.\ce1core.c

!IF  "$(CFG)" == "CE1CHScore - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Release"

DEP_CPP_CE1CO=\
	".\ce1core.h"\
	".\font.h"\
	".\layout.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ce1core.h
# End Source File
# Begin Source File

SOURCE=.\dbcs.c

!IF  "$(CFG)" == "CE1CHScore - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Release"

DEP_CPP_DBCS_=\
	".\dbcs.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dbcs.h
# End Source File
# Begin Source File

SOURCE=.\font.c

!IF  "$(CFG)" == "CE1CHScore - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Release"

DEP_CPP_FONT_=\
	".\ascii8x16.h"\
	".\font.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\layout.c

!IF  "$(CFG)" == "CE1CHScore - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Release"

DEP_CPP_LAYOU=\
	".\dbcs.h"\
	".\font.h"\
	".\layout.h"\
	".\render.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\layout.h
# End Source File
# Begin Source File

SOURCE=.\render.c

!IF  "$(CFG)" == "CE1CHScore - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Release"

DEP_CPP_RENDE=\
	".\render.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHScore - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\render.h
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="CE1CHS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (WCE x86em) Application" 0x0b01
# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (WCE MIPS) Application" 0x0a01
# TARGTYPE "Win32 (WCE SH) Application" 0x0901

CFG=CE1CHS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CE1CHS.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CE1CHS.MAK" CFG="CE1CHS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CE1CHS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CE1CHS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "CE1CHS - Win32 (WCE MIPS) Release" (based on\
 "Win32 (WCE MIPS) Application")
!MESSAGE "CE1CHS - Win32 (WCE MIPS) Debug" (based on\
 "Win32 (WCE MIPS) Application")
!MESSAGE "CE1CHS - Win32 (WCE SH) Release" (based on\
 "Win32 (WCE SH) Application")
!MESSAGE "CE1CHS - Win32 (WCE SH) Debug" (based on\
 "Win32 (WCE SH) Application")
!MESSAGE "CE1CHS - Win32 (WCE x86em) Release" (based on\
 "Win32 (WCE x86em) Application")
!MESSAGE "CE1CHS - Win32 (WCE x86em) Debug" (based on\
 "Win32 (WCE x86em) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "CE1CHS - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Release"

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
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /machine:MIPS /subsystem:windowsce
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /machine:MIPS /subsystem:windowsce
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Debug"

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
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /debug /machine:MIPS /subsystem:windowsce
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /debug /machine:MIPS /subsystem:windowsce
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Release"

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
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /machine:SH3 /subsystem:windowsce
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib ..\CE1CHScore\WCESHRel\CE1CHScore.lib /nologo /machine:SH3 /subsystem:windowsce
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Debug"

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
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /debug /machine:SH3 /subsystem:windowsce
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /debug /machine:SH3 /subsystem:windowsce
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Release"

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
# ADD BASE LINK32 comctl32.lib coredll.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386 /windowsce:emulation
# ADD LINK32 comctl32.lib coredll.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386 /windowsce:emulation
EMPFILE=empfile.exe
# ADD BASE EMPFILE -NOSHELL -COPY
# ADD EMPFILE -NOSHELL -COPY

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Debug"

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
# ADD BASE LINK32 comctl32.lib coredll.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /windowsce:emulation
# ADD LINK32 comctl32.lib coredll.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /windowsce:emulation
EMPFILE=empfile.exe
# ADD BASE EMPFILE -NOSHELL -COPY
# ADD EMPFILE -NOSHELL -COPY

!ENDIF 

# Begin Target

# Name "CE1CHS - Win32 Release"
# Name "CE1CHS - Win32 Debug"
# Name "CE1CHS - Win32 (WCE MIPS) Release"
# Name "CE1CHS - Win32 (WCE MIPS) Debug"
# Name "CE1CHS - Win32 (WCE SH) Release"
# Name "CE1CHS - Win32 (WCE SH) Debug"
# Name "CE1CHS - Win32 (WCE x86em) Release"
# Name "CE1CHS - Win32 (WCE x86em) Debug"
# Begin Source File

SOURCE=.\about.c

!IF  "$(CFG)" == "CE1CHS - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Release"

DEP_CPP_ABOUT=\
	".\about.h"\
	".\ce1core.h"\
	".\ui.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\about.h
# End Source File
# Begin Source File

SOURCE=.\CE1CHS.c

!IF  "$(CFG)" == "CE1CHS - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Release"

DEP_CPP_CE1CH=\
	".\about.h"\
	".\ce1core.h"\
	".\document.h"\
	".\home.h"\
	".\reader.h"\
	".\resource.h"\
	".\settings.h"\
	".\ui.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ce1core.h
# End Source File
# Begin Source File

SOURCE=.\document.c

!IF  "$(CFG)" == "CE1CHS - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Release"

DEP_CPP_DOCUM=\
	".\document.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\document.h
# End Source File
# Begin Source File

SOURCE=.\home.c

!IF  "$(CFG)" == "CE1CHS - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Release"

DEP_CPP_HOME_=\
	".\ce1core.h"\
	".\home.h"\
	".\ui.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\home.h
# End Source File
# Begin Source File

SOURCE=.\logo.c

!IF  "$(CFG)" == "CE1CHS - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Release"

DEP_CPP_LOGO_=\
	".\logo.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\logo.h
# End Source File
# Begin Source File

SOURCE=.\reader.c

!IF  "$(CFG)" == "CE1CHS - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Release"

DEP_CPP_READE=\
	".\ce1core.h"\
	".\document.h"\
	".\reader.h"\
	".\ui.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\reader.h
# End Source File
# Begin Source File

SOURCE=.\settings.c

!IF  "$(CFG)" == "CE1CHS - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Release"

DEP_CPP_SETTI=\
	".\ce1core.h"\
	".\settings.h"\
	".\ui.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\settings.h
# End Source File
# Begin Source File

SOURCE=.\ui.c

!IF  "$(CFG)" == "CE1CHS - Win32 Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Release"

DEP_CPP_UI_Ce=\
	".\ui.h"\
	

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE SH) Debug"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "CE1CHS - Win32 (WCE x86em) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ui.h
# End Source File
# Begin Source File

SOURCE=.\CE1CHS.rc
# End Source File
# End Target
# End Project

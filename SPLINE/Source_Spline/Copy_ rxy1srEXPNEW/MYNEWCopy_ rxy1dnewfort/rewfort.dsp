# Microsoft Developer Studio Project File - Name="rewfort" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=rewfort - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rewfort.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rewfort.mak" CFG="rewfort - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rewfort - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "rewfort - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rewfort - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /nologo /warn:nofileopt /winapp
# ADD F90 /compile_only /nologo /warn:nofileopt /winapp
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib kernel32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib kernel32.lib /nologo /subsystem:windows /machine:I386 /out:"Release/reorxy1.exe"

!ELSEIF  "$(CFG)" == "rewfort - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt /winapp
# ADD F90 /browser /check:bounds /compile_only /dbglibs /nologo /traceback /warn:argument_checking /warn:nofileopt /winapp
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib kernel32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib kernel32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /out:"Debug/Reorxy1.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "rewfort - Win32 Release"
# Name "rewfort - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=.\abdx1.for
DEP_F90_ABDX1=\
	".\resource.fd"\
	
# End Source File
# Begin Source File

SOURCE=.\Aldfp.for
DEP_F90_ALDFP=\
	".\Debug\abdx.mod"\
	
# End Source File
# Begin Source File

SOURCE=.\Almo.for
DEP_F90_ALMO_=\
	".\Debug\abdx.mod"\
	".\INTERFCS.INC"\
	
# End Source File
# Begin Source File

SOURCE=.\Alregm.for
DEP_F90_ALREG=\
	".\INTERFCS.INC"\
	
# End Source File
# Begin Source File

SOURCE=.\Conlaf.for
# End Source File
# Begin Source File

SOURCE=.\Func.for
DEP_F90_FUNC_=\
	".\COMMONS.INC"\
	".\INTERFCS.INC"\
	
# End Source File
# Begin Source File

SOURCE=.\Grad.for
DEP_F90_GRAD_=\
	".\COMMONS.INC"\
	".\INTERFCS.INC"\
	
# End Source File
# Begin Source File

SOURCE=.\Point.for
DEP_F90_POINT=\
	".\COMMONS.INC"\
	".\INTERFCS.INC"\
	
# End Source File
# Begin Source File

SOURCE=.\Point_new0.for
DEP_F90_POINT_=\
	".\COMMONS.INC"\
	".\INTERFCS.INC"\
	
# End Source File
# Begin Source File

SOURCE=.\Pointg.for
DEP_F90_POINTG=\
	".\COMMONS.INC"\
	".\INTERFCS.INC"\
	
# End Source File
# Begin Source File

SOURCE=.\Reo.for
DEP_F90_REO_F=\
	".\COMMONS.INC"\
	".\Debug\abdx.mod"\
	".\INTERFCS.INC"\
	
# End Source File
# Begin Source File

SOURCE=.\rewfort.f90
DEP_F90_REWFO=\
	".\Debug\abdx.mod"\
	
# End Source File
# Begin Source File

SOURCE=.\Start.for
DEP_F90_START=\
	".\COMMONS.INC"\
	".\INTERFCS.INC"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\resource.fd
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\rewfort.fi
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\rewfort.ico
# End Source File
# Begin Source File

SOURCE=.\rewfort.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\Commons.inc
# End Source File
# Begin Source File

SOURCE=.\Interfcs.inc
# End Source File
# End Target
# End Project

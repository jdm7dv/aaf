###############################################################################
#
# $Id: Windows_NT.mk,v 1.14.2.1 2004/03/19 17:40:50 stuart_hc Exp $ $Name:  $
#
# The contents of this file are subject to the AAF SDK Public
# Source License Agreement (the "License"); You may not use this file
# except in compliance with the License.  The License is available in
# AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
# Association or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.
#
# The Original Code of this file is Copyright 1998-2004, Licensor of the
# AAF Association.
#
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

#
# Contains elements for i386 MS Windows NT/2000/XP builds.
# Tested with cgywin-5.0, cygwin-5.1 on MS Windows 2000.
#

PTFM_SUFX = nt
OBJ = obj
AAF_LIB = lib
EXE = exe

RM = rm.exe
ECHO = echo.exe
CP = cp.exe
CAT = cat.exe
CHMOD = chmod.exe
MV = mv.exe
SH = bash.exe
TOUCH = touch.exe
PERL = perl.exe

SHELL = bash.exe
MAKE_SUFFIX =

CC = cl
LD = cl
AAF_CMD_LINK = link

AAF_LINK_FLAGS = -lib -nologo
AAF_LINK_OUTPUT_FLAG = -out:

LFLAGS = 
CFLAGS = /nologo /Gi- /MDd /W3 /GX /Z7 /Od /Yd /D "WIN32" /D "_WIN32" /D "_DEBUG" /D "_WINDOWS"\
	/D "_UNICODE" /D "UNICODE"
#CFLAGS = /nologo /Gi-
OBJFILE = /Fo$*.${OBJ}
EXEFLAG = /Fo
DLL_LINK_FLAGS = kernel32.lib user32.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib\
	/nologo /DLL /subsystem:windows /debug /pdb:none 
ARFLAGS = -r

MIDL = midl
MIDL_SYS_CFLAGS = /server none /client none

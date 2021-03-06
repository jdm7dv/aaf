###############################################################################
#
# $Id: GNUmakefile,v 1.14 2009/06/01 11:47:14 stuart_hc Exp $ $Name:  $
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
# All rights reserved.
#
###############################################################################

PACKAGE = MultiGenTest

AAFBASE ?= ../../..

ifeq ($(shell uname), IRIX)
# Warnings turned off:
# 1107 - signed bit field has length of 1 bit in various stl files
MODULE_PLATFORM_CFLAGS = -LANG:std -woff 1107
LIBCIO = -lCio
endif

# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR)


# CXXFILES2 is declared just to get the list of object files
# from MultiGenTests directory. These object files are created 
# when building target MultiGenTests
CXXFILES_TMP = $(wildcard MultiGenTests/*.$(CPP_EXTENSION)) $(wildcard MultiGenTests/*.c)
CXXFILES2 = $(CXXFILES_TMP:MultiGenTests/%=%)


.PHONY : all
all : MultiGenTests $(AAFSDKBINDIR)/MultiGenTest$(EXE)

$(AAFSDKBINDIR)/MultiGenTest$(EXE) : $(CXXOBJS) $(CXXOBJS2)
	$(LD) $(CXXOBJS) $(CXXOBJS2) $(LINK_AAF_APP)


.PHONY : MultiGenTests
MultiGenTests :
	@echo "\nBuilding MultiGenTests\n"
	cd MultiGenTests && $(MAKE)


.PHONY : clean
clean :
	cd MultiGenTests && $(MAKE) GENDEPS=0 $@
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/MultiGenTest$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean
	cd MultiGenTests && $(MAKE) GENDEPS=0 $@


# Check that the basic MultiGenTest operations work.
# NOTE: MultiGenTest is designed to be run across multiple
# platforms and SDK versions - this test merely checks that
# MultiGenTest is in good working order.
TEST_LIB_NAME = libcom-api$(DLL)
ifeq ($(AAFPLATFORM),Win)
  TEST_LIB_NAME = AAFCOAPI$(DLL)
endif

.PHONY : check
check : $(AAFSDKBINDIR)/MultiGenTest$(EXE)
	rm -f basic.aaf
	$(APP_RUN_ENV) $(AAFSDKBINDIR)/MultiGenTest$(EXE) -r LoadLib $(TEST_LIB_NAME) -r FileOp write S4KBinary basic.aaf -r AddMasterMobs A B -r FileOp save_and_close -r UnloadLib || exit 1
	$(APP_RUN_ENV) $(AAFSDKBINDIR)/MultiGenTest$(EXE) -r LoadLib $(TEST_LIB_NAME) -r FileOp read S4KBinary basic.aaf -r FindMasterMobsExclusive A B -r FileOp close -r UnloadLib || exit 1

.PHONY : valgrind-check
valgrind-check : $(AAFSDKBINDIR)/MultiGenTest$(EXE)
	rm -f basic.aaf
	$(APP_RUN_ENV) $(VALGRIND_CMD) $(AAFSDKBINDIR)/MultiGenTest$(EXE) -r LoadLib $(TEST_LIB_NAME) -r FileOp write S4KBinary basic.aaf -r AddMasterMobs A B -r FileOp save_and_close -r UnloadLib || exit 1
	$(APP_RUN_ENV) $(VALGRIND_CMD) $(AAFSDKBINDIR)/MultiGenTest$(EXE) -r LoadLib $(TEST_LIB_NAME) -r FileOp read S4KBinary basic.aaf -r FindMasterMobsExclusive A B -r FileOp close -r UnloadLib || exit 1


include $(AAFBASE)/build/rules.mk

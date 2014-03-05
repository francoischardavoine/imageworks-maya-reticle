# ***************************************************************************** # 
#              Copyright (c) 2005 Sony Pictures Imageworks, Inc.
#                             All rights reserved.
# 
#   This  material  contains  the confidential and  proprietary information
#   of Sony Pictures Imageworks,  Inc.  and may not be disclosed, copied or
#   duplicated  in any form,  electronic or hardcopy,  in whole  or in part,
#   without  the express prior written  consent of Sony Pictures Imageworks,
#   Inc. This copyright notice does not imply publication.
# 
# *****************************************************************************
#
# Makefile for a maya plugin.

# Type of build: bin, lib, dso or maya (or script)
PROJECT_TYPE = maya

ifndef MAYA_VERSION
    MAYA_VERSION = 2013
endif

# This will be the root of the plugin name as well as
# the directory name needed for installing any mel
PROJECT = spReticleLoc

# Lets you tell the install rule which show to install this on.
MAYA_SHOW = spi

# Lets the install rule know to install in your dev directory on a
# particular show.
MAYA_DEV = dev

# The extension used on your C++ files
C++SUFFIX=.cpp

# Any additional C++ and C flags to pass to the compiler
CXXFLAGS =
CFLAGS = 

IPATH = -I/usr/include/freetype2
LPATH = -L$(OPENGLDSO)

# This variable contains the roots of all file pairs that exist in
# .cpp and .h form. (for instance, "yourClass1" will be expanded
# to yourClass1.cpp and yourClass1.h). Any additional .cpp or .h files
# can go in the CXXFILES and HFILES variables
CLASSES = spReticleLoc GPURenderer OpenGLRenderer V2Renderer
CXXFILES = 
CFILES = 
HFILES = defines.h font.h util.h

# This contains all of the mel files that need to be installed for
# this tool to work. A "make install" will place these files in
# the correct mel folder. Place AE templates in MAYA_MELFILES_OTHERS.
# You can use MAYA_MELFILES = $(notdir $(wildcard $(SRCDIR)/*.mel))
MAYA_MELDIR   =
MAYA_MELFILES =
MAYA_MELFILES_OTHERS = AEspReticleLocTemplate.mel

# Use the SYSLIBS variable to link against Maya libs, for example:
SYSLIBS = -lOpenMaya -lOpenMayaUI -lOpenMayaAnim 
SYSLIBS += -lftgl

# Sibling directories that contain libraries that must
# be compiled as well
COMPONENTS = 

# This sets the optimisation level for the 'opt' target
OPT_LEVEL = 3

# Essentially, the Make System makes certain
# "intelligent" decisions about what to name libraries. When building
# Maya plug-ins, those decisions are not so great. This lets the
# system know to make a copy of the resulting dso with the name in the
# variable.
POST_LINK_RENAME = $(PROJECT).so

# Type of compiled code installed when using
# 'make install' and 'make install_local'
INSTALL_TYPE=opt
INSTALL_LOCAL_TYPE=debug

# IMPORTANT! You need to type in the exact name of this file here!
# There is a make flaw that does not let me know the name of this file
# and since I need to call it recursively, you need to tell me the
# name here.
MAKEFILE = Makefile

# You should not need to tweak this, it is used for debugging
MAKEFILE_SOURCE=/usr/local/spi/lib/make

# Don't touch this line - this where the work gets done!
include $(MAKEFILE_SOURCE)/Makefile.unique


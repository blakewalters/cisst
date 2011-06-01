#
# $Id$
#
# Author(s):  Anton Deguet
# Created on: 2011-05-31
#
# (C) Copyright 2011 Johns Hopkins University (JHU), All Rights
# Reserved.
#
# --- begin cisst license - do not edit ---
#
# This software is provided "as is" under an open source license, with
# no warranty.  The complete license can be found in license.txt and
# http://www.cisst.org/cisst/license.txt.
#
# --- end cisst license ---
#
# Try to find BlackMagic DeckLink SDK
#
# The following values are defined
#
# DeckLink_INCLUDE_DIR          - include directories to use
# DeckLink_EXTRA_CODE           - extra code to include in project
#

if (WIN32)
  set (DeckLink_OS_PREFIX "Win")
else (WIN32)
  if (APPLE)
    set (DeckLink_OS_PREFIX "Mac")
  else (APPLE)
    set (DeckLink_OS_PREFIX "Linux")
  endif (APPLE)
endif (WIN32)

# find header file
find_path (DeckLink_DIR ${DeckLink_OS_PREFIX}/include/DeckLinkAPI.h
           DOC "Path to SDK, i.e. directory containing Win, Linux, Mac, ...")
if (DeckLink_DIR)
  # set include dir
  set (DeckLink_INCLUDE_DIR "${DeckLink_DIR}/${DeckLink_OS_PREFIX}/include")
  # on windows, find idl file
  if (WIN32)

  # on other OSs, find DeckLinkAPIDispatch.cpp
  else (WIN32)
    find_file (DeckLink_EXTRA_CODE DeckLinkAPIDispatch.cpp
               PATHS ${DeckLink_INCLUDE_DIR}
               DOC "Full filename for DeckLinkAPIDispatch.cpp")
    if (DeckLink_EXTRA_CODE)
       set (DeckLink_FOUND true)
       mark_as_advanced (DeckLink_OS_PREFIX DeckLink_DIR)
    endif (DeckLink_EXTRA_CODE)
  endif (WIN32)
endif (DeckLink_DIR)

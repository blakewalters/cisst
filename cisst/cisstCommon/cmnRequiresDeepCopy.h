/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2009-11-17

  (C) Copyright 2009 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/


/*!
  \file
  \brief Declaration of cmnRequiresDeepCopy
  \ingroup cisstCommon
*/
#pragma once

#ifndef _cmnRequiresDeepCopy_h
#define _cmnRequiresDeepCopy_h

#include <cisstCommon/cmnPortability.h>
#include <cisstCommon/cmnRequiresDeepCopy.h>

/*! Templated global function used to determine if a type/class
  requires deep copy or not. To be safe, the assumption is that all
  types require deep copy.  To avoid deep copies, the user must
  specialize this function for any new type. */
template <class _elementType>
bool cmnRequiresDeepCopy(void) {
    return true;
}

/*! Template specialization for native C/C++ types.  The assumption is
  that native types don't need deep copy. */
//@{
template <> inline bool cmnRequiresDeepCopy<float>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<double>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<long double>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<bool>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<char>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<unsigned char>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<short>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<unsigned short>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<int>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<unsigned int>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<long long int>(void) { return false; }
template <> inline bool cmnRequiresDeepCopy<unsigned long long int>(void) { return false; }
//@}

#endif // _cmnRequiresDeepCopy_h
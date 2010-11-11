/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2004-08-18

  (C) Copyright 2004-2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/


#include <cisstCommon/cmnClassServicesBase.h>
#include <cisstCommon/cmnClassRegister.h>

cmnClassServicesBase::cmnClassServicesBase(const std::string & className, const std::type_info * typeInfo, LogLoDType lod):
    TypeInfoMember(typeInfo)
{
    this->SetLoD(lod);
    NameMember = cmnClassRegister::Register(this, className);
}


void cmnClassServicesBase::SetLoD(LogLoDType newLoD) {
    LoDMember = newLoD;
}

/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
Author(s):	Simon Leonard
Created on:   2011-08-22

(C) Copyright 2008 Johns Hopkins University (JHU), All Rights
Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisstParameterTypes/prmForceTorqueJointSet.h>

prmForceTorqueJointSet::~prmForceTorqueJointSet()
{
}

void prmForceTorqueJointSet::SetSize(size_type size)
{
    MaskMember.SetSize(size);
    ForceTorqueMember.SetSize(size);
}

void prmForceTorqueJointSet::SerializeRaw(std::ostream & outputStream) const
{
    BaseType::SerializeRaw(outputStream);
    MaskMember.SerializeRaw(outputStream);
    ForceTorqueMember.SerializeRaw(outputStream);
}

void prmForceTorqueJointSet::DeSerializeRaw(std::istream & inputStream)
{
    BaseType::DeSerializeRaw(inputStream);
    MaskMember.DeSerializeRaw(inputStream);
    ForceTorqueMember.DeSerializeRaw(inputStream);
}




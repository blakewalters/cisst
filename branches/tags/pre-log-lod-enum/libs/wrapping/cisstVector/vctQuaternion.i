/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: vctQuaternion.i,v 1.5 2007/04/26 19:33:58 anton Exp $

  Author(s):  Anton Deguet
  Created on: 2005-08-19

  (C) Copyright 2005-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/


#include "cisstVector/vctQuaternionBase.h"

// instantiate the templated base class
%include "cisstVector/vctQuaternionBase.h"
%template(vctQuat) vctQuaternionBase<vctDynamicVector<double> >;

%{
    typedef vctQuaternionBase<vctDynamicVector<double> > vctQuat;
%}

typedef vctQuaternionBase<vctDynamicVector<double> > vctQuat;


%types(vctQuat *);

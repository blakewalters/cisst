/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2004-08-31

  (C) Copyright 2004-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/


#include <fstream>

#include <cisstCommon/cmnLogger.h>


cmnLogger::cmnLogger(const std::string & defaultLogFileName):
    LoD(CMN_LOG_MASK_ALL),
    LoDMultiplexerStreambuf()
{
    LoDMultiplexerStreambuf.AddChannel(*(DefaultLogFile(defaultLogFileName)), CMN_LOG_MASK_DEFAULT);
}


std::ofstream * cmnLogger::DefaultLogFile(const std::string & defaultLogFileName)
{
    static std::ofstream defaultLogFile(defaultLogFileName.c_str());
    return &defaultLogFile;
}


cmnLogger * cmnLogger::Instance(void) {
    // create a static variable
    static cmnLogger instance;
    return &instance;
}

void cmnLogger::SetLoDInstance(LogLoDType lod) {
#if 0
    if (lod < CMN_LOG_LOD_NONE) {
        lod = CMN_LOG_LOD_NONE;
    } else if (lod > CMN_LOG_LOD_VERY_VERBOSE) {
        lod = CMN_LOG_LOD_VERY_VERBOSE;
    }
#endif
    CMN_LOG(CMN_LOG_BIT_INIT_WARNING) << "Class cmnLogger: level of Detail set to \"" << lod << "\"" << std::endl;
    LoD = lod;
}

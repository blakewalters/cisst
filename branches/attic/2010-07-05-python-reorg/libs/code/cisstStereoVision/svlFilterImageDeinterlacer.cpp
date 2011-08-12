/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$
  
  Author(s):  Balazs Vagvolgyi
  Created on: 2006 

  (C) Copyright 2006-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#include <cisstStereoVision/svlFilterImageDeinterlacer.h>
#include "ipDeinterlacing.h"

using namespace std;

/******************************************/
/*** svlFilterImageDeinterlacer class *****/
/******************************************/

CMN_IMPLEMENT_SERVICES(svlFilterImageDeinterlacer)

svlFilterImageDeinterlacer::svlFilterImageDeinterlacer() :
    svlFilterBase(),
    cmnGenericObject()
{
    AddSupportedType(svlTypeImageRGB, svlTypeImageRGB);
    AddSupportedType(svlTypeImageRGBStereo, svlTypeImageRGBStereo);
    MethodToUse = MethodNone;
}

svlFilterImageDeinterlacer::~svlFilterImageDeinterlacer()
{
    Release();
}

int svlFilterImageDeinterlacer::Initialize(svlSample* inputdata)
{
    Release();

    OutputData = inputdata;

    return SVL_OK;
}

int svlFilterImageDeinterlacer::ProcessFrame(svlProcInfo* procInfo, svlSample* inputdata)
{
    ///////////////////////////////////////////
    // Check if the input sample has changed //
      if (!IsNewSample(inputdata))
          return SVL_ALREADY_PROCESSED;
    ///////////////////////////////////////////

    // Passing the same image for the next filter
    OutputData = inputdata;

    svlSampleImageBase* img = dynamic_cast<svlSampleImageBase*>(inputdata);
    unsigned int videochannels = img->GetVideoChannels();
    unsigned int idx;

    _ParallelLoop(procInfo, idx, videochannels)
    {
        // Processing the input image directly
        switch (MethodToUse) {
            case MethodNone:
                // NOP
            break;

            case MethodBlending:
                Blending(img->GetUCharPointer(idx),
                         static_cast<int>(img->GetWidth(idx)), static_cast<int>(img->GetHeight(idx)));
            break;

            case MethodDiscarding:
                Discarding(img->GetUCharPointer(idx),
                           static_cast<int>(img->GetWidth(idx)), static_cast<int>(img->GetHeight(idx)));
            break;

            case MethodAdaptiveBlending:
                AdaptiveBlending(img->GetUCharPointer(idx),
                                 static_cast<int>(img->GetWidth(idx)), static_cast<int>(img->GetHeight(idx)));
            break;

            case MethodAdaptiveDiscarding:
                AdaptiveDiscarding(img->GetUCharPointer(idx),
                                   static_cast<int>(img->GetWidth(idx)), static_cast<int>(img->GetHeight(idx)));
            break;
        }
    }

    return SVL_OK;
}

int svlFilterImageDeinterlacer::Release()
{
    return SVL_OK;
}

void svlFilterImageDeinterlacer::SetMethod(svlFilterImageDeinterlacer::Method method)
{
    MethodToUse = method;
}

svlFilterImageDeinterlacer::Method svlFilterImageDeinterlacer::GetMethod()
{
    return MethodToUse;
}

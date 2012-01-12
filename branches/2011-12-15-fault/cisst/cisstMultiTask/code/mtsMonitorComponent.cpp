/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: mtsMonitorComponent.cpp 3303 2012-01-02 16:33:23Z mjung5 $

  Author(s):  Min Yang Jung
  Created on: 2011-12-29

  (C) Copyright 2004-2012 Johns Hopkins University (JHU), All Rights Reserved.

  --- begin cisst license - do not edit ---

  This software is provided "as is" under an open source license, with
  no warranty.  The complete license can be found in license.txt and
  http://www.cisst.org/cisst/license.txt.

  --- end cisst license ---
*/

#include <cisstMultiTask/mtsMonitorComponent.h>
#include <cisstMultiTask/mtsMonitorFilterBasics.h>

CMN_IMPLEMENT_SERVICES(mtsMonitorComponent);

const std::string NameOfMonitorComponent = "Monitor";

mtsMonitorComponent::mtsMonitorComponent()
    //: mtsTaskPeriodic(NameOfMonitorComponent, 10.0 * cmn_ms)
    : mtsTaskPeriodic(NameOfMonitorComponent, 1.0 * cmn_s) // MJ TEMP
{
    TargetComponents = new TargetComponentsType(true);
}

mtsMonitorComponent::~mtsMonitorComponent()
{
    TargetComponents->DeleteAll();
    delete TargetComponents;
}

void mtsMonitorComponent::Run(void)
{
    FetchNewValues();
    //PrintTargetComponents();
}

void mtsMonitorComponent::FetchNewValues(void)
{
    TargetComponentsType::const_iterator it = TargetComponents->begin();
    const TargetComponentsType::const_iterator itEnd = TargetComponents->end();
    for (; it != itEnd; ++it) {
        TargetComponent * target = it->second;
        CMN_ASSERT(target);
        target->GetPeriod(target->Period);
    }
}

void mtsMonitorComponent::PrintTargetComponents(void)
{
    TargetComponentsType::const_iterator it = TargetComponents->begin();
    const TargetComponentsType::const_iterator itEnd = TargetComponents->end();
    for (; it != itEnd; ++it) {
        TargetComponent * target = it->second;
        CMN_ASSERT(target);
        target->GetPeriod(target->Period);
        std::cout << target->Name << ": " << target->Period << std::endl;
    }
}

bool mtsMonitorComponent::AddTargetComponent(mtsTask * task)
{
    const std::string taskName = task->GetName();

    if (TargetComponents->FindItem(taskName)) {
        CMN_LOG_CLASS_RUN_WARNING << "AddTargetComponent: task \"" << taskName << "\" is already registered" << std::endl;
        return true;
    }

    // Add new target component
    TargetComponent * newTargetComponent = new TargetComponent;
    newTargetComponent->Name = taskName;
    newTargetComponent->InterfaceRequired = AddInterfaceRequired(GetNameOfStateTableAccessInterface(taskName));
    newTargetComponent->InterfaceRequired->AddFunction("GetPeriod", newTargetComponent->GetPeriod);
    // FIXME Add more default filters
    if (!TargetComponents->AddItem(taskName, newTargetComponent)) {
        CMN_LOG_CLASS_RUN_ERROR << "AddTargetComponent: Failed to add state table access interface for task \"" << taskName << "\"" << std::endl;
        return false;
    }

    // Add "Period" state to the monitoring state table of this component with the name of
    // (component name)+"Period".  Default filters will run on this state variable.
    std::string stateName(taskName);
    stateName += "Period";
    // MJ TEMP: Adding a new element to state table on the fly is not thread safe -- need to fix this.
    this->StateTableMonitor.NewElement(stateName, &newTargetComponent->Period);

    // Create default filters and add them to the monitor state table of this component (mtsMonitorComponent)
    mtsMonitorFilterBase * filter;
    // Bypass filter (for testing purpose) MJ: remove this later
    //filter = new mtsMonitorFilterBypass(mtsStateTable::NamesOfDefaultElements::Period); // for self-monitoring
#define CHECK_ERROR( _filterName )\
    if (!this->StateTableMonitor.AddFilter(filter)) {\
        std::stringstream ss;\
        ss << "mtsMonitorComponent::AddTargetComponent: Failed to add filter \"" << filter->GetFilterName()\
           << "\" to monitor a target component \"" << taskName << "\"";\
        cmnThrow(ss.str());\
    }
    filter = new mtsMonitorFilterBypass(stateName); CHECK_ERROR();
    filter = new mtsMonitorFilterTrendVel(stateName, this->Period); CHECK_ERROR();
#undef CHECK_ERROR

    return true;
}

bool mtsMonitorComponent::RemoveTargetComponent(const std::string & taskName)
{
    if (!TargetComponents->FindItem(taskName)) {
        CMN_LOG_CLASS_RUN_WARNING << "RemoveTargetComponent: task \"" << taskName << "\" is not found" << std::endl;
        return false;
    }

    return TargetComponents->RemoveItem(taskName);
}

//-----------------------------------------------
//  Getters
//-----------------------------------------------
const std::string  mtsMonitorComponent::GetNameOfStateTableAccessInterface(const std::string & taskName) const {
    return "Monitor" + taskName;
}

const std::string & mtsMonitorComponent::GetNameOfMonitorComponent(void) {
    return NameOfMonitorComponent;
}

/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Peter Kazanzides, Anton Deguet
  Created on: 2008-11-13

  (C) Copyright 2008-2011 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/


#ifndef _mtsInterfaceRequired_h
#define _mtsInterfaceRequired_h

#include <cisstMultiTask/mtsInterface.h>

#include <cisstCommon/cmnNamedMap.h>
#include <cisstOSAbstraction/osaThread.h>

#include <cisstMultiTask/mtsCommandBase.h>

#include <cisstMultiTask/mtsCallableVoidMethod.h>
#include <cisstMultiTask/mtsCallableVoidFunction.h>
#include <cisstMultiTask/mtsCommandVoid.h>
#include <cisstMultiTask/mtsCommandWrite.h>
#include <cisstMultiTask/mtsCommandQueuedVoid.h>
#include <cisstMultiTask/mtsCommandQueuedWrite.h>
#include <cisstMultiTask/mtsInterfaceCommon.h>

#include <cisstMultiTask/mtsFunctionBase.h>

// Always include last
#include <cisstMultiTask/mtsExport.h>

class mtsEventHandlerList;

/*!
  \file
  \brief Declaration of mtsInterfaceRequired
*/


/*!
  \ingroup cisstMultiTask

  This class implements the required interface for a component
  (mtsComponent, mtsTask, ...).  The required interface gets populated
  with pointers to command objects, which have four signatures:

  Void:           no parameters
  Read:           one non-const parameter
  Write:          one const parameter
  QualifiedRead:  one non-const (read) and one const (write) parameter

  The required interface may also have command object pointers for the
  following types of event handlers:

  Void:           no parameters
  Write:          one const parameter

  When the required interface of this component is connected to the
  provided interface of another component, the command object pointers
  are "bound" to command objects provided by the other component.
  Similarly, the event handlers are added as observers of events that
  are generated by the provided interface of the other component.

  This implementation is simpler than the provided interface because
  we assume that a required interface is never connected to more than
  one provided interface, whereas a provided interface can be used by
  multiple required interfaces.  While one can conceive of cases where
  it may be useful to have a required interface connect to multiple
  provided interfaces (e.g., running a robot simulation in parallel
  with a real robot), at this time it is not worth the trouble.
*/

class CISST_EXPORT mtsInterfaceRequired: public mtsInterface
{
    CMN_DECLARE_SERVICES(CMN_NO_DYNAMIC_CREATION, CMN_LOG_ALLOW_DEFAULT);

    friend class mtsComponentProxy;
    friend class mtsComponentInterfaceProxyClient;
    friend class mtsManagerLocal;
    friend class mtsManagerLocalTest;
    friend class mtsEventReceiverBase;
    friend class mtsManagerComponentClient;

 protected:

    /*! Indicates if the interface must be connected. */
    mtsRequiredType Required;

    /*! Mailbox (if supported). */
    mtsMailBox * MailBox;

    /*! Pointer to provided interface that we are connected to. */
    const mtsInterfaceProvided * InterfaceProvided;

    /*! Size to be used for mailboxes */
    size_t MailBoxSize;

    /*! Size to be used for argument queues */
    size_t ArgumentQueuesSize;

    /*! Default constructor. Does nothing, should not be used. */
    mtsInterfaceRequired(void);

    /*! Thread signal used for blocking calls.  It is shared between
      all functions */
    osaThreadSignal ThreadSignalForBlockingCommands;

    /*! For event receiver. */
    osaThreadSignal * GetThreadSignal(void);

 public:

    /*! Default size for mail boxes and argument queues used by event
      handlers. */
    enum {DEFAULT_MAIL_BOX_AND_ARGUMENT_QUEUES_SIZE = 64};

    /*! Constructor. Sets the name, device pointer, and mailbox for queued events.

      \param interfaceName Name of required interface

      \param mbox Mailbox to use for queued events (for tasks); set to
      0 for devices (i.e. mtsComponent) while tasks (derived from
      mtsTask) create a mailbox and then provide the mailbox to the
      required interface.

      \param isRequired Used to indicate if the component should
      function even if this interface is not connected to a provided
      interface.
    */
    mtsInterfaceRequired(const std::string & interfaceName, mtsComponent * component,
                         mtsMailBox * mailBox, mtsRequiredType required = MTS_REQUIRED);

    /*! Default destructor. */
    virtual ~mtsInterfaceRequired();

    const mtsInterfaceProvided * GetConnectedInterface(void) const;

    /*! Set the desired size for the event handlers mail box.  If
      queueing has been enabled for this interface, a single mailbox
      is created to handle all events.  Each write event handler
      manages it's own queue for the event argument.  To change the
      argument queue size, use SetArgumentQueuesSize.  To change both
      parameters at once, use SetMailBoxAndArgumentQueuesSize.

      The size of the mail box can't be changed while being used
      (i.e. while this required interface is connected to a provided
      interface. */
    bool SetMailBoxSize(size_t desiredSize);

    /*! Set the desired size for all argument queues.  If queueing has
      been enabled for this interface, each write event handler
      manages it's own queue of arguments.  The command itself is
      queued in the interface mailbox (see SetMailBoxSize) and the
      argument is queued by the command itself.  There is no reason to
      have an argument queue larger than the event handlers mail box
      as there can't be more arguments queued than events.  The
      reciprocal is not true as different events can be queued.  So,
      the argument queue size should be lesser or equal to the mail
      box size.

      The size of the mail box can't be changed while being used
      (i.e. while this required interface is connected to a provided
      interface. */
    bool SetArgumentQueuesSize(size_t desiredSize);

    /*! Set the desired size for the event handlers mail box and
      argument queues.  See SetMailBoxSize and
      SetArgumentQueuesSize. */
    bool SetMailBoxAndArgumentQueuesSize(size_t desiredSize);

    /*! Get the names of commands required by this interface. */
    //@{
    virtual std::vector<std::string> GetNamesOfFunctions(void) const;
    virtual std::vector<std::string> GetNamesOfFunctionsVoid(void) const;
    virtual std::vector<std::string> GetNamesOfFunctionsVoidReturn(void) const;
    virtual std::vector<std::string> GetNamesOfFunctionsWrite(void) const;
    virtual std::vector<std::string> GetNamesOfFunctionsWriteReturn(void) const;
    virtual std::vector<std::string> GetNamesOfFunctionsRead(void) const;
    virtual std::vector<std::string> GetNamesOfFunctionsQualifiedRead(void) const;
    //@}

    /*! Find a function based on its name. */
    //@{
    mtsFunctionVoid * GetFunctionVoid(const std::string & functionName) const;
    mtsFunctionVoidReturn * GetFunctionVoidReturn(const std::string & functionName) const;
    mtsFunctionWrite * GetFunctionWrite(const std::string & functionName) const;
    mtsFunctionWriteReturn * GetFunctionWriteReturn(const std::string & functionName) const;
    mtsFunctionRead * GetFunctionRead(const std::string & functionName) const;
    mtsFunctionQualifiedRead * GetFunctionQualifiedRead(const std::string & functionName) const;
    //@}

    /*! Get the names of event handlers that exist in this interface */
    //@{
    virtual std::vector<std::string> GetNamesOfEventHandlersVoid(void) const;
    virtual std::vector<std::string> GetNamesOfEventHandlersWrite(void) const;
    //@}

    /*! Find an event handler based on its name. */
    //@{
    virtual mtsCommandVoid * GetEventHandlerVoid(const std::string & eventName) const;
    virtual mtsCommandWriteBase * GetEventHandlerWrite(const std::string & eventName) const;
    //@}

    /* adeguet - seems deprecated or at least not used anywhere?
    inline bool CouldConnectTo(mtsInterfaceProvided * CMN_UNUSED(interfaceProvidedOrOutput)) {
        return true;
        } */

    bool ConnectTo(mtsInterfaceProvided * interfaceProvided);  // used by mtsManagerComponentClient.cpp
    //    bool Disconnect(void) { return DetachCommands(); }  // Should be deprecated -- adeguet1 OrOutput

    /*! Check if this interface is required or not for the component to function. */
    mtsRequiredType IsRequired(void) const;

    /*!
      \todo update documentation
      Bind command and events.  This method needs to provide a user
      Id so that GetCommandVoid and GetCommandWrite (queued
      commands) know which mailbox to use.  The user Id is provided
      by the provided interface when calling AllocateResources. */

    bool AddSystemEventHandlers(void);

 private:
    void BlockingCommandExecutedHandler(void);
    void BlockingCommandReturnExecutedHandler(void);

    bool BindCommands(const mtsInterfaceProvided * interfaceProvided);
    bool DetachCommands(void); // used by mtsManagerComponentClient

    void GetEventList(mtsEventHandlerList & eventList);
    bool CheckEventList(mtsEventHandlerList & eventList) const;
 public:

    void DisableAllEvents(void);

    void EnableAllEvents(void);

    /*! Process any queued events. */
    size_t ProcessMailBoxes(void);

    /*! Send a human readable description of the interface. */
    void ToStream(std::ostream & outputStream) const;

 protected:
 public: // adeguet1 todo fix -- this has been added for ostream << operator
#ifndef SWIG  // SWIG cannot deal with this
    template <class _PointerType>
        class FunctionOrReceiverInfo
    {
        // For GCM UI
        friend class mtsManagerLocal;
        friend class mtsInterfaceRequired;
    protected:
        _PointerType * Pointer;
        mtsRequiredType Required;
    public:
        FunctionOrReceiverInfo(_PointerType & func_or_recv, mtsRequiredType required):
            Pointer(&func_or_recv),
            Required(required)
        {}

        ~FunctionOrReceiverInfo() {}

        inline void Detach(void) {
            Pointer->Detach();
        }

        void ToStream(std::ostream & outputStream) const
        {
            outputStream << *Pointer;
            if (Required == MTS_OPTIONAL) {
                outputStream << " (optional)";
            } else {
                outputStream << " (required)";
            }
        }
    };

    typedef FunctionOrReceiverInfo<mtsFunctionBase> FunctionInfo;
    typedef FunctionOrReceiverInfo<mtsEventReceiverVoid> ReceiverVoidInfo;
    typedef FunctionOrReceiverInfo<mtsEventReceiverWrite> ReceiverWriteInfo;

#endif // !SWIG
 protected:

    /*! Utility method to determine if an event handler should be
      queued or not based on the default policy for the interface and
      the user's requested policy.  This method also generates a
      warning or error in the log if needed. */
    bool UseQueueBasedOnInterfacePolicy(mtsEventQueueingPolicy queueingPolicy,
                                        const std::string & methodName,
                                        const std::string & eventName);

    bool AddEventHandlerToReceiver(const std::string & eventName, mtsCommandVoid * handler) const;
    bool AddEventHandlerToReceiver(const std::string & eventName, mtsCommandWriteBase * handler) const;

    typedef cmnNamedMap<FunctionInfo> FunctionInfoMapType;

    /*! Typedef for a map of name of zero argument command and name of command. */
    FunctionInfoMapType FunctionsVoid; // Void (command)

    /*! Typedef for a map of name of zero argument command and name of command. */
    FunctionInfoMapType FunctionsVoidReturn; // Void return (command)

    /*! Typedef for a map of name of one argument command and name of command. */
    FunctionInfoMapType FunctionsWrite; // Write (command)

    /*! Typedef for a map of name of one argument command and name of command. */
    FunctionInfoMapType FunctionsWriteReturn; // Write return (command)

    /*! Typedef for a map of name of one argument command and name of command. */
    FunctionInfoMapType FunctionsRead; // Read (state read)

    /*! Typedef for a map of name of two argument command and name of command. */
    FunctionInfoMapType FunctionsQualifiedRead; // Qualified Read (conversion, read at time index, ...)

    typedef cmnNamedMap<ReceiverVoidInfo> EventReceiverVoidMapType;
    typedef cmnNamedMap<ReceiverWriteInfo> EventReceiverWriteMapType;

    /*! Typedef for a receiver of void events */
    EventReceiverVoidMapType EventReceiversVoid; // Void (event)

    /*! Typedef for a receiver of write events */
    EventReceiverWriteMapType EventReceiversWrite; // Write (event)

    /*! Typedef for a map of event name and event handler (command object). */
    typedef cmnNamedMap<mtsCommandVoid> EventHandlerVoidMapType;
    typedef cmnNamedMap<mtsCommandWriteBase> EventHandlerWriteMapType;
    EventHandlerVoidMapType EventHandlersVoid;
    EventHandlerWriteMapType EventHandlersWrite;

    /*! Get description of this interface (with serialized argument information) */
    void GetDescription(InterfaceRequiredDescription & requiredInterfaceDescription);

 public:

    bool AddFunction(const std::string & functionName, mtsFunctionVoid & function, mtsRequiredType required = MTS_REQUIRED);

    bool AddFunction(const std::string & functionName, mtsFunctionVoidReturn & function, mtsRequiredType required = MTS_REQUIRED);

    bool AddFunction(const std::string & functionName, mtsFunctionWrite & function, mtsRequiredType required = MTS_REQUIRED);

    bool AddFunction(const std::string & functionName, mtsFunctionWriteReturn & function, mtsRequiredType required = MTS_REQUIRED);

    bool AddFunction(const std::string & functionName, mtsFunctionRead & function, mtsRequiredType required = MTS_REQUIRED);

    bool AddFunction(const std::string & functionName, mtsFunctionQualifiedRead & function, mtsRequiredType required = MTS_REQUIRED);

    bool AddEventReceiver(const std::string & eventName, mtsEventReceiverVoid & receiver, mtsRequiredType required = MTS_REQUIRED);

    bool AddEventReceiver(const std::string & eventName, mtsEventReceiverWrite & receiver, mtsRequiredType required = MTS_REQUIRED);

    mtsCommandVoid * AddEventHandlerVoid(mtsCallableVoidBase * callable,
                                         const std::string & eventName,
                                         mtsEventQueueingPolicy queueingPolicy = MTS_INTERFACE_EVENT_POLICY);
    template <class __classType>
        inline mtsCommandVoid * AddEventHandlerVoid(void (__classType::*method)(void),
                                                    __classType * classInstantiation,
                                                    const std::string & eventName,
                                                    mtsEventQueueingPolicy queueingPolicy = MTS_INTERFACE_EVENT_POLICY) {
        mtsCallableVoidBase * callable = new mtsCallableVoidMethod<__classType>(method, classInstantiation);
        return this->AddEventHandlerVoid(callable, eventName, queueingPolicy);
    }

    inline mtsCommandVoid * AddEventHandlerVoid(void (*function)(void),
                                                const std::string & eventName,
                                                mtsEventQueueingPolicy queueingPolicy = MTS_INTERFACE_EVENT_POLICY) {
        mtsCallableVoidBase * callable = new mtsCallableVoidFunction(function);
        return this->AddEventHandlerVoid(callable, eventName, queueingPolicy);
    }

    template <class __classType, class __argumentType>
        inline mtsCommandWriteBase * AddEventHandlerWrite(void (__classType::*method)(const __argumentType &),
                                                          __classType * classInstantiation,
                                                          const std::string & eventName,
                                                          mtsEventQueueingPolicy queueingPolicy = MTS_INTERFACE_EVENT_POLICY);

    // PK: Can we get rid of this?
    template <class __classType>
        inline mtsCommandWriteBase * AddEventHandlerWriteGeneric(void (__classType::*method)(const mtsGenericObject &),
                                                                 __classType * classInstantiation,
                                                                 const std::string & eventName,
                                                                 mtsEventQueueingPolicy queueingPolicy = MTS_INTERFACE_EVENT_POLICY);

    bool RemoveEventHandlerVoid(const std::string & eventName);
    bool RemoveEventHandlerWrite(const std::string & eventName);
};


#ifndef SWIG
template <class __classType, class __argumentType>
inline mtsCommandWriteBase * mtsInterfaceRequired::AddEventHandlerWrite(void (__classType::*method)(const __argumentType &),
                                                                        __classType * classInstantiation,
                                                                        const std::string & eventName,
                                                                        mtsEventQueueingPolicy queueingPolicy)
{
    bool queued = this->UseQueueBasedOnInterfacePolicy(queueingPolicy, "AddEventHandlerWrite", eventName);
    mtsCommandWriteBase * actualCommand =
        new mtsCommandWrite<__classType, __argumentType>(method, classInstantiation, eventName, __argumentType());
    if (queued) {
        if (MailBox)
            EventHandlersWrite.AddItem(eventName,  new mtsCommandQueuedWrite<__argumentType>(MailBox, actualCommand, this->ArgumentQueuesSize));
        else
            CMN_LOG_CLASS_INIT_ERROR << "No mailbox for queued event handler write \"" << eventName << "\"" << std::endl;
    } else {
        EventHandlersWrite.AddItem(eventName, actualCommand);
    }
    mtsCommandWriteBase *handler = EventHandlersWrite.GetItem(eventName);
    AddEventHandlerToReceiver(eventName, handler);  // does nothing if event receiver does not exist
    return  handler;
}


template <class __classType>
inline mtsCommandWriteBase * mtsInterfaceRequired::AddEventHandlerWriteGeneric(void (__classType::*method)(const mtsGenericObject &),
                                                                               __classType * classInstantiation,
                                                                               const std::string & eventName,
                                                                               mtsEventQueueingPolicy queueingPolicy)
{
    bool queued = this->UseQueueBasedOnInterfacePolicy(queueingPolicy, "AddEventHandlerWriteGeneric", eventName);
    mtsCommandWriteBase * actualCommand =
        new mtsCommandWriteGeneric<__classType>(method, classInstantiation, eventName, 0);
    if (queued) {
        // PK: check for MailBox overlaps with code in UseQueueBasedOnInterfacePolicy
        if (MailBox) {
            EventHandlersWrite.AddItem(eventName,  new mtsCommandQueuedWriteGeneric(MailBox, actualCommand, this->ArgumentQueuesSize));
        } else {
            CMN_LOG_CLASS_INIT_ERROR << "No mailbox for queued event handler write generic \"" << eventName << "\"" << std::endl;
        }
    } else {
        EventHandlersWrite.AddItem(eventName, actualCommand);
    }
    mtsCommandWriteBase *handler = EventHandlersWrite.GetItem(eventName);
    AddEventHandlerToReceiver(eventName, handler);  // does nothing if event receiver does not exist
    return  handler;
}
#endif  // !SWIG


/*! Stream out operator. */
inline std::ostream & operator << (std::ostream & output,
                                   const mtsInterfaceRequired::FunctionInfo & functionInfo) {
    functionInfo.ToStream(output);
    return output;
}

CMN_DECLARE_SERVICES_INSTANTIATION(mtsInterfaceRequired)


#endif // _mtsInterfaceRequired_h

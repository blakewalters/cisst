/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: mtsQueueTest.cpp 1243 2010-02-28 05:33:17Z adeguet1 $
  
  Author(s):  Anton Deguet
  Created on: 2009-04-29
  
  (C) Copyright 2009 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include "mtsQueueTest.h"
#include "mtsMacrosTestClasses.h"
#include <cisstVector/vctRandom.h>

void mtsQueueTest::TestQueue_mtsDouble(void)
{
    // test default constructor
    mtsQueueGeneric queue;
    CPPUNIT_ASSERT(queue.GetSize() == 0);
    CPPUNIT_ASSERT(queue.GetAvailable() == 0);
    CPPUNIT_ASSERT(queue.IsEmpty());
    CPPUNIT_ASSERT(queue.IsFull());

    // test resize
    const mtsDouble original = 1.2345;
    const size_t size = 100;
    queue.SetSize(size, original);
    CPPUNIT_ASSERT(queue.GetSize() == size);
    CPPUNIT_ASSERT(queue.GetAvailable() == 0);
    CPPUNIT_ASSERT(queue.IsEmpty());
    CPPUNIT_ASSERT(!queue.IsFull());

    mtsDouble element;
    mtsGenericObject * basePointer;
    mtsDouble * retrieved;

    // test with one element, fill the queue at least twice to test circular buffer
    size_t index;
    for (index = 0; index < 3 * queue.GetSize(); index++) {
        element = index;
        queue.Put(element);
        CPPUNIT_ASSERT(queue.GetAvailable() == 1);
        basePointer = queue.Peek();
        CPPUNIT_ASSERT(basePointer);        
        CPPUNIT_ASSERT(queue.GetAvailable() == 1);
        retrieved = dynamic_cast<mtsDouble *>(basePointer);
        CPPUNIT_ASSERT(retrieved);
        CPPUNIT_ASSERT(retrieved->Data == index);
        basePointer = queue.Get();
        CPPUNIT_ASSERT(basePointer);
        CPPUNIT_ASSERT(queue.GetAvailable() == 0);
        retrieved = dynamic_cast<mtsDouble *>(basePointer);
        CPPUNIT_ASSERT(retrieved);
        CPPUNIT_ASSERT(retrieved->Data == index);
    }

    CPPUNIT_ASSERT(queue.IsEmpty());
    CPPUNIT_ASSERT(queue.GetAvailable() == 0);

    // test filling it up
    for (index = 0; index < queue.GetSize(); index++) {
        element = index;
        CPPUNIT_ASSERT(queue.Put(element));
        CPPUNIT_ASSERT(queue.GetAvailable() == index + 1);
        CPPUNIT_ASSERT(!queue.IsEmpty());
    }
    CPPUNIT_ASSERT(queue.IsFull());

    // test peek and empty
    for (index = 0; index < queue.GetSize(); index++) {
        // peek only
        basePointer = queue.Peek();
        CPPUNIT_ASSERT(basePointer);        
        CPPUNIT_ASSERT(queue.GetAvailable() == queue.GetSize() - index);
        retrieved = dynamic_cast<mtsDouble *>(basePointer);
        CPPUNIT_ASSERT(retrieved);
        CPPUNIT_ASSERT(retrieved->Data == index);
        // get element
        basePointer = queue.Get();
        CPPUNIT_ASSERT(basePointer);
        CPPUNIT_ASSERT(queue.GetAvailable() == queue.GetSize() - index - 1);
        retrieved = dynamic_cast<mtsDouble *>(basePointer);
        CPPUNIT_ASSERT(retrieved);
        CPPUNIT_ASSERT(retrieved->Data == index);
    }
    CPPUNIT_ASSERT(queue.IsEmpty());
}


void mtsQueueTest::TestConstructorDestructorCalls(void)
{
    // reset all counters
    mtsMacrosTestClassB::ResetAllCounters();
    mtsMacrosTestClassBProxy original;
    CPPUNIT_ASSERT(mtsMacrosTestClassB::DefaultConstructorCalls == 1);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::CopyConstructorCalls == 0);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::DestructorCalls == 0);

    // test default constructor
    mtsQueueGeneric * queue = new mtsQueueGeneric;

    // test constructor call when set size
    const size_t size = 100;
    size_t index;
    mtsMacrosTestClassB::ResetAllCounters();
    queue->SetSize(size, original);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::DefaultConstructorCalls == 0);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::CopyConstructorCalls == size + 1);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::DestructorCalls == 0);

    // test a couple of Put and Get
    mtsMacrosTestClassB::ResetAllCounters();
    for (index = 0; index < size; index++) {
        CPPUNIT_ASSERT(queue->Put(original));
    }
    mtsGenericObject * basePointer;
    for (index = 0; index < size; index++) {
        basePointer = queue->Get();
        CPPUNIT_ASSERT(basePointer);
    }
    CPPUNIT_ASSERT(mtsMacrosTestClassB::DefaultConstructorCalls == 0);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::CopyConstructorCalls == size);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::DestructorCalls == 0);

    // test resize
    mtsMacrosTestClassB::ResetAllCounters();
    queue->SetSize(size * 2, original);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::DefaultConstructorCalls == 0);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::CopyConstructorCalls == 2 * size + 1);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::DestructorCalls == size + 1);


    // test final delete
    mtsMacrosTestClassB::ResetAllCounters();
    delete queue;
    CPPUNIT_ASSERT(mtsMacrosTestClassB::DefaultConstructorCalls == 0);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::CopyConstructorCalls == 0);
    CPPUNIT_ASSERT(mtsMacrosTestClassB::DestructorCalls == 2 * size + 1);
}
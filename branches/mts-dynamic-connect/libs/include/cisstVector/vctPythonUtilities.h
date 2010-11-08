/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2005-08-21

  (C) Copyright 2005-2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/


/* This file is to be used only for the generation of SWIG wrappers.
   It includes all the regular header files from the libraries as well
   as some header files created only for the wrapping process
   (e.g. vctDynamicMatrixRotation3.h).

   For any wrapper using %import "cisstVector.i", the file
   cisstVector/vctPython.h should be included in the %header %{ ... %}
   section of the interface file. */


#ifndef _cisstVector_i_h
#define _cisstVector_i_h


/* Put header files here */
#include <Python.h>
#include <arrayobject.h>
#include <cisstCommon/cmnAssert.h>
#include <cisstCommon/cmnPortability.h>
#include <cisstVector/vctFixedSizeConstVectorBase.h>
#include <cisstVector/vctDynamicConstVectorBase.h>
#include <cisstVector/vctFixedSizeConstMatrixBase.h>
#include <cisstVector/vctDynamicConstMatrixBase.h>
#include <cisstVector/vctDynamicConstNArrayBase.h>
#include <cisstVector/vctTransformationTypes.h>


bool vctThrowUnlessIsPyArray(PyObject * input)
{
    if (!PyArray_Check(input)) {
        PyErr_SetString(PyExc_TypeError, "Object must be a NumPy array");
        return false;
    }
    return true;
}


template <class _elementType>
bool vctThrowUnlessIsSameTypeArray(PyObject * CMN_UNUSED(input))
{
    PyErr_SetString(PyExc_ValueError, "Unsupported data type");
    return false;
}


template <>
bool vctThrowUnlessIsSameTypeArray<bool>(PyObject * input)
{
    if (PyArray_ObjectType(input, 0) != NPY_BOOL) {
        PyErr_SetString(PyExc_ValueError, "Array must be of type bool");
        return false;
    }
    return true;
}


template <>
bool vctThrowUnlessIsSameTypeArray<char>(PyObject * input)
{
    if (PyArray_ObjectType(input, 0) != NPY_INT8) {
        PyErr_SetString(PyExc_ValueError, "Array must be of type char (int8)");
        return false;
    }
    return true;
}


template <>
bool vctThrowUnlessIsSameTypeArray<unsigned char>(PyObject * input)
{
    if (PyArray_ObjectType(input, 0) != NPY_UINT8) {
        PyErr_SetString(PyExc_ValueError, "Array must be of type unsigned char (uint8)");
        return false;
    }
    return true;
}


template <>
bool vctThrowUnlessIsSameTypeArray<short>(PyObject * input)
{
    if (PyArray_ObjectType(input, 0) != NPY_INT16) {
        PyErr_SetString(PyExc_ValueError, "Array must be of type short (int16)");
        return false;
    }
    return true;
}


template <>
bool vctThrowUnlessIsSameTypeArray<unsigned short>(PyObject * input)
{
    if (PyArray_ObjectType(input, 0) != NPY_UINT16) {
        PyErr_SetString(PyExc_ValueError, "Array must be of type unsigned short (uint16)");
        return false;
    }
    return true;
}


template <>
bool vctThrowUnlessIsSameTypeArray<int>(PyObject * input)
{
    if (PyArray_ObjectType(input, 0) != NPY_INT32) {
        PyErr_SetString(PyExc_ValueError, "Array must be of type int (int32)");
        return false;
    }
    return true;
}


template <>
bool vctThrowUnlessIsSameTypeArray<unsigned int>(PyObject * input)
{
    if (PyArray_ObjectType(input, 0) != NPY_UINT32) {
        PyErr_SetString(PyExc_ValueError, "Array must be of type unsigned int (uint32)");
        return false;
    }
    return true;
}


template <>
bool vctThrowUnlessIsSameTypeArray<long int>(PyObject * input)
{
    if (PyArray_ObjectType(input, 0) != NPY_INT64) {
        PyErr_SetString(PyExc_ValueError, "Array must be of type long int (int64)");
        return false;
    }
    return true;
}


template <>
bool vctThrowUnlessIsSameTypeArray<unsigned long int>(PyObject * input)
{
    if (PyArray_ObjectType(input, 0) != NPY_UINT64) {
        PyErr_SetString(PyExc_ValueError, "Array must be of type unsigned long int (uint64)");
        return false;
    }
    return true;
}


template <>
bool vctThrowUnlessIsSameTypeArray<double>(PyObject * input)
{
    if (PyArray_ObjectType(input, 0) != NPY_DOUBLE) {
        PyErr_SetString(PyExc_ValueError, "Array must be of type double ()");
        return false;
    }
    return true;
}


template <class _elementType>
int vctPythonType(void)
{
    return NPY_NOTYPE; // unsupported type
}


template <>
int vctPythonType<bool>(void)
{
    return NPY_BOOL;
}


template <>
int vctPythonType<char>(void)
{
    return NPY_INT8;
}


template <>
int vctPythonType<unsigned char>(void)
{
    return NPY_UINT8;
}


template <>
int vctPythonType<short>(void)
{
    return NPY_INT16;
}


template <>
int vctPythonType<unsigned short>(void)
{
    return NPY_UINT16;
}


template <>
int vctPythonType<int>(void)
{
    return NPY_INT32;
}


template <>
int vctPythonType<unsigned int>(void)
{
    return NPY_UINT32;
}


template <>
int vctPythonType<long int>(void)
{
    return NPY_INT64;
}


template <>
int vctPythonType<unsigned long int>(void)
{
    return NPY_UINT64;
}


template <>
int vctPythonType<double>(void)
{
    return NPY_DOUBLE;
}


bool vctThrowUnlessDimension1(PyObject * input)
{
    if (PyArray_NDIM(input) != 1) {
        PyErr_SetString(PyExc_ValueError, "Array must be 1D (vector)");
        return false;
    }
    return true;
}


bool vctThrowUnlessDimension2(PyObject * input)
{
    if (PyArray_NDIM(input) != 2) {
        PyErr_SetString(PyExc_ValueError, "Array must be 2D (matrix)");
        return false;
    }
    return true;
}


template <vct::size_type _dimension>
bool vctThrowUnlessDimensionN(PyObject * input)
{
    if (PyArray_NDIM(input) != _dimension) {
        std::stringstream stream;
        stream << "Array must have " << _dimension << " dimension(s)";
        std::string msg = stream.str();
        PyErr_SetString(PyExc_ValueError, msg.c_str());
        return false;
    }
    return true;
}


bool vctThrowUnlessIsWritable(PyObject * input)
{
    const int flags = PyArray_FLAGS(input);
    if(!(flags & NPY_WRITEABLE)) {
        PyErr_SetString(PyExc_ValueError, "Array must be writable");
        return false;
    }
    return true;
}


template <vct::size_type _size, vct::stride_type _stride, class _elementType, class _dataPtrType>
bool vctThrowUnlessCorrectVectorSize(PyObject * input,
                                     const vctFixedSizeConstVectorBase<_size, _stride, _elementType, _dataPtrType> & target)
{
    const vct::size_type inputSize = PyArray_DIM(input, 0);
    const vct::size_type targetSize = target.size();
    if (inputSize != targetSize) {
        std::stringstream stream;
        stream << "Input vector's size must be " << targetSize;
        std::string msg = stream.str();
        PyErr_SetString(PyExc_ValueError, msg.c_str());
        return false;
    }
    return true;
}


template <class _vectorOwnerType, typename _elementType>
bool vctThrowUnlessCorrectVectorSize(PyObject * CMN_UNUSED(input),
                                     const vctDynamicConstVectorBase<_vectorOwnerType, _elementType> & CMN_UNUSED(target))
{
    return true;
}


template <vct::size_type _rows, vct::size_type _cols,
          vct::stride_type _rowStride, vct::stride_type _colStride,
          class _elementType, class _dataPtrType>
bool vctThrowUnlessCorrectMatrixSize(PyObject * input,
                                     const vctFixedSizeConstMatrixBase<_rows, _cols, _rowStride, _colStride, _elementType, _dataPtrType> & target)
{
    const vct::size_type inputRows = PyArray_DIM(input, 0);
    const vct::size_type inputCols = PyArray_DIM(input, 1);
    const vct::size_type targetRows = target.rows();
    const vct::size_type targetCols = target.cols();
    if (   inputRows != targetRows
        || inputCols != targetCols) {
        std::stringstream stream;
        stream << "Input matrix's size must be " << targetRows << " rows by " << targetCols << " columns";
        std::string msg = stream.str();
        PyErr_SetString(PyExc_ValueError, msg.c_str());
        return false;
    }
    return true;
}


template <class _matrixOwnerType, typename _elementType>
bool vctThrowUnlessCorrectMatrixSize(PyObject * CMN_UNUSED(input),
                                     const vctDynamicConstMatrixBase<_matrixOwnerType, _elementType> & CMN_UNUSED(target))
{
    return true;
}


bool vctThrowUnlessOwnsData(PyObject * input)
{
    const int flags = PyArray_FLAGS(input);
    if(!(flags & NPY_OWNDATA)) {
        PyErr_SetString(PyExc_ValueError, "Array must own its data");
        return false;
    }
    return true;
}


template <vct::size_type _size, vct::stride_type _stride, class _elementType, class _dataPtrType>
bool vctThrowUnlessOwnsData(PyObject * CMN_UNUSED(input),
                            const vctFixedSizeConstVectorBase<_size, _stride, _elementType, _dataPtrType> & CMN_UNUSED(target))
{
    return true;
}


template <class _vectorOwnerType, typename _elementType>
bool vctThrowUnlessOwnsData(PyObject * input,
                            const vctDynamicConstVectorBase<_vectorOwnerType, _elementType> & CMN_UNUSED(target))
{
    const int flags = PyArray_FLAGS(input);
    if(!(flags & NPY_OWNDATA)) {
        PyErr_SetString(PyExc_ValueError, "Array must own its data");
        return false;
    }
    return true;
}


template <vct::size_type _rows, vct::size_type _cols,
	  vct::stride_type _rowStride, vct::stride_type _colStride,
	  class _elementType, class _dataPtrType>
bool vctThrowUnlessOwnsData(PyObject * CMN_UNUSED(input),
                            const vctFixedSizeConstMatrixBase<_rows, _cols, _rowStride, _colStride, _elementType, _dataPtrType> & CMN_UNUSED(target))
{
    return true;
}


template <class _matrixOwnerType, typename _elementType>
bool vctThrowUnlessOwnsData(PyObject * input,
                            const vctDynamicConstMatrixBase<_matrixOwnerType, _elementType> & CMN_UNUSED(target))
{
    const int flags = PyArray_FLAGS(input);
    if(!(flags & NPY_OWNDATA)) {
        PyErr_SetString(PyExc_ValueError, "Array must own its data");
        return false;
    }
    return true;
}


bool vctThrowUnlessNotReferenced(PyObject * input)
{
    if (PyArray_REFCOUNT(input) > 4) {
        PyErr_SetString(PyExc_ValueError, "Array must not be referenced by other objects.  Try making a deep copy of the array and call the function again.");
        return false;
    }
    return true;
}


template <vct::size_type _size, vct::stride_type _stride, class _elementType, class _dataPtrType>
bool vctThrowUnlessNotReferenced(PyObject * CMN_UNUSED(input),
                                 const vctFixedSizeConstVectorBase<_size, _stride, _elementType, _dataPtrType> & CMN_UNUSED(target))
{
    return true;
}


template <class _vectorOwnerType, typename _elementType>
bool vctThrowUnlessNotReferenced(PyObject * input,
                                 const vctDynamicConstVectorBase<_vectorOwnerType, _elementType> & CMN_UNUSED(target))
{
    if (PyArray_REFCOUNT(input) > 4) {
        PyErr_SetString(PyExc_ValueError, "Array must not be referenced by other objects.  Try making a deep copy of the array and call the function again.");
        return false;
    }
    return true;
}


template <vct::size_type _rows, vct::size_type _cols,
	  vct::stride_type _rowStride, vct::stride_type _colStride,
	  class _elementType, class _dataPtrType>
bool vctThrowUnlessNotReferenced(PyObject * CMN_UNUSED(input),
                                 const vctFixedSizeConstMatrixBase<_rows, _cols, _rowStride, _colStride, _elementType, _dataPtrType> & CMN_UNUSED(target))
{
    return true;
}


template <class _matrixOwnerType, typename _elementType>
bool vctThrowUnlessNotReferenced(PyObject * input,
                                 const vctDynamicConstMatrixBase<_matrixOwnerType, _elementType> & CMN_UNUSED(target))
{
    if (PyArray_REFCOUNT(input) > 4) {
        PyErr_SetString(PyExc_ValueError, "Array must not be referenced by other objects.  Try making a deep copy of the array and call the function again.");
        return false;
    }
    return true;
}



#endif // _cisstVector_i_h

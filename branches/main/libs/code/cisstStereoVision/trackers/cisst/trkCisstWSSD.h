/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: trkCisstWSSD.h,v 1.3 2007/09/27 00:14:03 vagvoba Exp $

  Author(s):  Maneesh Dewan (?)
  Created on: 2007 

  (C) Copyright 2006-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/


#pragma once
/*
	WeightedSSD tracker class based on the cisst library.
	Should be trivial to extend this to Siemens Data Type.
*/
#ifdef INCL_OPENCV
#include <cv.h>
#endif

#include "cisstCommon.h"
#include <cisstConfig.h>
#include <cisstVector.h>
#include <cisstNumerical.h>

#include <vector>
#include <iostream>

#include "trkCisstBase.h"
#include "vctWarpFunctionMatrix.h"
#include "vctExtraTemplateFunctions.h"

using namespace std;

#define IS_LOGGING

enum trkCisstWSSDModel
{
    TRANS,
    TRANS_ROT,
    TRANS_SCALE,
    TRANS_ROT_SCALE
};

class trkCisstWSSD : public trkCisstBase
{
public:
#ifdef INCL_OPENCV
	//Incomming Data format, should be different from MatrixType, otherwise
	//eleminate copyImage functions.
	typedef IplImage* ImageType;
#endif

	// Data type for image to interface with Siemens Radbuilder and ICE programs
	typedef unsigned short InterfaceImType;

	//Basic numeric type for processing, should be a real type
	typedef double RealType;
	//A matrix data type
	typedef vctDynamicMatrix < RealType > MatrixType;
	//A vector data Ref type
	typedef vctDynamicVector < RealType > VectorType;
	//A vector data Ref type
	typedef vctDynamicVectorRef < RealType > VectorRefType;
	//Datatype for indexes
	typedef MatrixType::index_type IndexType;
	//Submatrix Type
	typedef MatrixType::Submatrix::Type SubMatrixType;
	//Matrix data, if different from a Matrix view
	typedef vector < MatrixType > MatrixList;
	//Matrix data, if different from a Matrix view
	typedef vector < MatrixType * > MatrixRefList;

	//2D point data type for r,c
	typedef vctFixedSizeVector < IndexType , 2> PointType;
	//2D point data type for r,c
	typedef vctFixedSizeVector < RealType , 2> fPointType;
	//The type of data access we want
	typedef vctDynamicFunctionalMatrixBase< vctWarpFunctionMatrixOwner<MatrixType> > WarpedMatrixType;

public:

	//Constructor/Destructor
	trkCisstWSSD(trkCisstWSSDModel trackmodel = TRANS);
	~trkCisstWSSD(void);

	//Cisst Library Test Routines
	//void test();

	//Offine initialization routines
	
    //Select the number of templates to search through, 
	//should be at least 2 for multi template use
	//Defaults to all if not set
	void setTemplateLookahead(IndexType number);
	//Set the size of the template. 
	//All templates must be the same size.
	void setTemplateSize(IndexType rows, IndexType cols);
	//Set the center offset of the template.
	void setTemplateCenter(IndexType r, IndexType c);
	//Set the tracking window size
	inline void setWindowSize(IndexType CMN_UNUSED(rows), IndexType CMN_UNUSED(cols)){};
	//Set the center offset of the tracking window
	inline void setWindowCenter(IndexType CMN_UNUSED(r), IndexType CMN_UNUSED(c)){};
	//Set the initial position of the tracking window
	inline void setWindowPosition(IndexType CMN_UNUSED(r), IndexType CMN_UNUSED(c)){};
	// Set the initial position for tracking 
	void setInitPosition(IndexType r, IndexType c);
	// Set the image size field
	void setImageSize(IndexType rows, IndexType cols);
#ifdef INCL_OPENCV
	//Add another template to the tracker (uses opencv)
	void pushTemplate_opencv(ImageType &src, IndexType r, IndexType c);
#endif
	//Add another template to the tracker 
	void pushTemplate(InterfaceImType *src, IndexType src_rows, IndexType src_cols, IndexType r, IndexType c);
	//Add another template to the tracker 
	void pushTemplate(InterfaceImType *src, IndexType src_rows, IndexType src_cols);
	//Remove last template from the tracker
	void popTemplate();
	//Get the number of templates loaded
	//Explicit unsigned int required??
	IndexType getNumberOfTemplates();
#ifdef INCL_OPENCV
	//Copy a choosen template to an image (uses opencv)
	void getTemplate_opencv(IndexType index, ImageType &dst);
#endif
	//Copy a choosen template to an image 
	//void getTemplate(IndexType index, MatrixType &dst);
	//Add a loaded template to the end of the selected templates list
	void pushSelectedTemplate(IndexType index);
	//Remove the last selected template
	void popSelectedTemplate();
	// Set the size of the current image
	void setCurrentimageSize(IndexType rows, IndexType cols);
	// Copy the current image
	void copyCurrentImage(InterfaceImType *src, IndexType src_rows, IndexType src_cols);
	
	//Inititalize the pre-allocated data for tracking
	void initializeTrack();

	//Online tracking routines and feedback
#ifdef INCL_OPENCV
	//Update the current window position (uses opencv)
	void updateTrack_opencv(ImageType &  src);
#endif
	//Update the current window position
	void updateTrack();
	//Get image location
	IndexType getWindowRow();
	IndexType getWindowCol();
	//Get the correlation score, bounded [-1,1]	
	RealType getTrackScore();

	//Other useful functions

	//Cear stored data.
	void resetTrack();
	//Get template dimentions
	IndexType getTemplateNumRows();
	IndexType getTemplateNumCols();
	//Get template center location
	IndexType getTemplateCenterRow();
	IndexType getTemplateCenterCol();
	//Get the current image size
	IndexType getCurrentimageNumRows();
	IndexType getCurrentimageNumCols();
	// Get the current (tracked) location 
	RealType getOutputPosX();
	RealType getOutputPosY();
	//New interface commands
	//Set the termination threshold
	void setThreshold(RealType thres);

protected:
	//Initialization routines
	void computeXderivative(MatrixType &src, MatrixType &dst);	
	void computeYderivative(MatrixType &src, MatrixType &dst);
	void computeXderivative_filtered(MatrixType &src, MatrixType &dst);	
	void computeYderivative_filtered(MatrixType &src, MatrixType &dst);
	void computeXYvalues(MatrixType &x_vals, MatrixType &y_vals);
	void computeTemplateDerivative(IndexType index);
	void computeCurrentDerivative();
	void copyCurrentRegion();
	void updateParameters();
	//Other helper routines
#ifdef INCL_OPENCV
	//Copy a input image into buffer matrix (uses opencv)
	void copyImage_opencv(ImageType &src, MatrixType &dst, IndexType startR = 0, IndexType startC = 0);
		//Copy a buffered image back to an input image (uses opencv)
	void copyImage_opencv(MatrixType &src, ImageType &dst);
#endif
	//Copy a input image into buffer matrix
	void copyImage(InterfaceImType *src, IndexType src_rows, IndexType src_cols, MatrixType &dst, IndexType startR = 0, IndexType startC = 0);
	//Copy a input image into buffer matrix (within cisst)
	void copyImage_cisst(MatrixType &src, MatrixType &dst, IndexType startR = 0, IndexType startC = 0);

private:
	
	trkCisstWSSDModel trackmodelused;
	// Current image 
	MatrixType Current_image;
	//Storage of all the choosen templates, normalized, and the same dimentions
	MatrixRefList allTemplates; //
	//Order list of the templates selected, standard or FT domain
	MatrixRefList selectedTemplates; //
	// Order list of derivatives of selected templates
	MatrixRefList derTemplates;
	// current region
	MatrixType current_region;
	// derivative of the current region
	MatrixType derCurrent_region;

	MatrixType R_mat;
	MatrixType Q_mat;
	MatrixType Diff_mat;
	VectorRefType Diff_vec;
	MatrixType A_lsq;
	VectorType B_lsq;
	VectorType X_lsq;

	// Filtering matrices
	MatrixType X_der_filter;
	MatrixType Y_der_filter;
	IndexType der_filter_offset;

	IndexType numIter;
	
	//The index of next template
	IndexType currentTemplateIndex;//
	//The number of templates to search per update
	IndexType templateLookahead;

	//The parameter set
	VectorType C;
	//The warpedMatrix view
	WarpedMatrixType WarpedMatrix;

	//Scratch copy of the image, used in update
	MatrixType imageCopy;
	
	// Threshold value
	RealType threshold;
	//Last similarity measure
	RealType score;
	//The initial position in the 1st frame
	PointType initPos;//
	//X = cols, Y = rows
	//Size of all the templates.
	PointType templateSize;//
	//The "center" of the template for convolution purposes
	fPointType templateCenter;//
	// Tracked position in the current image
	fPointType CurLoc;
	// Rotation and Scale
	RealType CurAngle;
	RealType CurScale;
	// Image size (X corresponds to height and Y to width)
	PointType imageSize;
	// Output Location
	RealType outputPosX;
	RealType outputPosY;
};


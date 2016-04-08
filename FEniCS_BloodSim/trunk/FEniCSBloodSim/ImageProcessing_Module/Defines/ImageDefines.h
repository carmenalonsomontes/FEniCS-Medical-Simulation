#ifndef IMAGEDEFINES_H
#define IMAGEDEFINES_H

using namespace std;

#include "itkImageToVTKImageFilter.h"
#include "itkImage.h"

/*********************** IMAGE 3D & 2d ************************/
// Typical defines for Image 3D and 2D slices

typedef itk::Image< double, 3 >         ImageType;
typedef unsigned short PixelType;
typedef itk::Image<PixelType,2> ImageType2D;
typedef itk::Image<PixelType,3> ImageType3D;

// To read  & Write images in 2D and 3D

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

typedef itk::ImageFileReader< ImageType3D > ReaderType;
typedef itk::ImageFileWriter< ImageType2D > WriterType;

/*********************** Iterators ****************************/

// Iterators

#include "itkImageSliceConstIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"
typedef itk::ImageLinearIteratorWithIndex<ImageType2D> LinearIteratorType;

/** CONNECTORS */
typedef itk::ImageToVTKImageFilter<ImageType>       ConnectorType;

// DIRECTIONS

enum DIR_3D{
    X_DIRECTION = 0,
    Y_DIRECTION = 1,
    Z_DIRECTION = 2

};


#endif // IMAGEDEFINES_H

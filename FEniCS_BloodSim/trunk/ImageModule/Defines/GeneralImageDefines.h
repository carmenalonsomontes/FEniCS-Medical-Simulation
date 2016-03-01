#ifndef GENERALIMAGEDEFINES_H
#define GENERALIMAGEDEFINES_H

/*********************** IMAGE 3D & 2d ************************/
// Typical defines for Image 3D and 2D slices
#include "itkImage.h"
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
typedef itk::ImageSliceConstIteratorWithIndex<ImageType3D> SliceIteratorType;

#endif // GENERALIMAGEDEFINES_H

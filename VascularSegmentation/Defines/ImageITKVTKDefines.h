#ifndef IMAGEITKVTKDEFINES_H
#define IMAGEITKVTKDEFINES_H


/* ================================================ */
// Filters

enum TypeFilters{
    MEDIAN_FILTER  = 0,
    NO_FILTER_FOUND = -1
};

/* ================================================ */
/*                  ITK                             */
// General defines needed for those classes using ITK
#include <iostream>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

using namespace std;
using namespace itk;

enum ImageDimension{
    Two_Dimensions = 2,
    Three_Dimensions = 3
};

typedef Image<unsigned char, Two_Dimensions>  ImageType2D;
typedef Image<unsigned char, Three_Dimensions>  ImageType3D;

typedef Image<double, Two_Dimensions>  ImageType2DDouble;
typedef Image<double, Three_Dimensions>  ImageType3DDouble;

typedef Image<float, Two_Dimensions>  ImageType2DFloat;
typedef Image<float, Three_Dimensions>  ImageType3DFloat;


typedef ImageFileReader<ImageType2D> ReaderType2D;
typedef ImageFileWriter<ImageType2D> WriterType2D;


typedef ImageFileReader<ImageType3D> ReaderType3D;
typedef ImageFileWriter<ImageType3D> WriterType3D;

/* ================================================ */

/* ================================================ */
/*                  VTK                             */
// General defines needed for those classes using VTK - GLUE

#include <itkVTKImageToImageFilter.h>
typedef itk::VTKImageToImageFilter<ImageType2D> VTKImageToImage2DType;
typedef itk::VTKImageToImageFilter<ImageType3D> VTKImageToImage3DType;



#endif // IMAGEITKVTKDEFINES_H

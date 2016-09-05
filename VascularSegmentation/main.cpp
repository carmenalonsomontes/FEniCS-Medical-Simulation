#include <iostream>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <string>

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"


using namespace std;
#include "Defines/ImageITKVTKDefines.h"


#include "itkSimpleFilterWatcher.h"
#include "itkMedianImageFilter.h"
#include "itkSliceBySliceImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"

// Filters
//typedef SliceBySliceImageFilter<ImageType2D,ImageType2D> FilterType;
//typedef MedianImageFilter<FilterType::InternalInputImageType,FilterType::InternalOutputImageType> MedianType;

#include "itkMaskImageFilter.h"

#include "itkStripTsImageFilter.h"
#include <itkBinaryBallStructuringElement.h>


#include "itkAdaptiveHistogramEqualizationImageFilter.h"
#include <itkWhiteTopHatImageFilter.h>

// ANISOTROPIC DIFFUSION
#include "itkGradientAnisotropicDiffusionImageFilter.h"

//EROSION
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

// Hole Filter

#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include "itkBinaryFillholeImageFilter.h"

// Opening
#include "itkBinaryShapeOpeningImageFilter.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkBinaryMorphologicalClosingImageFilter.h"

  #include<sstream>

#include <itkNiftiImageIO.h>

/*************************************************/
/* B. Skull strip */
/*************************************************/
void skullStrip(string _inputFileName,string _outputFileName)
{

    // opcion seleccionada --> BET
    string command = "bet "+ _inputFileName + " "+ _outputFileName;
    std::system(command.c_str());


}


int saveNiiFormat(string _inputFileName,string _outputFileName)
{
    //typedef short      PixelType;
    const   unsigned int        Dimension = 3;
    typedef itk::Image< PixelType, Dimension >    ImageType;


    typedef itk::ImageFileReader< ImageType >  ReaderType;
    typedef itk::ImageFileWriter< ImageType >  WriterType;
    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();

    reader->SetFileName( _inputFileName  );
    writer->SetFileName( _outputFileName );
     writer->SetInput( reader->GetOutput() );
     try
     {
     writer->Update();
     }
   catch( itk::ExceptionObject & err )
     {
     std::cerr << "ExceptionObject caught !" << std::endl;
     std::cerr << err << std::endl;
     return EXIT_FAILURE;
     }


}

int main()
{
double startTime = time(0);
    /*************************************************/
    // SMART POINTERS DECLARATION & INITIALIZATION

    //FilterType::Pointer filter = FilterType::New();

    string _basePath = "/home/cmontes/ToolDevelopment/BloodSimulations/KTH_project/DATA_test/";
    string _fileBaseName = "Normal00";
    string _fileBin ="BIN00";
    string _fileNoSkull = "NoSkull00";
    string _fileSuffixName = "-MRA.mha";
    int _noTotalIm = 7;






    for (int _noFile = 1;_noFile<= _noTotalIm;_noFile++)
    {
        // Declaring the input file name and the output file names
        std::ostringstream os ;
        os << _noFile;
        string _cFileName = _basePath + _fileBaseName + os.str() + _fileSuffixName;
        string _cFileOutputNoSkullName =  _basePath + _fileNoSkull + os.str() + ".nii.gz";
        string _cFileOutputBINName =  _basePath + _fileBin + os.str() + _fileSuffixName;
        string _cFileOutputNIFTIName =  _basePath + _fileBaseName + os.str() + ".nii.gz";


        // Step 1. - Save as NII Format, skull strip

        saveNiiFormat(_cFileName,_cFileOutputNIFTIName);
        skullStrip(_cFileOutputNIFTIName,_cFileOutputNoSkullName);

        /***************************************************************************/
        /* A. READING THE IMAGES                                                   */
        /***************************************************************************/

        // Reading original image
   /*     ReaderType3D::Pointer origImageReader = ReaderType3D::New();
        ImageType3D::ConstPointer inputImage;

        origImageReader->SetFileName(_cFileName);
        try
        {
          origImageReader->Update();
          inputImage = origImageReader->GetOutput();
        }
        catch ( itk::ExceptionObject &err)
        {
          std::cerr << "[READ-IMAGE] ExceptionObject caught !" << std::endl;
          std::cerr << err << std::endl;
          return EXIT_FAILURE;
        }*/





      //  skullStrip(origImageReader, _cFileOutputNoSkullName,_cFileOutputBINName);
        /***************************************************************************/
        /* B. IMAGE ENHANCEMENT - NOISE DIFFUSION - ANISIOTROPIC DIFFUSION         */
        /***************************************************************************/

        // Step 1. Enhance the image

  /*      int _noIterations = 5;
        int _timeStep = 0.05;
        int _conductanceParameter = 1.0;


        typedef itk::GradientAnisotropicDiffusionImageFilter< ImageType2D,ImageType2D > EnhancementFilterType;
         EnhancementFilterType::Pointer enhancementFilter = EnhancementFilterType::New();
         enhancementFilter->SetNumberOfIterations(_noIterations);
         enhancementFilter->SetTimeStep(_timeStep);
         enhancementFilter->SetConductanceParameter(_conductanceParameter);
         */

         /***************************************************************************/
         /* C. BINARIZATION                                                         */
         /***************************************************************************/

 /*        int _lowerThreshold = 100;
         int _upperThreshold = 255;
         int _foregroundPixelValue = 255;
         int _outsideValue = 0;


         typedef itk::BinaryThresholdImageFilter <ImageType2D,ImageType2D> BinaryThresholdImageFilterType;
         BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();

         thresholdFilter->SetLowerThreshold(_lowerThreshold);
         thresholdFilter->SetUpperThreshold(_upperThreshold);
         thresholdFilter->SetInsideValue(_foregroundPixelValue);
         thresholdFilter->SetOutsideValue(_outsideValue);
*/
         /***************************************************************************/
         /* D. DILATION                                                           */
         /***************************************************************************/

 /*        int _kernelSize = 10;
        // Defining the structuring element
         typedef itk::BinaryBallStructuringElement<ImageType2D::PixelType, 2>   StructuringElementType;
         StructuringElementType structuringElement;
         structuringElement.SetRadius(_kernelSize); // if 1 == >3x3 matrix
         structuringElement.CreateStructuringElement();

          typedef itk::BinaryDilateImageFilter <ImageType2D, ImageType2D, StructuringElementType>   BinaryDilateImageFilterType;

         BinaryDilateImageFilterType::Pointer dilateFilter= BinaryDilateImageFilterType::New();
         dilateFilter->SetKernel(structuringElement);
         dilateFilter->SetDilateValue(_foregroundPixelValue); // This must be the same value as foreground value
*/

         /***************************************************************************/
         /* D. EROSION                                                           */
         /***************************************************************************/

/*         typedef itk::BinaryErodeImageFilter <ImageType2D, ImageType2D, StructuringElementType>  BinaryErodeImageFilterType;

         BinaryErodeImageFilterType::Pointer erodeFilter= BinaryErodeImageFilterType::New();
         erodeFilter->SetKernel(structuringElement);
         erodeFilter->SetErodeValue(_foregroundPixelValue);
*/
         /***************************************************************************/
         /* D. HOLE FILLING                                                         */
         /***************************************************************************/


         // Hole filter definition
    /*     typedef itk::BinaryFillholeImageFilter< ImageType2D > HoleFillingFilterType;
         HoleFillingFilterType::Pointer holeFilling = HoleFillingFilterType::New();
         holeFilling->SetFullyConnected( false );
         holeFilling->SetForegroundValue( _foregroundPixelValue );
*/



         /***************************************************************************/
         /* E. SLICE BY SLICE FILTERING                                             */
         /***************************************************************************/
/*         typedef itk::SliceBySliceImageFilter< ImageType3D, ImageType3D > FilterType;
         FilterType::Pointer filter = FilterType::New();
         filter->SetInput( origImageReader->GetOutput() );


         // PIPELIN CONFIGURATION
         // Linking the filters
         thresholdFilter->SetInput(enhancementFilter->GetOutput());
         dilateFilter->SetInput(thresholdFilter->GetOutput());
         erodeFilter->SetInput(dilateFilter->GetOutput());
         //holeFilling->SetInput(erodeFilter->GetOutput());

         // PASSING PIPELINE TO SLICE BY SLICE
         filter->SetFilter(enhancementFilter);
         filter->SetOutputFilter(erodeFilter);
*/

         /***************************************************************************/
         /* E. SAVING THE REULTS                                                     */
         /***************************************************************************/

  /*       typedef  itk::ImageFileWriter< ImageType3D  > WriterType;
         WriterType::Pointer writer = WriterType::New();
         try
         {
             writer->SetFileName(_cFileOutputBINName);
             writer->SetInput(filter->GetOutput());
             writer->Update();
         }
         catch ( itk::ExceptionObject &err)
         {
           std::cerr << "[WRITE-IMAGE] ExceptionObject caught !" << std::endl;
           std::cerr << err << std::endl;
           return EXIT_FAILURE;
         }
*/
         /***************************************************************************/
         /* F. SKULL STRIPTING                                                      */
         /***************************************************************************/

      /*   string _cROBEXPATH = "/home/cmontes/ToolDevelopment/BloodSimulations/KTH_project/ROBEX/";
         string _ROBEXCommand = "./runROBEX.sh ";
         string _commandROBEX = "cd "+_cROBEXPATH+";"+_ROBEXCommand+_cFileOutputBINName+" "+_cFileOutputNoSkullName;


         std::system(_commandROBEX.c_str());*/

    }

    cout << "DONE!" << endl;
    return EXIT_SUCCESS;
}


/* HISTOGRAM EQ -->DOES NOT WORK
typedef  itk::AdaptiveHistogramEqualizationImageFilter< ImageType2D > AdaptiveHistogramEqualizationImageFilterType;
AdaptiveHistogramEqualizationImageFilterType::Pointer adaptiveHistogramEqualizationImageFilter = AdaptiveHistogramEqualizationImageFilterType::New();
adaptiveHistogramEqualizationImageFilter->SetRadius(1);

filter->SetFilter(adaptiveHistogramEqualizationImageFilter);
*/

 /* MEDIAN FILTER
 typedef itk::MedianImageFilter< FilterType::InternalInputImageType,FilterType::InternalOutputImageType > MedianType;
    MedianType::Pointer median = MedianType::New();
    MedianType::InputSizeType rad;
    rad.Fill( 5 );
    median->SetRadius( rad );

    filter->SetFilter( median );

*/

/*************************************************/
/* Filtering */
/*************************************************/
/*  typedef itk::BinaryThresholdImageFilter <ImageType2D,ImageType2D> BinaryThresholdImageFilterType;
BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();

    thresholdFilter->SetInput(reader ->GetOutput());
    thresholdFilter->SetLowerThreshold(125);
    thresholdFilter->SetUpperThreshold(200);
    thresholdFilter->SetInsideValue(255);
    thresholdFilter->SetOutsideValue(0);

    thresholdFilter->Update();


*/

/*   typedef itk::SliceBySliceImageFilter< ImageType3D, ImageType3D > FilterType;
FilterType::Pointer filter = FilterType::New();
filter->SetInput( reader->GetOutput() );


typedef itk::MedianImageFilter< FilterType::InternalInputImageType,FilterType::InternalOutputImageType > MedianType;
MedianType::Pointer median = MedianType::New();
MedianType::InputSizeType rad;
rad.Fill( 5 );
median->SetRadius( rad );

filter->SetFilter( median );


itk::SimpleFilterWatcher watcher(filter, "filter");
typedef itk::ImageFileWriter< ImageType3D > WriterType;
WriterType::Pointer writer = WriterType::New();
writer->SetInput( filter->GetOutput() );
writer->SetFileName( _outputFileName );
writer->Update();

*/
//  maskFilter2D->SetInput2( maskImg2d );
//  sliceBySliceFilter->SetFilter( maskFilter2D );
//      sliceBySliceFilter->SetInput( img3D );



/*************************************************/
/* Setting the ouput to write  the images */
/*************************************************/
// writes the new binary image to a file.
/*WriterType3D::Pointer writer= WriterType3D::New();
writer->SetFileName(_outputFileName);
writer->SetInput(reader->GetOutput());

// TRY-CATCH Exception Handling
  try
  {
      writer ->Update();                      // updates writer
  }
  catch(itk::ExceptionObject &err)                // checks if there are any exceptions
  {
      std::cerr << "ExceptionObject caught!" <<std::endl;
      std::cerr << err <<std::endl;
      return EXIT_FAILURE;
  }
*/

// -------------
// Image Preprocessing per each slice

// Starting for slice 1

//typedef ImageFileWriter<ImageType> WriterType;
/*    WriterType2D::Pointer writer =WriterType2D::New();
   writer->SetInput(filter->GetOutput());
   writer->SetFileName("/home/calonso/output.mha");
   writer->Update();
*/
/* No funciona
 *   typedef itk::BinaryBallStructuringElement<unsigned char, 2> KernelType;
 typedef itk::WhiteTopHatImageFilter<ImageType2D, ImageType2D, KernelType> FilterType2;

     KernelType kernel;
     kernel.SetRadius(20);
     FilterType2::Pointer tophat = FilterType2::New();
     tophat->SetKernel(kernel);

 filter->SetFilter(tophat);
*/


/*


     // Step A.1. Binary segmentation + Dilation applied to each slice. The output of this image can be used to extract the skulll

     // Filter Definition



     BinaryDilateImageFilterType::Pointer dilateFilter2= BinaryDilateImageFilterType::New();
     //erodeFilter->SetInput(reader->GetOutput());
     dilateFilter2->SetKernel(structuringElement);
     dilateFilter2->SetDilateValue(_foregroundPixelValue); // This must be the same value as foreground value




     // Opening
     typedef itk::BinaryShapeOpeningImageFilter< ImageType2D > BinaryOpeningType;
     BinaryOpeningType::Pointer opening = BinaryOpeningType::New();
    // opening->SetInput( reader->GetOutput() );
     opening->SetForegroundValue( _foregroundPixelValue);
     opening->SetBackgroundValue( 0 );
     opening->SetLambda( 100 );
    // opening->SetReverseOrdering( false );
    // opening->SetFullyConnected( false );
    // opening->SetAttribute( argv[8] );

     typedef itk::BinaryMorphologicalClosingImageFilter< ImageType2D, ImageType2D, StructuringElementType > CloseType;
     CloseType::Pointer close = CloseType::New();
     //close->SetInput( filter->GetOutput() );
     close->SetKernel( structuringElement );
     close->SetForegroundValue( _foregroundPixelValue );









    typedef  itk::ImageFileWriter< ImageType3D  > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(_outputFileName);
    writer->SetInput(filter->GetOutput());
    writer->Update();

    */
/*
 * SKULL STRIP ITK VERSION ----> ESTO NO FUNCIONA
 *
string atlasImageFilename="/home/cmontes/Downloads/SourceCode4_Git_3917976(1)/test/Input/atlasImage.mha";
string atlasMaskFilename = "/home/cmontes/Downloads/SourceCode4_Git_3917976(1)/test/Input/atlasMask.mha";



// set up skull-stripping filter
typedef itk::Image<short, 3> AtlasImageType;
typedef itk::Image<unsigned char, 3> AtlasLabelType;


// image reading

typedef itk::ImageFileReader<AtlasImageType> AtlasReaderType;
typedef itk::ImageFileReader<AtlasLabelType> LabelReaderType;

AtlasReaderType::Pointer atlasReader = AtlasReaderType::New();
LabelReaderType::Pointer labelReader = LabelReaderType::New();

atlasReader->SetFileName( atlasImageFilename );
labelReader->SetFileName( atlasMaskFilename );

try
  {
  atlasReader->Update();
  labelReader->Update();
  }
catch ( itk::ExceptionObject &exception )
  {
  std::cerr << "Exception caught !  cannot read the atlas and mask images" << std::endl;
  std::cerr << exception << std::endl;

  return EXIT_FAILURE;
  }
std::cout << std::endl << "Performing skull-stripping" << std::endl;

typedef itk::StripTsImageFilter<ImageType3D, AtlasImageType, AtlasLabelType> StripTsFilterType;
StripTsFilterType::Pointer stripTsFilter = StripTsFilterType::New();


// set the required inputs for the stripTsImageFilter
stripTsFilter->SetInput( reader->GetOutput() );
stripTsFilter->SetAtlasImage( atlasReader->GetOutput() );
stripTsFilter->SetAtlasBrainMask( labelReader->GetOutput() );

try
   {
   stripTsFilter->Update();
   }
 catch ( itk::ExceptionObject &exception )
   {
   std::cerr << "Exception caught ! Cannot do the skull stripping" << std::endl;
   std::cerr << exception << std::endl;

   return EXIT_FAILURE;
   }

// mask the patient image using the output generated from the stripTsImageFilter as mask
typedef itk::MaskImageFilter<ImageType3D, AtlasLabelType, ImageType3D> MaskFilterType;
MaskFilterType::Pointer maskFilter = MaskFilterType::New();

maskFilter->SetInput1( reader->GetOutput() );
maskFilter->SetInput2( stripTsFilter->GetOutput() );

try
  {
  maskFilter->Update();
  }
catch ( itk::ExceptionObject &exception )
  {
  std::cerr << "Exception caught ! " << std::endl;
  std::cerr << exception << std::endl;

  return EXIT_FAILURE;
  }


// write mask and masked patient image
typedef itk::ImageFileWriter<AtlasLabelType> MaskWriterType;
MaskWriterType::Pointer maskWriter = MaskWriterType::New();
maskWriter->SetInput( stripTsFilter->GetOutput() );
maskWriter->SetFileName( _outputFileName_mask ); // TO REVIEW

typedef itk::ImageFileWriter<ImageType3D> ImageWriterType;
ImageWriterType::Pointer imageWriter = ImageWriterType::New();
imageWriter->SetInput( maskFilter->GetOutput() );
imageWriter->SetFileName( _outputFileName );// TO REVIEW

try
  {
  maskWriter->Update();
  imageWriter->Update();
  }
catch ( itk::ExceptionObject &exception )
  {
  std::cerr << "Exception caught ! " << std::endl;
  std::cerr << exception << std::endl;

  return EXIT_FAILURE;
  }

*/

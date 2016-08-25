#include <iostream>
#include "Defines/ImageITKVTKDefines.h"
using namespace std;

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




/*************************************************/
/* B. Skull strip */
/*************************************************/
int skullStrip(ReaderType3D::Pointer reader, string _outputFileName,string _outputFileName_mask )
{
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


}

#include "itkAdaptiveHistogramEqualizationImageFilter.h"
#include <itkWhiteTopHatImageFilter.h>

// ANISOTROPIC DIFFUSION
#include "itkGradientAnisotropicDiffusionImageFilter.h"

//EROSION
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"


int main()
{
double startTime = time(0);
    /*************************************************/
    // SMART POINTERS DECLARATION & INITIALIZATION

    //FilterType::Pointer filter = FilterType::New();

    string _basePath = "/home/cmontes/ToolDevelopment/BloodSimulations/DATA/MIDAS_DATA/Custom/Normal-001/MRA";
    string _fileBaseName = "/Normal001-MRA.mha";
    string _outFileBaseName="/home/cmontes/ToolDevelopment/BloodSimulations/KTH_project/ROBEX/output-slice-enhanced.mha";

    string _fileNameComplete = _basePath +_fileBaseName;
    string _outputFileName = _outFileBaseName;
    string _outputFileName_mask = _basePath + "/mask_patient.mha";






    /*************************************************/
    /* A. READING THE IMAGE  */
    /*************************************************/

    // At the moment, only one image read
    ReaderType3D::Pointer reader = ReaderType3D::New();
    ImageType3D::ConstPointer inputImage;
    reader->SetFileName(_fileNameComplete);
    try
    {
      reader->Update();
      inputImage = reader->GetOutput();
    }
    catch ( itk::ExceptionObject &err)
    {
      std::cerr << "[READ-IMAGE] ExceptionObject caught !" << std::endl;
      std::cerr << err << std::endl;
      return EXIT_FAILURE;
     }


    typedef itk::GradientAnisotropicDiffusionImageFilter< ImageType2D,ImageType2D > EnhancementFilterType;
     EnhancementFilterType::Pointer enhancementFilter = EnhancementFilterType::New();
     //filter->SetInput( reader->GetOutput() );
     enhancementFilter->SetNumberOfIterations(5);
     enhancementFilter->SetTimeStep( 0.05 );
     enhancementFilter->SetConductanceParameter( 1.0 );
     //filter->Update();

     // EROSION
     typedef itk::BinaryBallStructuringElement<ImageType2D::PixelType, 2>                  StructuringElementType;
     StructuringElementType structuringElement;
     structuringElement.SetRadius(1);
     structuringElement.CreateStructuringElement();

     typedef itk::BinaryErodeImageFilter <ImageType2D, ImageType2D, StructuringElementType>
       BinaryErodeImageFilterType;

     BinaryErodeImageFilterType::Pointer erodeFilter= BinaryErodeImageFilterType::New();
     //erodeFilter->SetInput(reader->GetOutput());
     erodeFilter->SetKernel(structuringElement);
     erodeFilter->SetErodeValue(255);

     typedef itk::BinaryDilateImageFilter <ImageType2D, ImageType2D, StructuringElementType>
       BinaryDilateImageFilterType;

     BinaryDilateImageFilterType::Pointer dilateFilter= BinaryDilateImageFilterType::New();
     //erodeFilter->SetInput(reader->GetOutput());
     dilateFilter->SetKernel(structuringElement);
     dilateFilter->SetDilateValue(255);



/* ESTE FUNCIONA*/

      typedef itk::SliceBySliceImageFilter< ImageType3D, ImageType3D > FilterType;
           FilterType::Pointer filter = FilterType::New();
           filter->SetInput( reader->GetOutput() );


        typedef itk::BinaryThresholdImageFilter <ImageType2D,ImageType2D> BinaryThresholdImageFilterType;
            BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();

              //  thresholdFilter->SetInput(reader ->GetOutput());
                thresholdFilter->SetLowerThreshold(100);
                thresholdFilter->SetUpperThreshold(255);
                thresholdFilter->SetInsideValue(255);
                thresholdFilter->SetOutsideValue(0);

                // Linking the filters
                thresholdFilter->SetInput(enhancementFilter->GetOutput());
                dilateFilter->SetInput(thresholdFilter->GetOutput());

                filter->SetFilter(enhancementFilter);
               // filter->SetFilter(thresholdFilter);
               // filter->SetInputFilter(enhancementFilter);
                //filter->SetOutputFilter(thresholdFilter);
               // filter->SetInputFilter(thresholdFilter);
                filter->SetOutputFilter(dilateFilter);




    typedef  itk::ImageFileWriter< ImageType3D  > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(_outputFileName);
    writer->SetInput(filter->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;






    cout << "Hello World!" << endl;
    return 0;
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


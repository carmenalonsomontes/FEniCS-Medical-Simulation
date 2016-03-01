#include "ejemplo1.h"
#include <stdlib.h>
#include <vtkNIFTIImageReader.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkImageData.h>
#include <vtkImageReslice.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageProperty.h>
#include <vtkImageSlice.h>
#include <vtkImageReader2.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>
#include <vtkErrorCode.h>



#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"

using namespace itk;
Ejemplo1::Ejemplo1()
{
}

// Funciona
void Ejemplo1::ejecutarVTKNiftiReaderVis(string filename)
{

    vtkSmartPointer<vtkRenderWindowInteractor> iren =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style =
      vtkSmartPointer<vtkInteractorStyleImage>::New();
    style->SetInteractionModeToImage3D();
    vtkSmartPointer<vtkRenderWindow> renWin =
      vtkSmartPointer<vtkRenderWindow>::New();
    iren->SetRenderWindow(renWin);
    iren->SetInteractorStyle(style);


    vtkSmartPointer<vtkNIFTIImageReader> reader =
      vtkSmartPointer<vtkNIFTIImageReader>::New();
    reader.GetPointer()->SetFileName(filename.c_str());
    reader.GetPointer()->Update();
    //reader->SetFileName(filename);
    //reader->Update();

  /*  if (reader->GetErrorCode() != vtkErrorCode::NoError)
      {
      return 1;
      }
*/
    vtkSmartPointer<vtkMatrix4x4> matrix =
      vtkSmartPointer<vtkMatrix4x4>::New();
    if (reader->GetQFormMatrix())
      {
      matrix->DeepCopy(reader->GetQFormMatrix());
      matrix->Invert();
      }
    else if (reader->GetSFormMatrix())
      {
      matrix->DeepCopy(reader->GetSFormMatrix());
      matrix->Invert();
      }

    vtkSmartPointer<vtkImageReslice> reslice =
      vtkSmartPointer<vtkImageReslice>::New();
    reslice->SetInputConnection(reader->GetOutputPort());
    reslice->SetResliceAxes(matrix);
    reslice->SetInterpolationModeToLinear();
    reslice->Update();

    double range[2];
    int extent[6];
    reslice->GetOutput()->GetScalarRange(range);
    reslice->GetOutput()->GetExtent(extent);

    static double viewport[3][4] = {
      { 0.67, 0.0, 1.0, 0.5 },
      { 0.67, 0.5, 1.0, 1.0 },
      { 0.0, 0.0, 0.67, 1.0 },
    };

    // check if image is 2D
    bool imageIs3D = (extent[5] > extent[4]);

    for (int i = 2*(imageIs3D == 0); i < 3; i++)
      {
      vtkSmartPointer<vtkImageSliceMapper> imageMapper =
        vtkSmartPointer<vtkImageSliceMapper>::New();
      if (i < 3)
        {
        imageMapper->SetInputConnection(reslice->GetOutputPort());
        }
      imageMapper->SetOrientation(i % 3);
      imageMapper->SliceAtFocalPointOn();

      vtkSmartPointer<vtkImageSlice> image =
        vtkSmartPointer<vtkImageSlice>::New();
      image->SetMapper(imageMapper);

      image->GetProperty()->SetColorWindow(range[1] - range[0]);
      image->GetProperty()->SetColorLevel(0.5*(range[0] + range[1]));
      image->GetProperty()->SetInterpolationTypeToNearest();

      vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
      renderer->AddViewProp(image);
      renderer->SetBackground(0.0, 0.0, 0.0);
      if (imageIs3D)
        {
        renderer->SetViewport(viewport[i]);
        }

      renWin->AddRenderer(renderer);

      // use center point to set camera
      double *bounds = imageMapper->GetBounds();
      double point[3];
      point[0] = 0.5*(bounds[0] + bounds[1]);
      point[1] = 0.5*(bounds[2] + bounds[3]);
      point[2] = 0.5*(bounds[4] + bounds[5]);
      double maxdim = 0.0;
      for (int j = 0; j < 3; j++)
        {
        double s = 0.5*(bounds[2*j+1] - bounds[2*j]);
        maxdim = (s > maxdim ? s : maxdim);
        }

      vtkCamera *camera = renderer->GetActiveCamera();
      camera->SetFocalPoint(point);
      if (imageMapper->GetOrientation() == 2)
        {
        point[imageMapper->GetOrientation()] -= 500.0;
        camera->SetViewUp(0.0, +1.0, 0.0);
        }
      else
        {
        point[imageMapper->GetOrientation()] += 500.0;
        camera->SetViewUp(0.0, 0.0, +1.0);
        }
      camera->SetPosition(point);
      camera->ParallelProjectionOn();
      camera->SetParallelScale(maxdim);
      }

    if (imageIs3D)
      {
      renWin->SetSize(600, 400);
      }
    else
      {
      renWin->SetSize(400, 400);
      }

    renWin->Render();
    iren->Start();
}

// NO Funciona
void Ejemplo1::grabarSlices(string filename)
{


  typedef unsigned char   PixelType;
  const unsigned int Dimension = 3;

  typedef itk::Image< PixelType, Dimension >  ImageType;
  typedef itk::ImageSeriesReader< ImageType > ReaderType;
  typedef itk::ImageFileWriter<   ImageType > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const unsigned int first = 0;//atoi( argv[2] );
  const unsigned int last  = 128;//atoi( argv[3] );

  const char * outputFilename = "_output";

  typedef itk::NumericSeriesFileNames    NameGeneratorType;

  NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();

  nameGenerator->SetSeriesFormat(".png");// argv[1] );

  nameGenerator->SetStartIndex( first );
  nameGenerator->SetEndIndex( last );
  nameGenerator->SetIncrementIndex( 1 );
  std::vector<std::string> names = nameGenerator->GetFileNames();

  // List the files
  //
  std::vector<std::string>::iterator nit;
  for (nit = names.begin();
       nit != names.end();
       nit++)
    {
    std::cout << "File: " << (*nit).c_str() << std::endl;
    }

  reader->SetFileNames( names  );

  writer->SetFileName( outputFilename );
  writer->SetInput( reader->GetOutput() );
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    //return EXIT_FAILURE;
    }

}


#include "itkExtractImageFilter.h"

// FUnciona!!!
void Ejemplo1::slicesMetodo2(string filename)
{

    typedef signed short  InputPixelType;
    typedef signed short OutputPixelType;


    typedef itk::Image< InputPixelType, 3 > InputImageType;
    typedef itk::Image< OutputPixelType, 2 > OutputImageType;
    typedef itk::ImageFileReader< InputImageType > ReaderType;
    typedef itk::ImageFileWriter< OutputImageType >  WriterType;


    // Reader and writer created
    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();


    // Reading the Image
    reader->SetFileName( filename );


    // Applying the filter
    typedef itk::ExtractImageFilter< InputImageType,
    OutputImageType > FilterType;

    FilterType::Pointer filter = FilterType::New();
    filter->InPlaceOn();
    filter->SetDirectionCollapseToSubmatrix();
    reader->UpdateOutputInformation();

    // Getting the slice
    InputImageType::RegionType inputRegion =     reader->GetOutput()->GetLargestPossibleRegion();
    InputImageType::SizeType size = inputRegion.GetSize();
    //____________________________________________________-
    //cout << size[0] << "||" << size[1] << "||" << size[2] << endl;
    //  X || Y  || Z
    int noSlices = size[2];
    size[2] = 0; // Componente Z
    InputImageType::IndexType start = inputRegion.GetIndex();

    //_______________________________________________________________________
    // Printing a single slice
    for (int i=0; i < noSlices;i++)
    {
        cout << "Processing slice "<< i << endl;
        const unsigned int sliceNumber = 75; //argv[3] ); // Extraemos solo esta slice
        start[2] = sliceNumber;

        InputImageType::RegionType desiredRegion;
        desiredRegion.SetSize( size );
        desiredRegion.SetIndex( start );
        filter->SetExtractionRegion( desiredRegion );
        filter->SetInput( reader->GetOutput() );
        stringstream ss;
        ss << i;
        string _no = ss.str();
        string _sliceName = "/home/calonso/OUT/n"+_no+".nii.gz";
        cout << _sliceName << endl;
        writer->SetFileName( _sliceName);
        writer->SetInput( filter->GetOutput() );


       try
        {
        writer->Update();
        }
        catch( itk::ExceptionObject & err )
        {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        //return EXIT_FAILURE;
        }

    }
//_______________________________________________________________________



 /*    ImageSliceConstIteratorWithIndex<ImageType> it( image, image->GetRequestedRegion() );

     it.SetFirstDirection(2);
     it.SetSecondDirection(0);

     it.GoToBegin();
     while( !it.IsAtEnd() )
     {
       while( !it.IsAtEndOfSlice() )
       {
         while( !it.IsAtEndOfLine() )
         {
            value = it.Get();  // it.Set() doesn't exist in the Const Iterator
            ++it;
         }
         it.NextLine();
       }
       it.NextSlice();
      }

*/


    /*   typedef signed short                            InputPixelType;
       typedef itk::Image< InputPixelType, 2 >         InputImageType;
       typedef itk::ImageFileReader< InputImageType >  ReaderType;

       typedef float                                   OutputPixelType;
       typedef itk::Image< OutputPixelType, 2 >        OutputImageType;
       typedef itk::ImageFileWriter< OutputImageType > WriterType;

       ReaderType::Pointer reader = ReaderType::New();
       WriterType::Pointer writer = WriterType::New();

       typedef itk::RescaleIntensityImageFilter<
           InputImageType, OutputImageType >   FilterType;

       FilterType::Pointe scaler = FilterType::New();

       scaler->SetInput( reader->GetOutput() );
       writer->SetInput( scaler->GetOutput() );

       scaler->SetOutputMaximum( 1.0 );
       scaler->SetOutputMinimum( 0.0 );

       FileNamesContainer::const_iterator filenameItr = fileNames.begin();
       while( filenameItr != fileNames.end() )
          {
          reader->SetFileName( *filenameItr );
          std::string outputFilename = outputDir + *filenameItr;
          writer->SetFileName( outputFilename );
          writer->Update();
          ++filenameItr;
          }*/
}


//#include <itkImageIteratorWithIndex.h>
//#include <itkImageSliceConstIteratorWithIndex.hxx>
//#include <itkImageLinearIteratorWithIndex.h>

#include <itkExtractImageFilter.h>
void Ejemplo1::extractingSlices(string filename)
{

    // Step 1. Read the image
    // Defining the images
    typedef signed short InputPixelType;
    typedef signed short OutputPixelType;

    typedef itk::Image<InputPixelType,3> InputImageType;
    typedef itk::Image<OutputPixelType,2> OutputImageType;

    // Reading the image
    typedef itk::ImageFileReader<InputImageType> ReaderType;
    //typedef itk::ImageFileWriter<OutputImageType> WriterType;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(filename);
    // Extracting the image



}


#include "itkImageSliceIteratorWithIndex.h"
#include "itkImageSliceConstIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"

void Ejemplo1::iteratingSlices(string filename)
{
    typedef unsigned short PixelType;
    typedef itk::Image<PixelType,2> ImageType2D;
    typedef itk::Image<PixelType,3> ImageType3D;
    // Defining the sliceIterator
    typedef itk::ImageLinearIteratorWithIndex<ImageType2D> LinearIteratorType;
    typedef itk::ImageSliceConstIteratorWithIndex<ImageType3D> SliceIteratorType;

    // Projection Image
    unsigned int projectionDirection = 2;
    unsigned int i,j;
    unsigned int direction[2];
    for (i = 0, j= 0;i < 3;++i)
        if (i!= projectionDirection)
        {
            direction[j] = i;
            j++;
        }
    //*********************************************************************************
    // Reading the image
    typedef itk::ImageFileReader< ImageType3D > ReaderType;
    typedef itk::ImageFileWriter< ImageType2D > WriterType;
    ImageType3D::ConstPointer inputImage;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(filename );
    try
      {
      reader->Update();
      inputImage = reader->GetOutput();
      }
    catch ( itk::ExceptionObject &err)
      {
      std::cout << "ExceptionObject caught !" << std::endl;
      std::cout << err << std::endl;
      //return -1;
      }
    //*********************************************************************************

    // ---------
    ImageType2D::RegionType region;
    ImageType2D::RegionType::SizeType size;
    ImageType2D::RegionType::IndexType index;

    ImageType3D::RegionType requestedRegion = inputImage->GetRequestedRegion();
    index[direction[0]] = requestedRegion.GetIndex()[direction[0]];
    index[1 - direction[0]] = requestedRegion.GetIndex()[direction[1]];
    size[direction[0]] = requestedRegion.GetSize()[direction[0]];
    size[1 - direction[0]] = requestedRegion.GetSize()[direction[1]];

    region.SetSize(size);
    region.SetIndex(index);

    ImageType2D::Pointer outputImage = ImageType2D::New();
    outputImage->SetRegions(region);
    outputImage->Allocate();

    // Iterating ...
    SliceIteratorType inputIt(inputImage,inputImage->GetRequestedRegion());
    LinearIteratorType outputIt(outputImage, outputImage->GetRequestedRegion());

    inputIt.SetFirstDirection(direction[1]);
    inputIt.SetSecondDirection(direction[0]);

    outputIt.SetDirection(1-direction[0]);

    //--------------------------------------------------------------------------
    // Compute the projection
    outputIt.GoToBegin();
    // Initialization of the projections to nonpositive min val.
    while (!outputIt.IsAtEnd())
    {
        while (!outputIt.IsAtEndOfLine())
        {
            outputIt.Set(itk::NumericTraits<unsigned short>::NonpositiveMin());
            ++outputIt;
        }
        outputIt.NextLine();
    }
    // Iterating
    inputIt.GoToBegin();
    outputIt.GoToBegin();
    int _count=0;
    while(!inputIt.IsAtEnd())
    {
        while(!inputIt.IsAtEndOfSlice())
        {
            while (!inputIt.IsAtEndOfLine())
            {
                outputIt.Set(vnl_math_max(outputIt.Get(),inputIt.Get()));
                ++inputIt;
                ++outputIt;
            }
            outputIt.NextLine();
            inputIt.NextLine();
        }
        outputIt.GoToBegin();
        inputIt.NextSlice();

        // Writing the slices
        stringstream ss;
        ss << _count;
        string _no = ss.str();
        string _sliceName = "/home/calonso/OUT/Slice_"+_no+".nii.gz";
        cout << _sliceName << endl;

        WriterType::Pointer writer = WriterType::New();
        writer->SetFileName( _sliceName );
        writer->SetInput(outputImage);
        try
          {
          writer->Update();
          _count++;
          }
        catch ( itk::ExceptionObject &err)
          {
          std::cout << "ExceptionObject caught !" << std::endl;
          std::cout << err << std::endl;
         // return -1;
          }

    }

}

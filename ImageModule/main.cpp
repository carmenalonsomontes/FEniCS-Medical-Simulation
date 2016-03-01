#include <iostream>
#include <string>

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkNiftiImageIO.h>


//#include "itkRescaleIntensityImageFilter.h"

#include "QuickView.h"

#include "itkImageToVTKImageFilter.h"
#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"

#include "vtkVersion.h"
#include "vtkImageViewer.h"
#include "vtkImageMapper3D.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "itkRGBPixel.h"

#include "ejemplo1.h"
using namespace std;
using namespace itk;


/** Creating the imageIO basic to read the file */
ImageIOBase::Pointer getImageIO(string input){
  ImageIOBase::Pointer imageIO = ImageIOFactory::CreateImageIO(input.c_str(), ImageIOFactory::ReadMode);

  imageIO->SetFileName(input);
  imageIO->ReadImageInformation();

  return imageIO;
}

size_t num_dimensions(ImageIOBase::Pointer imageIO){
  return imageIO->GetNumberOfDimensions();
}


ImageIOBase::IOComponentType component_type(ImageIOBase::Pointer imageIO){
  return imageIO->GetComponentType();
}


ImageIOBase::IOPixelType pixel_type(ImageIOBase::Pointer imageIO){
  return imageIO->GetPixelType();
}

// ==============================================================================================================

int main()
{
    typedef Image< double, 3 >         ImageType;
    typedef ImageFileReader<ImageType> ReaderType;
    typedef itk::ImageToVTKImageFilter<ImageType>       ConnectorType;
    string _fileName = "/home/calonso/ToolDevelopment/BloodSimulations/DATA/MIDAS_DATA/Custom/Normal-001/MRA/Normal001-MRA.mha";
//ImageIOBase::Pointer imageIO = getImageIO(_fileName);
//cout << "numDimensions: " << imageIO.GetPointer()->GetSliceStride() << endl;
     //===================================================================================================
    // Reading the NII.gz image
 //

    ReaderType::Pointer reader = ReaderType::New();
    ConnectorType::Pointer connector = ConnectorType::New();
    reader->SetFileName(_fileName);
    connector->SetInput(reader->GetOutput());

    vtkSmartPointer<vtkImageActor> actor =
      vtkSmartPointer<vtkImageActor>::New();
//  #if VTK_MAJOR_VERSION <= 5
//    actor->SetInput(connector->GetOutput());
//  #else
    connector->Update();
    actor->GetMapper()->SetInputData(connector->GetOutput());
//  #endif
    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->ResetCamera();

    vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style =
      vtkSmartPointer<vtkInteractorStyleImage>::New();

    renderWindowInteractor->SetInteractorStyle(style);

    renderWindowInteractor->SetRenderWindow(renderWindow);
   // renderWindowInteractor->Initialize();

   // renderWindowInteractor->Start();



    Ejemplo1 ej;
   // ej.ejecutarVTKNiftiReaderVis(_fileName);
    //ej.grabarSlices(_fileName);
   // ej.slicesMetodo2(_fileName);
    ej.iteratingSlices(_fileName);

    return EXIT_SUCCESS;


/*
 * ImageIOBase::Pointer imageIO = getImageIO(_fileName);
    // In order to read a image, we need its dimensionality and component type
    cout << "numDimensions: " << num_dimensions(imageIO) << endl;
    cout << "component type: " << imageIO->GetComponentTypeAsString(component_type(imageIO)) << endl;
    // The pixel type is not necessary. This is just to let you know that it exists
    cout << "pixel type: " << imageIO->GetPixelTypeAsString(pixel_type(imageIO)) << endl;

    cout << "Hello World! 2222222222222222222222" << endl;


   QuickView viewer;
    viewer.AddImage(imageIO.GetPointer());
//                  .AddImage();
 //     viewer.AddImage(rescaleFilter->GetOutput());
      viewer.Visualize();










    return 0; */
}


#include "imagehelper.h"

ImageHelper::ImageHelper()
{
}


int  ImageHelper::readAngiogram(AngiogramVO * _angiogramVO)
{

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(_angiogramVO->getAngiogramPathImFile());
    try
    {
          reader->Update();
          _angiogramVO->setAngiogramDataIm3D(reader->GetOutput());//);
    }
    catch ( itk::ExceptionObject &err)
    {
      std::cout << "ExceptionObject caught !" << std::endl;
      std::cout << err << std::endl;
      return -1;
    }
    return 0;
}

int ImageHelper::writeSlice(AngiogramVO * _angiogramVO, string _sliceName, int _sliceNo)
{
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName( _sliceName );
    writer->SetInput(_angiogramVO->getSlice()); // TBD
    try
    {
        writer->Update();
    }
    catch ( itk::ExceptionObject &err)
    {
      std::cout << "ExceptionObject caught !" << std::endl;
      std::cout << err << std::endl;
      return -1;
    }
    return 0;
}

int ImageHelper::writeSlices(AngiogramVO * _angiogramVO,string _slicePath, string _fileExtension)
{
    int _sliceCounter = 0;
    // Writing the slices
    stringstream ss;
    ss << _sliceCounter;
    string _no = ss.str();
    string _sliceName = _slicePath+_no+_fileExtension;
    cout << _sliceName << endl;

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName( _sliceName );
    writer->SetInput(_angiogramVO->getSlice()); // TBD
    try
      {
        writer->Update();
        _sliceCounter++;
      }
    catch ( itk::ExceptionObject &err)
      {
      std::cout << "ExceptionObject caught !" << std::endl;
      std::cout << err << std::endl;
      return -1;
      }
    return 0;
}

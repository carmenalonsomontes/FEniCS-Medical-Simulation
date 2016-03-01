#ifndef ANGIOGRAMVO_H
#define ANGIOGRAMVO_H
#include <string>
#include "Defines/GeneralImageDefines.h"

using namespace std;

class AngiogramVO
{
public:
    AngiogramVO();

    string getAngiogramFileName();
    void setAngiogramFileName(string _filename);

    string getAngiogramTypeImFile();
    void setAngiogramTypeImFile(string _imType);

    string getAngiogramPathImFile();
    void setAngiogramPathImFile(string _imPath);

    ImageType3D::ConstPointer getAngiogramDataIm3D();
    void setAngiogramDataIm3D(ImageType3D::ConstPointer imData);

    ImageType2D::Pointer getSlice();
    //outputImage = ImageType2D::New();

private:
    string _angiogramFileName;
    string _angiogramTypeImFile;
    string _angiogramPathImFile;
    ImageType3D::ConstPointer _angiogramDataIm3D;

};

#endif // ANGIOGRAMVO_H

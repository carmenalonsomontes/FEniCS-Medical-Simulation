#ifndef SLICEBYSLICEFILTER_H
#define SLICEBYSLICEFILTER_H
#include <QStringList>

#include "itkImage.h"
#include "itkSimpleFilterWatcher.h"
#include "itkMedianImageFilter.h"
#include "itkSliceBySliceImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "../defines/OperationDefines.h"

#include "../ioperation.h"


using namespace std;
using namespace itk;
typedef ImageFileReader<ImageType> ReaderType;
typedef SliceBySliceImageFilter<ImageType,ImageType> FilterType;

// Filters
typedef MedianImageFilter<FilterType::InternalInputImageType,FilterType::InternalOutputImageType> MedianType;


class SliceBySliceFilter: public IOperation
{
public:
    SliceBySliceFilter();
    void SetInPut(ImageType::Pointer _input);
    void SetParameters(QStringList _parameterList);

    static IOperation * Create();
    ImageType::Pointer GetOutput();

private:
    FilterType::Pointer filter;
    MedianType::Pointer median;
    MedianType::InputSizeType rad;


    void buildFilter(int radius, TypeFilters filterType);
    TypeFilters getFilterNumber(QString _filterName);
};

#endif // SLICEBYSLICEFILTER_H

#ifndef SLICEBYSLICEFILTER_H
#define SLICEBYSLICEFILTER_H
#include <QStringList>

#include "itkSimpleFilterWatcher.h"
#include "itkMedianImageFilter.h"
#include "itkSliceBySliceImageFilter.h"
#include "../defines/OperationDefines.h"
#include "../ioperation.h"

typedef SliceBySliceImageFilter<ImageType2D,ImageType2D> FilterType;

// Filters
typedef MedianImageFilter<FilterType::InternalInputImageType,FilterType::InternalOutputImageType> MedianType;


class SliceBySliceFilter //: public IOperation
{
public:
    SliceBySliceFilter();
    void SetInPut(ImageType2D::Pointer _input);
    void SetParameters(QStringList _parameterList);

    static IOperation * Create();
    ImageType2D::Pointer GetOutput();

private:
    FilterType::Pointer filter;
    MedianType::Pointer median;
    MedianType::InputSizeType rad;


    void buildFilter(int radius, TypeFilters filterType);
    TypeFilters getFilterNumber(QString _filterName);
};

#endif // SLICEBYSLICEFILTER_H

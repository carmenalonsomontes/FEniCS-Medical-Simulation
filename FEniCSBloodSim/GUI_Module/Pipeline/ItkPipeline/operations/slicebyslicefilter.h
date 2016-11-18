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


class SliceBySliceFilter : public IOperation
{
public:
    SliceBySliceFilter();
    void SetInPut(ImageType2D::Pointer _input);
    void SetInPut(ImageType3D::Pointer _input);

    void SetParameters(QStringList _parameterList);

    static IOperation * Create();
    ImageType2D::Pointer GetOutput2D();
    ImageType3D::Pointer GetOutput3D();

    void set3D(bool _val);
    bool is3D();

    void exec();
    void save(QString _path);

private:
    FilterType::Pointer filter;
    MedianType::Pointer median;
    MedianType::InputSizeType rad;
    bool _is3D;

    void buildFilter(int radius, TypeFilters filterType);
    TypeFilters getFilterNumber(QString _filterName);
};

#endif // SLICEBYSLICEFILTER_H

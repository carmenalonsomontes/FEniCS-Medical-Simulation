#include "slicebyslicefilter.h"
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "GUI_Module/Defines/Workflow/WorkflowDefines.h"
SliceBySliceFilter::SliceBySliceFilter()
{
    filter = FilterType::New();
}
void SliceBySliceFilter::SetInPut(ImageType2D::Pointer _input)
{
    filter->SetInput(_input);
}

void SliceBySliceFilter::SetParameters(QStringList _parameterList)
{
   ReaderType2D::Pointer reader = ReaderType2D::New();
   int _radius = -1;
   TypeFilters _typeFilter = NO_FILTER_FOUND;

   for (int i= 0; i <_parameterList.size();i++)
    {
        QString _cParameter = _parameterList.at(i);
        QStringList _parameterItems = _cParameter.split(PARAMETER_SEPARATOR);

        if (_parameterItems.isEmpty())
            continue;

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetInput") == STR_EQUAL)
        {
            reader->SetFileName(_parameterItems.at(PARAMETER_VALUE).toStdString());
            SetInPut(reader->GetOutput());
        }
        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetRadius") == STR_EQUAL)
            _radius=_parameterItems.at(PARAMETER_VALUE).toInt();

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetFilter") == STR_EQUAL)
            _typeFilter = getFilterNumber(_parameterItems.at(PARAMETER_VALUE));

    }

    buildFilter(_radius,_typeFilter);

   // SimpleFilterWatcher watcher(filter,"filter");
   /*typedef ImageFileWriter<ImageType> WriterType;
   WriterType::Pointer writer =WriterType::New();
   writer->SetInput(filter->GetOutput());
   writer->SetFileName("/home/calonso/output.mha");
   writer->Update();
   */
    //filter->Update();
}


void SliceBySliceFilter::buildFilter(int radius, TypeFilters filterType)
{
    switch (filterType) {
    case MEDIAN_FILTER:
        median = MedianType::New();

        rad.Fill(5);
        median->SetRadius(rad);
        filter->SetFilter(median);
        break;
    default:
        break;
    }
}

TypeFilters SliceBySliceFilter::getFilterNumber(QString _filterName)
{
    if (QString::compare(_filterName,"MEDIAN_FILTER") == STR_EQUAL)
        return MEDIAN_FILTER;
    return NO_FILTER_FOUND;
}


ImageType2D::Pointer SliceBySliceFilter::GetOutput2D()
{
    return filter->GetOutput();
}


IOperation * SliceBySliceFilter::Create() {
    return new SliceBySliceFilter();
}

// --------------------------------------------------------
// TODO
void SliceBySliceFilter::set3D(bool _val)
{
// TODO
}

bool SliceBySliceFilter::is3D()
{
// TODO
}

void SliceBySliceFilter::exec()
{
// TODO
}

void SliceBySliceFilter::save(QString _path)
{
// TODO
}
ImageType3D::Pointer SliceBySliceFilter::GetOutput3D()
{
// TODO
}

void SliceBySliceFilter::SetInPut(ImageType3D::Pointer _input)
{
  // TODO
}

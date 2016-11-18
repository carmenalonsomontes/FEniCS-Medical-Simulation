#include "adaptivehistogramequalization.h"
#include <QStringList>


AdaptiveHistogramEqualization::AdaptiveHistogramEqualization()
{
    _is3D = false;
    filter2D = AdaptiveHistogramEqualizationImageFilterType2D::New();
    //filter3D = AdaptiveHistogramEqualizationImageFilterType3D::New();


}

void AdaptiveHistogramEqualization::SetInPut(ImageType2D::Pointer _input)
{
    filter2D->SetInput(_input);
    _is3D = false;
}
void AdaptiveHistogramEqualization::SetInPut(ImageType3D::Pointer _input)
{
    //filter3D->SetInput(_input);
    _is3D = true;
}
void AdaptiveHistogramEqualization::SetParameters(QStringList _parameterList)
{
    if (_is3D)
        setParameters3D(_parameterList);
    else
        setParameters2D(_parameterList);
}

void AdaptiveHistogramEqualization::setParameters3D(QStringList _parameterList)
{
  /*  ReaderType3D::Pointer reader = ReaderType3D::New();


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
             filter3D->SetRadius(_parameterItems.at(PARAMETER_VALUE).toInt());

      }*/
}
void AdaptiveHistogramEqualization::setParameters2D(QStringList _parameterList)
{
    ReaderType3D::Pointer reader = ReaderType3D::New();


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
             filter2D->SetRadius(_parameterItems.at(PARAMETER_VALUE).toInt());

      }
}

void AdaptiveHistogramEqualization::exec()
{
   /* if (_is3D)
        filter3D->Update();
    else*/
        filter2D->Update();
}

void AdaptiveHistogramEqualization::save(QString _path)
{
   /* if (_is3D)
        save3D(_path);
    else */
        save2D(_path);

}
void AdaptiveHistogramEqualization::save3D(QString _path)
{
  /*  WriterType3D::Pointer writer = WriterType3D::New();

    writer->SetFileName(_path.toStdString());

    writer->SetInput(filter3D->GetOutput());

    try
    {
        writer->Update();
    }catch(itk::ExceptionObject & err)
    {
        std::cout << "Error !!!" << std::endl;
        std::cout << err << std::endl;
  } */

}

void AdaptiveHistogramEqualization::save2D(QString _path)
{
    WriterType2D::Pointer writer = WriterType2D::New();

    writer->SetFileName(_path.toStdString());

    writer->SetInput(filter2D->GetOutput());

    try
    {
        writer->Update();
    }catch(itk::ExceptionObject & err)
    {
        std::cout << "Error !!!" << std::endl;
        std::cout << err << std::endl;
    }

}

ImageType2D::Pointer AdaptiveHistogramEqualization::GetOutput2D()
{
    return filter2D->GetOutput();
}


ImageType3D::Pointer AdaptiveHistogramEqualization::GetOutput3D()
{
    //return filter3D->GetOutput();
}

IOperation * AdaptiveHistogramEqualization::Create() {
    return new AdaptiveHistogramEqualization();
}

void AdaptiveHistogramEqualization::set3D(bool _val)
{
    _is3D = _val;
}
bool  AdaptiveHistogramEqualization::is3D()
{
    return _is3D;
}



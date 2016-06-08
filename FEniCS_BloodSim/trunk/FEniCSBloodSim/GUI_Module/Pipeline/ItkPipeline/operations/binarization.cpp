#include "binarization.h"

#include <QString>
#include <QStringList>
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "GUI_Module/Defines/Workflow/WorkflowDefines.h"

Binarization::Binarization()
{
    _is3D = false;
    thresholdFilter2D = BinaryThresholdImageFilterType2D::New();
    thresholdFilter3D = BinaryThresholdImageFilterType3D::New();
}

void Binarization::SetInPut(ImageType2D::Pointer _input)
{
    thresholdFilter2D->SetInput(_input);
    _is3D = false;
}

void Binarization::SetInPut(ImageType3D::Pointer _input)
{
    thresholdFilter3D->SetInput(_input);
    _is3D = true;
}

void Binarization::SetParameters(QStringList _parameterList)
{
    if (_is3D)
        setParameters3D(_parameterList);
    else
        setParameters2D(_parameterList);
}


void Binarization::setParameters3D(QStringList _parameterList)
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

         if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetLowerThreshold") == STR_EQUAL)
             thresholdFilter3D->SetLowerThreshold(_parameterItems.at(PARAMETER_VALUE).toInt());

         if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetUpperThreshold") == STR_EQUAL)
             thresholdFilter3D->SetUpperThreshold(_parameterItems.at(PARAMETER_VALUE).toInt());

         if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetInsideValue") == STR_EQUAL)
             thresholdFilter3D->SetInsideValue(_parameterItems.at(PARAMETER_VALUE).toInt());

         if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetOutsideValue") == STR_EQUAL)
             thresholdFilter3D->SetOutsideValue(_parameterItems.at(PARAMETER_VALUE).toInt());

     }


}

void Binarization::setParameters2D(QStringList _parameterList)
{
    ReaderType2D::Pointer reader = ReaderType2D::New();

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

         if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetLowerThreshold") == STR_EQUAL)
             thresholdFilter2D->SetLowerThreshold(_parameterItems.at(PARAMETER_VALUE).toInt());

         if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetUpperThreshold") == STR_EQUAL)
             thresholdFilter2D->SetUpperThreshold(_parameterItems.at(PARAMETER_VALUE).toInt());

         if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetInsideValue") == STR_EQUAL)
             thresholdFilter2D->SetInsideValue(_parameterItems.at(PARAMETER_VALUE).toInt());

         if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetOutsideValue") == STR_EQUAL)
             thresholdFilter2D->SetOutsideValue(_parameterItems.at(PARAMETER_VALUE).toInt());

     }

}


void Binarization::exec()
{
    if (_is3D)
        thresholdFilter3D->Update();
    else
        thresholdFilter2D->Update();
}

void Binarization::save(QString _path)
{
    if (_is3D)
        save3D(_path);
    else
        save2D(_path);

}

void Binarization::save3D(QString _path)
{
    WriterType3D::Pointer writer = WriterType3D::New();

    writer->SetFileName(_path.toStdString());

    writer->SetInput(thresholdFilter3D->GetOutput());

    try
    {
        writer->Update();
    }catch(itk::ExceptionObject & err)
    {
        std::cout << "Error !!!" << std::endl;
        std::cout << err << std::endl;
    }

}

void Binarization::save2D(QString _path)
{
    WriterType2D::Pointer writer = WriterType2D::New();

    writer->SetFileName(_path.toStdString());

    writer->SetInput(thresholdFilter2D->GetOutput());

    try
    {
        writer->Update();
    }catch(itk::ExceptionObject & err)
    {
        std::cout << "Error !!!" << std::endl;
        std::cout << err << std::endl;
    }

}


ImageType2D::Pointer Binarization::GetOutput2D()
{
    return thresholdFilter2D->GetOutput();
}


ImageType3D::Pointer Binarization::GetOutput3D()
{
    return thresholdFilter3D->GetOutput();
}

IOperation * Binarization::Create() {
    return new Binarization();
}

void Binarization::set3D(bool _val)
{
    _is3D = _val;
}
bool  Binarization::is3D()
{
    return _is3D;
}


#include "binarization.h"

#include <QString>
#include <QStringList>
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "GUI_Module/Defines/Workflow/WorkflowDefines.h"

Binarization::Binarization()
{
}

void Binarization::SetInPut(ImageType::Pointer _input)
{
    thresholdFilter->SetInput(_input);
}

void Binarization::SetParameters(QStringList _parameterList)
{
   ReaderType::Pointer reader = ReaderType::New();
   thresholdFilter = BinaryThresholdImageFilterType::New();

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
           // thresholdFilter->SetInput(reader->GetOutput());
        }

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetLowerThreshold") == STR_EQUAL)
            thresholdFilter->SetLowerThreshold(_parameterItems.at(PARAMETER_VALUE).toInt());

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetUpperThreshold") == STR_EQUAL)
            thresholdFilter->SetUpperThreshold(_parameterItems.at(PARAMETER_VALUE).toInt());

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetInsideValue") == STR_EQUAL)
            thresholdFilter->SetInsideValue(_parameterItems.at(PARAMETER_VALUE).toInt());

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetOutsideValue") == STR_EQUAL)
            thresholdFilter->SetOutsideValue(_parameterItems.at(PARAMETER_VALUE).toInt());

    }
    thresholdFilter->Update();
}


ImageType::Pointer Binarization::GetOutput()
{
    return thresholdFilter->GetOutput();
}

IOperation * Binarization::Create() {
    return new Binarization();
}




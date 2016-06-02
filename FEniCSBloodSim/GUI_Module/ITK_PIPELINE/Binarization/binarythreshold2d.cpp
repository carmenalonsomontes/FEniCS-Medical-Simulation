#include "binarythreshold2d.h"


const std::string BinaryThreshold2D::Name = "BinaryThreshold2D";

/*
#include <QString>
#include <QStringList>
BinaryThreshold2D::BinaryThreshold2D()
{
    thresholdFilter = BinaryThresholdImageFilterType::New();
}

void BinaryThreshold2D::exec()
{
    // todo

}




void BinaryThreshold2D::SetParatemeters(QStringList parameters)
{
    for (int i= 0; i <parameters.size();i++)
    {
        QString _cParameter = parameters.at(i);
        QStringList _parameterItems = _cParameter.split(PARAMETER_SEPARATOR);

        if (_parameterItems.isEmpty())
            continue;

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetInput") == STR_EQUAL)
            loadInput(_parameterItems.at(PARAMETER_VALUE));

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetLowerThreshold") == STR_EQUAL)
            thresholdFilter->SetLowerThreshold(_parameterItems.at(PARAMETER_VALUE).toInt());

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetUpperThreshold") == STR_EQUAL)
            thresholdFilter->SetUpperThreshold(_parameterItems.at(PARAMETER_VALUE).toInt());

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetInsideValue") == STR_EQUAL)
            thresholdFilter->SetInsideValue(_parameterItems.at(PARAMETER_VALUE).toInt());

        if (QString::compare(_parameterItems.at(PARAMETER_NAME),"SetOutsideValue") == STR_EQUAL)
            thresholdFilter->SetOutsideValue(_parameterItems.at(PARAMETER_VALUE).toInt());



    }
}



void BinaryThreshold2D::loadInput(QString _path)
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(_path.toStdString());

    BinaryThresholdImageFilterType::Pointer thresholdFilter
      = BinaryThresholdImageFilterType::New();
    thresholdFilter->SetInput(reader->GetOutput());
  //  thresholdFilter->SetInput(reader->GetOutput()); // TODO
}
*/

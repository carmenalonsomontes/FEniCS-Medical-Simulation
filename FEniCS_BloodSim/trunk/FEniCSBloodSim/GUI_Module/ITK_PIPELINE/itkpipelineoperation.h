#ifndef ITKPIPELINEOPERATION_H
#define ITKPIPELINEOPERATION_H

#include <QList>
#include <QString>
#include <QStringList>
#include "GUI_Module/ITK_PIPELINE/Defines/ItkPipelineDefines.h"

class ItkPipelineOperation
{
public:
    ItkPipelineOperation();
    ItkPipelineOperation(bool isRegistered);
    //virtual ~ItkPipelineOperation(){}
    bool IsRegistered() const;


    //virtual void SetParatemeters(QStringList parameters);
    virtual std::string GetName() = 0;

   // void exec();
   //
//protected:
  //   void loadInput();
private:
     const bool _isRegistered;
    //QStringList _parameters;

};

#endif // ITKPIPELINEOPERATION_H

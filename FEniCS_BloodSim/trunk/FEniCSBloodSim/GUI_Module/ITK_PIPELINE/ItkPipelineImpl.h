#ifndef ITKPIPELINEIMPL_H
#define ITKPIPELINEIMPL_H

#include "itkpipelineoperation.h"
#include "itkpipelinefactory.h"
#include <string>

using namespace std;

template<typename T> class ItkPipelineImpl: public ItkPipelineOperation
{
public:
    static ItkPipelineOperation * Create(){return new T();}
    string GetName(){ return T::Name;}
protected:
    static const bool _isRegistered;
    ItkPipelineImpl() : ItkPipelineOperation(_isRegistered){}
};

template<typename T>
const bool ItkPipelineImpl<T>::_isRegistered = ItkPipelineFactory::Register(T::Name,&ItkPipelineImpl<T>::Create);


#endif // ITKPIPELINEIMPL_H

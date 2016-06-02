#include "itkpipelineoperation.h"

/*ItkPipelineOperation::ItkPipelineOperation()
{
}
*/
ItkPipelineOperation::ItkPipelineOperation() : _isRegistered(false)
{}

ItkPipelineOperation::ItkPipelineOperation( bool isRegistered) : _isRegistered(isRegistered)
{}

bool ItkPipelineOperation::IsRegistered() const
{
    return this->_isRegistered;

}
/*
void ItkPipelineOperation::exec()
{

}
*/

/*void ItkPipelineOperation::SetParatemeters(QStringList parameters)
{
//    _parameters = parameters;
}*/

/*
void ItkPipelineOperation::loadInput()
{

}*/

#include "operationfactory.h"
#include "operations/binarization.h"
#include "operations/slicebyslicefilter.h"

OperationFactory::OperationFactory()
{
    Register("BinaryThreshold2D",&Binarization::Create);
   // Register("SliceBySliceFilter",&SliceBySliceFilter::Create);
}
OperationFactory::OperationFactory(const OperationFactory &)
{

}

OperationFactory::~OperationFactory()
{
    m_FactoryMap.clear();
}

//-------------------------------------------------------------------------------------------

void OperationFactory::Register(const string &operationName, CreateOperationFn pfnCreate)
{
    m_FactoryMap[operationName] = pfnCreate;
}

IOperation * OperationFactory::CreateOperation(const string &operationName)
{
    FactoryMap::iterator it = m_FactoryMap.find(operationName);
    if (it != m_FactoryMap.end())
        return it->second();
    return NULL;
}

OperationFactory * OperationFactory::Get()
{
    static OperationFactory instance;
    return &instance;
}


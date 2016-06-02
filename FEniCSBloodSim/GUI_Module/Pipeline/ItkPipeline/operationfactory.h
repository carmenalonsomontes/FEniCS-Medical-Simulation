#ifndef OPERATIONFACTORY_H
#define OPERATIONFACTORY_H

#include <map>
#include <string>
#include "ioperation.h"

typedef IOperation* (*CreateOperationFn)();

class OperationFactory
{

public:
    ~OperationFactory();

    static OperationFactory * Get();

    void Register(const string &operationName, CreateOperationFn pfnCreate);

    IOperation * CreateOperation(const string &operationName);

private:
    typedef map<string,CreateOperationFn> FactoryMap;
    FactoryMap m_FactoryMap;
    OperationFactory &operator = (const OperationFactory & ){return * this;}


    OperationFactory();
    OperationFactory(const OperationFactory &);

};

#endif // OPERATIONFACTORY_H

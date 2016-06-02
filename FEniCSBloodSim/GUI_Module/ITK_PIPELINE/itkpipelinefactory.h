#ifndef ITKPIPELINEFACTORY_H
#define ITKPIPELINEFACTORY_H

#include "itkpipelineoperation.h"
#include "histogramequalization.h"
#include <string>
#include <map>
//#include "boost/assign.hpp"
using namespace std;

typedef ItkPipelineOperation * (*factoryMethod)();
/*using namespace boost::assign;

template<typename T> ItkPipelineOperation * createInstance() { return new T; }
//typedef map<string, ItkPipelineOperation*(*)()> map_type;
typedef map<string, FnPtr*(*)()> map_type;


//map_type map2 = map_list_of ("DerivedA", &createInstance<HistogramEqualization>);// (3, 'b') (5, 'c') (7, 'd');
typedef int (*FnPtr)(int, int);
*/
class ItkPipelineFactory
{
public:
    static bool Register(string name, factoryMethod createMethod);
    static ItkPipelineOperation * Create(string name);

protected:
    static map<string,factoryMethod> RegisteredNames;


/*
    // ItkPipelineFactory();
   //  ItkPipelineFactory(const std::string &sClassName);

    //ItkPipelineOperation * Create();
//FnPtr * Create();

private:

    map_type map;
    std::string msClassName;

    void loadMap();
*/

};

#endif // ITKPIPELINEFACTORY_H

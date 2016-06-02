#include "itkpipelinefactory.h"

//#include "Binarization/binarythreshold2d.h"




    bool ItkPipelineFactory::Register(string name, factoryMethod createMethod)
    {
        // Add the pair to the map
 /*       pair<map<string,factoryMethod>::iterator,bool> registeredPair =
                ItkPipelineFactory::RegisteredNames.insert(make_pair(name.c_str(),createMethod));
        return registeredPair.second;
*/
    }

    ItkPipelineOperation * ItkPipelineFactory::Create(string name)
    {
/*        map<string,factoryMethod>::iterator registeredPair = ItkPipelineFactory::RegisteredNames.find(name);
        if (registeredPair == ItkPipelineFactory::RegisteredNames.end())
            return NULL;
        return registeredPair->second();
*/
    }

    // Initialize registered names map
    //map<string,factoryMethod> ItkPipelineFactory::RegisteredNames = {};



/*
ItkPipelineFactory::ItkPipelineFactory(const std::string &sClassName)
{
    loadMap();
    msClassName = sClassName;

}


ItkPipelineOperation * ItkPipelineFactory::Create()
{
    return map[msClassName]();
}

void ItkPipelineFactory::loadMap()
{
      map["BinaryThreshold2D"] = &createInstance<BinaryThreshold2D>;
}
*/

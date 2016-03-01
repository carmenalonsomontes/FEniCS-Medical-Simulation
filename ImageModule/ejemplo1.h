#ifndef EJEMPLO1_H
#define EJEMPLO1_H

#include <string>

using namespace std;

class Ejemplo1
{
public:
    Ejemplo1();
    void ejecutarVTKNiftiReaderVis(string filename);
    void grabarSlices(string filename);
    void slicesMetodo2(string filename);
    void extractingSlices(string filename);
    void iteratingSlices(string filename);




};

#endif // EJEMPLO1_H

#include <QApplication>
//#include "renderwindowuisingleinheritance.h"
#include "fenics_blood_sim.h"

int main( int argc, char** argv )
{
  // QT Stuff
  QApplication app( argc, argv );

  FEniCS_Blood_Sim fenicsMainWindow;
  fenicsMainWindow.show();

  //RenderWindowUISingleInheritance renderWindowUISingleInheritance;
  //renderWindowUISingleInheritance.show();

  return app.exec();
}



/*#include <iostream>

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}*/


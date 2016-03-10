#include <QApplication>
#include "renderwindowuisingleinheritance.h"

int main( int argc, char** argv )
{
  // QT Stuff
  QApplication app( argc, argv );

  RenderWindowUISingleInheritance renderWindowUISingleInheritance;
  renderWindowUISingleInheritance.show();

  return app.exec();
}



/*#include <iostream>

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}*/


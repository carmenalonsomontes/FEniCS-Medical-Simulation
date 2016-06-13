#ifndef USERIMAGEINFOSESSION_H
#define USERIMAGEINFOSESSION_H

#include "GUI_Module/Defines/Menu/MenuDefines.h"

class UserImageInfoSession
{
public:
    UserImageInfoSession();

    int getNoImageRow();
    int getNoSlice();
    int getMaxSlice();
    int getMinSlice();

    ImageTabs getSliceType();


    void setNoImageRow(int row);
    void setNoSlice(int slice);
    void setMaxSlice(int max);
    void setMinSlice(int min);

    void setSliceType(ImageTabs type);

    void clear();

    bool isEmpty();



private:
    int _noImageRow;
    int _noSlice;
    int _maxSlice;
    int _minSlice;

    ImageTabs _sliceType;

    void initializeValues();

};

#endif // USERIMAGEINFOSESSION_H

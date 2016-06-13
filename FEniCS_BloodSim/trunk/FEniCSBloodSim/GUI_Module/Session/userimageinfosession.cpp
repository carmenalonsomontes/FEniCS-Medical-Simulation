#include "userimageinfosession.h"

UserImageInfoSession::UserImageInfoSession()
{
    initializeValues();
}


void UserImageInfoSession::initializeValues()
{
    _noImageRow = -1;
    _noSlice = -1;
    _minSlice = -1;
    _maxSlice = -1;
    _sliceType = NON_TYPE_SELECTED;
}

int UserImageInfoSession::getNoImageRow()
{
    return _noImageRow;
}

int UserImageInfoSession::getNoSlice()
{
    return _noSlice;
}
int UserImageInfoSession::getMaxSlice()
{
    return _maxSlice;
}
int UserImageInfoSession::getMinSlice()
{
    return _minSlice;
}

ImageTabs UserImageInfoSession::getSliceType()
{
    return _sliceType;
}


void UserImageInfoSession::setNoImageRow(int row)
{
    _noImageRow = row;
}
void UserImageInfoSession::setNoSlice(int slice)
{
    _noSlice = slice;
}
void UserImageInfoSession::setMaxSlice(int max)
{
    _maxSlice = max;
}
void UserImageInfoSession::setMinSlice(int min)
{
    _minSlice = min;
}

void UserImageInfoSession::setSliceType(ImageTabs type)
{
    _sliceType = type;
}


void UserImageInfoSession::clear()
{
    initializeValues();
}
bool UserImageInfoSession::isEmpty()
{
    if( (_noImageRow != -1) || (_noSlice != -1) || (_minSlice != -1) ||
            (_maxSlice != -1) || (_sliceType != NON_TYPE_SELECTED))
        return true;
    return false;
}

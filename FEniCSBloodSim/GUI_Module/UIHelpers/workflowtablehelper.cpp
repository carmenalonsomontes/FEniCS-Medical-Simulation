#include "workflowtablehelper.h"

#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include <QPixmap>

WorkflowTableHelper::WorkflowTableHelper()
{
}

void WorkflowTableHelper::registerTableUI(QTableWidget * _table)
{
    registeredTable = _table;
}


void WorkflowTableHelper::addElementToTable(QString _action,int iconType)
{
    int _lastRow = registeredTable->rowCount();
    registeredTable->insertRow(_lastRow);

    // Add Action
    registeredTable->setItem(_lastRow,DESC_COLUMN,addNonEditableText(_action));
    registeredTable->setCellWidget(_lastRow,EYE_COLUMN,addIcon(iconType));
    registeredTable->resizeColumnsToContents();

    eyeTableOpen.append(iconType);


}
void WorkflowTableHelper::clearTable()
{
    int _lastRow = registeredTable->rowCount();
    for (int i= _lastRow-1; i>=0; i--)
        registeredTable->removeRow(i);

}

QTableWidgetItem * WorkflowTableHelper::addNonEditableText(QString _value)
{
    QTableWidgetItem * _defaultTxt = new QTableWidgetItem(_value);
    _defaultTxt->setFlags(_defaultTxt->flags() &= ~Qt::ItemIsEditable);
    return _defaultTxt;

}


QLabel * WorkflowTableHelper::addIcon(int iconType)
{
    QPixmap icon;

    switch(iconType)
    {
    case EYE_OPEN:
        icon.load(":/frame/GUI_Module/images/frame/eye.png");
        break;
    case EYE_CLOSED:
        icon.load(":/frame/GUI_Module/images/frame/eye_close.png");
        break;
    }

    QLabel * _labelWithIcon = new QLabel;
    _labelWithIcon->setPixmap(icon);
    _labelWithIcon->setAlignment(Qt::AlignCenter);
    return _labelWithIcon;

}


int WorkflowTableHelper::modifyEyeInRow(int row)
{
    int _cValue = eyeTableOpen.at(row);
    int iconType = EYE_CLOSED;
    if (_cValue == EYE_CLOSED)
        iconType = EYE_OPEN;
    registeredTable->removeCellWidget(row, EYE_COLUMN); // removing old one
    registeredTable->setCellWidget(row,EYE_COLUMN,addIcon(iconType));
    eyeTableOpen.replace(row,iconType);
    return iconType;
}

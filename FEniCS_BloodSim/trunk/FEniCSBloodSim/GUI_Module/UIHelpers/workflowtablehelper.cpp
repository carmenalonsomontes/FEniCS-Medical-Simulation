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

void WorkflowTableHelper::registerTableListUI(QTableWidget * _table)
{
        registeredTableList.append(_table);

}

void WorkflowTableHelper::setCurrentRegisteredTableFromList(int index)
{
    if (index < registeredTableList.size())
        registeredTable = registeredTableList.at(index);
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


void WorkflowTableHelper::addOnlyDesc(QString _action)
{
    int _lastRow = registeredTable->rowCount();
    registeredTable->insertRow(_lastRow);

    // Add Action
    registeredTable->setItem(_lastRow,ONLY_DESC_COLUMN,addNonEditableText(_action));
    registeredTable->setItem(_lastRow,CHECKABLE_COLUMN,addSelectableCheckColumn());
    registeredTable->resizeColumnsToContents();
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
    if (eyeTableOpen.isEmpty())
        return -1;
    int _cValue = eyeTableOpen.at(row);
    int iconType = EYE_CLOSED;
    if (_cValue == EYE_CLOSED)
        iconType = EYE_OPEN;
    registeredTable->removeCellWidget(row, EYE_COLUMN); // removing old one
    registeredTable->setCellWidget(row,EYE_COLUMN,addIcon(iconType));
    eyeTableOpen.replace(row,iconType);
    return iconType;
}


QTableWidgetItem * WorkflowTableHelper::addSelectableCheckColumn()
{
    QTableWidgetItem * _checkStatusItem = new QTableWidgetItem("");
    _checkStatusItem->setCheckState(Qt::Unchecked);
    return _checkStatusItem;
}


bool WorkflowTableHelper::isRegistered(int index)
{
    bool isRegistered = true;
    if (index >= registeredTableList.size()) isRegistered = false;
    return isRegistered;
}

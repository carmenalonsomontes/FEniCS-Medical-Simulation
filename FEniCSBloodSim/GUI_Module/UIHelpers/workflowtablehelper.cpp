#include "workflowtablehelper.h"


#include <QPixmap>
#include <QIcon>
#include <QTableWidgetItem>



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

QTableWidget * WorkflowTableHelper::getRegisteredTable()
{
    return registeredTable;
}

void WorkflowTableHelper::setCurrentRegisteredTableFromList(int index)
{
    if (index < registeredTableList.size())
        registeredTable = registeredTableList.at(index);
}



void WorkflowTableHelper::addElementToTable(QString _action,ImProcMenuUI iconType)
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


void WorkflowTableHelper::addMethodsAndAction(QString _action, IconType _icon)
{
    int _lastRow = registeredTable->rowCount();
    registeredTable->insertRow(_lastRow);

    // Add Action
    registeredTable->setItem(_lastRow,METHOD_NAME_COLUMN,addNonEditableText(_action));
    registeredTable->setItem(_lastRow,ACTION_COLUMN,addIcon(_icon));
    registeredTable->resizeColumnsToContents();
}

#include <QPixmap>


int  WorkflowTableHelper::addEmptyRow()
{
    int _lastRow = registeredTable->rowCount();
    registeredTable->insertRow(_lastRow);

    registeredTable->resizeColumnsToContents();

    return _lastRow;
}


 void WorkflowTableHelper::addParameterRow (QString paramName, QString paramType, QString paramClassName, QString _value)
 {
     int _lastRow = registeredTable->rowCount();
     registeredTable->insertRow(_lastRow);

     // Add Action
     registeredTable->setItem(_lastRow,PARAM_TYPE_COLUMN,addNonEditableText(paramType));
     registeredTable->setItem(_lastRow,PARAM_NAME_COLUMN,addNonEditableText(paramName));
     registeredTable->setItem(_lastRow,PARAM_VALUE_COLUMN,addEditText(_value));
     registeredTable->setItem(_lastRow,PARAM_CLASS_NAME_COLUMN,addNonEditableText(paramClassName));

     registeredTable->resizeColumnsToContents();
 }


void WorkflowTableHelper::updateRow(QString _iconPath,QString _value, int row)
{
    if (row > registeredTable->rowCount())
        return;
    registeredTable->setItem(row,CHECKABLE_COLUMN,addIcon(_iconPath));
    registeredTable->setItem(row,ONLY_DESC_COLUMN,addNonEditableText(_value));
    registeredTable->setItem(row,UP_ICON_COLUMN,addIcon(UP_ICON));
    registeredTable->setItem(row,DOWN_ICON_COLUMN,addIcon(DOWN_ICON));
    registeredTable->setItem(row,DELETE_ICON_COLUMN,addIcon(DELETE_ICON));
    registeredTable->resizeColumnsToContents();
}


QTableWidgetItem * WorkflowTableHelper::addEditText(QString _value)
{
     QTableWidgetItem * _defaultTxt = new QTableWidgetItem(_value);
     return _defaultTxt;
}


QTableWidgetItem * WorkflowTableHelper::addIcon(QString _path)
{
    QIcon icon(_path);
    QTableWidgetItem *icon_item = new QTableWidgetItem;
    icon_item->setIcon(icon);
    icon_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    return icon_item;
}

QTableWidgetItem * WorkflowTableHelper::addIcon(IconType _iconType)
{
    QIcon icon;

    switch (_iconType) {
    case ADD_ICON:
        icon.addPixmap(QPixmap(ADD_BUTTON_ICON_FILE_PATH));
        break;
    case UP_ICON:
        icon.addPixmap(QPixmap(UP_ICON_PATH));
        break;
    case DOWN_ICON:
        icon.addPixmap(QPixmap(DOWN_ICON_PATH));
        break;
    case DELETE_ICON:
        icon.addPixmap(QPixmap(DELETE_ICON_PATH));
        break;
    default:
        break;
    }

    QTableWidgetItem *icon_item = new QTableWidgetItem;
    icon_item->setIcon(icon);
    icon_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    return icon_item;
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


QLabel * WorkflowTableHelper::addIcon(ImProcMenuUI iconType)
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
    ImProcMenuUI iconType = EYE_CLOSED;
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


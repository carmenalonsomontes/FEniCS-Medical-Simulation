#ifndef WORKFLOWTABLEHELPER_H
#define WORKFLOWTABLEHELPER_H

#include <QTableWidget>
#include <QString>
#include <QTableWidgetItem>
#include <QLabel>
#include <QList>

#include "GUI_Module/Defines/Workflow/WorkflowDefines.h"

#include "GUI_Module/Defines/Menu/MenuDefines.h"


class WorkflowTableHelper
{
public:
    WorkflowTableHelper();

    void registerTableUI(QTableWidget * _table);
    QTableWidget * getRegisteredTable();

    void registerTableListUI(QTableWidget * _table);

    void setCurrentRegisteredTableFromList(int index);


    void addElementToTable(QString _action,ImProcMenuUI iconType);

    void addOnlyDesc(QString _action);
    int addEmptyRow();
    int addHiddenEmptyRow();

    void addParameterRow (QString paramName, QString paramType, QString paramClassName, QString _value);

    void updateRow(QString _iconPath, QString _value, int row);

    int modifyEyeInRow(int row);

    void clearTable();

    bool isRegistered(int index);

    void addMethodsAndAction(QString _action,IconType _icon);

    void removeRowsFromThisOnwards(int initRow);


private:
    QTableWidget * registeredTable;
    QList<QTableWidget *> registeredTableList;
    QList<int> eyeTableOpen;

    QTableWidgetItem * addNonEditableText(QString _value);
    QLabel * addIcon(ImProcMenuUI iconType);
    QTableWidgetItem * addSelectableCheckColumn();
    QTableWidgetItem * addIcon(QString _path);
    QTableWidgetItem * addEditText(QString _value);
    QTableWidgetItem * addIcon(IconType _iconType);

};

#endif // WORKFLOWTABLEHELPER_H

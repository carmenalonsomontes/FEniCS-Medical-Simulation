#ifndef WORKFLOWTABLEHELPER_H
#define WORKFLOWTABLEHELPER_H

#include <QTableWidget>
#include <QString>
#include <QTableWidgetItem>
#include <QLabel>
#include <QList>

class WorkflowTableHelper
{
public:
    WorkflowTableHelper();

    void registerTableUI(QTableWidget * _table);

    void registerTableListUI(QTableWidget * _table);

    void setCurrentRegisteredTableFromList(int index);


    void addElementToTable(QString _action,int iconType);

    void addOnlyDesc(QString _action);
    int addEmptyRow();

    void addParameterRow (QString paramName, QString paramType, QString paramClassName, QString _value);
    //_parameterName,_parameterType,_parameterClassName);

    void updateRow(QString _iconPath, QString _value, int row);

    int modifyEyeInRow(int row);

    void clearTable();

    bool isRegistered(int index);


private:
    QTableWidget * registeredTable;
    QList<QTableWidget *> registeredTableList;
    QList<int> eyeTableOpen;

    QTableWidgetItem * addNonEditableText(QString _value);
    QLabel * addIcon(int iconType);
    QTableWidgetItem * addSelectableCheckColumn();
    QTableWidgetItem * addIcon(QString _path);
    QTableWidgetItem * addEditText(QString _value);

};

#endif // WORKFLOWTABLEHELPER_H

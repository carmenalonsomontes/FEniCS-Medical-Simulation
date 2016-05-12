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
};

#endif // WORKFLOWTABLEHELPER_H

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

    void addElementToTable(QString _action,int iconType);

    void addOnlyDesc(QString _action);

    int modifyEyeInRow(int row);

    void clearTable();
private:
    QTableWidget * registeredTable;
    QList<int> eyeTableOpen;

    QTableWidgetItem * addNonEditableText(QString _value);
    QLabel * addIcon(int iconType);
    QTableWidgetItem * addSelectableCheckColumn();
};

#endif // WORKFLOWTABLEHELPER_H

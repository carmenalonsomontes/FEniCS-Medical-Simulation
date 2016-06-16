#ifndef COMBOVALUESFENICSDATA_H
#define COMBOVALUESFENICSDATA_H

#include <QString>

class ComboValuesFEniCSData
{
public:
    ComboValuesFEniCSData();

    QString getName();
    QString getFenicsValue();

    void setName(QString val);
    void setFenicsValue(QString val);

    void clear();
    bool isEmpty();

private:
    QString _name;
    QString _fenicsValue;

};

#endif // COMBOVALUESFENICSDATA_H

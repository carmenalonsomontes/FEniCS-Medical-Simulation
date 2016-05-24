#ifndef CONFIGURATIONPIPELINEITEM_H
#define CONFIGURATIONPIPELINEITEM_H
#include <QString>
#include <QList>

class ConfigurationPipelineItem
{
public:
    ConfigurationPipelineItem();

    QString getOptionName();
    QString getOptionType();
    QString getOptionDefaultValue();

    void setOptionName(QString newName);
    void setOptionType(QString newName);
    void setOptionDefaultValue(QString newName);


    QString getCurrentValue();
    void setCurrentValue(QString newValue);

    QString getClassName();
    void setClassName(QString newName);

    bool isEmpty();
    void clear();

private:
    QString _optionName;
    QString _optionType;
    QString _optionDefaultValue;
    QString _className;
    QString _cValue;
};

#endif // CONFIGURATIONPIPELINEITEM_H

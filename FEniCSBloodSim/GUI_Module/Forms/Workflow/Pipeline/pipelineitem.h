#ifndef PIPELINEITEM_H
#define PIPELINEITEM_H

#include <QString>


class PipelineItem
{
public:
    PipelineItem();

    QString getIconPath();
    QString getDescription();
    int getPositionPipeline();



    void setIconPath(QString newPath);
    void setDescription(QString newDesc);
    void setPositionPipeline(int newPos);

    QString getCategoryName();
    void setCategoryName(QString newCatName);

    bool isEmpty();
    void clear();
private:
    QString _categoryName;
    QString _iconPath;
    QString _description;
    int _positionPipeline;
};

#endif // PIPELINEITEM_H

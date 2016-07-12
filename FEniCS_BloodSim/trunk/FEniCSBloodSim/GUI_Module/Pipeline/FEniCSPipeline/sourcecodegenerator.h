#ifndef SOURCECODEGENERATOR_H
#define SOURCECODEGENERATOR_H

#include <QString>
#include <QList>
#include <QTextStream>

#include "fenicspipelinedata.h"
#include "fenicsparameterpipelinedata.h"

class SourceCodeGenerator
{
public:
    SourceCodeGenerator();

    void setPipelineData(QList<FEniCSPipelineData> _data);

    void generateSourceCode();
    QString getSourceCode();

    void setFilePath(QString _path);

private:
   QList<FEniCSPipelineData> _pipelineData;


   void buildImports(QTextStream * stream);
   void buildMesh(QTextStream * stream);
   void buildBoundaries(QTextStream * stream);
   void buildSolutionComputation(QTextStream * stream);
   void buildSaveSolution(QTextStream * stream);
   void buildPlot(QTextStream * stream);

   QString _filePath;


};

#endif // SOURCECODEGENERATOR_H

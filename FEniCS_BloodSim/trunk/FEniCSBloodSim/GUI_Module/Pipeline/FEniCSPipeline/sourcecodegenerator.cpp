#include "sourcecodegenerator.h"

#include "GUI_Module/Defines/FEniCS/FEniCSDefines.h"

#include <QFile>


SourceCodeGenerator::SourceCodeGenerator()
{
}


void SourceCodeGenerator::setPipelineData(QList<FEniCSPipelineData> _data)
{
    _pipelineData = _data;
}

void SourceCodeGenerator::generateSourceCode()
{

    if (_pipelineData.isEmpty())
        return;
    QFile _file(_filePath);
    if ( _file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &_file );

        buildImports(&stream);
    /*_text = _text + buildMesh();
    _text = _text + buildBoundaries();
    _text = _text + buildSolutionComputation();
    _text = _text +  buildSaveSolution();
    _text = _text + buildPlot();
*/

      _file.close();
    }



}

QString SourceCodeGenerator::getSourceCode()
{
    QString _text;
    QFile _file(_filePath);
    if ( _file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &_file );
        _text = stream.readAll();
        _file.close();
    }
    return _text;
}

void SourceCodeGenerator::buildImports(QTextStream * stream)
{
    for (int i = 0; i < _pipelineData.size();i++)
    {
        FEniCSPipelineData _item = _pipelineData.at(i);
        if (QString::compare(_item.getCategoryName(),FENICS_LIBRARY_NAME) == 0)
        {
            QList<FEniCSParameterPipelineData> _list = _item.getParameterPipelineItems();
            if (_list.isEmpty())
                (* stream) <<"from "+_item.getFunctionFenicsName()+ " import *" << endl;
            else
                for (int i = 0; i< _list.size();i++)
                {
                    FEniCSParameterPipelineData _paramter = _list.at(i);
                    (* stream) <<"from "+_item.getFunctionFenicsName()+ " import "+ _paramter.getMethodName() << endl;
                }
        }
    }
}

QString SourceCodeGenerator::buildMesh()
{
}

QString SourceCodeGenerator::buildBoundaries()
{
}

QString SourceCodeGenerator::buildSolutionComputation()
{
}

QString SourceCodeGenerator::buildSaveSolution()
{
}

QString SourceCodeGenerator::buildPlot()
{
}

void SourceCodeGenerator::setFilePath(QString _path)
{
    _filePath = _path;
}

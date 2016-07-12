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

    // If file exists we remoe it
    if (_file.exists())
        _file.remove();

    if ( _file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &_file );

        buildImports(&stream);
        buildMesh(&stream);
        buildBoundaries(&stream);
        buildSolutionComputation(&stream);
        buildSaveSolution(&stream);
        buildPlot(&stream);

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
                    FEniCSParameterPipelineData _parameter = _list.at(i);
                    (* stream) <<"from "+_item.getFunctionFenicsName()+ " import "+ _parameter.getMethodName() << endl;
                }
        }
    }
}

void SourceCodeGenerator::buildMesh(QTextStream * stream)
{


    for (int i = 0; i < _pipelineData.size();i++)
    {
        FEniCSPipelineData _item = _pipelineData.at(i);
        if (QString::compare(_item.getCategoryName(),FENICS_MESH_NAME) == 0)
        {
            QList<FEniCSParameterPipelineData> _list = _item.getParameterPipelineItems();

            if ( (QString::compare(_item.getFunctionName(),FENICS_USER_MESH_NAME) == 0) &&
                 (!_list.isEmpty()))
            {
                FEniCSParameterPipelineData _parameter = _list.at(0);
                (* stream) << FENICS_FILE_MESH_VAR_NAME << "="+_item.getFunctionFenicsName()+ "(\" "+
                             _parameter.getSelectedValue()<< "\")" << endl;

            }
            else
            {
                // For each function I obtain the parameters
                (* stream) << FENICS_FILE_MESH_VAR_NAME << " = "+
                              _item.getFunctionFenicsName()+"(";
                for (int i = 0; i< _list.size();i++)
                {
                    FEniCSParameterPipelineData _parameter = _list.at(i);
                    (* stream) <<  _parameter.getSelectedValue();

                    if (i != _list.size()-1)
                        (* stream) << ",";
                }
                (* stream) << ")" << endl;
            }
        }
    }
}

void SourceCodeGenerator::buildBoundaries(QTextStream * stream)
{
}

void SourceCodeGenerator::buildSolutionComputation(QTextStream * stream)
{
}

void SourceCodeGenerator::buildSaveSolution(QTextStream * stream)
{
}

void SourceCodeGenerator::buildPlot(QTextStream * stream)
{
}

void SourceCodeGenerator::setFilePath(QString _path)
{
    _filePath = _path;
}

#ifndef ADAPTIVEHISTOGRAMEQUALIZATION_H
#define ADAPTIVEHISTOGRAMEQUALIZATION_H
#include "../ioperation.h"
#include <QStringList>
#include "itkAdaptiveHistogramEqualizationImageFilter.h"
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "GUI_Module/Defines/Workflow/WorkflowDefines.h"

typedef  itk::AdaptiveHistogramEqualizationImageFilter< ImageType2D > AdaptiveHistogramEqualizationImageFilterType2D;
//typedef  itk::AdaptiveHistogramEqualizationImageFilter< ImageType3D > AdaptiveHistogramEqualizationImageFilterType3D;

// Note: ONLY FOR 2D CASES!!

class AdaptiveHistogramEqualization: public IOperation
{
public:
    AdaptiveHistogramEqualization();

    void SetInPut(ImageType2D::Pointer _input);
    void SetInPut(ImageType3D::Pointer _input);

    void SetParameters(QStringList _parameterList);

    static IOperation * Create();

    ImageType2D::Pointer GetOutput2D();
    ImageType3D::Pointer GetOutput3D();

    void set3D(bool _val);
    bool is3D();

    void exec();
    void save(QString _path);

private:

    bool _is3D;
    AdaptiveHistogramEqualizationImageFilterType2D::Pointer filter2D;
    //AdaptiveHistogramEqualizationImageFilterType3D::Pointer filter3D;

    void setParameters3D(QStringList _parameterList);
    void setParameters2D(QStringList _parameterList);

    void save3D(QString _path);
    void save2D(QString _path);
};

#endif // ADAPTIVEHISTOGRAMEQUALIZATION_H

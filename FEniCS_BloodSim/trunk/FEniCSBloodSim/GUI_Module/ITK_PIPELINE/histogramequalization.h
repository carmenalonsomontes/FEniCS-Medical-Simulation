#ifndef HISTOGRAMEQUALIZATION_H
#define HISTOGRAMEQUALIZATION_H

#include "itkpipelineoperation.h"
class HistogramEqualization: public ItkPipelineOperation
{
public:
    HistogramEqualization();

    void exec();
};

#endif // HISTOGRAMEQUALIZATION_H

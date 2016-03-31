#include "imageprocessingsimmenu.h"

#include "GUI_Module/Forms/Imaging/medicalimagingdialog.h"

ImageProcessingSimMenu::ImageProcessingSimMenu()
{
}

void ImageProcessingSimMenu::launchMenuAction(int _action)
{
    switch (_action) {
    case IMAGE_PROCESSING:
        openImagingDialog();
        break;
    case MESH_TOOL:

        break;
    case FENICS_TOOL:

        break;
    case VISUALIZATION_TOOL:

        break;

    default:
        break;
    }

}


void ImageProcessingSimMenu::openImagingDialog()
{
    MedicalImagingDialog _setImagingParameterDialog;

    _setImagingParameterDialog.exec();
}

/*
void ImageProcessingSimMenu::updateImagingUI(int _menu)
{

}

void ImageProcessingSimMenu::enableImagingUIElements(int _menu)
{

}
*/

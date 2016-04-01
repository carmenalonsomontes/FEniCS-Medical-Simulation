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
        updateImagingDialog();
        break;
    case MESH_TOOL:
        // TODO
        break;
    case FENICS_TOOL:
        // TODO
        break;
    case VISUALIZATION_TOOL:
        // TODO
        break;

    default:
        break;
    }

}

void ImageProcessingSimMenu::launchMenuAction(int _action,QString _pathfile)
{
    switch (_action) {
    case IMAGE_PROCESSING:
        _imgPath = _pathfile;
        openImagingDialog();
        updateImagingDialog();
        break;
    case MESH_TOOL:
        // TODO
        break;
    case FENICS_TOOL:
        // TODO
        break;
    case VISUALIZATION_TOOL:
        // TODO
        break;

    default:
        break;
    }
}

void ImageProcessingSimMenu::openImagingDialog()
{
    MedicalImagingDialog _setImagingParameterDialog;

    if (!_imgPath.isEmpty())
        _setImagingParameterDialog.setImagePath(_imgPath);

    _setImagingParameterDialog.exec();

    // Updating the values in the main window
     _imgPath = _setImagingParameterDialog.getImagePath();

}

void ImageProcessingSimMenu::updateImagingDialog()
{
     emit updateImagingDialogUI(_imgPath);
}


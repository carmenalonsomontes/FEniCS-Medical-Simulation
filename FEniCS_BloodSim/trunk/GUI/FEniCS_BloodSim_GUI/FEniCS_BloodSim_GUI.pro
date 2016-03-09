#-------------------------------------------------
#
# Project created by QtCreator 2016-03-01T13:19:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = FEniCS_BloodSim_GUI
TEMPLATE = app

#--------------- VTK STUFF -------------------------------
#LIBS += -lvtk #-L/usr/local/lib/vtk-5.10 #folder to your VTK library - MIRAR !!!!!!!!!!!!!!!

#LIBS += -lvtkCommon \
#        -lvtkRendering \



#INCLUDEPATH += /usr/local/include/vtk-5.10 #folder to your VTK include files - MIRAR!!!!!!!!!!111

#CONFIG -= import_plugins
#----------------------------------


SOURCES += main.cpp\
        fenics_bs_gui.cpp

HEADERS  += fenics_bs_gui.h

FORMS    += fenics_bs_gui.ui

OTHER_FILES += \
    CMakeLists.txt


unix|win32: LIBS +=  -lvtkWidgets\
    -lvtkCommon \
    -lvtkRendering \
    -lvtkVolumeRendering \
    -lQVTK \
    -lvtkIO \
    -lvtkFiltering \
  #  -lvtkgdcm \


project(VesselSegmentation)
cmake_minimum_required(VERSION 2.8)
SET(CMAKE_BUILD_TYPE Debug)

 # Tell CMake to run moc when necessary:
  set(CMAKE_AUTOMOC ON)
  # As moc files are generated in the binary dir, tell CMake
  # to always look for includes there:
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  # Widgets finds its own dependencies (QtGui and QtCore).
  find_package(Qt5Widgets REQUIRED)

  # The Qt5Widgets_INCLUDES also includes the include directories for
  # dependencies QtCore and QtGui
  include_directories(${Qt5Widgets_INCLUDES})

  # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
  add_definitions(${Qt5Widgets_DEFINITIONS})

  # Executables fail to build with Qt 5 in the default configuration
  # without -fPIE. We add that here.
  set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

  #add_executable(hello_world main.cpp mainwindow.cpp)

  # The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
  #target_link_libraries(hello_world ${Qt5Widgets_LIBRARIES})




# Find ITK packages
find_package(ITK REQUIRED)
if(ITK_FOUND)
    include(${ITK_USE_FILE})
endif(ITK_FOUND)

# Find VTK packages
if(ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
 include(${VTK_USE_FILE})
else()
find_package(ItkVtkGlue REQUIRED)
include(${ItkVtkGlue_USE_FILE})
set(Glue ItkVtkGlue)
endif()

#---------------------------------------------
# If we want to create a Qt interface
# Check help ITK related to QT in official webpag
# http://www.itk.org/CourseWare/Training/GettingStarted-IV.pdf

#find_package(QT)
#if(QT_FOUND)
#    include_directories(${QT_INCLUDE_DIR})
#endif(QT_FOUND)
#LINK_LIBRARIES(ITKIO ${QT_LIBRARIES})
#SET(QtITK_SRCS itkQtProgressBar.cxx qtITK.cxx)
#SET(QtITK_MOC_SRCS itkQtAdaptor.h itkQtLightIndicator.h)
#if(QT_WRAP_CPP)
#   qt_wrap_cpp(QtITK ${QtITK_SRCS} ${QtITK_MOC_SRCS})
#endif(QT_WRAP_CPP)
#add_executable(QtITK QtITK_SRCS)

#------------------------------


aux_source_directory(. SRC_LIST)
# Add so many aux_source_directory as folders have your project
aux_source_directory(Helper/Image HELPER_SRC_LIST)
aux_source_directory(Data DATA_SRC_LIST)
aux_source_directory(Defines DEFINES_SRC_LIST)

# To add all the headers
set(HEADER_FILES Defines/GeneralImageDefines.h)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HELPER_SRC_LIST} ${DATA_SRC_LIST} ${DEFINES_SRC_LIST}  ${HEADER_FILES})

# Link with the libraries
target_link_libraries(${PROJECT_NAME} ${Glue} ${ITK_LIBRARIES}  ${VTK_LIBRARIES} ${Qt5Widgets_LIBRARIES})




#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})


#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})
#-----------------------------------------------------------------------------------
# PROJECT CONFIGURATION
project(FEniCSBloodSim)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_AUTORCC ON)
# GUI Modules files
file(GLOB_RECURSE UI_FILES *.ui)
file(GLOB_RECURSE QT_WRAP *.h)
file(GLOB_RECURSE CXX_FILES *.cpp)
file(GLOB_RECURSE QRC_FILES *.qrc)

# Adding files for QT
SET(QRC_FILES "GUI_Module/fenics_blood_sim.qrc")

#-----------------------------------------------------------------------------------
# CMAKE CONFIGURATION
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()


#-----------------------------------------------------------------------------------
# ITK CONFIGURATION.
find_package(ITK REQUIRED)
if(ITK_FOUND)
    include(${ITK_USE_FILE})
endif(ITK_FOUND)

#-----------------------------------------------------------------------------------
# ITKVTKGLUE CONFIGURATION
if(ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
 include(${VTK_USE_FILE})
else()
find_package(ItkVtkGlue REQUIRED)
include(${ItkVtkGlue_USE_FILE})
set(Glue ItkVtkGlue)
endif()


#-----------------------------------------------------------------------------------
# VTK CONFIGURATION
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  # Instruct CMake to run moc automatically when needed.
  set(CMAKE_AUTOMOC ON)
  find_package(Qt5Widgets REQUIRED QUIET)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()

#-----------------------------------------------------------------------------------
# LIBRARIES & EXECUTABLE CONFIGURATION


if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_ui(UISrcs ${UI_FILES} )
    # EXECUTABLE-------------------
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${QT_WRAP})
  qt5_use_modules(${PROJECT_NAME} Core Gui)
  target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})
else()
  QT4_WRAP_UI(UISrcs ${UI_FILES})
  QT4_WRAP_CPP(MOCSrcs ${QT_WRAP})
  QT4_ADD_RESOURCES(QRC_RESOURCES_RCC ${QRC_FILES})
    # EXECUTABLE-------------------
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${MOCSrcs} ${QRC_RESOURCES_RCC})
    # VTK + QT LIBRARIES
  if(VTK_LIBRARIES)
    if(${VTK_VERSION} VERSION_LESS "6")
      target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES} QVTK)
    else()
      target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})
    endif()
  else()
    target_link_libraries(${PROJECT_NAME} vtkHybrid QVTK vtkViews ${QT_LIBRARIES})
  endif()
endif()

# ITK + ITKVTKGLUE
target_link_libraries(${PROJECT_NAME} ${Glue} ${ITK_LIBRARIES})


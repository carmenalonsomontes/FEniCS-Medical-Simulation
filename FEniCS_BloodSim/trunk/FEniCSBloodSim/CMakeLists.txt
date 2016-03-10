#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})
#-----------------------------------------------------------------------------------
# PROJECT CONFIGURATION
project(FEniCSBloodSim)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})


file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *.h)
file(GLOB CXX_FILES *.cpp)



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



if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_ui(UISrcs ${UI_FILES} )
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  add_executable(RenderWindowUISingleInheritance MACOSX_BUNDLE
    ${CXX_FILES} ${UISrcs} ${QT_WRAP})
  qt5_use_modules(RenderWindowUISingleInheritance Core Gui)
  target_link_libraries(RenderWindowUISingleInheritance ${VTK_LIBRARIES})
else()
  QT4_WRAP_UI(UISrcs ${UI_FILES})
  QT4_WRAP_CPP(MOCSrcs ${QT_WRAP})
  add_executable(RenderWindowUISingleInheritance MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${MOCSrcs})

  if(VTK_LIBRARIES)
    if(${VTK_VERSION} VERSION_LESS "6")
      target_link_libraries(RenderWindowUISingleInheritance ${VTK_LIBRARIES} QVTK)
    else()
      target_link_libraries(RenderWindowUISingleInheritance ${VTK_LIBRARIES})
    endif()
  else()
    target_link_libraries(RenderWindowUISingleInheritance vtkHybrid QVTK vtkViews ${QT_LIBRARIES})
  endif()
endif()

###################################################################3
target_link_libraries(RenderWindowUISingleInheritance ${Glue} ${ITK_LIBRARIES})
###################################################################

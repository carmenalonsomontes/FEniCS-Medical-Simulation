#project(VascularSegmentation)
#cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})

#-----------------------------------------------------------------------------------
# CMAKE CONFIGURATION
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

#-----------------------------------------------------------------------------------
# PROJECT CONFIGURATION
project(VascularSegmentation)


#-----------------------------------------------------------------------------------
# ITK CONFIGURATION.
find_package(ITK 4.10 REQUIRED)
if(ITK_FOUND)
    include(${ITK_USE_FILE})
endif(ITK_FOUND)

#-----------------------------------------------------------------------------------
# ITKVTKGLUE CONFIGURATION
#if(ITKVtkGlue_LOADED)
  find_package(VTK 7.0 REQUIRED)
 include(${VTK_USE_FILE})



aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
# ITK + ITKVTKGLUE
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES} ${ItkVtkGlue} ${ITK_LIBRARIES})

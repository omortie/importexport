#ImportExport Library
project(ImportExport)

find_package(Qt6 REQUIRED COMPONENTS Sql)

if(DEFINED BUILD_WITH_SCRIPT_SUPPORT)
    find_package(PythonQt REQUIRED)
endif(DEFINED BUILD_WITH_SCRIPT_SUPPORT)

#Import Sources
list(APPEND SRCS
    importer/src/csvreader.cpp
    importer/src/howpage.cpp
    importer/src/importer.cpp
    importer/src/importwizard.cpp
    importer/src/intropage.cpp
    importer/src/logger.cpp
    importer/src/previewpage.cpp
    importer/src/setrcpage.cpp)

list(APPEND HDRS
    importer/inc/csvreader.h
    importer/inc/howpage.h
    importer/inc/importer.h
    importer/inc/importwizard.h
    importer/inc/intropage.h
    importer/inc/logger.h
    importer/inc/previewpage.h
    importer/inc/setrcpage.h
    importer/inc/wizardpages.h)

#Export Sources
list(APPEND SRCS
    exporter/src/asciitab.cpp
    exporter/src/captiontab.cpp
    exporter/src/csvexport.cpp
    exporter/src/exportdialog.cpp
    exporter/src/exporter.cpp
    exporter/src/hftab.cpp
    exporter/src/selecttab.cpp
    exporter/src/tabs.cpp
    exporter/src/typetab.cpp)

list(APPEND HDRS
    exporter/inc/asciitab.h
    exporter/inc/captiontab.h
    exporter/inc/csvexport.h
    exporter/inc/exportdialog.h
    exporter/inc/exporter.h
    exporter/inc/hftab.h
    exporter/inc/selecttab.h
    exporter/inc/tabs.h
    exporter/inc/typetab.h)

if(DEFINED BUILD_WITH_SCRIPT_SUPPORT)
    list(APPEND SRCS
        importer/src/scriptorpage.cpp
        importer/src/scriptgenerator.cpp)
    list(APPEND HDRS
        importer/inc/scriptorpage.h
        importer/inc/scriptgenerator.h)
endif(DEFINED BUILD_WITH_SCRIPT_SUPPORT)

list(APPEND SRCS settings.cpp)
list(APPEND HDRS settings.h)

file(GLOB RES importer/res/import.qrc
        exporter/res/export.qrc)

#Wrapping Qt Objects
qt6_wrap_cpp(MOC_SRCS ${HDRS})
qt6_add_resources(WRES ${RES})

#Headers
include_directories(importer/inc
        exporter/inc
        ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED BUILD_WITH_SCRIPT_SUPPORT)
    add_definitions(-DSCRIPT_SUPPORT)
endif(DEFINED BUILD_WITH_SCRIPT_SUPPORT)

#Defining Library
add_library(${CMAKE_PROJECT_NAME} STATIC ${SRCS} ${MOC_SRCS} ${WRES})
target_link_libraries(${CMAKE_PROJECT_NAME} Qt6::Core Qt6::Widgets Qt6::Sql)
if(DEFINED BUILD_WITH_SCRIPT_SUPPORT)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PythonQtLib})
endif(DEFINED BUILD_WITH_SCRIPT_SUPPORT)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME}${BUILD_POSTFIX})

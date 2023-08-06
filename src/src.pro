QT       += widgets sql

TARGET = ImportExport
TEMPLATE = lib
CONFIG += static
DESTDIR = $$PWD\..\lib\

# check if debug or release
CONFIG(debug, debug|release) {
  TARGET = $${TARGET}_d
}

INCLUDEPATH += exporter/inc \
             importer/inc \
             $$PWD

SOURCES +=  settings.cpp

HEADERS +=  settings.h \
    importer/inc/logger.h \
    importer/inc/scriptorpage.h \
    importer/inc/scriptgenerator.h \
    importer/inc/wizardpages.h \
    importer/inc/importwizard.h \
    importer/inc/importer.h \
    importer/inc/intropage.h \
    importer/inc/setrcpage.h \
    importer/inc/howpage.h \
    importer/inc/csvreader.h \
    importer/inc/previewpage.h

SOURCES += \
    importer/src/importwizard.cpp \
    importer/src/importer.cpp \
    importer/src/intropage.cpp \
    importer/src/setrcpage.cpp \
    importer/src/howpage.cpp \
    importer/src/csvreader.cpp \
    importer/src/previewpage.cpp \
    importer/src/logger.cpp \
    importer/src/scriptorpage.cpp \
    importer/src/scriptgenerator.cpp

HEADERS += \
    exporter/inc/captiontab.h \
    exporter/inc/csvexport.h \
    exporter/inc/exporter.h \
    exporter/inc/hftab.h \
    exporter/inc/selecttab.h \
    exporter/inc/tabs.h \
    exporter/inc/typetab.h \
    exporter/inc/exportdialog.h \
    exporter/inc/asciitab.h

SOURCES += \
    exporter/src/captiontab.cpp \
    exporter/src/csvexport.cpp \
    exporter/src/exporter.cpp \
    exporter/src/hftab.cpp \
    exporter/src/selecttab.cpp \
    exporter/src/tabs.cpp \
    exporter/src/typetab.cpp \
    exporter/src/exportdialog.cpp \
    exporter/src/asciitab.cpp

RESOURCES += \
    importer/res/import.qrc \
    exporter/res/export.qrc

include($$PWD/../../Modules-QMake/PythonQt.prf)
include($$PWD/../../Modules-QMake/ExtendedModel.prf)


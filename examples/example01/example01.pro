#-------------------------------------------------
#
# Project created by QtCreator 2018-03-29T09:42:11
#
#-------------------------------------------------

TEMPLATE = app
QT       += core widgets sql
DESTDIR = $$PWD/../../bin
TARGET = example01

include($$PWD/../../../Modules-QMake/ImportExport.prf)
include($$PWD/../../../Modules-QMake/ExtendedModel.prf)

# check if debug or release
CONFIG(debug, debug|release) {
  TARGET = $${TARGET}_d
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += inc

SOURCES += \
    src/main.cpp \
    src/widget.cpp

HEADERS += \
    inc/widget.h

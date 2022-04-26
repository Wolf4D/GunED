################################################################################
#
# FPSC's Gun editor (by Navy Lik)
#
# Requires Qt 5.x to compile, and gunspec.xml to run.
#
################################################################################

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#######################

CONFIG += c++11
RC_ICONS = icon.ico

#######################

VERSION = 0.2.5.1
VERSTR = '\\"$${VERSION}\\"'
DEFINES += VER=\"$${VERSTR}\"

#######################

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#######################

SOURCES += \
    airmsg.cpp \
    gunspecreader.cpp \
    gunspecxmlreader.cpp \
    main.cpp \
    mainwindow.cpp \
    propertywidget.cpp

HEADERS += \
    airmsg.h \
    gunspecreader.h \
    gunspecxmlreader.h \
    mainwindow.h \
    propertyfield.h \
    propertywidget.h

FORMS += \
    mainwindow.ui

#######################

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

################################################################################

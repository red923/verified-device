QT += core gui sql

CONFIG += c++17

TARGET = VisualIdentification
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/database.cpp \
    src/identifier.cpp

HEADERS += \
    src/mainwindow.h \
    src/database.h \
    src/identifier.h \
    src/objectmodel.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

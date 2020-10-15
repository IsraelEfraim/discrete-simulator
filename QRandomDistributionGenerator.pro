#-------------------------------------------------
#
# Project created by QtCreator 2019-09-11T00:33:26
#
#-------------------------------------------------

QT       += core gui charts widgets

TARGET = QRandomDistributionGenerator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        Distribution.cpp \
        DistributionRegex.cpp \
        ParallelSimulator.cpp \
        main.cpp \
        MainWindow.cpp \
        simulator/Export.cpp \
        simulator/QueuePolicy.cpp \
        simulator/Simulation.cpp \
        simulator/SimulationView.cpp

HEADERS += \
        Distribution.hpp \
        DistributionRegex.hpp \
        MainWindow.h \
        ParallelSimulator.hpp \
        simulator/Entity.hpp \
        simulator/Event.hpp \
        simulator/Export.hpp \
        simulator/QueuePolicy.hpp \
        simulator/Resource.hpp \
        simulator/Simulation.hpp \
        simulator/SimulationView.hpp \
        simulator/State.hpp \
        simulator/Types.hpp

FORMS += \
        MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    icons.qrc

RC_ICONS = icon.ico

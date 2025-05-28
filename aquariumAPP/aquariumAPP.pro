QT += quick
QT += charts
#QT += core gui
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ADS1115.cpp \
        GPIOActuator.cpp \
        MQueueHandler.cpp \
        PH.cpp \
        Sensors.cpp \
        ServoActuator.cpp \
        TDS.cpp \
        Temp.cpp \
        dataList.cpp \
        idealconditions.cpp \
        main.cpp \
        mainclass.cpp \
        msgqueue.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ADS1115.h \
    GPIOActuator.h \
    MQueueHandler.h \
    PH.h \
    Sensors.h \
    ServoActuator.h \
    TDS.h \
    Temp.h \
    dataList.h \
    idealconditions.h \
    main.h \
    mainclass.h \
    msgqueue.h

# DISTFILES += \
#     images/blueWpp.jpg \
#     images/check.png \
#     images/deadfish.png \
#     images/down_arrow.png \
#     images/fish.jpeg \
#     images/options.png \
#     images/return.png \
#     images/up_arrow.png

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


LIBS+=-L"/home/marco/proj/minime/lib104/API" -llib60870
INCLUDEPATH += /home/marco/proj/minime/lib104/API


SOURCES += \
    lib104/API/clientController.c \
    lib104/API/serverController.cpp \
    lib104/iecServerView.cpp \
    lib104/serverTableModel.cpp \
    lib104/startButtonDelegate.cpp \
    main.cpp


HEADERS += \
    lib104/API/Server104.h \
    lib104/API/cs104_slave.h \
    lib104/API/cs104_connection.h \
    lib104/API/serverController.h \
    lib104/iecServerView.h \
    lib104/serverTableModel.h \
    lib104/startButtonDelegate.h


FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

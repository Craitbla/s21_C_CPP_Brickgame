QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += sdk_no_version_check

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    internalwidgets.cc \
    mainwindow.cc \
    menuwidget.cc \
    gameview.cc \
    ../../../brick_game/common_back.c \
    ../../../brick_game/tetris/backend.c \
    ../../../brick_game/tetris/tetris_fsm_matrix.c \
    ../../../brick_game/snake/model.cc \
    ../../../brick_game/snake/snake_fsm_matrix.cc \
    ../../../main_desktop.cc

HEADERS += \
    ../../../brick_game/common_back.h \
    ../../../brick_game/snake/model.h \
    ../../../brick_game/tetris/backend.h \
    common_libs.h \
    gameview.h \
    internalwidgets.h \
    mainwindow.h \
    ../../controller.h \
    ../../i_view.h \
    menuwidget.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

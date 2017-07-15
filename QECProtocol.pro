#-------------------------------------------------
#
# Project created by QtCreator 2016-01-13T16:37:43
#
#-------------------------------------------------

#QT       += core gui
QT       += sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = QECProtocol
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    crashprob.cpp \
    graphwidget.cpp \
    edge.cpp \
    node.cpp \
    fnode.cpp \
    dialogsyndrome.cpp \
    nodesyndrome.cpp \
    edgesyndrome.cpp

HEADERS  += mainwindow.h \
    ../connection.h \
    qcustomplot.h \
    crashprob.h \
    graphwidget.h \
    edge.h \
    node.h \
    fnode.h \
    dialogsyndrome.h \
    nodesyndrome.h \
    edgesyndrome.h

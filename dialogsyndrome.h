#ifndef DIALOGSYNDROME_H
#define DIALOGSYNDROME_H

#include <QWidget>
#include <QtWidgets>
#include "graphwidget.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE


class DialogSyndrome : public QWidget
{
    Q_OBJECT
public:
    DialogSyndrome(QMenu *itemMenu, int pQb, int lQb, int q, int s, int NoAnc, QWidget *parent);

private slots:
    void syndWidgetF();
    void setSyndStepsF();
    void saveSyndF();

private:
    int pQb,physQbts,lQb,logQb,q,s,NoAnc,NoSyndSteps;
    GraphWidget *GWidgetSynd;
    QMenu *myMenu;
    QWidget *syndButtonWidget1, *syndButtonWidget2;
    QGridLayout *gridSynd;
    QLabel *label3;
    QGridLayout *grid;
    QComboBox *addAnc;
    QPushButton *saveSynd, *closeSynd;
    QHBoxLayout *syndButtonLayout1, *syndButtonLayout2;
    QComboBox *setSyndSteps;

signals:


public slots:

};

#endif // DIALOGSYNDROME_H

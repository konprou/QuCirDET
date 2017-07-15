#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QtWidgets>
#include <QMainWindow>
#include "crashprob.h"
#include "qcustomplot.h"
#include "node.h"
#include "dialogsyndrome.h"


QT_BEGIN_NAMESPACE
class QAction;
class QGraphicsView;//?
class QGraphicsScene;//?
class QGraphicsRectItem;//?
class QComboBox;
class QGroupBox;
class QToolBox;
class QButtonGroup;
class QGridLayout;
class QPushButton;
class QWidget;//?

class QGridLayout;
class QPushButton;
class QTableWidget;
class QDialogButtonBox;
class QPushButton;
class QSqlTableModel;
class QTableView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    //MainWindow(QWidget *parent = 0);
    //~MainWindow();
    int NoQ,NoS,NoAnc,NoSyndSteps,NoQECSteps,NoQEC,temp,selectedStep,physQbts,logiQbts;
    int gateTable2[500][1000];

    QComboBox *cb;
    QStringList colorNames;
    QModelIndex idx;
    QCustomPlot *customPlot;
    QCustomPlot *customPlot2;
    QPushButton *runCurrentCodeButton;
    QSqlTableModel *model;
    QTableView *view;
    QVector<double> paramList;
    QWidget *editorGBWidget;
    QCustomPlot *custPlot;
    DialogSyndrome *dialogSynd;
    void makeEditor();
    int counter;
    int counterGeneral;


    void makePlot(const QString tableName, QVector<double> x, QVector<double> y);
    void makePlot2(const QString tableName, QVector<double> x, QVector<double> y);

private slots:
    void submit();
    void runCurrentCode();
    void addRow();
    void clearPlot();

    void applySettings();
    void resetScene();
    void gateChanged(QAction *action);
    void changeNoSteps();
    void changeQbits();
    void about();

    void togglePointerMode();
    int transformF();
    void changeSteps();
    int setSyndrome();
    void setNoAncF();
    void setQECStepsF();

private:
    void createActions();
    void createMenus();
    void createDesignerWidget();
    void createPhysDesignerWidget();

    void cPlotSettings();
    void cPlotSettings2();

    int selectedGate;
    QLabel *label1, *label2;
    QWidget *circuitBuilderWidget, *physCircuitBuilderWidget;
    QHBoxLayout *circuitBuilderLayout, *circuitPhysBuilderLayout;
    QHBoxLayout *HBox;
    QWidget *editor_plot_Widget;

    Node *gateNode;
    GraphWidget *gw;
    GraphWidget *GWidget, *GWidget2;

    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *resetButton;
    QPushButton *addRowButton;
    QVBoxLayout *buttonBox;

    QMenu *fileMenu;
    QMenu *itemMenu;
    QMenu *aboutMenu;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *paintIAction;
    QAction *paintHAction;
    QAction *paintXAction;
    QAction *paintYAction;
    QAction *paintZAction;
    QAction *paintMAction;
    QAction *paintCntrAction;
    QAction *paintTrgtAction;
    QAction *paintS2Action;
    QAction *paintS4Action;
    QAction *paintS8Action;
    QAction *paintS16Action;
    QAction *paintS32Action;
    QAction *paintStateOneAction;
    QAction *paintStateZeroAction;
    QAction *paintStatePlusAction;
    QAction *paintStateMinusAction;

    QGridLayout *grid;
    QComboBox *comboBox1;
    QComboBox *comboBox2;
    //QComboBox *SetQECCode;
    //QComboBox *SetCorrNoise;
    QComboBox *SetNoQ;
    QComboBox *SetNoS;
    QComboBox *SetStep;
    QComboBox *SetNoAnc;
    QComboBox *SetNoQECSteps;
    QPushButton *addQubit;
    QPushButton *addStep;
    QPushButton *bApply;
    QPushButton *bReset;
    QPushButton *bckStepLines;
    QPushButton *clear;
    QPushButton *apply;
    QPushButton *transform;
    QPushButton *syndromeBuild;
    QToolButton *dragModeButton;
    //QToolBox *toolBox;

};

#endif // MAINWINDOW_H

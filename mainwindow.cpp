/*
* What is QuCirDET?
*
* QuCirDET is an Electronic Design Automation (EDA) software tool making easier the implementation of quantum
* circuitry. It is a graphical user interface tool for the design of the circuit model of quantum computation.
* It is written in C++ and it is based on the Qt Framework.
*
* Structure of the project
*
* It consists of 8 classes and also includes the library of qcustomplot which empowers for plotting diagrams.
* The mainwindow class calls the graphwidget class which is the elementary graphics view scene.
* The basic graphical components to implement the quantum circuits are fnode/node and edge. The fnode class
* is similar to nodeclass but is used graphically only for the initial states of qubits - the quantum memroy.
* The nodesyndrome and edgesyndrome are only used seperately for the design of QEC extraction syndromes.
*/

#include <QSqlQuery>
#include <QtSql>
#include <QtWidgets>

#include <iostream>
#include <iomanip>
using namespace std;

#include "mainwindow.h"
#include "graphwidget.h"
#include "node.h"
#include "dialogsyndrome.h"

MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createDesignerWidget();
    createPhysDesignerWidget();

    selectedStep=0;
    NoAnc=0; NoQ=0;    NoS=0;

    //Make the two descriptive labels of the interface
    label1 = new QLabel(this);
    label1->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label1->setText("Set the QBits of the memory and the computating steps. Press Apply to build it. Then, put the gates on the proper position (qubit/step) by right-click.");
    label1->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    label2 = new QLabel(this);
    label2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label2->setText("Select the desired QEC code from the list and define the circuit of the error detection extraction syndrome of the code by pressing Set Syndrome, "
                    "\nafter you select the needed number of auxilliary Ancilla qubits. Set the frequency with which recovery protocol (=application of syndrome and correction) will take place and then press "
                    "\nTransform to create the physical view of the circuit model. Ancilla Qbts has blue-black lines.");
    label2->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    //Create an object of your own class GraphWidget - the desing area
    GWidget = new GraphWidget(itemMenu,0,0, NoQ, NoS,0,0,0,selectedStep,1, this);
    GWidget2 = new GraphWidget(itemMenu,0,0, NoQ, NoS,0,0,0,selectedStep,1, this);

    //Make the editor of the table of the codes
    makeEditor();

    //Use the qcustomplot class to define two new objects-plots
    counter=0;counterGeneral=0;
    customPlot = new QCustomPlot();
    customPlot2 = new QCustomPlot();
    cPlotSettings();
    cPlotSettings2();
    QVector<double> xx(4), yy(4);
    makePlot("",xx,yy);
    makePlot2("",xx,yy);

    HBox = new QHBoxLayout;
    HBox->addWidget(customPlot2,5);
    HBox->addWidget(customPlot,5);
    HBox->addWidget(editorGBWidget,1);
    editor_plot_Widget = new QWidget;
    editor_plot_Widget -> setLayout(HBox);

    //Set the final layout of the interface
    grid = new QGridLayout();
    grid -> addWidget(label1, 0, 1);
    grid -> addWidget(circuitBuilderWidget, 1, 1);
    grid -> addWidget(GWidget, 2, 1);
    grid -> addWidget(label2, 3, 1);
    grid -> addWidget(view, 4, 1);
    grid -> addWidget(physCircuitBuilderWidget, 5, 1);
    grid -> addWidget(GWidget2, 6, 1);
    //grid -> addWidget(editor_plot_Widget, 6,2);//it deactivated due to overload of widgets in one window

    //Do all the connections between components and functions
    connect(SetNoQ, SIGNAL(activated(int)), this, SLOT(changeQbits()));
    connect(SetNoS, SIGNAL(activated(int)), this, SLOT(changeNoSteps()));
    connect(SetStep, SIGNAL(activated(int)), this, SLOT(changeSteps()));
    connect(apply, SIGNAL(clicked(bool)), this, SLOT(applySettings()));
    connect(clear, SIGNAL(clicked(bool)), this, SLOT(resetScene()));
    connect(SetNoAnc, SIGNAL(activated(int)), this, SLOT(setNoAncF()));
    connect(syndromeBuild, SIGNAL(clicked(bool)), this, SLOT(setSyndrome()));
    connect(SetNoQECSteps, SIGNAL(activated(int)), this, SLOT(setQECStepsF()));
    connect(transform, SIGNAL(clicked(bool)), this, SLOT(transformF()));
    connect(dragModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));


    QWidget *widget = new QWidget;
    widget->setLayout(grid);
    setCentralWidget(widget);
    setWindowTitle(tr("QuCirDET"));
}



void MainWindow::makeEditor()
{
    model = new QSqlTableModel(this);
    model->setTable("qec_codes");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("CodeName"));
    model->setHeaderData(2, Qt::Horizontal, tr("CodeType"));
    model->setHeaderData(3, Qt::Horizontal, tr("n"));
    model->setHeaderData(4, Qt::Horizontal, tr("k"));
    model->setHeaderData(5, Qt::Horizontal, tr("d"));

    view = new QTableView;
    view->setModel(model);
    view->resizeColumnsToContents();

    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);
    revertButton = new QPushButton(tr("&Revert"));
    resetButton = new QPushButton(tr("Clear"));
    addRowButton = new QPushButton(tr("Add"));
    runCurrentCodeButton = new QPushButton(tr("Run"));

    cb = new QComboBox(this);
    colorNames = QColor::colorNames();
    int index = 0;
    foreach (const QString &colorName, colorNames) {
        const QColor color(colorName);
        cb->addItem(colorName, color);
        idx = cb->model()->index(index++, 0);
        cb->model()->setData(idx, color, Qt::BackgroundColorRole);
    }
    cb->setMaxVisibleItems(11);
    cb->insertItem(0,"Colors");
    cb->setCurrentIndex(8);
    //selectedColor = cb->itemData(cb->currentIndex(), Qt::UserRole).toInt();

    buttonBox = new QVBoxLayout;
    buttonBox->addWidget(submitButton);
    buttonBox->addWidget(revertButton);
    buttonBox->addWidget(addRowButton);
    buttonBox->addWidget(runCurrentCodeButton);
    buttonBox->addWidget(resetButton);
    buttonBox->addWidget(cb);

    editorGBWidget = new QWidget;
    editorGBWidget -> setLayout(buttonBox);

    connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(revertButton, SIGNAL(clicked()), model, SLOT(revertAll()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(clearPlot()));
    connect(addRowButton, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(runCurrentCodeButton, SIGNAL(clicked()), this, SLOT(runCurrentCode()));

}
void MainWindow::addRow()
{
    //int rc=model->rowCount();
    model->insertRecord(-1,model->record());
    //QSqlQuery query;
    //query.exec("insert into qec_codes values(0,          '?[[?,?,?]]',   'CSS',     paramList[3], paramList[4], paramList[5],paramList[6],paramList[7],paramList[8],paramList[9],paramList[10],   paramList[11],paramList[12],paramList[13],paramList[14],paramList[15],paramList[16],paramList[17],paramList[18])");

    //QSqlDatabase::database().transaction();
    QSqlQuery query;
//    query.exec("INSERT INTO qec_codes "
// //               "values(0,          '?[[?,?,?]]',   'CSS',     paramList[3], paramList[4], paramList[5],paramList[6],paramList[7],paramList[8],paramList[9],paramList[10],   paramList[11],paramList[12],paramList[13],paramList[14],paramList[15],paramList[16],paramList[17],paramList[18])";
//               "values(0,          '?[[?,?,?]]',   'CSS',     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
// //                   + QString::number(employeeId) + ')');
    //QSqlDatabase::database().commit();
}
void MainWindow::submit()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("Cached Table"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
    }
}

void MainWindow::runCurrentCode()
{

    QItemSelectionModel *selmodel = view->selectionModel();
    QModelIndex current = selmodel->currentIndex(); // the "current" item
    int cRow=current.row();    //qDebug() << cRow;
    int colCount=model->columnCount();    //qDebug() << colCount;

    //get values from widgetand set into paramlist
    paramList.clear();
    paramList<<0<<0<<0;
    for(int i=3;i<colCount;i++){
        QModelIndex index = model->index( cRow, i);
        //qDebug() << "Cell of current row value:"<<i <<index.data().toDouble()<<" ";
        paramList<<index.data().toDouble();
    }
    //qDebug()<<"edw"<<paramList;

    QVector<double> x0(4), y0(4);
    x0[0]=1; x0[1]=2; x0[2]=3; x0[3]=4;
    double *data=paramList.data();
    //for (int i=0; i<?; ++i)
    //{
      //data[8]=x0[i];
      //qDebug()<<"After new set of x axis"<<paramList;
      //here run your own functions and get results bfor your plot
      //qDebug()<<"x0->"<<x0;
      //qDebug()<<"y0->"<<y0;
    //}
    //makePlot((model->index(cRow,1)).data().toString(),x0,y0);
}

void MainWindow::makePlot(const QString tableName, QVector<double> x, QVector<double> y)
{
    //qDebug()<<"after x0->"<<x;
    //qDebug()<<"after y0->"<<y;
    //qDebug()<<"Counter->"<<counter;

    customPlot->addGraph();

    QPen pen;
    pen.setColor(QColor(cb->itemData(cb->currentIndex(),Qt::UserRole).toString()));
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    customPlot->graph(counter)->setPen(pen);
    customPlot->graph(counter)->setName(tableName);
    customPlot->graph(counter)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,QColor(0,0,0),3));
    customPlot->graph(counter)->setData(x, y);
    customPlot->graph(counter)->removeDataBefore(1e-8);
    customPlot->replot();
    if(counterGeneral==0){customPlot->legend->removeItem(0);counterGeneral++;}
    counter++;

/*
    //replot and allows only the current plot(s) to take place all over the border
    customPlot->rescaleAxes();
    customPlot->graph(0)->rescaleAxes();
    //SecondaryGraph->rescaleAxes(true);
    customPlot->replot();
*/
}

void MainWindow::cPlotSettings()
{
    customPlot->setNoAntialiasingOnDrag(true); // better performance/responsiveness during dragging
    //customPlot->setBackground(QBrush(QColor(000,000,000,255)));
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop); // make legend align in top left corner or axis rect

    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
    customPlot->xAxis->setScaleLogBase(10);
    customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    customPlot->yAxis->setScaleLogBase(10);
    customPlot->yAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
    customPlot->xAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
    customPlot->yAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
    customPlot->xAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
    customPlot->xAxis->setSubTickCount(10);
    customPlot->yAxis->setSubTickCount(0);
    customPlot->yAxis->setRange(1e-13,1e-0);
    customPlot->xAxis->setRange(5e-8, 5e-2);
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // make range draggable and zoomable:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // make top right axes clones of bottom left axes:
    customPlot->axisRect()->setupFullAxesBox();
    // connect signals so top and right axes move in sync with bottom and left axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->replot();
    counter=0;
}

void MainWindow::makePlot2(const QString tableName, QVector<double> x, QVector<double> y)
{
    // create empty bar chart objects:
    QCPBars *regen = new QCPBars(customPlot2->xAxis, customPlot2->yAxis);
    QCPBars *nuclear = new QCPBars(customPlot2->xAxis, customPlot2->yAxis);
    QCPBars *fossil = new QCPBars(customPlot2->xAxis, customPlot2->yAxis);
    customPlot2->addPlottable(regen);
    customPlot2->addPlottable(nuclear);
    customPlot2->addPlottable(fossil);

    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    fossil->setName("NoGates");
    pen.setColor(QColor(255, 131, 0));
    fossil->setPen(pen);
    fossil->setBrush(QColor(255, 131, 0, 50));
    nuclear->setName("NoAncillas");
    pen.setColor(QColor(1, 92, 191));
    nuclear->setPen(pen);
    nuclear->setBrush(QColor(1, 92, 191, 50));
    regen->setName("Total");
    pen.setColor(QColor(150, 222, 0));
    regen->setPen(pen);
    regen->setBrush(QColor(150, 222, 0, 70));
    // stack bars ontop of each other:
    nuclear->moveAbove(fossil);
    regen->moveAbove(nuclear);

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3;
    labels << "Golay[[21,3,5]]" << "Golay[[23,1,7]]" << "Ham[[7,1,3]]";
    customPlot2->xAxis->setAutoTicks(false);
    customPlot2->xAxis->setAutoTickLabels(false);
    customPlot2->xAxis->setTickVector(ticks);
    customPlot2->xAxis->setTickVectorLabels(labels);
    customPlot2->xAxis->setTickLabelRotation(60);
    customPlot2->xAxis->setSubTickCount(0);
    customPlot2->xAxis->setTickLength(0, 4);
    customPlot2->xAxis->grid()->setVisible(true);
    customPlot2->xAxis->setRange(0, 8);

    // prepare y axis:
    customPlot2->yAxis->setRange(0, 12.1);
    customPlot2->yAxis->setPadding(5); // a bit more space to the left border
    customPlot2->yAxis->setLabel("Gates");
    customPlot2->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    customPlot2->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    customPlot2->yAxis->grid()->setSubGridPen(gridPen);

    // Add data:
    QVector<double> fossilData, nuclearData, regenData;
    fossilData  << 0.86*10.5 << 0.83*5.5 << 0.84*5.5 << 0.52*5.8 << 0.89*5.2 << 0.90*4.2 << 0.67*11.2;
    nuclearData << 0.08*10.5 << 0.12*5.5 << 0.12*5.5 << 0.40*5.8 << 0.09*5.2 << 0.00*4.2 << 0.07*11.2;
    regenData   << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2;
    fossil->setData(ticks, fossilData);
    nuclear->setData(ticks, nuclearData);
    regen->setData(ticks, regenData);

    // setup legend:
    customPlot2->legend->setVisible(true);
    customPlot2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    customPlot2->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    customPlot2->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot2->legend->setFont(legendFont);
    customPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

/*
    //qDebug()<<"after x0->"<<x;
    //qDebug()<<"after y0->"<<y;
    qDebug()<<"Counter->"<<counter;
    counter=0; //        sos auto bhke prosorina opws k olh h methodos

    customPlot2->addGraph();

    QPen pen;
    pen.setColor(QColor(cb->itemData(cb->currentIndex(),Qt::UserRole).toString()));
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    customPlot2->graph(counter)->setPen(pen);
    customPlot2->graph(counter)->setName(tableName);
    customPlot2->graph(counter)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,QColor(0,0,0),3));
    customPlot2->graph(counter)->setData(x, y);
    customPlot2->graph(counter)->removeDataBefore(1e-8);
    customPlot2->replot();
    if(counterGeneral==0){customPlot2->legend->removeItem(0);counterGeneral++;}
    counter++;
*/
/*
    //replot and allows only the current plot(s) to take place all over the border
    customPlot->rescaleAxes();
    customPlot->graph(0)->rescaleAxes();
    //SecondaryGraph->rescaleAxes(true);
    customPlot->replot();
*/
}

void MainWindow::cPlotSettings2()
{
    /*
    customPlot2->setNoAntialiasingOnDrag(true); // more performance/responsiveness during dragging
    //customPlot->setBackground(QBrush(QColor(000,000,000,255)));
    customPlot2->legend->setVisible(true);
    customPlot2->legend->setBrush(QBrush(QColor(255,255,255,150)));
    customPlot2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop); // make legend align in top left corner or axis rect

    customPlot2->yAxis->grid()->setSubGridVisible(true);
    customPlot2->xAxis->grid()->setSubGridVisible(true);
    customPlot2->xAxis->setScaleType(QCPAxis::stLogarithmic);
    customPlot2->xAxis->setScaleLogBase(10);
    customPlot2->yAxis->setScaleType(QCPAxis::stLogarithmic);
    customPlot2->yAxis->setScaleLogBase(10);
    customPlot2->yAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
    customPlot2->xAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
    customPlot2->yAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
    customPlot2->xAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
    customPlot2->xAxis->setSubTickCount(10);
    customPlot2->yAxis->setSubTickCount(0);
    customPlot2->yAxis->setRange(1e-13,1e-0);
    customPlot2->xAxis->setRange(5e-8, 5e-2);
    customPlot2->xAxis->setLabel("x");
    customPlot2->yAxis->setLabel("y");
    // make range draggable and zoomable:
    customPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // make top right axes clones of bottom left axes:
    customPlot2->axisRect()->setupFullAxesBox();
    // connect signals so top and right axes move in sync with bottom and left axes:
    connect(customPlot2->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot2->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot2->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot2->yAxis2, SLOT(setRange(QCPRange)));
*/
}

void MainWindow::setQECStepsF()
{
    NoQECSteps = SetNoQECSteps->itemData(SetNoQECSteps->currentIndex(), Qt::UserRole).toInt();
}

void MainWindow::setNoAncF()
{
    NoAnc = SetNoAnc->itemData(SetNoAnc->currentIndex(), Qt::UserRole).toInt();
}

int MainWindow::setSyndrome()
{
    QItemSelectionModel *selmodel = view->selectionModel();
    QModelIndex current = selmodel->currentIndex(); // it set the value of the current selected item from the QEC code list
    int cRow=current.row();    //qDebug() << cRow;

    QModelIndex indexN = model->index(cRow, 3); //get the value for the physical qubits of the selected code
    physQbts=indexN.data().toDouble();
    QModelIndex indexK = model->index(cRow, 4); //get the value for the logical qubits of the selected code
    logiQbts=indexK.data().toDouble();

    if(physQbts==0||logiQbts==0){
        QMessageBox::about(this, tr("Choose Encoding Code"),tr("You have to select an <b>encoding</b> code in order to continue."));
        return 0;
    }

    dialogSynd = new DialogSyndrome(itemMenu,physQbts,logiQbts,NoQ,NoS,NoAnc,0);
    dialogSynd->show();
    return 0;
}

int MainWindow::transformF() //it runs when Transform button is pressed
{
    QItemSelectionModel *selmodel = view->selectionModel();
    QModelIndex current = selmodel->currentIndex(); // it set the value of the current selected item from the QEC code list
    int cRow=current.row();    //qDebug() << cRow;

    QModelIndex indexN = model->index(cRow, 3); //get the value for the physical qubits of the selected code
    physQbts=indexN.data().toDouble();
    QModelIndex indexK = model->index(cRow, 4); //get the value for the logical qubits of the selected code
    logiQbts=indexK.data().toDouble();

    if(physQbts==0||logiQbts==0){
        QMessageBox::about(this, tr("Choose Encoding Code"),tr("You have to select an <b>encoding</b> code in order to continue."));
        return 0;
    }

    //create the logical layout with QECs in a matrix
    int c=0;
    NoQEC=NoS/NoQECSteps;
    for(int i=0;i<NoQ;i++){
        for(int j=0;j<NoS+NoQEC;j++){
            if(((j+1)%(NoQECSteps+1))==0){
                GWidget->gMtrxQEC[i][j]=19;
                c++;
            }else{
                temp=GWidget->gateTable[i][j-c];
                GWidget->gMtrxQEC[i][j]=temp;
            }
        }
        c=0;
    }
    //print the new table to see the new layout
    QDebug deb = qDebug();
    for(int i=0;i<NoQ;i++){
        for(int j=0;j<NoS+NoQEC;j++)
            deb <<GWidget->gMtrxQEC[i][j];
        deb << endl;
    }
    deb << endl;

    //int iMax=NoQ*physQbts;
    //int jMax=NoS*physQbts+NoS*NoQEC;
    int putter;
    int si=0;
    int sj=0;

    //create the physical layout in a matrix
    for(int i=0;i<NoQ;i++){
        for(int j=0;j<NoS+NoQEC;j++){
            putter=GWidget->gMtrxQEC[i][j];
            //if(putter==11||putter==12||putter==13||putter==14||putter==15||putter==16||putter==17)//2qbt gates
            if(putter==19){
                for(int k=0;k<physQbts+NoAnc;k++)
                    gateTable2[si+k][sj]=putter;
                sj++;
            }else{
                for(int k=0;k<physQbts;k++)
                    gateTable2[si+k][sj+k]=putter;
                sj=sj+physQbts;
            }
        }
        si=si+NoAnc;
        si=si+physQbts;
        sj=0;
    }

    //print the new table to see the new layout
    //QDebug deb = qDebug();
    for(int i=0;i<NoQ*physQbts+NoQ*NoAnc;i++){
        for(int j=0;j<NoS*physQbts+NoQEC;j++){
            deb << gateTable2[i][j];
        }
        deb << endl;
    }
    delete GWidget2;

    GWidget2 = new GraphWidget(itemMenu, physQbts, logiQbts, NoQ, NoS, NoAnc,0,gateTable2,NoQEC,3, this);
    grid -> addWidget(GWidget2, 6, 1);

    return 0;
}

void MainWindow::createDesignerWidget()
{
    circuitBuilderLayout = new QHBoxLayout;

    SetNoQ = new QComboBox;
    circuitBuilderLayout->addWidget(new QLabel(tr("Set NoQubits :")));
    circuitBuilderLayout->addWidget(SetNoQ);
    //SetNoQ->addItem("QBits");
    for(int i=0;i<2000;i++)   {SetNoQ->addItem(QString::number(i),i);}
    //-----
    SetNoS = new QComboBox;
    circuitBuilderLayout->addWidget(new QLabel(tr("Set NoSteps :")));
    circuitBuilderLayout->addWidget(SetNoS);
    //SetNoS->addItem("QSteps");
    for(int i=0;i<60;i++)    {SetNoS->addItem(QString::number(i),i);}
    //-----
    SetStep = new QComboBox;
    //circuitBuilderLayout->addWidget(new QLabel(tr("Set NoSteps :")));
    circuitBuilderLayout->addWidget(SetStep);
    SetStep->addItem("CSteps");
    for(int i=1;i<5;i++)    {SetStep->addItem(QString::number(i),i);}

    //comboBox1->setCurrentIndex(0);// it is set by default so u can ommit this line
    SetNoQ->setMaxVisibleItems(11);
    //comboBox2->setCurrentIndex(0);// it is set by default so u can ommit this line
    SetNoS->setMaxVisibleItems(11);

    apply = new QPushButton(tr("Apply"));
    circuitBuilderLayout -> addWidget(apply);

    QFrame* line = new QFrame();//you can also declare here a component
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    circuitBuilderLayout->addWidget(line);

    clear = new QPushButton(tr("Clear"));
    circuitBuilderLayout -> addWidget(clear);

    dragModeButton = new QToolButton;
    dragModeButton->setText(tr("Drag"));
    dragModeButton->setCheckable(true);
    dragModeButton->setChecked(false);
    circuitBuilderLayout -> addWidget(dragModeButton);

    circuitBuilderWidget = new QWidget;
    circuitBuilderWidget -> setLayout(circuitBuilderLayout);
}

void MainWindow::createPhysDesignerWidget()
{
    circuitPhysBuilderLayout = new QHBoxLayout;

    SetNoAnc = new QComboBox;
    //circuitPhysBuilderLayout->addWidget(new QLabel(tr("Set NoAnc :")));
    circuitPhysBuilderLayout->addWidget(SetNoAnc);
    SetNoAnc->addItem("Set NoAnc");
    for(int i=0;i<11;i++)    {SetNoAnc->addItem(QString::number(i),i);}

    syndromeBuild = new QPushButton(tr("Set Syndrome"));
    circuitPhysBuilderLayout -> addWidget(syndromeBuild);

    SetNoQECSteps = new QComboBox;
    //circuitPhysBuilderLayout->addWidget(new QLabel(tr("Set NoQECSteps :")));
    circuitPhysBuilderLayout->addWidget(SetNoQECSteps);
    SetNoQECSteps->addItem("Set Recovery Period");
    for(int i=1;i<11;i++)    {SetNoQECSteps->addItem(QString::number(i),i);}

    transform = new QPushButton(tr("Transform"));
    circuitPhysBuilderLayout -> addWidget(transform);

    physCircuitBuilderWidget = new QWidget;
    physCircuitBuilderWidget -> setLayout(circuitPhysBuilderLayout);
}

void MainWindow::resetScene()
{
    delete GWidget;
    GWidget = new GraphWidget(itemMenu,0,0,0,0,0,0,0,selectedStep,1, this );
    grid -> addWidget(GWidget, 2, 1);
}

void MainWindow::applySettings()
{
    NoQ = SetNoQ->itemData(SetNoQ->currentIndex(), Qt::UserRole).toInt();
    NoS = SetNoS->itemData(SetNoS->currentIndex(), Qt::UserRole).toInt();
    //qDebug() << "When pressing APPLY button ... NoQ = " << NoQ << " NoS = " << NoS;
    delete GWidget;
    GWidget = new GraphWidget(itemMenu,0,0, NoQ, NoS,0,0,0,selectedStep,1, this );
    grid->addWidget(GWidget, 2, 1);
}

void MainWindow::changeQbits()
{
    NoQ = SetNoQ->itemData(SetNoQ->currentIndex(), Qt::UserRole).toInt();
    //qDebug() << "NoQ = " << NoQ;
}

void MainWindow::changeNoSteps()
{
    NoS = SetNoS->itemData(SetNoS->currentIndex(), Qt::UserRole).toInt();
    //qDebug() << "NoS = " << NoS;
}

void MainWindow::changeSteps()
{
    selectedStep = SetStep->itemData(SetStep->currentIndex(), Qt::UserRole).toInt();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About QECProtocol Simulator"),
                       tr("This is the first version of <b>QuCirDET v1.0</b>. "
              "QuCirDET is a generic tool allowing various simulations using the circuit model of quantum "
              "computation. The interface is oriented to constructions of quantum circuits including      "
              "various encoding schemas."));
}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction ->setShortcut(tr("Ctrl+B"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    paintStateZeroAction = new QAction(tr("|0>"), this);
    paintStateZeroAction ->setStatusTip(tr("Zero State"));
    paintStateZeroAction ->setData(1);

    paintStateOneAction = new QAction(tr("|1>"), this);
    paintStateOneAction ->setStatusTip(tr("One State"));
    paintStateOneAction ->setData(2);

    paintStatePlusAction = new QAction(tr("|+>"), this);
    paintStatePlusAction ->setStatusTip(tr("+ State"));
    paintStatePlusAction ->setData(3);

    paintStateMinusAction = new QAction(tr("|->"), this);
    paintStateMinusAction ->setStatusTip(tr("- State"));
    paintStateMinusAction ->setData(4);

    paintIAction = new QAction(tr("I"), this);
    paintIAction ->setStatusTip(tr("Idle gate"));
    paintIAction ->setData(5);

    paintHAction = new QAction(tr("H"), this);
    paintHAction ->setStatusTip(tr("Hadamard gate"));
    paintHAction ->setData(6);

    paintXAction = new QAction(tr("X"), this);
    paintXAction ->setStatusTip(tr("Pauli X gate"));
    paintXAction ->setData(7);

    paintYAction = new QAction(tr("Y"), this);
    paintYAction ->setStatusTip(tr("Pauli Y gate"));
    paintYAction ->setData(8);

    paintZAction = new QAction(tr("Z"), this);
    paintZAction ->setStatusTip(tr("Pauli Z gate"));
    paintZAction ->setData(9);

    paintMAction = new QAction(tr("M"), this);
    paintMAction ->setStatusTip(tr("Pauli M gate"));
    paintMAction ->setData(10);

    paintCntrAction = new QAction(tr("Cntr"), this);
    paintCntrAction ->setStatusTip(tr("Cntr gate"));
    paintCntrAction ->setData(11);

    paintTrgtAction = new QAction(tr("Trgt"), this);
    paintTrgtAction ->setStatusTip(tr("Trgt gate"));
    paintTrgtAction ->setData(12);

    paintS2Action = new QAction(tr("π/2"), this);
    paintS2Action ->setStatusTip(tr("Phase S gate"));
    paintS2Action ->setData(13);

    paintS4Action = new QAction(tr("π/4"), this);
    paintS4Action ->setStatusTip(tr("Phase S gate"));
    paintS4Action ->setData(14);

    paintS8Action = new QAction(tr("π/8"), this);
    paintS8Action ->setStatusTip(tr("Phase S gate"));
    paintS8Action ->setData(15);

    paintS16Action = new QAction(tr("π/16"), this);
    paintS16Action ->setStatusTip(tr("Phase S gate"));
    paintS16Action ->setData(16);

    paintS32Action = new QAction(tr("π/32"), this);
    paintS32Action ->setStatusTip(tr("Phase S gate"));
    paintS32Action ->setData(17);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Gates"));//if you ommit this line run will not work without proper msg
    itemMenu->addAction(paintStateZeroAction);
    itemMenu->addAction(paintStateOneAction);
    itemMenu->addAction(paintStatePlusAction);
    itemMenu->addAction(paintStateMinusAction);
    itemMenu->addSeparator();
    itemMenu->addAction(paintIAction);
    itemMenu->addAction(paintXAction);
    itemMenu->addAction(paintHAction);
    itemMenu->addAction(paintYAction);
    itemMenu->addAction(paintZAction);
    itemMenu->addAction(paintMAction);
    itemMenu->addSeparator();
    itemMenu->addAction(paintCntrAction);
    itemMenu->addAction(paintTrgtAction);
    itemMenu->addSeparator();
    itemMenu->addAction(paintS2Action);
    itemMenu->addAction(paintS4Action);
    itemMenu->addAction(paintS8Action);
    itemMenu->addAction(paintS16Action);
    itemMenu->addAction(paintS32Action);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}

void MainWindow::gateChanged(QAction *action)
{
    int val = action->data().toInt();
    gateNode->setGate(val);
}

void MainWindow::togglePointerMode()
{
    GWidget->setDragMode(QGraphicsView::ScrollHandDrag);
}

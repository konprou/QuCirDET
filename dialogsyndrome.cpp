#include "dialogsyndrome.h"
#include "graphwidget.h"

DialogSyndrome::DialogSyndrome(QMenu *itemMenu, int pQb,int lQb,int q,int s,int NoAnc,QWidget *parent)
    : QWidget(parent)
//inherits from QWidget class
{
    syndWidgetF();
    this->NoAnc=NoAnc;
    NoSyndSteps=0;
    physQbts=pQb;
    logQb=lQb;
    myMenu=itemMenu;

    label3 = new QLabel();
    label3->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label3->setText("Design the syndrome detection circuit of the selected QEC code. Set as many ancilla qubits as you need.");
    label3->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    //Create an object of your own class GraphWidget - the desing area
    GWidgetSynd = new GraphWidget(myMenu,pQb,lQb,q,0,this->NoAnc,0,0,0,2,this);

    gridSynd = new QGridLayout(this); //sos i need to parameterize with this
    gridSynd -> addWidget(label3, 0, 1);
    gridSynd -> addWidget(syndButtonWidget1, 1, 1);
    gridSynd -> addWidget(GWidgetSynd, 2, 1);
    gridSynd -> addWidget(syndButtonWidget2, 3, 1);

    connect(setSyndSteps, SIGNAL(activated(int)), this, SLOT(setSyndStepsF()));
    connect(saveSynd, SIGNAL(clicked(bool)), this, SLOT(saveSyndF()));
    connect(closeSynd, SIGNAL(clicked(bool)), this, SLOT(close()));


    setGeometry(40,50,600,600);
    setWindowTitle(tr("Syndrome Circuit Layout"));
}

void DialogSyndrome::setSyndStepsF()
{
    NoSyndSteps = setSyndSteps ->itemData(setSyndSteps ->currentIndex(), Qt::UserRole).toInt();
    delete GWidgetSynd;
    GWidgetSynd = new GraphWidget(myMenu,physQbts,lQb,q,0,NoAnc,NoSyndSteps,0,0,2,this);
    gridSynd -> addWidget(GWidgetSynd, 2, 1);
}


void DialogSyndrome::saveSyndF()
{
    qDebug()<<"saveSyndF";
}

void DialogSyndrome::syndWidgetF()
{
    syndButtonLayout1 = new QHBoxLayout;

    setSyndSteps = new QComboBox;
    //circuitBuilderLayout->addWidget(new QLabel(tr("Set NoQubits :")));
    syndButtonLayout1 ->addWidget(setSyndSteps);
    setSyndSteps->addItem("Set Syndrome Steps");
    for(int i=0;i<50;i++)   {setSyndSteps->addItem(QString::number(i),i);}
    setSyndSteps->setMaxVisibleItems(11);

    syndButtonLayout2 = new QHBoxLayout;

    saveSynd = new QPushButton(tr("Save"));
    syndButtonLayout2 -> addWidget(saveSynd);

    closeSynd = new QPushButton(tr("Close"));
    syndButtonLayout2 -> addWidget(closeSynd);

    syndButtonWidget1 = new QWidget;
    syndButtonWidget1 -> setLayout(syndButtonLayout1);
    syndButtonWidget2 = new QWidget;
    syndButtonWidget2 -> setLayout(syndButtonLayout2);
}


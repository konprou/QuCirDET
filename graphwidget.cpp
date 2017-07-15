#include "graphwidget.h"
#include "edge.h"
#include "edgesyndrome.h"
#include "node.h"
#include "nodesyndrome.h"
#include "fnode.h"

#include <math.h>
#include <QKeyEvent>
#include <QtWidgets>
#include <QStandardItemModel>

//itemMenu is the popup list when right-click on a node
GraphWidget::GraphWidget(QMenu *itemMenu, int PQbts, int LQbts, int NoQ, int NoS, int NoAnc,int NoSyndSteps,int gateTable2[][1000], int NoQEC, int mode, QWidget *parent)
    : QGraphicsView(parent)
{
    stepper=stepp;
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //scene->setSceneRect(-250, -200, 1000, 500);
    setScene(scene);

    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 200);
    setWindowTitle(tr("The graphic circuit desighner"));
    NoS_Syndrome=NoSyndSteps;

    int gateTable[NoQ][NoS], gateTableSyndrome[PQbts+NoAnc][NoS*PQbts];

    //initiallization of the gate table - if u set differnt bounds from the declared it crashes
    for(int i=0;i<NoQ;i++){
        for(int j=0;j<NoS;j++){
            gateTable[i][j]=0;
        }
    }

    switch (mode) {
    case 1: //Build logical circuit without QEC code -----------------------------------------
        yCoor=0;//considers y axis
        xCoor=0;//considers x axis
        for(int i=0;i<NoQ;i++){//nodes
            xCoor=50;

            fnodess[i] = new fNode(this,NoQ-i-1);
            fnodess[i]->setPos(25, yCoor);
            scene->addItem(fnodess[i]);

            for(int j=0;j<NoS;j++){
                nodess[i][j] = new Node(itemMenu, this, NoQ, NoS,0);
                //set the addresses of these nodes
                nodess[i][j] -> setData(1,i);
                nodess[i][j] -> setData(2,j);
                nodess[i][j] -> setPos( xCoor, yCoor );
                scene -> addItem(nodess[i][j]);
                xCoor=xCoor+30;
            }
            yCoor=yCoor+20; //htan 50
        }
        //adding the edges
        for(int i=0;i<NoQ;i++){
            for(int j=0;j<NoS-1;j++){
                Edge *edge = new Edge(nodess[i][j], nodess[i][j+1],0);
                scene->addItem(edge);
            }
        }
        break;

    case 2: //Build syndrome circuit with QEC code----------------------------------------------
        yCoor=0;//considers y axis
        xCoor=0;//considers x axis
        for(int i=0;i<(PQbts+NoAnc);i++){
            xCoor=50;

            fnodessSynd[i] = new fNode(this,PQbts-i-1);
            fnodessSynd[i]->setPos(25, yCoor);
            scene->addItem(fnodessSynd[i]);

            if(i<PQbts){
                for(int j=0;j<NoS_Syndrome;j++){
                    nodesSynd[i][j] = new NodeSyndrome(itemMenu, this, PQbts, NoS_Syndrome,0);
                    //set the addresses of these nodes
                    nodesSynd[i][j] -> setData(1,i);
                    nodesSynd[i][j] -> setData(2,j);
                    nodesSynd[i][j] -> setPos( xCoor, yCoor );
                    scene -> addItem(nodesSynd[i][j]);
                    xCoor=xCoor+30;
                }
                yCoor=yCoor+20;
            }else{
                for(int j=0;j<NoS_Syndrome;j++){
                    nodesSynd[i][j] = new NodeSyndrome(itemMenu, this, NoAnc, NoS_Syndrome,18);
                    //set the addresses of these nodes
                    nodesSynd[i][j] -> setData(1,i);
                    nodesSynd[i][j] -> setData(2,j);
                    nodesSynd[i][j] -> setPos( xCoor, yCoor );
                    scene -> addItem(nodesSynd[i][j]);
                    xCoor=xCoor+30;
                }
                yCoor=yCoor+20;
            }
        }
        //adding the edges for the logical and ancillary part
        for(int i=0;i<(PQbts+NoAnc);i++)
            if(i<PQbts){
                for(int j=0;j<NoS_Syndrome-1;j++){
                    EdgeSyndrome *edge = new EdgeSyndrome(nodesSynd[i][j], nodesSynd[i][j+1],0);
                    scene->addItem(edge);
                }
            }else{
                for(int j=0;j<NoS_Syndrome-1;j++){
                    EdgeSyndrome *edge = new EdgeSyndrome(nodesSynd[i][j], nodesSynd[i][j+1],1);
                    scene->addItem(edge);
                }
            }

        break;

    case 3: //Transformation-------------------------------------------------------

        iMax=(PQbts+NoAnc)*NoQ;//for the qubit/lines in the phys layout
        jMax=(PQbts*NoS)+ NoQEC;//for the steps/nodes in the phys layout
        yCoor=0;//considers y axis - positioning on the scene
        xCoor=0;//considers x axis - positioning on the scene
        for(int i=0;i<(iMax);i++){
            xCoor=50;
            //setting initial states
            fnodess[i] = new fNode(this,iMax-i-1);
            fnodess[i]->setPos(20, yCoor);
            scene->addItem(fnodess[i]);

            if(i%(PQbts+NoAnc) < PQbts){
                for(int j=0;j<jMax;j++){
                    gate=gateTable2[i][j];
                    nodess[i][j] = new Node(itemMenu, this, iMax,jMax,gate);
                    nodess[i][j]->setPos( xCoor, yCoor );
                    scene->addItem(nodess[i][j]);
                    xCoor=xCoor+50;//let a space along x axis
                }
            }else{
                for(int j=0;j<jMax;j++){//built only ancilla system
                    gate=gateTable2[i][j];
                    nodess[i][j] = new Node(itemMenu, this, iMax,jMax,gate);//18 is for blue lines
                    nodess[i][j]->setPos( xCoor, yCoor );
                    scene->addItem(nodess[i][j]);
                    xCoor=xCoor+50;//let a space along x axis
                 }
            }
            yCoor=yCoor+20; //gia to keno metaxy twn blocks
            if(((i+1)%(PQbts+NoAnc))==0)
                yCoor=yCoor+20; //gia to keno metaxy twn qubits-grammwn
        }//for-for


    //adding the edges
    for(int i=0;i<iMax;i++)
       for(int j=0;j<jMax-1;j++){
           if( i%(PQbts+NoAnc) < PQbts){
               Edge *edge = new Edge(nodess[i][j], nodess[i][j+1],0);
               scene->addItem(edge);
            }else{
               Edge *edge = new Edge(nodess[i][j], nodess[i][j+1],1);
               scene->addItem(edge);
           }
       }

        boolean sema=false;int i1=0,j1=0,temp;
         for(int j=0;j<jMax;j++){
            for(int i=0;i<iMax;i++){
                temp=gateTable2[i][j];
                if(temp==11||temp==12||temp==13||temp==14||temp==15||temp==16||temp==17){
                    if(sema==true){
                        sema=false;
                        Edge *edge = new Edge(nodess[i1][j1],nodess[i][j],0);
                        scene->addItem(edge);
                        i1=0;j1=0;
                    }else{i1=i;j1=j;sema=true;}
                }
            }
         }
        break;
    }//switch
}//end of constructor GraphWidget



#ifndef QT_NO_WHEELEVENT
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 940.0));
}
#endif



void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)//the background compu-steps are drawn
{
    switch (stepper) {
    default:
        step=0;
        break;
    case 1:
        step=50;
        break;
    case 2:
        step=100;
        break;
    case 3:
        step=150;
        break;
    case 4:
        step=200;
        break;
    }

        QVector<QPoint> polyPointsX;
        QVector<QPoint> linePointsX;

        painter->setPen(Qt::blue);
        //painter->setPen(Qt::DashLine);
        polyPointsX << QPoint( 35, -500 ) << QPoint( 35, 1500 );
        //linePointsX <<  QPoint( -15, -500 ) << QPoint( -15, 500 );
        linePointsX << polyPointsX;
        painter->drawLines( linePointsX );
        polyPointsX.clear();
        linePointsX.clear();

        if(step!=0){
            painter->setPen(Qt::lightGray);
            for(int i=50;i<3000;i+=step){
                polyPointsX << QPoint( i-25, -500 ) << QPoint( i-25, 500 );
            }
            foreach( QPoint point, polyPointsX )
                linePointsX << point + QPoint( 50, 0 );
            painter->drawLines( linePointsX );
        }else{}
}


void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}


void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}


void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}


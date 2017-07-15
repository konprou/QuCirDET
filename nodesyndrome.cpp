#include "edgesyndrome.h"
#include "nodesyndrome.h"
#include "graphwidget.h"
#include <iostream>
#include <QtGlobal>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QMenu>
#include <QtWidgets>
#include <QWidget>

NodeSyndrome::NodeSyndrome(QMenu *contextMenu, GraphWidget *graphWidget, int NoQ, int NoS, int gate)
    : graph (graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    myContextMenu = contextMenu;
    NoQL=NoQ;
    NoSL=NoS;
    indexI=0;
    indexJ=0;

    selectedGate=gate;
}


void NodeSyndrome::addEdgeS(EdgeSyndrome *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<EdgeSyndrome *> NodeSyndrome::edges() const
{
    return edgeList;
}
QRectF NodeSyndrome::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath NodeSyndrome::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void NodeSyndrome::setGate(int gate)
{
}

void NodeSyndrome::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(QPen(Qt::black, 0));

    switch (selectedGate) {
    default:
        painter->setPen(QPen(Qt::black, 1));
        //painter->setBrush(QBrush(Qt::black));
        //painter->drawEllipse(-2, -2, 3, 3);
        painter->drawLine(-10, 0, 10, 0);
        break;

    case 1:
        //painter->drawRect(-10, -10, 20, 20);
        painter->drawText(-14, 4, "|0>" );
        break;
    case 2:
        //painter->drawRect(-10, -10, 20, 20);
        painter->drawText(-14, 4, "|1>" );
        break;
    case 3:
        //painter->drawRect(-10, -10, 20, 20);
        painter->drawText(-14, 4, "|+>" );
        break;
    case 4:
        //painter->drawRect(-10, -10, 20, 20);
        painter->drawText(-14, 4, "|->" );
        break;
    case 5: // this case should send 0 in the matrix, but it sends 5, beware
        painter->setPen(QPen(Qt::black, 1));
        painter->drawLine(-10, 0, 10, 0);
        break;
    case 6:
        painter->drawRect(-9, -9, 18, 18 );
        painter->drawText(-4, 4, "H" );
        break;
    case 7:
        painter->drawRect(-9, -9, 18, 18);
        painter->drawText(-4, 4, "X" );
        break;
    case 8:
        painter->drawRect(-9, -9, 18, 18);
        painter->drawText(-4, 4, "Y" );
        break;
    case 9:
        painter->drawRect(-10, -10, 20, 20);
        painter->drawText(-4, 4, "Z" );
        break;
    case 10:
        painter->setBrush(QBrush(Qt::gray));
        painter->drawRect(-10, -10, 20, 20);
        painter->drawText(-4, 4, "M" );
        break;
    case 11://cntr
        painter->setBrush(QBrush(Qt::black));
        painter->drawEllipse(-4, -4, 8, 8);
        painter->drawLine(-8,0,8,0);
        break;
    case 12://target
        painter->setPen(QPen(Qt::black, 1));
        painter->drawEllipse(-8, -8, 16, 16);
        painter->drawLine(-8,0,8,0);
        painter->drawLine(0,-8,0,8);
        break;
    case 13:
        painter->drawRect(-9, -9, 18, 18);
        painter->drawText(-8, 4, "π/2" );
        break;
    case 14:
        painter->drawRect(-9, -9, 18, 18);
        painter->drawText(-8, 4, "π/4" );
        break;
    case 15:
        painter->drawRect(-9, -9, 18, 18);
        painter->drawText(-8, 4, "π/8" );
        break;
    case 16:
        painter->drawRect(-9, -9, 23, 18);
        painter->drawText(-8, 4, "π/16" );
        break;
    case 17:
        painter->drawRect(-9, -9, 23, 18);
        painter->drawText(-8, 4, "π/32" );
        break;
    case 18:
        painter->setPen(QPen(Qt::blue, 1));
        painter->drawLine(-10, 0, 10, 0);
        break;
    }
}

void NodeSyndrome::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   act=myContextMenu->exec(event->screenPos());
   //qDebug() << "action = " << act; // print action code
   if (!act) {
       qDebug() << "No menu item was selected";
   } else {
       selectedGate = act->data().toInt();
       iPos=this->data(1).toInt();
       jPos=this->data(2).toInt();
       qDebug() << "The selected gate in action is -> " << selectedGate;
       qDebug() << "The coordinates of the item nodes are in the initial matrix (counting from 0,0)-> " << iPos <<" and "<<jPos;

       //if control-gate put connection btw gates
       if(selectedGate==11||selectedGate==12||selectedGate==13||selectedGate==14||selectedGate==15||selectedGate==16||selectedGate==17){
           for(int i=0;i<NoQL;i++){
               temp = graph->gateTableSyndrome[i][jPos];
               if(temp==11 || temp==12 || temp==13 || temp==14 || temp==15 || temp==16 || temp==17){
                   EdgeSyndrome *edge = new EdgeSyndrome(graph->nodesSynd[iPos][jPos], graph->nodesSynd[i][jPos],0);
                   graph->scene->addItem(edge);
               }
           }
       }

       graph->gateTableSyndrome[this->data(1).toInt()][this->data(2).toInt()]=selectedGate;

       //Print the initial gate table - u cannot print directly a matrix with qDebug(), so create an object
       QDebug deb = qDebug();
       for(int i=0;i<NoQL;i++){
           for(int j=0;j<NoSL;j++){
               deb << graph->gateTableSyndrome[i][j];
           }
           deb << endl;
       }

       update();
   }//if-else
}//func contextMenuEvent

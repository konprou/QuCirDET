#include "edge.h"
#include "fnode.h"
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

fNode::fNode(GraphWidget *graphWidget, int i)
    : graph (graphWidget)
{
    ii=i;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
        selectedGate=1;
}

void fNode::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> fNode::edges() const
{
    return edgeList;
}
bool fNode::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}
QRectF fNode::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}
QPainterPath fNode::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void fNode::setGate(int gate)
{
    //if(val==1){gateCode = H;}
    //selectedGate=gate;
    //gateCode = H;
    //qDebug() << "x = " << selectedGate;
    //this->update();
}

void fNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(QPen(Qt::black, 0));
    //qDebug() << "gate = " << selectedGate;
    switch (selectedGate) {
    default:
        painter->setPen(QPen(Qt::black, 1));
        //painter->setBrush(QBrush(Qt::black));
        //painter->drawEllipse(-2, -2, 3, 3);
        painter->drawLine(-10, 0, 10, 0);
        //qDebug() << "ooooooook";
        break;

    case 1:
        painter->setBrush(QBrush(Qt::blue));
        s=QString("|k%1>").arg(ii);
        //s->setText("|k"+ii+">");
        //painter->drawRect(-10, -10, 20, 20);
        painter->drawText(-14, 4, s );
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
    case 5:
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
        //qDebug()<<"indexI= "<<indexI<<" indexJ= "<<indexJ;
        for(int i=0;i<NoQ1;i++){
            if(graph->gateTable[i][indexJ] == 12){ //to eixes 9
                Edge *edge = new Edge(graph->nodess[indexI][indexJ], graph->nodess[i][indexJ],0);
                graph->scene->addItem(edge);
            };
        }
        break;
    case 12://target
        painter->setPen(QPen(Qt::black, 1));
        painter->drawEllipse(-8, -8, 16, 16);
        painter->drawLine(-8,0,8,0);
        painter->drawLine(0,-8,0,8);
        for(int i=0;i<NoQ1;i++){
            if(graph->gateTable[i][indexJ] == 11){ //t oeixes 8
                Edge *edge = new Edge(graph->nodess[indexI][indexJ], graph->nodess[i][indexJ],0);
                graph->scene->addItem(edge);
            };
        }
        break;
    case 13:
        //painter->setBrush(QBrush(Qt::black));
        painter->drawEllipse(-4, -4, 25, 25);
        painter->drawText(-14, 4, "|0>" );
        //painter->drawLine(-8,0,8,0);

        break;
    }
}

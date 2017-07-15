#include "nodesyndrome.h"
#include "edgesyndrome.h"

#include <math.h>
#include <QPainter>

EdgeSyndrome::EdgeSyndrome(NodeSyndrome *sourceNode, NodeSyndrome *destNode,int col)
    : arrowSize(10)
{
    source = sourceNode;
    dest = destNode;
    source->addEdgeS(this);
    dest->addEdgeS(this);
    adjust();
    color=col;
}

NodeSyndrome *EdgeSyndrome::sourceNode() const
{
    return source;
}

NodeSyndrome *EdgeSyndrome::destNode() const
{
    return dest;
}

void EdgeSyndrome::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

QRectF EdgeSyndrome::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void EdgeSyndrome::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
    // Draw the line itself
    if(color==0){
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }else{
        painter->setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->drawLine(line);
}


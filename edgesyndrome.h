#ifndef EDGESYNDROME_H
#define EDGESYNDROME_H

#include <QGraphicsItem>

class NodeSyndrome;

class EdgeSyndrome: public QGraphicsItem
{
public:
    EdgeSyndrome(NodeSyndrome *sourceNode, NodeSyndrome *destNode, int col);

    NodeSyndrome *sourceNode() const;
    NodeSyndrome *destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const Q_DECL_OVERRIDE { return Type; }

protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);Q_DECL_OVERRIDE
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    NodeSyndrome *source, *dest;
    int color;
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;

};

#endif // EDGESYNDROME_H

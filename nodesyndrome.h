#ifndef NODESYNDROME_H
#define NODESYNDROME_H

#include <QGraphicsItem>
#include <QList>

class EdgeSyndrome;
class GraphWidget;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QWidget;
class QGraphicsItem;
class QGraphicsScene;
class QPaintEvent;
class QAction;
QT_END_NAMESPACE


class NodeSyndrome : public QGraphicsItem
{
public:
    NodeSyndrome(QMenu *contextMenu, GraphWidget *graphWidget, int NoQ, int NoS, int gate);

    void addEdgeS(EdgeSyndrome *edge);
    QList<EdgeSyndrome *> edges() const;
    int iPos,jPos,temp;
    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;

public slots:
    void setGate(int gate);


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event); //Q_DECL_OVERRIDE

private:
    QList<EdgeSyndrome *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
    QMenu *myContextMenu;
    int selectedGate;
    int NoQL, NoSL;
    int indexI, indexJ;
    QAction *act;


};

#endif // NODESYNDROME_H

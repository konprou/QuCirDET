#ifndef FNODE_H
#define FNODE_H

#include <QGraphicsItem>
#include <QList>
//#include <QWidget>

class Edge;
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

//! [0]
class fNode : public QGraphicsItem
{
public:
    fNode(GraphWidget *graphWidget, int i);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }


    bool advance();

    QRectF boundingRect() const;
    QPainterPath shape() const;

public slots:
    void setGate(int gate);


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QString s;

    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
    QMenu *myContextMenu;
    int selectedGate,ii;
    int NoQ1, NoS1;
    int indexI, indexJ;
    QAction *act;
};

#endif // FNODE_H

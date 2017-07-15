#ifndef NODE_H
#define NODE_H

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

class Node : public QGraphicsItem
{
public:
    Node(QMenu *contextMenu, GraphWidget *graphWidget, int NoQ, int NoS, int gate);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;
    int iPos,jPos,temp;
    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;

public slots:
    void setGate(int gate);


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

//    void mousePressEvent(QGraphicsSceneMouseEvent *event);Q_DECL_OVERRIDE
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);Q_DECL_OVERRIDE
//    #ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event); //Q_DECL_OVERRIDE
//    #endif // QT_NO_CONTEXTMENU

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
    QMenu *myContextMenu;
    int selectedGate;
    int NoQL, NoSL;
    int indexI, indexJ;
    QAction *act;

};

#endif // NODE_H

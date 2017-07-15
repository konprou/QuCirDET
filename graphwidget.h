#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>

class Node;
class NodeSyndrome;
class fNode;
class QMenu;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    int NoQ, NoS, gate, step, stepper, stepp, m, iMax, jMax, yCoor, xCoor, kAnc, jAnc, NoPhysBlocks,NoS_Syndrome;
    int gateTable[500][1000], gMtrxQEC[500][1000], gateTableSyndrome[500][300];
    GraphWidget(QMenu *itemMenu, int PQbts, int LQbts, int NoQ, int NoS, int NoAnc, int NoSyndSteps, int gateTable2[][1000], int NoQEC, int mode, QWidget *parent = 0);
    QGraphicsScene *scene;
    Node *nodess[500][1000];
    NodeSyndrome *nodesSynd[200][500];
    //Node *nodessAnc[500][1000]; // boreis na to mikreneis argotera
    fNode *fnodess[100], *fnodessSynd[100];

public slots:
    void zoomIn();
    void zoomOut();

protected:
    #ifndef QT_NO_WHEELEVENT
        void wheelEvent(QWheelEvent *event)Q_DECL_OVERRIDE;
    #endif
    void drawBackground(QPainter *painter, const QRectF &rect)Q_DECL_OVERRIDE;
    void scaleView(qreal scaleFactor);
//    void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;


private:
    QMenu *myItemMenu;
    Node *centerNode;
};

#endif // GRAPHWIDGET_H

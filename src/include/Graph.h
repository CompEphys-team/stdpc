#ifndef GRAPH_H
#define GRAPH_H
using namespace std;

#include <fstream>

#include <QObject>
#include <QGraphicsScene>
#include "ObjectDataTypes.h"

class Graph : public QObject
{
     Q_OBJECT

  private:
    qreal wd, ht;
    qreal x0, y0;
    qreal xTickLn, yTickLn;
    qreal xoff, xrange, dxt;
    qreal dyt;
    qreal lbxOff, lbyOff;
    
    qreal xlast, ylast[4];
    qreal gxlast;
    
    bool initial[4];
    
    double yrange[4];
    QPen pen[4];

    int nItems, nClear;
    int itemIdx[4];
    QVector<QGraphicsItem *> Items[4];
  
  protected:
    graphData *p;
    
  public:
    Graph();
    void init(graphData *);
    void drawFrame();
    void run() { }
    
    QGraphicsScene Scene;

  public slots:
    void addPoint(double, double, int);
};

#endif

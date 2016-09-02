using namespace std;

#include <fstream>

#include "Graph.h"
#include <QGraphicsTextItem>
#include <QString>
#include <QMessageBox>
#include <cmath>

ofstream os("C:\test.log");

Graph::Graph()
{
  x0= 40;
  y0= 8;
  wd= 460;
  ht= 130;
  xTickLn= 5;
  yTickLn= 5;
  xoff= 0;
  dxt= 500;
  dyt= 20;
  lbxOff= 10;
  lbyOff= 10;
  Scene.setSceneRect(0, 0, wd+x0, ht+y0);
  for (int i= 0; i < 4; i++) {
    initial[i]= true;
  }
}

void Graph::init(graphData *inp)
{
  QColor clr;
  
  p= inp;
  for (int i= 0; i < 4; i++) {
    clr.setNamedColor(p->color[i]);
    pen[i].setColor(clr);
//    pen[i].setWidth(2);
    yrange[i]= p->maxy[i] - p->miny[i];
  }
  xoff= 0;
  drawFrame();
  nItems = 0.00096 * p->xrange / p->dt;
  nClear = 0.00001 * p->xrange / p->dt;
  for (int i= 0; i < 4; i++) {
    initial[i]= true;
    itemIdx[i] = 0;
    Items[i].clear();
    Items[i].resize(nItems);
  }
}

void Graph::drawFrame()
{
  static qreal x, y, dxTick, dyTick;
  static qreal lby, lbxoff;
  static QString lb;
  static QGraphicsTextItem *theLb;
  static QList<QGraphicsItem *> delItems;
  static QGraphicsItem *theDelItem;
 
  // clear it all off
  delItems= Scene.items();
  while (!delItems.isEmpty()) {
    theDelItem= delItems.front();
    delItems.pop_front();
    Scene.removeItem(theDelItem);
  }
    
  Scene.addLine(x0, 0, x0+wd, 0);
  Scene.addLine(x0+wd, 0, x0+wd, ht);
  Scene.addLine(x0+wd, ht, x0, ht);
  Scene.addLine(x0, ht, x0, 0);
    
  dxTick= wd/p->xtNo;
  double dxt= p->xrange/p->xtNo;
  for (int i= 0; i < p->xtNo; i++) {
    x= x0+i*dxTick;
    y= ht;
    Scene.addLine(x, y, x, y+xTickLn); 
    lb.setNum(xoff+i*dxt);
    theLb= Scene.addText(lb);
    theLb->setPos(x-lbxOff, y+y0);
  }
  dyTick= ht/p->ytNo;
  double dyt= yrange[0]/p->ytNo;
  for (int i= 0; i < p->ytNo; i++) {
    x= x0;
    y= ht-i*dyTick;
    Scene.addLine(x-yTickLn, y, x, y); 
    lby= p->miny[0]+i*dyt;
    lby= round(lby*p->yfac[0]*100.0)/100.0;
    lb.setNum(lby);
    theLb= Scene.addText(lb);
    if (lby != 0.0) lbxoff= 5.0-floor(log(fabs(lby))/log(10));
    else lbxoff= 5.0;
    if (lby < 0) lbxoff--;
    theLb->setPos(x-x0+5*lbxoff-yTickLn, y-lbyOff);
  }
}

void Graph::addPoint(double t, double data, int pn)
{
  static qreal x, y, gx, gy;
  static qreal rx;

  rx= t*1000.0;

  int idx = itemIdx[pn] % nItems;
  if ( Items[pn][idx] && !(itemIdx[pn] % nClear) )
    for ( int i = 0; i < nClear; i++ )
        Scene.removeItem(Items[pn][(itemIdx[pn]+i)%nItems]);
  itemIdx[pn]++;

  x= rx-xoff;
  if (x > p->xrange) {
    xoff+= p->xrange;
    x-= p->xrange;
    for (int i = 0; i < 4; i++) {
      initial[i]= true;
    }
  }
  gx= x/p->xrange*wd+x0;
  y= data - p->miny[pn];
  gy= (1.0-y/yrange[pn])*ht;
  if (initial[pn]) {
    xlast= gx;
    ylast[pn]= gy;
    initial[pn]= false;
  }
  Items[pn][idx] = Scene.addLine(xlast, ylast[pn], gx, gy, pen[pn]);
  ylast[pn]= gy;
  xlast= gx;
}

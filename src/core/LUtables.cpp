
#include "Global.h"
#include "LUtables.h"
#include <cmath>
#include <QString>
#include <QMessageBox>

#define max(x,y) x>y?x:y

LUtable::LUtable()
{
  generated= false;
  requested= false;
}

void LUtable::reset()
{
  requested= false;
}

void LUtable::require(double inxmin, double inxmax, double indx)
{
  if (inxmin < Rxmin) Rxmin= inxmin;
  if (inxmax > Rxmax) Rxmax= inxmax;
  if (indx < dx) Rdx= indx;
  requested= true;
}

int LUtable::allocate()
{
  int change= 0;
  int bins= 0;
  
  if (requested) {
    change+= Rxmin < xmin;
    change+= Rxmax > xmax;
    change+= Rdx < dx;

    if ((change > 0) || (!generated)) {  
      dx= Rdx;  
      if (generated) {  
        delete[] data;
      }
      xmax= Rxmax;
      xmin= Rxmin;
      bins= (int) ((xmax+xmin)/dx) +1;
      data= new double[bins];
    }
  }
  return bins;
}

LUtable::~LUtable() 
{
  if (generated) {
    delete[] data;
  }
}

double LUtable::operator()(double x)
{
  if (x < xmin) x= xmin;
  if (x > xmax) x= xmax;
  return data[ (int) ((x-xmin)/dx) ];
}

// Lookup tables for various functions

int tanhLUtable::generate() 
{
  static double x;
  static int bins;

  bins= allocate();     
  if (bins > 0) {
    for (int i= 0; i < bins; i++) {
      x= xmin+i*dx;
      data[i]= tanh(x);
    }
    generated= true; 
  }
  return bins;
}

int expLUtable::generate() 
{
  static double x;
  static int bins;

  bins= allocate();     
  if (bins > 0) {
    for (int i= 0; i < bins; i++) {
      x= xmin+i*dx;
      data[i]= exp(x);
    }
    generated= true; 
  }
  return bins;
}

int expSigmoidLUtable::generate() 
{
  static double x;
  static int bins;

  bins= allocate();     
  if (bins > 0) {
    for (int i= 0; i < bins; i++) {
      x= xmin+i*dx;
      data[i]= 1.0/(1.0+exp(x));
    }
    generated= true; 
  }
  return bins;
}


inline double tanhFunction::operator()(double x)
{
  return tanh(x); 
}

inline double expFunction::operator()(double x)
{
  return exp(x); 
}

inline double expSigmoidFunction::operator()(double x)
{
  return 1.0/(1.0+exp(x)); 
}


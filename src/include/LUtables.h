
#ifndef LUTABLES_H
#define LUTABLES_H

#include "ObjectDataTypes.h"

#define LUVMIN -200.0
#define LUVMAX 100.0
#define LUDV 0.1

namespace stdpc {

class function {
  public:
    function() { }
    virtual double operator()(double)= 0;
};  

}

class LUtable: public stdpc::function {
  protected: 
    double *data;
    double xmin;
    double xmax;
    double dx;
    double Rxmin;
    double Rxmax;
    double Rdx;
    bool generated;
    bool requested;
    
  public:
    LUtable();
    virtual ~LUtable();
    void reset();
    void require(double, double, double);
    int allocate();
    virtual double operator()(double);
};


class tanhLUtable: public LUtable {
  public:
    int generate();         
};


class expLUtable: public LUtable {
  public:
    int generate();         
};
    
class expSigmoidLUtable: public LUtable {
  public:
    int generate();         
};
    
class tanhFunction: public stdpc::function {
  public:
    tanhFunction() { }
    virtual double operator()(double);
};

class expFunction: public stdpc::function {
  public:
    expFunction() { }
    virtual double operator()(double);
};

class expSigmoidFunction: public stdpc::function {
  public:
    expSigmoidFunction() { }
    virtual double operator()(double);
};
    

#endif

/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


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

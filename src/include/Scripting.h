#ifndef SCRIPTING_H
#define SCRIPTING_H

#include "Global.h"

class  scriptInstruction
{
  public:
   double t;
   APTYPE type;
   int index;
   void *value;
   
   scriptInstruction();
   ~scriptInstruction();
   scriptInstruction(const scriptInstruction &);
   void set(double, APTYPE, int, void *); 
};

#endif
        

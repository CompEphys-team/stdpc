
#include "scripting.h"

   
scriptInstruction::scriptInstruction()
{
  t= 0.0;
  type= DBLTYPE;
  index= 0;
  value= NULL;
}

scriptInstruction::scriptInstruction(const scriptInstruction &scr)
{
  double *dval;
  int *ival;
  bool *bval;
  QString *sval;
  
  t=scr.t;
  type= scr.type;
  index= scr.index;
  switch (type) {
    case DBLTYPE: dval= new double; *dval= *((double *) scr.value); value= (void *) dval; break;
    case INTTYPE: ival= new int; *ival= *((int *) scr.value); value= (void *) ival; break;
    case BOOLTYPE: bval= new bool; *bval= *((bool *) scr.value); value= (void *) bval; break;
    case STRTYPE: sval= new QString; *sval= *((QString *) scr.value); value= (void *) sval; break;
  }
}
   
scriptInstruction::~scriptInstruction()
{
  if (value != NULL) {
    switch (type) {
      case DBLTYPE: delete ((double *) value); break;
      case INTTYPE: delete ((int *) value); break;
      case BOOLTYPE: delete ((bool *) value); break;
      case STRTYPE: delete ((QString *) value); break;
    }
  }
}
   
void scriptInstruction::set(double inT, APTYPE inType, int inIndex, void *inValue)
{
  double *dval;
  int *ival;
  bool *bval;
  QString *sval;
  
  t= inT;
  type= inType;
  index= inIndex;
  switch (type) { 
    case DBLTYPE: 
         if (value != NULL) delete (double *) value;
         dval= new double; *dval= *((double *) inValue); value= (void *) dval; 
         break;
    case INTTYPE: 
         if (value != NULL) delete (int *) value;
         ival= new int; *ival= *((int *) inValue); value= (void *) ival; 
         break;
    case BOOLTYPE: 
         if (value != NULL) delete (bool *) value;
         bval= new bool; *bval= *((bool *) inValue); value= (void *) bval; 
         break;
    case STRTYPE: 
         if (value != NULL) delete (QString *) value;
         sval= new QString; *sval= *((QString *) inValue); value= (void *) sval; 
         break;
  }
}


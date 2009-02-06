 using namespace std;
 
 #include <fstream>
 #include "DCThread.h"
 #include "AP.h"
 #include <cmath>
 #include <climits>
 #include <QMessageBox>
  
 #include "SimulDAQ.h" // vor debugging only ...
 
DCThread::DCThread() 
{
  stopped= true;
  finished= true;
  csyn= new ChemSyn[MAX_SYN_NO];
  esyn= new GapJunction[MAX_SYN_NO];
  hh= new HH[MAX_HH_NO];
  abhh= new abHH[MAX_HH_NO];
  csIdx= new short int[MAX_SYN_NO];
  esIdx= new short int[MAX_SYN_NO];
  hhIdx= new short int[MAX_HH_NO];
  abhhIdx= new short int[MAX_HH_NO];

  inChn= NULL;
  outChn= NULL;
  inIdx= NULL;
  outIdx= NULL;
  scripting= false;
}

DCThread::~DCThread() 
{
   delete[] csyn;
   delete[] esyn;
   delete[] hh;
   delete[] abhh;
   delete[] csIdx;
   delete[] esIdx;
   delete[] hhIdx;
   delete[] abhhIdx;
   
   if (inChn != NULL) delete[] inChn;
   if (outChn != NULL) delete[] outChn;
   if (inIdx != NULL) delete[] inIdx;
   if (outIdx != NULL) delete[] outIdx;
}
 

void DCThread::init(DAQ *inBoard) 
{
   if (inChn != NULL) delete[] inChn;
   if (outChn != NULL) delete[] outChn;
   if (inIdx != NULL) delete[] inIdx;
   if (outIdx != NULL) delete[] outIdx;

   board= inBoard;
   inChn= new inChannel[board->inChnNo+1];  // +1 for the spike Generator
   outChn= new outChannel[board->outChnNo+1]; // +1 for "none" output
   inIdx= new short int[board->inChnNo+1];
   outIdx= new short int[board->outChnNo+1];
}

void DCThread::run()
{
   static int i;
   static double evt;
   // reset Lookup tables; they not be needed in this run ...
//   tanhLU.reset();
//   expLU.reset();
//   expSigmoidLU.reset();
   // collect the active channels
   //exit(1);
   inNo= 0;  
   for (i= 0; i < board->inChnNo; i++) {
     if (inChnp[i].active) {
       inIdx[inNo++]= i;
       inChn[i].init(&inChnp[i]);
     }
   }
   inIdx[inNo]= board->inChnNo;  // this is for the Spike Generator
   inChn[board->inChnNo].init(&inSpkGenChnp);
   outNo= 0;
   for (i=0; i < board->outChnNo; i++) {
     if (outChnp[i].active) {
       outIdx[outNo++]= i;
       outChn[i].init(&outChnp[i]);
     }
   }
   outIdx[outNo]= board->outChnNo;  // this is for the None output
   outChn[board->outChnNo].init(&outSpkGenChnp);
   
   board->generate_scan_list(inNo, inIdx);
   board->generate_analog_out_list(outNo, outIdx);

   if (SGp.active) {
     SG.init(&SGp, inNo, inIdx, inChn);
     if (!SGp.active) {    
       message(QString("SpkGen: Could not open ")+SGp.STInFName+QString(" ... SpkGen disabled ..."));
       inChn[inIdx[inNo]].V= 0.0;
     }
     else {
       message(QString("SpkGen active ..."));
     }
   }
   else {
     inChn[inIdx[inNo]].V= 0.0;
   }
   
   // set up the graphic display channels
   int idx;
   QString lb;
   for (i= 0; i < 2; i++) {
     grpNo[i]= 0;
     for (int j= 0; j < 4; j++) {
       if (Graphp[i].active[j]) {
         idx= Graphp[i].chn[j];
         if (idx == inNo+outNo+1) { // SG
           grp[i][grpNo[i]]= &(SG.V);
           pen[i][grpNo[i]++]= j;
         } else {
           if (idx > inNo) { // we are doing an out channel
             grp[i][grpNo[i]]= &(outChn[outIdx[idx-inNo-1]].I);
             pen[i][grpNo[i]++]= j;
           }
           else { // we are doing an in channel  
             grp[i][grpNo[i]]= &(inChn[inIdx[idx-1]].V);
             pen[i][grpNo[i]++]= j;
           }
         }
       }             
     }
     lb.setNum(grpNo[i]);
     message(QString("Added ")+lb+QString(" channels for Display"));
   }
                 
   csNo= 0;
   esNo= 0;
   for (i= 0; i < MAX_SYN_NO; i++) {
     if (CSynp[i].active) {
       csyn[i].init(&CSynp[i],inIdx,outIdx,inChn,outChn);
       csIdx[csNo++]= i;
     }
     if (ESynp[i].active) {
       esyn[i].init(&ESynp[i],inIdx,outIdx,inChn,outChn);
       esIdx[esNo++]= i;
     }
   }
   hhNo= 0;
   abhhNo= 0;
   for (i= 0; i < MAX_HH_NO; i++) {
     if (mhHHp[i].active) {
       hh[i].init(&mhHHp[i],inIdx,outIdx,inChn,outChn);
       hhIdx[hhNo++]= i;
     }
     if (abHHp[i].active) {
       abhh[i].init(&abHHp[i],inIdx,outIdx,inChn,outChn);
       abhhIdx[abhhNo++]= i;
     }
   }
   QString cN, eN, hN, aN;
   cN.setNum(csNo);
   eN.setNum(esNo);
   hN.setNum(hhNo);
   aN.setNum(abhhNo);
   if (csNo > 0) message(QString("DynClamp: ")+cN+QString(" chemical synapse(s) "));
   if (esNo > 0) message(QString("DynClamp: ")+eN+QString(" gap junction(s) "));
   if (hhNo > 0) message(QString("DynClamp: ")+hN+QString(" HH conductance(s) "));
   if (abhhNo > 0) message(QString("DynClamp: ")+aN+QString(" abHH conductance(s) ..."));

   int sz;   
   sz= tanhLU.generate();
   if (sz > 0) message(QString("Tanh lookup table (re)generated"));
   sz= expLU.generate();
   if (sz > 0) message(QString("Exp lookup table (re)generated"));
   sz= expSigmoidLU.generate();
   if (sz > 0) message(QString("Exponential sigmoid lookup table (re)generated"));
   
   board->reset_RTC();
   message(QString("DynClamp: Clamping ..."));
   stopped= false;
   finished= false;    
   t= 0.0;
   for (int i= 0; i < 2; i++) lastWrite[i]= t;
   // init scripting
   if (scripting) {
     scrIter= scriptq.begin();
     evt= scrIter->t;
   }
   else evt= 1e10;
   while (!stopped) {
     dt= board->get_RTC();
     t+= dt;
     board->get_scan(inChn);
     if (SGp.active) { // SpkGen active
       SG.VUpdate(t, dt);
     }
     for (i= 0; i <= inNo; i++) {
       inChn[inIdx[i]].spike_detect(t);  // the inChn decides whether to do anything or not
     }
     for (i= 0; i <= outNo; i++) {
       outChn[outIdx[i]].I= 0.0;
     }
     for (i= 0; i < csNo; i++) 
       csyn[csIdx[i]].currentUpdate(t, dt);
     for (i= 0; i < esNo; i++) 
       esyn[esIdx[i]].currentUpdate(t, dt);
     for (i= 0; i < hhNo; i++) 
       hh[hhIdx[i]].currentUpdate(t, dt);
     for (i= 0; i < abhhNo; i++) 
       abhh[abhhIdx[i]].currentUpdate(t, dt);
     // spike generator ... write stuff to DAQ
     // outChn[1].I= inChn[inIdx[inNo]].V;
     board->write_analog_out(outChn);
     if (t-lastWrite[0] > Graphp[0].dt) {
       lastWrite[0]= t;   
       for (i= 0; i < grpNo[0]; i++) {
         addPoint1(t, *grp[0][i], pen[0][i]);
       }
     }
     if (t-lastWrite[1] > Graphp[1].dt) {
       lastWrite[1]= t;   
       for (i= 0; i < grpNo[1]; i++) {
         addPoint2(t, *grp[1][i], pen[1][i]);
       }
     }
     // do scripting
     if (t >= evt) { // event needs doing
       switch (scrIter->type) {
         case DBLTYPE: *dAP[scrIter->index]= *((double *) scrIter->value); break;
         case INTTYPE: *iAP[scrIter->index]= *((int *) scrIter->value); break;
         case BOOLTYPE: *bAP[scrIter->index]= *((bool *) scrIter->value); break;
         case STRTYPE: *sAP[scrIter->index]= *((QString *) scrIter->value); break;
       }
       scrIter++;
       if (scrIter == scriptq.end()) evt= 1e10;
       else {
         evt= scrIter->t;
       }
     }
   }
   board->reset_board();
   finished= true;
} 

// Scripting support

bool DCThread::LoadScript(QString &fname)
{
  double et; 
  QString pname;
  double dvalue;
  int ivalue, index;
  bool bvalue, done, success;
  QString svalue, qn, vn;
  scriptInstruction inst;
  char buf[80];
  
  ifstream is(fname.toAscii());
     
  is >> et;
  done= false;
  success= true;
  while (!done)
  {
    is >> buf;
    pname= QString(buf);
    index= APname.indexOf(pname);
    if (index == -1) { 
      success= false;
    }
    else {
      switch (APtype[index]) {
        case DBLTYPE: is >> dvalue; break;
        case INTTYPE: is >> ivalue; break;
        case BOOLTYPE: is >> bvalue; break;
        case STRTYPE: is >> buf; svalue= QString(buf); break;
      }
      if (is.good()) {
        switch (APtype[index]) {
          case DBLTYPE: inst.set(et, APtype[index], APindex[index], &dvalue); break;
          case INTTYPE: inst.set(et, APtype[index], APindex[index], &ivalue); break;
          case BOOLTYPE: inst.set(et, APtype[index], APindex[index], &bvalue); break;
          case STRTYPE: inst.set(et, APtype[index], APindex[index], &svalue); break;
        }
        scriptq.append(inst);
      }
      else success= false;
    }
    is >> et;
    if ((!is.good()) || (!success)) done= true;
  }
  if (!success) {
    scriptq.clear();
    scripting= false;
    qn.setNum(et);
    message(QString("error in script file at instruction \"")+qn+QString(" ")+pname+QString("\""));
    message(QString("script not loaded"));
  }
  else {
    qn.setNum(scriptq.size());
    message(QString("script read with ")+qn+QString(" instructions"));
    scripting= true;
/*    for (scrIter= scriptq.begin(); scrIter != scriptq.end(); scrIter++) {
      qn.setNum((*scrIter).t);
      vn.setNum(*((double *) ((*scrIter).value)));
      message(QString("t= ")+qn+QString(" something with value ")+vn);
    }
*/  }
  return success;
}

void DCThread::UnloadScript()
{
  scriptq.clear();
  scripting= false;
  message(QString("script unloaded"));
}


void MyMainWindow::setupAP()
{
 
  QString qs, ql;
 
//  QMessageBox::warning(this, tr("My Application"),
//                tr("entering setupAP"),
//               QMessageBox::Close); 
  
  APname.clear();
  APtype.clear();
  APindex.clear();
  dAP.clear();
  iAP.clear();
  bAP.clear();
  sAP.clear();
 
  for (int s= 0; s < MAX_SYN_NO; s++)
  {
    qs.setNum(s);
    
    // STPlast
    APname.append(QString("CSynp[") + qs + QString("].ST.AMinus")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.AMinus));
    APname.append(QString("CSynp[") + qs + QString("].ST.tauMinus")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.tauMinus));
    APname.append(QString("CSynp[") + qs + QString("].ST.APlus")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.APlus));
    APname.append(QString("CSynp[") + qs + QString("].ST.tauPlus")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.tauPlus));
    APname.append(QString("CSynp[") + qs + QString("].ST.Exponent")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(CSynp[s].ST.Exponent));
    APname.append(QString("CSynp[") + qs + QString("].ST.Shift")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.Shift));
    APname.append(QString("CSynp[") + qs + QString("].ST.History")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(CSynp[s].ST.History));
    APname.append(QString("CSynp[") + qs + QString("].ST.Table")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(CSynp[s].ST.Table));
    APname.append(QString("CSynp[") + qs + QString("].ST.tableDt")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.tableDt));
    APname.append(QString("CSynp[") + qs + QString("].ST.tableDgMin")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.tableDgMin));
    APname.append(QString("CSynp[") + qs + QString("].ST.gMax")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.gMax));
    APname.append(QString("CSynp[") + qs + QString("].ST.gMid")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.gMid));
    APname.append(QString("CSynp[") + qs + QString("].ST.gSlope")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.gSlope));
    APname.append(QString("CSynp[") + qs + QString("].ST.sigmoidTable")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(CSynp[s].ST.sigmoidTable));
    APname.append(QString("CSynp[") + qs + QString("].ST.sigmoidTableDg")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.sigmoidTableDg));
    APname.append(QString("CSynp[") + qs + QString("].ST.sigmoidTableMaxEntry")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ST.sigmoidTableMaxEntry));

  // ODEPlast
    APname.append(QString("CSynp[") + qs + QString("].ODE.InitialP")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.InitialP));
    APname.append(QString("CSynp[") + qs + QString("].ODE.InitialD")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.InitialD));
    APname.append(QString("CSynp[") + qs + QString("].ODE.betaP")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.betaP));
    APname.append(QString("CSynp[") + qs + QString("].ODE.betaD")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.betaD));
    APname.append(QString("CSynp[") + qs + QString("].ODE.gamma")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.gamma));
    APname.append(QString("CSynp[") + qs + QString("].ODE.eta")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(CSynp[s].ODE.eta));
    APname.append(QString("CSynp[") + qs + QString("].ODE.highP")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.highP));
    APname.append(QString("CSynp[") + qs + QString("].ODE.lowP")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.lowP));
    APname.append(QString("CSynp[") + qs + QString("].ODE.highD")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.highD));
    APname.append(QString("CSynp[") + qs + QString("].ODE.lowD")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.lowD));
    APname.append(QString("CSynp[") + qs + QString("].ODE.gMax")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.gMax));
    APname.append(QString("CSynp[") + qs + QString("].ODE.gMid")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.gMid));
    APname.append(QString("CSynp[") + qs + QString("].ODE.gSlope")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.gSlope));
    APname.append(QString("CSynp[") + qs + QString("].ODE.sigmoidTable")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(CSynp[s].ODE.sigmoidTable));
    APname.append(QString("CSynp[") + qs + QString("].ODE.sigmoidTableDg")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.sigmoidTableDg));
    APname.append(QString("CSynp[") + qs + QString("].ODE.sigmoidTableMaxEntry")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].ODE.sigmoidTableMaxEntry));

    // main Csynp
    APname.append(QString("CSynp[") + qs + QString("].active")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(CSynp[s].active));
    APname.append(QString("CSynp[") + qs + QString("].LUTables")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(CSynp[s].LUTables));
    APname.append(QString("CSynp[") + qs + QString("].PreSynChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(CSynp[s].PreSynChannel));
    APname.append(QString("CSynp[") + qs + QString("].PostSynChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(CSynp[s].PostSynChannel));
    APname.append(QString("CSynp[") + qs + QString("].OutSynChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(CSynp[s].OutSynChannel));
    APname.append(QString("CSynp[") + qs + QString("].gSyn")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].gSyn));
    APname.append(QString("CSynp[") + qs + QString("].VSyn")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].VSyn));
    APname.append(QString("CSynp[") + qs + QString("].tauSyn")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].tauSyn));
    APname.append(QString("CSynp[") + qs + QString("].VThresh")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].VThresh));
    APname.append(QString("CSynp[") + qs + QString("].VSlope")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].VSlope));
    APname.append(QString("CSynp[") + qs + QString("].STD")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(CSynp[s].STD));
    APname.append(QString("CSynp[") + qs + QString("].STDAmpl")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].STDAmpl));
    APname.append(QString("CSynp[") + qs + QString("].STDVThresh")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].STDVThresh));
    APname.append(QString("CSynp[") + qs + QString("].STDVSlope")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].STDVSlope));
    APname.append(QString("CSynp[") + qs + QString("].STDtauAmpl")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].STDtauAmpl));
    APname.append(QString("CSynp[") + qs + QString("].STDtau0")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].STDtau0));
    APname.append(QString("CSynp[") + qs + QString("].STDtauVThresh")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].STDtauVThresh));
    APname.append(QString("CSynp[") + qs + QString("].STDtauVSlope")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].STDtauVSlope));
    APname.append(QString("CSynp[") + qs + QString("].fixVpost")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(CSynp[s].fixVpost));
    APname.append(QString("CSynp[") + qs + QString("].Vpost"));  APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(CSynp[s].Vpost));
    APname.append(QString("CSynp[") + qs + QString("].Plasticity")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(CSynp[s].Plasticity));
  }
 
  
  for (int s= 0; s < MAX_SYN_NO; s++)
  {
    qs.setNum(s);
 
    APname.append(QString("ESynp[") + qs + QString("].active")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(ESynp[s].active));
    APname.append(QString("ESynp[") + qs + QString("].preInChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(ESynp[s].preInChannel));
    APname.append(QString("ESynp[") + qs + QString("].postInChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(ESynp[s].postInChannel));
    APname.append(QString("ESynp[") + qs + QString("].preOutChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(ESynp[s].preOutChannel));
    APname.append(QString("ESynp[") + qs + QString("].postOutChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(ESynp[s].postOutChannel));
    APname.append(QString("ESynp[") + qs + QString("].type")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(ESynp[s].type));
    APname.append(QString("ESynp[") + qs + QString("].gSyn")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(ESynp[s].gSyn));
  }

  for (int s= 0; s < MAX_HH_NO; s++) {
    qs.setNum(s);
      
    APname.append(QString("mhHHp[") + qs + QString("].active")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(mhHHp[s].active));
    APname.append(QString("mhHHp[") + qs + QString("].LUTables")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(mhHHp[s].LUTables));    
    APname.append(QString("mhHHp[") + qs + QString("].VChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(mhHHp[s].VChannel));
    APname.append(QString("mhHHp[") + qs + QString("].IChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(mhHHp[s].IChannel));
    APname.append(QString("mhHHp[") + qs + QString("].gMax")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].gMax));
    APname.append(QString("mhHHp[") + qs + QString("].Vrev")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].Vrev));
    APname.append(QString("mhHHp[") + qs + QString("].mExpo")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(mhHHp[s].mExpo));
    APname.append(QString("mhHHp[") + qs + QString("].hExpo")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(mhHHp[s].hExpo));
    APname.append(QString("mhHHp[") + qs + QString("].Vm")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].Vm));
    APname.append(QString("mhHHp[") + qs + QString("].sm")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].sm));
    APname.append(QString("mhHHp[") + qs + QString("].Cm")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].Cm));
    APname.append(QString("mhHHp[") + qs + QString("].taum")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].taum));
    APname.append(QString("mhHHp[") + qs + QString("].taumAmpl")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].taumAmpl));
    APname.append(QString("mhHHp[") + qs + QString("].Vtaum")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].Vtaum));
    APname.append(QString("mhHHp[") + qs + QString("].staum")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].staum));
    APname.append(QString("mhHHp[") + qs + QString("].Vh")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].Vh));
    APname.append(QString("mhHHp[") + qs + QString("].sh")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].sh));
    APname.append(QString("mhHHp[") + qs + QString("].Ch")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].Ch));
    APname.append(QString("mhHHp[") + qs + QString("].tauh")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].tauh));
    APname.append(QString("mhHHp[") + qs + QString("].tauhAmpl")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].tauhAmpl));
    APname.append(QString("mhHHp[") + qs + QString("].Vtauh")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].Vtauh));
    APname.append(QString("mhHHp[") + qs + QString("].stauh")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(mhHHp[s].stauh));
  }

  // abHHp

  for (int s= 0; s < MAX_HH_NO; s++) {
    qs.setNum(s);
    
    APname.append(QString("abHHp[") + qs + QString("].active")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(abHHp[s].active));
    APname.append(QString("abHHp[") + qs + QString("].LUTables")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(abHHp[s].LUTables));
    APname.append(QString("abHHp[") + qs + QString("].VChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(abHHp[s].VChannel));
    APname.append(QString("abHHp[") + qs + QString("].IChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(abHHp[s].IChannel));
    APname.append(QString("abHHp[") + qs + QString("].gMax")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].gMax));
    APname.append(QString("abHHp[") + qs + QString("].Vrev")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].Vrev));
    APname.append(QString("abHHp[") + qs + QString("].mExpo")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(abHHp[s].mExpo));
    APname.append(QString("abHHp[") + qs + QString("].hExpo")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(abHHp[s].hExpo));
    APname.append(QString("abHHp[") + qs + QString("].maFunc")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(abHHp[s].maFunc));
    APname.append(QString("abHHp[") + qs + QString("].mka")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].mka));
    APname.append(QString("abHHp[") + qs + QString("].mVa")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].mVa));
    APname.append(QString("abHHp[") + qs + QString("].msa")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].msa));
    APname.append(QString("abHHp[") + qs + QString("].mbFunc")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(abHHp[s].mbFunc));
    APname.append(QString("abHHp[") + qs + QString("].mkb")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].mkb));
    APname.append(QString("abHHp[") + qs + QString("].mVb")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].mVb));
    APname.append(QString("abHHp[") + qs + QString("].msb")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].msb));
    APname.append(QString("abHHp[") + qs + QString("].haFunc")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(abHHp[s].haFunc));
    APname.append(QString("abHHp[") + qs + QString("].hka")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].hka));
    APname.append(QString("abHHp[") + qs + QString("].hVa")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].hVa));
    APname.append(QString("abHHp[") + qs + QString("].hsa")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].hsa));
    APname.append(QString("abHHp[") + qs + QString("].hbFunc")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(abHHp[s].hbFunc));
    APname.append(QString("abHHp[") + qs + QString("].hkb")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].hkb));
    APname.append(QString("abHHp[") + qs + QString("].hVb")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].hVb));
    APname.append(QString("abHHp[") + qs + QString("].hsb")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(abHHp[s].hsb));
  }
  
  // Spike generator
   
  APname.append(QString("SGp.active")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
  bAP.append(&(SGp.active));
  APname.append(QString("SGp.method")); APtype.append(INTTYPE); APindex.append(iAP.size());      
  iAP.append(&(SGp.method));
  APname.append(QString("SGp.LUTables")); APtype.append(INTTYPE); APindex.append(iAP.size());
  iAP.append(&(SGp.LUTables));
  APname.append(QString("SGp.VSpike")); APtype.append(DBLTYPE); APindex.append(dAP.size());
  dAP.append(&(SGp.VSpike));  
  APname.append(QString("SGp.spkTimeScaling")); APtype.append(DBLTYPE); APindex.append(dAP.size());
  dAP.append(&(SGp.spkTimeScaling));
  APname.append(QString("SGp.VRest")); APtype.append(DBLTYPE); APindex.append(dAP.size());
  dAP.append(&(SGp.VRest));
  APname.append(QString("SGp.bdType")); APtype.append(INTTYPE); APindex.append(iAP.size());
  iAP.append(&(SGp.bdType));
  APname.append(QString("SGp.bdChannel")); APtype.append(INTTYPE); APindex.append(iAP.size());
  iAP.append(&(SGp.bdChannel));
  APname.append(QString("SGp.bdThresh")); APtype.append(DBLTYPE); APindex.append(dAP.size());
  dAP.append(&(SGp.bdThresh));
  APname.append(QString("SGp.bdNUnder")); APtype.append(INTTYPE); APindex.append(iAP.size());
  iAP.append(&(SGp.bdNUnder));
  APname.append(QString("SGp.bdNOver")); APtype.append(INTTYPE); APindex.append(iAP.size());
  iAP.append(&(SGp.bdNOver));
  APname.append(QString("SGp.period")); APtype.append(DBLTYPE); APindex.append(dAP.size());
  dAP.append(&(SGp.period));
  APname.append(QString("SGp.SpikeNo")); APtype.append(INTTYPE); APindex.append(iAP.size());
  iAP.append(&(SGp.SpikeNo));
  APname.append(QString("SGp.STInFName")); APtype.append(STRTYPE); APindex.append(sAP.size());
  sAP.append(&(SGp.STInFName));

  for (int k= 0; k < 10; k++) {
    qs.setNum(k);
      
    APname.append(QString("SGp.SpikeT[")+ qs + QString("]")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(SGp.SpikeT[k]));
  }
  
  for (int k= 0; k < board->inChnNo; k++) {
    qs.setNum(k);
    
    APname.append(QString("inChnp[")+ qs + QString("].active")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(inChnp[k].active)); 
    APname.append(QString("inChnp[")+ qs + QString("].gain")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(inChnp[k].gain)); 
    APname.append(QString("inChnp[")+ qs + QString("].gainFac")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(inChnp[k].gainFac)); 
    APname.append(QString("inChnp[")+ qs + QString("].spkDetect")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(inChnp[k].spkDetect)); 
    APname.append(QString("inChnp[")+ qs + QString("].spkDetectThresh")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(inChnp[k].spkDetectThresh)); 
  }

  for (int k= 0; k < board->outChnNo; k++) {
    qs.setNum(k);
    
    APname.append(QString("outChnp[")+ qs + QString("].active")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
    bAP.append(&(outChnp[k].active)); 
    APname.append(QString("outChnp[")+ qs + QString("].gain")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(outChnp[k].gain)); 
    APname.append(QString("outChnp[")+ qs + QString("].gainFac")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(outChnp[k].gainFac)); 
    APname.append(QString("outChnp[")+ qs + QString("].bias")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(outChnp[k].bias)); 
  }
  
  for (int k= 0; k < 2; k++) {
    qs.setNum(k);
    
    for (int l= 0; l < 4; l++) {
      ql.setNum(l);
      
      APname.append(QString("Graphp[") + qs + QString("].active[") + ql + QString("]")); APtype.append(BOOLTYPE); APindex.append(bAP.size());
      bAP.append(&(Graphp[k].active[l]));
      APname.append(QString("Graphp[") + qs + QString("].color[") + ql + QString("]")); APtype.append(STRTYPE);APindex.append(sAP.size());
      sAP.append(&(Graphp[k].color[l]));
      APname.append(QString("Graphp[") + qs + QString("].chn[") + ql + QString("]")); APtype.append(INTTYPE); APindex.append(iAP.size());
      iAP.append(&(Graphp[k].chn[l]));
      APname.append(QString("Graphp[") + qs + QString("].miny[") + ql + QString("]")); APtype.append(DBLTYPE); APindex.append(dAP.size());
      dAP.append(&(Graphp[k].miny[l]));
      APname.append(QString("Graphp[") + qs + QString("].maxy[") + ql + QString("]")); APtype.append(DBLTYPE); APindex.append(dAP.size());
      dAP.append(&(Graphp[k].maxy[l]));
      APname.append(QString("Graphp[") + qs + QString("].yfac[") + ql + QString("]")); APtype.append(DBLTYPE); APindex.append(dAP.size());
      dAP.append(&(Graphp[k].yfac[l]));
    }  
    APname.append(QString("Graphp[") + qs + QString("].xrange")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(Graphp[k].xrange));
    APname.append(QString("Graphp[") + qs + QString("].dt")); APtype.append(DBLTYPE); APindex.append(dAP.size());
    dAP.append(&(Graphp[k].dt));
    APname.append(QString("Graphp[") + qs + QString("].xtNo")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(Graphp[k].xtNo));
    APname.append(QString("Graphp[") + qs + QString("].ytNo")); APtype.append(INTTYPE); APindex.append(iAP.size());
    iAP.append(&(Graphp[k].ytNo));
  }    
 assert(APname.size() == dAP.size() + iAP.size() + bAP.size() + sAP.size());

}

void MyMainWindow::doSaveProtocol(QString &fname)
{
  ofstream os(fname.toAscii());
  
  exportData();

  os << DAQComboBox->currentIndex() << endl;
  os << SDAQp << endl;
  os << DigiDatap << endl;
#ifdef NIDAQ
  os << NIDAQp << endl;
#endif
  os << endl;

  for (int i= 0; i < APname.size(); i++) {
    os << APname[i].toStdString() << " ";
    switch (APtype[i]) {
      case DBLTYPE: os << *(dAP[APindex[i]]) << endl; break;
      case INTTYPE: os << *(iAP[APindex[i]]) << endl; break;
      case BOOLTYPE: os << *(bAP[APindex[i]]) << endl; break;
      case STRTYPE: os << sAP[APindex[i]]->toStdString() << endl; break;
    }
  }
  os.close();
}


void MyMainWindow::doLoadProtocol(QString &fname)
{
  ifstream is(fname.toAscii());
  char name[80];
  char buf[255];
  int i= 0, itmp;

  if (!is.good()) {
    DisplayMessage(QString("Error opening Protocol file")); 
    return;
  }  
  is >> itmp;
  is >> SDAQp;
  is >> DigiDatap;
#ifdef NIDAQ
  is >> NIDAQp;
#endif
  if (!is.good()) {
    DisplayMessage(QString("Protocol file truncated ... error")); 
    return;
  }  
  SDAQDlg->importData(SDAQp);
  DDataDlg->importData(DigiDatap);
#ifdef NIDAQ
  NDQDlg->importData(NIDAQp);
#endif
  DAQComboBox->setCurrentIndex(itmp);
  
//  DAQSetup();

  setupAP();
  is >> name;
  while (is.good()) {
    i= APname.indexOf(QString(name));
    switch (APtype[i]) {
      case DBLTYPE: is >> *(dAP[APindex[i]]); break;
      case INTTYPE: is >> *(iAP[APindex[i]]); break;
      case BOOLTYPE: is >> *(bAP[APindex[i]]); break;
      case STRTYPE: is >> buf; *(sAP[APindex[i]])= QString(buf); break;
    }
    is >> name;
  } 
  is.close();
  importData();
  QMessageBox::warning(this, tr("My Application"),
                QString("last parameter ")+APname[i],
               QMessageBox::Close); 

 
}

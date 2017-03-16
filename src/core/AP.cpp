#include "AP.h"
#include "Global.h"
#include "ModelManager.h"

void initAP()
{
    // CSynp STPlast
    addAP("CSynp[#].ST.AMinus", &CSynp, &CSynData::ST, &STPlast::AMinus);
    addAP("CSynp[#].ST.tauMinus", &CSynp, &CSynData::ST, &STPlast::tauMinus);
    addAP("CSynp[#].ST.APlus", &CSynp, &CSynData::ST, &STPlast::APlus);
    addAP("CSynp[#].ST.tauPlus", &CSynp, &CSynData::ST, &STPlast::tauPlus);
    addAP("CSynp[#].ST.Exponent", &CSynp, &CSynData::ST, &STPlast::Exponent);
    addAP("CSynp[#].ST.Shift", &CSynp, &CSynData::ST, &STPlast::Shift);
    addAP("CSynp[#].ST.History", &CSynp, &CSynData::ST, &STPlast::History);
    addAP("CSynp[#].ST.Table", &CSynp, &CSynData::ST, &STPlast::Table);
    addAP("CSynp[#].ST.tableDt", &CSynp, &CSynData::ST, &STPlast::tableDt);
    addAP("CSynp[#].ST.tableDgMin", &CSynp, &CSynData::ST, &STPlast::tableDgMin);
    addAP("CSynp[#].ST.gMax", &CSynp, &CSynData::ST, &STPlast::gMax);
    addAP("CSynp[#].ST.gMid", &CSynp, &CSynData::ST, &STPlast::gMid);
    addAP("CSynp[#].ST.gSlope", &CSynp, &CSynData::ST, &STPlast::gSlope);
    addAP("CSynp[#].ST.sigmoidTable", &CSynp, &CSynData::ST, &STPlast::sigmoidTable);
    addAP("CSynp[#].ST.sigmoidTableDg", &CSynp, &CSynData::ST, &STPlast::sigmoidTableDg);
    addAP("CSynp[#].ST.sigmoidTableMaxEntry", &CSynp, &CSynData::ST, &STPlast::sigmoidTableMaxEntry);

    // CSynp ODEPlast
    addAP("CSynp[#].ODE.InitialP", &CSynp, &CSynData::ODE, &ODEPlast::InitialP);
    addAP("CSynp[#].ODE.InitialD", &CSynp, &CSynData::ODE, &ODEPlast::InitialD);
    addAP("CSynp[#].ODE.betaP", &CSynp, &CSynData::ODE, &ODEPlast::betaP);
    addAP("CSynp[#].ODE.betaD", &CSynp, &CSynData::ODE, &ODEPlast::betaD);
    addAP("CSynp[#].ODE.gamma", &CSynp, &CSynData::ODE, &ODEPlast::gamma);
    addAP("CSynp[#].ODE.eta", &CSynp, &CSynData::ODE, &ODEPlast::eta);
    addAP("CSynp[#].ODE.highP", &CSynp, &CSynData::ODE, &ODEPlast::highP);
    addAP("CSynp[#].ODE.lowP", &CSynp, &CSynData::ODE, &ODEPlast::lowP);
    addAP("CSynp[#].ODE.highD", &CSynp, &CSynData::ODE, &ODEPlast::highD);
    addAP("CSynp[#].ODE.lowD", &CSynp, &CSynData::ODE, &ODEPlast::lowD);
    addAP("CSynp[#].ODE.gMax", &CSynp, &CSynData::ODE, &ODEPlast::gMax);
    addAP("CSynp[#].ODE.gMid", &CSynp, &CSynData::ODE, &ODEPlast::gMid);
    addAP("CSynp[#].ODE.gSlope", &CSynp, &CSynData::ODE, &ODEPlast::gSlope);
    addAP("CSynp[#].ODE.sigmoidTable", &CSynp, &CSynData::ODE, &ODEPlast::sigmoidTable);
    addAP("CSynp[#].ODE.sigmoidTableDg", &CSynp, &CSynData::ODE, &ODEPlast::sigmoidTableDg);
    addAP("CSynp[#].ODE.sigmoidTableMaxEntry", &CSynp, &CSynData::ODE, &ODEPlast::sigmoidTableMaxEntry);

    // main Csynp
    addAP("CSynp[#].active", &CSynp, &CSynData::active);
    addAP("CSynp[#].LUTables", &CSynp, &CSynData::LUTables);
    addAP("CSynp[#].MgBlock", &CSynp, &CSynData::MgBlock);
    addAP("CSynp[#].gSyn", &CSynp, &CSynData::gSyn);
    addAP("CSynp[#].VSyn", &CSynp, &CSynData::VSyn);
    addAP("CSynp[#].tauSyn", &CSynp, &CSynData::tauSyn);
    addAP("CSynp[#].VThresh", &CSynp, &CSynData::VThresh);
    addAP("CSynp[#].VSlope", &CSynp, &CSynData::VSlope);
    addAP("CSynp[#].STD", &CSynp, &CSynData::STD);
    addAP("CSynp[#].STDAmpl", &CSynp, &CSynData::STDAmpl);
    addAP("CSynp[#].STDVThresh", &CSynp, &CSynData::STDVThresh);
    addAP("CSynp[#].STDVSlope", &CSynp, &CSynData::STDVSlope);
    addAP("CSynp[#].STDtauAmpl", &CSynp, &CSynData::STDtauAmpl);
    addAP("CSynp[#].STDtau0", &CSynp, &CSynData::STDtau0);
    addAP("CSynp[#].STDtauVThresh", &CSynp, &CSynData::STDtauVThresh);
    addAP("CSynp[#].STDtauVSlope", &CSynp, &CSynData::STDtauVSlope);
    addAP("CSynp[#].fixVpost", &CSynp, &CSynData::fixVpost);
    addAP("CSynp[#].Vpost", &CSynp, &CSynData::Vpost);
    addAP("CSynp[#].Mgfac", &CSynp, &CSynData::Mgfac);
    addAP("CSynp[#].Mgexpo", &CSynp, &CSynData::Mgexpo);
    addAP("CSynp[#].Plasticity", &CSynp, &CSynData::Plasticity);
    addAP("CSynp[#].assign[#].active", &CSynp, &CSynData::assign, &SynapseAssignment::active);
    addAP("CSynp[#].assign[#].PreSynChannel", &CSynp, &CSynData::assign, &SynapseAssignment::PreSynChannel);
    addAP("CSynp[#].assign[#].PostSynChannel", &CSynp, &CSynData::assign, &SynapseAssignment::PostSynChannel);
    addAP("CSynp[#].assign[#].OutSynChannel", &CSynp, &CSynData::assign, &SynapseAssignment::OutSynChannel);
    addAP("CSynp[#].assign[#].delay", &CSynp, &CSynData::assign, &SynapseAssignment::delay);

    addAP("CSynp[#].PreSynChannel", &CSynp, &CSynData::legacy_PreSyn);
    addAP("CSynp[#].PostSynChannel", &CSynp, &CSynData::legacy_PostSyn);
    addAP("CSynp[#].OutSynChannel", &CSynp, &CSynData::legacy_OutSyn);


    // abSynp STPlast
    addAP("abSynp[#].ST.AMinus", &abSynp, &abSynData::ST, &STPlast::AMinus);
    addAP("abSynp[#].ST.tauMinus", &abSynp, &abSynData::ST, &STPlast::tauMinus);
    addAP("abSynp[#].ST.APlus", &abSynp, &abSynData::ST, &STPlast::APlus);
    addAP("abSynp[#].ST.tauPlus", &abSynp, &abSynData::ST, &STPlast::tauPlus);
    addAP("abSynp[#].ST.Exponent", &abSynp, &abSynData::ST, &STPlast::Exponent);
    addAP("abSynp[#].ST.Shift", &abSynp, &abSynData::ST, &STPlast::Shift);
    addAP("abSynp[#].ST.History", &abSynp, &abSynData::ST, &STPlast::History);
    addAP("abSynp[#].ST.Table", &abSynp, &abSynData::ST, &STPlast::Table);
    addAP("abSynp[#].ST.tableDt", &abSynp, &abSynData::ST, &STPlast::tableDt);
    addAP("abSynp[#].ST.tableDgMin", &abSynp, &abSynData::ST, &STPlast::tableDgMin);
    addAP("abSynp[#].ST.gMax", &abSynp, &abSynData::ST, &STPlast::gMax);
    addAP("abSynp[#].ST.gMid", &abSynp, &abSynData::ST, &STPlast::gMid);
    addAP("abSynp[#].ST.gSlope", &abSynp, &abSynData::ST, &STPlast::gSlope);
    addAP("abSynp[#].ST.sigmoidTable", &abSynp, &abSynData::ST, &STPlast::sigmoidTable);
    addAP("abSynp[#].ST.sigmoidTableDg", &abSynp, &abSynData::ST, &STPlast::sigmoidTableDg);
    addAP("abSynp[#].ST.sigmoidTableMaxEntry", &abSynp, &abSynData::ST, &STPlast::sigmoidTableMaxEntry);

    // abSynp ODEPlast
    addAP("abSynp[#].ODE.InitialP", &abSynp, &abSynData::ODE, &ODEPlast::InitialP);
    addAP("abSynp[#].ODE.InitialD", &abSynp, &abSynData::ODE, &ODEPlast::InitialD);
    addAP("abSynp[#].ODE.betaP", &abSynp, &abSynData::ODE, &ODEPlast::betaP);
    addAP("abSynp[#].ODE.betaD", &abSynp, &abSynData::ODE, &ODEPlast::betaD);
    addAP("abSynp[#].ODE.gamma", &abSynp, &abSynData::ODE, &ODEPlast::gamma);
    addAP("abSynp[#].ODE.eta", &abSynp, &abSynData::ODE, &ODEPlast::eta);
    addAP("abSynp[#].ODE.highP", &abSynp, &abSynData::ODE, &ODEPlast::highP);
    addAP("abSynp[#].ODE.lowP", &abSynp, &abSynData::ODE, &ODEPlast::lowP);
    addAP("abSynp[#].ODE.highD", &abSynp, &abSynData::ODE, &ODEPlast::highD);
    addAP("abSynp[#].ODE.lowD", &abSynp, &abSynData::ODE, &ODEPlast::lowD);
    addAP("abSynp[#].ODE.gMax", &abSynp, &abSynData::ODE, &ODEPlast::gMax);
    addAP("abSynp[#].ODE.gMid", &abSynp, &abSynData::ODE, &ODEPlast::gMid);
    addAP("abSynp[#].ODE.gSlope", &abSynp, &abSynData::ODE, &ODEPlast::gSlope);
    addAP("abSynp[#].ODE.sigmoidTable", &abSynp, &abSynData::ODE, &ODEPlast::sigmoidTable);
    addAP("abSynp[#].ODE.sigmoidTableDg", &abSynp, &abSynData::ODE, &ODEPlast::sigmoidTableDg);
    addAP("abSynp[#].ODE.sigmoidTableMaxEntry", &abSynp, &abSynData::ODE, &ODEPlast::sigmoidTableMaxEntry);

    // main abSynp
    addAP("abSynp[#].active", &abSynp, &abSynData::active);
    addAP("abSynp[#].LUTables", &abSynp, &abSynData::LUTables);
    addAP("abSynp[#].gSyn", &abSynp, &abSynData::gSyn);
    addAP("abSynp[#].Vrev", &abSynp, &abSynData::Vrev);
    addAP("abSynp[#].aS", &abSynp, &abSynData::aS);
    addAP("abSynp[#].bS", &abSynp, &abSynData::bS);
    addAP("abSynp[#].aR", &abSynp, &abSynData::aR);
    addAP("abSynp[#].VaR", &abSynp, &abSynData::VaR);
    addAP("abSynp[#].saR", &abSynp, &abSynData::saR);
    addAP("abSynp[#].bR", &abSynp, &abSynData::bR);
    addAP("abSynp[#].fixVpost", &abSynp, &abSynData::fixVpost);
    addAP("abSynp[#].Vpost", &abSynp, &abSynData::Vpost);
    addAP("abSynp[#].Plasticity", &abSynp, &abSynData::Plasticity);
    addAP("abSynp[#].assign[#].active", &abSynp, &abSynData::assign, &SynapseAssignment::active);
    addAP("abSynp[#].assign[#].PreSynChannel", &abSynp, &abSynData::assign, &SynapseAssignment::PreSynChannel);
    addAP("abSynp[#].assign[#].PostSynChannel", &abSynp, &abSynData::assign, &SynapseAssignment::PostSynChannel);
    addAP("abSynp[#].assign[#].OutSynChannel", &abSynp, &abSynData::assign, &SynapseAssignment::OutSynChannel);
    addAP("abSynp[#].assign[#].delay", &abSynp, &abSynData::assign, &SynapseAssignment::delay);

    addAP("abSynp[#].PreSynChannel", &abSynp, &abSynData::legacy_PreSyn);
    addAP("abSynp[#].PostSynChannel", &abSynp, &abSynData::legacy_PostSyn);
    addAP("abSynp[#].OutSynChannel", &abSynp, &abSynData::legacy_OutSyn);


    // DxheSynp STPlast
    addAP("DxheSynp[#].ST.AMinus", &DxheSynp, &DestexheSynData::ST, &STPlast::AMinus);
    addAP("DxheSynp[#].ST.tauMinus", &DxheSynp, &DestexheSynData::ST, &STPlast::tauMinus);
    addAP("DxheSynp[#].ST.APlus", &DxheSynp, &DestexheSynData::ST, &STPlast::APlus);
    addAP("DxheSynp[#].ST.tauPlus", &DxheSynp, &DestexheSynData::ST, &STPlast::tauPlus);
    addAP("DxheSynp[#].ST.Exponent", &DxheSynp, &DestexheSynData::ST, &STPlast::Exponent);
    addAP("DxheSynp[#].ST.Shift", &DxheSynp, &DestexheSynData::ST, &STPlast::Shift);
    addAP("DxheSynp[#].ST.History", &DxheSynp, &DestexheSynData::ST, &STPlast::History);
    addAP("DxheSynp[#].ST.Table", &DxheSynp, &DestexheSynData::ST, &STPlast::Table);
    addAP("DxheSynp[#].ST.tableDt", &DxheSynp, &DestexheSynData::ST, &STPlast::tableDt);
    addAP("DxheSynp[#].ST.tableDgMin", &DxheSynp, &DestexheSynData::ST, &STPlast::tableDgMin);
    addAP("DxheSynp[#].ST.gMax", &DxheSynp, &DestexheSynData::ST, &STPlast::gMax);
    addAP("DxheSynp[#].ST.gMid", &DxheSynp, &DestexheSynData::ST, &STPlast::gMid);
    addAP("DxheSynp[#].ST.gSlope", &DxheSynp, &DestexheSynData::ST, &STPlast::gSlope);
    addAP("DxheSynp[#].ST.sigmoidTable", &DxheSynp, &DestexheSynData::ST, &STPlast::sigmoidTable);
    addAP("DxheSynp[#].ST.sigmoidTableDg", &DxheSynp, &DestexheSynData::ST, &STPlast::sigmoidTableDg);
    addAP("DxheSynp[#].ST.sigmoidTableMaxEntry", &DxheSynp, &DestexheSynData::ST, &STPlast::sigmoidTableMaxEntry);

    // DxheSynp ODEPlast
    addAP("DxheSynp[#].ODE.InitialP", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::InitialP);
    addAP("DxheSynp[#].ODE.InitialD", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::InitialD);
    addAP("DxheSynp[#].ODE.betaP", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::betaP);
    addAP("DxheSynp[#].ODE.betaD", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::betaD);
    addAP("DxheSynp[#].ODE.gamma", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::gamma);
    addAP("DxheSynp[#].ODE.eta", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::eta);
    addAP("DxheSynp[#].ODE.highP", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::highP);
    addAP("DxheSynp[#].ODE.lowP", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::lowP);
    addAP("DxheSynp[#].ODE.highD", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::highD);
    addAP("DxheSynp[#].ODE.lowD", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::lowD);
    addAP("DxheSynp[#].ODE.gMax", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::gMax);
    addAP("DxheSynp[#].ODE.gMid", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::gMid);
    addAP("DxheSynp[#].ODE.gSlope", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::gSlope);
    addAP("DxheSynp[#].ODE.sigmoidTable", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::sigmoidTable);
    addAP("DxheSynp[#].ODE.sigmoidTableDg", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::sigmoidTableDg);
    addAP("DxheSynp[#].ODE.sigmoidTableMaxEntry", &DxheSynp, &DestexheSynData::ODE, &ODEPlast::sigmoidTableMaxEntry);

    // main DxheSynp
    addAP("DxheSynp[#].active", &DxheSynp, &DestexheSynData::active);
    addAP("DxheSynp[#].LUTables", &DxheSynp, &DestexheSynData::LUTables);
    addAP("DxheSynp[#].gSyn", &DxheSynp, &DestexheSynData::gSyn);
    addAP("DxheSynp[#].Vpre", &DxheSynp, &DestexheSynData::Vpre);
    addAP("DxheSynp[#].Vrev", &DxheSynp, &DestexheSynData::Vrev);
    addAP("DxheSynp[#].trelease", &DxheSynp, &DestexheSynData::trelease);
    addAP("DxheSynp[#].alpha", &DxheSynp, &DestexheSynData::alpha);
    addAP("DxheSynp[#].beta", &DxheSynp, &DestexheSynData::beta);
    addAP("DxheSynp[#].fixVpost", &DxheSynp, &DestexheSynData::fixVpost);
    addAP("DxheSynp[#].Vpost", &DxheSynp, &DestexheSynData::Vpost);
    addAP("DxheSynp[#].Plasticity", &DxheSynp, &DestexheSynData::Plasticity);
    addAP("DxheSynp[#].assign[#].active", &DxheSynp, &DestexheSynData::assign, &SynapseAssignment::active);
    addAP("DxheSynp[#].assign[#].PreSynChannel", &DxheSynp, &DestexheSynData::assign, &SynapseAssignment::PreSynChannel);
    addAP("DxheSynp[#].assign[#].PostSynChannel", &DxheSynp, &DestexheSynData::assign, &SynapseAssignment::PostSynChannel);
    addAP("DxheSynp[#].assign[#].OutSynChannel", &DxheSynp, &DestexheSynData::assign, &SynapseAssignment::OutSynChannel);
    addAP("DxheSynp[#].assign[#].delay", &DxheSynp, &DestexheSynData::assign, &SynapseAssignment::delay);

    addAP("DxheSynp[#].PreSynChannel", &DxheSynp, &DestexheSynData::legacy_PreSyn);
    addAP("DxheSynp[#].PostSynChannel", &DxheSynp, &DestexheSynData::legacy_PostSyn);
    addAP("DxheSynp[#].OutSynChannel", &DxheSynp, &DestexheSynData::legacy_OutSyn);


    // ESynp
    addAP("ESynp[#].active", &ESynp, &GJunctData::active);
    addAP("ESynp[#].type", &ESynp, &GJunctData::type);
    addAP("ESynp[#].gSyn", &ESynp, &GJunctData::gSyn);
    addAP("ESynp[#].assign[#].active", &ESynp, &GJunctData::assign, &GapJunctionAssignment::active);
    addAP("ESynp[#].assign[#].preInChannel", &ESynp, &GJunctData::assign, &GapJunctionAssignment::preInChannel);
    addAP("ESynp[#].assign[#].postInChannel", &ESynp, &GJunctData::assign, &GapJunctionAssignment::postInChannel);
    addAP("ESynp[#].assign[#].preOutChannel", &ESynp, &GJunctData::assign, &GapJunctionAssignment::preOutChannel);
    addAP("ESynp[#].assign[#].postOutChannel", &ESynp, &GJunctData::assign, &GapJunctionAssignment::postOutChannel);

    addAP("ESynp[#].preInChannel", &ESynp, &GJunctData::legacy_PreIn);
    addAP("ESynp[#].postInChannel", &ESynp, &GJunctData::legacy_PostIn);
    addAP("ESynp[#].preOutChannel", &ESynp, &GJunctData::legacy_PreOut);
    addAP("ESynp[#].postOutChannel", &ESynp, &GJunctData::legacy_PostOut);


    // mhHH
    addAP("mhHHp[#].active", &mhHHp, &mhHHData::active);
    addAP("mhHHp[#].LUTables", &mhHHp, &mhHHData::LUTables);
    addAP("mhHHp[#].gMax", &mhHHp, &mhHHData::gMax);
    addAP("mhHHp[#].Vrev", &mhHHp, &mhHHData::Vrev);
    addAP("mhHHp[#].mExpo", &mhHHp, &mhHHData::mExpo);
    addAP("mhHHp[#].hExpo", &mhHHp, &mhHHData::hExpo);
    addAP("mhHHp[#].Vm", &mhHHp, &mhHHData::Vm);
    addAP("mhHHp[#].sm", &mhHHp, &mhHHData::sm);
    addAP("mhHHp[#].Cm", &mhHHp, &mhHHData::Cm);
    addAP("mhHHp[#].taumType", &mhHHp, &mhHHData::taumType);
    addAP("mhHHp[#].taum", &mhHHp, &mhHHData::taum);
    addAP("mhHHp[#].taumAmpl", &mhHHp, &mhHHData::taumAmpl);
    addAP("mhHHp[#].Vtaum", &mhHHp, &mhHHData::Vtaum);
    addAP("mhHHp[#].staum", &mhHHp, &mhHHData::staum);
    addAP("mhHHp[#].Vh", &mhHHp, &mhHHData::Vh);
    addAP("mhHHp[#].sh", &mhHHp, &mhHHData::sh);
    addAP("mhHHp[#].Ch", &mhHHp, &mhHHData::Ch);
    addAP("mhHHp[#].tauhType", &mhHHp, &mhHHData::tauhType);
    addAP("mhHHp[#].tauh", &mhHHp, &mhHHData::tauh);
    addAP("mhHHp[#].tauhAmpl", &mhHHp, &mhHHData::tauhAmpl);
    addAP("mhHHp[#].Vtauh", &mhHHp, &mhHHData::Vtauh);
    addAP("mhHHp[#].stauh", &mhHHp, &mhHHData::stauh);
    addAP("mhHHp[#].assign[#].active", &mhHHp, &mhHHData::assign, &CurrentAssignment::active);
    addAP("mhHHp[#].assign[#].VChannel", &mhHHp, &mhHHData::assign, &CurrentAssignment::VChannel);
    addAP("mhHHp[#].assign[#].IChannel", &mhHHp, &mhHHData::assign, &CurrentAssignment::IChannel);

    addAP("mhHHp[#].VChannel", &mhHHp, &mhHHData::legacy_V);
    addAP("mhHHp[#].IChannel", &mhHHp, &mhHHData::legacy_I);


    // abHH
    addAP("abHHp[#].active", &abHHp, &abHHData::active);
    addAP("abHHp[#].LUTables", &abHHp, &abHHData::LUTables);
    addAP("abHHp[#].gMax", &abHHp, &abHHData::gMax);
    addAP("abHHp[#].Vrev", &abHHp, &abHHData::Vrev);
    addAP("abHHp[#].mExpo", &abHHp, &abHHData::mExpo);
    addAP("abHHp[#].hExpo", &abHHp, &abHHData::hExpo);
    addAP("abHHp[#].maFunc", &abHHp, &abHHData::maFunc);
    addAP("abHHp[#].mka", &abHHp, &abHHData::mka);
    addAP("abHHp[#].mVa", &abHHp, &abHHData::mVa);
    addAP("abHHp[#].msa", &abHHp, &abHHData::msa);
    addAP("abHHp[#].mbFunc", &abHHp, &abHHData::mbFunc);
    addAP("abHHp[#].mkb", &abHHp, &abHHData::mkb);
    addAP("abHHp[#].mVb", &abHHp, &abHHData::mVb);
    addAP("abHHp[#].msb", &abHHp, &abHHData::msb);
    addAP("abHHp[#].haFunc", &abHHp, &abHHData::haFunc);
    addAP("abHHp[#].hka", &abHHp, &abHHData::hka);
    addAP("abHHp[#].hVa", &abHHp, &abHHData::hVa);
    addAP("abHHp[#].hsa", &abHHp, &abHHData::hsa);
    addAP("abHHp[#].hbFunc", &abHHp, &abHHData::hbFunc);
    addAP("abHHp[#].hkb", &abHHp, &abHHData::hkb);
    addAP("abHHp[#].hVb", &abHHp, &abHHData::hVb);
    addAP("abHHp[#].hsb", &abHHp, &abHHData::hsb);
    addAP("abHHp[#].assign[#].active", &abHHp, &abHHData::assign, &CurrentAssignment::active);
    addAP("abHHp[#].assign[#].VChannel", &abHHp, &abHHData::assign, &CurrentAssignment::VChannel);
    addAP("abHHp[#].assign[#].IChannel", &abHHp, &abHHData::assign, &CurrentAssignment::IChannel);

    addAP("abHHp[#].VChannel", &abHHp, &abHHData::legacy_V);
    addAP("abHHp[#].IChannel", &abHHp, &abHHData::legacy_I);


    // Data saving
    addAP("dataSavingPs.enabled", &(dataSavingPs.enabled));
    addAP("dataSavingPs.fileName", &(dataSavingPs.fileName));
    addAP("dataSavingPs.savingFreq", &(dataSavingPs.savingFreq));
    addAP("dataSavingPs.isBinary", &(dataSavingPs.isBinary));


    // Graphs
    addAP("Plotp.bufferExp", &Plotp, &PlotData::bufferExp);
    addAP("Plotp.interval", &Plotp, &PlotData::interval);
    addAP("Plotp.graphs[#].active", &Plotp, &PlotData::graphs, &GraphData::active);
    AP *graphCol = addAP("Plotp.graphs[#].color", &Plotp, &PlotData::graphs, &GraphData::color);
    AP *graphVol = addAP("Plotp.graphs[#].isVoltage", &Plotp, &PlotData::graphs, &GraphData::isVoltage);
    AP *graphChn = addAP("Plotp.graphs[#].chan", &Plotp, &PlotData::graphs, &GraphData::chan);

    addDeprecatedAP("Graphp[#].chan", graphChn);
    addDeprecatedAP("Graphp[#].isVoltage", graphVol);
    addDeprecatedAP("Graphp[#].color", graphCol);


    // Sample & hold
    addAP("SampleHoldp.active", &(SampleHoldp.active));
    addAP("SampleHoldp.threshV", &(SampleHoldp.threshV));
    addAP("SampleHoldp.trigChn", &(SampleHoldp.trigChn));

}

std::vector<std::unique_ptr<AP>> deprecateChannelsTo(QString prefix)
{
    int skips = prefix.length() - QString(prefix).replace("[#]", "[]").length();
    QStringList names({
        "inChnp[#].active",
        "inChnp[#].gain",
        "inChnp[#].gainFac",
        "inChnp[#].spkDetect",
        "inChnp[#].spkDetectThresh",
        "inChnp[#].bias",
        "inChnp[#].chnlSaving",
        "outChnp[#].active",
        "outChnp[#].gain",
        "outChnp[#].gainFac",
        "outChnp[#].bias",
        "outChnp[#].chnlSaving"
    });
    std::vector<std::unique_ptr<AP>> ret;
    ret.reserve(names.size());
    AP *target;
    for ( QString name : names ) {
        if ( (target = AP::find(prefix + "." + QString(name).replace("Chnp[#]", "Chn[#]"))) )
            ret.push_back(std::unique_ptr<AP>(new APDeprec(name, target, skips)));
    }
    return ret;
}

#include "SimulDAQ.h"
#include "DigiData.h"
#ifdef NATIONAL_INSTRUMENTS
#include "Nidaq.h"
#endif
bool readProtocol(std::istream &is, std::function<bool(QString)> *callback)
{
    QString name, header;
    int version = 0;
    int pos = is.tellg();
    std::vector<std::unique_ptr<AP>> deprec;
    SDAQData sdaq;
    DigiDataData dd;
#ifdef NATIONAL_INSTRUMENTS
    NIDAQData nidaq;
#endif

    if ( is.good() ) {
        is >> header;
        if ( is.good() && !header.isEmpty() ) {
            if ( header == STDPC_PROTOCOL_HEADER )
                is >> version;
            else
                is.seekg(pos);
        } else {
            return false;
        }
    } else {
        return false;
    }

    // Unversioned legacy config
    if ( !version ) {
        int selection, sdaqInChnNo, sdaqOutChnNo;
        is >> selection;
        is >> sdaq.inFileName >> sdaq.outFileName >> sdaqInChnNo >> sdaqOutChnNo >> sdaq.inTFac >> sdaq.outDt;
        sdaq.inChn.resize(sdaqInChnNo);
        sdaq.outChn.resize(sdaqOutChnNo);
        is >> dd.baseAddress;
        if ( selection == 0 ) {
            sdaq.active = true;
            deprec = deprecateChannelsTo("SDAQp[#]");
        } else if ( selection == 1 ) {
            dd.active = true;
            deprec = deprecateChannelsTo("DigiDatap[#]");
        }
#ifdef NATIONAL_INSTRUMENTS
        if ( selection == 2 ) {
            is >> nidaq.deviceName;
            nidaq.active = true;
            deprec = deprecateChannelsTo("NIDAQp[#]");
        }
#endif
        if ( !is.good() )
            return false;

        AP *syncio = AP::find("DigiDatap[#].syncIOMask");
        if ( syncio )
            deprec.push_back(std::unique_ptr<AP>(new APDeprec("DigiDatap.syncIOMask", syncio, 1)));

        SimulDAQProxy::p.insert(SimulDAQProxy::p.begin(), sdaq);
        if ( sdaq.active )
            LEGACY_DAQ_CLASS = "SimulDAQ";
        DigiDataProxy::p.insert(DigiDataProxy::p.begin(), dd);
        if ( dd.active )
            LEGACY_DAQ_CLASS = "DigiData1200";
#ifdef NATIONAL_INSTRUMENTS
        if ( nidaq.active ) {
            NIDAQProxy::p.insert(NIDAQProxy::p.begin(), nidaq);
            LEGACY_DAQ_CLASS = "NIDAQ";
        }
#endif
        if ( !is.good() )
            return false;
    }

    // before version 2: 4 AEC channels only
    std::vector<elecCalibParams> elecCalibPs;
    std::vector<std::unique_ptr<AP>> elecAP;
    if ( version < 2 ) {
        QVector<QString> names(17);
        deprec.reserve(deprec.size() + 17);
        elecAP.reserve(17);
        int i = 0;
        addAP(elecAP, names[i++] = "elecCalibPs[#].copyChnOn", &elecCalibPs, &elecCalibParams::copyChnOn);
        addAP(elecAP, names[i++] = "elecCalibPs[#].copyChn", &elecCalibPs, &elecCalibParams::copyChn);
        addAP(elecAP, names[i++] = "elecCalibPs[#].samplingRate", &elecCalibPs, &elecCalibParams::samplingRate);
        addAP(elecAP, names[i++] = "elecCalibPs[#].inputChannelNumber", &elecCalibPs, &elecCalibParams::legacy_in);
        addAP(elecAP, names[i++] = "elecCalibPs[#].outputChannelNumber", &elecCalibPs, &elecCalibParams::legacy_out);
        addAP(elecAP, names[i++] = "elecCalibPs[#].iMaxElec", &elecCalibPs, &elecCalibParams::iMaxElec);
        addAP(elecAP, names[i++] = "elecCalibPs[#].iMinElec", &elecCalibPs, &elecCalibParams::iMinElec);
        addAP(elecAP, names[i++] = "elecCalibPs[#].numberOfLevels", &elecCalibPs, &elecCalibParams::numberOfLevels);
        addAP(elecAP, names[i++] = "elecCalibPs[#].injLenPerLevel", &elecCalibPs, &elecCalibParams::injLenPerLevel);
        addAP(elecAP, names[i++] = "elecCalibPs[#].iMembStep", &elecCalibPs, &elecCalibParams::iMembStep);
        addAP(elecAP, names[i++] = "elecCalibPs[#].numberOfRepeats", &elecCalibPs, &elecCalibParams::numberOfRepeats);
        addAP(elecAP, names[i++] = "elecCalibPs[#].injLenPerRepeat", &elecCalibPs, &elecCalibParams::injLenPerRepeat);
        addAP(elecAP, names[i++] = "elecCalibPs[#].hyperpolCurr", &elecCalibPs, &elecCalibParams::hyperpolCurr);
        addAP(elecAP, names[i++] = "elecCalibPs[#].injCalAmp", &elecCalibPs, &elecCalibParams::injCalAmp);
        addAP(elecAP, names[i++] = "elecCalibPs[#].injCalLen", &elecCalibPs, &elecCalibParams::injCalLen);
        addAP(elecAP, names[i++] = "elecCalibPs[#].fullKernelLen", &elecCalibPs, &elecCalibParams::fullKernelLen);
        addAP(elecAP, names[i++] = "elecCalibPs[#].electrodeKernelLen", &elecCalibPs, &elecCalibParams::electrodeKernelLen);
        for ( i = 0; i < names.size(); i++ ) {
            deprec.push_back(std::unique_ptr<AP>(new APDeprec(names[i], elecAP.at(i).get())));
        }
    }

    LOADED_PROTOCOL_VERSION = version;

    AP *it;
    is >> name;
    while ( is.good() ) {
        bool ok = false;
        if ( (it = AP::find(name)) ) {
            it->readNow(name, is, &ok);
        } else if ( version < STDPC_PROTOCOL_VERSION ) {
            if ( (it = AP::find(name, &deprec)) )
                it->readNow(name, is, &ok);
        }
        if ( !ok && callback )
            if ( (*callback)(name) )
                return false;
        is >> name;
    }

    if ( version == 0 ) {
        // Set up legacy index to new index conversion tables
        std::vector<ChannelIndex> legacyIn, legacyOut;
        DAQProxy *legacyDAQ = Devices.Register()[LEGACY_DAQ_CLASS];
        for ( size_t i = 0; i < legacyDAQ->param(0).inChn.size(); i++ ) {
            if ( legacyDAQ->param(0).inChn[i].active ) {
                legacyIn.push_back(ChannelIndex(legacyDAQ, 0, i, true));
            }
        }
        legacyIn.push_back(ChannelIndex(Models.Register()["SG"], 0, 0));
        for ( size_t i = 0; i < legacyDAQ->param(0).outChn.size(); i++ ) {
            if ( legacyDAQ->param(0).outChn[i].active ) {
                legacyOut.push_back(ChannelIndex(legacyDAQ, 0, i, false));
            }
        }

        for ( CSynData &syn : CSynp ) {
            SynapseAssignment assign;
            if ( syn.legacy_PreSyn >= 0 && syn.legacy_PreSyn < int(legacyIn.size()) )
                assign.PreSynChannel = legacyIn[syn.legacy_PreSyn];
            if ( syn.legacy_PostSyn >= 0 && syn.legacy_PostSyn < int(legacyIn.size()) )
                assign.PostSynChannel = legacyIn[syn.legacy_PostSyn];
            if ( syn.legacy_OutSyn >= 0 && syn.legacy_OutSyn < int(legacyOut.size()) )
                assign.OutSynChannel = legacyOut[syn.legacy_OutSyn];
            syn.assign.push_back(assign);
        }

        for ( abSynData &syn : abSynp ) {
            SynapseAssignment assign;
            if ( syn.legacy_PreSyn >= 0 && syn.legacy_PreSyn < int(legacyIn.size()) )
                assign.PreSynChannel = legacyIn[syn.legacy_PreSyn];
            if ( syn.legacy_PostSyn >= 0 && syn.legacy_PostSyn < int(legacyIn.size()) )
                assign.PostSynChannel = legacyIn[syn.legacy_PostSyn];
            if ( syn.legacy_OutSyn >= 0 && syn.legacy_OutSyn < int(legacyOut.size()) )
                assign.OutSynChannel = legacyOut[syn.legacy_OutSyn];
            syn.assign.push_back(assign);
        }

        for ( DestexheSynData &syn : DxheSynp ) {
            SynapseAssignment assign;
            if ( syn.legacy_PreSyn >= 0 && syn.legacy_PreSyn < int(legacyIn.size()) )
                assign.PreSynChannel = legacyIn[syn.legacy_PreSyn];
            if ( syn.legacy_PostSyn >= 0 && syn.legacy_PostSyn < int(legacyIn.size()) )
                assign.PostSynChannel = legacyIn[syn.legacy_PostSyn];
            if ( syn.legacy_OutSyn >= 0 && syn.legacy_OutSyn < int(legacyOut.size()) )
                assign.OutSynChannel = legacyOut[syn.legacy_OutSyn];
            syn.assign.push_back(assign);
        }

        for ( GJunctData &syn : ESynp ) {
            GapJunctionAssignment assign;
            if ( syn.legacy_PreIn >= 0 && syn.legacy_PreIn < int(legacyIn.size()) )
                assign.preInChannel = legacyIn[syn.legacy_PreIn];
            if ( syn.legacy_PostIn >= 0 && syn.legacy_PostIn < int(legacyIn.size()) )
                assign.postInChannel = legacyIn[syn.legacy_PostIn];
            if ( syn.legacy_PreOut >= 0 && syn.legacy_PreOut < int(legacyOut.size()) )
                assign.preOutChannel = legacyOut[syn.legacy_PreOut];
            if ( syn.legacy_PostOut >= 0 && syn.legacy_PostOut < int(legacyOut.size()) )
                assign.postOutChannel = legacyOut[syn.legacy_PostOut];
            syn.assign.push_back(assign);
        }

        for ( mhHHData &current : mhHHp ) {
            CurrentAssignment assign;
            if ( current.legacy_V >= 0 && current.legacy_V < int(legacyIn.size()) )
                assign.VChannel = legacyIn[current.legacy_V];
            if ( current.legacy_I >= 0 && current.legacy_I < int(legacyOut.size()) )
                assign.IChannel = legacyOut[current.legacy_I];
            current.assign.push_back(assign);
        }

        for ( abHHData &current : abHHp ) {
            CurrentAssignment assign;
            if ( current.legacy_V >= 0 && current.legacy_V < int(legacyIn.size()) )
                assign.VChannel = legacyIn[current.legacy_V];
            if ( current.legacy_I >= 0 && current.legacy_I < int(legacyOut.size()) )
                assign.IChannel = legacyOut[current.legacy_I];
            current.assign.push_back(assign);
        }

        for ( int i(elecCalibPs.size() - 1); i >= 0; i-- ) {
            if ( elecCalibPs[i].legacy_in >= 0 && elecCalibPs[i].legacy_in < int(legacyIn.size()) ) {
                inChnData *in = legacyIn[elecCalibPs[i].legacy_in].getInChnData();
                if ( !in )
                    continue;
                in->calib = elecCalibPs[i];
                if ( in->calib.legacy_out >= 0 && in->calib.legacy_out < int(legacyOut.size()) )
                    in->calib.outputChannelNumber = legacyOut[in->calib.legacy_out];
            }
        }

        if ( SampleHoldp.trigChn.isLegacy && SampleHoldp.trigChn.chanID < legacyIn.size() )
            SampleHoldp.trigChn = legacyIn[SampleHoldp.trigChn.chanID];
    }

    return true;
}

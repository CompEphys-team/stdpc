#include "AP.h"
#include "Global.h"

std::vector<std::unique_ptr<AP>> params;

void initAP()
{
    params.clear();

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
    AP *CSyn_pre = addAP("CSynp[#].assign[#].PreSynChannel", &CSynp, &CSynData::assign, &SynapseAssignment::PreSynChannel);
    AP *CSyn_pst = addAP("CSynp[#].assign[#].PostSynChannel", &CSynp, &CSynData::assign, &SynapseAssignment::PostSynChannel);
    AP *CSyn_out = addAP("CSynp[#].assign[#].OutSynChannel", &CSynp, &CSynData::assign, &SynapseAssignment::OutSynChannel);

    addDeprecatedAP("CSynp[#].PreSynChannel", CSyn_pre);
    addDeprecatedAP("CSynp[#].PostSynChannel", CSyn_pst);
    addDeprecatedAP("CSynp[#].OutSynChannel", CSyn_out);


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
    AP *abSyn_pre = addAP("abSynp[#].assign[#].PreSynChannel", &abSynp, &abSynData::assign, &SynapseAssignment::PreSynChannel);
    AP *abSyn_pst = addAP("abSynp[#].assign[#].PostSynChannel", &abSynp, &abSynData::assign, &SynapseAssignment::PostSynChannel);
    AP *abSyn_out = addAP("abSynp[#].assign[#].OutSynChannel", &abSynp, &abSynData::assign, &SynapseAssignment::OutSynChannel);

    addDeprecatedAP("abSynp[#].PreSynChannel", abSyn_pre);
    addDeprecatedAP("abSynp[#].PostSynChannel", abSyn_pst);
    addDeprecatedAP("abSynp[#].OutSynChannel", abSyn_out);


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
    AP *DxSyn_pre = addAP("DxheSynp[#].assign[#].PreSynChannel", &DxheSynp, &DestexheSynData::assign, &SynapseAssignment::PreSynChannel);
    AP *DxSyn_pst = addAP("DxheSynp[#].assign[#].PostSynChannel", &DxheSynp, &DestexheSynData::assign, &SynapseAssignment::PostSynChannel);
    AP *DxSyn_out = addAP("DxheSynp[#].assign[#].OutSynChannel", &DxheSynp, &DestexheSynData::assign, &SynapseAssignment::OutSynChannel);

    addDeprecatedAP("DxheSynp[#].PreSynChannel", DxSyn_pre);
    addDeprecatedAP("DxheSynp[#].PostSynChannel", DxSyn_pst);
    addDeprecatedAP("DxheSynp[#].OutSynChannel", DxSyn_out);


    // ESynp
    addAP("ESynp[#].active", &ESynp, &GJunctData::active);
    addAP("ESynp[#].type", &ESynp, &GJunctData::type);
    addAP("ESynp[#].gSyn", &ESynp, &GJunctData::gSyn);
    addAP("ESynp[#].assign[#].active", &ESynp, &GJunctData::assign, &GapJunctionAssignment::active);
    AP *ESyn_prein = addAP("ESynp[#].assign[#].preInChannel", &ESynp, &GJunctData::assign, &GapJunctionAssignment::preInChannel);
    AP *ESyn_pstin = addAP("ESynp[#].assign[#].postInChannel", &ESynp, &GJunctData::assign, &GapJunctionAssignment::postInChannel);
    AP *ESyn_preut = addAP("ESynp[#].assign[#].preOutChannel", &ESynp, &GJunctData::assign, &GapJunctionAssignment::preOutChannel);
    AP *ESyn_pstut = addAP("ESynp[#].assign[#].postOutChannel", &ESynp, &GJunctData::assign, &GapJunctionAssignment::postOutChannel);

    addDeprecatedAP("ESynp[#].preInChannel", ESyn_prein);
    addDeprecatedAP("ESynp[#].postInChannel", ESyn_pstin);
    addDeprecatedAP("ESynp[#].preOutChannel", ESyn_preut);
    addDeprecatedAP("ESynp[#].postOutChannel", ESyn_pstut);


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
    AP *mhHH_V = addAP("mhHHp[#].assign[#].VChannel", &mhHHp, &mhHHData::assign, &CurrentAssignment::VChannel);
    AP *mhHH_I = addAP("mhHHp[#].assign[#].IChannel", &mhHHp, &mhHHData::assign, &CurrentAssignment::IChannel);

    addDeprecatedAP("mhHHp[#].VChannel", mhHH_V);
    addDeprecatedAP("mhHHp[#].IChannel", mhHH_I);


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
    AP *abHH_V = addAP("abHHp[#].assign[#].VChannel", &abHHp, &abHHData::assign, &CurrentAssignment::VChannel);
    AP *abHH_I = addAP("abHHp[#].assign[#].IChannel", &abHHp, &abHHData::assign, &CurrentAssignment::IChannel);

    addDeprecatedAP("abHHp[#].VChannel", abHH_V);
    addDeprecatedAP("abHHp[#].IChannel", abHH_I);


    // SG
    addAP("SGp.active", &(SGp.active));
    addAP("SGp.method", &(SGp.method));
    addAP("SGp.saving", &(SGp.saving));
    addAP("SGp.LUTables", &(SGp.LUTables));
    addAP("SGp.VSpike", &(SGp.VSpike));
    addAP("SGp.spkTimeScaling", &(SGp.spkTimeScaling));
    addAP("SGp.VRest", &(SGp.VRest));
    addAP("SGp.bdType", &(SGp.bdType));
    addAP("SGp.bdChannel", &(SGp.bdChannel));
    addAP("SGp.bdThresh", &(SGp.bdThresh));
    addAP("SGp.bdNUnder", &(SGp.bdNUnder));
    addAP("SGp.bdNOver", &(SGp.bdNOver));
    addAP("SGp.period", &(SGp.period));
    addAP("SGp.SpikeNo", &(SGp.SpikeNo));
    addAP("SGp.STInFName", &(SGp.STInFName));
    addAP("SGp.SpikeT[#]", &(SGp.SpikeT));


    // SDAQp
    addAP("SDAQp[#].active", &SDAQp, &SDAQData::active);
    addAP("SDAQp[#].inFileName", &SDAQp, &SDAQData::inFileName);
    addAP("SDAQp[#].outFileName", &SDAQp, &SDAQData::outFileName);
    addAP("SDAQp[#].inTFac", &SDAQp, &SDAQData::inTFac);
    addAP("SDAQp[#].outDt", &SDAQp, &SDAQData::outDt);
    addAP("SDAQp[#].inChn[#].active", &SDAQp, &SDAQData::inChn, &inChnData::active);
    addAP("SDAQp[#].inChn[#].gain", &SDAQp, &SDAQData::inChn, &inChnData::gain);
    addAP("SDAQp[#].inChn[#].gainFac", &SDAQp, &SDAQData::inChn, &inChnData::gainFac);
    addAP("SDAQp[#].inChn[#].spkDetect", &SDAQp, &SDAQData::inChn, &inChnData::spkDetect);
    addAP("SDAQp[#].inChn[#].spkDetectThresh", &SDAQp, &SDAQData::inChn, &inChnData::spkDetectThresh);
    addAP("SDAQp[#].inChn[#].bias", &SDAQp, &SDAQData::inChn, &inChnData::bias);
    addAP("SDAQp[#].inChn[#].chnlSaving", &SDAQp, &SDAQData::inChn, &inChnData::chnlSaving);
    addAP("SDAQp[#].outChn[#].active", &SDAQp, &SDAQData::outChn, &outChnData::active);
    addAP("SDAQp[#].outChn[#].gain", &SDAQp, &SDAQData::outChn, &outChnData::gain);
    addAP("SDAQp[#].outChn[#].gainFac", &SDAQp, &SDAQData::outChn, &outChnData::gainFac);
    addAP("SDAQp[#].outChn[#].bias", &SDAQp, &SDAQData::outChn, &outChnData::bias);
    addAP("SDAQp[#].outChn[#].chnlSaving", &SDAQp, &SDAQData::outChn, &outChnData::chnlSaving);
    // Ignore unused inChnData::calib


    // DigiDatap
    addAP("DigiDatap[#].active", &DigiDatap, &DigiDataData::active);
    addAP("DigiDatap[#].baseAddress", &DigiDatap, &DigiDataData::baseAddress);
    addAP("DigiDatap[#].syncIOMask", &DigiDatap, &DigiDataData::syncIOMask);
    addAP("DigiDatap[#].inChn[#].active", &DigiDatap, &DigiDataData::inChn, &inChnData::active);
    addAP("DigiDatap[#].inChn[#].gain", &DigiDatap, &DigiDataData::inChn, &inChnData::gain);
    addAP("DigiDatap[#].inChn[#].gainFac", &DigiDatap, &DigiDataData::inChn, &inChnData::gainFac);
    addAP("DigiDatap[#].inChn[#].spkDetect", &DigiDatap, &DigiDataData::inChn, &inChnData::spkDetect);
    addAP("DigiDatap[#].inChn[#].spkDetectThresh", &DigiDatap, &DigiDataData::inChn, &inChnData::spkDetectThresh);
    addAP("DigiDatap[#].inChn[#].bias", &DigiDatap, &DigiDataData::inChn, &inChnData::bias);
    addAP("DigiDatap[#].inChn[#].chnlSaving", &DigiDatap, &DigiDataData::inChn, &inChnData::chnlSaving);
    addAP("DigiDatap[#].outChn[#].active", &DigiDatap, &DigiDataData::outChn, &outChnData::active);
    addAP("DigiDatap[#].outChn[#].gain", &DigiDatap, &DigiDataData::outChn, &outChnData::gain);
    addAP("DigiDatap[#].outChn[#].gainFac", &DigiDatap, &DigiDataData::outChn, &outChnData::gainFac);
    addAP("DigiDatap[#].outChn[#].bias", &DigiDatap, &DigiDataData::outChn, &outChnData::bias);
    addAP("DigiDatap[#].outChn[#].chnlSaving", &DigiDatap, &DigiDataData::outChn, &outChnData::chnlSaving);

    addAP("DigiDatap[#].inChn[#].calib.copyChnOn", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::copyChnOn);
    addAP("DigiDatap[#].inChn[#].calib.copyChn", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::copyChn);
    addAP("DigiDatap[#].inChn[#].calib.samplingRate", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::samplingRate);
    addAP("DigiDatap[#].inChn[#].calib.outputChannelNumber", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::outputChannelNumber);
    addAP("DigiDatap[#].inChn[#].calib.iMaxElec", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::iMaxElec);
    addAP("DigiDatap[#].inChn[#].calib.iMinElec", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::iMinElec);
    addAP("DigiDatap[#].inChn[#].calib.numberOfLevels", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::numberOfLevels);
    addAP("DigiDatap[#].inChn[#].calib.injLenPerLevel", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::injLenPerLevel);
    addAP("DigiDatap[#].inChn[#].calib.iMembStep", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::iMembStep);
    addAP("DigiDatap[#].inChn[#].calib.numberOfRepeats", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::numberOfRepeats);
    addAP("DigiDatap[#].inChn[#].calib.injLenPerRepeat", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::injLenPerRepeat);
    addAP("DigiDatap[#].inChn[#].calib.hyperpolCurr", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::hyperpolCurr);
    addAP("DigiDatap[#].inChn[#].calib.injCalAmp", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::injCalAmp);
    addAP("DigiDatap[#].inChn[#].calib.injCalLen", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::injCalLen);
    addAP("DigiDatap[#].inChn[#].calib.fullKernelLen", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::fullKernelLen);
    addAP("DigiDatap[#].inChn[#].calib.electrodeKernelLen", &DigiDatap, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::electrodeKernelLen);


#ifdef NATIONAL_INSTRUMENTS
    // NIDAQp
    addAP("NIDAQp[#].active", &NIDAQp, &NIDAQData::active);
    addAP("NIDAQp[#].deviceName", &NIDAQp, &NIDAQData::deviceName);
    addAP("NIDAQp[#].inChn[#].active", &NIDAQp, &NIDAQData::inChn, &inChnData::active);
    addAP("NIDAQp[#].inChn[#].gain", &NIDAQp, &NIDAQData::inChn, &inChnData::gain);
    addAP("NIDAQp[#].inChn[#].gainFac", &NIDAQp, &NIDAQData::inChn, &inChnData::gainFac);
    addAP("NIDAQp[#].inChn[#].spkDetect", &NIDAQp, &NIDAQData::inChn, &inChnData::spkDetect);
    addAP("NIDAQp[#].inChn[#].spkDetectThresh", &NIDAQp, &NIDAQData::inChn, &inChnData::spkDetectThresh);
    addAP("NIDAQp[#].inChn[#].bias", &NIDAQp, &NIDAQData::inChn, &inChnData::bias);
    addAP("NIDAQp[#].inChn[#].chnlSaving", &NIDAQp, &NIDAQData::inChn, &inChnData::chnlSaving);
    addAP("NIDAQp[#].outChn[#].active", &NIDAQp, &NIDAQData::outChn, &outChnData::active);
    addAP("NIDAQp[#].outChn[#].gain", &NIDAQp, &NIDAQData::outChn, &outChnData::gain);
    addAP("NIDAQp[#].outChn[#].gainFac", &NIDAQp, &NIDAQData::outChn, &outChnData::gainFac);
    addAP("NIDAQp[#].outChn[#].bias", &NIDAQp, &NIDAQData::outChn, &outChnData::bias);
    addAP("NIDAQp[#].outChn[#].chnlSaving", &NIDAQp, &NIDAQData::outChn, &outChnData::chnlSaving);

    addAP("NIDAQp[#].inChn[#].calib.copyChnOn", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::copyChnOn);
    addAP("NIDAQp[#].inChn[#].calib.copyChn", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::copyChn);
    addAP("NIDAQp[#].inChn[#].calib.samplingRate", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::samplingRate);
    addAP("NIDAQp[#].inChn[#].calib.outputChannelNumber", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::outputChannelNumber);
    addAP("NIDAQp[#].inChn[#].calib.iMaxElec", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::iMaxElec);
    addAP("NIDAQp[#].inChn[#].calib.iMinElec", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::iMinElec);
    addAP("NIDAQp[#].inChn[#].calib.numberOfLevels", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::numberOfLevels);
    addAP("NIDAQp[#].inChn[#].calib.injLenPerLevel", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::injLenPerLevel);
    addAP("NIDAQp[#].inChn[#].calib.iMembStep", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::iMembStep);
    addAP("NIDAQp[#].inChn[#].calib.numberOfRepeats", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::numberOfRepeats);
    addAP("NIDAQp[#].inChn[#].calib.injLenPerRepeat", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::injLenPerRepeat);
    addAP("NIDAQp[#].inChn[#].calib.hyperpolCurr", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::hyperpolCurr);
    addAP("NIDAQp[#].inChn[#].calib.injCalAmp", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::injCalAmp);
    addAP("NIDAQp[#].inChn[#].calib.injCalLen", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::injCalLen);
    addAP("NIDAQp[#].inChn[#].calib.fullKernelLen", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::fullKernelLen);
    addAP("NIDAQp[#].inChn[#].calib.electrodeKernelLen", &NIDAQp, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::electrodeKernelLen);
#endif


    // HHNeuronp
    addAP("HHNeuronp[#].active", &HHNeuronp, &HHNeuronData::active);
    addAP("HHNeuronp[#].C", &HHNeuronp, &HHNeuronData::C);
    addAP("HHNeuronp[#].gLeak", &HHNeuronp, &HHNeuronData::gLeak);
    addAP("HHNeuronp[#].ELeak", &HHNeuronp, &HHNeuronData::ELeak);
    addAP("HHNeuronp[#].inst[#].active", &HHNeuronp, &HHNeuronData::inst, &vInstData::active);
    addAP("HHNeuronp[#].inst[#].inChn.spkDetect", &HHNeuronp, &HHNeuronData::inst, &vInstData::inChn, &inChnData::spkDetect);
    addAP("HHNeuronp[#].inst[#].inChn.spkDetectThresh", &HHNeuronp, &HHNeuronData::inst, &vInstData::inChn, &inChnData::spkDetectThresh);
    addAP("HHNeuronp[#].inst[#].inChn.bias", &HHNeuronp, &HHNeuronData::inst, &vInstData::inChn, &inChnData::bias);
    addAP("HHNeuronp[#].inst[#].inChn.chnlSaving", &HHNeuronp, &HHNeuronData::inst, &vInstData::inChn, &inChnData::chnlSaving);
    addAP("HHNeuronp[#].inst[#].outChn.bias", &HHNeuronp, &HHNeuronData::inst, &vInstData::outChn, &outChnData::bias);
    addAP("HHNeuronp[#].inst[#].outChn.chnlSaving", &HHNeuronp, &HHNeuronData::inst, &vInstData::outChn, &outChnData::chnlSaving);


    // Data saving
    addAP("dataSavingPs.enabled", &(dataSavingPs.enabled));
    addAP("dataSavingPs.fileName", &(dataSavingPs.fileName));
    addAP("dataSavingPs.savingFreq", &(dataSavingPs.savingFreq));
    addAP("dataSavingPs.isBinary", &(dataSavingPs.isBinary));


    // Electrode compensation
//    addAP("elecCalibPs[#].copyChnOn", &elecCalibPs, &elecCalibParams::copyChnOn);
//    addAP("elecCalibPs[#].copyChn", &elecCalibPs, &elecCalibParams::copyChn);
//    addAP("elecCalibPs[#].samplingRate", &elecCalibPs, &elecCalibParams::samplingRate);
//    addAP("elecCalibPs[#].inputChannelNumber", &elecCalibPs, &elecCalibParams::inputChannelNumber);
//    addAP("elecCalibPs[#].outputChannelNumber", &elecCalibPs, &elecCalibParams::outputChannelNumber);
//    addAP("elecCalibPs[#].iMaxElec", &elecCalibPs, &elecCalibParams::iMaxElec);
//    addAP("elecCalibPs[#].iMinElec", &elecCalibPs, &elecCalibParams::iMinElec);
//    addAP("elecCalibPs[#].numberOfLevels", &elecCalibPs, &elecCalibParams::numberOfLevels);
//    addAP("elecCalibPs[#].injLenPerLevel", &elecCalibPs, &elecCalibParams::injLenPerLevel);
//    addAP("elecCalibPs[#].iMembStep", &elecCalibPs, &elecCalibParams::iMembStep);
//    addAP("elecCalibPs[#].numberOfRepeats", &elecCalibPs, &elecCalibParams::numberOfRepeats);
//    addAP("elecCalibPs[#].injLenPerRepeat", &elecCalibPs, &elecCalibParams::injLenPerRepeat);
//    addAP("elecCalibPs[#].hyperpolCurr", &elecCalibPs, &elecCalibParams::hyperpolCurr);
//    addAP("elecCalibPs[#].injCalAmp", &elecCalibPs, &elecCalibParams::injCalAmp);
//    addAP("elecCalibPs[#].injCalLen", &elecCalibPs, &elecCalibParams::injCalLen);
//    addAP("elecCalibPs[#].fullKernelLen", &elecCalibPs, &elecCalibParams::fullKernelLen);
//    addAP("elecCalibPs[#].electrodeKernelLen", &elecCalibPs, &elecCalibParams::electrodeKernelLen);


    // Graphs
    addAP("Graphp[#].active[#]", &Graphp, &graphData::active);
    addAP("Graphp[#].color[#]", &Graphp, &graphData::color);
    addAP("Graphp[#].chn[#]", &Graphp, &graphData::chn);
    addAP("Graphp[#].miny[#]", &Graphp, &graphData::miny);
    addAP("Graphp[#].maxy[#]", &Graphp, &graphData::maxy);
    addAP("Graphp[#].xrange", &Graphp, &graphData::xrange);
    addAP("Graphp[#].dt", &Graphp, &graphData::dt);
    addAP("Graphp[#].xtNo", &Graphp, &graphData::xtNo);
    addAP("Graphp[#].ytNo", &Graphp, &graphData::ytNo);
    addAP("Graphp[#].yfac[#]", &Graphp, &graphData::yfac);


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

        SDAQp.insert(SDAQp.begin(), sdaq);
        if ( sdaq.active )
            LEGACY_DAQ_CLASS = DAQClass::Simul;
        DigiDatap.insert(DigiDatap.begin(), dd);
        if ( dd.active )
            LEGACY_DAQ_CLASS = DAQClass::DD1200;
#ifdef NATIONAL_INSTRUMENTS
        if ( nidaq.active ) {
            NIDAQp.insert(NIDAQp.begin(), nidaq);
            LEGACY_DAQ_CLASS = DAQClass::NI;
        }
#endif
    }

    if ( !is.good() )
        return false;

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
    return true;
}

std::istream &operator>>(std::istream &is, QString &str)
{
    std::string tmp;
    is >> tmp;
    str = QString::fromStdString(tmp);
    return is;
}

std::ostream &operator<<(std::ostream &os, const QString &str)
{
    os << str.toStdString();
    return os;
}

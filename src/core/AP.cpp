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
    addAP("CSynp[#].PreSynChannel", &CSynp, &CSynData::PreSynChannel);
    addAP("CSynp[#].PostSynChannel", &CSynp, &CSynData::PostSynChannel);
    addAP("CSynp[#].OutSynChannel", &CSynp, &CSynData::OutSynChannel);
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
    addAP("abSynp[#].PreSynChannel", &abSynp, &abSynData::PreSynChannel);
    addAP("abSynp[#].PostSynChannel", &abSynp, &abSynData::PostSynChannel);
    addAP("abSynp[#].OutSynChannel", &abSynp, &abSynData::OutSynChannel);
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
    addAP("DxheSynp[#].PreSynChannel", &DxheSynp, &DestexheSynData::PreSynChannel);
    addAP("DxheSynp[#].PostSynChannel", &DxheSynp, &DestexheSynData::PostSynChannel);
    addAP("DxheSynp[#].OutSynChannel", &DxheSynp, &DestexheSynData::OutSynChannel);
    addAP("DxheSynp[#].gSyn", &DxheSynp, &DestexheSynData::gSyn);
    addAP("DxheSynp[#].Vpre", &DxheSynp, &DestexheSynData::Vpre);
    addAP("DxheSynp[#].Vrev", &DxheSynp, &DestexheSynData::Vrev);
    addAP("DxheSynp[#].trelease", &DxheSynp, &DestexheSynData::trelease);
    addAP("DxheSynp[#].alpha", &DxheSynp, &DestexheSynData::alpha);
    addAP("DxheSynp[#].beta", &DxheSynp, &DestexheSynData::beta);
    addAP("DxheSynp[#].fixVpost", &DxheSynp, &DestexheSynData::fixVpost);
    addAP("DxheSynp[#].Vpost", &DxheSynp, &DestexheSynData::Vpost);
    addAP("DxheSynp[#].Plasticity", &DxheSynp, &DestexheSynData::Plasticity);


    // ESynp
    addAP("ESynp[#].active", &ESynp, &GJunctData::active);
    addAP("ESynp[#].preInChannel", &ESynp, &GJunctData::preInChannel);
    addAP("ESynp[#].postInChannel", &ESynp, &GJunctData::postInChannel);
    addAP("ESynp[#].preOutChannel", &ESynp, &GJunctData::preOutChannel);
    addAP("ESynp[#].postOutChannel", &ESynp, &GJunctData::postOutChannel);
    addAP("ESynp[#].type", &ESynp, &GJunctData::type);
    addAP("ESynp[#].gSyn", &ESynp, &GJunctData::gSyn);


    // mhHH
    addAP("mhHHp[#].active", &mhHHp, &mhHHData::active);
    addAP("mhHHp[#].LUTables", &mhHHp, &mhHHData::LUTables);
    addAP("mhHHp[#].VChannel", &mhHHp, &mhHHData::VChannel);
    addAP("mhHHp[#].IChannel", &mhHHp, &mhHHData::IChannel);
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
    addAP("mhHHp[#].noLegacyAssignment", &mhHHp, &mhHHData::noLegacyAssign);


    // abHH
    addAP("abHHp[#].active", &abHHp, &abHHData::active);
    addAP("abHHp[#].LUTables", &abHHp, &abHHData::LUTables);
    addAP("abHHp[#].VChannel", &abHHp, &abHHData::VChannel);
    addAP("abHHp[#].IChannel", &abHHp, &abHHData::IChannel);
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
    addAP("abHHp[#].noLegacyAssignment", &abHHp, &abHHData::noLegacyAssign);


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


    // inChnp
    addAP("inChnp[#].active", &inChnp, &inChnData::active);
    addAP("inChnp[#].gain", &inChnp, &inChnData::gain);
    addAP("inChnp[#].gainFac", &inChnp, &inChnData::gainFac);
    addAP("inChnp[#].spkDetect", &inChnp, &inChnData::spkDetect);
    addAP("inChnp[#].spkDetectThresh", &inChnp, &inChnData::spkDetectThresh);
    addAP("inChnp[#].bias", &inChnp, &inChnData::bias);
    addAP("inChnp[#].chnlSaving", &inChnp, &inChnData::chnlSaving);


    // outChnp
    addAP("outChnp[#].active", &outChnp, &outChnData::active);
    addAP("outChnp[#].gain", &outChnp, &outChnData::gain);
    addAP("outChnp[#].gainFac", &outChnp, &outChnData::gainFac);
    addAP("outChnp[#].bias", &outChnp, &outChnData::bias);
    addAP("outChnp[#].chnlSaving", &outChnp, &outChnData::chnlSaving);


    // Data saving
    addAP("dataSavingPs.enabled", &(dataSavingPs.enabled));
    addAP("dataSavingPs.fileName", &(dataSavingPs.fileName));
    addAP("dataSavingPs.savingFreq", &(dataSavingPs.savingFreq));
    addAP("dataSavingPs.isBinary", &(dataSavingPs.isBinary));


    // Electrode compensation
    addAP("elecCalibPs[#].copyChnOn", &elecCalibPs, &elecCalibParams::copyChnOn);
    addAP("elecCalibPs[#].copyChn", &elecCalibPs, &elecCalibParams::copyChn);
    addAP("elecCalibPs[#].samplingRate", &elecCalibPs, &elecCalibParams::samplingRate);
    addAP("elecCalibPs[#].inputChannelNumber", &elecCalibPs, &elecCalibParams::inputChannelNumber);
    addAP("elecCalibPs[#].outputChannelNumber", &elecCalibPs, &elecCalibParams::outputChannelNumber);
    addAP("elecCalibPs[#].iMaxElec", &elecCalibPs, &elecCalibParams::iMaxElec);
    addAP("elecCalibPs[#].iMinElec", &elecCalibPs, &elecCalibParams::iMinElec);
    addAP("elecCalibPs[#].numberOfLevels", &elecCalibPs, &elecCalibParams::numberOfLevels);
    addAP("elecCalibPs[#].injLenPerLevel", &elecCalibPs, &elecCalibParams::injLenPerLevel);
    addAP("elecCalibPs[#].iMembStep", &elecCalibPs, &elecCalibParams::iMembStep);
    addAP("elecCalibPs[#].numberOfRepeats", &elecCalibPs, &elecCalibParams::numberOfRepeats);
    addAP("elecCalibPs[#].injLenPerRepeat", &elecCalibPs, &elecCalibParams::injLenPerRepeat);
    addAP("elecCalibPs[#].hyperpolCurr", &elecCalibPs, &elecCalibParams::hyperpolCurr);
    addAP("elecCalibPs[#].injCalAmp", &elecCalibPs, &elecCalibParams::injCalAmp);
    addAP("elecCalibPs[#].injCalLen", &elecCalibPs, &elecCalibParams::injCalLen);
    addAP("elecCalibPs[#].fullKernelLen", &elecCalibPs, &elecCalibParams::fullKernelLen);
    addAP("elecCalibPs[#].electrodeKernelLen", &elecCalibPs, &elecCalibParams::electrodeKernelLen);


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


    // Digidata
    addAP("DigiDatap.syncIOMask", &(DigiDatap.syncIOMask));


    // Sample & hold
    addAP("SampleHoldp.active", &(SampleHoldp.active));
    addAP("SampleHoldp.threshV", &(SampleHoldp.threshV));
    addAP("SampleHoldp.trigChn", &(SampleHoldp.trigChn));

}

QRegularExpressionMatch AP::matchName(QString &rawName)
{
    QString n = QString("^") + QRegularExpression::escape(name) + QString("$");
    n.replace("\\#", "(\\d+)");
    QRegularExpression re(n);
    return re.match(rawName);
}

int AP::getArrayIndex(QRegularExpressionMatch &match, int i, int arraysize)
{
    int idx = getIndex(match, i);
    if ( idx < 0 )
        return idx;
    if ( idx >= arraysize )
        return -1;
    return idx;
}


int AP::getIndex(QRegularExpressionMatch &match, int i)
{
    if ( match.lastCapturedIndex() < i )
        return -1;
    bool ok = true;
    uint idx = match.captured(i).toUInt(&ok);
    if ( !ok || idx > INT_MAX )
        return -1;
    return (int) idx;
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

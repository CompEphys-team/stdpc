#include "AP.h"
#include "Global.h"
#include "ModelManager.h"
#include "Synapse.h"
#include "GapJunction.h"
#include "IonicCurrent.h"

void initAP()
{
    // Data saving
    addAP("dataSavingPs.enabled", &(dataSavingPs.enabled));
    addAP("dataSavingPs.fileName", &(dataSavingPs.fileName));
    addAP("dataSavingPs.savingFreq", &(dataSavingPs.savingFreq));
    addAP("dataSavingPs.isBinary", &(dataSavingPs.isBinary));
    addAP("dataSavingPs.binaryLittleEndian", &dataSavingPs.binaryLittleEndian);
    addAP("dataSavingPs.binaryDoublePrecision", &dataSavingPs.binaryDoublePrecision);
    addAP("dataSavingPs.asciiSeparator", &dataSavingPs.asciiSeparator);
    addAP("dataSavingPs.asciiHeaderPrefix", &dataSavingPs.asciiHeaderPrefix);
    addAP("dataSavingPs.asciiCRLF", &dataSavingPs.asciiCRLF);


    // Graphs
    addAP("Plotp.active", &Plotp, &BasePlotData::active);
    addAP("Plotp.bufferExp", &Plotp, &BasePlotData::bufferExp);
    addAP("Plotp.interval", &Plotp, &BasePlotData::interval);
    addAP("Plotp.xRange", &Plotp, &BasePlotData::xRange);
    addAP("Plotp.plot[#].height", &Plotp, &BasePlotData::plot, &PlotData::height);
    addAP("Plotp.plot[#].yLower", &Plotp, &BasePlotData::plot, &PlotData::yLower);
    addAP("Plotp.plot[#].yUpper", &Plotp, &BasePlotData::plot, &PlotData::yUpper);
    AP *graphActive = addAP("Plotp.plot[#].graph[#].active", &Plotp, &BasePlotData::plot, &PlotData::graph, &GraphData::active);
    AP *graphChan =   addAP("Plotp.plot[#].graph[#].chan", &Plotp, &BasePlotData::plot, &PlotData::graph, &GraphData::chan);
    AP *graphVolt =   addAP("Plotp.plot[#].graph[#].isVoltage", &Plotp, &BasePlotData::plot, &PlotData::graph, &GraphData::isVoltage);
    AP *graphColor =  addAP("Plotp.plot[#].graph[#].color", &Plotp, &BasePlotData::plot, &PlotData::graph, &GraphData::color);

    addDeprecatedAP("Plotp.graphs[#].active", graphActive, 1);
    addDeprecatedAP("Plotp.graphs[#].color", graphColor, 1);
    addDeprecatedAP("Plotp.graphs[#].isVoltage", graphVolt, 1);
    addDeprecatedAP("Plotp.graphs[#].chan", graphChan, 1);
    addDeprecatedAP("Graphp[#].chan", graphChan, 1);
    addDeprecatedAP("Graphp[#].isVoltage", graphVolt, 1);
    addDeprecatedAP("Graphp[#].color", graphColor, 1);


    // Sample & hold
    addAP("SampleHoldp.active", &(SampleHoldp.active));
    addAP("SampleHoldp.threshV", &(SampleHoldp.threshV));
    addAP("SampleHoldp.trigChn", &(SampleHoldp.trigChn));


    // Performance monitor
    addAP("PerfMonp.active", &PerfMonp, &PerformanceMonitorData::active);
    addAP("PerfMonp.interval", &PerfMonp, &PerformanceMonitorData::interval);


    // Trigger
    addAP("Triggerp.active", &Triggerp, &TriggerData::active);
    addAP("Triggerp.channel", &Triggerp, &TriggerData::channel);

    // Settling
    addAP("Settlingp.active", &Settlingp, &SettlingData::active);
    addAP("Settlingp.duration", &Settlingp, &SettlingData::duration);
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
#ifdef DIGIDATA_PT
    #include "DigiData.h"
#endif
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
#ifdef DIGIDATA_PT
    DigiDataData dd;
#endif
#ifdef NATIONAL_INSTRUMENTS
    NIDAQData nidaq;
#endif

    struct legacyGraphData {
      bool active[4] = {0,0,0,0};
      QString color[4] = {"","","",""};
      int chn[4] = {0,0,0,0};
      double miny[4];
      double maxy[4];
      double xrange;
      double dt;
      int xtNo;
      int ytNo;
      double yfac[4];
    } legacyGraphp[2];

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
        short dd_baseAddress;
        is >> selection;
        is >> sdaq.inFileName >> sdaq.outFileName >> sdaqInChnNo >> sdaqOutChnNo >> sdaq.inTFac >> sdaq.outDt;
        sdaq.inChn.resize(sdaqInChnNo);
        sdaq.outChn.resize(sdaqOutChnNo);
        is >> dd_baseAddress;
        if ( selection == 0 ) {
            sdaq.active = true;
            deprec = deprecateChannelsTo("SDAQp[#]");
        }
#ifdef DIGIDATA_PT
        dd.baseAddress = dd_baseAddress;
        if ( selection == 1 ) {
            dd.active = true;
            deprec = deprecateChannelsTo("DigiDatap[#]");
        }
#endif
#ifdef NATIONAL_INSTRUMENTS
        if ( selection == 2 ) {
            is >> nidaq.deviceName;
            nidaq.active = true;
            deprec = deprecateChannelsTo("NIDAQp[#]");
        }
#endif
        if ( !is.good() )
            return false;

        SimulDAQProxy::p.insert(SimulDAQProxy::p.begin(), sdaq);
        if ( sdaq.active )
            LEGACY_DAQ_CLASS = "SimulDAQ";
#ifdef DIGIDATA_PT
        AP *syncio = AP::find("DigiDatap[#].syncIOMask");
        if ( syncio )
            deprec.push_back(std::unique_ptr<AP>(new APDeprec("DigiDatap.syncIOMask", syncio, 1)));

        DigiDataProxy::p.insert(DigiDataProxy::p.begin(), dd);
        if ( dd.active )
            LEGACY_DAQ_CLASS = "DigiData1200";
#endif
#ifdef NATIONAL_INSTRUMENTS
        if ( nidaq.active ) {
            NIDAQProxy::p.insert(NIDAQProxy::p.begin(), nidaq);
            LEGACY_DAQ_CLASS = "NIDAQ";
        }
#endif
        if ( !is.good() )
            return false;

        addAP(deprec, "Graphp[#].active[#]", &legacyGraphp, &legacyGraphData::active);
        addAP(deprec, "Graphp[#].color[#]", &legacyGraphp, &legacyGraphData::color);
        addAP(deprec, "Graphp[#].chn[#]", &legacyGraphp, &legacyGraphData::chn);
        addAP(deprec, "Graphp[#].miny[#]", &legacyGraphp, &legacyGraphData::miny);
        addAP(deprec, "Graphp[#].maxy[#]", &legacyGraphp, &legacyGraphData::maxy);
        addAP(deprec, "Graphp[#].xrange", &legacyGraphp, &legacyGraphData::xrange);
        addAP(deprec, "Graphp[#].dt", &legacyGraphp, &legacyGraphData::dt);
        addAP(deprec, "Graphp[#].xtNo", &legacyGraphp, &legacyGraphData::xtNo);
        addAP(deprec, "Graphp[#].ytNo", &legacyGraphp, &legacyGraphData::ytNo);
        addAP(deprec, "Graphp[#].yfac[#]", &legacyGraphp, &legacyGraphData::yfac);
    }

    // before version 2: 4 AEC channels only
    std::vector<elecCalibParams> elecCalibPs;
    if ( version < 2 ) {
        deprec.reserve(deprec.size() + 17);
        addAP(deprec, "elecCalibPs[#].copyChnOn", &elecCalibPs, &elecCalibParams::copyChnOn);
        addAP(deprec, "elecCalibPs[#].copyChn", &elecCalibPs, &elecCalibParams::copyChn);
        addAP(deprec, "elecCalibPs[#].samplingRate", &elecCalibPs, &elecCalibParams::samplingRate);
        addAP(deprec, "elecCalibPs[#].inputChannelNumber", &elecCalibPs, &elecCalibParams::legacy_in);
        addAP(deprec, "elecCalibPs[#].outputChannelNumber", &elecCalibPs, &elecCalibParams::legacy_out);
        addAP(deprec, "elecCalibPs[#].iMaxElec", &elecCalibPs, &elecCalibParams::iMaxElec);
        addAP(deprec, "elecCalibPs[#].iMinElec", &elecCalibPs, &elecCalibParams::iMinElec);
        addAP(deprec, "elecCalibPs[#].numberOfLevels", &elecCalibPs, &elecCalibParams::numberOfLevels);
        addAP(deprec, "elecCalibPs[#].injLenPerLevel", &elecCalibPs, &elecCalibParams::injLenPerLevel);
        addAP(deprec, "elecCalibPs[#].iMembStep", &elecCalibPs, &elecCalibParams::iMembStep);
        addAP(deprec, "elecCalibPs[#].numberOfRepeats", &elecCalibPs, &elecCalibParams::numberOfRepeats);
        addAP(deprec, "elecCalibPs[#].injLenPerRepeat", &elecCalibPs, &elecCalibParams::injLenPerRepeat);
        addAP(deprec, "elecCalibPs[#].hyperpolCurr", &elecCalibPs, &elecCalibParams::hyperpolCurr);
        addAP(deprec, "elecCalibPs[#].injCalAmp", &elecCalibPs, &elecCalibParams::injCalAmp);
        addAP(deprec, "elecCalibPs[#].injCalLen", &elecCalibPs, &elecCalibParams::injCalLen);
        addAP(deprec, "elecCalibPs[#].fullKernelLen", &elecCalibPs, &elecCalibParams::fullKernelLen);
        addAP(deprec, "elecCalibPs[#].electrodeKernelLen", &elecCalibPs, &elecCalibParams::electrodeKernelLen);
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

        for ( ConductanceProxy *prox : Conductances.Register() ) {
            SynapseProxy *synprox  = dynamic_cast<SynapseProxy*>(prox);
            if ( synprox ) {
                for ( size_t i = 0; i < synprox->size(); i++ ) {
                    SynapseAssignment assign;
                    SynapseData &p = synprox->param(i);
                    if ( p.legacy_PreSyn >= 0 && p.legacy_PreSyn < int(legacyIn.size()) )
                        assign.PreSynChannel = legacyIn[p.legacy_PreSyn];
                    if ( p.legacy_PostSyn >= 0 && p.legacy_PostSyn < int(legacyIn.size()) )
                        assign.PostSynChannel = legacyIn[p.legacy_PostSyn];
                    if ( p.legacy_OutSyn >= 0 && p.legacy_OutSyn < int(legacyIn.size()) )
                        assign.OutSynChannel = legacyIn[p.legacy_OutSyn];
                    if ( assign.PreSynChannel.isValid && assign.PostSynChannel.isValid && assign.OutSynChannel.isValid )
                        p.assign.push_back(assign);
                }
                continue;
            }
            GapJunctionProxy *gjprox = dynamic_cast<GapJunctionProxy*>(prox);
            if ( gjprox ) {
                for ( size_t i = 0; i < gjprox->size(); i++ ) {
                    GapJunctionAssignment assign;
                    GJunctData &p = gjprox->param(i);
                    if ( p.legacy_PreIn >= 0 && p.legacy_PreIn < int(legacyIn.size()) )
                        assign.preInChannel = legacyIn[p.legacy_PreIn];
                    if ( p.legacy_PostIn >= 0 && p.legacy_PostIn < int(legacyIn.size()) )
                        assign.postInChannel = legacyIn[p.legacy_PostIn];
                    if ( p.legacy_PreOut >= 0 && p.legacy_PreOut < int(legacyOut.size()) )
                        assign.preOutChannel = legacyOut[p.legacy_PreOut];
                    if ( p.legacy_PostOut >= 0 && p.legacy_PostOut < int(legacyOut.size()) )
                        assign.postOutChannel = legacyOut[p.legacy_PostOut];
                    if ( assign.preInChannel.isValid && assign.postInChannel.isValid
                         && assign.preOutChannel.isValid && assign.postOutChannel.isValid )
                        p.assign.push_back(assign);
                }
                continue;
            }
            IonicCurrentProxy *cprox = dynamic_cast<IonicCurrentProxy*>(prox);
            if ( cprox ) {
                for ( size_t i = 0; i < cprox->size(); i++ ) {
                    CurrentAssignment assign;
                    CurrentData &p = cprox->param(i);
                    if ( p.legacy_V >= 0 && p.legacy_V < int(legacyIn.size()) )
                        assign.VChannel = legacyIn[p.legacy_V];
                    if ( p.legacy_I >= 0 && p.legacy_I < int(legacyOut.size()) )
                        assign.IChannel = legacyOut[p.legacy_I];
                    if ( assign.VChannel.isValid && assign.IChannel.isValid )
                        p.assign.push_back(assign);
                }
                continue;
            }
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

        for ( int j = 0; j < 2; j++ ) {
            legacyGraphData &graphp = legacyGraphp[j];
            if ( graphp.dt )
                Plotp.interval = graphp.dt;
            PlotData plot;
            for ( int i = 0; i < 4; i++ ) {
                GraphData g;
                g.active = graphp.active[i];
                // legacy chan: 0=None, 1,...,m=in, m+1,...,n=out, last=SG
                // legacyIn includes SG
                int chan = graphp.chn[i];
                if ( chan == 0 )
                    g.chan = ChannelIndex::None();
                else if ( chan == int(legacyIn.size() + legacyOut.size()) )
                    g.chan = legacyIn.back(); // SG
                else if ( chan < int(legacyIn.size()) )
                    g.chan = legacyIn[chan-1]; // In
                else
                    g.chan = legacyOut[chan - legacyIn.size()]; // Out
                g.color = QColor(graphp.color[i]);
                g.isVoltage = g.chan.isInChn || g.chan.isVirtual;
                plot.graph.push_back(g);
            }
            Plotp.plot.push_back(plot);
        }
    }

    if ( version < 4 ) {
        // version 4 introduced a restriction on HH channel assignments: no model instances.
        // Compat: Reassigns all model instances to model prototypes
        for ( ConductanceProxy *prox : Conductances.Currents() ) {
            IonicCurrentProxy *cprox = dynamic_cast<IonicCurrentProxy*>(prox);
            if ( cprox ) {
                for ( size_t i = 0; i < cprox->size(); i++ ) {
                    CurrentData &p = cprox->param(i);
                    std::vector<ChannelIndex> mod;
                    std::vector<CurrentAssignment> dest;
                    dest.reserve(p.assign.size());
                    for ( CurrentAssignment &a : p.assign ) {
                        if ( a.IChannel.isVirtual && a.VChannel == a.IChannel ) {
                            a.IChannel.isPrototype = true;
                            a.VChannel.isPrototype = true;
                            a.IChannel.isVirtual = false;
                            a.VChannel.isVirtual = false;
                            if ( std::find(mod.begin(), mod.end(), a.IChannel)!=mod.end() ) {
                                continue; // Drop duplicate entries
                            }
                            mod.push_back(a.IChannel);
                        } else if ( a.IChannel.isPrototype && a.VChannel == a.IChannel ) {
                            mod.push_back(a.IChannel);
                        }
                        dest.push_back(a);
                    }
                    p.assign = dest;
                }
            }
        }
    }

    return true;
}

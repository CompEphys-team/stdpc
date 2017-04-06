#include "IonicCurrent.h"
#include "DCThread.h"

IonicCurrent::IonicCurrent(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) :
    Conductance(condID, assignID, multiID),
    in(in),
    out(out)
{
}

void IonicCurrentProxy::instantiate(size_t condID, size_t assignID, DCThread *DCT, std::vector<Conductance *> &preD, std::vector<Conductance *> &inD, std::vector<Conductance *> &)
{
    const CurrentAssignment &a = param(condID).assignment(assignID);
    size_t multi = 0;
    if ( a.VChannel == a.IChannel ) {
        // Input/Output on the same model => connect instances 1-to-1 rather than all-to-all
        for ( ChannelIndex VIChan : DCT->getChanIndices(a.VChannel) ) {
            inChannel *inC = DCT->getInChan(VIChan);
            outChannel *outC = DCT->getOutChan(VIChan);
            if ( inC && outC ) {
                inD.push_back(createAssigned(condID, assignID, multi++, inC, outC));
            }
        }
    } else { // NOTE: These channels are assumed to be analog only, although technically, a/d combos are permitted
        for ( ChannelIndex VChan : DCT->getChanIndices(a.VChannel) ) {
            for ( ChannelIndex IChan : DCT->getChanIndices(a.IChannel) ) {
                inChannel *inC = DCT->getInChan(VChan);
                outChannel *outC = DCT->getOutChan(IChan);
                if ( inC && outC ) {
                    preD.push_back(createAssigned(condID, assignID, multi++, inC, outC));
                }
            }
        }
    }
}

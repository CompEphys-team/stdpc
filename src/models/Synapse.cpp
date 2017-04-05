#include "Synapse.h"
#include "DCThread.h"

void SynapseProxy::instantiate(size_t conductanceID, size_t assignID, DCThread *DCT,
                               std::vector<Conductance *> &preD,
                               std::vector<Conductance *> &,
                               std::vector<Conductance *> &postD)
{
    const SynapseAssignment &a = param(conductanceID).assignment(assignID);
    std::vector<ChannelIndex> preSynInst = DCT->getChanIndices(a.PreSynChannel);
    std::vector<ChannelIndex> postSynInst = DCT->getChanIndices(a.PostSynChannel);
    std::vector<ChannelIndex> outSynInst = DCT->getChanIndices(a.OutSynChannel);
    inChannel *preC, *postC;
    outChannel *outC;

    if ( a.PostSynChannel == a.OutSynChannel ) {
        for ( ChannelIndex post : postSynInst ) {
            for ( ChannelIndex pre : preSynInst ) {
                if ( (preC=DCT->getInChan(pre)) && (postC=DCT->getInChan(post)) && (outC=DCT->getOutChan(post)) ) {
                    Synapse *tmp = createAssigned(conductanceID, assignID, DCT, preC, postC, outC);
                    if ( pre.isVirtual && post.isAnalog )
                        postD.push_back(tmp);
                    else
                        preD.push_back(tmp);
                }
            }
        }
    } else {
        for ( ChannelIndex post : postSynInst ) {
            for ( ChannelIndex out : outSynInst ) {
                for ( ChannelIndex pre : preSynInst ) {
                    if ( (preC=DCT->getInChan(pre)) && (postC=DCT->getInChan(post)) && (outC=DCT->getOutChan(out)) ) {
                        Synapse *tmp = createAssigned(conductanceID, assignID, DCT, preC, postC, outC);
                        if ( (pre.isVirtual || post.isVirtual) && out.isAnalog )
                            postD.push_back(tmp);
                        else
                            preD.push_back(tmp);
                    }
                }
            }
        }
    }
}


Synapse::Synapse(size_t condID, size_t assignID, inChannel *pre, inChannel *post, outChannel *out) :
    Conductance(condID, assignID),
    pre(pre),
    post(post),
    out(out),
    buffered(false)
{

}

void Synapse::setupBuffer(DCThread *DCT)
{
    if ( assignment().delay > 0. ) {
        bufferHandle = pre->getBufferHandle(assignment().delay, DCT->bufferHelper);
        buffered = true;
    }
}

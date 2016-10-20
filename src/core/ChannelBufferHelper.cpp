#include "ChannelBufferHelper.h"

ChannelBufferHelper::ChannelBufferHelper() :
    tbuf(nullptr),
    tbufsize(0),
    _absOffset(0)
{}

ChannelBufferHelper::~ChannelBufferHelper()
{
    delete tbuf;
}

void ChannelBufferHelper::advance(double t)
{
    if ( !tbufsize )
        return;

    ++_absOffset;
    size_t currentIndex = _absOffset % tbufsize;
    tbuf[currentIndex] = t;

    size_t idx;
    double expectedT;
    size_t mv;

    for ( BufferHandler &h : handler ) {
        if ( h.initial ) {
            h.offset = currentIndex;
            if ( h.duration >= t ) {
                h.initial = false;
            }
        } else {
            idx = index(currentIndex - h.offset);
            expectedT = t - h.duration;
            if ( tbuf[idx] > expectedT ) {
                for ( mv = 1; mv < tbufsize - h.offset - 1; mv++ )
                    if ( tbuf[index(idx - mv)] <= expectedT )
                        break;
                h.offset += mv-1;
            } else if ( tbuf[idx] < expectedT ) {
                for ( mv = 1; mv < h.offset; mv++ )
                    if ( tbuf[index(idx + mv)] >= expectedT )
                        break;
                h.offset -= mv;
            }
        }
    }
}

size_t ChannelBufferHelper::getHandle(double duration)
{
    size_t handle;

    // Expand t buffer
    size_t reqSize = requiredSize(duration);
    if ( reqSize > tbufsize ) {
        tbufsize = reqSize;
        delete tbuf;
        tbuf = new double[tbufsize]();
    }

    // Find/create handler
    size_t i;
    for ( i = 0; i < handler.size(); i++ ) {
        if ( handler[i].duration == duration )
            handle = i;
    }
    if ( i == handler.size() ) {
        handler.push_back(BufferHandler {0, duration, true});
        handle = i;
    }

    return handle;
}

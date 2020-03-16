/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "ChannelBufferHelper.h"
#include <cassert>

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
    bool up, down;

    for ( BufferHandler &h : handler ) {
        if ( h.initial ) {
            h.offset = currentIndex - 1;
            if ( t >= h.duration ) {
                h.initial = false;
            }
        } else if ( _absOffset < tbufsize ) {
            // Boundary conditions apply. General case with comments below
            // Note that the very first point recorded is at _absOffset == 1
            idx = currentIndex - h.offset; // 1 >= h.offset >= currentIndex is given, no need to wrap around
            expectedT = t - h.duration;
            up = tbuf[idx] < expectedT;
            down = !up;
            while ( up != down && (up || idx > 1) && (down || idx < currentIndex) ) {
                if ( tbuf[idx] > expectedT ) {
                    down = true;
                    idx--;
                } else if ( tbuf[idx] < expectedT ) {
                    down = false;
                    idx++;
                } else {
                    break;
                }
            }

            // Edge cases
            if ( up && !down && idx == currentIndex ) {
                idx--;
                down = true;
            } else if ( !up && down && idx == 1 ) {
                idx++;
                down = false;
            }

            // Rounding
            if ( up && down ) {
                if ( expectedT - tbuf[idx] > tbuf[idx+1] - expectedT )
                    idx++;
            } else if ( !up && !down ) {
                if ( expectedT - tbuf[idx-1] < tbuf[idx] - expectedT )
                    idx--;
            }

            assert(idx >= 1);
            assert(idx <= currentIndex);
            h.offset = currentIndex - idx;
        } else {
            // Add tbufsize to let idx wrap around the buffer without having to use further addition/%
            idx = tbufsize + currentIndex - h.offset;
            expectedT = t - h.duration;
            up = tbuf[index(idx)] < expectedT;
            down = !up;
            //      Don't change direction
            //                    Come closer, or stay clear of the far end of the allowable range ]currI, currI+bufsz]
            //                                                  Go farther, or stay clear of the near end
            while ( up != down && (up || idx > currentIndex+1) && (down || idx < tbufsize + currentIndex) ) {
                if ( tbuf[index(idx)] > expectedT) {
                    down = true;
                    idx--;
                } else if ( tbuf[index(idx)] < expectedT ) {
                    down = false;
                    idx++;
                } else { // tbuf[index(idx)] == expectedT
                    break;
                }
            }

            // Redirect edge cases:
            if ( up && !down && idx == tbufsize + currentIndex ) {
            // Either h.offset==0, or tbuf[index(idx-1)] < expectedT; in either case, idx is at the near end
            // => Let case (up && down) below take care of sorting out whether idx should be at the end or one short
                idx--;
                down = true;
            } else if ( !up && down && idx == currentIndex+1 ) {
            // Either h.offset==tbufsize-1, or tbuf[index(idx+1)] > expectedT; in either case, idx is at the far end
            // => Let case (!up && !down) below take care ...
                idx++;
                down = false;
            }

            // Rounding
            if ( up && down ) { // tbuf[index(idx)] < expectedT < tbuf[index(idx+1)] :
                                // (or expectedT > tbuf[index(idx+1)], if redirected edge case)
                if ( expectedT - tbuf[index(idx)] > tbuf[index(idx+1)] - expectedT )
                // expectedT is closer to tbuf[index(idx+1)]
                    idx++;
            } else if ( !up && !down ) { // tbuf[index(idx-1)] < expectedT < tbuf[index(idx)] :
                                         // (or tbuf[index(idx-1)] > expectedT, if redirected edge case)
                if ( expectedT - tbuf[index(idx-1)] < tbuf[index(idx)] - expectedT )
                // expectedT is closer to tbuf[index(idx-1)]
                    idx--;
            }
            // other cases:
            // no edge, up != down : Precise match, expectedT == tbuf[index(idx)]; no adjustments needed

            assert(idx > currentIndex);             // ==> h.offset < tbufsize
            assert(idx <= tbufsize + currentIndex); // ==> h.offset >= 0
            h.offset = tbufsize + currentIndex - idx;
        }
    }
}

void ChannelBufferHelper::rewind()
// Resets the offset, discarding all buffered data.
{
    _absOffset = 0;
    for ( BufferHandler &h : handler )
        h.initial = true;
}

size_t ChannelBufferHelper::getHandle(double duration)
{
    size_t handle = 0;

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

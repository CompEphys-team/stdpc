/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
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

#ifndef CHANNELBUFFERHELPER_H
#define CHANNELBUFFERHELPER_H

#include <vector>
using std::size_t;

class ChannelBufferHelper
{
public:
    ChannelBufferHelper();
    ~ChannelBufferHelper();

    void advance(double t);

    inline static size_t requiredSize(double duration)
    {
        // Assuming max cycle frequency of 50 MHz, which should be plenty:
        return 1 + (size_t)(duration * 5e7);
    }

    size_t getHandle(double duration);

    inline unsigned long long absOffset() const { return _absOffset; }
    inline size_t offset(size_t handle) const { return handler[handle].offset; }

private:
    inline size_t index(int offset) const { return (offset + tbufsize) % tbufsize; }

    double *tbuf;
    size_t tbufsize;

    struct BufferHandler
    {
        size_t offset;
        double duration;
        bool initial;
    };
    std::vector<BufferHandler> handler;

    unsigned long long _absOffset;
};

#endif // CHANNELBUFFERHELPER_H

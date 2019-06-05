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

/* CircularFifo is a single-reader, single-writer queue.
 * It is adapted from http://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular
 * (I changed variable names to be clearer, as "head"/"tail" are more confusing than useful to me.)
 * Note that the actual queue capacity is SIZE-1 elements.
 */
#ifndef CIRCULARFIFO_H
#define CIRCULARFIFO_H

#include <atomic>
#include <new>


template <typename T>
class CircularFifo
{
public:
    CircularFifo(size_t size) : SIZE(size), _read(0), _write(0)
    {
        bool success = false;
        while ( !success ) {
            try {
                _data = new T[SIZE];
                success = true;
            } catch (std::bad_alloc) {
                if ( SIZE == 1 )
                    throw;
                success = false;
                SIZE /= 2;
            }
        }
    }
    ~CircularFifo()
    {
        delete _data;
    }

    bool push(const T& item)
    {
        const auto current_write = _write.load(std::memory_order_relaxed);
        const auto next_write = increment(current_write);
        if(next_write != _read.load(std::memory_order_acquire)) {
            _data[current_write] = item;
            _write.store(next_write, std::memory_order_release);
            return true;
        }
        return false; // full queue
    }

    bool pop(T& item)
    {
        const auto current_read = _read.load(std::memory_order_relaxed);
        if(current_read == _write.load(std::memory_order_acquire))
            return false; // empty queue
        item = _data[current_read];
        _read.store(increment(current_read), std::memory_order_release);
        return true;
    }

    void reset()
    {
        _read.store(0);
        _write.store(0);
    }

private:
    inline size_t increment(size_t i) const
    {
        return (i + 1) % SIZE;
    }

    size_t SIZE;
    T *_data;
    std::atomic<size_t> _read;
    std::atomic<size_t> _write;
};

#endif // CIRCULARFIFO_H

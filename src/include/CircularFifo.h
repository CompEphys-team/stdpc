/* CircularFifo is a single-reader, single-writer queue.
 * It is adapted from http://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular
 * (I changed variable names to be clearer, as "head"/"tail" are more confusing than useful to me.)
 * Note that the actual queue capacity is SIZE-1 elements.
 */
#ifndef CIRCULARFIFO_H
#define CIRCULARFIFO_H

#include <atomic>


template <typename T, size_t SIZE>
class CircularFifo
{
public:
    CircularFifo() : _read(0), _write(0) {}
    ~CircularFifo() {}

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

    T _data[SIZE];
    std::atomic<size_t> _read;
    std::atomic<size_t> _write;
};

#endif // CIRCULARFIFO_H

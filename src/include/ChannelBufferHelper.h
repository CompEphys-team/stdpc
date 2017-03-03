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

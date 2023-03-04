#pragma once
#include <stdint.h>

class RingBuffer
{
public:
    RingBuffer(uint8_t *buffer, uint32_t len);

    uint32_t count();
    uint32_t capacity();
    bool isFull();
    bool isEmpty();
    uint8_t peek();
    bool push(uint8_t element, bool force = false);
    bool pop(uint8_t &element);
    void clear();

    // unsafe
    void _push(uint8_t element);
    uint8_t _pop();

private:
    volatile uint8_t *buffer;
    volatile uint32_t bufferLen;
    volatile uint32_t head;
    volatile uint32_t tail;
};

#define DEFINE_RING_BUFFER(attrs, name, len) \
    attrs uint8_t name##_buffer[len];        \
    attrs RingBuffer name(name##_buffer, len);

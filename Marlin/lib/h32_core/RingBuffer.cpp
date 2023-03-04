#include "RingBuffer.h"

//
// public
//
RingBuffer::RingBuffer(uint8_t *buffer, uint32_t len)
{
    this->buffer = buffer;
    this->bufferLen = len;
    this->head = 0;
    this->tail = 0;
}

uint32_t RingBuffer::count()
{
    if (this->head <= this->tail)
    {
        return this->tail - this->head;
    }
    else
    {
        return this->tail + (this->bufferLen - this->head);
    }
}

uint32_t RingBuffer::capacity() 
{
    return this->bufferLen;
}

bool RingBuffer::isFull()
{
    return this->count() >= this->bufferLen;
}

bool RingBuffer::isEmpty()
{
    return this->head == this->tail;
    // return this->count() == 0;
}

uint8_t RingBuffer::peek()
{
    return this->isEmpty() ? 0 : this->buffer[this->head];
}

bool RingBuffer::push(uint8_t element, bool force)
{
    if (this->isFull())
    {
        if (force)
        {
            // remove the last element to make space
            this->_pop();
        }
        else
        {
            return false;
        }
    }

    this->_push(element);
    return true;
}

bool RingBuffer::pop(uint8_t &element)
{
    if (this->isEmpty())
    {
        return false;
    }

    element = this->_pop();
    return true;
}

void RingBuffer::clear()
{
    this->head = 0;
    this->tail = 0;
}

//
// protected
//
#define INCREMENT_AND_LOOP_AROUND(ii)   \
    {                                   \
        ii++;                           \
        if (ii >= (this->bufferLen - 1)) \
        {                               \
            ii = 0;                     \
        }                               \
    }

void RingBuffer::_push(uint8_t element)
{
    this->buffer[this->tail] = element;
    INCREMENT_AND_LOOP_AROUND(this->tail);
}

uint8_t RingBuffer::_pop()
{
    uint8_t element = this->buffer[this->head];
    INCREMENT_AND_LOOP_AROUND(this->head);
    return element;
}

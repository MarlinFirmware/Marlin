# Temporary Buffer Allocator

Marlin is not using the heap to allocate memory.
Instead, Marlin uses static allocation (stored in .bss section) for its buffers.

However, there are numerous places in the code base where it requires temporary *large* work buffers.
In the past, large buffer were reused around based on the assumption it'd not interfer with each other.

Yet, this makes maintaining the code harder, since we can't change the buffer size or declaration without 
impacting unrelated code elsewhere.

So in order to solve this issue, a new hybrid allocator was created. 
It's allocating (leasing) fixed size static buffer to a scope limited handler and free (release) the buffer upon leaving the scope.
Unlike an heap, it can't fragment and will always ensure a buffer is available (unless the call stack is too deep)  

This shouldn't be used for long lived buffer (like static class member's instance).

## Design

At the heart of this allocator, is an array of fixed sized buffers:
```
[ N bytes ][ N bytes ]  ... [ N bytes ]
           ^ next
```
And a counter that's tracking the next buffer to use.
Then a C++ RAII class is used to grab (lease) a buffer by incrementing the counter and decrementing it when leaving the scope.

Since Marlin is mono-tasked, this ensures that 2 functions cannot grab the counter or release it simultaneously (care should be taken not to use the allocator in INT context, but that's usually the case).

The allocator is tracking multiple array size this way, so it's able to provide a larger buffer whenever required by mergeing (literally incrementing the counter more than once) buffer and releasing them.

With a buffer size of 1 byte, it's acting like a dumb stack. With a different buffer size, it's acting like a pool stack. In that case, it'll lease aligned buffers to the size `N` (but could waste as much as `N-1` bytes per buffer).

### How is it different than a stack allocation ?

Obviously, the description above triggers the question: 

What's the interest compared to plain old fixed size array: `char buffer[N]` that would also be released automatically by the compiler upon exiting the scope ? 

1. It's not using stack space (so it's interesting in recursive code too)
2. It can reallocate larger/lower on usage: no more `char buffer[34 + strlen("SOMETHING") + 1 + 3]; // for end of line+ path separator +... ` code. Use it with the dynamic string class and it'll adjust its size whenever it needs to. This is done transparently (and obviously, you can preallocate if you know beforehand how much you'll need). Reallocation in this context is increasing the counter, so it's a very very low overhead.
3. No more stack overflow (printer crash) if you declare a too large buffer, but a resumable runtime failure (with a message on the serial console instead)

### When not to use it ?

If a buffer needs to be used by something exterior to the current code path:
1. For example: a DMA transfer that'll continue/start after the function exits
2. In an interrupt context
3. In a very large allocation (there's size checking in the allocator but it's a runtime check so it'll BUG_ON overallocation). The maximum allocator's tracked size is MAX_ALLOC_SIZE


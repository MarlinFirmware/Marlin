#pragma once

#include <cstdint>
#include <mutex>
#include <atomic>
#include <type_traits>

template<typename T, std::size_t S, typename IndexType = std::size_t> class RingBuffer {
public:
  static_assert(std::is_integral<IndexType>() && !std::is_signed<IndexType>());
  static_assert(S > 2 && ((S & (S - 1)) == 0), "RingBuffer<T, S, IndexType>: Implementation Requires S is a power of 2");
  static_assert(S <= (std::numeric_limits<IndexType>::max() >> 1), "RingBuffer<T, S, IndexType>: Implementation Requires S is less than IndexType::Max / 2");

  RingBuffer() {
    index_read = index_write = static_cast<IndexType>(0);
  }

  inline std::size_t available() {
    std::scoped_lock lock{access_mutex};
    return static_cast<std::size_t>(static_cast<IndexType>(index_write - index_read));
  }

  inline std::size_t free() {
    return size() - available();
  }

  inline bool empty() {
    std::scoped_lock lock{access_mutex};
    return index_read == index_write;
  }

  inline bool full() {
    return available() == S;
  }

  inline void clear() {
    std::scoped_lock lock{access_mutex};
    index_read = index_write = static_cast<IndexType>(0);
  }

  inline std::size_t read(T* dst, std::size_t const length, bool const drop = true) {
    std::scoped_lock lock{access_mutex};
    IndexType length0                 = static_cast<IndexType>(std::min(length, available()));
    IndexType const masked_index_read = mask(index_read);
    IndexType const length1           = std::min(length0, static_cast<IndexType>(buffer_size - masked_index_read));
    memcpy(dst, (char*)buffer + masked_index_read, length1);
    memcpy(dst + length1, (char*)buffer, length0 - length1);
    if (drop == true) index_read += length0;
    return length0;
  }

  template<typename ExtBuffer>
  std::size_t read(ExtBuffer& buff) {
    static_assert(std::is_same_v<typename ExtBuffer::element_type, element_type>);
    std::scoped_lock lock{access_mutex, buff.access_mutex};
    IndexType length0                 = static_cast<IndexType>(std::min(buff.free(), available()));
    if (length0 == 0) return 0;
    IndexType const masked_index_read = mask(index_read);
    IndexType const length1           = std::min(length0, static_cast<IndexType>(buffer_size - masked_index_read));
    buff.write(buffer + masked_index_read, length1);
    buff.write(buffer, length0 - length1);
    index_read += length0;
    return length0;
  }

  inline std::size_t write(T const* src, std::size_t const length) {
    std::scoped_lock lock{access_mutex};
    IndexType length0                  = static_cast<IndexType>(std::min(length, free()));
    IndexType const masked_index_write = mask(index_write);
    IndexType const length1            = std::min(length0, static_cast<IndexType>(buffer_size - masked_index_write));
    memcpy((char*)buffer + masked_index_write, src, length1);
    memcpy((char*)buffer, src + length1, length0 - length1);
    index_write += length0;
    return length0;
  }

  inline bool peek(T* const value, std::size_t const index) {
    std::scoped_lock lock{access_mutex};
    if (value == nullptr || index > available()) return false;
    *value = buffer[mask(index_read + index)];
    return true;
  }

  inline bool find_next_index_of(T const& value, size_t& index, size_t offset = 0) {
    std::scoped_lock lock{access_mutex};
    for (size_t i = offset; i < available(); ++i) {
      if (buffer[mask(index_read + i)] == value) {
        index = i;
        return true;
      }
    }
    return false;
  }

  inline std::size_t read(T* const value) {
    std::scoped_lock lock{access_mutex};
    if (value == nullptr || empty()) return 0;
    *value = buffer[mask(index_read++)];
    return 1;
  }

  inline std::size_t write(T const value) {
    std::scoped_lock lock{access_mutex};
    if (full()) return 0;
    buffer[mask(index_write++)] = value;
    return 1;
  }

  inline bool peek(T* const value) {
    std::scoped_lock lock{access_mutex};
    if (value == nullptr || empty()) return false;
    *value = buffer[mask(index_read)];
    return true;
  }

  inline void drop(std::size_t length) {
    std::scoped_lock lock{access_mutex};
    index_read += static_cast<IndexType>(std::min(length, available()));
  }

  static constexpr inline std::size_t size() {
    return static_cast<std::size_t>(buffer_size);
  }

private:
  inline IndexType mask(IndexType val) const {
    return val & buffer_mask;
  }

  std::recursive_mutex access_mutex;
  static constexpr IndexType const buffer_size = static_cast<IndexType>(S);
  static constexpr IndexType const buffer_mask = buffer_size - static_cast<IndexType>(1);
  alignas(uintptr_t) T buffer[static_cast<std::size_t>(buffer_size)];
  std::atomic<IndexType> index_write;
  std::atomic<IndexType> index_read;
  using element_type = T;
};

template<typename T, std::size_t S> class InOutRingBuffer {
public:
  static constexpr std::size_t size() {
    return S;
  }

  RingBuffer<T, S> in;
  RingBuffer<T, S> out;
};
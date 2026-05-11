#pragma once

template<class F> struct return_type;

template<class R, class... A> struct return_type<R (*)(A...)> {
  typedef R type;
};

template<class R, class... A> struct return_type<R (*)(A..., ...)> {
  typedef R type;
};

template <typename T> constexpr auto to_integral(T e) { return static_cast<std::underlying_type_t<T>>(e); }

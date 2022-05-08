#ifndef STACK_HPP
#define STACK_HPP

#include "vector.hpp"

namespace ft {

template <class _Tp, class _Container = ft::vector<_Tp> >
class stack {
 public:
  typedef _Container                               container_type;
  typedef typename container_type::value_type      value_type;
  typedef typename container_type::size_type       size_type;

 protected:
  container_type c;

 public:
  // Default constructor
  stack() : c() {}

  explicit stack(const container_type& __c) : c(__c) {}

  stack(const stack& __q) : c(__q.c) {}

  stack& operator=(const stack& __q) {
    if (this != &__q) {
      c = __q.c;
    }
    return *this;
  }

  virtual ~stack() {}

  bool empty() const { return c.empty(); }

  size_type size() const { return c.size(); }

  value_type& top() { return c.back(); }

  const value_type& top() const { return c.back(); }

  void push(const value_type& __v) { c.push_back(__v); }

  void pop() { c.pop_back(); }

  template <class T1, class _C1>
  friend
  bool
  operator==(const stack<T1, _C1>& __x, const stack<T1, _C1>& __y);

  template <class T1, class _C1>
  friend
  bool
  operator< (const stack<T1, _C1>& __x, const stack<T1, _C1>& __y);
};

template <class _Tp, class _Container>
bool
operator==(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y) {
  return __x.c == __y.c;
}

template <class _Tp, class _Container>
bool
operator< (const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y) {
  return __x.c < __y.c;
}

template <class _Tp, class _Container>
bool
operator!=(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y) {
  return !(__x == __y);
}

template <class _Tp, class _Container>
bool
operator> (const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y) {
  return __y < __x;
}

template <class _Tp, class _Container>
bool
operator>=(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y) {
  return !(__x < __y);
}

template <class _Tp, class _Container>
bool
operator<=(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y) {
  return !(__y < __x);
}

}

#endif // STACK_HPP

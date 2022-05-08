#ifndef VECTOR_ITERATOR_HPP
#define VECTOR_ITERATOR_HPP

#include <iterator>
#include "iterator_traits.hpp"

namespace ft {

template <typename _Iter>
class vector_iterator {

 public:
  typedef _Iter                                                          iterator_type;
  typedef typename ft::iterator_traits<iterator_type>::iterator_category iterator_category;
  typedef typename ft::iterator_traits<iterator_type>::value_type        value_type;
  typedef typename ft::iterator_traits<iterator_type>::difference_type   difference_type;
  typedef typename ft::iterator_traits<iterator_type>::pointer           pointer;
  typedef typename ft::iterator_traits<iterator_type>::reference         reference;

 private:
  iterator_type __i_;

 public:

  // Default constructor
  vector_iterator() : __i_() {}

  // Constructor
  vector_iterator(iterator_type __x) : __i_(__x) {}

  // Copy constructor and assignation operator

  // TODO: Do we need to use enable_if and is_convertible to check if _Up is valid parameter?

  template <class _Up>
  vector_iterator(const vector_iterator<_Up>& __u) : __i_(__u.base()) {}

  template <class _Up>
  vector_iterator& operator=(const vector_iterator<_Up>& __u) {
    __i_ = __u.base();
    return *this;
  }

  virtual ~vector_iterator(){}

  reference operator*() const { return *__i_; }

  pointer operator->() const { return &(*__i_); }

  reference operator[](difference_type __n) { return __i_[__n]; }

  vector_iterator& operator++() {
    ++__i_;
    return *this;
  }

  vector_iterator operator++(int) {
    vector_iterator __tmp(*this);
    ++(*this);
    return __tmp;
  }

  vector_iterator& operator--() {
    --__i_;
    return *this;
  }

  vector_iterator operator--(int) {
    vector_iterator __tmp(*this);
    --(*this);
    return __tmp;
  }

  vector_iterator& operator+=(difference_type __n) {
    __i_ += __n;
    return *this;
  }

  vector_iterator& operator-=(difference_type __n) {
    *this += -__n;
    return *this;
  }

  // TODO:
  // Should this return a reference of vector_iterator?
  // How does __v can be lived outside this function? Is that beyond the scope of __v?

  // ANSWER:
  // The returned value should not be a reference as the returned value from + operator should be rvalue
  // The function will return the copy of __v so the returned value should be lived outside of the function

  vector_iterator operator+(difference_type __n) const {
    vector_iterator __v(*this);
    __v += __n;
    return __v;
  }

  vector_iterator operator-(difference_type __n) const { return *this + (-__n); }

  iterator_type base() const { return __i_; }

};

template <class _Iter1, class _Iter2>
bool operator==(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) {
  return __x.base() == __y.base();
}

template <class _Iter1, class _Iter2>
bool operator<(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) {
  return __x.base() < __y.base();
}

template <class _Iter1, class _Iter2>
bool operator!=(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) {
  return !(__x == __y);
}

template <class _Iter1, class _Iter2>
bool operator>(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) {
  return __y < __x;
}

template <class _Iter1, class _Iter2>
bool operator>=(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) {
  return !(__x < __y);
}

template <class _Iter1, class _Iter2>
bool operator<=(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) {
  return !(__y < __x);
}

// TODO: Why don't we need operator+ version of this non-member overloads?

template <class _Iter1, class _Iter2>
typename vector_iterator<_Iter1>::difference_type
operator-(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) {
  return __x.base() - __y.base();
}

// TODO: Why don't we need operator- version of this non-member overloads?

template <class _Iter>
vector_iterator<_Iter>
operator+(typename vector_iterator<_Iter>::difference_type __n,
          vector_iterator<_Iter> __x) {
  __x += __n;
  return __x;
}

}

#endif

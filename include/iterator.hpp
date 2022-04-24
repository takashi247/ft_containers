#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "iterator_traits.hpp"

namespace ft {

template <class _Iter>
class reverse_iterator : public std::iterator<
    typename ft::iterator_traits<_Iter>::iterator_category,
    typename ft::iterator_traits<_Iter>::value_type,
    typename ft::iterator_traits<_Iter>::difference_type,
    typename ft::iterator_traits<_Iter>::pointer,
    typename ft::iterator_traits<_Iter>::reference> {

 protected:
  _Iter current;
 public:
  typedef _Iter                                                iterator_type;
  typedef typename ft::iterator_traits<_Iter>::difference_type difference_type;
  typedef typename ft::iterator_traits<_Iter>::reference       reference;
  typedef typename ft::iterator_traits<_Iter>::pointer         pointer;

  // Default constructor
  reverse_iterator() : current() {}

  // Initialization constructor
  explicit reverse_iterator(iterator_type it) : current(it) {}

  // Copy constructor

  // TODO: Why don't we need to use enable_if and is_convertible to check if _Up is the valid type?

  template <class _Up>
  reverse_iterator(const reverse_iterator<_Up>& __u) : current(__u.base()) {}

  template <class _Up>
  reverse_iterator& operator=(const reverse_iterator<_Up>& __u) {
    if (this != &__u) {
      current = __u.base();
    }
    return *this;
  }

  iterator_type base() const { return current; }

  reference operator*() const {
    iterator_type __tmp = current;
    return *--__tmp;
  }

  // Base iterator must be a random-access iterator
  reverse_iterator operator+(difference_type __n) const {
    return reverse_iterator(current - __n);
  }

  reverse_iterator& operator++() {
    --current;
    return *this;
  }

  reverse_iterator operator++(int) {
    reverse_iterator __tmp(*this);
    --current;
    return __tmp;
  }

  // Base iterator must be a random-access iterator
  reverse_iterator& operator+=(difference_type __n) {
    current -= __n;
    return *this;
  }

  // Base iterator must be a random-access iterator
  reverse_iterator operator-(difference_type __n) const {
    return reverse_iterator(current + __n);
  }

  reverse_iterator& operator--() {
    ++current;
    return *this;
  }

  reverse_iterator operator--(int) {
    reverse_iterator __tmp(*this);
    ++current;
    return __tmp;
  }

  // Base iterator must be a random-access iterator
  reverse_iterator& operator-=(difference_type __n) {
    current += __n;
    return *this;
  }

  pointer operator->() const { return &**this; }

  // Base iterator must be a random-access iterator
  reference operator[](difference_type __n) const { return *(*this + __n); }

};

// Relational operators

// TODO: Why do we need to specify 2 template parameters (i.e., _Iter1 and _Iter2)?

template <class _Iter1, class _Iter2>
bool operator==(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y) {
  return __x.base() == __y.base();
}

template <class _Iter1, class _Iter2>
bool operator!=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y) {
  return __x.base() != __y.base();
}

template <class _Iter1, class _Iter2>
bool operator<(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y) {
  return __x.base() > __y.base();
}

template <class _Iter1, class _Iter2>
bool operator<=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y) {
  return __x.base() >= __y.base();
}

template <class _Iter1, class _Iter2>
bool operator>(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y) {
  return __x.base() < __y.base();
}

template <class _Iter1, class _Iter2>
bool operator>=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y) {
  return __x.base() <= __y.base();
}

template <class _Iter>
reverse_iterator<_Iter>
operator+(typename reverse_iterator<_Iter>::difference_type __n, const reverse_iterator<_Iter>& __x) {
  return reverse_iterator<_Iter>(__x.base() - __n);
}

// TODO: Confirm if it's ok to use difference_type of _Iter1 for the type of the returned value

template <class _Iter1, class _Iter2>
typename reverse_iterator<_Iter1>::difference_type
operator-(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y) {
  return __y.base() - __x.base();
}

}

#endif

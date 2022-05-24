#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "iterator_traits.hpp"

namespace ft {

// ft::advance

template <class _InputIter>
void __advance(_InputIter& __i,
               typename iterator_traits<_InputIter>::difference_type __n,
               std::input_iterator_tag) {
  for (; 0 < __n; --__n) {
    ++__i;
  }
}

template <class _BiDirIter>
void __advance(_BiDirIter& __i,
               typename iterator_traits<_BiDirIter>::difference_type __n,
               std::bidirectional_iterator_tag) {
  if (0 <= __n) {
    for (; 0 < __n; --__n) {
      ++__i;
    }
  } else {
    for (; __n < 0; ++__n) {
      --__i;
    }
  }
}

template <class _RandIter>
void __advance(_RandIter& __i,
               typename iterator_traits<_RandIter>::difference_type __n,
               std::random_access_iterator_tag) {
  __i += __n;
}

template <class _InputIter>
void advance(_InputIter& __i,
             typename ft::iterator_traits<_InputIter>::difference_type __n) {
  ft::__advance(__i, __n, typename ft::iterator_traits<_InputIter>::iterator_category());
}

// ft::distance

template <class _InputIter>
typename ft::iterator_traits<_InputIter>::difference_type
__distance(_InputIter __first, _InputIter __last, std::input_iterator_tag) {
  typename ft::iterator_traits<_InputIter>::difference_type __r = 0;
  for (; __first != __last; ++__first) {
    ++__r;
  }
  return __r;
}

template <class _RandIter>
typename ft::iterator_traits<_RandIter>::difference_type
__distance(_RandIter __first, _RandIter __last, std::random_access_iterator_tag) {
  return __last - __first;
}

template <class _InputIter>
typename ft::iterator_traits<_InputIter>::difference_type
distance(_InputIter __first, _InputIter __last) {
  return ft::__distance(__first, __last, typename ft::iterator_traits<_InputIter>::iterator_category());
}

// ft::reverse_iterator

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

  /*
  ** Q: Why don't we need to use enable_if and is_convertible to check if _Up is the valid type?
  ** A: reverse_iterator uses iterator_traits and if _Up is not appropriate,
  ** they don't have proper iterator traits. Therefore, no need to check if _Up is
  ** proper iterators
  **
  ** Q: What happens when initiating reverse_iterator with input_iterator?
  ** A: Can be initiated but not fully operational as increments and decrements of
  ** iterators are not supported
  */

  template <class _Up>
  reverse_iterator(const reverse_iterator<_Up>& __u) : current(__u.base()) {}

  ~reverse_iterator() {}

  template <class _Up>
  reverse_iterator& operator=(const reverse_iterator<_Up>& __u) {
    current = __u.base();
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

/*
** Q: Why do we need to specify 2 template parameters (i.e., _Iter1 and _Iter2)?
** A: To enable comparison between const iterator and non-const iterator
*/

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

template <class _Iter1, class _Iter2>
typename reverse_iterator<_Iter1>::difference_type
operator-(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y) {
  return __y.base() - __x.base();
}

}

#endif

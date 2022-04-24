#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory> // for allaocator, allocator_traits
#include <iterator> // for reverse_iterator, distance
#include <algorithm> // for copy, min
#include <stdexcept> // for out_of_range and length_error
#include "iterator.hpp"
#include "vector_iterator.hpp"
#include "iterator_traits.hpp"
#include "type_traits.hpp"
#include "algorithm.hpp"

namespace ft {

template <class _Tp, class _Allocator = std::allocator<_Tp> >
class vector {

 public:
  typedef _Tp                                           value_type;
  typedef _Allocator                                    allocator_type;
  typedef typename allocator_type::reference            reference;
  typedef typename allocator_type::const_reference      const_reference;
  typedef typename allocator_type::size_type            size_type;
  typedef typename allocator_type::difference_type      difference_type;
  typedef typename allocator_type::pointer              pointer;
  typedef typename allocator_type::const_pointer         const_pointer;
  typedef typename ft::vector_iterator<pointer>         iterator;
  typedef typename ft::vector_iterator<const_pointer>   const_iterator;
  typedef typename ft::reverse_iterator<iterator>       reverse_iterator;
  typedef typename ft::reverse_iterator<const_iterator> const_reverse_iterator;

 private:
  pointer __begin_;
  pointer __end_;
  pointer __end_cap_;
  allocator_type __alloc_;

 public:

  // Default constructor

  vector()
    : __begin_(NULL), __end_(NULL), __end_cap_(NULL), __alloc_() {}

  explicit vector(const allocator_type& __a)
    : __begin_(NULL), __end_(NULL), __end_cap_(NULL), __alloc_(__a) {}


  // Fill constructor

  explicit vector(size_type __n) {
    if (0 < __n) {
      __allocate(__n);
      __end_ = __construct_to_fill(__begin_, __n, value_type());
    }
  }

  vector(size_type __n, const_reference __x) {
    if (0 < __n) {
      __allocate(__n);
      __end_ = __construct_to_fill(__begin_, __n, __x);
    }
  }

  vector(size_type __n, const_reference __x, const allocator_type& __a)
    : __alloc_(__a) {
    if (0 < __n) {
      __allocate(__n);
      __end_ = __construct_to_fill(__begin_, __n, __x);
    }
  }

  // Range constructor

  template <class _Iterator>
  vector(_Iterator __first,
         typename ft::enable_if<!ft::is_integral<_Iterator>::value, _Iterator>::type __last)
    : __begin_(NULL), __end_(NULL), __end_cap_(NULL), __alloc_() {
    insert(begin(), __first, __last);
  }

  template <class _Iterator>
  vector(_Iterator __first, _Iterator __last, const allocator_type& __a,
         typename ft::enable_if<!ft::is_integral<_Iterator>::value, _Iterator>::type*)
    : __begin_(NULL), __end_(NULL), __end_cap_(NULL), __alloc_(__a) {
    insert(begin(), __first, __last);
  }

  // Copy constructor

  vector(const vector& __x) : __alloc_(__x.__alloc_) {
    size_type __n = __x.size();
    if (0 < __n) {
      __allocate(__n);
      __end_ = __construct_to_copy(__x.begin(), __x.end(), __begin_);
    }
  }

  // Destructor

  virtual ~vector() {
    if (__begin_ != NULL) {
      clear();
      __deallocate();
    }
  }

  // Assignation operator

  vector& operator=(const vector& __x) {
    if (this != &__x) {
      if (__x.size() == 0) {
        clear();
        __deallocate();
      } else if (__x.size() <= size()) {
        pointer __new_end = std::copy(__x.begin(), __x.end(), __begin_);
        __destroy(__new_end, __end_);
        __end_ = __begin_ + __x.size();
      } else if (__x.size() <= capacity()) {
        const_iterator __s = __x.begin() + size();
        std::copy(__x.begin(), __s, __begin_);
        __end_ = __construct_to_copy(__s, __x.end(), __end_);
      } else {
        clear();
        __deallocate();
        __allocate(__x.size());
        __end_ = __construct_to_copy(__x.begin(), __x.end(), __begin_);
      }
    }
    return *this;
  }

  // Iterators

  iterator begin() {
    return iterator(__begin_);
  }

  const_iterator begin() const {
    return const_iterator(__begin_);
  }

  iterator end() {
    return iterator(__end_);
  }

  const_iterator end() const {
    return const_iterator(__end_);
  }

  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() {
    return reverse_iterator(begin());
  }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  // Capacity

  size_type size() const {
    return static_cast<size_type>(__end_ - __begin_);
  }

  size_type max_size() const {
    return std::min<size_type>(__alloc_.max_size(),
                               std::numeric_limits<difference_type>::max());
  }

  void resize(size_type __sz) {
    resize(__sz, value_type());
  }

  void resize(size_type __sz, const_reference __x) {
    size_type __cs = size();
    if (__cs < __sz) {
      insert(end(), __sz - __cs, __x);
    } else if (__sz < __cs) {
      erase(begin() + __sz, end());
    }
  }

  size_type capacity() const {
    return static_cast<size_type>(__end_cap_ - __begin_);
  }

  bool empty() const {
    return __begin_ == __end_;
  }

  void reserve(size_type __n) {
    if (max_size() < __n) {
      __throw_length_error();
    } else if (capacity() < __n) {
      pointer __new_begin = __alloc_.allocate(__n);
      try {
        __construct_to_copy(begin(), end(), __new_begin);
      } catch (...) {
        __alloc_.deallocate(__new_begin, __n);
        throw;
      }
      if (__begin_ != NULL) {
        __destroy(__begin_, __end_);
        __alloc_.deallocate(__begin_, capacity());
      }
      __end_cap_ = __new_begin + __n;
      __end_ = __new_begin + size();
      __begin_ = __new_begin;
    }
  }

  // Element access

  reference operator [](size_type __n) {
    return __begin_[__n];
  }

  const_reference operator [](size_type __n) const {
    return __begin_[__n];
  }

  reference at(size_type __n) {
    if (size() <= __n) {
      __throw_out_of_range();
    }
    return __begin_[__n];
  }

  const_reference at(size_type __n) const {
    if (size() <= __n) {
      __throw_out_of_range();
    }
    return __begin_[__n];
  }

  reference front() {
    return *__begin_;
  }

  const_reference front() const {
    return *__begin_;
  }

  reference back() {
    return *(__end_ - 1);
  }

  const_reference back() const {
    return *(__end_ - 1);
  }

  // Modifiers

  template <class _Iterator>
  void assign(_Iterator __first,
              typename ft::enable_if<!ft::is_integral<_Iterator>::value, _Iterator>::type __last) {
    erase(begin(), end());
    insert(begin(), __first, __last);
  }

  void assign(size_type __n, const_reference __u) {
    erase(begin(), end());
    insert(begin(), __n, __u);
  }

  void push_back(const_reference __x) { insert(end(), __x); }

  void pop_back() { erase(end() - 1); }

  iterator insert(iterator __position, const_reference __x) {
    size_type __off = size() == 0 ? 0 : __position - begin();
    insert(__position, static_cast<size_type>(1), __x);
    return begin() + __off;
  }

  void insert(iterator __position, size_type __n, const_reference __x) {
    size_type __cap = capacity();
    size_type __s = size();
    size_type __ms = max_size();
    if (__n == 0) {
      ;
    } else if (__ms - __s < __n) {
      __throw_length_error();
    } else if (__cap < __s + __n) {
      __cap = __ms - __cap / 2 < __cap ? 0 : __cap + __cap / 2;
      if (__cap < __s + __n) {
        __cap = __s + __n;
      }
      pointer __new_begin = __alloc_.allocate(__cap);
      pointer __tmp;
      try {
        __tmp = __construct_to_copy(begin(), __position, __new_begin);
        __tmp = __construct_to_fill(__tmp, __n, __x);
        __construct_to_copy(__position, end(), __tmp);
      } catch (...) {
        __destroy(__new_begin, __tmp);
        __alloc_.deallocate(__new_begin, __cap);
        throw;
      }
      if (__begin_ != NULL) {
        __destroy(__begin_, __end_);
        __alloc_.deallocate(__begin_, capacity());
      }
      __end_cap_ = __new_begin + __cap;
      __end_ = __new_begin + __s + __n;
      __begin_ = __new_begin;
    } else if (static_cast<size_type>(end() - __position) < __n) {
      __construct_to_copy(__position, end(), &(*__position) + __n);
      try {
        __construct_to_fill(__end_, __n - static_cast<size_type>(end() - __position), __x);
      } catch (...) {
        __destroy(&(*__position) + __n, __end_ + __n);
        throw;
      }
      __end_ += __n;
      std::fill(__position, end() - __n, __x);
    } else {
      iterator __old_end = end();
      __end_ = __construct_to_copy(__old_end - __n, __old_end, __end_);
      std::copy_backward(__position, __old_end - __n, __old_end);
      std::fill(__position, __position + __n, __x);
    }
  }

  // TODO: Why input iterators need to be handled separately?
  // TODO: implement is_convertible

  template <class _Iterator>
  void insert(iterator __position, _Iterator __first,
              typename ft::enable_if<!ft::is_integral<_Iterator>::value, _Iterator>::type __last) {
      __insert_range(__position, __first, __last,
                     typename ft::iterator_traits<_Iterator>::iterator_category());
  }

//  template <class _Iterator>
//  void insert(iterator __position, _Iterator __first,
//              typename ft::enable_if<!ft::is_integral<_Iterator>::value, _Iterator>::type __last) {
//    typedef typename ft::iterator_traits<_Iterator>::iterator_category __iterator_category;
//    if (typeid(__iterator_category) == typeid(std::input_iterator_tag)) {
//      __insert_input_range(__position, __first, __last);
//    } else if (typeid(__iterator_category) == typeid(std::forward_iterator_tag)
//               || typeid(__iterator_category) == typeid(std::bidirectional_iterator_tag)
//               || typeid(__iterator_category) == typeid(std::random_access_iterator_tag)) {
//      __insert_forward_range(__position, __first, __last);
//    }
//  }

  iterator erase(iterator __position) {
    std::copy(__position + 1, end(), __position);
    __destroy(__end_ - 1, __end_);
    --__end_;
    return __position;
  }

  iterator erase(iterator __first, iterator __last) {
    if (__first != __last) {
      pointer __new_end = std::copy(__last, end(), __first);
      __destroy(__new_end, __end_);
      __end_ = __new_end;
    }
    return __first;
  }

  // Need to check what happens if 2 vectors with different allocators are swapped

  void swap(vector& __x) {
    if (__alloc_ == __x.__alloc_) {
      std::swap(__begin_, __x.__begin_);
      std::swap(__end_, __x.__end_);
      std::swap(__end_cap_, __x.__end_cap_);
    } else {
      vector tmp = *this;
      *this = __x;
      __x = tmp;
    }
  }

  void clear() {
    __destroy(__begin_, __end_);
  }

  // Allocator

  allocator_type get_allocator() const {
    return __alloc_;
  }

 private :

  // Helper functions

  void __allocate(size_type __n) {
    if (__n > max_size()) {
      __throw_length_error();
    }
    __begin_ = __end_ = __alloc_.allocate(__n);
    __end_cap_ = __begin_ + __n;
  }

  void __deallocate() {
    if (__begin_ != NULL) {
      clear();
      __alloc_.deallocate(__begin_, capacity());
      __begin_ = __end_ = __end_cap_ = NULL;
    }
  }

  void __destroy(pointer __begin, pointer __end) {
    for (; __begin != __end; ++__begin) {
      __alloc_.destroy(__begin);
    }
  }

  pointer __construct_to_fill(pointer __end, size_type __n, const_reference __x) {
    pointer __old_end = __end;
    try {
      for (; 0 < __n; --__n, ++__end) {
        __alloc_.construct(__end, __x);
      }
    } catch (...) {
      __destroy(__old_end, __end);
      throw;
    }
    return __end;
  }

  template <class _ForwardIterator>
  pointer __construct_to_copy(_ForwardIterator __first, _ForwardIterator __last, pointer __end) {
    pointer __old_end = __end;
    try {
      for (; __first != __last; ++__end, ++__first) {
        __alloc_.construct(__end, *__first);
      }
    } catch (...) {
      __destroy(__old_end, __end);
      throw;
    }
    return __end;
  }

  void __throw_length_error() const {
    throw std::length_error("vector");
  }

  void __throw_out_of_range() const {
    throw std::out_of_range("vector");
  }

  template <class _Iterator>
  void __insert_range(iterator __position, _Iterator __first, _Iterator __last,
                      std::input_iterator_tag) {
    for (; __first != __last; ++__first, ++__position) {
      __position = insert(__position, *__first);
    }
  }

  template <class _Iterator>
  void __insert_range(iterator __position, _Iterator __first, _Iterator __last,
                      std::forward_iterator_tag) {
    difference_type __n = std::distance(__first, __last);
    size_type __cap = capacity();
    size_type __ms = max_size();
    size_type __s = size();
    if (__n == 0) {
      ;
    } else if (__ms - __s < static_cast<size_type>(__n)) {
      __throw_length_error();
    } else if (__cap < __s + __n) {
      __cap = __ms - __cap / 2 < __cap ? 0 : __cap + __cap / 2;
      if (__cap < __s + __n) {
        __cap = __s + __n;
      }
      pointer __new_begin = __alloc_.allocate(__cap);
      pointer __tmp;
      try {
        __tmp = __construct_to_copy(begin(), __position, __new_begin);
        __tmp = __construct_to_copy(__first, __last, __tmp);
        __construct_to_copy(__position, end(), __tmp);
      } catch (...) {
        __destroy(__new_begin, __tmp);
        __alloc_.deallocate(__new_begin, __cap);
        throw;
      }
      if (__begin_ != NULL) {
        __destroy(__begin_, __end_);
        __alloc_.deallocate(__begin_, capacity());
      }
      __end_cap_ = __new_begin + __cap;
      __end_ = __new_begin + __s + __n;
      __begin_ = __new_begin;
    } else if (end() - __position < __n) {
      __construct_to_copy(__position, end(), &(*__position) + __n);
      _Iterator __middle = __first;
      std::advance(__middle, end() - __position);
      try {
        __construct_to_copy(__middle, __last, __end_);
      } catch (...) {
        __destroy(&(*__position) + __n, __end_ + __n);
        throw;
      }
      __end_ += __n;
      std::copy(__first, __middle, __position);
    } else if (0 < __n) {
      iterator __old_end = end();
      __end_ = __construct_to_copy(__old_end - __n, __old_end, __end_);
      std::copy_backward(__position, __old_end - __n, __old_end);
      std::copy(__first, __last, __position);
    }
  }

}; // class vector

// Non-member functions

template <class _Tp, class _Allocator>
bool operator==(const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y) {
  const typename vector<_Tp, _Allocator>::size_type __sz = __x.size();
  return __sz == __y.size() && ft::equal(__x.begin(), __x.end(), __y.begin());
}

template <class _Tp, class _Allocator>
bool operator!=(const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y) {
  return !(__x == __y);
}

template <class _Tp, class _Allocator>
bool operator< (const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y) {
  return ft::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
}

template <class _Tp, class _Allocator>
bool operator> (const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y) {
  return __y < __x;
}

template <class _Tp, class _Allocator>
bool operator>=(const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y) {
  return !(__x < __y);
}

template <class _Tp, class _Allocator>
bool operator<=(const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y) {
  return !(__y < __x);
}

template <class _Tp, class _Allocator>
void swap(vector<_Tp, _Allocator>& __x, vector<_Tp, _Allocator>& __y) {
  __x.swap(__y);
}

}

#endif

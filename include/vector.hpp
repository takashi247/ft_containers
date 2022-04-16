#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory> // for allaocator, allocator_traits
#include <iterator> // for reverse_iterator, distance
#include <algorithm> // for copy
#include <stdexcept> // for out_of_range
#include "iterator.hpp"

namespace ft {

template <class T, class Allocator = std::allocator<T> >
class vector {

 public :
  typedef T value_type;
  typedef T* pointer;
  typedef std::allocator<value_type> allocator_type;
  typedef const pointer const_pointer;
  typedef value_type & reference;
  typedef const value_type & const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

  // Constructor

  explicit vector(const allocator_type& alloc = allocator_type())
    : first_(NULL), last_(NULL), reserved_last_(NULL), alloc_(alloc) {}

  explicit vector(size_type n, const_reference val = value_type(),
    const allocator_type& alloc = allocator_type())
    : first_(NULL), last_(NULL), reserved_last_(NULL), alloc_(alloc) {
    resize(n, val);
  }

  template <class InputIterator>
  vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
    : first_(NULL), last_(NULL), reserved_last_(NULL), alloc_(alloc) {
    reserve(std::distance(first, last));
    for (pointer i = first; i != last; ++i) {
      push_back(*i);
    }
  }

  vector(const vector& x) : first_(NULL), last_(NULL), reserved_last_(NULL), alloc_(x.alloc) {
    reserve(x.size());
    for (pointer dest = first_, src = x.begin(), last = x.end();
         src != last_; ++dest, ++src) {
      construct(dest, *src);
    }
    last_ = first_ + x.size();
  }

  // Destructor

  virtual ~vector() {
    clear();
    deallocate();
  }

  // Assignation operator

  vector& operator=(const vector& x) {
    if (this != &x) {
      if (size() == x.size()) {
        std::copy(x.begin(), x.end(), begin());
      } else {
        if (capacity() >= x.size()) {
          std::copy(x.begin(), x.begin() + x.size(), begin());
          for (iterator src_iter = x.begin() + x.size(), src_end = x.end();
               src_iter != src_end; ++src_iter, ++last_) {
            construct(last_, *src_iter);
          }
        } else {
          clear();
          reserve(x.size());
          for (iterator src_iter = x.begin(), src_end = x.end(), dest_iter = begin();
               src_iter != src_end; ++src_iter, ++last_) {
            construct(dest_iter, *src_iter);
          }
        }
      }
    }
    return *this;
  }

  // Iterators
  iterator begin() {
    return first_;
  }

  iterator end() {
    return last_;
  }

  iterator begin() const {
    return first_;
  }

  iterator end() const {
    return last_;
  }

  reverse_iterator rbegin() {
    return reverse_iterator(last_);
  }

  reverse_iterator rend() {
    return reverse_iterator(first_);
  }

  reverse_iterator rbegin() const {
    return reverse_iterator(last_);
  }

  reverse_iterator rend() const {
    return reverse_iterator(first_);
  }

  // Capacity

  size_type size() const {
    return end() - begin();
  }

  size_type max_size() const {
    size_type alloc_max = alloc_.max_size();
    size_type diff_max = std::numeric_limits<difference_type>::max();
    if (alloc_max <= diff_max) {
      return alloc_max;
    } else {
      return diff_max;
    }
  }

  void resize(size_type n, value_type val = value_type()) {
    size_type current_size = size();
    if (n < current_size) {
      size_type diff = current_size - n;
      destroy_until(rbegin() + diff);
      last_ = first_ + n;
    }
    else if (n > current_size) {
      reserve(n);
      for (; last_ != reserved_last_; ++last_) {
        construct(last_, val);
      }
    }
  }

  size_type capacity() const {
    return reserved_last_ - first_;
  }

  bool empty() const {
    return begin() == end();
  }

  void reserve(size_type n) {
    if (n <= capacity()) {
      return ;
    }
    pointer ptr = allocate(n);
    pointer old_first = first_;
    pointer old_last = last_;
    size_type old_capacity = capacity();
    first_ = ptr;
    last_ = first_;
    reserved_last_ = first_ + n;
    for (pointer old_iter = old_first; old_iter != old_last; ++old_iter, ++last_) {
      construct(last_, *old_iter);
    }
    for (reverse_iterator riter = reverse_iterator(old_last),
        rend = reverse_iterator(old_first); riter != rend; ++riter) {
      destroy(&*riter);
    }
    traits::deallocate(alloc_, old_first, old_capacity);
  }

  // Element access

  reference operator [](size_type i) {
    return first_[i];
  }

  const_reference operator [](size_type i) const {
    return first_[i];
  }

  reference at(size_type i) {
    if (i >= size()) {
      throw std::out_of_range("index is out of range.");
    }
    return first_[i];
  }

  const_reference at(size_type i) const {
    if (i >= size()) {
      throw std::out_of_range("index is out of range.");
    }
    return first_[i];
  }

  reference front() {
    return *first_;
  }

  reference back() {
    return *(last_ - 1);
  }

  const_reference front() const {
    return *first_;
  }

  const_reference back() const {
    return *(last_ - 1);
  }

  // Modifiers

  void push_back(const_reference val) {
    size_type sz = size();
    if (sz + 1 > capacity()) {
      if (sz == 0) {
        sz = 1;
      } else {
        sz *= 2;
      }
      reserve(sz);
    }
    construct(last_, val);
    ++last_;
  }

  void clear() {
    destroy_until(rend());
  }

 private :
  typedef std::allocator_traits<allocator_type> traits;

  // Helper functions

  pointer allocate(size_type n) {
    return traits::allocate(alloc_, n);
  }

  void deallocate() {
    traits::deallocate(alloc_, first_, capacity());
  }

  void construct(pointer ptr, const_reference value = value_type()) {
    traits::construct(alloc_, ptr, value);
  }

  void destroy(pointer ptr) {
    traits::destroy(alloc_, ptr);
  }

  void destroy_until(reverse_iterator rend) {
    for (reverse_iterator riter = rbegin(); riter != rend; ++riter, --last_) {
      destroy( &*riter);
    }
  }

  pointer first_;
  pointer last_;
  pointer reserved_last_;
  allocator_type alloc_;
};

}

#endif

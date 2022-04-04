#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory> // for allaocator, allocator_traits
#include <iterator> // for reverse_iterator, distance
#include <algorithm> // for copy
#include <stdexcept> // for out_of_range

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
  typedef std::reverse_iterator<iterator> reverse_iterator; // Need to implement ft::reverse_iterator
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator; // Need to implement ft::reverse_iterator

  // Constructor

  explicit vector(const allocator_type& alloc = allocator_type())
    : first(NULL), last(NULL), reserved_last(NULL), alloc(alloc) {}

  explicit vector(size_type n, const_reference val = value_type(),
    const allocator_type& alloc = allocator_type())
    : first(NULL), last(NULL), reserved_last(NULL), alloc(alloc) {
    resize(n, val);
  }

  template <class InputIterator>
  vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
    : first(NULL), last(NULL), reserved_last(NULL), alloc(alloc) {
    reserve(std::distance(first, last));
    for (pointer i = first; i != last; ++i) {
      push_back(*i);
    }
  }

  vector(const vector& x) : first(NULL), last(NULL), reserved_last(NULL), alloc(x.alloc) {
    reserve(x.size());
    for (pointer dest = first, src = x.begin(), last = x.end();
         src != last; ++dest, ++src) {
      construct(dest, *src);
    }
    last = first + x.size();
  }

  // Destructor

  ~vector() {
    clear();
    deallocate();
  }

  // Assignation operator

  vector& operator =(const vector& x) {
    if (this != &x) {
      if (size() == x.size()) {
        std::copy(x.begin(), x.end(), begin());
      } else {
        if (capacity() >= x.size()) {
          std::copy(x.begin(), x.begin() + x.size(), begin());
          for (iterator src_iter = x.begin() + x.size(), src_end = x.end();
               src_iter != src_end; ++src_iter, ++last) {
            construct(last, *src_iter);
          }
        } else {
          clear();
          reserve(x.size());
          for (iterator src_iter = x.begin(), src_end = x.end(), dest_iter = begin();
               src_iter != src_end; ++src_iter, ++last) {
            construct(dest_iter, *src_iter);
          }
        }
      }
    }
    return *this;
  }

  // Iterators
  iterator begin() {
    return first;
  }

  iterator end() {
    return last;
  }

  iterator begin() const {
    return first;
  }

  iterator end() const {
    return last;
  }

  reverse_iterator rbegin() {
    return reverse_iterator(last);
  }

  reverse_iterator rend() {
    return reverse_iterator(first);
  }

  reverse_iterator rbegin() const {
    return reverse_iterator(last);
  }

  reverse_iterator rend() const {
    return reverse_iterator(first);
  }

  // Capacity

  size_type size() const {
    return end() - begin();
  }

  size_type max_size() const {
    // Need to implement
    return size();
  }

  void resize(size_type n, value_type val = value_type()) {
    size_type current_size = size();
    if (n < current_size) {
      size_type diff = current_size - n;
      destroy_until(rbegin() + diff);
      last = first + n;
    }
    else if (n > current_size) {
      reserve(n);
      for (; last != reserved_last; ++last) {
        construct(last, val);
      }
    }
  }

  size_type capacity() const {
    return reserved_last - first;
  }

  bool empty() const {
    return begin() == end();
  }

  void reserve(size_type n) {
    if (n <= capacity()) {
      return ;
    }
    pointer ptr = allocate(n);
    pointer old_first = first;
    pointer old_last = last;
    size_type old_capacity = capacity();
    first = ptr;
    last = first;
    reserved_last = first + n;
    for (pointer old_iter = old_first; old_iter != old_last; ++old_iter, ++last) {
      construct(last, *old_iter);
    }
    for (reverse_iterator riter = reverse_iterator(old_last),
        rend = reverse_iterator(old_first); riter != rend; ++riter) {
      destroy(&*riter);
    }
    traits::deallocate(alloc, old_first, old_capacity);
  }

  // Element access

  reference operator [](size_type i) {
    return first[i];
  }

  const_reference operator [](size_type i) const {
    return first[i];
  }

  reference at(size_type i) {
    if (i >= size()) {
      throw std::out_of_range("index is out of range.");
    }
    return first[i];
  }

  const_reference at(size_type i) const {
    if (i >= size()) {
      throw std::out_of_range("index is out of range.");
    }
    return first[i];
  }

  reference front() {
    return *first;
  }

  reference back() {
    return *(last - 1);
  }

  const_reference front() const {
    return *first;
  }

  const_reference back() const {
    return *(last - 1);
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
    construct(last, val);
    ++last;
  }

  void clear() {
    destroy_until(rend());
  }

 private :
  typedef std::allocator_traits<allocator_type> traits;

  // Helper functions

  pointer allocate(size_type n) {
    return traits::allocate(alloc, n);
  }

  void deallocate() {
    traits::deallocate(alloc, first, capacity());
  }

  void construct(pointer ptr, const_reference value = value_type()) {
    traits::construct(alloc, ptr, value);
  }

  void destroy(pointer ptr) {
    traits::destroy(alloc, ptr);
  }

  void destroy_until(reverse_iterator rend) {
    for (reverse_iterator riter = rbegin(); riter != rend; ++riter, --last) {
      destroy( &*riter);
    }
  }

  pointer first;
  pointer last;
  pointer reserved_last;
  allocator_type alloc;
};
}

#endif

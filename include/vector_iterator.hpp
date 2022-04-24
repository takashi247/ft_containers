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

  vector_iterator(const vector_iterator& __x) : __i_(__x.base()) {}

  virtual ~vector_iterator(){}

  vector_iterator& operator=(const vector_iterator& __x) {
    if (this != &__x) {
      __i_ = __x.__i_;
    }
    return *this;
  }

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
    __i_ -= __n;
    return *this;
  }

  // Should this return a reference of vector_iterator?
  // How does r can be lived outside this function? Is that beyond the scope of r?
  vector_iterator operator+(difference_type __n) const {
    vector_iterator __v(*this);
    __v += __n;
    return __v;
  }

  vector_iterator operator-(difference_type __n) const { return *this + (-__n); }

  difference_type operator-(vector_iterator const& r) const { return __i_ - r.__i_; }

  iterator_type base() const { return __i_; }

  // Note that comparing iterator from different containers is undefined behavior
  // so we don't need to check if they are created from the same container

  // Need to revise by levaraging symmetric relationship
  bool operator<(vector_iterator const& r) const { return __i_ < r.__i_; }
  bool operator<=(vector_iterator const& r) const { return __i_ <= r.__i_; }
  bool operator>(vector_iterator const& r) const { return __i_ > r.__i_; }
  bool operator>=(vector_iterator const& r) const { return __i_ >= r.__i_; }
  bool operator!=(vector_iterator const& r) const { return __i_ != r.__i_; }
  bool operator==(vector_iterator const& r) const { return __i_ == r.__i_; }

};

}

#endif

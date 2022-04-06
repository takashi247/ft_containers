#ifndef VECTOR_ITERATOR_HPP
#define VECTOR_ITERATOR_HPP

#include <iterator>
#include "vector.hpp"

namespace ft {

template <typename T>
class vector_iterator : public std::iterator<std::random_access_iterator_tag, T> {

 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const pointer const_pointer;
  typedef value_type & reference;
  typedef const value_type & const_reference;
  typedef int size_type; // Should it be std::size_t?
  typedef std::ptrdiff_t difference_type;

  // Default constructor
  vector_iterator() : v_ptr_(NULL), i_(0) {};

  // Constructor
  vector_iterator(ft::vector<T>* v_ptr, size_type i) : v_ptr_(v_ptr), i_(i) {};

  vector_iterator(const vector_iterator& other) {
    v_ptr_ = other.v_ptr_;
    i_ = other.i_;
  }

  virtual ~vector_iterator(){};

  vector_iterator& operator =(const vector_iterator& other) {
    if (this != &other) {
      v_ptr_ = other.v_ptr_;
      i_ = other.i_;
    }
    return *this;
  }

  reference operator*() { return (*v_ptr_)[i]; }

  const_reference operator*() const { return (*v_ptr_)[i]; }

  pointer operator->() { return &((*v_ptr_)[i]); }

  const_pointer operator->() const { return &((*v_ptr_)[i]); }

  reference operator[](size_type m) { return (*v)[i + m]; }

  const_reference operator[](size_type m) const { return (*v)[i + m]; }

  vector_iterator& operator++() {
    ++i_;
    return *this;
  }

  vector_iterator operator++(int) {
    vector_iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  vector_iterator& operator--() {
    --i_;
    return *this;
  }

  vector_iterator operator--(int) {
    vector_iterator tmp = *this;
    --(*this);
    return tmp;
  }

  vector_iterator& operator+=(int n) {
    i_ += n;
    return *this;
  }

  vector_iterator& operator-=(int n) {
    i_ -= n;
    return *this;
  }

  // Should this return a reference of vector_iterator?
  // How does r can be lived outside this function? Is that beyond the scope of r?
  vector_iterator operator+(int n) const {
    vector_iterator r(*this);
    return r += n;
  }

  vector_iterator operator-(int n) const {
    vector_iterator r(*this);
    return r -= n;
  }

  difference_type operator-(vector_iterator const& r) const { return i - r.i_; }

  // Note that comparing iterator from different containers is undefined behavior
  // so we don't need to check if they are created from the same container

  // Need to revise by levaraging symmetric relationship
  bool operator<(vector_iterator const& r) const { return i_ < r.i_; }
  bool operator<=(vector_iterator const& r) const { return i_ <= r.i_; }
  bool operator>(vector_iterator const& r) const { return i_ > r.i_; }
  bool operator>=(vector_iterator const& r) const { return i_ >= r.i_; }
  bool operator!=(vector_iterator const& r) const { return i_ != r.i_; }
  bool operator==(vector_iterator const& r) const { return i_ == r.i_; }

 private:
  ft::vector<T>* v_ptr_;
  size_type i_;

};

}

#endif

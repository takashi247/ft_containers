#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "iterator_traits.hpp"

namespace ft {

template <class Iterator>
class reverse_iterator : public std::iterator<
    typename ft::iterator_traits<Iterator>::iterator_category,
    typename ft::iterator_traits<Iterator>::value_type,
    typename ft::iterator_traits<Iterator>::difference_type,
    typename ft::iterator_traits<Iterator>::pointer,
    typename ft::iterator_traits<Iterator>::reference> {

 public:
  // typedef-s
  typedef Iterator iterator_type;
  typedef typename ft::iterator_traits<Iterator>::iterator_category iterator_category;
  typedef typename ft::iterator_traits<Iterator>::value_type value_type;
  typedef typename ft::iterator_traits<Iterator>::difference_type difference_type;
  typedef typename ft::iterator_traits<Iterator>::pointer pointer;
  typedef typename ft::iterator_traits<Iterator>::reference reference;

  // Default constructor
  reverse_iterator() {}

  // Initialization constructor
  explicit reverse_iterator(iterator_type it) : current(it) {}

  // Copy constructor
  template <class Iter>
  reverse_iterator(const reverse_iterator<Iter>& rev_it) : current(rev_it.base()) {}

  iterator_type base() const { return current; }

  reference operator*() const {
    iterator_type tmp = current;
    return *(--tmp);
  }

  // Base iterator must be a random-access iterator
  reverse_iterator operator+(difference_type n) const {
    return reverse_iterator(current - n);
  }

  reverse_iterator& operator++() {
    --current;
    return *this;
  }

  reverse_iterator operator++(int) {
    reverse_iterator tmp = *this;
    --current;
    return tmp;
  }

  // Base iterator must be a random-access iterator
  reverse_iterator& operator+=(difference_type n) {
    current -= n;
    return *this;
  }

  // Base iterator must be a random-access iterator
  reverse_iterator operator-(difference_type n) const {
    return reverse_iterator(current + n);
  }

  reverse_iterator& operator--() {
    ++current;
    return *this;
  }

  reverse_iterator operator--(int) {
    reverse_iterator tmp = *this;
    ++current;
    return tmp;
  }

  // Base iterator must be a random-access iterator
  reverse_iterator& operator-=(difference_type n) {
    current += n;
    return *this;
  }

  pointer operator->() const { return &**this; }

  // Base iterator must be a random-access iterator
  reference operator[](difference_type n) const { return *(*this + n); }

  // helper functions
  bool Eq(const reverse_iterator& rhs) const { return current == rhs.current; }

  bool Lt(const reverse_iterator& rhs) const { return rhs.current < current; }

  difference_type Mi(const reverse_iterator& rhs) const {
    return rhs.current - current;
  }

 protected:
  iterator_type current;
};

// Relational operators

template <class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return lhs.Eq(rhs);
}

template <class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return !(lhs == rhs);
}

template <class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return lhs.Lt(rhs);
}

template <class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return rhs < lhs;
}

template <class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return !(rhs < lhs);
}

template <class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return !(lhs < rhs);
}

template <class Iterator>
reverse_iterator<Iterator> operator+(
    typename reverse_iterator<Iterator>::difference_type n,
    const reverse_iterator<Iterator>& rev_it) {
  return rev_it + n;
}

template <class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(
    const reverse_iterator<Iterator>& lhs,
    const reverse_iterator<Iterator>& rhs) {
  return lhs.Mi(rhs);
}

}

#endif

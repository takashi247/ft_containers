#ifndef MAP_HPP
#define MAP_HPP

#include <functional> // for less
#include <memory> // for allocator

#include "utility.hpp" // for ft::pair
#include "type_traits.hpp" // for __select_first
#include "__tree.hpp"

namespace ft {

template <class _Key, class _Tp, class _Compare = std::less<_Key>,
          class _Allocator = std::allocator<ft::pair<const _Key, _Tp> > >
class map {
 public:

  typedef _Key                                        key_type;
  typedef _Tp                                         mapped_type;
  typedef ft::pair<const key_type, mapped_type>       value_type;
  typedef _Compare                                    key_compare;
  typedef _Allocator                                  allocator_type;
  typedef value_type&                                 reference;
  typedef const value_type&                           const_reference;

  // TODO: Understand why "friend class map" is needed here

  class value_compare
    : public std::binary_function<value_type, value_type, bool> {
    friend class map;
   protected:
    key_compare comp;
    value_compare(key_compare c) : comp(c) {}
   public:
    bool operator()(const value_type& __x, const value_type& __y) const {
      return comp(__x.first, __y.first);
    }
  };

 private:

  typedef ft::__tree<ft::__tree_traits<key_type, value_type,
                                       ft::__select_first<value_type>,
                                       key_compare, allocator_type> > __base;

  __base __tree_;

 public:
  typedef typename allocator_type::pointer                   pointer;
  typedef typename allocator_type::const_pointer             const_pointer;
  typedef typename allocator_type::size_type                 size_type;
  typedef typename allocator_type::difference_type           difference_type;
  typedef typename __base::iterator                          iterator;
  typedef typename __base::const_iterator                    const_iterator;
  typedef typename __base::reverse_iterator                  reverse_iterator;
  typedef typename __base::const_reverse_iterator            const_reverse_iterator;

  map() : __tree_(key_compare(), allocator_type()) {}

  explicit map(const key_compare& __comp)
    : __tree_(__comp, allocator_type()) {}

  explicit map(const allocator_type& __a)
    : __tree_(key_compare(), __a) {}

  map(const key_compare& __comp, const allocator_type& __a)
    : __tree_(__comp, __a) {}

  template <class _InputIterator>
  map(_InputIterator __f, _InputIterator __l,
      const key_compare& __comp = key_compare(),
      const allocator_type& __a = allocator_type())
    : __tree_(__comp, __a) {
    insert(__f, __l);
  }

  map(const map& __m) : __tree_(__m.__tree_) {}

  virtual ~map() {}

  map& operator=(const map& __m) {
    if (this != &__m) {
      __tree_ = __m.__tree_;
    }
    return *this;
  }

  // Iterators

  iterator begin() { return __tree_.begin(); }

  const_iterator begin() const { return __tree_.begin(); }

  iterator end() { return __tree_.end(); }

  const_iterator end() const { return __tree_.end(); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

  reverse_iterator rend() {return reverse_iterator(begin()); }

  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // Capacity

  bool empty() const { return __tree_.size() == 0; }

  size_type size() const { return __tree_.size(); }

  size_type max_size() const { return __tree_.max_size(); }


  // Element access

  mapped_type& operator[](const key_type& __k) {
    iterator __it = insert(value_type(__k, mapped_type())).first;
    return (*__it).second;
  }

  // Modifiers

  ft::pair<iterator, bool> insert(const value_type& __v) {
    return __tree_.insert(__v);
  }

  iterator insert(iterator __it, const value_type& __v) {
    return __tree_.insert(__it, __v);
  }

  template <class _InputIterator>
  void insert(_InputIterator __first, _InputIterator __last) {
    __tree_.insert(__first, __last);
  }

  void erase(iterator __p) {
    __tree_.erase(__p);
  }

  size_type erase(const key_type& __k) {
    return __tree_.erase(__k);
  }

  void erase(iterator __first, iterator __last) {
    __tree_.erase(__first, __last);
  }

  void swap(map& __m) { __tree_.swap(__m.__tree_); }

  void clear() { __tree_.clear(); }

  // Observers

  key_compare key_comp() const { return __tree_.key_comp(); }

  value_compare value_comp() const { return value_compare(__tree_.key_comp()); }

  // Operations

  iterator find(const key_type& __k) { return __tree_.find(__k); }

  const_iterator find(const key_type& __k) const { return __tree_.find(__k); }

  size_type count(const key_type& __k) const { return __tree_.count(__k); }

  iterator lower_bound(const key_type& __k) { return __tree_.lower_bound(__k); }

  const_iterator lower_bound(const key_type& __k) const { return __tree_.lower_bound(__k); }

  iterator upper_bound(const key_type& __k) { return __tree_.upper_bound(__k); }

  const_iterator upper_bound(const key_type& __k) const { return __tree_.upper_bound(__k); }

  ft::pair<const_iterator, const_iterator> equal_range(const key_type& __k) const {
    return __tree_.equal_range(__k);
  }

  ft::pair<iterator, iterator> equal_range(const key_type& __k) {
    return __tree_.equal_range(__k);
  }

  // Allocator

  allocator_type get_allocator() const { return __tree_.get_allocator(); }

};

}

#endif // MAP_HPP

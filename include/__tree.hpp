#ifndef __TREE_HPP
#define __TREE_HPP

#include <iterator> // for bidirectional iterator tag, distance
#include <limits> // for numeric_limits
#include <algorithm> // for min, swap
#include <memory> // for allocator max_size function
#include <stdexcept> // for length_error, out_of_range

#include "iterator.hpp"
#include "utility.hpp"
#include "iterator_traits.hpp"
#include "algorithm.hpp"

namespace ft {

template <class _Key, class _Tp, class _KeyGetter, class _Compare,
          class _Allocator = std::allocator<_Tp> >
struct __tree_traits {

  typedef _Key       key_type;
  typedef _Tp        value_type;
  typedef _KeyGetter key_getter;
  typedef _Compare   key_compare;
  typedef _Allocator allocator_type;

}; // __tree_traits

template <class _TreeTraits>
struct __tree_node {

  typedef typename _TreeTraits::value_type                              value_type;
  typedef typename _TreeTraits::allocator_type                          allocator_type;
  typedef typename allocator_type::template rebind<void>::other::pointer void_pointer;

  struct __node {
    void_pointer __parent_;
    void_pointer __left_;
    void_pointer __right_;
    value_type __value_;
    char __color_;
    bool __isnil_;
  };

};

template <class _TreeTraits>
class __tree {

 public:
  typedef __tree<_TreeTraits>                        tree;
  typedef typename _TreeTraits::key_type             key_type;
  typedef typename _TreeTraits::key_compare          key_compare;
  typedef typename _TreeTraits::value_type           value_type;
  typedef typename _TreeTraits::allocator_type       allocator_type;
  typedef typename _TreeTraits::key_getter           key_getter;
  typedef typename allocator_type::size_type          size_type;
  typedef typename allocator_type::difference_type    difference_type;
  typedef typename allocator_type::pointer            pointer;
  typedef typename allocator_type::const_pointer      const_pointer;
  typedef typename allocator_type::reference          reference;
  typedef typename allocator_type::const_reference    const_reference;

 protected:
  typedef typename __tree_node<_TreeTraits>::void_pointer  void_pointer;
  typedef typename __tree_node<_TreeTraits>::__node        node;

  // TODO: Check enum naming convention

  enum Redbl {
    Red,
    Black
  };

  typedef typename allocator_type::template rebind<node>::other::pointer
    node_pointer;
  typedef typename allocator_type::template rebind<node_pointer>::other::reference
    node_pointer_reference;
  typedef typename allocator_type::template rebind<key_type>::other::const_reference
    key_reference;
  typedef typename allocator_type::template rebind<value_type>::other::reference
    value_reference;

  // Member variables

  node_pointer __head_;
  size_type __size_;
  key_compare __comp_;
  allocator_type __alloc_value_;
  typename allocator_type::template rebind<node>::other __alloc_node_;
  typename allocator_type::template rebind<node_pointer>::other __alloc_node_pointer_;


  // TODO: Why these functions need to be static?

  static char& __color(node_pointer __p) {
    return (*__p).__color_;
  }

  static bool& __isnil(node_pointer __p) {
    return (*__p).__isnil_;
  }

  static value_reference __value(node_pointer __p) {
    return static_cast<value_reference>((*__p).__value_);
  }

  static key_reference __key(node_pointer __p) {
    return key_getter()(__value(__p));
  }

  // TODO: Understand why reinterpret_cast is required here
  // Probably, void pointer needs to be casted by reinterpret_cast

  static node_pointer_reference __parent(node_pointer __p) {
    return reinterpret_cast<node_pointer_reference>((*__p).__parent_);
  }

  static node_pointer_reference __left(node_pointer __p) {
    return reinterpret_cast<node_pointer_reference>((*__p).__left_);
  }

  static node_pointer_reference __right(node_pointer __p) {
    return reinterpret_cast<node_pointer_reference>((*__p).__right_);
  }

 public:

  // iterator class
  // TODO: Check why friend modifier is needed here

  class __tree_iterator;
  friend class __tree_iterator;
  class __tree_iterator : public std::iterator<std::bidirectional_iterator_tag,
                                        value_type,
                                        difference_type,
                                        pointer,
                                        reference> {
   public:
    typedef std::iterator<std::bidirectional_iterator_tag, value_type,
                          difference_type, pointer, reference>             iterator_base;
    typedef typename ft::iterator_traits<iterator_base>::iterator_category iterator_category;
    typedef typename ft::iterator_traits<iterator_base>::value_type        value_type;
    typedef typename ft::iterator_traits<iterator_base>::difference_type   difference_type;
    typedef typename ft::iterator_traits<iterator_base>::pointer           pointer;
    typedef typename ft::iterator_traits<iterator_base>::reference         reference;

    // Member variable
    // TODO: Check if this should be private or protected

    node_pointer __node_ptr_;

    __tree_iterator() : __node_ptr_(NULL) {}

    __tree_iterator(node_pointer __p) : __node_ptr_(__p) {}

    reference operator*() const { return __value(__node_ptr_); }

    pointer operator->() const { return &**this; }

    __tree_iterator& operator++() {
      __increment();
      return *this;
    }

    __tree_iterator operator++(int) {
      __tree_iterator __tmp = *this;
      ++*this;
      return __tmp;
    }

    __tree_iterator& operator--() {
      __decrement();
      return *this;
    }

    __tree_iterator operator--(int) {
      __tree_iterator __tmp = *this;
      --*this;
      return __tmp;
    }

    bool operator==(const __tree_iterator& __x) const {
      return __node_ptr_ == __x.__node_ptr_;
    }

    bool operator!=(const __tree_iterator& __x) const {
      return !(*this == __x);
    }

   private:

    // TODO: Fully understand logics of increment and decrement here
    // TODO: Check if those helper functions should be public

    void __decrement() {
      if (__isnil(__node_ptr_)) {
        __node_ptr_ = __right(__node_ptr_);
      } else if (!__isnil(__left(__node_ptr_))) {
        __node_ptr_ = __max(__left(__node_ptr_));
      } else {
        node_pointer __p = __parent(__node_ptr_);
        while (!__isnil(__p) && __node_ptr_ == __left(__p)) {
          __node_ptr_ = __p;
          __p = __parent(__node_ptr_);
        }
        if (!__isnil(__node_ptr_)) {
          __node_ptr_ = __p;
        }
      }
    }

    void __increment() {
      if (__isnil(__node_ptr_)) {
        ;
      } else if (!__isnil(__right(__node_ptr_))) {
        __node_ptr_ = __min(__right(__node_ptr_));
      } else {
        node_pointer __p = __parent(__node_ptr_);
        while (!__isnil(__p) && __node_ptr_ == __right(__p)) {
          __node_ptr_ = __p;
          __p = __parent(__node_ptr_);
        }
        __node_ptr_ = __p;
      }
    }

    friend class __tree_const_iterator;

  }; // __tree_iterator class


  // __tree_const_iterator class

  class __tree_const_iterator;
  friend class __tree_const_iterator;
  class __tree_const_iterator : public std::iterator<std::bidirectional_iterator_tag,
                                        value_type,
                                        difference_type,
                                        const_pointer,
                                        const_reference> {

   public:
    typedef std::iterator<std::bidirectional_iterator_tag, value_type,
                          difference_type, const_pointer, const_reference> iterator_base;
    typedef typename ft::iterator_traits<iterator_base>::iterator_catetory iterator_category;
    typedef typename ft::iterator_traits<iterator_base>::value_type        value_type;
    typedef typename ft::iterator_traits<iterator_base>::difference_type   difference_type;
    typedef typename ft::iterator_traits<iterator_base>::pointer           pointer;
    typedef typename ft::iterator_traits<iterator_base>::reference         reference;

    // Member variable
    // TODO: Check if this should be private or protected

    node_pointer __node_ptr_;

    __tree_const_iterator() : __node_ptr_(NULL) {}

    __tree_const_iterator(node_pointer __p) : __node_ptr_(__p) {}

    __tree_const_iterator(const typename __tree<_TreeTraits>::__tree_iterator& __x)
      : __node_ptr_(__x.__node_ptr_) {}

    const_reference operator*() const { return __value(__node_ptr_); }

    const_pointer operator->() const { return &**this; }

    __tree_const_iterator& operator++() {
      __increment();
      return *this;
    }

    __tree_const_iterator operator++(int) {
      __tree_const_iterator __tmp = *this;
      ++*this;
      return __tmp;
    }

    __tree_const_iterator& operator--() {
      __decrement();
      return *this;
    }

    __tree_const_iterator operator--(int) {
      __tree_const_iterator __tmp = *this;
      --*this;
      return __tmp;
    }

    bool operator==(const __tree_const_iterator& __x) const {
      return __node_ptr_ == __x.__node_ptr_;
    }

    bool operator!=(const __tree_const_iterator& __x) const {
      return !(*this == __x);
    }

   private:

    // TODO: Check if this __decrement implementation causes any issues

    void __decrement() {
      if (__isnil(__node_ptr_)) {
        ;
      } else if (!__isnil(__left(__node_ptr_))) {
        __node_ptr_ = __max(__left(__node_ptr_));
      } else {
        node_pointer __p = __parent(__node_ptr_);
        while (!__isnil(__p) && __node_ptr_ == __left(__p)) {
          __node_ptr_ = __p;
          __p = __parent(__node_ptr_);
        }
        __node_ptr_ = __p;
      }
    }

    void __increment() {
      if (__isnil(__node_ptr_)) {
        ;
      } else if (!__isnil(__right(__node_ptr_))) {
        __node_ptr_ = __min(__right(__node_ptr_));
      } else {
        node_pointer __p = __parent(__node_ptr_);
        while (!__isnil(__p) && __node_ptr_ == __right(__p)) {
          __node_ptr_ = __p;
          __p = __parent(__node_ptr_);
        }
        __node_ptr_ = __p;
      }
    }

  }; // __tree_const_iterator

  typedef __tree_iterator                                        iterator;
  typedef __tree_const_iterator                                  const_iterator;
  typedef ft::reverse_iterator<iterator>                         reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>                   const_reverse_iterator;
  typedef ft::pair<__tree_iterator, bool>                        pair_ib;
  typedef ft::pair<__tree_iterator, __tree_iterator>             pair_ii;
  typedef ft::pair<__tree_const_iterator, __tree_const_iterator> pair_cc;

  __tree(const key_compare& __comp, const allocator_type& __a)
    : __comp_(__comp), __alloc_value_(__a), __alloc_node_(__a), __alloc_node_pointer_(__a) {
    __init();
  }

  __tree(const value_type *__first, const value_type *__last,
         const key_compare& __comp, const allocator_type& __a)
    : __comp_(__comp), __alloc_value_(__a), __alloc_node_(__a), __alloc_node_pointer_(__a) {
    __init();
    insert(__first, __last);
  }

  __tree(const tree& __t)
    : __comp_(__t.key_comp()), __alloc_value_(__t.get_allocator()),
      __alloc_node_(__t.get_allocator()), __alloc_node_pointer_(__t.get_allocator()) {
    __init();
    __copy(__t);
  }

  virtual ~__tree() {
    erase(begin(), end());
    __freenode(__head_);
    __head_ = NULL;
    __size_ = 0;
  }

  tree& operator=(const tree& __t) {
    if (this != &__t) {
      erase(begin(), end());
      __copy(__t);
    }
    return *this;
  }

  iterator begin() { return iterator(__lmost()); }

  const_iterator begin() const { return const_iterator(__lmost()); }

  iterator end() { return iterator(__head_); }

  const_iterator end() const { return const_iterator(__head_); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  size_type size() const { return __size_; }

  size_type max_size() const {
    return std::min<size_type>(
           __alloc_value_.max_size(),
           std::numeric_limits<difference_type>::max());
  }

  bool empty() const { return size() == 0; }

  allocator_type get_allocator() const { return __alloc_value_; }

  key_compare key_comp() const { return __comp_; }

  // First, identify the point where __v should be added
  // Next, check if __v already exists in the tree

  pair_ib insert(const value_type& __v) {
    node_pointer __x = __root();
    node_pointer __y = __head_;
    bool __add_left = true;
    while (!__isnil(__x)) {
      __y = __x;
      __add_left = __comp_(key_getter()(__v), __key(__x));
      __x = __add_left ? __left(__x) : __right(__x);
    }
    iterator __it = iterator(__y);
    if (!__add_left) {
      ;
    } else if (__it == begin()) {
      return pair_ib(__insert(true, __y, __v), true);
    } else {
      --__it;
    }
    if (__comp_(__key(__it.__node_ptr_), key_getter()(__v))) {
      return pair_ib(__insert(__add_left, __y, __v), true);
    } else {
      return pair_ib(__it, false);
    }
  }

  // First, try to insert __v using __it as a hint
  // If that does not work, insert using the implementation above

  iterator insert(iterator __it, const value_type& __v) {
    if (size() == 0) {
      return __insert(true, __head_, __v);
    } else if (__it == begin()) {
      if (__comp_(key_getter()(__v), __key(__it.__node_ptr_))) {
        return __insert(true, __it.__node_ptr_, __v);
      }
    } else if (__it == end()) {
      if (__comp_(__key(__rmost()), key_getter()(__v))) {
        return __insert(false, __rmost(), __v);
      }
    } else {
      iterator __it_prev = __it;
      --__it_prev;
      if (__comp_(__key(__it_prev.__node_ptr_), key_getter()(__v))
          && __comp_(key_getter()(__v), __key(__it.__node_ptr_))) {
        if (__isnil(__right(__it_prev.__node_ptr_))) {
          return __insert(false, __it_prev.__node_ptr_, __v);
        } else {
          return __insert(true, __it.__node_ptr_, __v);
        }
      }
    }
    return insert(__v).first;
  }

  template <class _Iterator>
  void insert(_Iterator __first, _Iterator __last) {
    for (; __first != __last; ++__first) {
      insert(*__first);
    }
  }

  void erase(iterator __p) {
    if (__isnil(__p.__node_ptr_)) {
      throw std::out_of_range("map/set<T> iterator");
    }
    node_pointer __target = __p.__node_ptr_;
    node_pointer __target_copy = __target;
    node_pointer __replace;
    node_pointer __target_parent;
    if (__isnil(__left(__target))) {
      __replace = __right(__target);
    } else if (__isnil(__right(__target))) {
      __replace = __left(__target);
    } else {
      __target = __min(__right(__target));
      __replace = __right(__target);
    }
    if (__target == __target_copy) { // __p has no children or only one child
      __target_parent = __parent(__target_copy);
      if (!__isnil(__replace)) {
        __parent(__replace) = __target_parent;
      }
      if (__root() == __target_copy) {
        __root() = __replace;
      } else if (__left(__target_parent) == __target_copy) {
        __left(__target_parent) = __replace;
      } else {
        __right(__target_parent) = __replace;
      }
      if (__lmost() != __target_copy) {
        ;
      } else if (__isnil(__replace)) {
        __lmost() = __target_parent;
      } else {
        __lmost() = __min(__replace);
      }
      if (__rmost() != __target_copy) {
        ;
      } else if (__isnil(__replace)) {
        __rmost() = __target_parent;
      } else {
        __rmost() = __max(__replace);
      }
    } else { // __p has both children
      __parent(__left(__target_copy)) = __target;
      __left(__target) = __left(__target_copy);
      if (__target == __right(__target_copy)) {
        __target_parent = __target;
      } else {
        __target_parent = __parent(__target);
        if (!__isnil(__replace)) {
          __parent(__replace) = __target_parent;
        }
        __left(__target_parent) = __replace;
        __right(__target) = __right(__target_copy);
        __parent(__right(__target_copy)) = __target;
      }
      if (__root() == __target_copy) {
        __root() = __target;
      } else if (__left(__parent(__target_copy)) == __target_copy) {
        __left(__parent(__target_copy)) = __target;
      } else {
        __right(__parent(__target_copy)) = __target;
      }
      __parent(__target) = __parent(__target_copy);
      std::swap(__color(__target), __color(__target_copy));
    }
    if (__color(__target_copy) == Black) {
      for (; __replace != __root() && __color(__replace) == Black; __target_parent = __parent(__replace)) {
        if (__replace == __left(__target_parent)) {
          node_pointer __replace_sib = __right(__target_parent);
          if (__color(__replace_sib) == Red) {
            __color(__replace_sib) = Black;
            __color(__target_parent) = Red;
            __lrotate(__target_parent);
            __replace_sib = __right(__target_parent);
          }
          if (__isnil(__replace_sib)) {
            __replace = __target_parent; // should not happen
          } else if (__color(__left(__replace_sib)) == Black && __color(__right(__replace_sib)) == Black) {
            __color(__replace_sib) = Red;
            __replace = __target_parent;
          } else {
            if (__color(__right(__replace_sib)) == Black) {
              __color(__left(__replace_sib)) = Black;
              __color(__replace_sib) = Red;
              __rrotate(__replace_sib);
              __replace_sib = __right(__target_parent);
            }
            __color(__replace_sib) = __color(__target_parent);
            __color(__target_parent) = Black;
            __color(__right(__replace_sib)) = Black;
            __lrotate(__target_parent);
            break;
          }
        } else {
          node_pointer __replace_sib = __left(__target_parent);
          if (__color(__replace_sib) == Red) {
            __color(__replace_sib) = Black;
            __color(__target_parent) = Red;
            __rrotate(__target_parent);
            __replace_sib = __left(__target_parent);
          }
          if (__isnil(__replace_sib)) {
            __replace = __target_parent; // should not happen
          } else if (__color(__right(__replace_sib)) == Black && __color(__left(__replace_sib)) == Black) {
            __color(__replace_sib) = Red;
            __replace = __target_parent;
          } else {
            if (__color(__left(__replace_sib)) == Black) {
              __color(__right(__replace_sib)) = Black;
              __color(__replace_sib) = Red;
              __lrotate(__replace_sib);
              __replace_sib = __left(__target_parent);
            }
            __color(__replace_sib) = __color(__target_parent);
            __color(__target_parent) = Black;
            __color(__left(__replace_sib)) = Black;
            __rrotate(__target_parent);
            break;
          }
        }
      }
      __color(__replace) = Black;
    }
    __destval(&__value(__target_copy));
    __freenode(__target_copy);
    if (0 < __size_) {
      --__size_;
    }
  }

  void erase(iterator __first, iterator __last) {
    if (size() != 0 && __first == begin() && __last == end()) {
      __erase(__root());
      __root() = __head_;
      __size_ = 0;
      __lmost() = __head_;
      __rmost() = __head_;
    } else {
      while (__first != __last) {
        erase(__first++);
      }
    }
  }

  size_type erase(const key_type& __x) {
    pair_ii __range = equal_range(__x);
    size_type __n = std::distance(__range.first, __range.second);
    erase(__range.first, __range.second);
    return __n;
  }

  void erase(const key_type *__first, const key_type *__last) {
    while (__first != __last) {
      erase(*__first++);
    }
  }

  void clear() { erase(begin(), end()); }

  iterator find(const key_type& __k) {
    iterator __p = lower_bound(__k);
    return (__p == end() || __comp_(__k, __key(__p.__node_ptr_))) ? end() : __p;
  }

  const_iterator find(const key_type& __k) const {
    const_iterator __p = lower_bound(__k);
    return (__p == end() || __comp_(__k, __key(__p.__node_ptr_))) ? end() : __p;
  }

  size_type count(const key_type& __k) const {
    pair_cc __ans = equal_range(__k);
    return std::distance(__ans.first, __ans.second);
  }

  iterator lower_bound(const key_type& __k) {
    return iterator(__lbound(__k));
  }

  const_iterator lower_bound(const key_type& __k) const {
    return const_iterator(__lbound(__k));
  }

  iterator upper_bound(const key_type& __k) {
    return iterator(__ubound(__k));
  }

  const_iterator upper_bound(const key_type& __k) const {
    return const_iterator(__ubound(__k));
  }

  pair_ii equal_range(const key_type& __k) {
    return pair_ii(lower_bound(__k), upper_bound(__k));
  }

  pair_cc equal_range(const key_type& __k) const {
    return pair_cc(lower_bound(__k), upper_bound(__k));
  }

  void swap(tree& __x) {
    if (get_allocator() == __x.get_allocator()) {
      std::swap(__comp_, __x.__comp_);
      std::swap(__head_, __x.__head_);
      std::swap(__size_, __x.__size_);
    } else {
      tree __tmp = *this;
      *this = __x;
      __x = __tmp;
    }
  }

 protected:

  // TODO: Do we need initialization values for construction of left and right?

  node_pointer __buynode(node_pointer __parent_ptr, char __c) {
    node_pointer __s = __alloc_node_.allocate(1);
    __alloc_node_pointer_.construct(&__left(__s));
    __alloc_node_pointer_.construct(&__right(__s));
    __alloc_node_pointer_.construct(&__parent(__s), __parent_ptr);
    __color(__s) = __c;
    __isnil(__s) = false;
    return __s;
  }

  void __consval(pointer __p, const value_type& __v) {
    __alloc_value_.construct(__p, __v);
  }

  void __destval(pointer __p) { __alloc_value_.destroy(__p); }

  void __freenode(node_pointer __s) {
    __alloc_node_pointer_.destroy(&__parent(__s));
    __alloc_node_pointer_.destroy(&__right(__s));
    __alloc_node_pointer_.destroy(&__left(__s));
    __alloc_node_.deallocate(__s, 1);
  }

  // Initiate __head_ pointer that should be black and nill
  // At the initialization, root, lmost, rmost should be pointed at __head_

  void __init() {
    __head_ = __buynode(NULL, Black);
    __isnil(__head_) = true;
    __root() = __head_;
    __lmost() = __head_;
    __rmost() = __head_;
    __size_ = 0;
  }

  node_pointer& __lmost() { return __left(__head_); }

  node_pointer& __lmost() const { return __left(__head_); }

  node_pointer& __rmost() { return __right(__head_); }

  node_pointer& __rmost() const { return __right(__head_); }

  node_pointer& __root() { return __parent(__head_); }

  node_pointer& __root() const { return __parent(__head_); }

  // TODO: Why these functions need to be static?

  static node_pointer __max(node_pointer __p) {
    while (!__isnil(__right(__p))) {
      __p = __right(__p);
    }
    return __p;
  }

  static node_pointer __min(node_pointer __p) {
    while (!__isnil(__left(__p))) {
      __p = __left(__p);
    }
    return __p;
  }

  // TODO: Fully understand logics for helper functions below from here

  node_pointer __lbound(const key_type& __k) const {
    node_pointer __x = __root();
    node_pointer __y = __head_;
    while (!__isnil(__x)) {
      if (__comp_(__key(__x), __k)) {
        __x = __right(__x);
      } else {
        __y = __x;
        __x = __left(__x);
      }
    }
    return __y;
  }

  node_pointer __ubound(const key_type& __k) const {
    node_pointer __x = __root();
    node_pointer __y = __head_;
    while (!__isnil(__x)) {
      if (__comp_(__k, __key(__x))) {
        __y = __x;
        __x = __left(__x);
      } else {
        __x = __right(__x);
      }
    }
    return __y;
  }

  void __lrotate(node_pointer __x) {
    node_pointer __y = __right(__x);
    __right(__x) = __left(__y);
    if (!__isnil(__left(__y))) {
      __parent(__left(__y)) = __x;
    }
    __parent(__y) = __parent(__x);
    if (__x == __root()) {
      __root() = __y;
    } else if (__x == __left(__parent(__x))) {
      __left(__parent(__x)) = __y;
    } else {
      __right(__parent(__x)) = __y;
    }
    __left(__y) = __x;
    __parent(__x) = __y;
  }

  void __rrotate(node_pointer __x) {
    node_pointer __y = __left(__x);
    __left(__x) = __right(__y);
    if (!__isnil(__right(__y))) {
      __parent(__right(__y)) = __x;
    }
    __parent(__y) = __parent(__x);
    if (__x == __root()) {
      __root() = __y;
    } else if (__x == __right(__parent(__x))) {
      __right(__parent(__x)) = __y;
    } else {
      __left(__parent(__x)) = __y;
    }
    __right(__y) = __x;
    __parent(__x) = __y;
  }

  // The parent node of the root node should be __head_ by definition
  // The root node and __head_ point at each other as their parent nodes

  void __copy(const tree& __x) {
    __root() = __copy(__x.__root(), __head_);
    __size_ = __x.size();
    __comp_ = __x.__comp_;
    if (!__isnil(__root())) {
      __lmost() = __min(__root());
      __rmost() = __max(__root());
    } else {
      __lmost() = __head_;
      __rmost() = __head_;
    }
  }

  node_pointer __copy(node_pointer __x, node_pointer __parent_ptr) {
    node_pointer __r = __head_;
    if (!__isnil(__x)) {
      node_pointer __y = __buynode(__parent_ptr, __color(__x));
      try {
        __consval(&__value(__y), __value(__x));
      } catch (...) {
        __freenode(__y);
        __erase(__r);
        throw;
      }
      __left(__y) = __head_;
      __right(__y) = __head_;
      if (__isnil(__r)) {
        __r = __y;
      }
      try {
        __left(__y) = __copy(__left(__x), __y);
        __right(__y) = __copy(__right(__x), __y);
      } catch (...) {
        __erase(__r);
        throw;
      }
    }
    return __r;
  }

  void __erase(node_pointer __x) {
    for (node_pointer __y = __x; !__isnil(__y); __x = __y) {
      __erase(__right(__y));
      __y = __left(__y);
      __destval(&__value(__x));
      __freenode(__x);
    }
  }

  iterator __insert(bool __addleft, node_pointer __y, const value_type& __v) {
    if (max_size() - 1 <= __size_) {
      throw std::length_error("map/set<T> too long");
    }
    node_pointer __z = __buynode(__y, Red);
    __left(__z) = __head_;
    __right(__z) = __head_;
    try {
      __consval(&__value(__z), __v);
    } catch (...) {
      __freenode(__z);
      throw;
    }
    ++__size_;
    if (__y == __head_) {
      __root() = __z;
      __lmost() = __z;
      __rmost() = __z;
    } else if (__addleft) {
      __left(__y) = __z;
      if (__y == __lmost()) {
        __lmost() = __z;
      }
    } else {
      __right(__y) = __z;
      if (__y == __rmost()) {
        __rmost() = __z;
      }
    }
    for (node_pointer __x = __z; __color(__parent(__x)) == Red; ) {
      if (__parent(__x) == __left(__parent(__parent(__x)))) {
        __y = __right(__parent(__parent(__x)));
        if (__color(__y) == Red) {
          __color(__parent(__x)) = Black;
          __color(__y) = Black;
          __color(__parent(__parent(__x))) = Red;
          __x = __parent(__parent(__x));
        } else {
          if (__x == __right(__parent(__x))) {
            __x = __parent(__x);
            __lrotate(__x);
          }
          __color(__parent(__x)) = Black;
          __color(__parent(__parent(__x))) = Red;
          __rrotate(__parent(__parent(__x)));
        }
      } else {
        __y = __left(__parent(__parent(__x)));
        if (__color(__y) == Red) {
          __color(__parent(__x)) = Black;
          __color(__y) = Black;
          __color(__parent(__parent(__x))) = Red;
          __x = __parent(__parent(__x));
        } else {
          if (__x == __left(__parent(__x))) {
            __x = __parent(__x);
            __rrotate(__x);
          }
          __color(__parent(__x)) = Black;
          __color(__parent(__parent(__x))) = Red;
          __lrotate(__parent(__parent(__x)));
        }
      }
    }
    __color(__root()) = Black;
    return iterator(__z);
  }

}; // __tree class

// Non-member functions

template <class _TreeTraits>
inline void swap(__tree<_TreeTraits>& __x,
                 __tree<_TreeTraits>& __y) {
  __x.swap(__y);
}

template <class _TreeTraits>
inline bool operator==(const __tree<_TreeTraits>& __x, const __tree<_TreeTraits>& __y) {
  return __x.size() == __y.size() && ft::equal(__x.begin(), __x.end(), __y.begin());
}

template <class _TreeTraits>
inline bool operator!=(const __tree<_TreeTraits>& __x, const __tree<_TreeTraits>& __y) {
  return !(__x == __y);
}

template <class _TreeTraits>
inline bool operator<(const __tree<_TreeTraits>& __x, const __tree<_TreeTraits>& __y) {
  return ft::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end(), __x.value_comp());
}

template <class _TreeTraits>
inline bool operator>(const __tree<_TreeTraits>& __x, const __tree<_TreeTraits>& __y) {
  return __y < __x;
}

template <class _TreeTraits>
inline bool operator<=(const __tree<_TreeTraits>& __x, const __tree<_TreeTraits>& __y) {
  return !(__y < __x);
}

template <class _TreeTraits>
inline bool operator>=(const __tree<_TreeTraits>& __x, const __tree<_TreeTraits>& __y) {
  return !(__x < __y);
}

}

#endif // __TREE_HPP

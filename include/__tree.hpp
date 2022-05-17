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

  typedef typename _TreeTraits::allocator_type  allocator_type;
  typedef typename allocator_type::value_type   value_type;
  typedef __tree_node*                          node_pointer;

  value_type __value_;
  node_pointer __parent_;
  node_pointer __left_;
  node_pointer __right_;
  char __color_;
  bool __isnil_;

  __tree_node()
    : __value_(), __parent_(NULL), __left_(NULL), __right_(NULL), __color_(), __isnil_(true)
  {}

  __tree_node(const value_type& __value)
    : __value_(__value), __parent_(NULL), __left_(NULL), __right_(NULL), __color_(), __isnil_(false)
  {}

  node_pointer max_node() {
    node_pointer __p = this;
    while (!(__p->__right_->__isnil_)) {
      __p = __p->__right_;
    }
    return __p;
  }

  node_pointer min_node() {
    node_pointer __p = this;
    while (!(__p->__left_->__isnil_)) {
      __p = __p->__left_;
    }
    return __p;
  }

  node_pointer next_node() {
    if (this->__isnil_) {
      return this;
    } else if (!(this->__right_->__isnil_)) {
      return this->__right_->min_node();
    } else {
      node_pointer __tmp = this;
      node_pointer __p = __tmp->__parent_;
      while (!(__p->__isnil_) && __tmp == __p->__right_) {
        __tmp = __p;
        __p = __tmp->__parent_;
      }
      return __p;
    }
  }

  // end() returns an iterator pointing at __head_ which is nil
  // Therefore, if the iterator is nil, prev_node returns __right_ of that node
  // which is __rmost()

  node_pointer prev_node() {
    if (this->__isnil_) {
      return this->__right_;
    } else if (!(this->__left_->__isnil_)) {
      return this->__left_->max_node();
    } else {
      node_pointer __tmp = this;
      node_pointer __p = __tmp->__parent_;
      while (!(__p->__isnil_) && __tmp == __p->__left_) {
        __tmp = __p;
        __p = __tmp->__parent_;
      }
      return __p;
    }
  }

};

template <class _TreeTraits>
class __tree_iterator {
 public:
  typedef typename std::bidirectional_iterator_tag     iterator_category;
  typedef typename _TreeTraits::allocator_type         allocator_type;
  typedef typename allocator_type::value_type          value_type;
  typedef typename allocator_type::difference_type     difference_type;
  typedef typename allocator_type::pointer             pointer;
  typedef typename allocator_type::reference           reference;

  typedef __tree_node<_TreeTraits>                     node;
  typedef node*                                        node_pointer;

 private:
  node_pointer __node_ptr_;

 public:
  __tree_iterator() : __node_ptr_(NULL) {}

  __tree_iterator(node_pointer __p) : __node_ptr_(__p) {}

  __tree_iterator(const __tree_iterator& __x)
    : __node_ptr_(__x.base()) {}

  __tree_iterator& operator=(const __tree_iterator& __x) {
    __node_ptr_ = __x.base();
    return *this;
  }

  ~__tree_iterator() {}

  reference operator*() const { return __node_ptr_->__value_; }

  pointer operator->() const { return &**this; }

  __tree_iterator& operator++() {
    __node_ptr_ = __node_ptr_->next_node();
    return *this;
  }

  __tree_iterator operator++(int) {
    __tree_iterator __tmp = *this;
    ++*this;
    return __tmp;
  }

  __tree_iterator& operator--() {
    __node_ptr_ = __node_ptr_->prev_node();
    return *this;
  }

  __tree_iterator operator--(int) {
    __tree_iterator __tmp = *this;
    --*this;
    return __tmp;
  }

  bool operator==(const __tree_iterator& __x) const {
    return __node_ptr_ == __x.base();
  }

  bool operator!=(const __tree_iterator& __x) const {
    return !(*this == __x);
  }

  node_pointer base() const { return __node_ptr_; }

}; // __tree_iterator class

template <class _TreeTraits>
class __tree_const_iterator {
 public:
  typedef typename std::bidirectional_iterator_tag     iterator_category;
  typedef typename _TreeTraits::allocator_type         allocator_type;
  typedef typename allocator_type::value_type          value_type;
  typedef typename allocator_type::difference_type     difference_type;
  typedef typename allocator_type::const_pointer       pointer;
  typedef typename allocator_type::const_reference     reference;

  typedef __tree_node<_TreeTraits>                     node;
  typedef node*                                        node_pointer;

 private:
  typedef __tree_iterator<_TreeTraits> non_const_iterator;

  node_pointer __node_ptr_;

 public:
  __tree_const_iterator() : __node_ptr_(NULL) {}

  __tree_const_iterator(node_pointer __p) : __node_ptr_(__p) {}

  // To enable a conversion from non-const iterator to const iterator

  __tree_const_iterator(const non_const_iterator& __x)
    : __node_ptr_(__x.base()) {}

  __tree_const_iterator& operator=(const __tree_const_iterator& __x) {
    __node_ptr_ = __x.base();
    return *this;
  }

  ~__tree_const_iterator() {}

  reference operator*() const { return __node_ptr_->__value_; }

  pointer operator->() const { return &**this; }

  __tree_const_iterator& operator++() {
    __node_ptr_ = __node_ptr_->next_node();
    return *this;
  }

  __tree_const_iterator operator++(int) {
    __tree_const_iterator __tmp = *this;
    ++*this;
    return __tmp;
  }

  __tree_const_iterator& operator--() {
    __node_ptr_ = __node_ptr_->prev_node();
    return *this;
  }

  __tree_const_iterator operator--(int) {
    __tree_const_iterator __tmp = *this;
    --*this;
    return __tmp;
  }

  bool operator==(const __tree_const_iterator& __x) const {
    return __node_ptr_ == __x.base();
  }

  bool operator!=(const __tree_const_iterator& __x) const {
    return !(*this == __x);
  }

  node_pointer base() const { return __node_ptr_; }

}; // __tree_const_iterator

template <class _TreeTraits>
class __tree {

 public:
  typedef __tree<_TreeTraits>                         tree;
  typedef typename _TreeTraits::key_type              key_type;
  typedef const key_type&                             key_reference;
  typedef typename _TreeTraits::key_compare           key_compare;
  typedef typename _TreeTraits::value_type            value_type;
  typedef typename _TreeTraits::allocator_type        allocator_type;
  typedef typename _TreeTraits::key_getter            key_getter;
  typedef typename allocator_type::size_type          size_type;
  typedef typename allocator_type::difference_type    difference_type;
  typedef typename allocator_type::pointer            pointer;
  typedef typename allocator_type::const_pointer      const_pointer;
  typedef typename allocator_type::reference          reference;
  typedef typename allocator_type::const_reference    const_reference;
  typedef __tree_iterator<_TreeTraits>                iterator;
  typedef __tree_const_iterator<_TreeTraits>          const_iterator;
  typedef ft::reverse_iterator<iterator>              reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>        const_reverse_iterator;

  typedef __tree_node<_TreeTraits>
    node;
  typedef typename allocator_type::template rebind<node>::other::pointer
    node_pointer;
  typedef typename allocator_type::template rebind<node_pointer>::other::reference
    node_pointer_reference;

  typedef ft::pair<iterator, bool>                    pair_ib;
  typedef ft::pair<iterator, iterator>                pair_ii;
  typedef ft::pair<const_iterator, const_iterator>    pair_cc;

 protected:

  enum RedBlack {
    kRed, // = 0
    kBlack // = 1
  };

  // Member variables

  node_pointer __head_;
  size_type __size_;
  key_compare __comp_;
  allocator_type __alloc_value_;
  typename allocator_type::template rebind<node>::other __alloc_node_;
  typename allocator_type::template rebind<node_pointer>::other __alloc_node_pointer_;

  // key_getter is either __identity (for set) or __select_first (for map) defined in type_traits.hpp

  static key_reference __key(node_pointer __p) {
    return key_getter()(__p->__value_);
  }

 public:

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
    return std::min(
           __alloc_node_.max_size(),
           static_cast<size_type>(std::numeric_limits<difference_type>::max()));
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
    while (!(__x->__isnil_)) {
      __y = __x;
      __add_left = __comp_(key_getter()(__v), __key(__x));
      __x = __add_left ? __x->__left_ : __x->__right_;
    }
    iterator __it = iterator(__y);
    if (!__add_left) {
      ;
    } else if (__it == begin()) {
      return pair_ib(__insert(true, __y, __v), true);
    } else {
      --__it;
    }
    if (__comp_(__key(__it.base()), key_getter()(__v))) {
      return pair_ib(__insert(__add_left, __y, __v), true);
    } else {
      return pair_ib(__it, false);
    }
  }

  /*
  ** First, try to insert __v using __it as a hint
  ** If that does not work, insert using the implementation above
  */

  iterator insert(iterator __it, const value_type& __v) {
    if (size() == 0) {
      return __insert(true, __head_, __v);
    } else if (__it == begin()) {
      if (__comp_(key_getter()(__v), __key(__lmost()))) {
        return __insert(true, __lmost(), __v);
      }
    } else if (__it == end()) {
      if (__comp_(__key(__rmost()), key_getter()(__v))) {
        return __insert(false, __rmost(), __v);
      }
    } else {
      iterator __it_prev = __it;
      --__it_prev;
      if (__comp_(__key(__it_prev.base()), key_getter()(__v))
          && __comp_(key_getter()(__v), __key(__it.base()))) {
        if (__it_prev.base()->__right_->__isnil_) {
          return __insert(false, __it_prev.base(), __v);
        } else {
          return __insert(true, __it.base(), __v);
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
    if (__p.base()->__isnil_) {
      throw std::out_of_range("map/set<T> iterator");
    }
    node_pointer __target = __p.base();
    node_pointer __node_to_erase = __target;
    node_pointer __replace;
    node_pointer __target_parent;
    __determine_target_n_replace(__target, __replace);
    __separate_node_to_erase(__target, __node_to_erase, __replace, __target_parent);
    if (__node_to_erase->__color_ == kBlack) {
      __update_color(__replace, __target_parent);
    }
    __erase_node(__node_to_erase);
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
    return (__p == end() || __comp_(__k, __key(__p.base()))) ? end() : __p;
  }

  /*
  ** If the container is empty, __p is an iterator of __head_ which equals to
  ** end()
  ** lower_bound returns an iterator that satisfies __k <= __key(__p.base())
  ** Therefore, if __comp_(__k, __key(__p.base())) is false, __p's key equals
  ** to __k
  */

  const_iterator find(const key_type& __k) const {
    const_iterator __p = lower_bound(__k);
    return (__p == end() || __comp_(__k, __key(__p.base()))) ? end() : __p;
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
    }
  }

 protected:

  // TODO: Do we need initialization values for construction of left and right?

  node_pointer __buynode(node_pointer __parent_ptr, char __c) {
    node_pointer __s = __alloc_node_.allocate(1);
    __alloc_node_pointer_.construct(&(__s->__left_));
    __alloc_node_pointer_.construct(&(__s->__right_));
    __alloc_node_pointer_.construct(&(__s->__parent_), __parent_ptr);
    __s->__color_ = __c;
    __s->__isnil_ = false;
    return __s;
  }

  void __consval(pointer __p, const value_type& __v) {
    __alloc_value_.construct(__p, __v);
  }

  void __destval(pointer __p) { __alloc_value_.destroy(__p); }

  void __freenode(node_pointer __s) {
    __alloc_node_pointer_.destroy(&(__s->__parent_));
    __alloc_node_pointer_.destroy(&(__s->__right_));
    __alloc_node_pointer_.destroy(&(__s->__left_));
    __alloc_node_.deallocate(__s, 1);
  }

  // Initiate __head_ pointer that should be black and nill
  // At the initialization, root, lmost, rmost should be pointed at __head_

  void __init() {
    __head_ = __buynode(NULL, kBlack);
    __head_->__isnil_ = true;
    __root() = __head_;
    __lmost() = __head_;
    __rmost() = __head_;
    __size_ = 0;
  }

  node_pointer& __lmost() { return __head_->__left_; }

  node_pointer& __lmost() const { return __head_->__left_; }

  node_pointer& __rmost() { return __head_->__right_; }

  node_pointer& __rmost() const { return __head_->__right_; }

  node_pointer& __root() { return __head_->__parent_; }

  node_pointer& __root() const { return __head_->__parent_; }

  /*
  ** Returns a node pointing to the first element in the container whose key
  ** is not considered to go before k (i.e., either it is equivalent or goes after).
  **
  ** __k <= *(__lbound(__k))
  **
  **        * <- __lbound('b')
  ** {'a', 'b', 'd', 'e'}
  **             * <- __lbound('c')
  **
  */

  node_pointer __lbound(const key_type& __k) const {
    node_pointer __x = __root();
    node_pointer __y = __head_;
    while (!(__x->__isnil_)) {
      if (__comp_(__key(__x), __k)) {
        __x = __x->__right_;
      } else {
        __y = __x;
        __x = __x->__left_;
      }
    }
    return __y;
  }

  /*
  ** Returns a node pointing to the first element in the container whose key is
  ** considered to go after k.
  **
  ** __k < *(__ubound(__k))
  **
  **             * <- __ubound('b')
  ** {'a', 'b', 'd', 'e'}
  **             * <- __lbound('c')
  */

  node_pointer __ubound(const key_type& __k) const {
    node_pointer __x = __root();
    node_pointer __y = __head_;
    while (!(__x->__isnil_)) {
      if (__comp_(__k, __key(__x))) {
        __y = __x;
        __x = __x->__left_;
      } else {
        __x = __x->__right_;
      }
    }
    return __y;
  }

  /*
  **            x                               y
  **           / \      __lrotate(x)           / \
  **          b   y          =>               x   g
  **         / \ / \                         / \
  **        d  e f  g                       b   f
  **                                       / \
  **                                      d   e
  */

  void __lrotate(node_pointer __x) {
    node_pointer __y = __x->__right_;
    __x->__right_ = __y->__left_;
    if (!(__y->__left_->__isnil_)) {
      __y->__left_->__parent_ = __x;
    }
    __y->__parent_ = __x->__parent_;
    if (__x == __root()) {
      __root() = __y;
    } else if (__x == __x->__parent_->__left_) {
      __x->__parent_->__left_ = __y;
    } else {
      __x->__parent_->__right_ = __y;
    }
    __y->__left_ = __x;
    __x->__parent_ = __y;
  }

  /*
  **            x                           y
  **           / \       __rrotate(x)      / \
  **          y   c          =>           d   x
  **         / \ / \                         / \
  **        d  e f  g                       e   c
  **                                           / \
  **                                          f   g
  */

  void __rrotate(node_pointer __x) {
    node_pointer __y = __x->__left_;
    __x->__left_ = __y->__right_;
    if (!(__y->__right_->__isnil_)) {
      __y->__right_->__parent_ = __x;
    }
    __y->__parent_ = __x->__parent_;
    if (__x == __root()) {
      __root() = __y;
    } else if (__x == __x->__parent_->__right_) {
      __x->__parent_->__right_ = __y;
    } else {
      __x->__parent_->__left_ = __y;
    }
    __y->__right_ = __x;
    __x->__parent_ = __y;
  }

  // The parent node of the root node should be __head_ by definition
  // The root node and __head_ point at each other as their parent nodes

  void __copy(const tree& __x) {
    __root() = __copy(__x.__root(), __head_);
    __size_ = __x.size();
    __comp_ = __x.__comp_;
    if (!(__root()->__isnil_)) {
      __lmost() = __root()->min_node();
      __rmost() = __root()->max_node();
    } else {
      __lmost() = __head_;
      __rmost() = __head_;
    }
  }

  node_pointer __copy(node_pointer __x, node_pointer __parent_ptr) {
    node_pointer __r = __head_;
    if (!(__x->__isnil_)) {
      node_pointer __y = __buynode(__parent_ptr, __x->__color_);
      try {
        __consval(&(__y->__value_), __x->__value_);
      } catch (...) {
        __freenode(__y);
        __erase(__r);
        throw;
      }
      __y->__left_ = __head_;
      __y->__right_ = __head_;
      if (__r->__isnil_) {
        __r = __y;
      }
      try {
        __y->__left_ = __copy(__x->__left_, __y);
        __y->__right_ = __copy(__x->__right_, __y);
      } catch (...) {
        __erase(__r);
        throw;
      }
    }
    return __r;
  }

  void __erase(node_pointer __x) {
    for (node_pointer __y = __x; !(__y->__isnil_); __x = __y) {
      __erase(__y->__right_);
      __y = __y->__left_;
      __destval(&(__x->__value_));
      __freenode(__x);
    }
  }

  iterator __insert(bool __addleft, node_pointer __parent, const value_type& __v) {
    if (max_size() - 1 <= __size_) {
      throw std::length_error("map/set<T> too long");
    }
    node_pointer __new = __buynode(__parent, kRed);
    __new->__left_ = __head_;
    __new->__right_ = __head_;
    try {
      __consval(&(__new->__value_), __v);
    } catch (...) {
      __freenode(__new);
      throw;
    }
    ++__size_;
    if (__parent == __head_) {
      __root() = __new;
      __lmost() = __new;
      __rmost() = __new;
    } else if (__addleft) {
      __parent->__left_ = __new;
      if (__parent == __lmost()) {
        __lmost() = __new;
      }
    } else {
      __parent->__right_ = __new;
      if (__parent == __rmost()) {
        __rmost() = __new;
      }
    }
    __adjust_color(__new);
    return iterator(__new);
  }

  // Helper functions for __insert

  void __adjust_color(node_pointer __new) {
    for (node_pointer __x = __new; __x->__parent_->__color_ == kRed; ) {
      if (__x->__parent_ == __x->__parent_->__parent_->__left_) {
        __handle_left_parent_case(__x);
      } else {
        __handle_right_parent_case(__x);
      }
    }
    __root()->__color_ = kBlack;
  }

  void __handle_left_parent_case(node_pointer __x) {
    node_pointer_reference __uncle = __x->__parent_->__parent_->__right_;
    if (__uncle->__color_ == kRed) {
      __x->__parent_->__color_ = kBlack;
      __uncle->__color_ = kBlack;
      __x->__parent_->__parent_->__color_ = kRed;
      __x = __x->__parent_->__parent_;
    } else {
      if (__x == __x->__parent_->__right_) {
        __x = __x->__parent_;
        __lrotate(__x);
      }
      __x->__parent_->__color_ = kBlack;
      __x->__parent_->__parent_->__color_ = kRed;
      __rrotate(__x->__parent_->__parent_);
    }
  }

  void __handle_right_parent_case(node_pointer __x) {
    node_pointer_reference __uncle = __x->__parent_->__parent_->__left_;
    if (__uncle->__color_ == kRed) {
      __x->__parent_->__color_ = kBlack;
      __uncle->__color_ = kBlack;
      __x->__parent_->__parent_->__color_ = kRed;
      __x = __x->__parent_->__parent_;
    } else {
      if (__x == __x->__parent_->__left_) {
        __x = __x->__parent_;
        __rrotate(__x);
      }
      __x->__parent_->__color_ = kBlack;
      __x->__parent_->__parent_->__color_ = kRed;
      __lrotate(__x->__parent_->__parent_);
    }
  }

  // Helper functions for erase function

  void __determine_target_n_replace(node_pointer_reference __target,
                                    node_pointer_reference __replace) {
    if (__target->__left_->__isnil_) {
      __replace = __target->__right_;
    } else if (__target->__right_->__isnil_) {
      __replace = __target->__left_;
    } else {
      __target = __target->__right_->min_node();
      __replace = __target->__right_;
    }
  }

  bool __has_two_ophans(const node_pointer_reference __target,
                        const node_pointer_reference __node_to_erase) {
    return __target != __node_to_erase;
  }

  void __connect_replace_n_parent(const node_pointer_reference __replace,
                                  const node_pointer_reference __node_to_erase,
                                  const node_pointer_reference __target_parent) {
    if (!(__replace->__isnil_)) {
      __replace->__parent_ = __target_parent;
    }
    if (__root() == __node_to_erase) {
      __root() = __replace;
    } else if (__target_parent->__left_ == __node_to_erase) {
      __target_parent->__left_ = __replace;
    } else {
      __target_parent->__right_ = __replace;
    }
  }

  void __update_lmost_n_rmost(const node_pointer_reference __replace,
                              const node_pointer_reference __node_to_erase,
                              const node_pointer_reference __target_parent) {
    if (__lmost() != __node_to_erase) {
      ;
    } else if (__replace->__isnil_) {
      __lmost() = __target_parent;
    } else {
      __lmost() = __replace->min_node();
    }
    if (__rmost() != __node_to_erase) {
      ;
    } else if (__replace->__isnil_) {
      __rmost() = __target_parent;
    } else {
      __rmost() = __replace->max_node();
    }
  }

  void __set_target_left(const node_pointer_reference __target,
                         const node_pointer_reference __node_to_erase) {
    __node_to_erase->__left_->__parent_ = __target;
    __target->__left_ = __node_to_erase->__left_;
  }

  void __connect_replace_w_parent(const node_pointer_reference __replace,
                                  const node_pointer_reference __target_parent) {
    if (!(__replace->__isnil_)) {
      __replace->__parent_ = __target_parent;
    }
    __target_parent->__left_ = __replace;
  }

  void __set_target_right(const node_pointer_reference __target,
                          const node_pointer_reference __node_to_erase) {
    __target->__right_ = __node_to_erase->__right_;
    __node_to_erase->__right_->__parent_ = __target;
  }

  void __set_target_parent(const node_pointer_reference __target,
                           const node_pointer_reference __node_to_erase) {
    if (__root() == __node_to_erase) {
      __root() = __target;
    } else if (__node_to_erase->__parent_->__left_ == __node_to_erase) {
      __node_to_erase->__parent_->__left_ = __target;
    } else {
      __node_to_erase->__parent_->__right_ = __target;
    }
    __target->__parent_ = __node_to_erase->__parent_;
  }

  /*
  ** It is worth noting that, when __node_to_erase has 2 child nodes, the color
  ** of __node_to_erase should be swapped with the color of __target before
  ** updating colors
  */

  void __separate_node_to_erase(node_pointer_reference __target,
                                node_pointer_reference __node_to_erase,
                                node_pointer_reference __replace,
                                node_pointer_reference __target_parent) {
    if (__has_two_ophans(__target, __node_to_erase)) {
      __set_target_left(__target, __node_to_erase);
      if (__target == __node_to_erase->__right_) {
        __target_parent = __target;
      } else {
        __target_parent = __target->__parent_;
        __connect_replace_w_parent(__replace, __target_parent);
        __set_target_right(__target, __node_to_erase);
      }
      __set_target_parent(__target, __node_to_erase);
      std::swap(__target->__color_, __node_to_erase->__color_);
    } else {
      __target_parent = __node_to_erase->__parent_;
      __connect_replace_n_parent(__replace, __node_to_erase, __target_parent);
      __update_lmost_n_rmost(__replace, __node_to_erase, __target_parent);
    }
  }

  bool __handle_left_case(node_pointer_reference __replace,
                          node_pointer_reference __target_parent) {
    node_pointer __replace_sib = __target_parent->__right_;
    if (__replace_sib->__color_ == kRed) {
      __replace_sib->__color_ = kBlack;
      __target_parent->__color_ = kRed;
      __lrotate(__target_parent);
      __replace_sib = __target_parent->__right_;
    }
    if (__replace_sib->__isnil_) {
      __replace = __target_parent;
    } else if (__replace_sib->__left_->__color_ == kBlack && __replace_sib->__right_->__color_ == kBlack) {
      __replace_sib->__color_ = kRed;
      __replace = __target_parent;
    } else {
      if (__replace_sib->__right_->__color_ == kBlack) {
        __replace_sib->__left_->__color_ = kBlack;
        __replace_sib->__color_ = kRed;
        __rrotate(__replace_sib);
        __replace_sib = __target_parent->__right_;
      }
      __replace_sib->__color_ = __target_parent->__color_;
      __target_parent->__color_ = kBlack;
      __replace_sib->__right_->__color_ = kBlack;
      __lrotate(__target_parent);
      return true;
    }
    return false;
  }

  bool __handle_right_case(node_pointer_reference __replace,
                           node_pointer_reference __target_parent) {
    node_pointer __replace_sib = __target_parent->__left_;
    if (__replace_sib->__color_ == kRed) {
      __replace_sib->__color_ = kBlack;
      __target_parent->__color_ = kRed;
      __rrotate(__target_parent);
      __replace_sib = __target_parent->__left_;
    }
    if (__replace_sib->__isnil_) {
      __replace = __target_parent; // should not happen
    } else if (__replace_sib->__right_->__color_ == kBlack && __replace_sib->__left_->__color_ == kBlack) {
      __replace_sib->__color_ = kRed;
      __replace = __target_parent;
    } else {
      if (__replace_sib->__left_->__color_ == kBlack) {
        __replace_sib->__right_->__color_ = kBlack;
        __replace_sib->__color_ = kRed;
        __lrotate(__replace_sib);
        __replace_sib = __target_parent->__left_;
      }
      __replace_sib->__color_ = __target_parent->__color_;
      __target_parent->__color_ = kBlack;
      __replace_sib->__left_->__color_ = kBlack;
      __rrotate(__target_parent);
      return true;
    }
    return false;
  }

  void __update_color(node_pointer_reference __replace,
                      node_pointer_reference __target_parent) {
    bool is_loop_end = false;
    for (; !is_loop_end && __replace != __root() && __replace->__color_ == kBlack; __target_parent = __replace->__parent_) {
      if (__replace == __target_parent->__left_) {
        is_loop_end = __handle_left_case(__replace, __target_parent);
      } else {
        is_loop_end = __handle_right_case(__replace, __target_parent);
      }
    }
    __replace->__color_ = kBlack;
  }

  void __erase_node(node_pointer_reference __node_to_erase) {
    __destval(&(__node_to_erase->__value_));
    __freenode(__node_to_erase);
    if (0 < __size_) {
      --__size_;
    }
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
  return ft::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
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

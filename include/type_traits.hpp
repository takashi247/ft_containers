#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

namespace ft {

// enable_if

template <bool, class _Tp = void>
struct enable_if {};

template <class _Tp>
struct enable_if<true, _Tp> { typedef _Tp type; };

// integral_constant

template <class _Tp, _Tp __v>
struct integral_constant {
  typedef _Tp               value_type;
  typedef integral_constant type;

  static const _Tp value = __v;
  operator value_type() const { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

// remove_const

template <class _Tp> struct remove_const            { typedef _Tp type; };
template <class _Tp> struct remove_const<const _Tp> { typedef _Tp type; };

// remove_volatile

template <class _Tp> struct remove_volatile               { typedef _Tp type; };
template <class _Tp> struct remove_volatile<volatile _Tp> { typedef _Tp type; };

// remove_cv

template <class _Tp>
struct remove_cv {
  typedef typename remove_volatile<typename remove_const<_Tp>::type>::type type;
};

// is_integral

template <class _Tp> struct __ft_is_integral : public false_type {};
template <>          struct __ft_is_integral<bool> : public true_type {};
template <>          struct __ft_is_integral<char> : public true_type {};
template <>          struct __ft_is_integral<signed char> : public true_type {};
template <>          struct __ft_is_integral<unsigned char> : public true_type {};
template <>          struct __ft_is_integral<wchar_t> : public true_type {};
template <>          struct __ft_is_integral<short> : public true_type {};
template <>          struct __ft_is_integral<unsigned short> : public true_type {};
template <>          struct __ft_is_integral<int> : public true_type {};
template <>          struct __ft_is_integral<unsigned int> : public true_type {};
template <>          struct __ft_is_integral<long> : public true_type {};
template <>          struct __ft_is_integral<unsigned long> : public true_type {};

template <class _Tp>
struct is_integral : public __ft_is_integral<typename remove_cv<_Tp>::type> {};

// type detectors

template <class _Tp>
struct __identity {
  _Tp& operator()(_Tp& __x) const { return __x; }

  const _Tp& operator()(const _Tp& __x) const { return __x; }
};

template <class _Tp>
struct __identity<const _Tp> : __identity<_Tp> {};

template <class _Pair>
struct __select_first {
  typename _Pair::first_type& operator()(_Pair& __x) const { return __x.first; }

  const typename _Pair::first_type& operator()(const _Pair& __x) const {
    return __x.first;
  }
};

} // namespace ft

#endif // TYPE_TRAITS_HPP

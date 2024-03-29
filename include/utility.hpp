#ifndef UTILITY_HPP
#define UTILITY_HPP

namespace ft {

template <class T1, class T2>
struct pair {

  typedef T1 first_type;
  typedef T2 second_type;

  pair() : first(T1()), second(T2()) {}

  pair(const T1& v1, const T2& v2) : first(v1), second(v2) {}

  template <class U1, class U2>
  pair(const pair<U1, U2>& x) : first(x.first), second(x.second) {}

  T1 first;
  T2 second;
};

template <class T1, class T2>
bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return x.first == y.first && x.second == y.second;
}

template <class T1, class T2>
bool operator!=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return !(x == y);
}

template <class T1, class T2>
bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class T1, class T2>
bool operator>(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return y < x;
}

template <class T1, class T2>
bool operator<=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return !(y < x);
}

template <class T1, class T2>
bool operator>=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return !(x < y);
}

template <class T1, class T2>
pair<T1, T2> make_pair(const T1& v1, const T2& v2) {
  return pair<T1, T2>(v1, v2);
}

}

#endif

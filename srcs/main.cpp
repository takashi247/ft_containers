#include "vector.hpp"
#include "iterator_traits.hpp"
#include "vector_iterator.hpp"
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <algorithm>

// std::vector<int> vec;

void print_ln(int elem) {
  std::cout << elem << std::endl;
}

int main() {
  ft::vector<int> v(10);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  for (ft::vector<int>::iterator iter = v.begin(), last = v.end(); iter != last; ++iter) {
    std::cout << *iter << std::endl;
  }
  ft::vector<int> v2(10, 42);
  for (ft::vector<int>::reverse_iterator riter = v2.rbegin(), last = v2.rend(); riter != last; ++riter) {
    std::cout << *riter << std::endl;
  }
  ft::vector<int> v3(v.begin(), v.end());
  for (ft::vector<int>::reverse_iterator riter = v3.rbegin(), last = v3.rend(); riter != last; ++riter) {
    std::cout << *riter << std::endl;
  }

  std::cout << "\nTest for input iterator\n";

  const std::string s("123 456 789");

  std::istringstream iss(s);
  std::istream_iterator<int> isIt(iss);
  std::istream_iterator<int> isItEnd;

  ft::vector<int> v4(isIt, isItEnd);
  std::for_each(v4.begin(), v4.end(), print_ln);
}

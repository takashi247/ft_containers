#include "vector.hpp"
#include <iostream>
#include <vector>

// std::vector<int> vec;

int main() {
  ft::vector<int> v(10);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  for (ft::vector<int>::iterator iter = v.begin(), last = v.end(); iter != last; ++iter) {
    std::cout << *iter << std::endl;
  }
  for (ft::vector<int>::reverse_iterator riter = v.rbegin(), last = v.rend(); riter != last; ++riter) {
    std::cout << *riter << std::endl;
  }
}

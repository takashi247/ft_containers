#include "vector.hpp"
#include <iostream>

int main() {
  ft::vector<int> v(10);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  for (ft::vector<int>::iterator iter = v.begin(), last = v.end(); iter != last; ++iter) {
    std::cout << *iter << std::endl;
  }
}

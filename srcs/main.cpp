#include "vector.hpp"
#include "iterator_traits.hpp"
#include "vector_iterator.hpp"
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <algorithm>
#include <stack>
#include <map>
#include <set>
#include "map.hpp"

// std::vector<int> vec;
// std::stack<int> stack;
// std::map<char, int> map;
// std::set<int> set;

void print_ln(int elem) {
  std::cout << elem << std::endl;
}

int main() {
  /*
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

  ft::map<char,int> mymap;

  // first insert function version (single parameter):
  mymap.insert ( ft::pair<char,int>('a',100) );
  mymap.insert ( ft::pair<char,int>('z',200) );

  ft::pair<ft::map<char,int>::iterator,bool> ret;
  ret = mymap.insert ( ft::pair<char,int>('z',500) );
  if (ret.second==false) {
    std::cout << "element 'z' already existed";
    std::cout << " with a value of " << ret.first->second << '\n';
  }

  // second insert function version (with hint position):
  ft::map<char,int>::iterator it = mymap.begin();
  mymap.insert (it, ft::pair<char,int>('b',300));  // max efficiency inserting
  mymap.insert (it, ft::pair<char,int>('c',400));  // no max efficiency inserting

  // third insert function version (range insertion):
  ft::map<char,int> anothermap;
  anothermap.insert(mymap.begin(),mymap.find('c'));

  // showing contents:
  std::cout << "mymap contains:\n";
  for (it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "anothermap contains:\n";
  for (it=anothermap.begin(); it!=anothermap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
  */

  ft::vector<std::string> Z1;
  ft::vector<std::string> Z2;

  Z1.insert(Z1.begin(), 10, "test");
  Z2.insert(Z2.begin(), Z1.begin(), Z1.end());

  for (ft::vector<std::string>::iterator it = Z1.begin(), end = Z1.end(); it != end; ++it) {
    std::cout << *it << std::endl;
  }

  for (ft::vector<std::string>::iterator it = Z2.begin(), end = Z2.end(); it != end; ++it) {
    std::cout << *it << std::endl;
  }

  return 0;
}
